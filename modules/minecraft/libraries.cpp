//
// Created by wsw on 2024/11/16.
//

#include "assets.h"

void assets::parserLibraries() {
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(versionJson_,value)) return;
    Json::Value content = value;
    if (content.empty()) {
        LogPrint("JSON文件是空的","ERROR");
        return;
    }

    const Json::Value& libraries = content["libraries"];
    for (int i = 0;i < libraries.size();++i) {
        const Json::Value& library = libraries[i];

        // 获取一些基本的键值，方便后续做判断
        const Json::Value& download = library["downloads"];
        const Json::Value& rules = library["rules"];
        const Json::Value& artifact = download["artifact"];

        if (!artifact.empty()) {
            if (!core::isContainsAllCharacters(library["name"].asString(),"natives")) {
                std::string path = core::formatPath(
                        core::joinPath(minecraftDirectory_, "libraries\\" + artifact["path"].asString()));
                Json::Value aLibrary;
                aLibrary["url"] = artifact["url"].asString();
                aLibrary["path"] = path;
                // 如果有rules键值，对其进行解析
                if (rules.empty()) { libraries_.append(aLibrary); }
                else if (!rules.empty()) {
                    std::vector<bool> allow; // 判断组，当所条件均为true，方可加入libraries列表
                    for (int j = 0; j < rules.size(); ++j) {
                        Json::Value rule = rules[j];
                        if (rule["action"] == "allow") {
                            if (!rule["os"].empty()) {
                                if (rule["os"]["name"].asString() == consolas::getOS()) {
                                    if (!rule["os"]["arch"].empty()) {
                                        if (rule["os"]["arch"].asString() == consolas::getArch()) {
                                            allow.push_back(true);
                                        }
                                        else { allow.push_back(false); }
                                    } else { allow.push_back(true); }
                                } else { allow.push_back(false); }
                            } else { allow.push_back(true); }
                        } // allow
                        if (rule["action"] == "disallow") {
                            if (!rule["os"].empty()) {
                                if (rule["os"]["name"].asString() == consolas::getOS()) {
                                    if (!rule["os"]["arch"].empty()) {
                                        if (rule["os"]["arch"].asString() == consolas::getArch()) {
                                            allow.push_back(false);
                                        }
                                        else { allow.push_back(true); }
                                    } else { allow.push_back(false); }
                                } else { allow.push_back(true); }
                            } else { allow.push_back(false); }
                        }
                    }
                    if (std::all_of(allow.begin(), allow.end(), [](bool val) { return val; })) {
                        libraries_.append(aLibrary);
                    }
                }
            }
        }
    }
}