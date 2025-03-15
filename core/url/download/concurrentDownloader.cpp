//
// Created by WswDay2022 on 2025/2/4.
//

#include "concurrentDownloader.h"

concurrentDownloader::concurrentDownloader(size_t max_threads)
    : max_workers(max_threads) {
    curl_global_init(CURL_GLOBAL_ALL);
}

concurrentDownloader::~concurrentDownloader() {
    stop();
    curl_global_cleanup();
}

void concurrentDownloader::setMaxSpeed(int64_t speedPerThread) {
    maxSpeed = speedPerThread;
}

void concurrentDownloader::setRetryTimes(int times) {
    maxRetries = times;
}

void concurrentDownloader::setVerifySSL(bool verify) {
    verifySSL = verify;
}

void concurrentDownloader::setProxy(const std::string &_proxy) {
    proxy = _proxy;
}

void concurrentDownloader::addTask(const std::string& url, const std::filesystem::path& save_path) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    task_queue.push({url, save_path});
    queue_cv.notify_one();
}

void concurrentDownloader::start() {
    if (running) return;
    running = true;
    stop_flag = false;
    for (size_t i = 0; i < max_workers; ++i) {
        workers.emplace_back(&concurrentDownloader::workerThread, this);
    }
}

void concurrentDownloader::stop() {
    if (!running) return;
    stop_flag = true;
    queue_cv.notify_all();
    for (auto& t : workers) {
        if (t.joinable()) t.join();
    }
    workers.clear();
    running = false;
}

void concurrentDownloader::waitUntilFinished() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    queue_cv.wait(lock, [this]{
        return task_queue.empty() && !running;
    });
}

std::vector<concurrentDownloader::FailedTask> concurrentDownloader::getFailedTasks() const {
    std::lock_guard<std::mutex> lock(failed_mutex);
    return failed_tasks;
}

size_t concurrentDownloader::getFailedCount() const {
    std::lock_guard<std::mutex> lock(failed_mutex);
    return failed_tasks.size();
}

void concurrentDownloader::workerThread() {
    CURL* curl = curl_easy_init();
    if (!curl) return;
    while (!stop_flag) {
        DownloadTask task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cv.wait(lock, [this]{
                return !task_queue.empty() || stop_flag;
            });
            if (stop_flag) break;
            if (!task_queue.empty()) {
                task = std::move(task_queue.front());
                task_queue.pop();
            }
        }

        if (task.url.empty()) continue;
        std::string error;
        bool success = false;
        size_t retries = maxRetries;
        while (retries --> 0 && !stop_flag) {
            LogPrint("[DOWNLOADER]:开始下载任务 " + task.url);
            if (downloadFile(task, error)) {
                success = true;
                break;
            }
        }

        if (!success) {
            LogPrint("[DOWNLOADER]:下载任务 " + task.url + " 失败，错误信息:" + error,"ERROR");
            std::lock_guard<std::mutex> lock(failed_mutex);
            failed_tasks.push_back({std::move(task), error});
        } else {
            LogPrint("[DOWNLOADER]:下载任务 " + task.url + " 成功且验证通过");
        }
    }

    curl_easy_cleanup(curl);
}

bool concurrentDownloader::downloadFile(const DownloadTask& task, std::string& error) {
    std::filesystem::create_directories(task.save_path.parent_path());
    std::ofstream file(task.save_path, std::ios::binary | std::ios::trunc);
    if (!file) {
        error = "打开输出文件失败";
        return false;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        error = "CURL初始化失败";
        return false;
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, task.url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &concurrentDownloader::writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, webFetcher::generateDynamicUserAgent().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verifySSL ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verifySSL ? 2L : 0L);

    if (!proxy.empty()) curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    if (maxSpeed > 0) curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, maxSpeed);

    CURLcode res = curl_easy_perform(curl);
    file.close();

    if (res != CURLE_OK) {
        error = curl_easy_strerror(res);
        curl_easy_cleanup(curl);
        std::filesystem::remove(task.save_path);
        return false;
    }

    // 验证文件大小
    double dl_size;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &dl_size);
    size_t actual_size = std::filesystem::file_size(task.save_path);

    if (actual_size != static_cast<size_t>(dl_size)) {
        error = "文件大小不匹配";
        std::filesystem::remove(task.save_path);
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return true;
}

size_t concurrentDownloader::writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::ofstream* file = static_cast<std::ofstream*>(userdata);
    if (!file || !file->is_open()) return 0;
    size_t realSize = size * nmemb;
    file->write(ptr, realSize);
    return realSize;
}