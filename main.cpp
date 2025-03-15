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

    std::string url = "https://software.download.prss.microsoft.com/dbazure/Win11_24H2_Pro_Chinese_Simplified_x64.iso?t=9d133a1c-e960-4a2b-a6d7-4c80a98d6c29&P1=1741423806&P2=601&P3=2&P4=Y0eWtKtk%2b%2fiAga%2bm4doJVOJDU7aIbI0hrxAfu44aa7wegu05TT7RuFMzooCETaioXqf2WwJlzqYfruKqtO9%2b%2bLGoZ6CNDiT06rsdnkNCCvzz7KULcWEbs8XS%2f7ZDKu%2bvTBZj%2fPks0qQwGxRsn3r4nMGqCmiW0W5ytXNL9nQ0tNaNorV6N0GQUORLBZgA%2fY%2fA%2fCfFnjbSrVmren17il4MBvZ5QgJk%2fma2mDNZnayJRr%2fK1%2fadndDpZdfYxBPs9KVkHW3Js510jCUeMFho6MPXxogksYPKMVtjyzeA3jamX8mvEg0bHnxJHRScE6NceBUNan9nY3E5qkmoB2SKK7VMPQ%3d%3d";
    std::string path = "G:\\Users\\wsz\\Desktop\\testa\\win11.iso";

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
    //downloader.download(url,path);

    QApplication app(argc,argv); // 主窗口程序
    mainApplication mainWindow;
    mainWindow.show();
    return app.exec();
}
