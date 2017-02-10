#include "HUD.h"

bool Hud::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	auto ref = UserDefault::getInstance()->sharedUserDefault();
	/*int choice = ref->getIntegerForKey(KEY_SELECTION);*/

	//if (choice == 0) {
	
	float jtXRatio = ref->getFloatForKey(KEYJOYSTICK_X);
	float jtYRatio = ref->getFloatForKey(KEYJOYSTICK_Y);

	float jumpXRatio = ref->getFloatForKey(KEYBTNJUMP_X);
	float jumpYRatio = ref->getFloatForKey(KEYBTNJUMP_Y);

	float fireXRatio = ref->getFloatForKey(KEYBTNFIRE_X);
	float fireYRatio = ref->getFloatForKey(KEYBTNFIRE_Y);

	if (jtXRatio == NULL) {
		jtXRatio = 0.17f;
		jtYRatio = 0.20f;
		
		jumpXRatio = 0.73f;
		jumpYRatio = 0.19f;

		fireXRatio = 0.83f;
		fireYRatio = 0.38f;
	}

	addJoystick(Point(origin.x + winSize.width * jtXRatio, origin.y + winSize.height * jtYRatio));
	addJoystickButtonJump(Point(origin.x + winSize.width * jumpXRatio, origin.y + winSize.height * jumpYRatio));
	addJoystickButtonFire(Point(origin.x + winSize.width * fireXRatio, origin.y + winSize.height * fireYRatio));

	/*} else
		addButton();*/
	life_1 = Sprite::create("send/item-life.png");
	life_1->setScale(SCREEN_SIZE.height / 12 / life_1->getContentSize().height);
	life_1->setPosition(Point(origin.x + winSize.width * 0.05f, origin.y + winSize.height * 0.9f));
	addChild(life_1);


	life_2 = Sprite::create("send/item-life.png");
	life_2->setScale(SCREEN_SIZE.height / 12 / life_2->getContentSize().height);
	life_2->setPosition(Point(origin.x + winSize.width * 0.077f, origin.y + winSize.height * 0.9f));
	addChild(life_2);


	life_3 = Sprite::create("send/item-life.png");
	life_3->setScale(SCREEN_SIZE.height / 12 / life_3->getContentSize().height);
	life_3->setPosition(Point(origin.x + winSize.width * 0.104f, origin.y + winSize.height * 0.9f));
	addChild(life_3);


	life_4 = Sprite::create("send/item-life.png");
	life_4->setScale(SCREEN_SIZE.height / 12 / life_4->getContentSize().height);
	life_4->setPosition(Point(origin.x + winSize.width * 0.131f, origin.y + winSize.height * 0.9f));
	addChild(life_4);


	life_5 = Sprite::create("send/item-life.png");
	life_5->setScale(SCREEN_SIZE.height / 12 / life_5->getContentSize().height);
	life_5->setPosition(Point(origin.x + winSize.width * 0.158f, origin.y + winSize.height * 0.9f));
	addChild(life_5);

	return true;
}

void Hud::addJoystick(Point pos)
{
	auto rect_size = SCREEN_SIZE.height / 3;
	Rect joystickBaseDimensions = Rect(0, 0, rect_size, rect_size);

	SneakyJoystickSkinnedBase *joystickBase = new SneakyJoystickSkinnedBase();
	joystickBase->init();
	joystickBase->setPosition(pos);

	auto joystick_thumb = Sprite::create("send/btn-move.png");
	auto joystick_bg = Sprite::create("send/move-area.png");
	joystickBase->setThumbSprite(joystick_thumb);
	joystickBase->setBackgroundSprite(joystick_bg);
	joystickBase->setScale(SCREEN_SIZE.height / 2.4f / joystick_bg->getBoundingBox().size.height);

	joystick = new SneakyJoystick();
	joystick->initWithRect(joystickBaseDimensions);
	joystick->autorelease();
	joystick->retain();

	joystickBase->setJoystick(joystick);
	addChild(joystickBase);
}

void Hud::addJoystickButtonJump(Point pos)
{
	auto rect_size = SCREEN_SIZE.height / 8;
	Rect joystickButtonRect = Rect(0, 0, rect_size, rect_size);

	SneakyButtonSkinnedBase *joystickButtonBase = new SneakyButtonSkinnedBase();
	joystickButtonBase->init();
	auto b = Sprite::create("send/btn-jump.png");

	joystickButtonBase->setPosition(pos);
	joystickButtonBase->setDefaultSprite(b);
	joystickButtonBase->setActivatedSprite(Sprite::create("send/btn-jump.png"));
	joystickButtonBase->setPressSprite(Sprite::create("send/btn-jump2.png"));
	joystickButtonBase->setScale(SCREEN_SIZE.height / 5.0f / b->getContentSize().height);

	btnJump = new SneakyButton();
	btnJump->initWithRect(joystickButtonRect);
	btnJump->autorelease();
	btnJump->retain();


	joystickButtonBase->setButton(btnJump);

	addChild(joystickButtonBase);
}

void Hud::addJoystickButtonFire(Point pos)
{
	auto rect_size = SCREEN_SIZE.height / 8;
	Rect joystickButtonRect = Rect(0, 0, rect_size, rect_size);

	SneakyButtonSkinnedBase *joystickButtonBase = new SneakyButtonSkinnedBase();
	joystickButtonBase->init();
	auto b = Sprite::create("send/btn-shoot.png");

	joystickButtonBase->setPosition(pos);
	joystickButtonBase->setDefaultSprite(b);

	joystickButtonBase->setActivatedSprite(Sprite::create("send/btn-shoot.png"));
	joystickButtonBase->setPressSprite(Sprite::create("send/btn-shoot2.png"));
	joystickButtonBase->setScale(SCREEN_SIZE.height / 5.0f / b->getContentSize().height);

	btnFire = new SneakyButton();
	btnFire->initWithRect(joystickButtonRect);
	btnFire->autorelease();
	btnFire->retain();


	joystickButtonBase->setButton(btnFire);

	addChild(joystickButtonBase);
}

//void Hud::addButton()
//{
//	btnLeft = Button::create("send/move-left.png", Point(SCREEN_SIZE.width * 0.15f, SCREEN_SIZE.height * 0.165f));
//
//	btnLeft->setScale(SCREEN_SIZE.height / 5.5f / btnLeft->getContentSize().height);
//	addChild(btnLeft);
//
//
//	btnRight = Button::create("send/move-right.png", Point(SCREEN_SIZE.width * 0.33f, SCREEN_SIZE.height * 0.165f));
//	btnRight->setScale(SCREEN_SIZE.height / 5.5f / btnLeft->getContentSize().height);
//	addChild(btnRight);
//}



