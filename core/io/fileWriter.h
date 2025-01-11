//
// Created by WswDay2022 on 2024/11/1.
//

#ifndef CMCL_FILEWRITER_H
#define CMCL_FILEWRITER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class fileWriter {
public:
    explicit fileWriter(const std::string& filename);
    bool writeFile(std::string data);
    std::string getFilename() const;

private:
    std::string filename_;
};

#endif //CMCL_FILEWRITER_H
