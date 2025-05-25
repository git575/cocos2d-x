#include "PublicSupport.h"

PublicObserver::~PublicObserver()
{
	for (auto iter = m_sunjects.begin(); iter != m_sunjects.end(); ++iter)
	{
		(*iter)->removeObserve(this);
	}
}

void PublicSubject::addObserver(const std::string& key, PublicObserver* obs)
{
	m_Observers[key].push_back(obs);
	obs->addSubject(this);
}

void PublicSubject::removeObserve(const std::string& key, int index)
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

void PublicSubject::removeObserve(const std::string& key, PublicObserver* obs)
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

void PublicSubject::removeObserve(PublicObserver* obs)
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

void PublicSubject::emitChange(const std::string& key, int val)
{
	if (m_Observers.find(key) == m_Observers.end()) { return; }
	auto iter = m_Observers[key].begin();
	auto end = m_Observers[key].end();
	while (iter != end)
	{
		(*iter)->onSubjectChanged(val);
		++iter;
	}
}
