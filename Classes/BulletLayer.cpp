#include "BulletLayer.h"

CBulletLayer::CBulletLayer()
{
}

CBulletLayer::~CBulletLayer()
{
}

bool CBulletLayer::init()
{
	if (!Node::init())
	{
		return false;
	}
	scheduleUpdate();
	return true;
}

void CBulletLayer::addBullet(Node* pFireNode, string bulletPath,string name,int nDamage)
{
	CBullet* pBullet = CBullet::create();
	pBullet->initWithFile(bulletPath);
	pBullet->setDamge(nDamage);
	if (bulletPath=="Resources/Image/Bullets/PB1_0.png")
	{
		pBullet->setType("slow");
	}
	this->addChild(pBullet);
	//Vec2 pos = pFireNode->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 pos = pFireNode->getPosition();
	pos.y += 15;
	pos.x += 10;
	pBullet->setPosition(pos);

	if (name == "Threepeater")
	{
		CBullet* pBullet1 = CBullet::create();
		pBullet1->initWithFile(bulletPath);
		this->addChild(pBullet1);
		pBullet1->setPosition(pos.x, pos.y + 100);

		CBullet* pBullet2 = CBullet::create();
		pBullet2->initWithFile(bulletPath);
		this->addChild(pBullet2);
		pBullet2->setPosition(pos.x, pos.y - 100);
	}
}

void CBulletLayer::update(float fDetal)
{

}



