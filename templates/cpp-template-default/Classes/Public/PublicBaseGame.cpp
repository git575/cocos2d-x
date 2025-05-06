#include "PublicBaseGame.h"
USING_NS_CC;
#include<fstream>
#include "StartScene.h"
#include "ui/CocosGUI.h"
#include "PublicBaseHelper.h"

PublicNumberScrollingData::PublicNumberScrollingData(cocos2d::Label* _ptr, float _from_num, float _to_num, float _total_time, float _delay_time):
	ptr(_ptr),
	from_num(_from_num),
	to_num(_to_num),
	cur_num(_from_num),
	delta_num(0.f),
	total_time(_total_time),
    delay_time(_delay_time),
	cur_time(0.f)
{
	delta_num = (to_num - cur_num) / total_time;
}

PublicNumberScrollingData::~PublicNumberScrollingData()
{

}

std::unordered_map<Label*, PublicNumberScrollingData> PublicNumberScrollingData::scheduleMap;
void PublicNumberScrollingData::insertToScheduleMap(cocos2d::Label* ptr, float from_num, float to_num, float total_time, float delay_time)
{
	ptr->retain();
	scheduleMap[ptr] = PublicNumberScrollingData(ptr, from_num, to_num, total_time, delay_time);
}

void PublicNumberScrollingData::eraseFromScheduleMap(cocos2d::Label* ptr)
{
	if (!ptr) { return; }
	auto iter = scheduleMap.find(ptr);
	if (iter != scheduleMap.end())
	{
		scheduleMap.erase(iter);
		ptr->release();
	}
}

float epsilon = 1e-5;
void PublicNumberScrollingData::start(cocos2d::Label* ptr, float from_num, float to_num, float total_time, float delay_time, bool bClear)
{
	if (total_time < epsilon && total_time > -epsilon) { return; }
	if (bClear)
	{
		eraseFromScheduleMap(ptr);
	}
	if (scheduleMap.find(ptr) != scheduleMap.end())
	{
		auto& data = scheduleMap[ptr];
		data.total_time = total_time;
		data.to_num = to_num;
		data.delay_time = delay_time;
		data.delta_num = (data.to_num - data.cur_num) / data.total_time;
		return;
	}

	insertToScheduleMap(ptr, from_num, to_num, total_time, delay_time);
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
		
		if (data.ptr->getReferenceCount() == 1)
		{
			eraseIters.emplace_back(data.ptr);
			continue;
		}
		if (data.delay_time > 0.f)
		{
			data.delay_time -= dt;
            if (data.delay_time > 0.f) { continue; }
            dt -= data.delay_time;
            data.delay_time = 0.f;
            
		}
		data.cur_time += dt;
		if (data.cur_time < data.total_time)
		{
			data.cur_num = data.from_num + (data.to_num - data.from_num) * data.cur_time / data.total_time;
		}
		else
		{
			data.cur_num = data.to_num;
			eraseIters.emplace_back(data.ptr);
		}
		data.ptr->setString(std::to_string(int(data.cur_num)));
	}
	for (auto key : eraseIters)
	{
		eraseFromScheduleMap(key);
	}
	return;
}

void PublicCreateLayout(cocos2d::Node* reference_node)
{
	if (!reference_node || !reference_node->getParent()) { return; }
	auto layout = ui::Layout::create();
	layout->setContentSize(reference_node->getContentSize());
	layout->setTouchEnabled(true);
	layout->setSwallowTouches(true);
	layout->setAnchorPoint(Vec2(0.5f, 0.5f));
	layout->setPosition(reference_node->getPosition());
	reference_node->getParent()->addChild(layout, reference_node->getLocalZOrder() - 1);
}





ButtonBase::ButtonBase():
    cocos2d::ui::Button(),
    m_sClickEffect("")
{ }

ButtonBase* ButtonBase::create()
{
    ButtonBase* widget = new (std::nothrow) ButtonBase();
    if (widget && widget->init())
    {
        widget->setZoomScale(0.03f);
        widget->setCascadeOpacityEnabled(true);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

ButtonBase* ButtonBase::create(const std::string& normalImage,
    const std::string& selectedImage,
    const std::string& disableImage,
    TextureResType texType)
{
    ButtonBase* btn = new (std::nothrow) ButtonBase;
    if (btn && btn->init(normalImage, selectedImage, disableImage, texType))
    {
        btn->setZoomScale(0.03f);
        btn->setCascadeOpacityEnabled(true);
        btn->autorelease();
        return btn;
    }
    CC_SAFE_DELETE(btn);
    return nullptr;
}

void ButtonBase::onPressStateChangedToNormal()
{
    _buttonNormalRenderer->setVisible(true);
    _buttonClickedRenderer->setVisible(false);
    _buttonDisabledRenderer->setVisible(false);
    _buttonNormalRenderer->setState(ui::Scale9Sprite::State::NORMAL);

    if (_pressedTextureLoaded)
    {
        if (_pressedActionEnabled)
        {
            // change children scale
            auto children = getChildren();
            for (auto child : children)
            {
                if (child->getTag() != 1) { continue; }
                child->setScale(1.f);
            }
            _buttonNormalRenderer->stopAllActions();
            _buttonClickedRenderer->stopAllActions();

            // Action *zoomAction = ScaleTo::create(ZOOM_ACTION_TIME_STEP, _normalTextureScaleXInSize, _normalTextureScaleYInSize);
            // fixme: the zoomAction will run in the next frame which will cause the _buttonNormalRenderer to a wrong scale
            _buttonNormalRenderer->setScale(1.0);
            _buttonClickedRenderer->setScale(1.0);

            if (nullptr != _titleRenderer)
            {
                _titleRenderer->stopAllActions();
                if (_unifySize)
                {
                    Action* zoomTitleAction = ScaleTo::create(0.05f, 1.0f, 1.0f);
                    _titleRenderer->runAction(zoomTitleAction);
                }
                else
                {
                    _titleRenderer->setScaleX(1.0f);
                    _titleRenderer->setScaleY(1.0f);
                }
            }
        }
    }
    else
    {
        // change children scale
        auto children = getChildren();
        for (auto child : children)
        {
            if (child->getTag() != 1) { continue; }
            child->setScale(1.f);
        }

        _buttonNormalRenderer->stopAllActions();
        _buttonNormalRenderer->setScale(1.0);

        if (nullptr != _titleRenderer)
        {
            _titleRenderer->stopAllActions();
            _titleRenderer->setScaleX(1.0f);
            _titleRenderer->setScaleY(1.0f);
        }

    }
}

void ButtonBase::onPressStateChangedToPressed()
{
    _buttonNormalRenderer->setState(ui::Scale9Sprite::State::NORMAL);

    if (_pressedTextureLoaded)
    {
        _buttonNormalRenderer->setVisible(false);
        _buttonClickedRenderer->setVisible(true);
        _buttonDisabledRenderer->setVisible(false);

        if (_pressedActionEnabled)
        {
            // change children scale
            auto children = getChildren();
            for (auto child : children)
            {
                if (child->getTag() != 1) { continue; }
                child->setScale(1.0f + _zoomScale, 1.0f + _zoomScale);
            }

            _buttonNormalRenderer->stopAllActions();
            _buttonClickedRenderer->stopAllActions();

            Action* zoomAction = ScaleTo::create(0.05f,
                1.0f + _zoomScale,
                1.0f + _zoomScale);
            _buttonClickedRenderer->runAction(zoomAction);

            _buttonNormalRenderer->setScale(1.0f + _zoomScale,
                1.0f + _zoomScale);

            if (nullptr != _titleRenderer)
            {
                _titleRenderer->stopAllActions();
                Action* zoomTitleAction = ScaleTo::create(0.05f,
                    1.0f + _zoomScale, 1.0f + _zoomScale);
                _titleRenderer->runAction(zoomTitleAction);
            }
        }
    }
    else
    {
        // change children scale
        auto children = getChildren();
        for (auto child : children)
        {
            if (child->getTag() != 1) { continue; }
            child->setScale(1.0f + _zoomScale, 1.0f + _zoomScale);
        }

        _buttonNormalRenderer->setVisible(true);
        _buttonClickedRenderer->setVisible(true);
        _buttonDisabledRenderer->setVisible(false);

        _buttonNormalRenderer->stopAllActions();
        _buttonNormalRenderer->setScale(1.0f + _zoomScale, 1.0f + _zoomScale);

        if (nullptr != _titleRenderer)
        {
            _titleRenderer->stopAllActions();
            _titleRenderer->setScaleX(1.0f + _zoomScale);
            _titleRenderer->setScaleY(1.0f + _zoomScale);
        }
    }
}

void ButtonBase::onPressStateChangedToDisabled()
{
    //if disable resource is null
    if (!_disabledTextureLoaded)
    {
        if (_normalTextureLoaded)
        {
            _buttonNormalRenderer->setState(ui::Scale9Sprite::State::GRAY);
        }
    }
    else
    {
        _buttonNormalRenderer->setVisible(false);
        _buttonDisabledRenderer->setVisible(true);
    }

    // change children scale
    auto children = getChildren();
    for (auto child : children)
    {
        if (child->getTag() != 1) { continue; }
        child->setScale(1.f);
    }

    _buttonClickedRenderer->setVisible(false);
    _buttonNormalRenderer->setScale(1.0);
    _buttonClickedRenderer->setScale(1.0);
}

void ButtonBase::releaseUpEvent()
{
    auto play_effect = PublicStatic::getString("PUBLIC_SOUND_EFFECT");
    if (m_sClickEffect != "")
    {
        play_effect = m_sClickEffect;
    }
    if (play_effect != "")
    {
        PublicPlayEffect(play_effect);
    }
    cocos2d::ui::Button::releaseUpEvent();
}

void PublicShowLayer(const std::string& name, cocos2d::Layer* layer)
{
    if (!layer || layer->getParent()) { return; }
    Director::getInstance()->getRunningScene()->addChild(layer, 99);
    if (name != "")
    {
        layer->setName(name);
    }
}