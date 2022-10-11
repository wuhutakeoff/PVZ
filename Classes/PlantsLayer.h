#ifndef __PLANTSLAYER_H__
#define __PLANTSLAYER_H__
#include "cocos2d.h"
#include "Plant.h"
USING_NS_CC;
using namespace std;

class CPlantsLayer : public Node
{
public:
    CPlantsLayer();
    ~CPlantsLayer();
    virtual bool init();
    bool havePlant(Vec2 pos);
    bool isInBox(Rect box);

    CREATE_FUNC(CPlantsLayer);
private:
    
};

#endif