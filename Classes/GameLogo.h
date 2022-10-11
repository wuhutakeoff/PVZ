#ifndef __GAMELOG_H__
#define __GAMELOG_H__

#include "cocos2d.h"
USING_NS_CC;

class GameLogo : public Scene
{
public:
    GameLogo();
    ~GameLogo();
    virtual bool init();
    void scheduleCallback(float fDelta);

    CREATE_FUNC(GameLogo);

private:
};

#endif