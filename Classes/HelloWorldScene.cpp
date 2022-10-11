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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <string>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    m_pLabel = Label::createWithTTF("Hello World", "fonts/simkai.ttf", 24);
    if (m_pLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        m_pLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - m_pLabel->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(m_pLabel, 1);
    }

    // add "HelloWorld" splash screen"
    // 
    auto spritecache = SpriteFrameCache::getInstance();
    //spritecache->addSpriteFramesWithFile("WallNut.plist");
    //SpriteFrame* spriteFrame=spritecache->getSpriteFrameByName("person.png");

    m_pSprite = Sprite::create("star.png");
    if (m_pSprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        m_pSprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(m_pSprite, 0);
    }

    auto sprite = Sprite::create("pool.jpg");
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(sprite, -1);

    auto zombie = Sprite::create("Zombie_0.png");
    zombie->setPosition(Vec2(1000,300));
    this->addChild(zombie, 0);
    auto pMoveTo = MoveTo::create(15.0f, Vec2(500,300));
    auto pRep = RepeatForever::create(pMoveTo);
    zombie->runAction(pRep);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Zombie.plist");
    auto cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> images0;
    for (int i = 0; i < 21; i++)
    {
        std::string str = "Zombie_" + std::to_string(i) + ".png";
        images0.pushBack(cache->getSpriteFrameByName(str));
    }
    Animation* animation1= Animation::createWithSpriteFrames(images0, 1.f/8 );
    zombie->runAction(RepeatForever::create(Animate::create(animation1)));

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SunFlower.plist");
    auto SunFlowerSprite = Sprite::createWithSpriteFrameName("SunFlower_0.png");
    SunFlowerSprite->setPosition(200, 200);
    this->addChild(SunFlowerSprite, 0);

    cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> images;
    for (int i = 0; i < 18; i++)
    {
        std::string str = "SunFlower_" + std::to_string(i) + ".png";
        images.pushBack(cache->getSpriteFrameByName(str));
    }
    Animation* animation = Animation::createWithSpriteFrames(images, 1.f / 8);
    SunFlowerSprite->runAction(RepeatForever::create(Animate::create(animation)));



    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    MoveTo* pMoveTo = MoveTo::create(3.0f, Vec2(100, 100));
    RotateBy* pRota = RotateBy::create(3.0f, -1080);
    Spawn* pSpawn = Spawn::create(pMoveTo, pRota, nullptr);
    JumpBy* pJump = JumpBy::create(2.0f, Vec2::ZERO, 100, 10);

    //CallFunc:可以把函数封装成动作，用runAction执行。
    CallFunc* pCall = CallFunc::create([=]() {
        m_pLabel->setString(this->FontToUTF8("跳跃结束"));
        });
    //pJump动作执行完要执行这个函数。
    DelayTime* pDalay1 = DelayTime::create(1.0f);

    CallFunc* pCall1 = CallFunc::create([=]() {
        m_pLabel->setString(this->FontToUTF8("准备上下浮动"));
     });
    DelayTime* pDalay2 = DelayTime::create(1.0f);
    
    //MoveBy* pMoveBack = MoveBy::create(1.0f, Vec2(0, -100));

    CallFunc* pCall2 = CallFunc::create([=]() {
        MoveBy* pMoveBy = MoveBy::create(0.5f, Vec2(0, 100));
        MoveBy* pMoveBack = pMoveBy->reverse();
        Sequence* pMove = Sequence::createWithTwoActions(pMoveBy, pMoveBack);
        //重复动作。
        //Repeat:要重复执行的动作，执行多少次2~2的30次方
        //Repeat* pRep = Repeat::create(pMove, 10);

        //永久执行某个动作，参数是动作。
        RepeatForever* pRep = RepeatForever::create(pMove);
        m_pSprite->runAction(pRep);
     });

    Sequence* pSeq = Sequence::create(pSpawn, pJump, pCall, pDalay1,pCall1,pDalay2, pCall2,  nullptr);
    m_pSprite->runAction(pSeq);


    //Close the cocos2d-x game scene and quit the application
    //Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/
    EventCustom customEndEvent("game_scene_close_event");
    _eventDispatcher->dispatchEvent(&customEndEvent);

}

char* HelloWorld::FontToUTF8(const char* font)
{
    int len = MultiByteToWideChar(CP_ACP, 0, font, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, font, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr)delete[] wstr;
    return str;
}