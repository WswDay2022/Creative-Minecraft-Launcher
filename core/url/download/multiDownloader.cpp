//
// Created by WswDay2022 on 2024/11/29.
//

#include "downloader.h"

multiDownloader::multiDownloader(size_t numThreads) : stop(false), failedDownloads(0) {
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back(&multiDownloader::worker, this);
    }
}

multiDownloader::~multiDownloader() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &thread : threads) {
        thread.join();
    }
}

void multiDownloader::addDownload(const std::string &url, const std::string &outputPath) {
    std::filesystem::path file(outputPath);
    std::string dir = file.parent_path().string();
    if (consolas::exist(dir) != 0) {
        int flag = consolas::createDirectory(dir);
        if (flag != 0) { LogPrint("IO", "Creat directory failed.Path:" + dir); }
    }
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push({url, outputPath});
    }
    condition.notify_one();
}

size_t multiDownloader::getFailedDownloadCount() const {
    std::lock_guard<std::mutex> lock(failedMutex);
    return failedDownloads;
}

void multiDownloader::worker() {
    while (true) {
        DownloadTask task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !taskQueue.empty() || stop; });
            if (stop && taskQueue.empty()) { return; }
            task = taskQueue.front();
            taskQueue.pop();
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500 + 500)); // 500ms到1000ms的延迟
        performDownload(task);
    }
}

// 进行下载
void multiDownloader::performDownload(const DownloadTask &task) {
    CURL *curl;
    std::ofstream ofs(task.outputPath, std::ios::binary | std::ios::trunc | std::ios::out | std::ios::in );

    if (!ofs.is_open()) {
        std::cerr << "Failed to open file: " << task.outputPath << std::endl;
        {
            std::lock_guard<std::mutex> lock(failedMutex);
            ++failedDownloads;
        }
        return;
    }
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, task.url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36");

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            {
                std::lock_guard<std::mutex> lock(failedMutex);
                ++failedDownloads;
            }
        }
        curl_easy_cleanup(curl);
    }
    ofs.close();
}

size_t multiDownloader::WriteCallback(void *ptr, size_t size, size_t nmemb, std::ofstream *stream) {
    std::streamsize total_size = size * nmemb;
    if (stream->is_open()) {
        stream->write(reinterpret_cast<const char*>(ptr), total_size);
        if (!stream->good()) {
            std::cerr << "ERROR: Write error occurred while writing data to file." << std::endl;
            return 0;
        }
    } else {
        std::cerr << "ERROR: Output file stream is not open." << std::endl;
        return 0;
    }
    return total_size;
}