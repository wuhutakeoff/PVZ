#include "GameMenu.h"
#include "ui/CocosGUI.h"
#include "GameLevel.h"
#include "Tool.h"
#include "OptionLayer.h"
#include "SimpleAudioEngine.h"
#include "UserMenu.h"
#include "LevelSelect.h"
#include "HelpLayer.h"
using namespace CocosDenshion;
using namespace ui;

GameMenu::GameMenu()
{
}

GameMenu::~GameMenu()
{
}

bool GameMenu::init()
{
	if (!Scene::init())
	{
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.MP3",true);
	int volume = UserDefault::getInstance()->getIntegerForKey("volumeBg",100);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
	bool bOn = UserDefault::getInstance()->getBoolForKey("bgMusic",true);
	if (!bOn)
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	auto winSize = Director::getInstance()->getWinSize();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/1.plist");

	auto pBg = Sprite::createWithSpriteFrameName("main_background.png");
	pBg->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(pBg);

	//冒险模式菜单
	auto menuSprite = GameMenu::addSpriteMenu("maoxian", "select10.png", "select11.png", 2, Vec2(570, 440));
	
	//迷你游戏菜单
	auto menuSprite1 = GameMenu::addSpriteMenu("mini", "select20.png", "select21.png", 1.9, Vec2(570, 340));

	//益智游戏菜单
	auto menuSprite2 = GameMenu::addSpriteMenu("yizhi", "select30.png", "select31.png", 1.7, Vec2(570, 260));

	//生存游戏菜单
	auto menuSprite3 = GameMenu::addSpriteMenu("shengcun", "select40.png", "select41.png", 1.7, Vec2(570,190));

	auto menu = Menu::create(menuSprite,menuSprite1,menuSprite2,menuSprite3,NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);


	//选项按钮
	auto labelOption = Label::createWithTTF(CTool::FontToUTF8("设置"), "fonts/simkai.ttf", 30);
	labelOption->setName("option");
	labelOption->setTextColor(Color4B::BLACK);
	auto menuItem = MenuItemLabel::create(labelOption, CC_CALLBACK_1(GameMenu::callback, this));
	menuItem->setPosition(680, 100);

	//帮助按钮
	auto labelHelp = Label::createWithTTF(CTool::FontToUTF8("帮助"), "fonts/simkai.ttf", 30);
	labelHelp->setName("help");
	labelHelp->setTextColor(Color4B::BLACK);
	auto menuItem1 = MenuItemLabel::create(labelHelp, CC_CALLBACK_1(GameMenu::callback, this));
	menuItem1->setPosition(756, 72);

	//退出按钮
	auto labelExit= Label::createWithTTF(CTool::FontToUTF8("退出"), "fonts/simkai.ttf", 30);
	labelExit->setName("exit");
	labelExit->setTextColor(Color4B::BLACK);
	auto menuItem2 = MenuItemLabel::create(labelExit, CC_CALLBACK_1(GameMenu::callback, this));
	menuItem2->setPosition(838, 80);

	Menu* menu1 = Menu::create(menuItem,menuItem1,menuItem2 ,NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1);

	//用户栏
	auto playerSc = Sprite::createWithSpriteFrameName("PlayerScreen.png");
	playerSc->setPosition(150, winSize.height-65);
	this->addChild(playerSc);

	auto changePlayer1 = Sprite::createWithSpriteFrameName("changePlayer1.png");

	auto changePlayer2 = Sprite::createWithSpriteFrameName("changePlayer2.png");

	auto menuItemSprite = MenuItemSprite::create(changePlayer1, changePlayer2,CC_CALLBACK_1(GameMenu::showUserCallback,this));
	menuItemSprite->setPosition(150, winSize.height - 165);

	Menu* menu2 = Menu::create(menuItemSprite, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2);

	std::string name=UserDefault::getInstance()->getStringForKey("username", "Dave");
	auto labelUserName = Label::createWithTTF(CTool::FontToUTF8(name.c_str()), "fonts/simkai.ttf", 20);
	labelUserName->setPosition(playerSc->getContentSize().width / 2, 45);
	playerSc->addChild(labelUserName);

	return true;
}

void GameMenu::scheduleCallback(float fDelta)
{

}

void GameMenu::onBtnEvent(Ref* pSender)
{
	bool bOn = UserDefault::getInstance()->getBoolForKey("efMusic",true);
	int volume = UserDefault::getInstance()->getIntegerForKey("volumeEf",100);
	SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
	if (bOn&&volume>0)
	{
		SimpleAudioEngine::getInstance()->playEffect("pause.mp3");
	}

	MenuItemSprite* menuSprite = static_cast<MenuItemSprite*>(pSender);
	std::string name = menuSprite->getName();
	if ("maoxian"==name)
	{
		/*GameLevel* gamelevel = GameLevel::create();
		TransitionMoveInL* trans = TransitionMoveInL::create(1.0f, gamelevel);
		Director::getInstance()->replaceScene(trans);*/
		Size winSize = Director::getInstance()->getWinSize();
		LevelSelect* pSelect = LevelSelect::create();
		pSelect->setPosition(winSize.width / 2, winSize.height / 2);
		this->addChild(pSelect);
	}
}

MenuItemSprite* GameMenu::addSpriteMenu(std::string name, std::string normalName, std::string selectName, float scale, Vec2 pos)
{
	auto btn1 = Sprite::createWithSpriteFrameName(normalName);
	btn1->setScale(scale);

	auto btn2 = Sprite::createWithSpriteFrameName(selectName);
	btn2->setScale(scale);

	MenuItemSprite* menuSprite = MenuItemSprite::create(btn1, btn2, CC_CALLBACK_1(GameMenu::onBtnEvent, this));
	menuSprite->setPosition(pos);//635, 530
	menuSprite->setName(name);
	return menuSprite;
}

void GameMenu::callback(Ref* ref)
{
	auto menuItemLabel = static_cast<MenuItemLabel*>(ref);
	auto label=menuItemLabel->getLabel();
	if ("option" == label->getName())
	{
		Size winSize = Director::getInstance()->getWinSize();
		auto optionLayer = OptionLayer::create();
		optionLayer->setPosition(winSize.width / 2, winSize.height / 2 + 200);
		optionLayer->setShowLongBtn(false);
		auto moveTo = MoveTo::create(1.0f, Vec2(winSize.width/2,winSize.height/2));
		auto ease = EaseBackInOut::create(moveTo);
		optionLayer->runAction(ease);
		this->addChild(optionLayer);
	}
	else if ("help"==label->getName())
	{
		Size winSize = Director::getInstance()->getWinSize();
		auto helpLayer = CHelpLayer::create();
		helpLayer->setPosition(winSize.width / 2, winSize.height / 2);

		TransitionFade* pFade = TransitionFade::create(1.0, helpLayer);
		Director::getInstance()->replaceScene(pFade);
	}
	else if ("exit"==label->getName())
	{
		Director::getInstance()->end();
	}
}

void GameMenu::showUserCallback(Ref* pSender)
{
	//显示用户界面
	auto winSize = Director::getInstance()->getWinSize();
	auto userMenu = UserMenu::create();
	userMenu->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(userMenu);
}
