//
// Created by WswDay2022 on 2024/11/16.
//

#include "assets.h"

void assets::parserObjects() {
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(versionJson_,value)) {
        LogPrint("[IO]:版本JSON文件内容是空的","ERROR");
        return;
    }
    Json::Value content = value;
    if (content.empty()) {
        LogPrint("[IO]:JSON文件是空的","ERROR");
        return;
    }

    Json::Value assetIndex = content["assetIndex"];
    const std::string &id = assetIndex["id"].asString();
    const std::string &url = assetIndex["url"].asString();
    const std::string &path = core::formatPath(core::joinPath(minecraftDirectory_,"assets\\indexes\\" + id + ".json"));
    Json::Value index;

    if (!core::exist(path)) {
        simpleDownloader downloader;
        if (!downloader.download(url, path)) {
            LogPrint("[DOWNLOADER]:下载资源索引文件失败!","ERROR");
            return;
        }
    }

    fileReader indexesFile(path);
    indexesFile.readFile();
    const std::string &indexes = indexesFile.getFullContent(indexesFile.getFileContent());
    LogPrint("[IO]:资源文件路径:"+path,"INFO");
    if (!reader.parse(indexes, index)) {
        LogPrint("[IO]:无法解析资源文件","ERROR");
        return;
    }

    Json::Value objects = index["objects"];
    Json::Value anObjectKey;

    for (const auto& object : objects.getMemberNames()) {
        Json::Value anObject = objects[object];
        const std::string hash = anObject["hash"].asString();
        std::string path_ = "assets\\objects\\" + hash.substr(0,2) + "\\" + hash;
        const std::string fullPath = core::formatPath(core::joinPath(minecraftDirectory_,path_));

        anObjectKey["url"] = "https://resources.download.minecraft.net/" + hash.substr(0,2) + "/" + hash;
        anObjectKey["path"] = fullPath;
        objects_.append(anObjectKey);
    }
}