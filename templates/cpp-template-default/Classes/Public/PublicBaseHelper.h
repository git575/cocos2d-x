#pragma once
#include <string>
#include "PublicMacros.h"
#include <unordered_map>
#include <ctime>
#include <vector>
#include <functional>
#include <cstdint>

// 初始化
void PublicInitGame(const std::string& gameName);

// 获取游戏名
std::string PublicGetGameName();

// 添加资源搜索路径
void PulicSetFileSearchPath(const std::string& path);

// 读取文件
std::string PublicGetFileString(const std::string& path);
// 删除文件
bool PublicRemoveFile(const std::string& path);

// 随机数[] 
int PublicRandInt(int min, int max);

// 读取plist文件并加载纹理的函数
void PublicLoadSpriteFrameData(const std::string& plistPath);

// 返回开始界面
void PublicReturnToStartScene();

// 获取语言
std::string PublicGetLanguageString();

// 设置音效开关
class PublicAudioData
{
	SINGLETON_IMPLEMENT(PublicAudioData);
public:
	bool getEffectSwitch(const std::string& gameName);
	bool getMusicSwitch(const std::string& gameName);
	void setEffectSwitch(const std::string& gameName, bool isOpen);
	void setMusicSwitch(const std::string& gameName, bool isOpen);
private:
	bool m_bGlobalEffectSwitch;
	bool m_bGlobalMusicSwitch;
	std::unordered_map<std::string, bool> m_mapGameEffectSwitch;
	std::unordered_map<std::string, bool> m_mapGameMusicSwitch;
};
void PublicInitGameAudioSwitch(const std::string& gameName);
// 设置音效开关
void PublicSetEffectSwitch(bool isOpen);
bool PublicGetEffectSwitch();
// 设置背景音乐开关
void PublicSetMusicSwitch(bool isOpen);
bool PublicGetMusicSwitch();
// 预加载
void PublicPreloadEffect(const std::string& effect, std::function<void(bool isSuccess)> callback = nullptr);
void PublicPreloadEffect(const std::vector<std::string>& effects);
// 播放音效
void PublicPlayEffect(const std::string& effect);
// 播放背景音乐
void PublicPlayMusic(const std::string& music);

// 获取时间戳
std::time_t PublicGetCurrentTime();
/*
获取当前时间
format 格式参考：
------------------ -
% Y : 年份（如 2025）
% m : 月份（01 - 12）
% d : 日（01 - 31）
% H : 小时（00 - 23）
% M : 分钟（00 - 59）
% S : 秒（00 - 59）
% a : 星期几简称（如 Wed）
% A : 星期几全称（如 Wednesday）
% Z : 时区（如 CST）
% c : 日期和时间（如 Wed Apr 23 16 : 45 : 00 2025）
*/
std::string PublicGetCurrentTimeString(const std::string& format = "%Y-%m-%d %H:%M:%S");
// 获取时间
std::string PublicGetTimeString(std::time_t, const std::string& format = "%Y-%m-%d %H:%M:%S");

// 静态变量
class PublicStatic
{
	SINGLETON_IMPLEMENT(PublicStatic);
public:
	static void setInt(const std::string& key, int val);
	static int getInt(const std::string& key);
	static void setString(const std::string& key, const std::string& val);
	static std::string getString(const std::string& key);
	static void setInt64(const std::string& key, std::int64_t val);
	static std::int64_t getInt64(const std::string& key);
private:
	std::unordered_map<std::string, int> m_ints;
	std::unordered_map<std::string, std::string> m_strings;
	std::unordered_map<std::string, std::int64_t> m_int64;
};

// 洗牌
template <class _RanIt>
void PublicShuffle(_RanIt _First, _RanIt _Last)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(_First, _Last, gen);
}