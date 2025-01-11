//
// Created by WswDay2022 on 2024/11/16.
//

#ifndef CMCL_ASSETS_H
#define CMCL_ASSETS_H

#include <string>
#include "../../core/json/reader.h"
#include "../../core/json/value.h"
#include "../../core/core.h"
#include "../../core/consolas.h"
#include "../../core/url/download/downloader.h"
#include "../../core/io/fileReader.h"

using namespace Log;

class assets {
public:
    assets(const std::string& minecraftDirectory, const std::string& versionJson);

    void parsers();

    void parserNatives();
    void parserObjects();
    void parserLibraries();
    void checkRuleKey(Json::Value rule);

    Json::Value getLibraries() { return libraries_; };
    Json::Value getObjects() { return objects_; };
    Json::Value getNatives() { return natives_; }

private:
    Json::Value parser(const std::string& content);

    Json::Value libraries_;
    Json::Value natives_;
    Json::Value objects_;

    std::string versionId;
    std::string versionMainClass;

    std::string minecraftDirectory_;
    std::string versionJson_;
};

#endif //CMCL_ASSETS_H
