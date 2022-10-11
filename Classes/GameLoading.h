#ifndef __GAMELOADING_H__
#define __GAMELOADING_H__

#include "cocos2d.h"

USING_NS_CC;

class GameLoading : public Scene
{
public:
    GameLoading();
    ~GameLoading();
    virtual bool init();
    virtual void update(float dt);
    void callback(Ref* pSender);

    CREATE_FUNC(GameLoading);

private:
    ProgressTimer* timer;
    Label* label;
    Sprite* tag;
};

#endif