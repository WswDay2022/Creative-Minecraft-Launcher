//
// Created by WswDay2022 on 2025/2/3.
//
/*
#include "multiThreadDownloader.h"

multiThreadDownloader::multiThreadDownloader(int threadNum) : threadNum(threadNum) {
    curl_global_init(CURL_GLOBAL_ALL);
}

multiThreadDownloader::~multiThreadDownloader() {
    stop();
    curl_global_cleanup();
}

void multiThreadDownloader::setMaxSpeed(int64_t speedPerThread) {
    maxSpeed = speedPerThread;
}

void multiThreadDownloader::setRetryTimes(int times) {
    maxRetries = std::max(1, times);
}

void multiThreadDownloader::setProxy(const std::string& _proxy) {
    this->proxy = _proxy;
}

void multiThreadDownloader::setVerifySSL(bool verify) {
    verifySSL = verify;
}

void multiThreadDownloader::setProgressCallback(ProgressCallback callback) {
    progressCallback = callback;
}

void multiThreadDownloader::stop() {
    abortFlag.store(true);
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads.clear();
}

bool multiThreadDownloader::download(const std::string& url, const std::string& outputPath) {
    auto start = std::chrono::high_resolution_clock::now();
    abortFlag.store(false);
    totalDownloaded.store(0);

    // 获取文件信息
    int64_t fileSize;
    if (!getFileInfo(url, fileSize) || fileSize <= 0) {
        LogPrint("[DOWNLOADER]:文件请求失败","ERROR");
        return false;
    }
    totalSize = fileSize;

    // 创建任务
    std::filesystem::path outPath(outputPath);
    createDownloadTasks(url,outputPath);
    std::filesystem::create_directories(outPath.parent_path());

    // 启动工作线程
    for (int i = 0; i < threadNum; ++i) {
        threads.emplace_back([this]() { workerThread(); });
    }

    // 进度报告线程
    std::thread progressThread([this, url]() {
        ProgressData pd;
        pd.lastTime = std::chrono::system_clock::now();

        while (!abortFlag.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - pd.lastTime).count();
            double speed = (totalDownloaded.load() - pd.lastBytes) * 1000.0 / duration;

            if (progressCallback) {
                progressCallback(url, totalSize, totalDownloaded.load(), speed);
            }

            pd.lastBytes = totalDownloaded.load();
            pd.lastTime = now;
        }
    });

    // 等待下载完成
    LogPrint("[DOWNLOADER]:开始下载文件"+url);
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    LogPrint("[DOWNLOADER]:下载任务已完成");

    abortFlag.store(true);
    if (progressThread.joinable()) {
        progressThread.join();
    }

    // 合并文件
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start;
    LogPrint("[DOWNLOADER]:文件下载完成，耗时 " + std::to_string(duration1.count() * 1000) + " 毫秒 (" + std::to_string(int(duration1.count() / 60)) + "分钟)");
    LogPrint("[DOWNLOADER]:下载速度:" + std::to_string(int(totalSize / (1024 * 1024 * duration1.count()))) + "MB/s");
    LogPrint("[DOWNLOADER]:准备合并文件");
    LogPrint("[DOWNLOADER]:一共有 " + std::to_string(threadNum) + " 个部分需要合并");

    if (mergeFiles(outPath)) {
        LogPrint("[DOWNLOADER]:合并文件完毕，正在验证文件大小");
        bool check = verifyFile(outputPath);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        if (check) {
            LogPrint("[DOWNLOADER]:验证成功，下载完成！");
            LogPrint("[DOWNLOADER]:完成本次任务共耗时 " + std::to_string(duration.count() * 1000) + " 毫秒 (" + std::to_string(int(duration1.count() / 60)) + "分钟)");
            return true;
        } else {
            LogPrint("[DOWNLOADER]:验证不通过！","ERROR");
            LogPrint("[DOWNLOADER]:完成本次任务共耗时 " + std::to_string(duration.count() * 1000) + " 毫秒 (" + std::to_string(int(duration1.count() / 60)) + "分钟)");
            return false;
        }
    } else {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        LogPrint("[DOWNLOADER]:完成本次任务共耗时 " + std::to_string(duration.count() * 1000) + " 毫秒 (" + std::to_string(int(duration1.count() / 60)) + "分钟)");
        return false;
    }
}

bool multiThreadDownloader::getFileInfo(const std::string& url, int64_t& fileSize) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, webFetcher::generateDynamicUserAgent().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verifySSL ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verifySSL ? 2L : 0L);

    if (!proxy.empty()) {
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    }

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &fileSize);
    }

    curl_easy_cleanup(curl);
    return res == CURLE_OK;
}

bool multiThreadDownloader::createDownloadTasks(const std::string& url, const std::filesystem::path& outputPath) {
    int64_t partSize = totalSize / threadNum;
    for (int i = 0; i < threadNum; ++i) {
        LogPrint("[DOWNLOADER]:正在创建文件第 " + std::to_string(i+1) + " 部分的下载任务");
        DownloadTask task;
        task.url = url;
        task.startByte = i * partSize;
        task.endByte = (i == threadNum - 1) ? totalSize - 1 : (i + 1) * partSize - 1;
        task.partNumber = i;
        task.priority = i; // 前面的分块优先级更高
        task.outputPath = outputPath.string() + ".part" + std::to_string(i+1);

        // 检查已有文件
        if (std::filesystem::exists(task.outputPath)) {
            task.downloaded = std::filesystem::file_size(task.outputPath);
            task.startByte += task.downloaded;
        }
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(task);
        }
    }
    return true;
}

void multiThreadDownloader::workerThread() {
    while (!abortFlag.load()) {
        DownloadTask task;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (taskQueue.empty()) return;
            task = taskQueue.top();
            taskQueue.pop();
        }
        bool success = downloadPart(task);
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (success) {
                totalDownloaded.fetch_add(task.endByte - task.startByte + 1, std::memory_order_relaxed);
            } else {
                if (task.retryCount < maxRetries && !abortFlag.load()) {
                    task.retryCount++;
                    task.nextRetryTime = std::chrono::system_clock::now() + std::chrono::seconds(task.retryCount * task.retryCount);
                    taskQueue.push(task);
                }
            }
        }
    }
}

bool multiThreadDownloader::downloadPart(DownloadTask& task) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::ofstream outFile(task.outputPath.string().c_str(), std::ios::binary | std::ios::trunc); // 覆盖模式以及二进制写入
    LogPrint("[DOWNLOADER]:正在下载第 " + std::to_string(task.partNumber+1) + " 部分文件");
    if (!outFile) {
        curl_easy_cleanup(curl);
        return false;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, task.url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
    curl_easy_setopt(curl, CURLOPT_RANGE,(std::to_string(task.startByte) + "-" + std::to_string(task.endByte)).c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, webFetcher::generateDynamicUserAgent().c_str());
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verifySSL ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verifySSL ? 2L : 0L);

    if (!proxy.empty()) curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    if (maxSpeed > 0) curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, maxSpeed);

    // 进度控制
    auto progressData = std::make_unique<ProgressData>();
    progressData->downloader = this;
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, curlProgressCallback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, progressData.get());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) remove(task.outputPath.string().c_str());
    if (res == CURLE_OK) LogPrint("[DOWNLOADER]:下载第 " + std::to_string(task.partNumber+1) + " 部分完成");

    outFile.close();
    curl_easy_cleanup(curl);
    if (headers) curl_slist_free_all(headers);
    return res == CURLE_OK;
}

constexpr size_t BUFFER_ALIGNMENT = 1024 * 4;

bool multiThreadDownloader::prepareParts(const std::filesystem::path& outputPath, std::vector<PartInfo>& parts, uint64_t& _totalSize) const {
    _totalSize = 0;
    parts.clear();
    for (int i = 0; i < threadNum; ++i) {
        PartInfo info;
        info.path = outputPath.string() + ".part" + std::to_string(i+1);
        if (!std::filesystem::exists(info.path)) return false;
        info.size = std::filesystem::file_size(info.path);
        info.offset = _totalSize;
        _totalSize += info.size;
        parts.push_back(info);
    }

    std::ofstream out(outputPath, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    out.close();
    std::filesystem::resize_file(outputPath, _totalSize);
    return true;
}

bool multiThreadDownloader::mergeFiles(const std::filesystem::path& outputPath) {
    std::vector<PartInfo> parts;
    uint64_t _totalSize = 0;
    if (!prepareParts(outputPath, parts, _totalSize)) return false;
    std::ofstream outFile(outputPath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    if (!outFile) return false;

    std::atomic<uint32_t> nextPart(0);
    std::atomic<uint64_t> bytesCopied(0);
    std::vector<std::thread> workers;
    //std::mutex fileMutex;

    // 智能缓冲区分配（4KB对齐）
    auto allocBuffer = [](uint64_t size) {
        const size_t alignedSize = (size + BUFFER_ALIGNMENT - 1) & ~(BUFFER_ALIGNMENT - 1);
        return std::make_unique<char[]>(alignedSize);
    };

    auto mergeTask = [&]() {
        while (true) {
            const uint32_t idx = nextPart.fetch_add(1);
            if (idx >= parts.size()) break;
            const auto& part = parts[idx];
            std::ifstream inFile(part.path, std::ios::binary);
            if (!inFile) continue;

            // 使用智能指针管理缓冲区
            auto buffer = allocBuffer(part.size);
            if (!inFile.read(buffer.get(), part.size)) continue;
            {
                std::lock_guard<std::mutex> lock(fileMutex);
                outFile.seekp(part.offset);
                outFile.write(buffer.get(), part.size);
            }
            bytesCopied += part.size;
        }
    };

    // 根据CPU核心数动态调整线程数
    unsigned numWorkers = std::max(1u, std::thread::hardware_concurrency());
    for (unsigned i = 0; i < numWorkers; ++i) workers.emplace_back(mergeTask);
    for (auto& t : workers) if (t.joinable()) t.join();
    outFile.close();
    LogPrint("[DOWNLOADER]:合并完成，总大小 " + std::to_string(_totalSize) + " 字节");
    LogPrint("[DOWNLOADER]:正在删除.part文件");
    for (const auto& part : parts) {
        // 删除分块文件
        std::error_code ec;
        std::filesystem::remove(part.path, ec);
    }
    LogPrint("[DOWNLOADER]:.part文件删除完毕");
    return true;
}

size_t multiThreadDownloader::writeData(void* buffer, size_t size, size_t nmemb, std::ofstream* stream) {
    size_t totalBytes = size * nmemb;
    if (stream && stream->is_open()) {
        stream->write(static_cast<char*>(buffer), totalBytes);
        if (!stream->good()) return 0;
    } else return 0;
    return totalBytes;
}

int multiThreadDownloader::curlProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    auto* progressData = static_cast<multiThreadDownloader::ProgressData*>(clientp);
    auto now = std::chrono::system_clock::now();

    // 计算瞬时速度
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - progressData->lastTime).count();
    if (elapsed > 0) {
        double speed = (dlnow - progressData->lastBytes) * 1000.0 / elapsed;
        progressData->lastSpeed = speed;
    }

    progressData->lastBytes = dlnow;
    progressData->lastTime = now;
    multiThreadDownloader* downloader = progressData->downloader;
    return downloader->abortFlag.load() ? 1 : 0;
}

bool multiThreadDownloader::verifyFile(const std::string& outputPath) const {
    std::error_code ec;
    auto fileSize = std::filesystem::file_size(outputPath, ec);
    if (ec) {
        LogPrint("[VERIFY] 获取文件大小失败: " + ec.message(), "ERROR");
        return false;
    }
    return totalSize == fileSize;
}
*/

//
// Created by WswDay2022 on 2025/2/3.
//

#include "multiThreadDownloader.h"

multiThreadDownloader::multiThreadDownloader(int threadNum) : threadNum(threadNum) {
    curl_global_init(CURL_GLOBAL_ALL);
}

multiThreadDownloader::~multiThreadDownloader() {
    stop();
    curl_global_cleanup();
}

void multiThreadDownloader::setMaxSpeed(int64_t speedPerThread) {
    maxSpeed = speedPerThread;
}

void multiThreadDownloader::setRetryTimes(int times) {
    maxRetries = std::max(1, times);
}

void multiThreadDownloader::setProxy(const std::string& _proxy) {
    this->proxy = _proxy;
}

void multiThreadDownloader::setVerifySSL(bool verify) {
    verifySSL = verify;
}

void multiThreadDownloader::setProgressCallback(ProgressCallback callback) {
    progressCallback = callback;
}

void multiThreadDownloader::stop() {
    abortFlag.store(true);
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads.clear();
}

bool multiThreadDownloader::download(const std::string& url, const std::string& outputPath) {
    auto start = std::chrono::high_resolution_clock::now();
    abortFlag.store(false);
    totalDownloaded.store(0);

    // 获取文件信息
    int64_t fileSize;
    if (!getFileInfo(url, fileSize) || fileSize <= 0) {
        LogPrint("[DOWNLOADER]:文件请求失败","ERROR");
        return false;
    }
    totalSize = fileSize;

    // 创建任务
    std::filesystem::path outPath(outputPath);
    createDownloadTasks(url,outputPath);
    std::filesystem::create_directories(outPath.parent_path());

    // 启动工作线程
    for (int i = 0; i < threadNum; ++i) {
        threads.emplace_back([this]() { workerThread(); });
    }

    // 进度报告线程
    std::thread progressThread([this, url]() {
        ProgressData pd;
        pd.lastTime = std::chrono::system_clock::now();

        while (!abortFlag.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - pd.lastTime).count();
            double speed = (totalDownloaded.load() - pd.lastBytes) * 1000.0 / duration;

            if (progressCallback) {
                progressCallback(url, totalSize, totalDownloaded.load(), speed);
            }

            pd.lastBytes = totalDownloaded.load();
            pd.lastTime = now;
        }
    });

    // 等待下载完成
    LogPrint("[DOWNLOADER]:开始下载文件"+url);
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    LogPrint("[DOWNLOADER]:下载任务已完成");

    abortFlag.store(true);
    if (progressThread.joinable()) {
        progressThread.join();
    }

    // 合并文件
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start;
    LogPrint("[DOWNLOADER]:文件下载完成，耗时 " + std::to_string(duration1.count() * 1000) + " 毫秒 (" + std::to_string(int(duration1.count() / 60)) + "分钟)");
    LogPrint("[DOWNLOADER]:下载速度:" + std::to_string(int(totalSize / (1024 * 1024 * duration1.count()))) + "MB/s");
    LogPrint("[DOWNLOADER]:准备合并文件");
    LogPrint("[DOWNLOADER]:一共有 " + std::to_string(threadNum) + " 个部分需要合并");

    if (mergeFiles(outPath)) {
        LogPrint("[DOWNLOADER]:合并文件完毕，正在验证文件大小");
        bool check = verifyFile(outputPath);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        if (check) {
            LogPrint("[DOWNLOADER]:验证成功，下载完成！");
            LogPrint("[DOWNLOADER]:完成本次任务共耗时 " + std::to_string(duration.count() * 1000) + " 毫秒 (" + std::to_string(int(duration1.count() / 60)) + "分钟)");
            return true;
        } else {
            LogPrint("[DOWNLOADER]:验证不通过！","ERROR");
            LogPrint("[DOWNLOADER]:完成本次任务共耗时 " + std::to_string(duration.count() * 1000) + " 毫秒 (" + std::to_string(int(duration1.count() / 60)) + "分钟)");
            return false;
        }
    } else {
        LogPrint("[DOWNLOADER]:合并文件失败！","ERROR");
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        LogPrint("[DOWNLOADER]:完成本次任务共耗时 " + std::to_string(duration.count() * 1000) + " 毫秒 (" + std::to_string(int(duration1.count() / 60)) + "分钟)");
        return false;
    }
}

bool multiThreadDownloader::getFileInfo(const std::string& url, int64_t& fileSize) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, webFetcher::generateDynamicUserAgent().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verifySSL ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verifySSL ? 2L : 0L);

    if (!proxy.empty()) {
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    }

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &fileSize);
    }

    curl_easy_cleanup(curl);
    return res == CURLE_OK;
}

bool multiThreadDownloader::createDownloadTasks(const std::string& url, const std::filesystem::path& outputPath) {
    int64_t partSize = totalSize / threadNum;
    for (int i = 0; i < threadNum; ++i) {
        LogPrint("[DOWNLOADER]:正在创建文件第 " + std::to_string(i+1) + " 部分的下载任务");
        DownloadTask task;
        task.url = url;
        task.outputPath = outputPath.string() + ".part" + std::to_string(i+1);
        task.startByte = i * partSize;
        task.endByte = (i == threadNum - 1) ? totalSize - 1 : (i + 1) * partSize - 1;
        task.partNumber = i;
        task.priority = i; // 前面的分块优先级更高

        // 检查已有文件
        if (std::filesystem::exists(task.outputPath)) {
            task.downloaded = std::filesystem::file_size(task.outputPath);
            task.startByte += task.downloaded;
        }
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(task);
        }
    }
    return true;
}

void multiThreadDownloader::workerThread() {
    while (!abortFlag.load()) {
        DownloadTask task;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (taskQueue.empty()) return;
            task = taskQueue.top();
            taskQueue.pop();
        }
        bool success = downloadPart(task);
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (success) {
                totalDownloaded.fetch_add(task.endByte - task.startByte + 1);
            } else {
                if (task.retryCount < maxRetries) {
                    task.retryCount++;
                    task.nextRetryTime = std::chrono::system_clock::now() + std::chrono::seconds(task.retryCount * task.retryCount);
                    taskQueue.push(task);
                }
            }
        }
    }
}

bool multiThreadDownloader::downloadPart(DownloadTask& task) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::ofstream outFile(task.outputPath.string().c_str(), std::ios::binary | std::ios::trunc); // 覆盖模式以及二进制写入
    LogPrint("[DOWNLOADER]:正在下载第 " + std::to_string(task.partNumber+1) + " 部分文件");
    if (!outFile) {
        curl_easy_cleanup(curl);
        return false;
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, task.url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
    curl_easy_setopt(curl, CURLOPT_RANGE,(std::to_string(task.startByte) + "-" + std::to_string(task.endByte)).c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, webFetcher::generateDynamicUserAgent().c_str());
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verifySSL ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verifySSL ? 2L : 0L);

    if (!proxy.empty()) curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    if (maxSpeed > 0) curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, maxSpeed);

    // 进度控制
    auto progressData = std::make_unique<ProgressData>();
    progressData->downloader = this;
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, curlProgressCallback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, progressData.get());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::string ec = curl_easy_strerror(res);
        LogPrint("[DOWNLOADER]:下载第 " + std::to_string(task.partNumber+1) + " 部分失败","ERROR");
        LogPrint("[DOWNLOADER]:失败信息: " + ec,"ERROR");
        remove(task.outputPath.string().c_str());
    }
    if (res == CURLE_OK) LogPrint("[DOWNLOADER]:下载第 " + std::to_string(task.partNumber+1) + " 部分完成");

    outFile.close();
    curl_easy_cleanup(curl);
    return res == CURLE_OK;
}

// 4KB对齐的缓冲区大小（优化磁盘IO）
constexpr size_t BUFFER_ALIGNMENT = 4096;

bool multiThreadDownloader::prepareParts(const std::filesystem::path& outputPath, std::vector<PartInfo>& parts, uint64_t& _totalSize) {
    _totalSize = 0;
    parts.clear();
    for (int i = 0; i < threadNum; ++i) {
        PartInfo info;
        info.path = outputPath.string() + ".part" + std::to_string(i+1);
        if (!std::filesystem::exists(info.path)) return false;
        info.size = std::filesystem::file_size(info.path);
        info.offset = _totalSize;
        _totalSize += info.size;
        parts.push_back(info);
    }

    std::ofstream out(outputPath, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    out.close();
    std::filesystem::resize_file(outputPath, _totalSize);
    return true;
}

bool multiThreadDownloader::mergeFiles(const std::filesystem::path& outputPath) {
    std::vector<PartInfo> parts;
    uint64_t _totalSize = 0;
    if (!prepareParts(outputPath, parts, _totalSize)) return false;
    std::ofstream outFile(outputPath, std::ios::binary | std::ios::in | std::ios::out);
    if (!outFile) return false;

    std::atomic<uint32_t> nextPart(0);
    std::atomic<uint64_t> bytesCopied(0);
    std::vector<std::thread> workers;
    std::mutex fileMutex;

    // 智能缓冲区分配（4KB对齐）
    auto allocBuffer = [](uint64_t size) {
        const size_t alignedSize = (size + BUFFER_ALIGNMENT - 1) & ~(BUFFER_ALIGNMENT - 1);
        return std::make_unique<char[]>(alignedSize);
    };

    auto mergeTask = [&]() {
        while (true) {
            const uint32_t idx = nextPart.fetch_add(1);
            if (idx >= parts.size()) break;

            const auto& part = parts[idx];
            std::ifstream inFile(part.path, std::ios::binary);
            if (!inFile) continue;

            // 使用智能指针管理缓冲区
            auto buffer = allocBuffer(part.size);
            if (!inFile.read(buffer.get(), part.size)) continue;
            {
                std::lock_guard<std::mutex> lock(fileMutex);
                outFile.seekp(part.offset);
                outFile.write(buffer.get(), part.size);
            }
            bytesCopied += part.size;
        }
    };

    // 根据CPU核心数动态调整线程数
    unsigned numWorkers = std::max(1u, std::thread::hardware_concurrency());
    for (unsigned i = 0; i < numWorkers; ++i) workers.emplace_back(mergeTask);
    for (auto& t : workers) if (t.joinable()) t.join();
    outFile.close();
    LogPrint("[DOWNLOADER]:合并完成，总大小 " + std::to_string(_totalSize) + " 字节");
    LogPrint("[DOWNLOADER]:正在删除.part文件");
    for (const auto& part : parts) {
        // 删除分块文件
        std::error_code ec;
        std::filesystem::remove(part.path, ec);
    }
    LogPrint("[DOWNLOADER]:.part文件删除完毕");
    return true;
}

size_t multiThreadDownloader::writeData(void* buffer, size_t size, size_t nmemb, std::ofstream* stream) {
    size_t totalBytes = size * nmemb;
    if (stream && stream->is_open()) {
        stream->write(static_cast<char*>(buffer), totalBytes);
        if (!stream->good()) return 0;
    } else return 0;
    return totalBytes;
}

int multiThreadDownloader::curlProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    auto* progressData = static_cast<multiThreadDownloader::ProgressData*>(clientp);
    auto now = std::chrono::system_clock::now();

    // 计算瞬时速度
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - progressData->lastTime).count();
    if (elapsed > 0) {
        double speed = (dlnow - progressData->lastBytes) * 1000.0 / elapsed;
        progressData->lastSpeed = speed;
    }

    progressData->lastBytes = dlnow;
    progressData->lastTime = now;
    multiThreadDownloader* downloader = progressData->downloader;
    return downloader->abortFlag.load() ? 1 : 0;
}

bool multiThreadDownloader::verifyFile(std::string outputPath) {
    std::error_code ec;
    auto fileSize = std::filesystem::file_size(outputPath, ec);
    if (ec) {
        LogPrint("[VERIFY]:获取文件大小失败: " + ec.message(), "ERROR");
        return false;
    }
    LogPrint("[VERIFY]:网络请求文件大小(Byte):" + std::to_string(totalSize));
    LogPrint("[VERIFY]:实际文件大小(Byte):" + std::to_string(fileSize));
    return fileSize == totalSize;
}