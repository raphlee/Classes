#ifndef __MODIFY_JOYSTICK_POS_SCENE_H__
#define __MODIFY_JOYSTICK_POS_SCENE_H__

#include "cocos2d.h"
#include "Global.h"
#include "GameScene.h"


class ModifyJoystickPosScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	Sprite *joystick;
	Sprite *submit;
	UserDefault *reference;

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

    // implement the "static create()" method manually
    CREATE_FUNC(ModifyJoystickPosScene);
};

#endif // __MODIFY_JOYSTICK_POS_SCENE_H__
