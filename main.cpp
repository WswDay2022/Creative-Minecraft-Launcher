#include <QApplication>
#include "core/core.h"
#include "modules/minecraft/launch.h"
#include "modules/minecraft/assets.h"
#include "modules/interface/mainApplication.h"
#include "modules/minecraft/download/downloads.h"
#include "core/cryptoManager.h"
#include "core/url/download/multiThreadDownloader.h"
#include "core/url/webFetcher.h"
// #include "core/data/unzip/multiThreadUnzip.h"
#include "modules/minecraft/versions/manifestParser.h"
#include "core/url/HTMLParser.h"
#include "modules/minecraft/modloader/modCurseForge.h"

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
    cleanLogFile();
    LogPrint("[Launch]: 这是一个日志。");

    fileReader reader("G:\\Users\\wsz\\Desktop\\新建文本文档 (4).html");
    reader.readFile();

    std::string url = "https://software.download.prss.microsoft.com/dbazure/Win11_24H2_Pro_Chinese_Simplified_x64.iso?t=faca0af0-946a-43b6-a844-d372e1122039&P1=1742096610&P2=601&P3=2&P4=dMpdcw%2fqxAPHLugtbltpw00MFoev928TWo4WjjGeUx1q62G9q2a1v0rvjGL9o7f42CfNBDBQFie9rECsTxZPJWAJWoFhO2gdqbgWudRjhTl9E%2fSLNl4edOCWgf8Bgqrk90r%2bRwujLafcB3odlTsFA4ERVc7Jot5udBl1FsOxR95TL2CxmTDpNLl8VbdotESInVb5yylS5i075jCm6rqApmjQCnYzWx5rdARS9bbGUlSTt1EDvtVeZNurOECqUP7qpHNWqSNOZ5cfux2dfGNHBRZcfRE%2bGrHbzgk1nPPeGy1o%2f%2f6WWl8MrCfy%2bPwwJiiM6nVyQyjVijB0%2fgx4knxfSg%3d%3d";
    std::string path = "G:\\Users\\wsz\\Desktop\\testab\\win11.iso";

    manifestParser list(repoType::FASTER_REPO);
    // list.downloadMinecraftManifest("G:\\DownloadTest\\manifest.json");
    // list.downloadVersionJson("1.16.5","G:\\DownloadTest\\1.16.5.json");
    /*
    modCurseForge curseForge;
    for (auto a : curseForge.getModList()) {
        std::string modInfo = "Mod Name:" + a["name"].asString() +
                              "Mod Size:" + a["size"].asString() +
                              "Mod URL:" + a["url"].asString() +
                              "Mod Des:" + a["description"].asString();
        LogPrint("[TEST]:Mod:\n" + modInfo,"ERROR");
    }

    // downloads download("G:\\DownloadTest",reader.getFullContent(reader.getFileContent()));
    // download.downloadLibraries();
    // download.downloadObjects();
    */

    multiThreadDownloader downloader(10);
    downloader.download(url,path);

    QApplication app(argc,argv); // 主窗口程序
    mainApplication mainWindow;
    mainWindow.show();
    return app.exec();
}
