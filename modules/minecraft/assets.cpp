//
// Created by wsw on 2024/11/16.
//

#include "assets.h"

assets::assets(const std::string &minecraftDirectory, const std::string &versionJson)
    : minecraftDirectory_(minecraftDirectory), versionJson_(versionJson) {}

Json::Value assets::parser(const std::string &content) {
    Json::Reader reader;
    Json::Value value;
    if (reader.parse(content,value)) { return value; }
    else { LogPrint("[IO]:无法解析JSON文件","ERROR"); }
    return value;
}

void assets::checkRuleKey(Json::Value rule) {
    // 检查rule键值
    std::string name;
    std::string arch;
    std::string version;
    if (core::isContainsAllCharacters(core::getSystemOSName(),"Windows")) {

    } else if (core::isContainsAllCharacters(core::getSystemOSName(),"MacOs"));
}

void assets::parsers() {
    Json::Value content = parser(versionJson_);
    if (content.empty()) {
        LogPrint("[IO]:JSON文件是空的","ERROR");
        return;
    }
    versionId = content["id"].asString();
    versionMainClass = content["mainClass"].asString();

    parserLibraries();
    parserNatives();
    parserObjects();
}