//
// Created by WswDay2022 on 2024/11/16.
//

#include "simpleDownloader.h"

size_t WriteCallback(void* ptr, size_t size, size_t nmemb, std::ofstream* stream) {
    std::streamsize total_size = size * nmemb;
    if (stream->is_open()) {
        stream->write(reinterpret_cast<const char*>(ptr), total_size);
        if (!stream->good()) return 0;
    } else return 0;
    return total_size;
}

simpleDownloader::simpleDownloader() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

simpleDownloader::~simpleDownloader() {
    curl_global_cleanup();
}

void simpleDownloader::setVerifySSL(bool verify) {
    verifySSL = verify;
}

void simpleDownloader::setMaxSpeed(int64_t speedPerThread) {
    maxSpeed = speedPerThread;
}

void simpleDownloader::setProxy(const std::string &_proxy) {
    proxy = _proxy;
}

bool simpleDownloader::download(const std::string& url, const std::string& destPath) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        LogPrint("[DOWNLOADER]:初始化CURL失败","ERROR");
        return false;
    }

    std::filesystem::path file(destPath);
    std::string dir = file.parent_path().string();
    if (consolas::exist(dir) != 0) {
        int flag = consolas::createDirectory(dir);
        if (flag != 0) {
            LogPrint("[DOWNLOADER]:创建路径失败:" + dir,"IO");
            return false;
        }
    }
    std::ofstream outFile(destPath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        LogPrint("[DOWNLOADER]:无法打开文件进行写入:" + destPath,"ERROR");
        curl_easy_cleanup(curl);
        return false;
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, webFetcher::generateDynamicUserAgent().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verifySSL ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verifySSL ? 2L : 0L);

    if (!proxy.empty()) curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    if (maxSpeed > 0) curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, maxSpeed);

    CURLcode res = curl_easy_perform(curl);
    outFile.close();
    if (res != CURLE_OK) {
        LogPrint("[DOWNLOADER]:文件 " + url + " 下载失败","ERROR");
        outFile.close();
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return true;
}