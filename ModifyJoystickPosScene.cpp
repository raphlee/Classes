#include "ModifyJoystickPosScene.h"
#include "SimpleAudioEngine.h"


Scene* ModifyJoystickPosScene::createScene()
{

    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = ModifyJoystickPosScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ModifyJoystickPosScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("You can change joystick's position", "fonts/Marker Felt.ttf", 200);
	label->setScale(SCREEN_SIZE.height / 14.0f / label->getBoundingBox().size.height);
	label->setColor(Color3B::WHITE);
	label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.83f);
	addChild(label);
	
	reference = UserDefault::sharedUserDefault();
	
	float jtXRatio = reference->getFloatForKey(KEYJOYSTICK_X);
	float jtYRatio = reference->getFloatForKey(KEYJOYSTICK_Y);

	joystick = Sprite::create("send/joystick.png");
	joystick->setScale(SCREEN_SIZE.height / 2.4f / joystick->getContentSize().height);

	if(jtXRatio == NULL)
		joystick->setPosition(origin.x + visibleSize.width * 0.17f, origin.y + visibleSize.height * 0.2f);
	else
		joystick->setPosition(origin.x + visibleSize.width * jtXRatio, origin.y + visibleSize.height * jtYRatio);

	addChild(joystick, 1);

	submit =  Sprite::create("send/ok-button.png");
	submit->setScale(SCREEN_SIZE.width / 4.0f / submit->getBoundingBox().size.width);
	submit->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(submit);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ModifyJoystickPosScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ModifyJoystickPosScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ModifyJoystickPosScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

bool ModifyJoystickPosScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	auto location = touch->getLocation();
	convertToNodeSpace(location);
	if (joystick->getBoundingBox().containsPoint(location)) {
		return true;
	}

	if (submit->getBoundingBox().containsPoint(location)) {
		if (reference->getFloatForKey(KEYJOYSTICK_X) == NULL) {

			reference->setFloatForKey(KEYJOYSTICK_X, joystick->getPositionX() / SCREEN_SIZE.width);
			reference->setFloatForKey(KEYJOYSTICK_Y, joystick->getPositionY() / SCREEN_SIZE.height);
		}

		Director::getInstance()->replaceScene(GameScene::createScene());
	}

	return false;
}

void ModifyJoystickPosScene::onTouchMoved(Touch * touch, Event * unused_event)
{
	auto location = touch->getLocation();
	convertToNodeSpace(location);

	if (joystick->getBoundingBox().containsPoint(location)) {
		joystick->setPosition(location);
	}
}

void ModifyJoystickPosScene::onTouchEnded(Touch * touch, Event * unused_event)
{
	auto location = touch->getLocation();
	convertToNodeSpace(location);

	if (joystick->getBoundingBox().containsPoint(location)) {
		
		reference->setFloatForKey(KEYJOYSTICK_X, joystick->getPositionX() / SCREEN_SIZE.width);
		reference->setFloatForKey(KEYJOYSTICK_Y, joystick->getPositionY() / SCREEN_SIZE.height);
	}
}


