#include "GameLevel.h"
#include "DataStruct.h"
#include "Tool.h"
GameLevel* GameLevel::m_spInstance = nullptr;

GameLevel::GameLevel()
{
    SunFlowerSprite = nullptr;
    m_nCurSunCount = 0;
    m_bRemove = false;
}

GameLevel::~GameLevel()
{
}

bool GameLevel::init()
{
	if (!Scene::init())
	{
		return false;
	}
    
    m_nCurSunCount = 500;

    //UI层
    m_pUILayer = CUILayer::create();
    this->addChild(m_pUILayer, 3);

    //子弹层
    m_pBulletLayer = CBulletLayer::create();
    this->addChild(m_pBulletLayer,2);

    //植物层
    m_pPlantLayer = CPlantsLayer::create();
    this->addChild(m_pPlantLayer, 2);


    //读取僵尸动作plist
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/BoomDie.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/EnemyDeath.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/Zombie.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/ZombieAttack.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/ZombieLostHead.plist");

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/FlagZombieLostHead.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/FlagZombie.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/FlagZombieAttack.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Zombie/FlagZombieLostHeadAttack.plist");

    Size winSize = Director::getInstance()->getWinSize();
    //读取数据
    CLevelDt* pCurLevelDt = CDataMgr::getInstance()->getMgr<CLevelDtMgr>("LevelDtMgr")->getCurLevelDt();

    //大地图

    m_pTiledMapCtrl = CTiledMapCtrl::createWithData(pCurLevelDt);
    this->addChild(m_pTiledMapCtrl);
    m_pTiledMapCtrl->setIgnoreAnchorPointForPosition(true);
    m_pTiledMapCtrl->setAnchorPoint(Size(0, 0));
    m_pTiledMapCtrl->setPosition(0, 0);
    auto delay = DelayTime::create(2.0f);
    auto moveTo = MoveTo::create(2.0f, Vec2(-500, 0));
    auto seq = Sequence::create(delay, moveTo, NULL);
    m_pTiledMapCtrl->runAction(seq);
    m_pTiledMapCtrl->setName("bg1");


 //   bg1 = Sprite::create(pCurLevelDt->strMap);
 //   bg1->setAnchorPoint(Size(0, 0));
	//bg1->setPosition(0, 0);
 //   auto delay = DelayTime::create(2.0f);
 //   auto moveTo = MoveTo::create(2.0f, Vec2(-500, 0));
 //   auto seq = Sequence::create(delay, moveTo,NULL);
 //   bg1->runAction(seq);
 //   bg1->setName("bg1");
	//this->addChild(bg1);


    //植物菜单层
    plantsMenu = PlantsMenu::create();
    plantsMenu->setName("plantsMenu");
    this->addChild(plantsMenu);



    //设置按钮
    Button* pButton = Button::create("Resources/userBtn.png", "Resources/userBtn2.png");
    pButton->setTitleText(CTool::FontToUTF8("菜单"));
    pButton->setTitleFontSize(30);
    pButton->setPosition(Vec2(winSize.width -85, winSize.height -25));
    this->addChild(pButton);
    pButton->addClickEventListener([=](Ref* pSender) {
        auto optionLayer = OptionLayer::create();
        optionLayer->setShowLongBtn(true);
        optionLayer->setPosition(winSize.width / 2, winSize.height / 2 + 200);
        auto moveTo = MoveTo::create(1.0f, Vec2(winSize.width / 2, winSize.height / 2));
        auto ease = EaseBackInOut::create(moveTo);
        optionLayer->runAction(ease);
        this->addChild(optionLayer,3);
        });

    m_pZombieMgr = CZombieMgr::create();
    m_pTiledMapCtrl->addChild(m_pZombieMgr);

    //铲子
    shovelBack = Sprite::create("ShovelBack.png");
    shovelBack->setPosition(665, 570);
    shovelBack->setVisible(false);
    this->addChild(shovelBack);

    shovel = Button::create("Shovel.png");
    shovel->setPosition(Vec2(665, 570));
    shovel->setVisible(false);
    this->addChild(shovel);    

    m_pTrolleyLayer = CTrolleyLayer::create();
    m_pTiledMapCtrl->addChild(m_pTrolleyLayer,2);

    scheduleUpdate();

	return true;
}

GameLevel* GameLevel::getInstance()
{
    if (!m_spInstance)
    {
        m_spInstance = GameLevel::create();
    }
    return m_spInstance;

}

void GameLevel::update(float dt)
{
    //選擇植物界面完成
    if (plantsMenu->getisFinish())
    {
        auto moveTo = MoveTo::create(1.0f, Vec2(-220, 0));
        auto delay = DelayTime::create(1.0f);
        auto callFunc = CallFunc::create([=]() {
            m_pUILayer->startGame();
            plantsMenu->setMenuEnable(true);
            m_pUILayer->showProgress();
            m_pUILayer->setChangeProgress(true);
            });
        auto seq = Sequence::create(delay, moveTo, callFunc, NULL);
        m_pTiledMapCtrl->runAction(seq);


        plantsMenu->setisFinish(false);

        auto delay1 = DelayTime::create(5.f);
        auto pCall = CallFunc::create([=](){
            m_pZombieMgr->setIsCreateZombie(true);
            });
        m_pZombieMgr->runAction(Sequence::createWithTwoActions(delay1,pCall));


        shovel->setVisible(true);
        shovelBack->setVisible(true);
        shovel->addClickEventListener(CC_CALLBACK_0(GameLevel::clickShovel, this));



        //開始生產陽光
        this->produceSun();
    }
   

    if (plantsMenu->getChildByName("plantsColumn"))
    {
        plantsMenu->setSunCount(m_nCurSunCount);
    }
    

    Vector<Node*> vecBullet = m_pBulletLayer->getChildren();
    Vector<Node*> vecZombie = m_pZombieMgr->getChildren();
    for (int i = 0; i < vecBullet.size(); i++)
    {
        for (int j = 0; j < vecZombie.size(); j++)
        {
            auto pos = vecZombie.at(j)->convertToWorldSpaceAR(Vec2::ZERO);
            auto box = vecZombie.at(j)->getBoundingBox();
            pos.x += 30;
            pos.y -= 50;
            box.setRect(pos.x, pos.y, 75, 115);

            if (vecBullet.at(i) && box.containsPoint(vecBullet.at(i)->getPosition()))
            {
                //产生伤害
                auto zombie=(CZombie*)vecZombie.at(j);
                auto bullet = (CBullet*)vecBullet.at(i);
                zombie->onDamage(bullet->getDamge());
                
                //如果是寒冰豌豆
                if (bullet->getType()=="slow")
                {
                    zombie->changeState("slow");
                }

                //删除BulletLayer容器中的子弹
                vecBullet.at(i)->removeFromParent();
                break;
            }
        }
    }

    Vector<Node*> vecPlant = m_pPlantLayer->getChildren();
    for (int j = 0; j < vecZombie.size(); j++)
    {
        auto zombie = (CZombie*)vecZombie.at(j);

        if (vecPlant.size() <= 0)
        {
            zombie->setPlantTag(-1);
        }
        for (int i = 0; i < vecPlant.size(); i++)
        {

            /*auto pos1 = vecZombie.at(j)->getPosition();
            pos1.x -= 190;

            auto pos2 = vecPlant.at(i)->convertToWorldSpaceAR(Vec2::ZERO);
            Rect box2 = vecPlant.at(i)->getBoundingBox();
            box2.origin = pos2;*/

            auto pos1=vecZombie.at(j)->getPosition();
            pos1.x -= 190;
            auto box1=vecZombie.at(j)->getBoundingBox();
            box1.origin = pos1;
            box1.size=Size(box1.size.width / 2, box1.size.height / 2);


            auto pos2 = vecPlant.at(i)->getPosition();
            auto box2 = vecPlant.at(i)->getBoundingBox();
            box2.origin = pos2;

            if (box2.intersectsRect(box1))
            {
                zombie->changeState("attack");
                zombie->setPlantTag(vecPlant.at(i)->getTag());
                break;
            }
            else if (i==vecPlant.size()-1)
            {
                zombie->setPlantTag(-1);
            }
        }
    }

    Vector<Node*> vecTrolley=m_pTrolleyLayer->getChildren();
    for (int i = 0; i < vecTrolley.size(); i++)
    {
        for (int j = 0; j < vecZombie.size(); j++)
        {
            auto box = vecTrolley.at(i)->getBoundingBox();
            box.size = Size(box.size.width / 2, box.size.height / 2);

            auto box1 = vecZombie.at(j)->getBoundingBox();
            box1.size = Size(box1.size.width / 2, box1.size.height / 2);
            if (box.intersectsRect(box1))
            {
                auto moveTo = MoveTo::create(10.f, Vec2(1200, vecTrolley.at(i)->getPositionY()));
                auto callFunc = CallFunc::create([=]() {vecTrolley.at(i)->removeFromParent(); });
                vecTrolley.at(i)->runAction(Sequence::createWithTwoActions(moveTo, callFunc));
                auto zombie = (CZombie*)vecZombie.at(j);
                zombie->changeState("lostDie", [=]() {
                    zombie->removeFromParent();
                    });
            }
        }
    }
}

void GameLevel::scheduleCallback(float fd)
{

}

void GameLevel::deletePlant(CPlant* plant)
{
    Vector<Node*> vecPlant = m_pPlantLayer->getChildren();

    for (int i = 0; i < vecPlant.size(); i++)
    {
        if (vecPlant.at(i) == plant)
        {
            vecPlant.at(i)->removeFromParent();
            vecPlant.erase(vecPlant.begin() + i);
        }
    }
}

void GameLevel::produceSun()
{
    this->schedule([=](float) {

        std::default_random_engine e;
        std::uniform_int_distribution<int> u(100, 800);
        std::uniform_int_distribution<int> v(200, 500);// 左闭右闭区间
        e.seed(time(0));

        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/Image/Sun.plist");
        auto sun = Sprite::createWithSpriteFrameName("Sun_0.png");
        Vec2 pos = Vec2(u(e), v(e));

        sun->setPosition(pos.x,600);
        this->addChild(sun);


        Animation* pAnimation = Animation::create();
        for (int i = 0; i < 22; i++)
        {
            string name = StringUtils::format("Sun_%d.png", i);
            SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
            pAnimation->addSpriteFrame(pFrame);
        }
        pAnimation->setDelayPerUnit(0.1f);

        auto animate = Animate::create(pAnimation);
        auto moveTo = MoveTo::create((600-pos.y) / 100, pos);

        ActionInterval* pAction = RepeatForever::create(animate);
        auto sp = Spawn::create(animate, moveTo, NULL);
        auto delay = DelayTime::create(3.0f);
        auto callFunc = CallFunc::create([=]() {
            sun->removeFromParent();
            });
        auto seq = Sequence::create(sp, delay, callFunc, NULL);
        sun->runAction(seq);

        EventListenerMouse* pListener = EventListenerMouse::create();
        pListener->onMouseDown = [=](EventMouse* event)
        {
            auto but = event->getMouseButton();
            auto box = sun->getBoundingBox();
            Vec2 pos = event->getLocationInView();

            if (but == EventMouse::MouseButton::BUTTON_LEFT && box.containsPoint(pos))
            {
                sun->stopAllActions();
                auto moveTo = MoveTo::create(1.f, Vec2(42, 565));
                auto callFunc = CallFunc::create([=]() {
                    //_eventDispatcher->removeEventListenersForTarget(sun);
                    sun->removeFromParentAndCleanup(true);
                    GameLevel::getInstance()->addSunCount(25);
                    });
                auto seq = Sequence::create(moveTo, callFunc, NULL);
                sun->runAction(seq);
            }
        };
        auto even = (Event*)pListener;
        _eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, sun);

    }, 5.f, "produceSun");

}

void GameLevel::clickShovel()
{
    Vector<Node*> vecPlant = m_pPlantLayer->getChildren();

    EventListenerMouse* pListener = EventListenerMouse::create();
    pListener->onMouseDown = [=](EventMouse* event) {
        auto but = event->getMouseButton();
        if (but == EventMouse::MouseButton::BUTTON_LEFT)
        {
            if (shovel->getBoundingBox().containsPoint(event->getLocationInView()))
            {
                m_bRemove = false;
            }
            //铲子碰植物
            for (int i = 0; i < vecPlant.size(); i++)
            {
                Rect rc = vecPlant.at(i)->getBoundingBox();
                if (rc.containsPoint(event->getLocationInView())&&!m_bRemove)
                {
                    vecPlant.at(i)->removeFromParent();

                    m_bRemove = true;
                    shovel->setPosition(shovelBack->getPosition());
                    _eventDispatcher->removeEventListener(pListener);
                }
            }
        }
        else if (but == EventMouse::MouseButton::BUTTON_RIGHT)
        {
            //右键取消
            shovel->setPosition(shovelBack->getPosition());
            _eventDispatcher->removeEventListener(pListener);
        }
    };

    pListener->onMouseMove = [=](EventMouse* event) {
        if (m_bRemove)
        {
            return;
        }
        Vec2 pos = event->getLocationInView(); 
        shovel->setPosition(pos);
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, shovel);
}

void GameLevel::deleteAll()
{
    m_pPlantLayer->removeFromParent();
}
