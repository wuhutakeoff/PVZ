#include "UserMenu.h"
#include "Tool.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
USING_NS_CC;
using namespace rapidjson;
using namespace std;

UserMenu::UserMenu()
{
}

UserMenu::~UserMenu()
{
}

bool UserMenu::init()
{
	if (!Node::init())
	{
		return false;
	}

	this->addTouch();

	auto bg = Sprite::create("Resources/userBg.png");
	bg->setPosition(Vec2::ZERO);
	this->addChild(bg);

	auto btn1 = addItem("Resources/userBtn.png", "Resources/userBtn2.png", "重命名",Vec2(100,85), [=](Ref* pSender)
		{

		});
	bg->addChild(btn1);


	auto btn2 = addItem("Resources/userBtn.png", "Resources/userBtn2.png", "删除", Vec2(300, 85), [=](Ref* pSender)
		{

		});
	bg->addChild(btn2);


	auto btn3 = addItem("Resources/userBtn.png", "Resources/userBtn2.png", "确定", Vec2(100, 35), [=](Ref* pSender)
		{

		});
	bg->addChild(btn3);

	auto btn4 = addItem("Resources/userBtn.png", "Resources/userBtn2.png", "取消", Vec2(300, 35), [=](Ref* pSender)
		{
			_eventDispatcher->removeEventListenersForTarget(this);
			this->removeFromParent();
		});
	bg->addChild(btn4);

	return true;
}

Button* UserMenu::addItem(std::string normalImage, std::string selectedImage, std::string btnName, Vec2 pos, std::function<void(Ref*)> callback)
{
	auto btn = Button::create(normalImage, selectedImage);
	btn->setPosition(pos);
	btn->setTitleText(CTool::FontToUTF8(btnName.c_str()));
	btn->setTitleFontSize(20);
	btn->addClickEventListener(callback);
	return btn;
}

void UserMenu::addTouch()
{
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent) {
		return true;
	};
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}
