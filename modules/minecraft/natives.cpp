//
// Created by WswDay2022 on 2024/11/16.
//

#include "assets.h"

void assets::parserNatives() {
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(versionJson_,value)) return;
    Json::Value content = value;
    if (content.empty()) {
        LogPrint("[IO]:JSON文件是空的","ERROR");
        return;
    }

    const Json::Value& libraries = content["libraries"];

    for (int i = 0;i < libraries.size();++i) {
        const Json::Value& library = libraries[i];

        // 获取一些基本的键值，方便后续做判断
        const Json::Value& download = library["downloads"];
        const Json::Value& rules = library["rules"];
        const Json::Value& artifact = download["artifact"];
        const Json::Value& classifiers = download["classifiers"];
        const Json::Value& natives = library["natives"];

        if (!classifiers.empty()) { // 含有classifiers，一定是natives
            // rules仅适用于artifact，classifiers不受rules限制，所以无需判断
            const std::string nativeName = natives[consolas::getOS()].asString();
            if (!nativeName.empty()) { // 如果没有对应系统的name键，则说明此native不支持此系统
                Json::Value aNative;
                aNative["url"] = classifiers[nativeName]["url"];
                // 只给出相对路径
                aNative["path"] = classifiers[nativeName]["path"];
                natives_.append(aNative);
            }
        }
        if (!artifact.empty()) { // artifact不是空的，有可能是natives
            // artifact不是空的，有可能是libraries，检测其路径中是否有natives
            if (core::isContainsAllCharacters(artifact["path"].asString(),"natives")) { // path路径中含natives字样，一定是natives
                std::string path = core::formatPath(core::joinPath(minecraftDirectory_, "libraries\\" + artifact["path"].asString()));
                Json::Value aNative;
                aNative["url"] = artifact["url"].asString();
                aNative["path"] = path;
                // 如果有rules键值，对其进行解析
                if (rules.empty()) { natives_.append(aNative); }
                else if (!rules.empty()) {
                    std::vector<bool> allow; // 判断组，当所条件均为true，方可加入libraries列表
                    for (int j = 0; j < rules.size(); ++j) {
                        Json::Value rule = rules[j];
                        if (rule["action"] == "allow") {
                            if (!rule["os"].empty()) {
                                if (rule["os"]["name"].asString() == consolas::getOS()) {
                                    if (!rule["os"]["arch"].empty()) {
                                        if (rule["os"]["arch"].asString() == consolas::getArch()) { allow.push_back(true); }
                                        else { allow.push_back(false); }
                                    } else { allow.push_back(true); }
                                } else { allow.push_back(false); }
                            } else { allow.push_back(true); }
                        } // allow
                        if (rule["action"] == "disallow") {
                            if (!rule["os"].empty()) {
                                if (rule["os"]["name"].asString() == consolas::getOS()) {
                                    if (!rule["os"]["arch"].empty()) {
                                        if (rule["os"]["arch"].asString() == consolas::getArch()) { allow.push_back(false); }
                                        else { allow.push_back(true); }
                                    } else { allow.push_back(false); }
                                } else { allow.push_back(true); }
                            } else { allow.push_back(false); }
                        }
                    }
                    if (std::all_of(allow.begin(), allow.end(), [](bool val) { return val; })) { natives_.append(aNative); }
                }
            }
        }
    }
}