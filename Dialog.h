#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;


class Dialog : public Layer
{
public:
	float scaleOfRetry;
	virtual bool init();

	bool isLoseTheGame;
	static Dialog* create(bool isLoseTheGame);

	Sprite* background;
	Sprite* goSettingBtn;
	Sprite* resumeGameBtn;
	Sprite* exitGameBtn;
	EventListenerTouchOneByOne* _listener;

	// listener
	bool onTouchBegan(Touch *touch, Event *event);

	//CREATE_FUNC(Dialog);
};

#endif // __DIALOG_H__