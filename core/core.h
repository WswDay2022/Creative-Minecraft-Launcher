//
// Created by WswDay2022 on 2024/11/30.
//

/**
 * Creative Minecraft Launcher Core
 * CMCL Core
 * Created by WswDay2022
 * */

#ifndef CMCL_CORE_H
#define CMCL_CORE_H

#include <direct.h>
#include <filesystem>
#include <string>
#include <qstring.h>
#include <QColor>
#include <QSysInfo>
#include <mutex>
#include <qfont.h>
#include "json/value.h"
#include "io/fileReader.h"
#include "consolas.h"
#include "json/reader.h"
#include "io/fileWriter.h"
#include <QFontDatabase>

/* 日志 */
namespace Log {
    void LogPrint(const std::string &args = nullptr,const std::string decr = "INFO");
    void NoWriteToFileLog(const std::string &args = nullptr,const std::string decr = "INFO");

    void cleanLogFile();
}

/* 核心 */
class core {
public:
    void globalInit();

    void readSettingFile();
    void readCSSFile();
    void writeLaunchFile(const std::string& content);

    std::string getSettingFile() { return settingFileContent_; };
    std::string getCssFile() { return cssFileContent_; }
    QString getQCssFile() { return QCssFileContent_; }
    Json::Value getSettingJson() { return settingFileJson_;}

    /* 时间获取 */
    tm* getTime();
    double getNowHour() { return getTime()->tm_hour; }
    double getNowMinute() { return getTime()->tm_min; }
    double getNowSecond() { return getTime()->tm_sec; }

    /* 系统信息 */
    static std::string getSystemOSName();
    static std::string getSystemOSVersion();
    static std::string getSystemArchitecture();

    /* 一些常量 */
    QColor fontColor;
    QColor themeColor;
    QColor warnColor = QColor(200,175,90);
    QColor errColor = QColor(220,70,70);

    /* 文件操作 */
    static std::string joinPath(const std::string& pathA, const std::string& pathB);
    static int createDirectory(std::string path);
    static std::string formatPath(const std::string &path);
    static int exist(std::string path);

    /* 字符串操作 */
    static bool isContainsAllCharacters(const std::string& mainString, const std::string& subString);
    static QFont getFont(int size = 10);

private:
    std::string settingFileContent_;
    std::string cssFileContent_;
    QString QCssFileContent_;
    Json::Value settingFileJson_;

};

#endif //CMCL_CORE_H
