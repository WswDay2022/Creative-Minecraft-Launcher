//
// Created by WswDay2022 on 2024/10/19.
//

#ifndef CMCL_CONSOLAS_H
#define CMCL_CONSOLAS_H

#include <iostream>
#include <vector>

class consolas {
private:
    static tm *get_time();
    static double get_now_hour();
    static double get_now_minute();
    static double get_now_second();
    static std::string get_full_time();
public:
    static void sclean();
    static void cclean();

    static void print(const std::string &args);
    static void printf(const std::string &args);
    static void print_err(const std::string &args);
    static void printf_err(const std::string &args);
    static void input(const std::string &args, std::string to);
    static void input(std::string to);

    static std::string getOS();
    static std::string getArch();
    static std::string getOSName();

    static std::string rinput(const std::string &args);
    static std::string rinput();
    static int createDirectory(std::string path);
    static int exist(std::string path);
    static void run_cmd(const char *args);

    class logo {
    public:
        static void info(std::string describe, std::string args);
        static void err(std::string describe, std::string args);

        static void wran(std::string describe, std::string args);
    };

    static bool isContainsAllCharacters(const std::string &mainString, const std::string &subString);
    static void sleep(int time);

    static std::string joinPath(std::string pathA,std::string pathB);
    static std::string formatPath(std::string path);

    static std::vector<std::string> stringSplit(std::string str, std::string pattern);
};


#endif //CMCL_CONSOLAS_H
