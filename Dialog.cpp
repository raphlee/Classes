#include "Dialog.h"
#include "GameScene.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"

bool Dialog::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->setVisible(false);

	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	// SCREEN_SIZE
	auto win_size = Director::getInstance()->getVisibleSize();

	background = Sprite::create("dialog.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(win_size / 2);
	addChild(background);


	resumeGameBtn = Sprite::create("cancel.png");
	resumeGameBtn->setAnchorPoint(Vec2::ZERO);
	resumeGameBtn->setScale(win_size.height / 10 / resumeGameBtn->getContentSize().height);
	resumeGameBtn->setPosition(win_size.width * 0.3f, win_size.height * 0.12f);
	addChild(resumeGameBtn);


	exitGameBtn = Sprite::create("ok.png");
	exitGameBtn->setAnchorPoint(Vec2::ZERO);
	exitGameBtn->setScale(win_size.height / 10 / exitGameBtn->getContentSize().height);
	exitGameBtn->setPosition(win_size.width * 0.56f, win_size.height * 0.12f);
	addChild(exitGameBtn);
	
	return true;
}


bool Dialog::onTouchBegan(Touch * touch, Event * event)
{
	if (exitGameBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/sound-button.mp3");
		Director::getInstance()->replaceScene(StartScene::createScene());
	}

	if (resumeGameBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/sound-button.mp3");
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
		this->setVisible(false);
		GameScene* gameScene = (GameScene*) this->getParent()->getChildByTag(TAG_GAME);
		if (UserDefault::getInstance()->sharedUserDefault()->getBoolForKey(KEYSOUND)) {
			experimental::AudioEngine::resumeAll();
		}

		gameScene->resumeGame();
	}

	return false;
}




