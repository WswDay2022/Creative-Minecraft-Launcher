//
// Created by wsz on 2024/12/20.
//

#ifndef CMCL_MULTITHREADDOWNLOADER_H
#define CMCL_MULTITHREADDOWNLOADER_H

#include "../../core.h"

using namespace Log;

#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <map>
#include <curl/curl.h>
#include <filesystem>
#include <random>

class multiThreadDownloader {
public:
    multiThreadDownloader(size_t threadCount);
    ~multiThreadDownloader();

    void addDownloadTask(const std::string& url, const std::string& savePath);
    void waitAllTasks();
    void startThreadPool(int threadCount);
    void stopAllThreads();
    void retryFailedTasks();
    [[nodiscard]] size_t getFailedTaskCount() const;
    std::vector<std::string> getFailedUrls();
    std::vector<std::string> getFailedSavePaths();

private:
    void downloadTask(const std::string& url, const std::string& savePath);
    void workerThread();
    static size_t writeDataCallback(void* buffer, size_t size, size_t nmemb, std::ofstream* stream);
    static size_t headerCallback(void* buffer, size_t size, size_t nitems, void* userdata);
    void logError(const std::string& message);
    void ensureDirectory(const std::string& path);

    std::vector<std::thread> threads_;
    std::queue<std::pair<std::string, std::string>> taskQueue_;
    std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    std::map<std::string, std::string> failedTasks_;
    std::mutex failedMutex_;
    size_t threadCount_;
    curl_slist* headers_ = nullptr;
    std::mt19937 randomEngine_;
    std::uniform_int_distribution<int> distribution_;
};


#endif //CMCL_MULTITHREADDOWNLOADER_H