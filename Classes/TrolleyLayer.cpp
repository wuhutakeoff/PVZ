#include "TrolleyLayer.h"

CTrolleyLayer::CTrolleyLayer()
{
}

CTrolleyLayer::~CTrolleyLayer()
{
}

bool CTrolleyLayer::init()
{
    if (!Node::init())
    {
        return false;
    }

    //Ð¡ÍÆÜ‡  87,190,280,390,485
    int arr[5] = { 87,190,280,390,485 };
    /*for (int i = 0; i < 5; i++)
    {
        auto trolley = Sprite::create("Resources/trolley.png");
        trolley->setPosition(227, arr[i]);
        trolley->setName("trolley");
        this->addChild(trolley);
    }*/

    return true;
}
