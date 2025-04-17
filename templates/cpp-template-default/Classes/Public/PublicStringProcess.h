#pragma once
#include <vector>
#include <string>

// 字符串分割
std::vector<std::string> PublicSplit(const std::string& str, const std::string& separator);

// 格式化字符串
std::string formatString(const std::string& format, ...);

// 读取文本字符串
std::string getStringForKey(const std::string& key);
std::string getStringByID(int id);
std::string getErrorCode(int errorCode);