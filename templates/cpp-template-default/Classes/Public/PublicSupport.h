#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <list>

class PublicSubject;
class PublicObserver
{
public:
	virtual void onSubjectChanged(int value) = 0;
	int getIndex() { return m_observe_index; }
	void addSubject(PublicSubject* sub) { m_sunjects.push_back(sub); }
	PublicObserver() :m_observe_index(0) {}
	virtual ~PublicObserver();
private:
	int m_observe_index;
	std::vector<PublicSubject*> m_sunjects;
};

class PublicSubject
{
public:
	void addObserver(const std::string& key, PublicObserver* obs);
	void removeObserve(const std::string& key, int index);
	void removeObserve(const std::string& key, PublicObserver* obs);
	void removeObserve(PublicObserver* obs);
protected:
	void emitChange(const std::string& key, int val);
private:
	std::unordered_map <std::string, std::list<PublicObserver*>> m_Observers;
};