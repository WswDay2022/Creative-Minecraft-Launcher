//
// Created by WswDay2022 on 2024/11/16.
//

#include "launch.h"

void launch::setGameArguments() {
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(versionJson_,value)) return;
    Json::Value content = value;
    if (content.empty()) {
        LogPrint("JSON文件是空的","ERROR");
        return;
    }

    const Json::Value& argument = content["arguments"];
    const Json::Value& argument_ = content["minecraftArguments"];

    fileReader reader1("../assets/setting.json");
    reader1.readFile();
    const std::string& setString = reader1.getFullContent(reader1.getFileContent());
    Json::Value value1;

    if (!argument_.empty()) {
        gameArguments_ += argument_.asString() + " ";
    }

    if (!argument.empty()) {
        for (int i = 0;i < argument["game"].size();i++) {
            Json::Value aKey = argument["game"][i];
            if (aKey.isString()) {
                gameArguments_ += aKey.asString() + " ";
            } else {
                const Json::Value& rules = aKey["rules"];
                const Json::Value& values = aKey["value"];
                for (int j = 0;j < rules.size();j++) {
                    if (rules[j]["action"].asString() == "allow") {
                        if (values.isArray()) for (int ji = 0;ji < values.size();ji++) {
                                gameArguments_ += values[ji].asString() + " ";
                            } else if (values.isString()) {
                            gameArguments_ += values.asString() + " ";
                        }
                    }
                }
            }
        }
    }
    if (reader.parse(setString,value1)) {
        gameArguments_ += value1["gameArguments"]["arguments"].asString() + " ";
        if (!consolas::isContainsAllCharacters(gameArguments_,"--width")) {
            gameArguments_ += "--width " + std::to_string(value1["gameArguments"]["width"].asInt()) + " ";
        } else gameArguments_ = args_(gameArguments_,R"(${resolution_width})",std::to_string(value1["gameArguments"]["width"].asInt()));
        if (!consolas::isContainsAllCharacters(gameArguments_,"--height")) {
            gameArguments_ += "--height " + std::to_string(value1["gameArguments"]["height"].asInt()) + " ";
        } else gameArguments_ = args_(gameArguments_,R"(${resolution_height})",std::to_string(value1["gameArguments"]["height"].asInt()));
        if (!consolas::isContainsAllCharacters(gameArguments_,"--versionType")) {
            gameArguments_ += "--versionType " + value1["launcherShortName"].asString() + " " + value1["launcherVersion"].asString();
        } else gameArguments_ = args_(gameArguments_,R"(${version_type})",value1["launcherShortName"].asString());

        if (value1["gameArguments"]["fullScreen"].asBool()) { if (!consolas::isContainsAllCharacters(gameArguments_,"--fullScreen")) { gameArguments_ += "--fullScreen "; }
        } else { if (consolas::isContainsAllCharacters(gameArguments_,"--fullScreen")) { gameArguments_ = deletes_(gameArguments_, R"(--fullScreen )"); } }
        if (value1["gameArguments"]["demo"].asBool()) { if (!consolas::isContainsAllCharacters(gameArguments_,"--demo")) { gameArguments_ += "--demo "; }
        } else { if (consolas::isContainsAllCharacters(gameArguments_,"--demo")) { gameArguments_ = deletes_(gameArguments_,R"(--demo )"); } }

        if (!value["gameArguments"]["quickPlay"].asBool()) {
            gameArguments_ = deletes_(gameArguments_,"--quickPlayPath ${quickPlayPath}");
            gameArguments_ = deletes_(gameArguments_,"--quickPlaySingleplayer ${quickPlaySingleplayer}");
            gameArguments_ = deletes_(gameArguments_,"--quickPlayMultiplayer ${quickPlayMultiplayer}");
            gameArguments_ = deletes_(gameArguments_,"--quickPlayRealms ${quickPlayRealms}");
        }

        gameArguments_ = args_(gameArguments_, R"(${version_name})", versionId_);
        gameArguments_ = args_(gameArguments_, R"(${game_directory})", "\"" + versionDirectory_ + "\"");
        gameArguments_ = args_(gameArguments_, R"(${assets_root})","\"" + core::joinPath(minecraftDirectory_, "assets") + "\"");
        gameArguments_ = args_(gameArguments_, R"(${assets_index_name})", value["assets"].asString());
    }
}

void launch::setCPClasses() {
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(versionJson_,value)) return;
    Json::Value content = value;
    if (content.empty()) {
        LogPrint("JSON文件是空的","ERROR");
        return;
    }
    #if defined(WIN32) || defined(WIN64)
        std::string cpSplit = ";";
    #else
        std::string cpSplit = ":";
    #endif

    assets asset(minecraftDirectory_,versionJson_);
    asset.parserLibraries();
    asset.parserNatives();
    const Json::Value libraries = asset.getLibraries();
    const Json::Value natives = asset.getNatives();

    int notFoundLibraries = 0;

    for (int i = 0;i < libraries.size();i++) {
        cpClasses_ += libraries[i]["path"].asString() + cpSplit;
        if (consolas::exist(libraries[i]["path"].asString()) != 0) {
            notFoundLibraries++;
        }
    }
    for (int i = 0;i < natives.size();i++) {
        cpClasses_ += natives[i]["path"].asString() + cpSplit;
        if (consolas::exist(natives[i]["path"].asString()) != 0) {
            notFoundLibraries++;
        }
    }
    LogPrint("一共有"+std::to_string(notFoundLibraries)+"个库未找到。","WARNING");
    cpClasses_ += core::joinPath(versionDirectory_,versionId_+".jar");
}

void launch::setJVMOptions() {
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(versionJson_,value)) return;
    Json::Value content = value;
    if (content.empty()) {
        LogPrint("JSON文件是空的","ERROR");
        return;
    }

    fileReader reader1("../assets/setting.json");
    reader1.readFile();
    const std::string& setString = reader1.getFullContent(reader1.getFileContent());
    Json::Value value1;
    Json::Value jvmOption = value["arguments"]["jvm"];

    if (reader.parse(setString,value1)) {
        jvmArguments_ += value1["gameArguments"]["jvm"].asString() + " ";
        jvmArguments_ += "-Xmn" + std::to_string(value1["gameArguments"]["minMemory"].asInt()) +"m ";
        jvmArguments_ += "-Xmx" + std::to_string(value1["gameArguments"]["maxMemory"].asInt()) +"m ";
    }

    if (!jvmOption.empty()) {
        for (int i = 0; i < jvmOption.size(); i++) {
            const Json::Value &jvm = jvmOption[i];
            if (!jvm.empty()) {
                if (!jvm.isString()) {
                    Json::Value rules = jvm["rules"];
                    std::vector<bool> allow;
                    for (int j = 0; j < rules.size(); ++j) {
                        const Json::Value &rule = rules[j];
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
                    if (std::all_of(allow.begin(), allow.end(), [](bool val) { return val; })) {
                        if (jvm["value"].isArray()) { for (int j = 0; j < jvm["value"].size(); j++) { jvmArguments_ += jvm["value"][j].asString() + " "; }}
                        if (jvm["value"].isString()) { jvmArguments_ += jvm["value"].asString() + " "; }
                    }
                } else if (jvm.isString()) { jvmArguments_ += jvm.asString() + " "; }
            }
        }
    } else {
        if (consolas::getOS() == "windows") {
            jvmArguments_ += "-XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump ";
            if (consolas::getArch() == "x86") { jvmArguments_ += "-Xss1M "; }
        } else if (consolas::getOS() == "osx") { jvmArguments_ += "-XstartOnFirstThread "; }
        jvmArguments_ += "-cp ${classpath} -Djava.library.path=${natives_directory} ";
        jvmArguments_ += "-Dminecraft.launcher.brand=${launcher_name} ";
        jvmArguments_ += "-Dminecraft.launcher.version=${launcher_version} ";
        jvmArguments_ += "-Dos.name=${os_name} -Dos.version=${os_version} ";
    }

    if (reader.parse(setString,value1)) {
        jvmArguments_ = args_(jvmArguments_,R"(${launcher_name})","\"" + value1["launcherLongName"].asString() + "\"");
        jvmArguments_ = args_(jvmArguments_,R"(${launcher_version})","\"" + value1["launcherVersion"].asString() + "\"");
    }
}

void launch::writeToFile(const std::string& filePath) {
    fileWriter writer(filePath);
    writer.writeFile(launchArguments_);
}

void launch::setGameUser() {
    fileReader reader("../assets/setting.json");
    reader.readFile();
    const std::string& content = reader.getFullContent(reader.getFileContent());
    Json::Value value;
    Json::Reader jsonReader;
    if (!jsonReader.parse(content,value)) return;
    for (int i = 0;i < value.size();i++) {
        if (value["users"][i]["action"].asBool()) {
            const std::string &uuid = value["users"][i]["uuid"].asString();
            const std::string &user = value["users"][i]["userName"].asString();
            const std::string &userType = value["users"][i]["userType"].asString();

            launchArguments_ = args_(launchArguments_,R"(${auth_player_name})",user);
            launchArguments_ = args_(launchArguments_,R"(${auth_uuid})",uuid);
            launchArguments_ = args_(launchArguments_,R"(${auth_access_token})",uuid);
            launchArguments_ = args_(launchArguments_,R"(${user_type})",userType);
        }
    }
}

void launch::setSystemInfo() {
    if (core::isContainsAllCharacters(jvmArguments_,"-Dos.name=")) {
        jvmArguments_ = args_(jvmArguments_,"${os_name}",core::getSystemOSName());
    } else {
        jvmArguments_ += "-Dos.name=\"" + core::getSystemOSName() + "\" ";
    }
    if (core::isContainsAllCharacters(jvmArguments_,"-Dos.version=")) {
        jvmArguments_ = args_(jvmArguments_,"${os_version}",core::getSystemOSVersion());
    } else {
        jvmArguments_ += "-Dos.version=" + core::getSystemOSVersion() + " ";
    }
}

void launch::setJAVA(const std::string& javaPath) {
    launchArguments_ = args_(launchArguments_,R"(${java_path})",javaPath);
}