#include "HUD.h"

bool Hud::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize();


	addJoystick(Point(winSize.width * 0.17f, winSize.height * 0.17f));
	addJoystickButton(Point(winSize.width * 0.85f, winSize.height * 0.19f));

	return true;
}

void Hud::addJoystick(Point pos)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto rect_size = visibleSize.height / 3;
	Rect joystickBaseDimensions = Rect(0, 0, rect_size, rect_size);

	SneakyJoystickSkinnedBase *joystickBase = new SneakyJoystickSkinnedBase();
	joystickBase->init();
	joystickBase->setPosition(pos);

	auto joystick_thumb = Sprite::create("send/btn-move.png");
	auto joystick_bg = Sprite::create("send/move-area.png");
	joystickBase->setThumbSprite(joystick_thumb);
	joystickBase->setBackgroundSprite(joystick_bg);
	joystickBase->setScale(visibleSize.height / 3.0 / joystick_bg->getBoundingBox().size.height);

	joystick = new SneakyJoystick();
	joystick->initWithRect(joystickBaseDimensions);
	joystick->autorelease();
	joystick->retain();

	joystickBase->setJoystick(joystick);
	addChild(joystickBase);
}

void Hud::addJoystickButton(Point pos)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto rect_size = visibleSize.height / 8;
	Rect joystickButtonRect = Rect(0, 0, rect_size, rect_size);

	SneakyButtonSkinnedBase *joystickButtonBase = new SneakyButtonSkinnedBase();
	joystickButtonBase->init();
	auto b = Sprite::create("send/btn-jump.png");
	
	joystickButtonBase->setPosition(pos);
	joystickButtonBase->setDefaultSprite(b);
	joystickButtonBase->setActivatedSprite(Sprite::create("send/btn-jump.png"));
	joystickButtonBase->setPressSprite(Sprite::create("send/btn-jump.png"));
	joystickButtonBase->setScale(visibleSize.height / 6.0f / b->getContentSize().height);

	btnJump = new SneakyButton();
	btnJump->initWithRect(joystickButtonRect);
	btnJump->autorelease();
	btnJump->retain();
	

	joystickButtonBase->setButton(btnJump);

	addChild(joystickButtonBase);
}




