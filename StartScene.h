#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "GameScene.h"
#include "ControlSettingScene.h"



USING_NS_CC;
using namespace spine;

class StartScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	Sprite *play;
	Sprite *setting;

	Sprite *gp1;
	Sprite *gp2;
	Sprite *gp3;
	Sprite *gp4;

	Sprite *grass1;
	Sprite *grass2;
	Sprite *grass3;
	Action * action_1;
	Action * action_2;
	Action * action_3;

	Sprite *soundOn;
	Sprite *soundOff;
	int backgroudSoundID;
	UserDefault *ref;

	void createAction();
	void onEnter();

	bool onTouchBegan(Touch *touch, Event *unused_event);

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);
};

#endif // __START_SCENE_H__