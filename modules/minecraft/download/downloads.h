//
// Created by WswDay2022 on 2024/11/29.
//

#ifndef CMCL_DOWNLOADS_H
#define CMCL_DOWNLOADS_H

#include <string>
#include "../assets.h"
#include "../../../core/consolas.h"
#include "../../../core/json/value.h"
#include "../../../core/core.h"
#include "../../../core/json/reader.h"

class downloads {
public:
    downloads(const std::string &minecraftDirectory, const std::string &versionJson);

    void downloadAnGameVersion(const std::string &versionName,const std::string &versionId);
    void downloadObjects();
    void downloadAssets();
    void downloadLibraries();

private:


    std::string minecraftDirectory_;
    std::string versionJson_;
};

#endif //CMCL_DOWNLOADS_H
