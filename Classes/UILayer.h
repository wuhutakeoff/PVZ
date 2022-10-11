#ifndef __UILAYER_H__
#define	__UILAYER_H__
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class CUILayer : public Node
{
public:
	CUILayer();
	~CUILayer();
	virtual bool init();
	void startGame();
	void showProgress();
	void showLastWave();
	void showLargeWave();
	void showDefeat();

	virtual void update(float fDetal);


	CREATE_FUNC(CUILayer);

	CC_SYNTHESIZE(int, m_nPercent, Percent);
	CC_SYNTHESIZE(bool, m_bChangeProgress, ChangeProgress);
private:
	int m_nCurCount;
	ProgressTimer* progress;
	int m_nCurPercent;
	float m_fTime;
};
#endif