#ifndef __PLANT_H__
#define __PLANT_H__
#include "cocos2d.h"
#include "DataStruct.h"
USING_NS_CC;
using namespace std;

class CPlant : public Sprite
{
public:
    CPlant();
    ~CPlant();
    bool initWithData(CPlantDt* pData);
    static CPlant* createWithData(CPlantDt* pData);
    void addAnimation(CPlantDt* pData);
    void runAnimation();
    void onDamage(int nDamage);
    virtual void update(float fDetla);

    CC_SYNTHESIZE(bool, m_bIsFire, IsFire);
    CC_SYNTHESIZE(string, type, Type);
    CC_SYNTHESIZE(string, bulletPath, BulletPath);
    CC_SYNTHESIZE(int, nValue, Value);
    CC_SYNTHESIZE(int, nRow, Row);
    CC_SYNTHESIZE(int, m_nDamage, Damage);
    CC_SYNTHESIZE(int, m_nHp, Hp);
private:
    Animate* pAnimate;
    string m_strName;
};

#endif