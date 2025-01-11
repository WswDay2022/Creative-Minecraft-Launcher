//
// Created by wsz on 2024/12/21.
//

#ifndef CMCL_CONCURRENTDOWNLOADER_H
#define CMCL_CONCURRENTDOWNLOADER_H

#include "../../core.h"
#include <curl/curl.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <filesystem>
#include <fstream>

using namespace Log;

class concurrentDownloader {
public:
    concurrentDownloader(size_t numThreads);
    ~concurrentDownloader();
    void addDownloadTask(const std::string& url, const std::string& savePath);
    void setRetryCount(size_t numRetries);
    size_t getFailedCount() const { return failedCount; }
    void retryFailedTasks();
    bool verifyDownloadTasks();

    void startAllTasks();
    void stopAllTasks();
    void waitAllTasks();

private:
    struct aDownloadTask {
        std::string url;
        std::string outputPath;
    };

    void workThread();
    static size_t writeData(void* buffer, size_t size, size_t nmemb, std::ofstream* stream);
    bool downloadTask(const aDownloadTask& task);
    long getExpectedFileSize(const std::string& url);
    bool verifyFile(const std::string& filePath, long expectedSize);

    size_t threadCount; // 线程数量
    size_t failedCount; // 失败任务数量
    size_t retryCount = 3; // 重试数量
    std::mutex queueMutex; // 队列锁
    std::condition_variable condition; // 条件变量
    std::queue<aDownloadTask> taskQueue; // 任务队列
    std::queue<aDownloadTask> failedQueue; // 失败任务队列
    std::vector<std::thread> workThreads; // 工作线程
    std::atomic<bool> running; // 运行标
};

#endif // CMCL_CONCURRENTDOWNLOADER_H