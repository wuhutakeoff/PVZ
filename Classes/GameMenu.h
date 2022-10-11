#ifndef __GAMEMENU_H__
#define __GAMEMENU_H__

#include "cocos2d.h"
USING_NS_CC;

class GameMenu : public Scene
{
public:
    GameMenu();
    ~GameMenu();
    virtual bool init();
    void scheduleCallback(float fDelta);
    void onBtnEvent(Ref* pSender);
    MenuItemSprite* addSpriteMenu(std::string name,std::string normalName,std::string selectName,float scale,Vec2 pos);
    void callback(Ref* ref);
    void showUserCallback(Ref* pSender);

    CREATE_FUNC(GameMenu);

private:
};

#endif