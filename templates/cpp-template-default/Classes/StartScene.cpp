/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "StartScene.h"
#include "Public/PublicBaseHelper.h"
#include "Public/PublicBaseGame.h"
#include "Public/PublicStartGame.h"

USING_NS_CC;

Scene* StartScene::createScene()
{
    PulicSetFileSearchPath("StartScene");

    return StartScene::create();
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto size = Director::getInstance()->getVisibleSize();
    auto btn_start_block_blast = ButtonBase::create("btn_start.png");
    btn_start_block_blast->setName("BlockBlastGame");
    btn_start_block_blast->setPosition(Vec2(size.width / 2, size.height / 2));
    addChild(btn_start_block_blast);
    btn_start_block_blast->addClickEventListener(CC_CALLBACK_1(StartScene::clickStartCallback, this));

    return true;
}

void StartScene::clickStartCallback(Ref* pSender)
{
    auto node = dynamic_cast<Node*>(pSender);
    if (!node) { return; }
    auto name = node->getName();
    if (name == "BlockBlastGame")
    {
        PublicStartGame("BLOCK_BLAST");
    }
}
