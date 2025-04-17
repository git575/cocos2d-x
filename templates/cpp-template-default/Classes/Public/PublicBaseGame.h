#pragma once
#include "cocos2d.h"

// 添加资源搜索路径
void PulicSetFileSearchPath(const std::string& path);

// 读取文件
std::string PublicGetFileString(const std::string& path);
// 删除文件
bool PublicRemoveFile(const std::string& path);

// 随机数[] 
int PublicRandInt(int min, int max);

// 滚动数字
// TODO: 非线性增长
class PublicNumberScrollingData {
public:
	PublicNumberScrollingData() {}
	~PublicNumberScrollingData();
	static void start(const std::shared_ptr<cocos2d::Label>& ptr, float from_num, float to_num, float total_time = 1.f, float update_time = 0.2f, bool bClear = false);
	static void scheduleUpdate(float dt);
private:
	PublicNumberScrollingData(const std::shared_ptr<cocos2d::Label>& _ptr, float _from_num, float _to_num, float _total_time, float _update_time);
	std::shared_ptr<cocos2d::Label> ptr;
	float from_num;
	float to_num;
	float cur_num;
	float delta_num;
	float total_time;
	float update_time; // deprecated
	float cur_time;
	
private:
	static std::unordered_map<cocos2d::Label*, PublicNumberScrollingData> scheduleMap;
};

// 读取plist文件并加载纹理的函数
void PublicLoadSpriteFrameData(const std::string& plistPath);