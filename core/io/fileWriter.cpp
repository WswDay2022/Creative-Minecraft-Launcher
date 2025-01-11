//
// Created by WswDay2022 on 2024/10/20.
//

#include "fileWriter.h"

fileWriter::fileWriter(const std::string& filename) : filename_(filename) {}

bool fileWriter::writeFile(std::string data) {
    std::ofstream file(filename_, std::ios::binary);
    if (!file) { return false; }
    file.write(data.data(), data.size());
    if (!file) { return false; }
    return true;
}

std::string fileWriter::getFilename() const {
    return filename_;
}