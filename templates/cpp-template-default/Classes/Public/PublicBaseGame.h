#pragma once
#include "cocos2d.h"
#include "ui/UIButton.h"

// 滚动数字
// TODO: 非线性增长
class PublicNumberScrollingData {
public:
	PublicNumberScrollingData():
		ptr(nullptr),
		from_num(0.f),
		to_num(0.f),
		total_time(1.f),
		delay_time(0.f),
		cur_time(0.f),
		cur_num(0.f),
		delta_num(0.f)
	{}
	~PublicNumberScrollingData();
	static void start(cocos2d::Label* ptr, float from_num, float to_num, float total_time = 1.f, float delay_time = 0.f, bool bClear = false);
	static void scheduleUpdate(float dt);
private:
	PublicNumberScrollingData(cocos2d::Label* _ptr, float _from_num, float _to_num, float _total_time, float _delay_time);
	//std::shared_ptr<cocos2d::Label> ptr;
	cocos2d::Label* ptr;
	float from_num;
	float to_num;
	float cur_num;
	float delta_num;
	float total_time;
	float delay_time;
	float cur_time;
	
private:
	static std::unordered_map<cocos2d::Label*, PublicNumberScrollingData> scheduleMap;
	static void insertToScheduleMap(cocos2d::Label* ptr, float from_num, float to_num, float total_time, float delay_time);
	static void eraseFromScheduleMap(cocos2d::Label* ptr);
};

// 给reference_node添加SwallowTouches
void PublicCreateLayout(cocos2d::Node* reference_node);

// 按钮   点击时带动子节点(tag == 1)一起缩放   带音效
class ButtonBase : public cocos2d::ui::Button
{
public:
	ButtonBase();
	static ButtonBase* create();

	static ButtonBase* create(const std::string& normalImage,
		const std::string& selectedImage = "",
		const std::string& disableImage = "",
		TextureResType texType = TextureResType::LOCAL);
	void setClickEffectSound(const  std::string& effect) { m_sClickEffect = effect; }
protected:
	virtual void onPressStateChangedToNormal() override;
	virtual void onPressStateChangedToPressed() override;
	virtual void onPressStateChangedToDisabled() override;

	virtual void releaseUpEvent()override;
private:
	std::string m_sClickEffect;
};

// TODO:增加显示界面和隐藏界面，增加缓存
void PublicShowLayer(const std::string& name, cocos2d::Layer* layer);
void PublicHideLayer(const std::string& name);