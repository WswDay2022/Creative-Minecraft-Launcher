//
// Created by WswDay2022 on 2024/10/20.
//

#ifndef CMCL_FILEREADER_H
#define CMCL_FILEREADER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class fileReader {
public:
    explicit fileReader(const std::string& filename);
    bool readFile();
    const std::vector<char>& getFileContent() const;
    std::string getFullContent(std::vector<char> vector);

private:
    std::string filename_;
    std::vector<char> fileContent_;
};

#endif //CMCL_FILEREADER_H
