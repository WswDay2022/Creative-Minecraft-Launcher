//
// Created by WswDay2022 on 2024/11/29.
//

#ifndef CMCL_DOWNLOADER_H
#define CMCL_DOWNLOADER_H

#include "../../consolas.h"
#include "../../core.h"
#include <filesystem>
#include <curl/curl.h>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <filesystem>
#include <condition_variable>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>

using namespace Log;

class multiDownloader {
public:
    multiDownloader(size_t numThreads);
    ~multiDownloader();

    void addDownload(const std::string &url, const std::string &outputPath);
    size_t getFailedDownloadCount() const; // 返回下载失败文件的个数

private:
    struct DownloadTask {
        std::string url;
        std::string outputPath;
    };

    void worker();
    static size_t WriteCallback(void *ptr, size_t size, size_t nmemb, std::ofstream *stream);
    void performDownload(const DownloadTask &task);

    std::vector<std::thread> threads;      // 工作线程
    std::queue<DownloadTask> taskQueue;    // 任务队列
    std::mutex queueMutex;                  // 互斥锁
    std::condition_variable condition;       // 条件变量
    bool stop;                               // 停止标志
    mutable std::mutex failedMutex;         // 失败计数的互斥锁
    size_t failedDownloads;                  // 下载失败文件的计数
};

class simpleDownloader {
public:
    simpleDownloader();
    ~simpleDownloader();

    bool download(const std::string& url, const std::string& filePath);
};

#endif //CMCL_DOWNLOADER_H
