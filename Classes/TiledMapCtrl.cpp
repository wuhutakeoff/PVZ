#include "TiledMapCtrl.h"

CTiledMapCtrl::CTiledMapCtrl()
{
}

CTiledMapCtrl::~CTiledMapCtrl()
{
}

bool CTiledMapCtrl::initWithData(CLevelDt* pLevelDt)
{
	if (!Node::init())
	{
		return false;
	}

	m_pTiledMap = TMXTiledMap::create(pLevelDt->strTmx);
	this->addChild(m_pTiledMap);

	this->getPlantRange();

	return true;
}

CTiledMapCtrl* CTiledMapCtrl::createWithData(CLevelDt* pLevelDt)
{
	CTiledMapCtrl* pRet = new CTiledMapCtrl();
	if (pRet && pRet->initWithData(pLevelDt))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void CTiledMapCtrl::getPlantRange()
{
	//��ȡ������ֲ�㡣
	vector<Vec2> vecPlantPos;
	//�õ�������
	TMXObjectGroup* pGroup = m_pTiledMap->getObjectGroup("plantsPos");
	//�õ��������������ж���
	ValueVector vObjs = pGroup->getObjects();
	for (int i = 0; i < vObjs.size(); i++)
	{
		ValueMap vObj = vObjs[i].asValueMap();
		float fX = vObj["x"].asFloat();
		float fY = vObj["y"].asFloat();                                                                                                                                                                                       

		float width = vObj["width"].asFloat();
		float height = vObj["height"].asFloat();

		Rect rectRange = Rect(fX, fY, width, height);
		m_vecRectRange.push_back(rectRange);

		//vecPlantPos.push_back(Vec2(fX, fY));
	}

	//ͨ����ֲ�㣬��ȡ��ֲ��Χ������ֲ�����20���أ��ұ�20���أ�����30���أ�����30���أ�
	//������ȥ�õ�����ֲ�����꣬ȥ������Χ�����Ρ�
	//for (Vec2 pos : vecPlantPos)
	//{
	//	float fBottomLeftX = pos.x - 30;
	//	float fBottomLeftY = pos.y - 20;
	//	//���½��������ˣ���ȸ߶� 40   60
	//	Rect rectRange = Rect(fBottomLeftX, fBottomLeftY, 50, 70);
	//	m_vecRectRange.push_back(rectRange);
	//}
}

Vec2 CTiledMapCtrl::getPlantPos(Vec2 pos)
{
	for (Rect rectRange : m_vecRectRange)
	{
		//�ж��ƶ������Ƿ���ĳ�������ڣ�����ڷ����������꣬�������������ֲ�㡣
		if (rectRange.containsPoint(pos))
		{
			return Vec2(rectRange.getMidX(), rectRange.getMidY());
		}
	}
	return Vec2::ZERO;
}
