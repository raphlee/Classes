#ifndef __HUD_H__
#define __HUD_H__

#include "Global.h"
#include "SneakyButtonSkinnedBase.h"
#include "SneakyJoystickSkinnedBase.h"
//#include "Button.h"



class Hud : public Layer
{
public:

	virtual bool init();
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	SneakyJoystick *joystick;
	SneakyButton *btnJump;
	SneakyButton *btnFire;

	Sprite *life_1;
	Sprite *life_2;
	Sprite *life_3;
	Sprite *life_4;
	Sprite *life_5;

	//Button *btnLeft;
	//Button *btnRight;

	void addJoystick(Point pos);
	void addJoystickButtonJump(Point pos);
	void addJoystickButtonFire(Point pos);

	//void addButton();

	CREATE_FUNC(Hud);
};

#endif // __HUD_H__