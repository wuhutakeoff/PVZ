#include "HelpLayer.h"
#include "ui/CocosGUI.h"
#include "GameMenu.h"
using namespace ui;

CHelpLayer::CHelpLayer()
{
}

CHelpLayer::~CHelpLayer()
{
}

bool CHelpLayer::init()
{
	if (!Node::init())
	{
		return false;
	}

	auto size = Director::getInstance()->getWinSize();

	auto sprite = Sprite::create("Resources/bghelp.png");
	sprite->setPosition(size.width / 2, size.height / 2);
	sprite->setScaleX(1.2);
	this->addChild(sprite);

	auto btn = Button::create("Resources/menuBtn.png");
	btn->setPosition(Vec2(size.width / 2, 40));
	btn->addClickEventListener([=](Ref*) {
		Size winSize = Director::getInstance()->getWinSize();
		auto gameMenu = GameMenu::create();
		gameMenu->setPosition(winSize.width / 2, winSize.height / 2);

		TransitionFade* pFade = TransitionFade::create(1.0, gameMenu);
		Director::getInstance()->replaceScene(pFade);
		});
	this->addChild(btn);

	return true;
}
