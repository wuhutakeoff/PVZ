#include "UILayer.h"
#include "Tool.h"
#include "GameLevel.h"
#include "GameMenu.h"

CUILayer::CUILayer()
{
	progress = NULL;
	m_nCurCount = 0;
	m_nCurPercent = 0;
	m_bChangeProgress = false;
	m_fTime = 0;
}

CUILayer::~CUILayer()
{
}

bool CUILayer::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_nCurCount = 1;

	this->scheduleUpdate();

	return true;
}

void CUILayer::startGame()
{
	Size size = GameLevel::getInstance()->getContentSize();
	Sprite* sprite = Sprite::create("Resources/PrepareGrowPlants_0.png");
	sprite->setPosition(size.width / 2, size.height / 2);
	this->addChild(sprite);
	this->schedule([=](float) {
		if (m_nCurCount >= 3)
		{
			this->unschedule("change");
			sprite->removeFromParent();
			return;
		}
		string texture=StringUtils::format("Resources/PrepareGrowPlants_%d.png", m_nCurCount++);
		sprite->setTexture(texture);

		}, 1.f, "change");
}

void CUILayer::showProgress()
{
	auto bg = Sprite::create("Resources/FlagMeterEmpty.png");
	auto fg = Sprite::create("Resources/FlagMeterFull.png");
	auto name = Sprite::create("Resources/FlagMeterLevelProgress.png");
	bg->setPosition(700, 20);
	this->addChild(bg);
	name->setPosition(700, 34);
	this->addChild(name);

	progress = ProgressTimer::create(fg);
	progress->setPosition(700, 20);
	progress->setType(kCCProgressTimerTypeBar);
	progress->setPercentage(0);
	progress->setMidpoint(CCPoint(0, 1));
	progress->setBarChangeRate(CCPoint(1, 0));
	this->addChild(progress);
}

void CUILayer::showLastWave()
{
	Size size = GameLevel::getInstance()->getContentSize();
	Sprite* sprite = Sprite::create("Resources/FinalWave.png");
	sprite->setPosition(size.width / 2, size.height / 2);
	this->addChild(sprite);
	auto delay = DelayTime::create(1.0f);
	;	auto callFunc = CallFunc::create([=]() {
		sprite->removeFromParent();
		});
	sprite->runAction(Sequence::createWithTwoActions(delay, callFunc));
}

void CUILayer::showLargeWave()
{
	Size size = GameLevel::getInstance()->getContentSize();
	Sprite* sprite = Sprite::create("Resources/LargeWave.png");
	sprite->setPosition(size.width / 2, size.height / 2);
	this->addChild(sprite);
	auto delay = DelayTime::create(1.0f);
;	auto callFunc = CallFunc::create([=]() {
		sprite->removeFromParent();
		});
	sprite->runAction(Sequence::createWithTwoActions(delay, callFunc));
}

void CUILayer::showDefeat()
{
	Size size = GameLevel::getInstance()->getContentSize();
	Sprite* sprite = Sprite::create("Resources/gameOver.png");
	sprite->setPosition(size.width / 2, size.height / 2+20);
	this->addChild(sprite);
	
	auto mainMenuBtn = Button::create("Resources/longBtn.png", "Resources/longBtn2.png");
	mainMenuBtn->setTitleText(CTool::FontToUTF8("Ö÷²Ëµ¥"));
	mainMenuBtn->setTitleFontSize(20);
	mainMenuBtn->setPosition(Vec2(size.width / 2, 100));
	this->addChild(mainMenuBtn);
	mainMenuBtn->addClickEventListener([=](Ref* pSender)
		{
			auto menu = (PlantsMenu*)GameLevel::getInstance()->getChildByName("plantsMenu");
			menu->setIsCool(false);
			GameLevel::getInstance()->unscheduleUpdate();
			GameLevel::getInstance()->deleteAll();
			GameLevel::destoryInstance();
			GameMenu* gameMenu = GameMenu::create();
			auto pTrans = TransitionFade::create(1.0f, gameMenu);
			Director::getInstance()->replaceScene(pTrans);
		});

	//auto delay = DelayTime::create(1.0f);
	//;	auto callFunc = CallFunc::create([=]() {
	//	//sprite->removeFromParent();
	//	});
	//sprite->runAction(Sequence::createWithTwoActions(delay, callFunc));
}


void CUILayer::update(float fDetal)
{
	if (progress&&m_nCurPercent<=m_nPercent&&m_bChangeProgress)
	{
		m_fTime += fDetal;
		if (m_fTime>=0.5)
		{
			m_nCurPercent++;
			progress->setPercentage(m_nCurPercent);
			m_fTime = 0;
		}
	}
}

