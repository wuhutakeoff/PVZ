#ifndef __OPTIONLAYER_H__
#define __OPTIONLAYER_H__

#include "cocos2d.h"
USING_NS_CC;

class OptionLayer : public Node
{
public:
    OptionLayer();
    ~OptionLayer();
    virtual bool init();
    void callback(Ref* pSender);
    MenuItemToggle* createToggle(std::string strBg, std::string strSelect, std::string strUnSelect, Vec2 pos, std::function<void(Ref*)> callback,bool bOn);
    void addTouch();
    void sliderCallback(Ref* pSender);
    virtual void update(float dt);
    bool isClick(Vec2 pos);

    CC_SYNTHESIZE(bool,m_bShowLongBtn,ShowLongBtn)
    CREATE_FUNC(OptionLayer);

private:
    bool m_bBgMusic;
    bool m_bEfMusic;
    Sprite* bg;
    bool m_bIsClick;
};

#endif