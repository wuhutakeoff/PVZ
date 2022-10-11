#ifndef __USERMENU_H__
#define __USERMENU_H__
#include "ui/CocosGUI.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;

class UserMenu : public Node
{
public:
    UserMenu();
    ~UserMenu();
    virtual bool init();
    Button* addItem(std::string normalImage,std::string selectedImage,std::string btnName,Vec2 pos, std::function<void(Ref*)> callback);
    void addTouch();

    CREATE_FUNC(UserMenu);

private:
};

#endif