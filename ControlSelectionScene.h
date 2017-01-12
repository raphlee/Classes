#ifndef __CONTROL_SELECTION_SCENE_H__
#define __CONTROL_SELECTION_SCENE_H__

#include "cocos2d.h"
#include "ModifyJoystickPosScene.h"


class ControlSelectionScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	Sprite *joystick;
	Sprite *button;
	UserDefault *reference;

	bool onTouchBegan(Touch *touch, Event *unused_event);

    // implement the "static create()" method manually
    CREATE_FUNC(ControlSelectionScene);
};

#endif // __CONTROL_SELECTION_SCENE_H__
