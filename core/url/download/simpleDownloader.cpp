//
// Created by WswDay2022 on 2024/11/16.
//

#include "downloader.h"

size_t WriteCallback(void* ptr, size_t size, size_t nmemb, std::ofstream* stream) {
    std::streamsize total_size = size * nmemb;
    if (stream->is_open()) {
        stream->write(reinterpret_cast<const char*>(ptr), total_size);
        if (!stream->good()) {
            LogPrint("ERROR","Write error occurred while writing data to file.");
            return 0;
        }
    } else {
        LogPrint("ERROR","Output file stream is not open.");
        return 0;
    }
    return total_size;
}

simpleDownloader::simpleDownloader() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

simpleDownloader::~simpleDownloader() {
    curl_global_cleanup();
}

bool simpleDownloader::download(const std::string& url, const std::string& destPath) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        LogPrint("ERROR","Error: Could not initialize cURL.");
        return false;
    }

    std::ofstream outFile(destPath, std::ios::binary | std::ios::trunc);
    std::filesystem::path file(destPath);
    std::string dir = file.parent_path().string();
    if (consolas::exist(dir) != 0) {
        int flag = consolas::createDirectory(dir);
        if (flag != 0) { LogPrint("IO", "Creat directory failed.Path:" + dir); }
    }

    if (!outFile.is_open()) {
        LogPrint("ERROR","Error: Could not open file for writing: " + destPath);
        curl_easy_cleanup(curl);
        return false;
    }

    std::string header = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, header.c_str());

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LogPrint("ERROR","Error: cURL download failed.");
        outFile.close();
        curl_easy_cleanup(curl);
        return false;
    }

    outFile.close();
    curl_easy_cleanup(curl);
    return true;
}