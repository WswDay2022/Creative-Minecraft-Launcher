//
// Created by wsz on 2024/12/21.
//

#include "concurrentDownloader.h"

concurrentDownloader::concurrentDownloader(size_t numThreads)
        : threadCount(numThreads), failedCount(0), running(false) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

concurrentDownloader::~concurrentDownloader() {
    curl_global_cleanup();
}

void concurrentDownloader::addDownloadTask(const std::string& url, const std::string& savePath) {
    if (url.empty() || savePath.empty()) {
        LogPrint("URL或保存路径为空", "ERROR");
        return;
    }

    std::lock_guard<std::mutex> lock(queueMutex);
    aDownloadTask task{url, savePath};
    taskQueue.emplace(task);
    condition.notify_one(); // 唤醒线程
}

bool concurrentDownloader::downloadTask(const aDownloadTask& task) {
    CURL* curl = curl_easy_init();
    core core_; // 初始化核心
    core_.globalInit();

    const std::string& savePath = task.outputPath;
    const std::string& url = task.url;

    if (!curl) {
        LogPrint("初始化CURL句柄失败", "ERROR");
        return false;
    }

    std::filesystem::path saveTo(savePath);
    if (!core::exist(saveTo.parent_path().string())) {
        LogPrint("未检测到保存路径的父目录，先创建", "INFO");
        core::createDirectory(saveTo.parent_path().string());
    }

    std::ofstream outFile(savePath, std::ios::binary | std::ios::trunc); // 覆盖模式以及二进制写入
    if (!outFile) {
        LogPrint("未能正常打开输出文件: " + savePath, "ERROR");
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // 设置url
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, core_.getSettingJson()["timeout"].asInt());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile); // 写入到
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // SSL 验证
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

    CURLcode res = curl_easy_perform(curl); // 捕捉CURL状态
    if (res == CURLE_OK) {
        if (verifyFile(savePath, getExpectedFileSize(url))) {
            LogPrint("下载" + url + "成功且验证成功!");
            curl_easy_cleanup(curl);
            return true;
        } else {
            LogPrint("下载" + url + "成功，但验证失败","ERROR");
            curl_easy_cleanup(curl);
            return false;
        }
    } else {
        LogPrint("下载" + url + "文件失败!","ERROR");
        curl_easy_cleanup(curl);
        return false;
    }

}

void concurrentDownloader::setRetryCount(size_t numRetries) {
    retryCount = numRetries;
}

bool concurrentDownloader::verifyFile(const std::string& filePath, long expectedSize) {
    if (!std::filesystem::exists(filePath)) {
        LogPrint("文件不存在: " + filePath, "ERROR");
        return false;
    }
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        LogPrint("打开文件失败!", "ERROR");
        return false;
    }
    file.seekg(0, std::ios::end);
    std::streamsize actualSize = file.tellg();
    LogPrint("实际文件大小为: " + std::to_string(actualSize));
    if (actualSize != expectedSize) {
        LogPrint("文件完整性验证失败，期望大小: " + std::to_string(expectedSize) + ", 实际大小: " + std::to_string(actualSize), "WARNING");
        return false;
    }
    return true;
}

long concurrentDownloader::getExpectedFileSize(const std::string& url) {
    CURL* tempCurl = curl_easy_init();
    if (!tempCurl) {
        LogPrint("无法初始化CURL以获取文件大小", "ERROR");
        return -1;
    }

    curl_easy_setopt(tempCurl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(tempCurl, CURLOPT_NOBODY, 1L); // 不下载主体
    curl_easy_setopt(tempCurl, CURLOPT_HEADER, 1L); // 检索头信息

    double contentLength;
    CURLcode res = curl_easy_perform(tempCurl);
    if (res != CURLE_OK || curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &contentLength) != CURLE_OK || contentLength <= 0) {
        LogPrint("无法获取文件大小，URL: " + url, "ERROR");
        curl_easy_cleanup(tempCurl);
        return -1;
    }

    LogPrint("获取文件: " + url + "的大小为:" + std::to_string(contentLength));
    curl_easy_cleanup(tempCurl);
    return static_cast<long>(contentLength);
}

void concurrentDownloader::workThread() {
    while (running) {
        aDownloadTask task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !taskQueue.empty() || !running; });
            if (!running) {
                return; // 如果停止标志为 false，退出线程
            }
            if (taskQueue.empty()) {
                continue; // 如果队列为空，继续等待
            }
            task = taskQueue.front();
            taskQueue.pop();
        }
        if (!downloadTask(task)) {
            failedQueue.push(task);
            ++failedCount;
        }
    }
}

void concurrentDownloader::waitAllTasks() {
    for (auto& thread : workThreads) {
        if (thread.joinable()) {
            thread.join(); // 等待每个线程完成
        }
    }
}

void concurrentDownloader::startAllTasks() {
    running = true;
    for (size_t i = 0; i < threadCount; ++i) {
        workThreads.emplace_back(&concurrentDownloader::workThread, this);
    }
}

size_t concurrentDownloader::writeData(void* buffer, size_t size, size_t nmemb, std::ofstream* stream) {
    size_t totalBytes = size * nmemb;
    if (stream && stream->is_open()) {
        stream->write(static_cast<char*>(buffer), totalBytes);
        if (!stream->good()) {
            LogPrint("写入文件失败!", "ERROR");
            return 0;
        }
    } else {
        LogPrint("输出流未打开!", "ERROR");
        return 0;
    }
    return totalBytes;
}

void concurrentDownloader::stopAllTasks() {
    running = false; // 设置标志位为 false
    condition.notify_all(); // 唤醒所有线程
}

bool concurrentDownloader::verifyDownloadTasks() {
    std::vector<bool> allow;
    int goodTask = 0;
    int failedTask = 0;

    std::lock_guard<std::mutex> lock(queueMutex);
    std::queue<aDownloadTask> tempQueue = taskQueue;
    while (!tempQueue.empty()) {
        const aDownloadTask& task = tempQueue.front();
        if (verifyFile(task.outputPath, getExpectedFileSize(task.url))) {
            allow.push_back(true);
            goodTask++;
        } else {
            failedQueue.push(task);
            allow.push_back(false);
            failedTask++;
        }
        tempQueue.pop();
    }

    LogPrint("验证完毕! 一共有" + std::to_string(goodTask) + "任务通过验证, " + std::to_string(failedTask) + "个任务没有通过验证!");
    return std::all_of(allow.begin(), allow.end(), [](bool val) { return val; });
}

void concurrentDownloader::retryFailedTasks() {
    std::queue<aDownloadTask> alsoFailedQueue;
    while (running) {
        aDownloadTask task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !failedQueue.empty() || !running; });
            if (!running) {
                return;
            }
            if (failedQueue.empty()) {
                continue;
            }
            task = failedQueue.front();
            failedQueue.pop();
        }
        for (size_t i = 0; i < retryCount; ++i) {
            LogPrint("正在重试第" + std::to_string(i + 1) + "次: " + task.url, "INFO");
            if (downloadTask(task)) {
                LogPrint("重试下载: " + task.url + "成功!");
                break;
            } else if (i == retryCount - 1) {
                LogPrint("重试: " + task.url + " 已达到最大重试次数，无需再次尝试。", "ERROR");
                alsoFailedQueue.push(task);
                ++failedCount;
            }
        }
    }
    std::lock_guard<std::mutex> lock(queueMutex);
    while (!alsoFailedQueue.empty()) {
        failedQueue.push(alsoFailedQueue.front());
        alsoFailedQueue.pop();
    }
}