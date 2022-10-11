#include "PlantsMenu.h"
#include "ui/CocosGUI.h"
#include "DataStruct.h"
#include "Plant.h"
#include "GameLevel.h"

using namespace ui;

PlantsMenu::PlantsMenu()
{
    m_bisFinsh = false;
    menuInColumn = nullptr;
    m_bIsCool = false;
    m_bRemove = false;
    m_nTag = 0;
}

PlantsMenu::~PlantsMenu()
{
}

bool PlantsMenu::init()
{
	if (!Node::init())
	{
		return false;
	}

    auto winSize = Director::getInstance()->getWinSize();

    //植物行
    menuInColumn = Menu::create();
    menuInColumn->setPosition(Vec2::ZERO);
    menuInColumn->setZOrder(2);
    plantsColumn = Sprite::create("Resources/plantsColumn.png");
    plantsColumn->setPosition(225, winSize.height + 45);
    auto moveToUp = MoveTo::create(1.0f, Vec2(225, winSize.height - 45));
    auto delay1 = DelayTime::create(4.0f);
    auto seq1 = Sequence::create(delay1, moveToUp, NULL);
    this->addChild(plantsColumn,1);
    plantsColumn->addChild(menuInColumn,2);
    plantsColumn->setName("plantsColumn");
    plantsColumn->runAction(seq1);
    plantsColumn->setZOrder(2);

    //植物盒子
    plantsBox = Sprite::create("Resources/plantsBox.png");//464 514
    plantsBox->setPosition(235, -260);
    moveToUp = MoveTo::create(1.0f, Vec2(235, 255));
    seq1 = Sequence::create(delay1, moveToUp, NULL);
    this->addChild(plantsBox,1);
    plantsBox->runAction(seq1);
    m_bShowPlantsBox = true;

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/plants/plants.plist");

    vector<string> vecNames = CDataMgr::getInstance()->getMgr<CPlantDtMgr>("PlantDtMgr")->getNames();

    for (int i = 0; i < vecNames.size(); i++)
    {
        string name = vecNames[i];
        name += ".png";
        addPlantsInBox(name, Vec2((52 * i + 50), 442));
    }

    //addPlantsInBox("PeaShooter.png", Vec2(50, 442));

    //addPlantsInBox("SunFlower.png", Vec2(102, 444));

    //addPlantsInBox("CherryBomb.png", Vec2(154, 442));

    //addPlantsInBox("WallNut.png", Vec2(206, 442));

    //addPlantsInBox("PotatoMine.png", Vec2(258, 442));

    //addPlantsInBox("SnowPea.png", Vec2(310, 442));

    //addPlantsInBox("Threepeater.png", Vec2(362, 442));

    menuInBox = Menu::createWithArray(m_vecSprite);
    menuInBox->setPosition(Vec2::ZERO);
    plantsBox->addChild(menuInBox,2);

    Size size = plantsBox->getContentSize();
    auto start = Sprite::create("Resources/StartGameBtn.png");
    start->setPosition(size.width / 2, 20);
    start->setName("start");
    plantsBox->addChild(start);

    labelSun = Label::createWithTTF("0", "fonts/simkai.ttf", 20);
    labelSun->setPosition(39, 17);
    labelSun->setColor(Color3B::RED);
    plantsColumn->addChild(labelSun);

    for (int i = 0; i < 7; i++)
    {
        m_vecPos.push_back(Vec2(100 + i * 52, 40));
    }

    scheduleUpdate();

	return true;
}

void PlantsMenu::addPlantsInBox(std::string strName, Vec2 pos)
{
    auto plants = Sprite::createWithSpriteFrameName(strName);
    plants->setName(strName);
    std::string blackName = "black" + strName;
    auto blackPlants = Sprite::createWithSpriteFrameName(blackName);
    auto disablePlants = Sprite::createWithSpriteFrameName(blackName);
    auto menuItemPlants = MenuItemSprite::create(plants, blackPlants,disablePlants);
    menuItemPlants->setCallback(CC_CALLBACK_1(PlantsMenu::clickBoxCallback, this));
    menuItemPlants->setPosition(pos);
    menuItemPlants->setName(strName);
    menuItemPlants->setZOrder(2);
    m_vecSprite.pushBack(menuItemPlants);
}

void PlantsMenu::clickBoxCallback(Ref* pSender)
{
    int count = 0;
    if (menuInColumn)
    {
        count = menuInColumn->getChildrenCount();
    }

    auto item = static_cast<MenuItemSprite*>(pSender);
    auto image = (Sprite*)item->getNormalImage();
    std::string name = image->getName();
    item->setEnabled(false);

    string sname = name;
    int size = sname.find('.');
    sname = sname.substr(0, size);

    auto plant = Sprite::createWithSpriteFrameName(name);
    plant->setName(sname);
    auto blackPlant = Sprite::createWithSpriteFrameName("black" + name);
    auto diablePlant = Sprite::createWithSpriteFrameName("black" + name);

    auto menuItem = MenuItemSprite::create(plant, blackPlant, diablePlant);
    menuItem->setZOrder(3);
    menuItem->setName(name);

    if (count >= 5)
    {
        Size size = plantsBox->getContentSize();
        auto btnStart = Button::create("Resources/StartGameBtn1.png", "Resources/StartGameBtn2.png");
        btnStart->setPosition(Vec2(size.width / 2, 30));
        btnStart->setName("startBtn");
        //plantsBox->removeChildByName("start");
        plantsBox->addChild(btnStart);
        btnStart->addClickEventListener([=](Ref* pSender)
            {
                setMenuEnable(false);
                auto size = plantsBox->getContentSize();
                auto moveTo = MoveTo::create(1.0f, Vec2(235, -size.height / 2));
                auto remove = CallFunc::create([=]() {
                    plantsBox->removeAllChildren();
                    plantsBox->removeFromParent();
                    plantsBox = NULL;
                    m_bShowPlantsBox = false;
                    m_bIsCool = true;
                    m_bisFinsh = true;
                    });
                auto seq = Sequence::create(moveTo, remove, NULL);
                plantsBox->runAction(seq);
            });
    }

    menuItem->setCallback([=](Ref* pSender) {
        if (m_bShowPlantsBox)
        {
            Node* child;
            string name = menuItem->getName();
            if (menuInBox)
            {
                child=menuInBox->getChildByName(name);
            }

            /*child->setPosition(child->convertToWorldSpace(child->getPosition()));
            menuItem->setPosition(menuItem->convertToWorldSpace(menuItem->getPosition()));*/
           

            Vec2 pos1 = plantsColumn->getPosition();
            Vec2 pos2 = child->getPosition();
            pos2.y = pos2.y - pos1.y;

            auto moveTo = MoveTo::create(0.5f, pos2);
            auto remove = CallFunc::create([=]() {
                menuItem->removeFromParent();
                item->setEnabled(true);

                int count1;
                if (menuInColumn)
                {
                    count1 = menuInColumn->getChildrenCount();
                }

                if (plantsBox->getChildByName("startBtn") && count1 < 6)
                {
                    plantsBox->removeChildByName("startBtn");
                }
                });  

            auto seq = Sequence::create(moveTo, remove, NULL);
            menuItem->runAction(seq);

            Vector<Node*> vecItem = menuInColumn->getChildren();
            for (int i = 0; i < vecItem.size(); i++)
            {
                if (menuItem == (MenuItemSprite*)vecItem.at(i))
                {
                    for (int j = i + 1; j < vecItem.size(); j++)
                    {
                        vecItem.at(j)->setPosition(m_vecPos.at(j - 1));
                    }
                }
            }
        }
        else
        {
            auto item = static_cast<MenuItemSprite*>(pSender);
            string name = item->getName();

            string sname = name;
            int size = sname.find('.');
            sname = sname.substr(0, size);
            auto plantDt=CDataMgr::getInstance()->getMgr<CPlantDtMgr>("PlantDtMgr")->getDataByName(sname);

            if (plantDt)
            {
                if (GameLevel::getInstance()->getPlantLayer()->isInBox(plantsColumn->getBoundingBox()))
                {
                    return;
                }
                CPlant* plant = CPlant::createWithData(plantDt);
                plant->setName("plant");
                plant->setPosition(menuItem->getPosition().x, menuItem->getPosition().y + 510);//510
                this->addChild(plant);


                EventListenerMouse* pListener = EventListenerMouse::create();
                pListener->onMouseDown = [=](EventMouse* event) {
                    auto but=event->getMouseButton();
                    if (but==EventMouse::MouseButton::BUTTON_LEFT)
                    {     
                        Vec2 pos = event->getLocationInView();
                        Vec2 pos1 = Vec2(pos.x + 220, 600 - pos.y);


                        auto plantPos = GameLevel::getInstance()->getTiledMapCtrl()->getPlantPos(pos1);


                        if (plantPos == Vec2::ZERO||!m_bRemove)
                        {
                            return;
                        }

                        plant->setPosition(plantPos.x - 220, 550 - plantPos.y);
                        plant->setOpacity(255);
                        plant->setVisible(false);

                        //添加植物到植物层里
                        CPlant* plant1 = CPlant::createWithData(plantDt);
                        plant1->setTag(m_nTag++);
                        plant1->setPosition(plantPos.x - 220, 550 - plantPos.y);
                        GameLevel::getInstance()->getPlantLayer()->addChild(plant1);
                        plant1->runAnimation();
                        plant1->setIsFire(true);
                        if (plantPos.y<82)
                        {
                            plant1->setRow(5);
                        }
                        else if (plantPos.y<178)
                        {
                            plant1->setRow(4);
                        }
                        else if (plantPos.y<281)
                        {
                            plant1->setRow(3);
                        }
                        else if (plantPos.y<376)
                        {
                            plant1->setRow(2);
                        }
                        else
                        {
                            plant1->setRow(1);
                        }

                        //当前阳光数量减少
                        GameLevel::getInstance()->addSunCount(-(plant->getValue()));

                        //冷却进度条
                        menuItem->setEnabled(false);
                        auto normal = (Sprite*)item->getNormalImage();
                        auto loadFore = Sprite::createWithSpriteFrame(normal->getSpriteFrame());
                        auto timer = ProgressTimer::create(loadFore);
                        timer->setPosition(item->getPosition());
                        timer->setType(ProgressTimer::Type::RADIAL);
                        timer->setPercentage(0);
                        timer->setMidpoint(CCPoint(0.5, 0.5));
                        timer->setName(sname);
                        CCProgressFromTo* ac2 = CCProgressFromTo::create(5.f, 0, 100);
                        auto callFunc = CallFunc::create([=]() {
                            menuItem->setEnabled(true);
                            plantsColumn->removeChild(timer, true);
                            });
                        auto seq = Sequence::create(ac2, callFunc, NULL);
                        timer->runAction(seq);
                        plantsColumn->addChild(timer, 2);

                        //植物播放动画
                        /*plant->runAnimation();
                        plant->setIsFire(true);*/

                        _eventDispatcher->removeEventListenersForTarget(plant);
                        plant->removeFromParent();
                    }
                    else if (but == EventMouse::MouseButton::BUTTON_RIGHT)
                    {
                        plant->removeFromParentAndCleanup(true);
                    }
                    
                };

                pListener->onMouseMove = [=](EventMouse* event) {
                    Vec2 pos = event->getLocationInView();
                    plant->setPosition(pos);

                    Vec2 pos1 = Vec2(pos.x + 220, 600 - pos.y);
                    auto plantPos = GameLevel::getInstance()->getTiledMapCtrl()->getPlantPos(pos1);


                    if (plantPos!=Vec2::ZERO&& !GameLevel::getInstance()->getPlantLayer()->havePlant(Vec2(plantPos.x - 220, 550 - plantPos.y)))
                    {
                        plant->setOpacity(150);
                        plant->setPosition(plantPos.x - 220, 550 - plantPos.y);
                        m_bRemove = true;
                    }
                    else
                    {
                        m_bRemove = false;
                    }
                };

                _eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, plant);

            }

        }

    });

    //if (!m_vecHavePlant[m_nIndex])
    //{
    //    m_nIndex++;
    //    m_vecHavePlant[m_nIndex] = true;
    //    menuItem->setPosition(m_vecPos[m_nIndex]);
    //}

    menuItem->setPosition(m_vecPos[count]);
    menuInColumn->addChild(menuItem,2);
}

void PlantsMenu::update(float time)
{
    if (m_bIsCool)
    {
        int nSunCount = 0;
        if (GameLevel::getInstance())
        {
            nSunCount = GameLevel::getInstance()->getCurSunCount();
        }

        Vector<Node*> vecItem = menuInColumn->getChildren();
        for (int i = 0; i < vecItem.size(); i++)
        {
            auto item = (MenuItemSprite*)vecItem.at(i);
            string name = item->getNormalImage()->getName();
            auto data = CDataMgr::getInstance()->getMgr<CPlantDtMgr>("PlantDtMgr")->getDataByName(name);

            int n = 0;
            auto timer = (ProgressTimer*)plantsColumn->getChildByName(name);

            if (data && nSunCount < data->nValue)
            {
                item->setEnabled(false);
            }
            else if(data && nSunCount >= data->nValue)
            {
                //判断冷却是否完成
                if (timer&&timer->getPercentage()>=100)
                {
                    item->setEnabled(true);
                }
                else if(timer&&timer->getPercentage()<100)
                {
                    item->setEnabled(false);
                }
                else if (!timer)
                {
                    item->setEnabled(true);
                }
            }
        }
    }
}

void PlantsMenu::setMenuEnable(bool enable)
{
    Vector<Node*> vecChild = menuInColumn->getChildren();
    for (int i = 0; i < vecChild.size(); i++)
    {
        auto child = (MenuItemSprite*)vecChild.at(i);
        child->setEnabled(enable);
    }
}
