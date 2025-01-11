//
// Created by WswDay2022 on 2024/11/29.
//

#include "downloads.h"
#include "../../../core/url/download/concurrentDownloader.h"

void downloads::downloadObjects() {
    assets asset(minecraftDirectory_,versionJson_);
    asset.parserObjects();

    Json::Value objects = asset.getObjects();
    concurrentDownloader downloader(objects.size());

    for (auto &object : objects) {
        if (!object.empty()) {
            LogPrint("[DOWNLOADER]:下载文件:" + object["url"].asString() + ",保存至:" + object["path"].asString(),"INFO");
            downloader.addDownloadTask(object["url"].asString(),object["path"].asString());
        }
    }
    downloader.startAllTasks();
    downloader.waitAllTasks();
    // downloader.verifyDownloadTasks();
}

void downloads::downloadLibraries() {
    assets asset(minecraftDirectory_,versionJson_);
    asset.parserLibraries();

    Json::Value libraries = asset.getLibraries();
    concurrentDownloader downloader(libraries.size());

    for (auto &library : libraries) {
        if (!library.empty()) {
            LogPrint("[DOWNLOADER]:下载文件:" + library["url"].asString() + ",保存至:" + library["path"].asString(),"INFO");
            downloader.addDownloadTask(library["url"].asString(), library["path"].asString());
        }
    }
    downloader.startAllTasks();
    downloader.waitAllTasks();
    if (downloader.getFailedCount() > 0) {
        downloader.retryFailedTasks();
    }

    //downloader.verifyDownloadTasks();
}

