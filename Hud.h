#ifndef __HUD_H__
#define __HUD_H__

#include "Global.h"
#include "SneakyButtonSkinnedBase.h"
#include "SneakyJoystickSkinnedBase.h"
#include "Button.h"



class Hud : public Layer
{
public:

	virtual bool init();
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	SneakyJoystick *joystick;
	SneakyButton *btnJump;

	Button *btnLeft;
	Button *btnRight;

	void addJoystick(Point pos);
	void addJoystickButton(Point pos);

	void addButton();

	CREATE_FUNC(Hud);
};

#endif // __HUD_H__