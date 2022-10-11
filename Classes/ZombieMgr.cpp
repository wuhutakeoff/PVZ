#include "ZombieMgr.h"
#include "GameLevel.h"

CZombieMgr::CZombieMgr()
{
	m_nCurZombieCount = 0;
	m_bIsCreateZombie = false;
	m_nRow = 0;
	m_nCurWaveCount = 0;
	isFinish = false;
}

CZombieMgr::~CZombieMgr()
{
}

bool CZombieMgr::init()
{
	if (!Node::init())
	{
		return false;
	}

	auto levelDt=CDataMgr::getInstance()->getMgr<CLevelDtMgr>("LevelDtMgr")->getCurLevelDt();
	if (levelDt->nID==1001)
	{
		m_nRow = 1;
	}
	else if (levelDt->nID==1002)
	{
		m_nRow = 3;
	}
	else
	{
		m_nRow = 5;
	}

	m_nWaveTotalCount = levelDt->vecEnemyCount.size();

	m_nTotalZombieCount = 5;

	m_vecZombies =this->createZombies();

	for (int i = 0; i < m_vecZombies.size(); i++)
	{
		this->addChild(m_vecZombies[i]);
	}


	scheduleUpdate();

	return true;
}


void CZombieMgr::update(float fDetal)
{
	if (m_nCurWaveCount == 0 && m_bIsCreateZombie)
	{
		GameLevel::getInstance()->getUILayer()->setPercent(100 / m_nWaveTotalCount);
	}


	//僵尸位置隨機  468		373		262		172		77
	addZombies();
	

	if (m_nCurZombieCount>=4&&m_nCurWaveCount<m_nWaveTotalCount-1)
	{
		GameLevel::getInstance()->getUILayer()->setPercent(200 / m_nWaveTotalCount);
		GameLevel::getInstance()->getUILayer()->showLargeWave();
		this->scheduleOnce([=](float) {
			this->showZombies();
			},5.f,"1");
	}

	//最后一波
	if (m_nCurWaveCount ==m_nWaveTotalCount-1&&!isFinish)
	{
		GameLevel::getInstance()->getUILayer()->setPercent(300 / m_nWaveTotalCount);
		GameLevel::getInstance()->getUILayer()->showLastWave();
		//所有僵尸一起出来
		this->scheduleOnce([=](float) {
			this->showAllZombies();
		}, 5.f, "2");
		isFinish = true;
	}

	if (m_nCurWaveCount >m_nWaveTotalCount)
	{
		this->unscheduleUpdate();
	}
}

bool CZombieMgr::isHaveZombieByRow(int nRow)
{
	int arr[5] = { 87,190,280,390,485 };

	Vector<Node*> vecZombie = this->getChildren();
	for (int i = 0; i < vecZombie.size(); i++)
	{
		if (vecZombie.at(i)->getPositionY()==arr[nRow-1])
		{
			return true;
		}
	}
	return false;
}

vector<CZombie*> CZombieMgr::createZombies()
{
	vector<CZombie*> vecZombie;

	//僵尸初始位置隨機
	std::default_random_engine e;
	std::uniform_int_distribution<int> u(1100, 1300); // 左閉右閉區間
	std::uniform_int_distribution<int> v(100, 500);
	e.seed(time(0));

	int xx[5];
	int yy[5];
	for (int i = 0; i < m_nTotalZombieCount; i++) {
		xx[i] = u(e);
	}
	for (int i = 0; i < m_nTotalZombieCount; i++)
	{
		yy[i] = v(e);
	}

	for (int i = 0; i < m_nTotalZombieCount; i++)
	{
		int x = xx[i];
		int y = yy[i];

		if (i == m_nTotalZombieCount - 1)
		{
			CZombiesDt* pData = CDataMgr::getInstance()->getMgr<CZombiesDtMgr>("ZombiesDtMgr")->getDataByID<CZombiesDt>(3002);
			auto m_pZombie = CZombie::createWithData(pData);
			m_pZombie->setPosition(Vec2(x, y));
			m_pZombie->changeState("run");
			vecZombie.push_back(m_pZombie);
			break;
		}

		CZombiesDt* pData = CDataMgr::getInstance()->getMgr<CZombiesDtMgr>("ZombiesDtMgr")->getDataByID<CZombiesDt>(3001);
		auto m_pZombie = CZombie::createWithData(pData);
		m_pZombie->setPosition(Vec2(x, y));
		m_pZombie->changeState("run");
		vecZombie.push_back(m_pZombie);
	}
	return vecZombie;
}


void CZombieMgr::addZombies()
{
	Vector<Node*> vecZombie = this->getChildren();
	if (m_bIsCreateZombie)
	{
		//僵尸產生	5秒出一個僵尸
		this->schedule([=](float) {
			int arr[5] = { 87,190,280,390,485 };

			std::srand(std::time(nullptr));
			int n = (rand() % 5);

			if (m_nRow == 1)
			{
				n = 2;
			}
			else if (m_nRow == 3)
			{
				n = (rand() % 3 + 1);
			}

			if (m_nCurZombieCount < m_nTotalZombieCount)
			{
				auto zombie = (CZombie*)vecZombie.at(m_nCurZombieCount);
				auto delay1 = DelayTime::create(3.f);
				auto rep = CallFunc::create([=]() {
					zombie->setPositionY(arr[n]);
					zombie->getPositionY();

					zombie->changeState("move");
					});
				auto callFun = CallFunc::create([=]() {
					zombie->scheduleUpdate();
					if (m_nCurZombieCount >= m_nTotalZombieCount - 1)
					{
						m_bIsCreateZombie = false;
						m_nCurWaveCount++;
						m_nCurZombieCount = 0;
						this->unschedule("createZombie");
						return;
					}
					m_nCurZombieCount++;
					});
				auto seq1 = Sequence::create(delay1, rep, callFun, NULL);
				zombie->runAction(seq1);
			}
			}, 5.0f, "createZombie");
		m_bIsCreateZombie = false;
	}
}

void CZombieMgr::showZombies()
{
	this->schedule([=](float) {
		int arr[5] = { 87,190,280,390,485 };

		std::srand(std::time(nullptr));
		int n = (rand() % 5);

		if (m_nRow == 1)
		{
			n = 2;
		}
		else if (m_nRow == 3)
		{
			n = (rand() % 3 + 1);
		}

		if (m_nCurZombieCount < m_nTotalZombieCount)
		{
			//僵尸初始位置隨機
			std::default_random_engine e;
			std::uniform_int_distribution<int> u(1100, 1300); // 左閉右閉區間
			std::uniform_int_distribution<int> v(100, 500);
			e.seed(time(0));

			int x = u(e);
			int y = v(e);

			
			CZombie* m_pZombie;
			if (m_nCurZombieCount==m_nTotalZombieCount-1)
			{
				CZombiesDt* pData = CDataMgr::getInstance()->getMgr<CZombiesDtMgr>("ZombiesDtMgr")->getDataByID<CZombiesDt>(3002);
				m_pZombie = CZombie::createWithData(pData);
				m_pZombie->setPosition(Vec2(x, y));
				m_pZombie->changeState("run");
				this->addChild(m_pZombie);
			}
			else
			{
				CZombiesDt* pData = CDataMgr::getInstance()->getMgr<CZombiesDtMgr>("ZombiesDtMgr")->getDataByID<CZombiesDt>(3001);
				m_pZombie = CZombie::createWithData(pData);
				m_pZombie->setPosition(Vec2(x, y));
				m_pZombie->changeState("run");
				this->addChild(m_pZombie);
			}

			

			auto delay1 = DelayTime::create(3.f);
			auto rep = CallFunc::create([=]() {
				m_pZombie->setPositionY(arr[n]);
				auto y = m_pZombie->getPositionY();

				m_pZombie->changeState("move");
				});
			auto callFun = CallFunc::create([=]() {
				m_pZombie->scheduleUpdate();
				if (m_nCurZombieCount >= m_nTotalZombieCount - 1)
				{
					m_nCurWaveCount++;
					this->unschedule("createZombie");
					m_nCurZombieCount = 0;
					return;
				}
				m_nCurZombieCount++;
				});
			auto seq1 = Sequence::create(delay1, rep, callFun, NULL);
			m_pZombie->runAction(seq1);
		}

		}, 5.f, "createZombie");
}

void CZombieMgr::showAllZombies()
{
	this->schedule([=](float) {
		int arr[5] = { 87,190,280,390,485 };

		std::srand(std::time(nullptr));
		int n = (rand() % 5);

		if (m_nRow == 1)
		{
			n = 2;
		}
		else if (m_nRow == 3)
		{
			n = (rand() % 3 + 1);
		}

		if (m_nCurZombieCount < m_nTotalZombieCount)
		{
			//僵尸初始位置隨機
			std::default_random_engine e;
			std::uniform_int_distribution<int> u(1100, 1300); // 左閉右閉區間
			std::uniform_int_distribution<int> v(100, 500);
			e.seed(time(0));

			int x = u(e);
			int y = v(e);

			int xx[5];
			int yy[5];
			for (int i = 0; i < m_nTotalZombieCount; i++) {
				xx[i] = u(e);
			}
			for (int i = 0; i < m_nTotalZombieCount; i++)
			{
				yy[i] = v(e);
			}

			CZombie* m_pZombie;
			if (m_nCurZombieCount == m_nTotalZombieCount - 1)
			{
				CZombiesDt* pData = CDataMgr::getInstance()->getMgr<CZombiesDtMgr>("ZombiesDtMgr")->getDataByID<CZombiesDt>(3002);
				m_pZombie = CZombie::createWithData(pData);
				m_pZombie->setPosition(Vec2(xx[m_nCurZombieCount],yy[m_nCurZombieCount]));
				m_pZombie->changeState("run");
				this->addChild(m_pZombie);
			}
			else
			{
				CZombiesDt* pData = CDataMgr::getInstance()->getMgr<CZombiesDtMgr>("ZombiesDtMgr")->getDataByID<CZombiesDt>(3001);
				m_pZombie = CZombie::createWithData(pData);
				m_pZombie->setPosition(Vec2(xx[m_nCurZombieCount], yy[m_nCurZombieCount]));
				m_pZombie->changeState("run");
				this->addChild(m_pZombie);
			}



			auto delay1 = DelayTime::create(3.f);
			auto rep = CallFunc::create([=]() {
				m_pZombie->setPositionY(arr[m_nCurZombieCount]);
				auto y = m_pZombie->getPositionY();

				m_pZombie->changeState("move");
				});
			auto callFun = CallFunc::create([=]() {
				m_pZombie->scheduleUpdate();
				if (m_nCurZombieCount >= m_nTotalZombieCount - 1)
				{
					m_nCurWaveCount++;
					this->unschedule("createAllZombie");
					m_nCurZombieCount = 0;
					return;
				}
				m_nCurZombieCount++;
				});
			auto seq1 = Sequence::create(delay1, rep, callFun, NULL);
			m_pZombie->runAction(seq1);
		}

		}, 0.5f, "createAllZombie");
}
