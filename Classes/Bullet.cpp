#include "Bullet.h"
#include "GameLevel.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
}

bool CBullet::init()
{
	if (!Sprite::initWithFile("Resources/Image/Bullets/PB0_R.png"))
	{
		return false;
	}

	this->scheduleUpdate();

	return true;
}

bool CBullet::initWithFile(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

	this->scheduleUpdate();

    return false;
}

void CBullet::update(float fDetal)
{
	float fDist = fDetal * 200;
	Vec2 pos = this->getPosition();
	pos.x += fDist;
	this->setPosition(pos);

	if (pos.x <0 || pos.x > Director::getInstance()->getWinSize().width
		|| pos.y < 0 || pos.y >Director::getInstance()->getWinSize().height)
	{
		this->removeFromParent();
		return;
	}

	/*if (GameLevel::getInstance()->getChildByName("bg1") && GameLevel::getInstance()->getChildByName("bg1")->getChildByName("zombie"))
	{
		auto pos = GameLevel::getInstance()->getChildByName("bg1")->getChildByName("zombie")->convertToWorldSpaceAR(Vec2::ZERO);
		auto box = GameLevel::getInstance()->getChildByName("bg1")->getChildByName("zombie")->getBoundingBox();
		pos.x += 30;
		pos.y -= 50;
		box.setRect(pos.x, pos.y, 75, 115);

		if (this&&box.containsPoint(this->getPosition()))
		{
			this->removeFromParent();
		}
	}*/
}


