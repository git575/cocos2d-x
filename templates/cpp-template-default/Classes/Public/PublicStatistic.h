#pragma once
#include "PublicMacros.h"
#include <string>
#include <unordered_map>
#include <list>
#include "cocos/base/CCEventCustom.h"

class PublicStatisticObserver
{
public:
	virtual void onSubjectChanged(const std::string& key, int value) = 0;
	int getIndex() { return m_observe_index; }
	PublicStatisticObserver() :m_observe_index(0) {}
	virtual ~PublicStatisticObserver();
private:
	int m_observe_index;
};

class PublicStatistic
{
	SINGLETON_IMPLEMENT(PublicStatistic);
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

	void addObserver(const std::string& key, PublicStatisticObserver* obs);
	void removeObserve(const std::string& key, int index);
	void removeObserve(const std::string& key, PublicStatisticObserver* obs);
	void removeObserve(PublicStatisticObserver* obs);

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
	std::unordered_map <std::string, std::list<PublicStatisticObserver*>> m_Observers;
	bool m_bChanged;
};