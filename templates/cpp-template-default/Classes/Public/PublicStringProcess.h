#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include "Public/PublicMacros.h"

// 字符串分割
std::vector<std::string> PublicSplit(const std::string& str, const std::string& separator);

// 格式化字符串
std::string PublicFormatString(const char* format, ...);

// 读取文本字符串
void PublicInitGameText(const std::string& game);
std::string getStringForKey(const std::string& key);
std::string getStringByID(int id);
std::string getErrorCode(int errorCode);
std::string getErrorCode(const std::string& errorCode);

class PublicTextStringManager {
	SINGLETON_IMPLEMENT(PublicTextStringManager);
public:
	void loadText(const std::string& prefix);
	std::string getTextBy(const std::string& key);
	std::string getTextBy(int key);
	std::string getErrorBy(const std::string& key);
public:
	std::string m_sLanguage;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_mapKeyTexts;
	std::unordered_map<std::string, std::unordered_map<int, std::string>> m_mapIDTexts;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_mapErrorTexts;
};

