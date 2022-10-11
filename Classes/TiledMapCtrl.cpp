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
	//获取所有种植点。
	vector<Vec2> vecPlantPos;
	//拿到对象组
	TMXObjectGroup* pGroup = m_pTiledMap->getObjectGroup("plantsPos");
	//拿到这个对象组的所有对象
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

	//通过种植点，获取种植范围（离种植点左边20像素，右边20像素，上面30像素，下面30像素）
	//首先先去拿到所有植物坐标，去定出范围，矩形。
	//for (Vec2 pos : vecPlantPos)
	//{
	//	float fBottomLeftX = pos.x - 30;
	//	float fBottomLeftY = pos.y - 20;
	//	//左下角坐标有了，宽度高度 40   60
	//	Rect rectRange = Rect(fBottomLeftX, fBottomLeftY, 50, 70);
	//	m_vecRectRange.push_back(rectRange);
	//}
}

Vec2 CTiledMapCtrl::getPlantPos(Vec2 pos)
{
	for (Rect rectRange : m_vecRectRange)
	{
		//判断移动坐标是否在某个矩形内，如果在返回中心坐标，中心坐标就是种植点。
		if (rectRange.containsPoint(pos))
		{
			return Vec2(rectRange.getMidX(), rectRange.getMidY());
		}
	}
	return Vec2::ZERO;
}
