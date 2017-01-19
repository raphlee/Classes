#include "StartScene.h"
#include "SimpleAudioEngine.h"


Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
	
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add Start sprite
	auto backGround = Sprite::create("background.png");
	backGround->setScaleX(visibleSize.width / backGround->getContentSize().width);
	backGround->setScaleY(visibleSize.height / backGround->getContentSize().height);
	backGround->setPosition(origin + visibleSize / 2);
	addChild(backGround);

	// cache frame
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map1.plist");
	Director::getInstance()->getTextureCache()->addImage("box/lid.png");

	// sound and effect
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/backGroundMenu.mp3");

	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/riverSound.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/frogSound.mp3");		// touch me
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/hippo.wav");			// attack
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/leafSound.mp3");		// on me
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/sound-button.mp3");		// 

	//CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.75f);
	//CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.75f);

	auto ground = Sprite::createWithSpriteFrameName("ground.png");
	ground->setAnchorPoint(Vec2(0.5, 0));
	ground->setPosition(origin.x + visibleSize.width / 2, origin.y);
	ground->setScale(visibleSize.width / ground->getContentSize().width);
	addChild(ground);

	auto rambo = Sprite::createWithSpriteFrameName("title-ramball.png");
	rambo->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.65f);
	rambo->setScale(visibleSize.width * 0.4f / rambo->getContentSize().width);
	addChild(rambo);

	auto skeleton = SkeletonAnimation::createWithFile("soldier/soldier.json", "soldier/soldier.atlas", visibleSize.height / 5.0f / 182.0f);
	skeleton->setPosition(origin.x + visibleSize.width / 2, origin.y + ground->getBoundingBox().size.height);
	skeleton->addAnimation(0, "running", true);
	addChild(skeleton);

	play = Sprite::createWithSpriteFrameName("btn-play.png");
	play->setScale(visibleSize.height / 5.0f / play->getContentSize().height);
	auto posX = visibleSize.width / 2 + rambo->getBoundingBox().size.width / 2 + play->getBoundingBox().size.width * 0.7f;
	play->setPosition(origin.x + posX, origin.y + visibleSize.height * 0.65f);
	addChild(play);

	setting = Sprite::create("send/setting.png");
	setting->setScale(visibleSize.height / 5.0f / setting->getContentSize().height);
	setting->setPosition(origin.x + visibleSize.width * 0.1f, origin.y + visibleSize.height * 0.85f);
	addChild(setting);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StartScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

bool StartScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (play->getBoundingBox().containsPoint(touch->getLocation())) {
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	if (setting->getBoundingBox().containsPoint(touch->getLocation())) {
		Director::getInstance()->replaceScene(ControlSettingScene::createScene());
	}

	return false;
}
