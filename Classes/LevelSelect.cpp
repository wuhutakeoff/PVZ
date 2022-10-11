#include "LevelSelect.h"
#include "Tool.h"
#include "GameLevel.h"
#include "DataStruct.h"

LevelSelect::LevelSelect()
{
}

LevelSelect::~LevelSelect()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool LevelSelect::init()
{
	if (!Node::init())
	{
		return false;
	}

	addTouch();

	addPageView();

	Button* pButton = Button::create("Resources/userBtn.png", "Resources/userBtn2.png");
	pButton->setTitleText(CTool::FontToUTF8("进入游戏"));
	pButton->setTitleFontSize(30);
	pButton->setPositionY(-250);
	this->addChild(pButton);
	pButton->addClickEventListener([=](Ref* pSender) {
		//点击按钮的时候要获取当前的关卡选择页面是在哪个页面。 获取页面索引
		int nIndex = m_pPageView->getCurrentPageIndex();
		nIndex = nIndex < 0 ? 0 : nIndex;
		CDataMgr::getInstance()->getMgr<CLevelDtMgr>("LevelDtMgr")->setCurIndex(nIndex);
		CCLOG("index:%d", nIndex);
		//进入游戏主场景。
  		TransitionFade* pFade = TransitionFade::create(1.0, GameLevel::getInstance());
		//TransitionFade* pFade = TransitionFade::create(1.0, GameLevel::create());
		Director::getInstance()->replaceScene(pFade);
		});

	return true;
}

void LevelSelect::addPageView()
{
	m_pPageView = PageView::create();
	m_pPageView->setContentSize(Size(800, 400));

	int size=CDataMgr::getInstance()->getMgr<CLevelDtMgr>("LevelDtMgr")->getSize();
	for (int i = 0; i <size ; i++)
	{
		//层背景
		Layout* pLayout = Layout::create();
		auto bg = Sprite::create("Resources/Image/Background/blackBg.png");
		bg->setContentSize(Size(800, 400));
		bg->setPosition(400, 200);
		pLayout->addChild(bg);

		auto levelDt = CDataMgr::getInstance()->getMgr<CLevelDtMgr>("LevelDtMgr")->getDataByID<CLevelDt>(1001+i);
		//std::string strPath = StringUtils::format("Resources/Image/Background/PVZBackground_%d.jpg", i);
		Sprite* pSprite = Sprite::create(levelDt->strMap);
		pSprite->setPosition(400, 200);
		pSprite->setScale(0.5);
		pLayout->addChild(pSprite);

		m_pPageView->addChild(pLayout);
	}
	m_pPageView->setPosition(Vec2(-400, -200));
	this->addChild(m_pPageView);
}

void LevelSelect::addTouch()
{
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent) {
		return true;
	};
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}
