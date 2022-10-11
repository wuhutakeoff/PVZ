#ifndef __TILEDMAPCTRL_H__
#define __TILEDMAPCTRL_H__
#include "cocos2d.h"
#include "DataStruct.h"
USING_NS_CC;
using namespace std;

class CTiledMapCtrl : public Node
{
public:
    CTiledMapCtrl();
    ~CTiledMapCtrl();

    virtual bool initWithData(CLevelDt* pLevelDt);
    static CTiledMapCtrl* createWithData(CLevelDt* pLevelDt);

    void getPlantRange();
    Vec2 getPlantPos(Vec2 pos);

    CC_SYNTHESIZE(TMXTiledMap*, m_pTiledMap, TiledMap);


private:
    std::vector<Rect> m_vecRectRange;
};

#endif