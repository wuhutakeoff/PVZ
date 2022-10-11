#include "Zombie.h"
#include "GameLevel.h"

CZombie::CZombie()
{
	m_pTargePlant = NULL;
	m_nPlantTag = -1;
	isFinish = false;
	m_fTotalTime = 0;
	m_fSped = 1;
	m_strDebuff = "normal";
	m_fDeBuffTime = 0;
}

CZombie::~CZombie()
{
	for (map<string, Animate*>::iterator iter = m_mapAnimate.begin(); iter != m_mapAnimate.end(); ++iter)
	{
		iter->second->release();
	}
	m_mapAnimate.clear();
}

bool CZombie::initWithData(CZombiesDt* pData)
{
	if (!Sprite::initWithSpriteFrameName(pData->mapAnimateName["run"][0]))
	{
		return false;
	}
	m_strState = "idle";
	this->addAnimation(pData);
	this->m_nHp = pData->nHp;
	this->m_nDamage = pData->nDamge;


	color = this->getColor();

	return true;
}
CZombie* CZombie::createWithData(CZombiesDt* pData)
{
	CZombie* pRet = new CZombie();
	if (pRet && pRet->initWithData(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void CZombie::addAnimation(CZombiesDt* pData)
{
	for (map<string, vector<string>>::iterator iter = pData->mapAnimateName.begin(); iter != pData->mapAnimateName.end(); ++iter)
	{
		string strAnimateName = iter->first;
		//����֡����
		vector<string> vecAnimateName = iter->second;

		//��������
		Animation* pAnimation = Animation::create();
		for (int i = 0; i < vecAnimateName.size(); i++)
		{
			SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(vecAnimateName[i]);
			pAnimation->addSpriteFrame(pFrame);
		}
		//�л�ʱ�䣺���Ҳ�������õ����ݡ�
		pAnimation->setDelayPerUnit(pData->mapDelay[strAnimateName]);
		//����Animate
		Animate* pAnimate = Animate::create(pAnimation);

		//���ж��������������ã���ǰ֡�����ͻᱻ�ͷš�
		//��������
		pAnimate->retain();
		m_mapAnimate[strAnimateName] = pAnimate;
	}
}

void CZombie::update(float fDetal)
{
	//��ʬ����ֲ���ʱ�����1s,ֲ���Ѫ
	if (this->getPlantTag()>0)
	{
		m_fTotalTime += fDetal;
	}

	if (GameLevel::getInstance()&&m_strState=="attack"&&m_fTotalTime>=1&& GameLevel::getInstance())
	{
		auto plant=(CPlant*)GameLevel::getInstance()->getPlantLayer()->getChildByTag(this->m_nPlantTag);
		//plant->onDamage(this->getDamge());
		m_fTotalTime = 0;
	}

	//Debuff��ʱ��
	if (m_strDebuff == "slow")
	{
		m_fDeBuffTime += fDetal;
	}
	if (m_fDeBuffTime>=4)
	{
		this->changeState("normal");
		m_fDeBuffTime = 0;
		m_fSped = 1;
		this->setColor(color);
	}


	if (this->getPosition().x <= 200&&!isFinish)
	{
		MoveTo* moveTo = MoveTo::create(1.f, Vec2(0, 0));
		auto callFunc = CallFunc::create([=]() {
			GameLevel::getInstance()->getUILayer()->showDefeat();
			});
		GameLevel::getInstance()->getTiledMapCtrl()->runAction(Sequence::createWithTwoActions(moveTo, callFunc));
		isFinish = true;
	}


	if (m_nHp <= 0&&m_strState=="lostAttack")
	{
		this->changeState("lostDie", [=]() {
			this->removeFromParent();
			});
	}
	else if (m_nHp<=100&&m_strState=="attack")
	{
		this->changeState("lostAttack");
	}
	else if (m_nHp <= 0 && m_strState == "move")
	{
		this->changeState("lostDie", [=]() {
			this->removeFromParent();
			});
	}


	if (m_nPlantTag<0)
	{
		this->changeState("move");
		return;
	}

}

void CZombie::onDamage(int nDamage)
{
	this->m_nHp -= nDamage;
}

void CZombie::changeState(string strState, function<void()> callBack /*= nullptr*/)
{
	//�жϵ�ǰ״̬��û�иġ�
	if (m_strState == strState)
	{
		return;
	}

	if (m_strState=="lostAttack"&&strState=="attack")
	{
		return;
	}

	if (m_strState=="lostDie"&&(strState=="move"|| strState == "attack"))
	{
		return;
	}

	if (m_strState=="bombDie"&&(strState == "attack"||strState=="move"))
	{
		return;
	}

	//��״̬������������һ��
	m_strState = strState;
	//ֹͣ��ǰ����
	this->stopAllActions();
	//������һ��������
	//��Щ����ֻҪ����һ��
	if ("lostDie" == strState || "bombDie" == strState)
	{
		pAction = Sequence::createWithTwoActions(m_mapAnimate[strState], CallFunc::create(callBack));
		this->runAction(pAction);
		return;
	}
	else if ("slow"==strState)
	{
		m_fDeBuffTime = 0;
		if (m_fSped<=0.5)
		{
			return;
		}
		this->m_fSped *= 0.5;
		this->setColor(Color3B::BLUE);
		this->m_strDebuff = "slow";
		return;
	}
	else if ("normal"==strState)
	{
		this->m_fSped = 1;
		return;
	}
	else if ("move"==strState)
	{
		auto y = this->getPositionY();
		auto moveBy = MoveBy::create(2/m_fSped, Vec2(-100, 0));
 		pAction = RepeatForever::create(m_mapAnimate["run"]);
		Speed* speed = Speed::create(pAction, m_fSped);
		this->runAction(speed);
		auto rep = RepeatForever::create(moveBy);
		this->runAction(rep);
		return;
	}
	else
	{
		pAction = RepeatForever::create(m_mapAnimate[strState]);
	}

	Speed* speed = Speed::create(pAction, m_fSped);
	this->runAction(speed);

}
