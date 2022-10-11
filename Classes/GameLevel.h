#ifndef __GAMELEVEL_H__
#define __GAMELEVEL_H__

#include "cocos2d.h"
#include "OptionLayer.h"
#include "PlantsMenu.h"
#include "Zombie.h"
#include "BulletLayer.h"
#include <external/json/document.h>
#include "ZombieMgr.h"
#include "Plant.h"
#include "TiledMapCtrl.h"
#include "ui/CocosGUI.h"
#include "PlantsLayer.h"
#include "UILayer.h"
#include "TrolleyLayer.h"

USING_NS_CC;
using namespace std;
using namespace ui;

class GameLevel : public Scene
{
public:
    GameLevel();
    ~GameLevel();
    virtual bool init();
    static GameLevel* getInstance();

    virtual void update(float dt);
    void scheduleCallback(float fd);
    void deletePlant(CPlant* plant);
    //产生阳光
    void produceSun();
    //点击铲子
    void clickShovel();

    void addSunCount(int count) {
        this->m_nCurSunCount += count;
    }
    static void destoryInstance()
    {
        m_spInstance = nullptr;
    }
    void deleteAll();

    CC_SYNTHESIZE(CBulletLayer*, m_pBulletLayer, BulletLayer);
    CC_SYNTHESIZE(int, m_nCurSunCount, CurSunCount);
    CC_SYNTHESIZE(CZombieMgr*, m_pZombieMgr, ZombieMgr);
    CC_SYNTHESIZE(CTiledMapCtrl*, m_pTiledMapCtrl, TiledMapCtrl);
    CC_SYNTHESIZE(CPlantsLayer*, m_pPlantLayer, PlantLayer);
    CC_SYNTHESIZE(CUILayer*, m_pUILayer, UILayer);
    CC_SYNTHESIZE(CTrolleyLayer*, m_pTrolleyLayer, TrolleyLayer);

    CREATE_FUNC(GameLevel);

private:
    static GameLevel* m_spInstance;

    Sprite* SunFlowerSprite;
    //Sprite* bg1;
    PlantsMenu* plantsMenu;
    Button* shovel;
    Sprite* shovelBack;
    Sprite* bg1;
    bool m_bRemove;
};

#endif