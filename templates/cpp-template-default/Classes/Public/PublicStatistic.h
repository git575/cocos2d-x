#pragma once
#include "PublicMacros.h"
#include <string>
#include <unordered_map>
#include <list>
#include "cocos/base/CCEventCustom.h"



class PublicGameStatisticObserver
{
public:
	virtual void onSubjectChanged(const std::string& key, int value) = 0;
	int getIndex() { return m_observe_index; }
	PublicGameStatisticObserver() :m_observe_index(0) {}
	virtual ~PublicGameStatisticObserver();
private:
	int m_observe_index;
};


// 游戏统计数据
class PublicGameStatistic
{
	SINGLETON_IMPLEMENT(PublicGameStatistic);
public:
	enum class StatisticType
	{
		INT,
		FLOAT,
		STRING
	};
	enum class StatisticUpdateType
	{
		UPDATE,
		INCREASE
	};
	class StatisticConfigData
	{
	public:
		std::string key;
		StatisticType statisticType;
		StatisticUpdateType updateType;
	};
	void init(const std::string& statisticConfig);
	void saveStatistic();
	void readStatistic();

	void addObserver(const std::string& key, PublicGameStatisticObserver* obs);
	void removeObserve(const std::string& key, int index);
	void removeObserve(const std::string& key, PublicGameStatisticObserver* obs);
	void removeObserve(PublicGameStatisticObserver* obs);

	int getStatisticInt(const std::string& key);
	float getStatisticFloat(const std::string& key);
	std::string getStatisticString(const std::string& key);
private:
	void onStatisticEvent(cocos2d::EventCustom* event);
private:
	std::unordered_map<std::string, StatisticConfigData> m_TypeMap;
	std::unordered_map<std::string, int> m_IntMap;
	std::unordered_map<std::string, float> m_FloatMap;
	std::unordered_map<std::string, std::string> m_StringMap;
	std::unordered_map <std::string, std::list<PublicGameStatisticObserver*>> m_Observers;
	bool m_bChanged;
};




// 统计上报
class PublicStatistic
{
public:
	static void CustomEvent(const std::string& eventId);
};