//
// Created by WswDay2022 on 2025/2/3.
//

#ifndef CMCL_MULTITHREADDOWNLOADER_H
#define CMCL_MULTITHREADDOWNLOADER_H

#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <functional>
#include <filesystem>
#include <fstream>
#include <future>
#include <execution>
#include <iostream>
#include <memory>
#include <algorithm>
#include <curl/curl.h>
#include "../webFetcher.h"
#include "../../core.h"

using namespace Log;

using ProgressCallback = std::function<void(
        const std::string& url,
        int64_t total,
        int64_t downloaded,
        double speed
)>;

struct DownloadTask {
    std::string url;
    std::filesystem::path outputPath;
    int64_t startByte;
    int64_t endByte;
    int64_t downloaded = 0;
    int partNumber;
    int priority; // 优先级（数值越小优先级越高）
    int retryCount = 0;
    bool success = false;
    std::chrono::system_clock::time_point nextRetryTime;

    // 优先队列比较函数
    bool operator<(const DownloadTask& other) const {
        return priority > other.priority; // 小顶堆
    }
};

class multiThreadDownloader {
public:
    explicit multiThreadDownloader(int threadNum = 1);
    ~multiThreadDownloader();

    bool download(const std::string& url,const std::string& outputPath);

    void stop();
    void setMaxSpeed(int64_t speedPerThread);
    void setRetryTimes(int times);
    void setProxy(const std::string& _proxy);
    void setVerifySSL(bool verify);
    void setProgressCallback(ProgressCallback callback);

private:
    int threadNum;
    std::priority_queue<DownloadTask> taskQueue;
    std::vector<std::thread> threads;
    std::mutex queueMutex;
    std::mutex fileMutex;
    std::atomic<bool> abortFlag{false};
    std::atomic<int64_t> totalDownloaded{0};
    int64_t totalSize = 0;
    int64_t maxSpeed = 0;
    int maxRetries = 3;
    std::string proxy;
    bool verifySSL = true;
    ProgressCallback progressCallback;

    struct PartInfo {
        std::filesystem::path path;
        uint64_t offset;
        uint64_t size;
    };

    struct ProgressData {
        int64_t lastBytes = 0;
        double lastSpeed = 0;
        std::chrono::system_clock::time_point lastTime;
        multiThreadDownloader* downloader = nullptr; // 添加下载器指针
    };

    bool getFileInfo(const std::string& url, int64_t& fileSize);
    bool createDownloadTasks(const std::string& url, const std::filesystem::path& outputPath);
    void workerThread();
    bool downloadPart(DownloadTask& task);
    bool resumeDownload(DownloadTask& task);
    bool verifyFile(const std::string& outputPath) const;
    bool mergeFiles(const std::filesystem::path& outputPath);
    bool prepareParts(const std::filesystem::path& outputPath,std::vector<PartInfo>& parts,uint64_t& _totalSize) const;
    static int curlProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,curl_off_t ultotal, curl_off_t ulnow);
    static size_t writeData(void* buffer, size_t size, size_t nmemb, std::ofstream* stream);
};

#endif //CMCL_MULTITHREADDOWNLOADER_H
