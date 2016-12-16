#ifndef __HUD_H__
#define __HUD_H__

#include "cocos2d.h"
#include "SneakyButtonSkinnedBase.h"
#include "SneakyJoystickSkinnedBase.h"
#include <string>

USING_NS_CC;
using namespace std;


class Hud : public Layer
{
public:

	virtual bool init();


	SneakyJoystick *joystick;
	SneakyButton *btnJump;
	void addJoystick(Point pos);
	void addJoystickButton(Point pos);

	CREATE_FUNC(Hud);
};

#endif // __HUD_H__