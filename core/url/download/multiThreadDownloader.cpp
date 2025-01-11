#include "multiThreadDownloader.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <filesystem>

multiThreadDownloader::multiThreadDownloader(size_t threadCount) : stop_(false), threadCount_(threadCount), randomEngine_(std::random_device{}()), distribution_(1, 5) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    // Simulate browser headers
    headers_ = curl_slist_append(headers_, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3");
}

multiThreadDownloader::~multiThreadDownloader() {
    stopAllThreads();
    curl_global_cleanup();
    curl_slist_free_all(headers_);
}

void multiThreadDownloader::addDownloadTask(const std::string& url, const std::string& savePath) {
    std::lock_guard<std::mutex> lock(queueMutex_);
    taskQueue_.push(std::make_pair(url, savePath));
    condition_.notify_one();
}

void multiThreadDownloader::waitAllTasks() {
    std::unique_lock<std::mutex> lock(queueMutex_);
    condition_.wait(lock, [this] { return taskQueue_.empty() && threads_.size() == 0; });
}

void multiThreadDownloader::stopAllThreads() {
    stop_ = true;
    condition_.notify_all();
    for (std::thread& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads_.clear();
}

void multiThreadDownloader::retryFailedTasks() {
    std::lock_guard<std::mutex> lock(failedMutex_);
    for (auto& task : failedTasks_) {
        addDownloadTask(task.first, task.second);
    }
    failedTasks_.clear();
}

size_t multiThreadDownloader::getFailedTaskCount() const {
    //std::lock_guard<std::mutex> lock(failedMutex_);
    return failedTasks_.size();
}

std::vector<std::string> multiThreadDownloader::getFailedUrls() {
    std::lock_guard<std::mutex> lock(failedMutex_);
    std::vector<std::string> urls;
    for (auto& task : failedTasks_) {
        urls.push_back(task.first);
    }
    return urls;
}

std::vector<std::string> multiThreadDownloader::getFailedSavePaths() {
    std::lock_guard<std::mutex> lock(failedMutex_);
    std::vector<std::string> savePaths;
    for (auto& task : failedTasks_) {
        savePaths.push_back(task.second);
    }
    return savePaths;
}

void multiThreadDownloader::downloadTask(const std::string& url, const std::string& savePath) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::ofstream outFile(savePath, std::ios::binary);
        if (outFile) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multiThreadDownloader::writeDataCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);

            std::string range = std::to_string(0) + "-" + std::to_string(std::numeric_limits<long long>::max());
            curl_easy_setopt(curl, CURLOPT_RANGE, range.c_str());

            int retryCount = 0;
            while (retryCount < 3) {
                CURLcode res = curl_easy_perform(curl);
                if (res != CURLE_OK) {
                    logError("Failed to download: " + url);
                    std::lock_guard<std::mutex> lock(failedMutex_);
                    failedTasks_[savePath] = url;
                    retryCount++;
                } else {
                    break;
                }
            }

            outFile.close();
        } else {
            logError("Failed to open file: " + savePath);
        }
        curl_easy_cleanup(curl);
    }
}

void multiThreadDownloader::workerThread() {
    while (!stop_) {
        std::pair<std::string, std::string> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            condition_.wait(lock, [this] { return !taskQueue_.empty() || stop_; });
            if (!taskQueue_.empty() && !stop_) {
                task = taskQueue_.front();
                taskQueue_.pop();
            }
        }
        if (!task.first.empty()) {
            downloadTask(task.first, task.second);
        }
    }
}

size_t multiThreadDownloader::writeDataCallback(void* buffer, size_t size, size_t nmemb, std::ofstream* stream) {
    //std::lock_guard<std::mutex> lock(queueMutex_);
    stream->write(static_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}

size_t multiThreadDownloader::headerCallback(void* buffer, size_t size, size_t nitems, void* userdata) {
    // Process HTTP headers here if needed
    return size * nitems;
}

void multiThreadDownloader::logError(const std::string& message) {
    // Assuming LogPrint is a function that prints the log with the given arguments and level
    LogPrint(message, "ERROR");
}

void multiThreadDownloader::ensureDirectory(const std::string& path) {
    if (!std::filesystem::exists(std::filesystem::path(path).parent_path())) {
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    }
}

void multiThreadDownloader::startThreadPool(int threadCount) {
    for (int i = 0; i < threadCount; ++i) {
        threads_.emplace_back(&multiThreadDownloader::workerThread, this);
    }
}