//
// Created by wsw on 2024/10/19.
//

#include "consolas.h"
#include <iostream>
#include <ctime>
#include <io.h>
#include <string>
#include <direct.h>
#include <algorithm>
#include <regex>

tm* consolas::get_time() {
    time_t nowtime;
    time(&nowtime);
    tm* p = localtime(&nowtime);
    return p;
}

double consolas::get_now_hour() {return get_time()->tm_hour;}
double consolas::get_now_minute() {return get_time()->tm_min;}
double consolas::get_now_second() {return get_time()->tm_sec;}

std::string consolas::get_full_time() {
    return std::to_string(int (get_now_hour())) + ":" + std::to_string(int (get_now_minute())) + ":" + std::to_string(int (get_now_second()));
}

void consolas::run_cmd(const char *args) {system(args);}

void consolas::sclean() {run_cmd("cls");}
void consolas::cclean() {std::cout << "\033c";}

void consolas::print(const std::string& args) {std::cout << args << std::endl;}
void consolas::printf(const std::string& args) {std::cout << args;}
void consolas::print_err(const std::string& args) {std::cerr << args << std::endl;}
void consolas::printf_err(const std::string& args) {std::cerr << args;}

std::string consolas::getOS() {
    #if (defined _WIN32) || (defined _WIN64)
        return "windows";
    #elif defined __APPLE__
        return "osx";
    #elif defined __linux__
        return "linux";
    #endif
}

std::string consolas::getArch() {
    #if (defined __arm__) || (defined __aarch64__)
        return "arm";
    #elif (defined __i386__) || (defined __x86_64__)
    #ifdef __x86_64__
        return "x64";
    #elif defined(__i386__)
        return "x32";
    #endif
    #elif defined __APPLE__
        return "mac";
    #endif
}

std::string consolas::getOSName() {
    return std::getenv("OS");
}

void consolas::input(std::string to) {
    std::cin >> to;
    to = to;
}

void consolas::input(const std::string& args, std::string to) {
    printf(args);
    std::cin >> to;
    to = to;
}

std::string consolas::rinput(const std::string& args) {
    printf(args);
    std::string objs;
    std::cin >> objs;
    return objs;
}

std::string consolas::rinput() {
    std::string objs;
    std::cin >> objs;
    return objs;
}

void consolas::logo::info(std::string describe,std::string args) {
    std::clog << "[ INFO / "+get_full_time()+" / "+describe+" ]: "+args << std::endl;
}

void consolas::logo::err(std::string describe,std::string args) {
    std::cerr << "[ ERROR / "+get_full_time()+" / "+describe+" ]: "+args << std::endl;
}

void consolas::logo::wran(std::string describe,std::string args) {
    std::clog << "[ WARNING / "+get_full_time()+" / "+describe+" ]:"+args << std::endl;
}

int consolas::createDirectory(std::string path) {
    int m = 0, n;
    std::string str1, str2;
    str1 = path;
    str2 = str1.substr(0, 2);
    str1 = str1.substr(3, str1.size());
    while (m >= 0) {
        m = str1.find('\\');
        str2 += '\\' + str1.substr(0, m);
        n = _access(str2.c_str(), F_OK); //判断该目录是否存在
        if (n == -1) if (_mkdir(str2.c_str()) != 0) return -1;
        str1 = str1.substr(m + 1, str1.size());
    }
    return 0;
}

int consolas::exist(std::string path) {
    return _access(path.c_str(),F_OK);
}

bool consolas::isContainsAllCharacters(const std::string& mainString, const std::string& subString) {
    return mainString.find(subString) != std::string::npos;
}

void consolas::sleep(int time) {
    clock_t now = clock();
    while(clock() - now < time);
}

std::string consolas::joinPath(std::string pathA, std::string pathB) {
    std::string path;
    #if (defined _WIN32) || (defined _WIN64)
        if (std::to_string(pathA.at(pathA.size()-1)) != "\\") { path = pathA + "\\" + pathB;
        } else { path = pathA + pathB; }
    #else
        if (std::to_string(pathA.at(pathA.size()-1)) != "/") { path = pathA + "/" + pathB;
        } else { path = pathA + pathB; }
    #endif
    return path;
}

std::string consolas::formatPath(std::string path) {
    #if (defined _WIN32) || (defined _WIN64)
        return std::regex_replace(path, std::regex("/"), "\\");
    #else
        return std::regex_replace(path, std::regex("\\"), "/");
    #endif
}

std::vector<std::string> consolas::stringSplit(std::string str, std::string pattern) {
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}