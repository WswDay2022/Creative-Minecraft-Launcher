//
// Created by WswDay2022 on 2024/11/22.
//

#include "launch.h"

launch::launch(const std::string &minecraftDirectory, const std::string &versionJson, const std::string& versionDirectory, const std::string& versionId)
    : minecraftDirectory_(minecraftDirectory), versionJson_(versionJson), versionDirectory_(versionDirectory), versionId_(versionId) {}

void launch::init() {
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(versionJson_,value)) return;
    Json::Value content = value;
    if (content.empty()) {
        LogPrint("[IO]:JSON文件是空的","ERROR");
        return;
    }
    mainClass_ = value["mainClass"].asString();
    javaPath_ = "${java_path}";

    setGameArguments();
    setCPClasses();
    setJVMOptions();
    setSystemInfo();

    const std::string jvm = "\"" + javaPath_ + "\" " + jvmArguments_;
    const std::string arguments = mainClass_ + " " + gameArguments_;

    launchArguments_ = jvm + arguments;
    launchArguments_ = args_(launchArguments_,R"(${natives_directory})","\"" + core::joinPath(versionDirectory_,versionId_+"-natives") + "\"");
    launchArguments_ = args_(launchArguments_,R"(${classpath})","\"" + cpClasses_ + "\"");

    setGameUser();
}

std::string launch::args_(std::string arg,std::string key, std::string to) {
    size_t start_pos = 0;
    while ((start_pos = arg.find(key, start_pos)) != std::string::npos) {
        arg.replace(start_pos, key.length(), to);
        start_pos += to.length();
    }
    return arg;
}

std::string launch::deletes_(std::string arg, std::string key) {
    return args_(std::move(arg),std::move(key),"");
}

bool launch::easyLaunchGame() {
    if (std::system(launchArguments_.c_str()) == 0) { return true;
    } else { return false; }
}

bool launch::easyLaunchGameForFile(const std::string &filePath) {
    std::filesystem::path _path(filePath);
    std::filesystem::path fullPath = std::filesystem::absolute(_path);
    if (std::system(fullPath.string().c_str()) == 0) { return true;
    } else { return false; }
}