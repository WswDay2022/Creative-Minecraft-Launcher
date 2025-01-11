//
// Created by WswDay2022 on 2024/10/20.
//

#include "fileReader.h"

fileReader::fileReader(const std::string& filename) : filename_(filename) {}

bool fileReader::readFile() {
    std::ifstream file(filename_, std::ios::binary);
    if (!file) { return false; }
    fileContent_.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (!file) { return false; }
    return true;
}

const std::vector<char>& fileReader::getFileContent() const {
    return fileContent_;
}

std::string fileReader::getFullContent(std::vector<char> vector) {
    std::vector<char> fullContent = vector;
    std::string content;
    for (size_t i = 0; i < fullContent.size(); ++i) {
        content += vector[i];
    }
    return content;
}