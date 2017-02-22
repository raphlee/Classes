#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

#ifdef SDKBOX_ENABLED
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"
#include "PluginAdMob/PluginAdMob.h"
#endif


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

	if (!Layer::init())
	{
		return false;
	}
#ifdef SDKBOX_ENABLED
	sdkbox::PluginGoogleAnalytics::logScreen("StartScene");
	//sdkbox::PluginGoogleAnalytics::dispatchHits();
#endif
#ifdef SDKBOX_ENABLED
	if (sdkbox::PluginAdMob::isAvailable("home")) {
		sdkbox::PluginAdMob::show("home");
	}
	
#endif
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	experimental::AudioEngine::stopAll();

	// add Start sprite
	auto backGround = Sprite::create("background.png");

	backGround->setScaleX(visibleSize.width / backGround->getContentSize().width);
	backGround->setScaleY(visibleSize.height / backGround->getContentSize().height);
	backGround->setPosition(origin + visibleSize / 2);

	addChild(backGround);

	ref = UserDefault::getInstance()->sharedUserDefault();

	auto labelVer = Label::createWithTTF("2017-02-22-Build 1", "fonts/Marker Felt.ttf", 200);
	labelVer->setScale(visibleSize.height / 24.0f / labelVer->getContentSize().height);
	labelVer->setPosition(origin.x + visibleSize.width * 0.90f, origin.y + visibleSize.height * 0.96f);
	addChild(labelVer);

	//////////////////////////////////////////////////////////////////////////
	auto ground = Sprite::createWithSpriteFrameName("ground.png");
	ground->setAnchorPoint(Vec2(0.5, 0));
	ground->setPosition(origin.x + visibleSize.width / 2, origin.y);
	ground->setScale(visibleSize.width / ground->getContentSize().width);
	addChild(ground);

	grass1 = Sprite::createWithSpriteFrameName("grass-1.png");
	grass1->setScale(visibleSize.height / 20 / grass1->getContentSize().height);

	grass2 = Sprite::createWithSpriteFrameName("grass-2.png");
	grass2->setScale(visibleSize.height / 20 / grass1->getContentSize().height);

	grass3 = Sprite::createWithSpriteFrameName("grass-3.png");
	grass3->setScale(visibleSize.height / 23 / grass1->getContentSize().height);

	addChild(grass1);
	addChild(grass2);
	addChild(grass3);

	grass1->setAnchorPoint(Point(0.5, 0));
	grass2->setAnchorPoint(Point(0.5, 0));
	grass3->setAnchorPoint(Point(0.5, 0));

	grass1->setPosition(visibleSize.width * 3 / 8, visibleSize.height / 4.71f);
	grass2->setPosition(visibleSize.width * 7 / 8, visibleSize.height / 5.34f);
	grass3->setPosition(visibleSize.width * 8 / 9, visibleSize.height / 5.45f);


	auto rambo = Sprite::createWithSpriteFrameName("title-ramball.png");
	rambo->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.65f);
	rambo->setScale(visibleSize.width * 0.4f / rambo->getContentSize().width);
	addChild(rambo);

	auto skeleton = SkeletonAnimation::createWithFile("soldier/soldier.json", "soldier/soldier.atlas", visibleSize.height / 5.0f / 182.0f);
	skeleton->setPosition(origin.x + visibleSize.width / 2, origin.y + ground->getBoundingBox().size.height);
	skeleton->addAnimation(0, "running", true);
	addChild(skeleton);

	play = Sprite::createWithSpriteFrameName("btn-play.png");
	play->setScale(visibleSize.height / 4.5f / play->getContentSize().height);
	auto posX = visibleSize.width / 2 + rambo->getBoundingBox().size.width / 2 + play->getBoundingBox().size.width * 0.7f;
	play->setPosition(origin.x + posX, origin.y + visibleSize.height * 0.6f);
	addChild(play);

	setting = Sprite::create("send/btn-setting.png");
	setting->setScale(visibleSize.height / 8.0f / setting->getContentSize().height);
	setting->setPosition(origin.x + visibleSize.width * 0.1f, origin.y + visibleSize.height * 0.83f);
	addChild(setting, 3);


	if (ref->getIntegerForKey(KEYGUIDE) == NULL) {
		auto scale1 = setting->getScale();
		setting->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.4f, scale1*1.1f), ScaleTo::create(0.4f, scale1), nullptr)));
		auto setting_text = Sprite::create("send/setting.png");
		setting_text->setScale(visibleSize.height / 9.0f / setting_text->getContentSize().height);
		setting_text->setPosition(origin.x + visibleSize.width * 0.1f, origin.y + visibleSize.height * 0.71f);
		addChild(setting_text, 3);
		auto scale2 = setting_text->getScale();
		setting_text->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.4f, scale2*1.1f), ScaleTo::create(0.4f, scale2), nullptr)));
	}

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

	soundOn = Sprite::createWithSpriteFrameName("btn-sound-on.png");
	soundOff = Sprite::createWithSpriteFrameName("btn-sound-off.png");

	soundOn->setPosition(visibleSize.width * 0.85f, visibleSize.height / 10);
	soundOff->setPosition(visibleSize.width * 0.85f, visibleSize.height / 10);

	this->addChild(soundOn);
	this->addChild(soundOff);
	float scaleOfSound = (visibleSize.height / 11) / soundOn->getContentSize().height;
	soundOn->setScale(scaleOfSound);
	soundOff->setScale(scaleOfSound);

	bool checkSound = ref->getBoolForKey(KEYSOUND, true);
	if (checkSound) {
		soundOff->setVisible(false);
		backgroudSoundID = experimental::AudioEngine::play2d(SOUND_BACKGROUND, true);
		if (ref->getIntegerForKey(KEY_ID_BG_MUSIC) == NULL) {
			ref->setIntegerForKey(KEY_ID_BG_MUSIC, backgroudSoundID);
		}
	}
	else {
		soundOn->setVisible(false);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StartScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void StartScene::createAction()
{
	action_1 = RepeatForever::create(Sequence::create(Spawn::create(Sequence::create(ScaleTo::create(0.75, 1.2), ScaleTo::create(0.75, 0.1), nullptr),
		Sequence::create(MoveTo::create(0.75, Vec2(Director::getInstance()->getVisibleSize().width * 1 / 4, Director::getInstance()->getVisibleSize().height / 5)),
			MoveTo::create(0.75, Vec2(Director::getInstance()->getVisibleSize().width * 1 / 8, Director::getInstance()->getVisibleSize().height / 6.0f)), nullptr), nullptr),
		CallFunc::create([&]() {
		grass1->setPosition(Director::getInstance()->getVisibleSize().width * 3 / 8,
			Director::getInstance()->getVisibleSize().height / 4.9f);
	}), nullptr));

	action_2 = RepeatForever::create(Sequence::create(Spawn::create(Sequence::create(ScaleTo::create(1, 1.2), ScaleTo::create(1, 0.1), nullptr),
		Sequence::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width * 6 / 8, Director::getInstance()->getVisibleSize().height / 5)),
			MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width * 5 / 8, Director::getInstance()->getVisibleSize().height / 4.71f)), nullptr), nullptr),
		CallFunc::create([&]() {
		grass2->setPosition(Director::getInstance()->getVisibleSize().width * 7 / 8,
			Director::getInstance()->getVisibleSize().height / 5.32f);
	}), nullptr));

	action_3 = RepeatForever::create(Sequence::create(DelayTime::create(0.5), Spawn::create(Sequence::create(ScaleTo::create(0.75, 1.2), ScaleTo::create(0.75, 0.1), nullptr),
		Sequence::create(MoveTo::create(0.75, Vec2(Director::getInstance()->getVisibleSize().width * 7 / 9, Director::getInstance()->getVisibleSize().height / 5)),
			MoveTo::create(0.75, Vec2(Director::getInstance()->getVisibleSize().width * 6 / 9, Director::getInstance()->getVisibleSize().height / 4.4f)), nullptr), nullptr),
		CallFunc::create([&]() {
		grass3->setPosition(Director::getInstance()->getVisibleSize().width * 8 / 9,
			Director::getInstance()->getVisibleSize().height / 5.45f);
	}), nullptr));

}

void StartScene::onEnter()
{
	Node::onEnter();
	createAction();
	grass1->runAction(action_1);
	grass2->runAction(action_2);
	grass3->runAction(action_3);
#ifdef SDKBOX_ENABLED
	if (sdkbox::PluginAdMob::isAvailable("home")) {
		sdkbox::PluginAdMob::show("home");
	}
#endif

}

bool StartScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (play->getBoundingBox().containsPoint(touch->getLocation())) {
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	else if (setting->getBoundingBox().containsPoint(touch->getLocation())) {
		//grass1->stopAllActions(); grass2->stopAllActions(); grass3->stopAllActions();
		
		ref->setIntegerForKey(KEYGUIDE, 1); ref->flush();
		Director::getInstance()->replaceScene(ControlSettingScene::createScene());
	}

	else if (gp1->getBoundingBox().containsPoint(touch->getLocation())) {
		ref->setIntegerForKey(KEY_CHOICE, 1); ref->flush();
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	else if (gp2->getBoundingBox().containsPoint(touch->getLocation())) {
		ref->setIntegerForKey(KEY_CHOICE, 9); ref->flush();
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	else if (gp3->getBoundingBox().containsPoint(touch->getLocation())) {
		ref->setIntegerForKey(KEY_CHOICE, 12); ref->flush();
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	else if (gp4->getBoundingBox().containsPoint(touch->getLocation())) {
		ref->setIntegerForKey(KEY_CHOICE, 15); ref->flush();
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	else if (soundOn->getBoundingBox().containsPoint(touch->getLocation())) {
		//if (soundOn->isVisible()) {
		if (ref->getBoolForKey(KEYSOUND, true)) {
			soundOn->setVisible(false);
			soundOff->setVisible(true);
			ref->setBoolForKey(KEYSOUND, false); ref->flush();
			experimental::AudioEngine::stop(backgroudSoundID);
		}
		else {
			soundOff->setVisible(false);
			soundOn->setVisible(true);
			ref->setBoolForKey(KEYSOUND, true); ref->flush();
			backgroudSoundID = experimental::AudioEngine::play2d(SOUND_BACKGROUND, true);
		}
	}

	return false;
}
