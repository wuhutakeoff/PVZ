#ifndef __BULLET_H__
#define __BULLET_H__
#include "cocos2d.h"
#include "DataStruct.h"
USING_NS_CC;
using namespace std;

class CBullet : public Sprite
{
public:
    CBullet();
    ~CBullet();
    virtual bool init();
    virtual bool initWithFile(const std::string& filename);
    virtual void update(float fDetal);

    CREATE_FUNC(CBullet);

    CC_SYNTHESIZE(int, m_nDamge, Damge);
    CC_SYNTHESIZE(string, m_strType, Type);
private:
};

#endif