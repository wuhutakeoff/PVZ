#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__
#include "cocos2d.h"
#include "DataStruct.h"
#include "Plant.h"
USING_NS_CC;
class CZombie : public Sprite
{
public:
	CZombie();
	~CZombie();
	bool initWithData(CZombiesDt* pData);
	static CZombie* createWithData(CZombiesDt* pData);

	//创建这个僵尸所有的动画。
	void addAnimation(CZombiesDt* pData);
	virtual void update(float fDetal);
	void onDamage(int nDamage);

	//根据不同状态播放不同动画。
	void changeState(string strState, function<void()> callBack = nullptr);

	CC_SYNTHESIZE(int, m_nPlantTag, PlantTag);
	CC_SYNTHESIZE(CPlant*, m_pTargePlant, TargetPlant);
	CC_SYNTHESIZE(int, m_nHp, Hp);
	CC_SYNTHESIZE(int, m_nDamage, Damge);
	CC_SYNTHESIZE(float, m_fSped, Sped);
private:
	string m_strState;
	map<string, Animate*> m_mapAnimate;
	ActionInterval* pAction;
	bool isFinish;
	float m_fTotalTime;
	float m_fDeBuffTime;
	string m_strDebuff;
	Color3B color;
};


#endif // !_



