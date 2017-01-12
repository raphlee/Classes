#include "ControlSelectionScene.h"
#include "SimpleAudioEngine.h"


Scene* ControlSelectionScene::createScene()
{

    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = ControlSelectionScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ControlSelectionScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto whiteBg = DrawNode::create();
	whiteBg->drawSolidRect(origin, Vec2(visibleSize.width + origin.x, visibleSize.height + origin.y),
		Color4F::WHITE);
	addChild(whiteBg);

	auto controlLabel = Label::createWithTTF("Select one of two controllers below", "fonts/Marker Felt.ttf", 200);
	controlLabel->setScale(SCREEN_SIZE.height / 12.0f / controlLabel->getBoundingBox().size.height);
	controlLabel->setColor(Color3B::BLUE);
	controlLabel->setPosition(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.83f);
	addChild(controlLabel);

	joystick = Sprite::create("send/joystick-selection.png");
	joystick->setScale(SCREEN_SIZE.height / 2.5f / joystick->getContentSize().height);
	joystick->setPosition(origin.x + visibleSize.width * 0.3f, origin.y + visibleSize.height / 2);
	addChild(joystick);

	auto labelJoystick = Label::createWithTTF("Joystick", "fonts/Marker Felt.ttf", 200);
	labelJoystick->setScale(SCREEN_SIZE.height / 14.0f / labelJoystick->getBoundingBox().size.height);
	labelJoystick->setColor(Color3B::ORANGE);
	labelJoystick->setPosition(origin.x + visibleSize.width * 0.3f, origin.y + visibleSize.height * 0.23f);
	addChild(labelJoystick);

	button = Sprite::create("send/move-selection.png");
	button->setScale(SCREEN_SIZE.height / 2.5f / button->getContentSize().height);
	button->setPosition(origin.x + visibleSize.width * 0.7f, origin.y + visibleSize.height / 2);
	addChild(button);

	auto labelButton = Label::createWithTTF("Button", "fonts/Marker Felt.ttf", 200);
	labelButton->setScale(SCREEN_SIZE.height / 14.0f / labelButton->getBoundingBox().size.height);
	labelButton->setColor(Color3B::GRAY);
	labelButton->setPosition(origin.x + visibleSize.width * 0.7f, origin.y + visibleSize.height * 0.23f);
	addChild(labelButton);

	reference = UserDefault::sharedUserDefault();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ControlSelectionScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

bool ControlSelectionScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	auto location = touch->getLocation();
	int choice;
	convertToNodeSpace(location);
	if (joystick->getBoundingBox().containsPoint(location)) {
		choice = 0;
		reference->setIntegerForKey(KEY_SELECTION, choice);
		//Director::getInstance()->replaceScene(TransitionFade::create(0.67f, GameScene::createScene()));
		Director::getInstance()->replaceScene(GameScene::createScene());
	}

	else if (button->getBoundingBox().containsPoint(location)) {
		choice = 1;
		reference->setIntegerForKey(KEY_SELECTION, choice);
		//Director::getInstance()->replaceScene(TransitionFade::create(0.67f, GameScene::createScene()));
		Director::getInstance()->replaceScene(GameScene::createScene());
	}

	return false;
}


