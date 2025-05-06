#include "PublicStringProcess.h"
#include "PublicBaseHelper.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <string>
#include <cstdarg>
#include <cstdio>
using std::string;
using std::unordered_map;
using std::vector;
using std::stringstream;

std::vector<std::string> PublicSplit(const std::string& str, const std::string& separator)
{
	std::vector<std::string> ret;
	size_t sp_len = separator.size();
	size_t pre = 0;
	size_t cut_at = str.find(separator);
	while (cut_at != std::string::npos)
	{
		ret.push_back(str.substr(pre, cut_at - pre));
		pre = cut_at + sp_len;
		cut_at = str.find(separator, pre);
	}
	if (pre < str.size()) {
		ret.push_back(str.substr(pre));
	}
	return ret;
}

std::string PublicFormatString(const char* format, ...)
{
	// 初始化va_list
	va_list args;
	va_start(args, format);

	// 预估一个合理的缓冲区大小，假设最大为1024字节
	const size_t buffer_size = 1024;
	char buffer[buffer_size];

	// 使用vsprintf_s来执行格式化操作
	int n = vsprintf_s(buffer, buffer_size, format, args);

	va_end(args);

	// 如果格式化成功，返回一个std::string
	if (n >= 0) {
		return std::string(buffer);
	}
	else {
		return "";  // 格式化失败时返回空字符串
	}
};

void PublicInitGameText(const std::string& game)
{
	PublicTextStringManager::getInstance()->loadText(game);
}

std::string getStringForKey(const std::string& key)
{
	return PublicTextStringManager::getInstance()->getTextBy(key);
}

std::string getStringByID(int id)
{
	return PublicTextStringManager::getInstance()->getTextBy(id);
}

std::string getErrorCode(int errorCode)
{
	return getErrorCode(std::to_string(errorCode));
}

std::string getErrorCode(const std::string& errorCode)
{
	return PublicTextStringManager::getInstance()->getErrorBy(errorCode);
}

void PublicTextStringManager::loadText(const std::string& prefix)
{
	m_sLanguage = PublicGetLanguageString();
	auto textFile = prefix + "Text.json";
	std::string textString = PublicGetFileString(textFile);

	rapidjson::Document doc;
	doc.Parse(textString.c_str());

	if (!doc.HasParseError() && doc.HasMember("key_value") && doc["key_value"].IsObject())
	{
		const auto& val = doc["key_value"].GetObject();
		for (auto iter = val.MemberBegin(); iter != val.MemberEnd(); ++iter)
		{
			if (!iter->value.IsString()) { continue; }
			auto key = iter->name.GetString();
			auto value = iter->value.GetString();
			m_mapKeyTexts[m_sLanguage][key] = value;
		}
	}
}

std::string PublicTextStringManager::getTextBy(const std::string& key)
{
	if (m_sLanguage == "" || m_mapKeyTexts.find(m_sLanguage) == m_mapKeyTexts.end()) { return ""; }
	if (m_mapKeyTexts[m_sLanguage].find(key) == m_mapKeyTexts[m_sLanguage].end()) { return ""; }
	return m_mapKeyTexts[m_sLanguage][key];
}

std::string PublicTextStringManager::getTextBy(int key)
{
	if (m_sLanguage == "" || m_mapIDTexts.find(m_sLanguage) == m_mapIDTexts.end()) { return ""; }
	if (m_mapIDTexts[m_sLanguage].find(key) == m_mapIDTexts[m_sLanguage].end()) { return ""; }
	return m_mapIDTexts[m_sLanguage][key];
}

std::string PublicTextStringManager::getErrorBy(const std::string& key)
{
	if (m_sLanguage == "" || m_mapErrorTexts.find(m_sLanguage) == m_mapErrorTexts.end()) { return ""; }
	if (m_mapErrorTexts[m_sLanguage].find(key) == m_mapErrorTexts[m_sLanguage].end()) { return ""; }
	return m_mapErrorTexts[m_sLanguage][key];
}