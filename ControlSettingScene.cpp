#include "ControlSettingScene.h"
#include "SimpleAudioEngine.h"


Scene* ControlSettingScene::createScene()
{

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ControlSettingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ControlSettingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("Control Setting", "fonts/Marker Felt.ttf", 200);
	label->setScale(visibleSize.height / 14.0f / label->getBoundingBox().size.height);
	label->setColor(Color3B::WHITE);
	label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.83f);
	addChild(label);

	reference = UserDefault::getInstance()->sharedUserDefault();

	auto jtXRatio = reference->getFloatForKey(KEYJOYSTICK_X);
	auto jtYRatio = reference->getFloatForKey(KEYJOYSTICK_Y);

	auto jumpXRatio = reference->getFloatForKey(KEYBTNJUMP_X);
	auto jumpYRatio = reference->getFloatForKey(KEYBTNJUMP_Y);

	auto fireXRatio = reference->getFloatForKey(KEYBTNFIRE_X);
	auto fireYRatio = reference->getFloatForKey(KEYBTNFIRE_Y);

	joystick = Sprite::create("send/joystick.png");
	joystick->setScale(visibleSize.height / 2.4f / joystick->getContentSize().height);

	btnJump = Sprite::create("send/btn-jump.png");
	btnJump->setScale(visibleSize.height / 5.0f / btnJump->getContentSize().height);

	btnFire = Sprite::create("send/btn-shoot.png");
	btnFire->setScale(visibleSize.height / 5.0f / btnFire->getContentSize().height);

	if (jtXRatio == NULL) {
		joystick->setPosition(visibleSize.width * 0.17f, visibleSize.height * 0.2f);
		btnJump->setPosition(visibleSize.width * 0.73f, visibleSize.height * 0.19f);
		btnFire->setPosition(visibleSize.width * 0.87f, visibleSize.height * 0.38f);
	}
	else {
		joystick->setPosition(jtXRatio, jtYRatio);
		btnJump->setPosition(jumpXRatio, jumpYRatio);
		btnFire->setPosition(fireXRatio, fireYRatio);
	}

	addChild(joystick, 1);
	addChild(btnJump, 1);
	addChild(btnFire, 1);

	submit = Sprite::create("send/ok-button.png");
	submit->setScale(visibleSize.width / 4.0f / submit->getBoundingBox().size.width);
	submit->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(submit);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ControlSettingScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ControlSettingScene::onTouchMoved, this);
	//listener->onTouchEnded = CC_CALLBACK_2(ControlSettingScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool ControlSettingScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	auto location = touch->getLocation();
	//convertToNodeSpace(location);
	if (joystick->getBoundingBox().containsPoint(location) ||
		btnFire->getBoundingBox().containsPoint(location) ||
		btnJump->getBoundingBox().containsPoint(location)
		) {
		return true;
	}

	if (submit->getBoundingBox().containsPoint(location)) {

		reference->setFloatForKey(KEYJOYSTICK_X, joystick->getPositionX());
		reference->setFloatForKey(KEYJOYSTICK_Y, joystick->getPositionY());

		reference->setFloatForKey(KEYBTNJUMP_X, btnJump->getPositionX());
		reference->setFloatForKey(KEYBTNJUMP_Y, btnJump->getPositionY());

		reference->setFloatForKey(KEYBTNFIRE_X, btnFire->getPositionX());
		reference->setFloatForKey(KEYBTNFIRE_Y, btnFire->getPositionY());


		Director::getInstance()->replaceScene(TransitionFade::create(0.67f, GameScene::createScene()));
	}

	return false;
}

void ControlSettingScene::onTouchMoved(Touch * touch, Event * unused_event)
{
	auto location = touch->getLocation();
	//convertToNodeSpace(location);

	if (joystick->getBoundingBox().containsPoint(location)) {
		joystick->setPosition(location);
	}
	else if (btnJump->getBoundingBox().containsPoint(location)) {
		btnJump->setPosition(location);
	}
	else if (btnFire->getBoundingBox().containsPoint(location)) {
		btnFire->setPosition(location);
	}
}

//void ControlSettingScene::onTouchEnded(Touch * touch, Event * unused_event)
//{
//	auto location = touch->getLocation();
//	convertToNodeSpace(location);
//
//	if (joystick->getBoundingBox().containsPoint(location)) {
//		
//		reference->setFloatForKey(KEYJOYSTICK_X, joystick->getPositionX());
//		reference->setFloatForKey(KEYJOYSTICK_Y, joystick->getPositionY());
//	}
//
//	if (btnJump->getBoundingBox().containsPoint(location)) {
//
//		reference->setFloatForKey(KEYBTNJUMP_X, btnJump->getPositionX());
//		reference->setFloatForKey(KEYBTNJUMP_Y, btnJump->getPositionY());
//	}
//
//	if (btnFire->getBoundingBox().containsPoint(location)) {
//
//		reference->setFloatForKey(KEYBTNFIRE_X, btnFire->getPositionX());
//		reference->setFloatForKey(KEYBTNFIRE_Y, btnFire->getPositionY());
//	}
//}


