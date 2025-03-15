//
// Created by WswDay2022 on 2024/11/29.
//

#include "downloads.h"
#include "../../../core/url/download/multiThreadDownloader.h"
#include "../../../core/url/download/concurrentDownloader.h"

void downloads::downloadObjects() {
    assets asset(minecraftDirectory_,versionJson_);
    asset.parserObjects();

    Json::Value objects = asset.getObjects();
    concurrentDownloader downloader(objects.size());

    for (auto &object : objects) {
        if (!object.empty()) {
            LogPrint("[DOWNLOADER]:下载文件:" + object["url"].asString() + ",保存至:" + object["path"].asString(),"INFO");
            downloader.addTask(object["url"].asString(),object["path"].asString());
        }
    }
    downloader.start();
    LogPrint("[DOWNLOADER]:一共有 " + std::to_string(downloader.getFailedCount()) + " 个资源文件下载失败");
    LogPrint("[DOWNLOADER]:所有下载任务已完成");
}

void downloads::downloadLibraries() {
    assets asset(minecraftDirectory_,versionJson_);
    asset.parserLibraries();

    Json::Value libraries = asset.getLibraries();
    concurrentDownloader downloader(libraries.size());
    downloader.setRetryTimes(3);
    for (auto &library : libraries) {
        if (!library.empty()) {
            LogPrint("[DOWNLOADER]:下载文件:" + library["url"].asString() + ",保存至:" + library["path"].asString(),"INFO");
            downloader.addTask(library["url"].asString(),library["path"].asString());
        }
    }

    downloader.start();
    LogPrint("[DOWNLOADER]:一共有 " + std::to_string(downloader.getFailedCount()) + " 个库文件下载失败");
    LogPrint("[DOWNLOADER]:所有下载任务已完成");
}

