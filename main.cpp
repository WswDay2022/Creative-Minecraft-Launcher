#include <QApplication>
#include "core/core.h"
#include "modules/minecraft/launch.h"
#include "modules/minecraft/assets.h"
#include "modules/interface/mainApplication.h"
#include "modules/minecraft/download/downloads.h"
#include "core/cryptoManager.h"

/**
 * Creative Minecraft Launch For Minecraft:Java
 * Author : WswDay2022
 * Version : alpha-0.1
 */

/**
 * 一切的起源！
 * ----入口函数！
 * 一切都从这里开始！
 **/

using namespace Log;

int main(int argc, char *argv[]) {
    system("chcp 65001");
    cleanLogFile(); /* 清空日志文件，每次开始先将其清空 */
    LogPrint("[Launch]: 这是一个日志。");

    std::string key = "ABCDE-FGHIJ-KLMNO-PQRST-UVWXY";
    fileReader reader("D:\\.minecraft\\versions\\1.16.5-Forge_36.2.34\\1.16.5-Forge_36.2.34.json");
    reader.readFile();

    QApplication app(argc,argv); // 主窗口程序
    mainApplication mainWindow;
    mainWindow.show();
    return app.exec();
}
