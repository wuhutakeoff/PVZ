#include "GameLogo.h"
#include "GameLoading.h"

GameLogo::GameLogo()
{
}

GameLogo::~GameLogo()
{
}

bool GameLogo::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();
	auto pBg = Sprite::create("Resources/Image/Background/firstpage.jpg");
	pBg->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(pBg);

	this->scheduleOnce(CC_CALLBACK_1(GameLogo::scheduleCallback, this), 2.0f, "changScene");
	return true;
}

void GameLogo::scheduleCallback(float fDelta)
{
	GameLoading* pGameLoading = GameLoading::create();
	TransitionFade* pTran = TransitionFade::create(1.0f, pGameLoading);
	Director::getInstance()->replaceScene(pTran);
}
