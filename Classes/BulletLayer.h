#ifndef __BULLETLAYER_H__
#define	__BULLETLAYER_H__
#include "cocos2d.h"
#include "Bullet.h"
#include <vector>

using namespace std;
USING_NS_CC;
class CBulletLayer : public Node
{
public:
	CBulletLayer();
	~CBulletLayer();
	virtual bool init();
	void addBullet(Node* pFireNode, string bulletPath, string name, int nDamge);
	virtual void update(float fDetal);
	
	CREATE_FUNC(CBulletLayer);

private:

};
#endif