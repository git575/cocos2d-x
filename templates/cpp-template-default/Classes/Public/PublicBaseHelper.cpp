#include "cocos2d.h"
#include "ccUtils.h"
#include "StartScene.h"
#include "PublicBaseHelper.h"
#include "PublicStringProcess.h"
#include "PublicStatistic.h"
#include "PublicCallJavaMethod.h"
USING_NS_CC;
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
//using namespace cocos2d::experimental;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#endif

void PublicInitGame(const std::string& gameName)
{
	PublicStatic::setString("CUR_GAME", gameName);
	PulicSetFileSearchPath(gameName);
	PublicInitGameText(gameName);
	PublicInitGameAudioSwitch(gameName);
}

std::string PublicGetGameName()
{
	return PublicStatic::getString("CUR_GAME");
}

void PulicSetFileSearchPath(const std::string& path)
{
	FileUtils::getInstance()->setSearchPaths(std::vector<std::string>());
	FileUtils::getInstance()->addSearchPath(path + "/res");
	auto lang = PublicGetLanguageString();
	FileUtils::getInstance()->addSearchPath(path + "/" + lang);
	FileUtils::getInstance()->addSearchPath(path);
	FileUtils::getInstance()->addSearchPath("Public/res");
	FileUtils::getInstance()->addSearchPath("Public");
	FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath());
}

std::string PublicGetFileString(const std::string& path)
{
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
	return FileUtils::getInstance()->getStringFromFile(fullPath);
}

bool PublicRemoveFile(const std::string& path)
{
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
	return FileUtils::getInstance()->removeFile(fullPath);
}

std::mt19937& PublicGetRandomEngine()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
}

int PublicRandInt(int min, int max)
{
	std::uniform_int_distribution<> dis(min, max);
	return dis(PublicGetRandomEngine());
}

void PublicLoadSpriteFrameData(const std::string& plistPath)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistPath);
}

void PublicReturnToStartScene()
{
	PublicHideBanner();

	PublicStatic::setString("CUR_GAME", "StartScene");
	auto scene = StartScene::createScene();
	// run
	Director::getInstance()->replaceScene(scene);
}

std::string PublicGetLanguageString()
{
	auto system_language = cocos2d::Application::getInstance()->getCurrentLanguage();
	switch (system_language) 
	{
	case cocos2d::LanguageType::ENGLISH:
		return "en";
	case cocos2d::LanguageType::CHINESE:
		return "cn";
	default:
		return "en";
	}
}

bool PublicAudioData::getEffectSwitch(const std::string& gameName)
{
	if ("" == gameName) { return m_bGlobalEffectSwitch; }
	auto iter = m_mapGameEffectSwitch.find(gameName);
	if (iter != m_mapGameEffectSwitch.end())
	{
		return iter->second;
	}
	return m_bGlobalEffectSwitch;
}

bool PublicAudioData::getMusicSwitch(const std::string& gameName)
{
	if ("" == gameName) { return m_bGlobalMusicSwitch; }
	auto iter = m_mapGameMusicSwitch.find(gameName);
	if (iter != m_mapGameMusicSwitch.end())
	{
		return iter->second;
	}
	return m_bGlobalMusicSwitch;
}

void PublicAudioData::setEffectSwitch(const std::string& gameName, bool isOpen)
{
	if ("" == gameName)
	{
		m_bGlobalEffectSwitch = isOpen;
	}
	else
	{
		m_mapGameEffectSwitch[gameName] = isOpen;
	}
}

void PublicAudioData::setMusicSwitch(const std::string& gameName, bool isOpen)
{
	if ("" == gameName)
	{
		m_bGlobalMusicSwitch = isOpen;
	}
	else
	{
		m_mapGameMusicSwitch[gameName] = isOpen;
	}
}

void PublicInitGameAudioSwitch(const std::string& gameName)
{
	auto effect = cocos2d::UserDefault::getInstance()->getBoolForKey("game_effect_switch", true);
	auto music = cocos2d::UserDefault::getInstance()->getBoolForKey("game_music_switch", true);
	PublicAudioData::getInstance()->setEffectSwitch("", effect);
	PublicAudioData::getInstance()->setMusicSwitch("", music);
	PublicAudioData::getInstance()->setEffectSwitch(gameName, effect);
	PublicAudioData::getInstance()->setMusicSwitch(gameName, music);

}

void PublicSetEffectSwitch(bool isOpen)
{
	cocos2d::UserDefault::getInstance()->setBoolForKey("game_effect_switch", isOpen);
	PublicAudioData::getInstance()->setEffectSwitch(PublicGetGameName(), isOpen);
	PublicAudioData::getInstance()->setEffectSwitch("", isOpen);
}

bool PublicGetEffectSwitch()
{
	return PublicAudioData::getInstance()->getEffectSwitch(PublicGetGameName()) &&
		PublicAudioData::getInstance()->getEffectSwitch("");
}

void PublicSetMusicSwitch(bool isOpen)
{
	cocos2d::UserDefault::getInstance()->setBoolForKey("game_music_switch", isOpen);
	PublicAudioData::getInstance()->setMusicSwitch(PublicGetGameName(), isOpen);
	PublicAudioData::getInstance()->setMusicSwitch("", isOpen);
}

bool PublicGetMusicSwitch()
{
	return PublicAudioData::getInstance()->getMusicSwitch(PublicGetGameName()) &&
		PublicAudioData::getInstance()->getMusicSwitch("");
}

void PublicPreloadEffect(const std::string& effect, std::function<void(bool isSuccess)> callback)
{
#if USE_AUDIO_ENGINE
	AudioEngine::preload(effect, callback);
#endif
}

void PublicPreloadEffect(const std::vector<std::string>& effects)
{
	for (const auto& str : effects)
	{
		PublicPreloadEffect(str);
	}
}

void PublicPlayEffect(const std::string& effect)
{
#if USE_AUDIO_ENGINE
	AudioEngine::play2d(effect, false);
#endif
}

void PublicPlayMusic(const std::string& music)
{
#if USE_AUDIO_ENGINE
	AudioEngine::play2d(music, true);
#endif
}

time_t PublicGetCurrentTime()
{
	std::time_t timestamp = std::time(nullptr);
	return timestamp;
}

std::string PublicGetCurrentTimeString(const std::string& format)
{
	return  PublicGetTimeString(std::time(nullptr), format);
}

std::string PublicGetTimeString(std::time_t timeStamp, const std::string& format)
{
	std::tm* localTime = std::localtime(&timeStamp);

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), format.c_str(), localTime);
	return std::string(buffer);
}

void PublicShowBanner()
{
	PublicCallMethodVoidVoid("showBanner");
}

void PublicHideBanner()
{
	PublicCallMethodVoidVoid("hideBanner");
}

void PublicShowInterstital()
{
	PublicCallMethodVoidVoid("showInterstitial");
}

bool PublicIsRewardVideoReady()
{
	return PublicCallMethodBoolVoid("isRewardedVideoReady");
}

void PublicPlayRewardVideo()
{
	PublicCallMethodVoidVoid("playRewardVideo");
}

void PublicStatic::setInt(const std::string& key, int val)
{
	getInstance()->m_ints[key] = val;
}

int PublicStatic::getInt(const std::string& key)
{
	auto intMap = getInstance()->m_ints;
	if (intMap.find(key) == intMap.end()) { return 0; }
	return intMap[key];
}

void PublicStatic::setString(const std::string& key, const std::string& val)
{
	getInstance()->m_strings[key] = val;
}

std::string PublicStatic::getString(const std::string& key)
{
	auto m_strings = getInstance()->m_strings;
	if (m_strings.find(key) == m_strings.end()) { return ""; }
	return m_strings[key];
}

void PublicStatic::setInt64(const std::string& key, std::int64_t val)
{
	getInstance()->m_int64[key] = val;
}

std::int64_t PublicStatic::getInt64(const std::string& key)
{
	auto intMap = getInstance()->m_int64;
	if (intMap.find(key) == intMap.end()) { return 0; }
	return intMap[key];
}