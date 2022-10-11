#include "Plant.h"
#include "GameLevel.h"

CPlant::CPlant():m_bIsFire(false),type("")
{
	int nRow = 0;
}

CPlant::~CPlant()
{
	pAnimate->release();
}


bool CPlant::initWithData(CPlantDt* pData)
{
	if (!Sprite::init())
	{
		return false;
	}

	string path = "Resources/Image/" + pData->strName + ".plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(path);

	string strName = pData->strName+"_0.png";
	this->initWithSpriteFrameName(strName);
	this->addAnimation(pData);
	this->type = pData->type;
	this->nValue = pData->nValue;
	this->m_strName = pData->strName;
	this->m_nHp = pData->nHp;


	if (type=="attack")
	{
		this->bulletPath = pData->bulletPath;
	}
	
	//攻击类植物攻击
	this->schedule([=](float) {
		if (nRow<=0)
		{
			return;
		}
		if (GameLevel::getInstance()&&m_bIsFire && "attack" == this->type && GameLevel::getInstance()->getZombieMgr()->isHaveZombieByRow(nRow)&&(m_strName == "PeaShooter"||m_strName=="SnowPea"))
		{
			GameLevel::getInstance()->getBulletLayer()->addBullet(this, bulletPath, m_strName,pData->nDamage);
		}
		else if (m_bIsFire && "attack" == this->type && (GameLevel::getInstance()->getZombieMgr()->isHaveZombieByRow(nRow)|| 
			GameLevel::getInstance()->getZombieMgr()->isHaveZombieByRow(nRow-1)|| GameLevel::getInstance()->getZombieMgr()->isHaveZombieByRow(nRow+1))
			&& m_strName == "Threepeater")
		{
			GameLevel::getInstance()->getBulletLayer()->addBullet(this, bulletPath, m_strName, pData->nDamage);
		}
		}, 2.f, "fire");

	//向日葵生产阳光
	this->schedule([=](float) {
		if (m_bIsFire && "producte" == this->type)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Image/Sun.plist");
			auto sun = Sprite::createWithSpriteFrameName("Sun_0.png");
			Vec2 pos = this->convertToWorldSpaceAR(Vec2::ZERO);
			pos.x += 10;
			sun->setPosition(pos);
			this->getParent()->addChild(sun);


			Animation* pAnimation = Animation::create();
			for (int i = 0; i < 22; i++)
			{
				string name = StringUtils::format("Sun_%d.png", i);
				SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
				pAnimation->addSpriteFrame(pFrame);
			}
			pAnimation->setDelayPerUnit(0.1f);
			auto animate = Animate::create(pAnimation);
			auto delay = DelayTime::create(3.0f);
			auto callFunc = CallFunc::create([=]() {
				sun->removeFromParent();
				});
			auto seq = Sequence::create(animate, delay, callFunc, NULL);
			sun->runAction(seq);


			EventListenerMouse* pListener = EventListenerMouse::create();
			pListener->onMouseDown = [=](EventMouse* event)
			{
				auto but = event->getMouseButton();
				auto box=sun->getBoundingBox();
				Vec2 pos=event->getLocationInView();

				if (but == EventMouse::MouseButton::BUTTON_LEFT&& box.containsPoint(pos))
				{
					auto moveTo = MoveTo::create(1.f,Vec2(42, 565));
					auto callFunc = CallFunc::create([=]() {
						//_eventDispatcher->removeEventListenersForTarget(sun);
						sun->removeFromParentAndCleanup(true);
						GameLevel::getInstance()->addSunCount(25);
						});
					auto seq = Sequence::create(moveTo, callFunc, NULL);
					sun->runAction(seq);
				}
			};

			_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, sun);


		}

	}, 10.f, "producteSun");

	this->scheduleUpdate();

	return true;
}

CPlant* CPlant::createWithData(CPlantDt* pData)
{
	CPlant* pRet = new CPlant();
	if (pRet && pRet->initWithData(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void CPlant::addAnimation(CPlantDt* pData)
{
	vector<string> vecAnim = pData->vecAnimPath;

	Animation* pAnimation = Animation::create();
	for (int i = 0; i < vecAnim.size(); i++)
	{
		SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(vecAnim[i]);
		pAnimation->addSpriteFrame(pFrame);
	}
	pAnimation->setDelayPerUnit(0.1f);
	pAnimate = Animate::create(pAnimation);
	pAnimate->retain();
}

void CPlant::runAnimation()
{
	if (this->type=="consume")
	{
		auto callFunc = CallFunc::create([=]() {
			//GameLevel::getInstance()->deletePlant(this);
			auto vecChild=GameLevel::getInstance()->getZombieMgr()->getChildren();
			for (int i = 0; i < vecChild.size(); i++)
			{
				auto pos1 = vecChild.at(i)->getPosition();
				pos1.x -= 200;
				auto box1 = vecChild.at(i)->getBoundingBox();
				box1.origin = pos1;

				Rect box;
				box.origin = this->getPosition();//250 285
				box.size = Size(250, 285);

				if (box.intersectsRect(box1))
				{
					auto zombie = (CZombie*)vecChild.at(i);
					zombie->changeState("bombDie",[=](){
						zombie->removeFromParent();
					});
				}
			}
			this->removeFromParent();
			});
		auto seq = Sequence::create(pAnimate, callFunc, NULL);
		this->runAction(seq);
	}
	else
	{
		this->runAction(RepeatForever::create(pAnimate));
	}
}

void CPlant::onDamage(int nDamge)
{
	m_nHp -= nDamge;
}

void CPlant::update(float fDetla)
{
	if (nRow<=0)
	{
		return;
	}
	if (m_nHp<=0)
	{
		this->removeFromParent();
	}
}
