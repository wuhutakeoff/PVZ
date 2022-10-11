#ifndef __LEVELSELECT_H__
#define __LEVELSELECT_H__
#include "ui/CocosGUI.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;

class LevelSelect : public Node
{
public:
    LevelSelect();
    ~LevelSelect();
    virtual bool init();
    void addPageView();
    void addTouch();

    CREATE_FUNC(LevelSelect);

private:
    PageView* m_pPageView;
};

#endif