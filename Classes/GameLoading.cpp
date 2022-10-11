#include "GameLoading.h"
#include "GameMenu.h"
#include "ui/CocosGUI.h"

GameLoading::GameLoading()
{
}

GameLoading::~GameLoading()
{
}

bool GameLoading::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("Resources/Image/Background/LoadingPage.png");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	bg ->setScale(1.9);
	this->addChild(bg);

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Resources/2.plist");

	auto loadBg = Sprite::createWithSpriteFrameName("fp_floor.png");
	auto loadFore = Sprite::createWithSpriteFrameName("fp_grass.png");
	loadBg->setPosition(winSize.width / 2, 35);
	addChild(loadBg);
	
	timer = ProgressTimer::create(loadFore);
	timer->setPosition(winSize.width / 2, 60);
	timer->setType(kCCProgressTimerTypeBar);
	timer->setPercentage(0);
	timer->setMidpoint(CCPoint(0, 1));
	timer->setBarChangeRate(CCPoint(1, 0));
	
	addChild(timer);

	auto pic = Sprite::createWithSpriteFrameName("fp_title.png");
	pic->setPosition(winSize.width / 2, winSize.height - 100);
	addChild(pic);


	label = Label::createWithTTF("start", "Resources/fonts/Marker Felt.ttf", 20);
	label->setTextColor(Color4B::BLACK);
	label->setVisible(false);
	MenuItemLabel* menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(GameLoading::callback, this));
	menuItem->setPosition(winSize.width / 2, 35);
	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	tag = Sprite::createWithSpriteFrameName("fp_tag.png");
	tag->setPosition(300, 85);
	this->addChild(tag);
	auto moveTo = MoveTo::create(5.0f, Vec2(600,60));
	auto rot = RotateBy::create(5.0f, 360 * 5);
	auto scale = ScaleBy::create(5.0f, 0);
	auto rep = Spawn::create(moveTo, rot,scale,NULL);

	tag->runAction(rep);

	scheduleUpdate();

	return true;
}

void GameLoading::update(float dt)
{
	float x=tag->getPosition().x - 300;
	timer->setPercentage(x/3);
	int pro = timer->getPercentage();
	if (pro>=100)
	{
		label->setVisible(true);
	}
}

void GameLoading::callback(Ref* pSender)
{
	int pro = timer->getPercentage();
	if (pro >= 100)
	{
		label->setTextColor(Color4B::RED);
		GameMenu* gameMenu = GameMenu::create();
		TransitionCrossFade* pTrans = TransitionCrossFade::create(1.0f, gameMenu);
		Director::getInstance()->replaceScene(pTrans);
	}

}
