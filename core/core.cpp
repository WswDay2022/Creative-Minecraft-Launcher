//
// Created by WswDay2022 on 2024/11/30.
//

#include "core.h"

void Log::LogPrint(const std::string &args, const std::string decr) {
    core core_;core_.globalInit();
    std::string launcherName = "[" + core_.getSettingJson()["launcherShortName"].asString() + "]";

    const std::string hour = std::to_string(int(core_.getNowHour()));
    const std::string second = std::to_string(int(core_.getNowSecond()));
    const std::string minute = std::to_string(int(core_.getNowMinute()));
    std::string time = hour + ":" + minute + ":" + second;

    std::string content = launcherName + "[" + time + "][" + decr + "]" + args;
    std::cout << content << std::endl;

    const std::string filePath = core::joinPath(core_.getSettingJson()["launcherTempDir"].asString(),"LauncherLogging.log");
    std::filesystem::path filePath_(filePath);
    if (!core::exist(filePath_.parent_path().string())) {
        std::cout << launcherName + "[IO]:未检测到缓存文件夹，先创建" << std::endl;
        if(!core::createDirectory(filePath_.parent_path().string())) {
            std::cout << launcherName + "[IO]:缓存文件夹创建失败" << std::endl;
        }
    }
    std::ofstream logFile(filePath,std::ios::binary | std::ios::app);
    if (logFile.is_open()) {
        if (!logFile.write((content+"\n").c_str(),(content+"\n").size())) {
            std::cout << launcherName + ":写入日志文件失败，将会导致无法查看历史日志" << std::endl;
        }
    } else {
        std::cout << launcherName + ":打开日志文件失败，将会导致无法查看历史日志" << std::endl;
    }
}

void Log::NoWriteToFileLog(const std::string &args, const std::string decr) {
    core core_;core_.globalInit();
    std::string launcherName = "[" + core_.getSettingJson()["launcherShortName"].asString() + "]";
    const std::string hour = std::to_string(int(core_.getNowHour()));
    const std::string second = std::to_string(int(core_.getNowSecond()));
    const std::string minute = std::to_string(int(core_.getNowMinute()));
    std::string time = hour + ":" + minute + ":" + second;
    std::string content = launcherName + "[" + time + "][" + decr + "]" + args;
    std::cout << content << std::endl;
}

void Log::cleanLogFile() {
    core core_;core_.globalInit();
    std::string path = core::joinPath(core_.getSettingJson()["launcherTempDir"].asString(),"LauncherLogging.log");
    fileWriter writer(path);
    writer.writeFile(""); // 写入空内容，清空日志文件
}

void core::globalInit() {
    readCSSFile();
    readSettingFile();
    QColor fontColor_(
            getSettingJson()["fontColor"][0].asInt(),
            getSettingJson()["fontColor"][1].asInt(),
            getSettingJson()["fontColor"][2].asInt());
    fontColor = fontColor_;
    QColor themeColor_(
            getSettingJson()["themeColor"][0].asInt(),
            getSettingJson()["themeColor"][1].asInt(),
            getSettingJson()["themeColor"][2].asInt());
    themeColor = themeColor_;
}

void core::readSettingFile() {
    fileReader reader("../assets/setting.json");
    if (!reader.readFile()) {
        std::cout << "[Launcher-CORE][ERROR]:无法读取设置文件" << std::endl;
        return;
    }
    const std::string& content = reader.getFullContent(reader.getFileContent());
    settingFileContent_ = content;
    Json::Value value;Json::Reader jsonReader;
    if (!jsonReader.parse(content,value)) {
        std::cout << "[Launcher-CORE][ERROR]:无法解析设置文件" << std::endl;
        return;
    }
    settingFileJson_ = value;
}

void core::readCSSFile() {
    fileReader reader("../assets/main.css");
    if (!reader.readFile()) {
        std::cout << "[Launcher-CORE][ERROR]:无法读取样式文件" << std::endl;
        return;
    }
    const std::string& content = reader.getFullContent(reader.getFileContent());
    cssFileContent_ = content;
    QCssFileContent_ = QString::fromStdString(content);
}

void core::writeLaunchFile(const std::string &content) {
    fileWriter writer("../assets/launch.bat");
    if (!writer.writeFile(content)) {
        std::cout << "[Launcher-CORE][ERROR]:无法写入启动文件" << std::endl;
        return;
    }
}

tm* core::getTime() {
    time_t nowTime;
    time(&nowTime);
    tm* pTime = localtime(&nowTime);
    return pTime;
}

std::string core::joinPath(const std::string& pathA, const std::string& pathB) {
    std::filesystem::path path1(pathA);
    std::filesystem::path path2(pathB);
    std::filesystem::path fullPath = path1 / path2;
    return fullPath.string();
}

std::string core::formatPath(const std::string& path) {
    std::filesystem::path inputPath(path);
    std::filesystem::path resultPath;
    for (const auto& part : inputPath) {
        if (!part.empty()) {
            resultPath /= part;
        }
    }
    return resultPath.string();
}

int core::createDirectory(std::string path) {
    std::filesystem::path path1(path);
    return std::filesystem::create_directories(path1);
}

int core::exist(std::string path) {
    std::filesystem::path path1(path);
    return std::filesystem::exists(path1);
}

std::string core::getSystemOSName() { return QSysInfo::prettyProductName().toStdString(); }
std::string core::getSystemOSVersion() { return QSysInfo::productVersion().toStdString(); }
std::string core::getSystemArchitecture() { return QSysInfo::currentCpuArchitecture().toStdString(); }

bool core::isContainsAllCharacters(const std::string &mainString, const std::string &subString) {
    return mainString.find(subString) != std::string::npos;
}

QFont core::getFont(int size) {
    int fontId = QFontDatabase::addApplicationFont("../assets/font.ttf");
    QFont customFont(QFontDatabase::applicationFontFamilies(fontId).at(0),size);
    return customFont;
}

QFont core::getFont(int size) {
    int fontId = QFontDatabase::addApplicationFont("../assets/font.ttf");
    QFont customFont(QFontDatabase::applicationFontFamilies(fontId).at(0),size);
    return customFont;
}
