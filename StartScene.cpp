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

	ref = UserDefault::getInstance()->sharedUserDefault();
	ref->setIntegerForKey(KEY_CHOICE, 1);

	// sound and effect
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/boss_explosion.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/cannon shoot.mp3");		// touch me
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy bullet.mp3");			// attack
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy_bomb.mp3");		// on me
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy_bomb_explosion.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/F bullet.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/FormatFactoryplane_bomb.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/get item.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/helicopter.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/machine gun.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/missle.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/normal bullet.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/plane_bomb.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/plane_drop.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/S Bullet.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/sound_lose.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/tank move.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/tank shoot .mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/tank_explosion.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/theme_music.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/transform.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/transform_2.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Win.mp3");
	//
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.75f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.75f);

	auto labelVer = Label::createWithTTF("2017-02-10-Build 1", "fonts/Marker Felt.ttf", 200);
	labelVer->setScale(visibleSize.width / 7.5f / labelVer->getContentSize().width);
	labelVer->setPosition(origin.x + visibleSize.width * 0.90f, origin.y + visibleSize.height * 0.96f);
	addChild(labelVer);
				

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

	gp1 = Sprite::create("send/GP1.png");
	gp1->setScale(visibleSize.height / 9.0f / gp1->getContentSize().height);
	gp1->setPosition(origin.x + visibleSize.width * 0.15f, origin.y + visibleSize.height * 0.4f);
	addChild(gp1);

	gp2 = Sprite::create("send/GP2.png");
	gp2->setScale(visibleSize.height / 9.0f / gp2->getContentSize().height);
	gp2->setPosition(origin.x + visibleSize.width * 0.3f, origin.y + visibleSize.height * 0.4f);
	addChild(gp2);

	gp3 = Sprite::create("send/GP3.png");
	gp3->setScale(visibleSize.height / 9.0f / gp3->getContentSize().height);
	gp3->setPosition(origin.x + visibleSize.width * 0.7f, origin.y + visibleSize.height * 0.4f);
	addChild(gp3);

	gp4 = Sprite::create("send/GP4.png");
	gp4->setScale(visibleSize.height / 9.0f / gp4->getContentSize().height);
	gp4->setPosition(origin.x + visibleSize.width * 0.85f, origin.y + visibleSize.height * 0.4f);
	addChild(gp4);

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

	if (gp1->getBoundingBox().containsPoint(touch->getLocation())) {
		ref->setIntegerForKey(KEY_CHOICE, 1);
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	if (gp2->getBoundingBox().containsPoint(touch->getLocation())) {
		ref->setIntegerForKey(KEY_CHOICE, 9);
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	if (gp3->getBoundingBox().containsPoint(touch->getLocation())) {
		ref->setIntegerForKey(KEY_CHOICE, 12);
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	if (gp4->getBoundingBox().containsPoint(touch->getLocation())) {
		ref->setIntegerForKey(KEY_CHOICE, 15);
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	return false;
}
