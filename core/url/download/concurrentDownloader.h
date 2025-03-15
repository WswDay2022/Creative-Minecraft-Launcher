//
// Created by WswDay2022 on 2025/2/4.
//

#ifndef CMCL_CONCURRENTDOWNLOADER_H
#define CMCL_CONCURRENTDOWNLOADER_H

#include <atomic>
#include <vector>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <curl/curl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include "../webFetcher.h"
#include "../../core.h"

using namespace Log;

class concurrentDownloader {
public:
    struct DownloadTask {
        std::string url;
        std::filesystem::path save_path;
        size_t expected_size = 0;
    };

    struct FailedTask {
        DownloadTask task;
        std::string error;
    };

    concurrentDownloader(size_t max_threads = 5);
    ~concurrentDownloader();

    void addTask(const std::string& url, const std::filesystem::path& save_path);
    void start();
    void stop();
    void waitUntilFinished();
    void setMaxSpeed(int64_t speedPerThread);
    void setRetryTimes(int times);
    void setProxy(const std::string& _proxy);
    void setVerifySSL(bool verify);

    std::vector<FailedTask> getFailedTasks() const;
    size_t getFailedCount() const;

private:
    void workerThread();
    bool downloadFile(const DownloadTask& task, std::string& error);
    static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

    std::vector<std::thread> workers;
    std::queue<DownloadTask> task_queue;
    mutable std::mutex queue_mutex;
    std::condition_variable queue_cv;

    std::vector<FailedTask> failed_tasks;
    mutable std::mutex failed_mutex;

    int64_t maxSpeed = 0;
    std::string proxy;
    int maxRetries = 3;
    bool verifySSL = true;
    std::atomic<bool> running{false};
    std::atomic<bool> stop_flag{false};
    size_t max_workers;
    std::map<CURL*, std::ofstream*> curl_handles;
};

#endif //CMCL_CONCURRENTDOWNLOADER_H
