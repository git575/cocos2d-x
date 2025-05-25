#include "PublicStatistic.h"
#include "PublicBaseHelper.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/allocators.h"
#include "json/writer.h"
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#endif
PublicGameStatisticObserver::~PublicGameStatisticObserver()
{
	PublicGameStatistic::getInstance()->removeObserve(this);
}


void PublicGameStatistic::init(const std::string& statisticConfig)
{
	m_bChanged = false;
	auto textFileString = PublicGetFileString(statisticConfig);
	auto doc = rapidjson::Document();
	doc.Parse(textFileString.c_str());
	if (doc.HasParseError() || doc.IsObject())
	{
		return;
	}

	m_TypeMap.clear();
	m_IntMap.clear();
	m_FloatMap.clear();
	m_StringMap.clear();
	for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
	{
		auto& item = doc[i];
		if (item.HasMember("statistic_key") && item["statistic_key"].IsString() 
			&& item.HasMember("value_type") && item["value_type"].IsString()
			&& item.HasMember("value_update_type") && item["value_update_type"].IsString()
			)
		{
			StatisticConfigData data;
			std::string key = item["statistic_key"].GetString();
			std::string type = item["value_type"].GetString();
			std::string update = item["value_update_type"].GetString();
			const std::unordered_map<std::string, StatisticType> str_2_type = {
				{"int", StatisticType::INT},
				{"float", StatisticType::FLOAT},
				{"string", StatisticType::STRING}
			};
			const std::unordered_map<std::string, StatisticUpdateType> str_2_update = {
				{"update", StatisticUpdateType::UPDATE},
				{"increase", StatisticUpdateType::INCREASE}
			};
			if (str_2_type.find(type) == str_2_type.end() || str_2_update.find(update) == str_2_update.end())
			{
				continue;
			}
			data.key = key;
			data.statisticType = str_2_type.at(type);
			data.updateType = str_2_update.at(update);
			m_TypeMap[key] = data;
		}
	}
	
	for (const auto& item : m_TypeMap)
	{
		cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(item.first, CC_CALLBACK_1(PublicGameStatistic::onStatisticEvent, this));
	}
	if (!cocos2d::Director::getInstance()->getScheduler()->isScheduled("PublicGameStatisticSchedule", cocos2d::Director::getInstance())) {

		cocos2d::Director::getInstance()->getScheduler()->schedule([=](float dt) {
			this->saveStatistic();
			}, cocos2d::Director::getInstance(), 0.5f, false, "PublicGameStatisticSchedule");
	}

	for (const auto& item : m_TypeMap)
	{
		switch (item.second.statisticType)
		{
		case StatisticType::INT:
			m_IntMap[item.first] = 0;
			break;
		case StatisticType::FLOAT:
			m_FloatMap[item.first] = 0;
			break;
		case StatisticType::STRING:
			m_StringMap[item.first] = "";
			break;
		default:
			break;
		}
	}
	readStatistic();
}

void PublicGameStatistic::saveStatistic()
{
	if (!m_bChanged) { return; }
	std::string statisticFile = PublicGetGameName() + "_StatisticSaveData.json";
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	for (const auto& item : m_IntMap)
	{
		doc.AddMember(rapidjson::Value().SetString(item.first.c_str(), allocator), item.second, allocator);
	}
	for (const auto& item : m_FloatMap)
	{
		doc.AddMember(rapidjson::Value().SetString(item.first.c_str(), allocator), item.second, allocator);
	}
	for (const auto& item : m_StringMap)
	{
		doc.AddMember(rapidjson::Value().SetString(item.first.c_str(), allocator), 
			rapidjson::Value().SetString(item.second.c_str(), allocator), allocator);
	}
	// 将JSON写入文件
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + statisticFile;
	cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), filePath);

	m_bChanged = false;
	
}

void PublicGameStatistic::readStatistic()
{
	std::string statisticFile = PublicGetGameName() + "_StatisticSaveData.json";
	std::string dataString = PublicGetFileString(statisticFile);
	rapidjson::Document doc;
	doc.Parse(dataString.c_str());
	if (doc.HasParseError())
	{
		return;
	}
	for (const auto& item : m_TypeMap)
	{
		if (doc.HasMember(item.first.c_str()))
		{
			if (item.second.statisticType == StatisticType::INT && doc[item.first.c_str()].IsInt())
			{
				m_IntMap[item.first] = doc[item.first.c_str()].GetInt();
			}
			else if (item.second.statisticType == StatisticType::FLOAT && doc[item.first.c_str()].IsFloat())
			{
				m_FloatMap[item.first] = doc[item.first.c_str()].GetFloat();
			}
			else if (item.second.statisticType == StatisticType::STRING && doc[item.first.c_str()].IsString())
			{
				m_StringMap[item.first] = doc[item.first.c_str()].GetString();
			}
		}
	}
}

void PublicGameStatistic::addObserver(const std::string& key, PublicGameStatisticObserver* obs)
{
	m_Observers[key].push_back(obs);
}

void PublicGameStatistic::removeObserve(const std::string& key, int index)
{
	if (m_Observers.find(key) == m_Observers.end()) { return; }
	auto iter = m_Observers[key].begin();
	while (iter != m_Observers[key].end())
	{
		if ((*iter)->getIndex() == index)
		{
			iter = m_Observers[key].erase(iter);
			continue;
		}
		++iter;
	}
}

void PublicGameStatistic::removeObserve(const std::string& key, PublicGameStatisticObserver* obs)
{
	if (m_Observers.find(key) == m_Observers.end()) { return; }
	auto iter = m_Observers[key].begin();
	while (iter != m_Observers[key].end())
	{
		if ((*iter) == obs)
		{
			iter = m_Observers[key].erase(iter);
			continue;
		}
		++iter;
	}
}

void PublicGameStatistic::removeObserve(PublicGameStatisticObserver* obs)
{
	for (auto iter = m_Observers.begin(); iter != m_Observers.end(); ++iter)
	{
		auto iterItem = (*iter).second.begin();
		while (iterItem != (*iter).second.end())
		{
			if ((*iterItem) == obs)
			{
				iterItem = (*iter).second.erase(iterItem);
			}
			else
			{
				++iterItem;
			}
		}
	}
}

int PublicGameStatistic::getStatisticInt(const std::string& key)
{
	if (m_IntMap.find(key) == m_IntMap.end()) { return 0; }
	return m_IntMap[key];
}

float PublicGameStatistic::getStatisticFloat(const std::string& key)
{
	if (m_FloatMap.find(key) == m_FloatMap.end()) { return 0.f; }
	return m_FloatMap[key];
}

std::string PublicGameStatistic::getStatisticString(const std::string& key)
{
	if (m_StringMap.find(key) == m_StringMap.end()) { return ""; }
	return m_StringMap[key];
}

void PublicGameStatistic::onStatisticEvent(cocos2d::EventCustom* event)
{
	auto eventKey = event->getEventName();
	auto type = m_TypeMap.find(eventKey);
	if (type == m_TypeMap.end())
	{
		return;
	}

	if (type->second.statisticType == StatisticType::INT)
	{
		auto data = static_cast<int*>(event->getUserData());
		if (type->second.updateType == StatisticUpdateType::UPDATE)
		{
			if (!data) { return; }
			m_IntMap[eventKey] = m_IntMap[eventKey] > *data ? m_IntMap[eventKey] : *data;
			m_bChanged = true;
		}
		else if (type->second.updateType == StatisticUpdateType::INCREASE)
		{
			m_IntMap[eventKey] += data ? *data : 1;
			m_bChanged = true;
		}
		for (const auto& lst: m_Observers)
		{
			if (lst.first == eventKey)
			{
				auto copy = lst;
				for (const auto& ptr : copy.second)
				{
					ptr->onSubjectChanged(eventKey, m_IntMap[eventKey]);
				}
			}
		}
	}
	else if (type->second.statisticType == StatisticType::FLOAT)
	{
		auto data = static_cast<float*>(event->getUserData());
		if (type->second.updateType == StatisticUpdateType::UPDATE)
		{
			if (!data) { return; }
			m_FloatMap[eventKey] = m_FloatMap[eventKey] > *data ? m_FloatMap[eventKey] : *data;
			m_bChanged = true;
		}
		else if (type->second.updateType == StatisticUpdateType::INCREASE)
		{
			m_FloatMap[eventKey] += data ? *data : 0.f;
			m_bChanged = true;
		}
	}
	else if (type->second.statisticType == StatisticType::STRING)
	{
		auto data = static_cast<std::string*>(event->getUserData());
		if (!data) { return; }
		m_bChanged = true;
		if (type->second.updateType == StatisticUpdateType::UPDATE)
		{
			m_StringMap[eventKey] = *data;
		}
		else if (type->second.updateType == StatisticUpdateType::INCREASE)
		{
			m_StringMap[eventKey] += *data;
		}
	}
	
}




void PublicStatistic::CustomEvent(const std::string& eventId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CCLOG("EventCustom:%s", eventId.c_str());
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo,
		"org/cocos2dx/cpp/PublicStatistics",  // Java 类路径
		"logEvent",
		"(Ljava/lang/String;)V")) {

        jstring jEventId = methodInfo.env->NewStringUTF(eventId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jEventId);
        methodInfo.env->DeleteLocalRef(jEventId);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
#endif
}