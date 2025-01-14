//
// Created by WswDay2022 on 2025/1/5.
//

#include "cryptoManager.h"

cryptoManager::cryptoManager(Algorithm algo, const std::string& keyString)
    : algorithm(algo) {
    setKey(keyString);  // 调用设置密钥函数
}

void cryptoManager::setKey(const std::string& keyString) {
    if (!isValidKeyFormat(keyString)) {  // 检查密钥格式
        throw std::invalid_argument("无效的密钥格式。期望格式: XXXXX-XXXXX-XXXXX-XXXXX-XXXXX");
    }

    // 移除密钥字符串中的连字符
    std::string cleanKey = keyString;
    cleanKey.erase(remove(cleanKey.begin(), cleanKey.end(), '-'), cleanKey.end());

    // 确保密钥大小符合 DES（8字节）或 AES（16字节）的要求
    if (algorithm == DES_MODE && cleanKey.size() != 16) {
        throw std::invalid_argument("DES 密钥必须为 16 个字符（去掉连字符）。");
    } else if (algorithm == AES_MODE && cleanKey.size() != 32) {
        throw std::invalid_argument("AES 密钥必须为 32 个字符（去掉连字符）。");
    }

    std::copy(cleanKey.begin(), cleanKey.begin() + (algorithm == DES_MODE ? 16 : 32), key);  // 将密钥复制到缓冲区
}

bool cryptoManager::isValidKeyFormat(const std::string& keyString) {
    // 检查密钥长度和格式是否符合要求
    return keyString.length() == 23 && keyString[5] == '-' && keyString[11] == '-' && keyString[17] == '-';
}

std::string cryptoManager::encrypt(const std::string& plainText) {
    std::string cipherText;

    try {
        if (algorithm == DES_MODE) {
            // 使用 DES 加密
            CryptoPP::ECB_Mode<CryptoPP::DES>::Encryption encryptor(key, sizeof(key));
            CryptoPP::StringSource(plainText, true,new CryptoPP::StreamTransformationFilter(encryptor,new CryptoPP::StringSink(cipherText)));
        } else if (algorithm == AES_MODE) {
            // 使用 AES 加密
            CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryptor(key, sizeof(key));
            CryptoPP::StringSource(plainText, true,new CryptoPP::StreamTransformationFilter(encryptor,new CryptoPP::StringSink(cipherText)));
        }
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "加密错误: " << e.what() << std::endl;  // 输出错误信息
        throw;
    }

    return cipherText;  // 返回密文
}

std::string cryptoManager::decrypt(const std::string& cipherText) {
    std::string recoveredText;

    try {
        if (algorithm == DES_MODE) {
            // 使用 DES 解密
            CryptoPP::ECB_Mode<CryptoPP::DES>::Decryption decryptor(key, sizeof(key));
            CryptoPP::StringSource(cipherText, true,new CryptoPP::StreamTransformationFilter(decryptor,new CryptoPP::StringSink(recoveredText)));
        } else if (algorithm == AES_MODE) {
            // 使用 AES 解密
            CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decryptor(key, sizeof(key));
            CryptoPP::StringSource(cipherText, true,new CryptoPP::StreamTransformationFilter(decryptor,new CryptoPP::StringSink(recoveredText)));
        }
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "解密错误: " << e.what() << std::endl;  // 输出错误信息
        throw;
    }

    return recoveredText;  // 返回明文
}

void cryptoManager::encryptChunk(const std::string& chunk, std::string& output) {
    std::string encryptedChunk = encrypt(chunk);  // 加密数据块
    std::lock_guard<std::mutex> lock(mtx);  // 确保线程安全
    output += encryptedChunk;  // 将加密结果添加到输出中
}

void cryptoManager::fileEncryptThread(const std::string& chunk, std::string& output, Algorithm algo, const CryptoPP::byte* key) {
    // 创建一个局部的 CryptoManager 实例
    cryptoManager tempManager(algo, "");  // 创建临时管理器，密钥用空字符串初始化
    std::copy(key, key + (algo == DES_MODE ? 16 : 32), tempManager.key);  // 复制密钥到临时管理器
    std::string encryptedChunk = tempManager.encrypt(chunk);  // 加密数据块
    std::lock_guard<std::mutex> lock(tempManager.mtx);  // 确保线程安全
    output += encryptedChunk;  // 将加密结果添加到输出中
}

void cryptoManager::encryptFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream file(inputFile, std::ios::binary);  // 打开输入文件
    std::ofstream outFile(outputFile, std::ios::binary);  // 打开输出文件

    if (!file.is_open() || !outFile.is_open()) {  // 检查文件是否成功打开
        throw std::runtime_error("无法打开文件。");
    }

    std::string chunk;
    std::vector<std::thread> threads;  // 线程列表
    std::vector<std::string> encryptedChunks;  // 存储每个线程加密后的结果
    std::mutex outputMutex;  // 用于保护访问输出的互斥量

    // 持续读取文件，直到文件结束
    while (std::getline(file, chunk)) {
        if (!chunk.empty()) {  // 检查读取到的行是否为空
            // 使用一个 lambda 函数来定义线程的工作内容
            threads.emplace_back([this, chunk, &encryptedChunks, &outputMutex]() {
                std::string encryptedChunk = this->encrypt(chunk);  // 加密数据块
                std::lock_guard<std::mutex> lock(outputMutex);  // 确保线程安全
                encryptedChunks.push_back(encryptedChunk);  // 将加密结果添加到 vector 中
            });
        }
    }

    // 等待所有线程结束
    for (auto& th : threads) { th.join(); }
    // 将所有加密块合并成最终输出
    std::string output;
    for (const auto& encChunk : encryptedChunks) {
        output += encChunk;  // 将所有加密结果合并
    }

    outFile << output;  // 将加密结果写入输出文件
    outFile.close();     // 关闭输出文件
    file.close();        // 关闭输入文件
}

void cryptoManager::decryptFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream file(inputFile, std::ios::binary);  // 打开输入文件
    std::ofstream outFile(outputFile, std::ios::binary);  // 打开输出文件

    if (!file.is_open() || !outFile.is_open()) {  // 检查文件是否成功打开
        throw std::runtime_error("无法打开文件。");
    }

    std::string cipherText((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));  // 读取密文
    std::string decryptedText = decrypt(cipherText);  // 解密密文

    outFile << decryptedText;  // 写入解密结果
    outFile.close();
    file.close();
}

std::vector<std::string> cryptoManager::bruteForceDES(const std::string& cipherText) {
    std::vector<std::string> results;  // 存储可能的密钥
    for (int i = 0; i < 256; ++i) {  // 这是一个简化的示例，应该进行改进
        std::string attempt(16, 'A' + (i % 26));  // 生成尝试的密钥
        setKey(attempt);  // 设置密钥
        if (decrypt(cipherText) == "") {  // 简单的验证检查
            results.push_back(attempt);  // 添加可能的密钥
        }
    }
    return results;  // 返回破解结果
}

std::vector<std::string> cryptoManager::bruteForceAES(const std::string& cipherText) {
    std::vector<std::string> results;  // 存储可能的密钥
    for (int i = 0; i < 256; ++i) {  // 这是一个简化的示例，应该进行改进
        std::string attempt(32, 'A' + (i % 26));  // 生成尝试的密钥
        setKey(attempt);  // 设置密钥
        if (decrypt(cipherText) == "") {  // 简单的验证检查
            results.push_back(attempt);  // 添加可能的密钥
        }
    }
    return results;  // 返回破解结果
}