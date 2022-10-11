#ifndef __ZOMBIEMGR_H__
#define	__ZOMBIEMGR_H__
#include "cocos2d.h"
#include <vector>
#include "Zombie.h"
#include "DataStruct.h"

using namespace std;
USING_NS_CC;
class CZombieMgr : public Node
{
public:
	CZombieMgr();
	~CZombieMgr();
	virtual bool init();
	vector<CZombie*> getZombies() {
		return m_vecZombies;
	}
	virtual void update(float fDetal);
	bool isHaveZombieByRow(int nRow);
	vector<CZombie*> createZombies();
	void addZombies();
	void showZombies();
	void showAllZombies();

	CREATE_FUNC(CZombieMgr);
	CC_SYNTHESIZE(bool, m_bIsCreateZombie, IsCreateZombie);

private:
	vector<CZombie*> m_vecZombies;
	int m_nCurZombieCount;
	int m_nTotalZombieCount;
	int m_nRow;
	int m_nCurWaveCount;
	int m_nWaveTotalCount;
	bool isFinish;
	int m_nTotalCount;
};
#endif