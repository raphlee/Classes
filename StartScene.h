#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "GameScene.h"
#include "ControlSelectionScene.h"



USING_NS_CC;
using namespace spine;

class StartScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	Sprite *play;

	bool onTouchBegan(Touch *touch, Event *unused_event);

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);
};

#endif // __START_SCENE_H__