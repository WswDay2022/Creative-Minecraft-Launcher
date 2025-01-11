//
// Created by WSW on 2024/11/16.
//

#ifndef CMCL_LAUNCH_H
#define CMCL_LAUNCH_H

#include <string>
#include "../../core/json/reader.h"
#include "../../core/json/value.h"
#include "../../core/core.h"
#include "../../core/consolas.h"
#include "../../core/url/download/downloader.h"
#include "../../core/io/fileReader.h"
#include "../../core/io/fileWriter.h"
#include <filesystem>
#include "assets.h"
#include <regex>
#include <random>

using namespace Log;

class launch {
public:
    launch(const std::string& minecraftDirectory, const std::string& versionJson,
           const std::string& versionDirectory, const std::string& versionId);

    void setGameArguments();
    void setJVMOptions();
    void setCPClasses();
    void setSystemInfo();

    std::string args_(std::string arg, std::string key, std::string to);
    std::string deletes_(std::string arg, std::string key);

    void init();
    void writeToFile(const std::string& filePath);
    bool easyLaunchGame();
    bool easyLaunchGameForFile(const std::string& filePath);

    void setGameUser();
    void setJAVA(const std::string &javaPath);

    std::string getArguments() { return launchArguments_; }

private:
    std::string javaPath_;
    std::string jvmArguments_;
    std::string gameArguments_;
    std::string cpClasses_;
    std::string mainClass_;
    std::string launchArguments_;

    std::string minecraftDirectory_;
    std::string versionJson_;
    std::string versionDirectory_;
    std::string versionId_;


};

#endif //CMCL_LAUNCH_H
