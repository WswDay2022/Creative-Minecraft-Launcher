//
// Created by wsz on 2025/1/5.
//

#ifndef CMCL_CRYPTOMANAGER_H
#define CMCL_CRYPTOMANAGER_H

#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <cryptopp/cryptlib.h>
#include <cryptopp/aes.h>
#include <cryptopp/des.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

// 加密管理类
class cryptoManager {
public:
    enum Algorithm {
        DES_MODE,  // 数据加密标准
        AES_MODE   // 高级加密标准
    };

    cryptoManager(Algorithm algo, const std::string& key);  // 构造函数，设定算法和密钥

    std::string encrypt(const std::string& plainText);  // 加密明文
    std::string decrypt(const std::string& cipherText);  // 解密密文

    void encryptFile(const std::string& inputFile, const std::string& outputFile);  // 加密文件
    void decryptFile(const std::string& inputFile, const std::string& outputFile);  // 解密文件

    std::vector<std::string> bruteForceDES(const std::string& cipherText);  // 暴力破解DES
    std::vector<std::string> bruteForceAES(const std::string& cipherText);  // 暴力破解AES

private:
    void setKey(const std::string& keyString);  // 设置密钥
    void encryptChunk(const std::string& chunk, std::string& output);  // 加密数据块
    static void fileEncryptThread(const std::string& chunk, std::string& output, Algorithm algo, const CryptoPP::byte* key);  // 文件加密线程

    bool isValidKeyFormat(const std::string& keyString);  // 验证密钥格式

    Algorithm algorithm;  // 加密算法
    CryptoPP::byte key[32];  // 密钥缓冲区
    std::mutex mtx;  // 确保文件加密时线程安全
};

#endif //CMCL_CRYPTOMANAGER_H
