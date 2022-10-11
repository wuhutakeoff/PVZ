#include "OptionLayer.h"
#include "Tool.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameMenu.h"
#include "GameLevel.h"
using namespace CocosDenshion;
using namespace ui;

OptionLayer::OptionLayer()
{
	m_bShowLongBtn = false;
}

OptionLayer::~OptionLayer()
{
}

bool OptionLayer::init()
{
	if (!Node::init())
	{
		return false;
	}

	m_bBgMusic = UserDefault::getInstance()->getBoolForKey("bgMusic", true);
	m_bEfMusic = UserDefault::getInstance()->getBoolForKey("efMusic", true);

	addTouch();

	//auto bg = LayerColor::create(Color4B::WHITE, 400, 300);
	//bg->setIgnoreAnchorPointForPosition(false);//层调用这个接口，false，那么锚点就可修改。
	//bg->setAnchorPoint(Vec2(0.5, 0.5));
	//bg->setOpacity(160);
	//this->addChild(bg);

	bg = Sprite::create("Resources/option.png");
	bg->setPosition(Vec2::ZERO);
	this->addChild(bg);

	auto size=bg->getContentSize();

	/*auto label= Label::createWithTTF(CTool::FontToUTF8("确定"), "fonts/simkai.ttf", 30);
	MenuItemLabel* menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(OptionLayer::callback, this));
	menuItem->setPosition(680, 100);
	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);*/

	auto label1 = Label::createWithTTF(CTool::FontToUTF8("音乐"), "fonts/simkai.ttf", 30);
	auto label2 = Label::createWithTTF(CTool::FontToUTF8("音效"), "fonts/simkai.ttf", 30);
	label1->setPosition(128, 350);
	label2->setPosition(128, 300);
	bg->addChild(label1);
	bg->addChild(label2);

	auto taggleBgMs = this->createToggle("", "Resources/box2.png", "Resources/box1.png", Vec2(188, 350), [=](Ref* pSender) {
		MenuItemToggle* pToggle = static_cast<MenuItemToggle*>(pSender);
		MenuItem* pItem = pToggle->getSelectedItem();
		std::string strName = pItem->getName();
		if ("select" == strName)
		{
			m_bBgMusic = true;
			CCLOG("on bgMusic");
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		else if ("unSelect" == strName)
		{
			m_bBgMusic = false;
			CCLOG("off bgMusic");
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
		UserDefault::getInstance()->setBoolForKey("bgMusic", m_bBgMusic);
		},m_bBgMusic);

	auto taggleEfMs = this->createToggle("", "Resources/box2.png", "Resources/box1.png", Vec2(188, 300), [=](Ref* pSender) {
		MenuItemToggle* pToggle = static_cast<MenuItemToggle*>(pSender);
		MenuItem* pItem = pToggle->getSelectedItem();
		std::string strName = pItem->getName();
		if ("select" == strName)
		{
			m_bEfMusic = true;
			CCLOG("on bgMusic");
		}
		else if ("unSelect" == strName)
		{
			m_bEfMusic = false;
			CCLOG("off bgMusic");
		}
		UserDefault::getInstance()->setBoolForKey("efMusic", m_bEfMusic);
		},m_bEfMusic);

	Menu* pMenu = Menu::create(taggleBgMs, taggleEfMs, NULL);
	pMenu->setPosition(Vec2::ZERO);
	bg->addChild(pMenu);

	//滑块1
	auto sliderBg = Slider::create("slider.png", "btt.png");
	sliderBg->setPosition(Vec2(280, 350));
	sliderBg->setName("sliderBg");
	bg->addChild(sliderBg);
	sliderBg->addEventListener(CC_CALLBACK_1(OptionLayer::sliderCallback, this));
	int volumeBg=UserDefault::getInstance()->getIntegerForKey("volumeBg", 100);
	sliderBg->setPercent(volumeBg);


	//滑块2
	auto sliderEf = Slider::create("slider.png", "btt.png");
	sliderEf->setPosition(Vec2(280, 300));
	sliderEf->setName("sliderEf");
	bg->addChild(sliderEf);
	sliderEf->addEventListener(CC_CALLBACK_1(OptionLayer::sliderCallback, this));
	int volumeEf = UserDefault::getInstance()->getIntegerForKey("volumeEf", 100);
	sliderEf->setPercent(volumeEf);

	auto label = Label::createWithTTF(CTool::FontToUTF8("确定"), "fonts/simkai.ttf", 40);
	auto btn = Button::create("Resources/btn1.png", "Resources/btn2.png");
	btn->setTitleLabel(label);
	btn->setPosition(Vec2(210, 50));
	bg->addChild(btn);

	btn->addClickEventListener([=](Ref* pSender) {
		Size winSize = Director::getInstance()->getWinSize();
		//处理逻辑。
		//this->removeFromParent();
		MoveBy* pMoveBy = MoveBy::create(1.0f, Vec2(0, -winSize.height / 2 - 250));
		EaseElasticInOut* pEase = EaseElasticInOut::create(pMoveBy);
		Sequence* pSeq = Sequence::createWithTwoActions(pEase, CallFunc::create([=]() {
			//当前层要移除，那么这个曾相关的事件对象也要移除。
			_eventDispatcher->removeEventListenersForTarget(this);
			this->removeFromParent();
			}));
		this->runAction(pSeq);
		UserDefault::getInstance()->setIntegerForKey("volumeBg", sliderBg->getPercent());
		UserDefault::getInstance()->setIntegerForKey("volumeEf", sliderEf->getPercent());
		});


	scheduleUpdate();

	return true;
}

void OptionLayer::callback(Ref* pSender)
{

}

MenuItemToggle* OptionLayer::createToggle(std::string strBg, std::string strSelect, std::string strUnSelect, Vec2 pos, std::function<void(Ref*)> callback,bool bOn)
{
	/*Sprite* pBg = Sprite::create(strBg);
	this->addChild(pBg);
	pBg->setPosition(pos);*/
	MenuItemImage* pSelect = MenuItemImage::create(strSelect, strSelect);
	pSelect->setName("select");
	MenuItemImage* pUnSelect = MenuItemImage::create(strUnSelect, strUnSelect);
	pUnSelect->setName("unSelect");
	MenuItemToggle* pToggle = MenuItemToggle::createWithCallback(callback, pSelect, pUnSelect, nullptr);
	pToggle->setPosition(pos);
	int index = bOn ? 0 : 1;
	pToggle->setSelectedIndex(index);
	return  pToggle;
	return nullptr;
}

void OptionLayer::addTouch()
{
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	//2.设置监听回调。
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent) {
		//获取触摸坐标
		Vec2 pos = pTouch->getLocation();
		CCLOG("touchbegan:%f,%f", pos.x, pos.y);
		if (isClick(pos))
		{
			m_bIsClick = true;
		}
		return true;
	};
	pListener->onTouchMoved = [=](Touch* pTouch, Event* pEvent) {
		Vec2 pos = pTouch->getLocation();
		CCLOG("touchmove:%f,%f", pos.x, pos.y);
		if (m_bIsClick)
		{
			Vec2 curPos = pTouch->getLocation();
			Vec2 prePos = pTouch->getPreviousLocation();
			Vec2 deltaPos = curPos - prePos;
			Vec2 playerPos = this->getPosition();
			playerPos += deltaPos;
			this->setPosition(playerPos);
		}
	};
	pListener->onTouchEnded = [=](Touch* pTouch, Event* pEvent) {
		Vec2 pos = pTouch->getLocation();
		CCLOG("touchend:%f,%f", pos.x, pos.y);
		m_bIsClick = false;
	};
	//触摸穿透，触摸不会穿透到下一层这个层后面的触摸全部被拦截。
	pListener->setSwallowTouches(true);
	//3.添加事件监听。
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

void OptionLayer::sliderCallback(Ref* pSender)
{
	auto slider = static_cast<Slider*>(pSender);
	float volume = slider->getPercent();

	if ("sliderBg"==slider->getName())
	{
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume * 2);
	}
	else if ("sliderEf"==slider->getName())
	{
		SimpleAudioEngine::getInstance()->setEffectsVolume(volume * 2);
	}

}

void OptionLayer::update(float dt)
{
	if (m_bShowLongBtn)
	{
		auto size = bg->getContentSize();
		auto mainMenuBtn = Button::create("Resources/longBtn.png", "Resources/longBtn2.png");
		mainMenuBtn->setTitleText(CTool::FontToUTF8("主菜单"));
		mainMenuBtn->setTitleFontSize(20);
		mainMenuBtn->setPosition(Vec2(size.width / 2, 130));
		bg->addChild(mainMenuBtn);
		mainMenuBtn->addClickEventListener([=](Ref* pSender)
			{
				auto menu=(PlantsMenu*)GameLevel::getInstance()->getChildByName("plantsMenu");
				menu->setIsCool(false);
				//menu->unscheduleUpdate();
				GameLevel::getInstance()->unscheduleUpdate();
				GameLevel::getInstance()->deleteAll();
				GameLevel::destoryInstance();
				GameMenu* gameMenu = GameMenu::create();
				auto pTrans = TransitionFade::create(1.0f, gameMenu);
				Director::getInstance()->replaceScene(pTrans);
			});
		m_bShowLongBtn = false;
	}
}

bool OptionLayer::isClick(Vec2 pos)
{
	Rect rcModel = bg->getBoundingBox();
	rcModel.origin = bg->convertToWorldSpaceAR(rcModel.origin);
	return rcModel.containsPoint(pos);
}
