#ifndef __PLANTSMENU_H__
#define __PLANTSMENU_H__
#include "cocos2d.h"
#include <unordered_map>
USING_NS_CC;
using namespace std;

class PlantsMenu : public Node
{
public:
    PlantsMenu();
    ~PlantsMenu();
    virtual bool init();
    void addPlantsInBox(std::string strName, Vec2 pos);
    void clickBoxCallback(Ref* pSender);
    virtual void update(float time);
    void setMenuEnable(bool enable);
    void setSunCount(int nCount)
    {
        string s = std::to_string(nCount);
        labelSun->setString(s);
    }

    CREATE_FUNC(PlantsMenu);
    CC_SYNTHESIZE(bool, m_bisFinsh, isFinish);
    CC_SYNTHESIZE(bool, m_bShowPlantsBox, ShowPlantsBox);
    CC_SYNTHESIZE(bool, m_bIsCool, IsCool);
private:
    //植物盒子
    Sprite* plantsBox;
    //植物列
    Sprite* plantsColumn;
    //植物盒子里的植物卡片 
    Vector<MenuItem*> m_vecSprite;

    vector<Vec2> m_vecPos;
    Menu* menuInColumn;
    Menu* menuInBox;
    Label* labelSun;
    bool m_bRemove;
    int m_nTag;
};

#endif