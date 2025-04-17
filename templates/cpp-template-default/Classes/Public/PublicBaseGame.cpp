#include "PublicBaseGame.h"
USING_NS_CC;
#include<fstream>
void PulicSetFileSearchPath(const std::string& path)
{
	//return;
	FileUtils::getInstance()->setSearchPaths(std::vector<std::string>());
	FileUtils::getInstance()->addSearchPath(path, true);
	FileUtils::getInstance()->addSearchPath(path + "/cn", true);
	FileUtils::getInstance()->addSearchPath(path + "/res", true);
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

int PublicRandInt(int min, int max)
{
	static std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

PublicNumberScrollingData::PublicNumberScrollingData(const std::shared_ptr<Label>& _ptr, float _from_num, float _to_num, float _total_time, float _update_time):
	ptr(_ptr),
	from_num(_from_num),
	to_num(_to_num),
	cur_num(_from_num),
	delta_num(0.f),
	total_time(_total_time),
	update_time(_update_time),
	cur_time(0.f)
{
	delta_num = (to_num - cur_num) / total_time;
}

PublicNumberScrollingData::~PublicNumberScrollingData()
{

}

std::unordered_map<Label*, PublicNumberScrollingData> PublicNumberScrollingData::scheduleMap;
float epsilon = 1e-5;
void PublicNumberScrollingData::start(const std::shared_ptr<cocos2d::Label>& ptr, float from_num, float to_num, float total_time, float update_time, bool bClear)
{
	if (total_time < epsilon && total_time > -epsilon) { return; }
	if (bClear)
	{
		auto iter = scheduleMap.find(ptr.get());
		if (iter != scheduleMap.end())
		{
			scheduleMap.erase(iter);
		}
	}
	if (scheduleMap.find(ptr.get()) != scheduleMap.end())
	{
		auto& data = scheduleMap[ptr.get()];
		data.total_time = total_time;
		data.to_num = to_num;
		data.update_time = update_time;
		data.delta_num = (data.to_num - data.cur_num) / data.total_time;
		return;
	}

	scheduleMap[ptr.get()] = PublicNumberScrollingData(ptr, from_num, to_num, total_time, update_time);
	if (!Director::getInstance()->getScheduler()->isScheduled("PublicNumberScrollingSchedule", Director::getInstance()))
	{
		Director::getInstance()->getScheduler()->schedule([](float dt) {
			PublicNumberScrollingData::scheduleUpdate(dt);
			}, cocos2d::Director::getInstance(), 1 / 60.f, false, "PublicNumberScrollingSchedule");
	}
}

void PublicNumberScrollingData::scheduleUpdate(float dt)
{
	if (scheduleMap.empty())
	{
		Director::getInstance()->getScheduler()->unschedule("PublicNumberScrollingSchedule", Director::getInstance());
		return;
	}
	std::vector<Label*> eraseIters;
	for (auto& key_val : scheduleMap)
	{
		auto& data = key_val.second;
		if (data.ptr.use_count() == 1)
		{
			eraseIters.emplace_back(data.ptr.get());
			continue;
		}
		data.cur_time += dt;
		if (data.cur_time < data.total_time)
		{
			data.cur_num = data.from_num + (data.to_num - data.from_num) * data.cur_time / data.total_time;
		}
		else
		{
			data.cur_num = data.to_num;
			eraseIters.emplace_back(data.ptr.get());
		}
		data.ptr.get()->setString(std::to_string(int(data.cur_num)));
	}
	for (auto key : eraseIters)
	{
		scheduleMap.erase(key);
	}
	return;
}

void PublicLoadSpriteFrameData(const std::string& plistPath)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistPath);
}