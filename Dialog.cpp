#include "Dialog.h"
#include "GameScene.h"
#include "StartScene.h"
#include "ControlSettingScene.h"
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

	background = Sprite::create("send/dialog.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(win_size / 2);
	addChild(background);


	goSettingBtn = Sprite::create("send/btn-setting.png");
	goSettingBtn->setAnchorPoint(Vec2::ZERO);
	goSettingBtn->setScale(win_size.height / 7 / goSettingBtn->getContentSize().height);
	goSettingBtn->setPosition(win_size.width * 0.3f, win_size.height * 0.4f);
	addChild(goSettingBtn);


	resumeGameBtn = Sprite::create("send/play-button.png");
	resumeGameBtn->setAnchorPoint(Vec2(0.5f, 0));
	resumeGameBtn->setScale(win_size.height / 7 / resumeGameBtn->getContentSize().height);
	resumeGameBtn->setPosition(win_size.width * 0.5f, win_size.height * 0.4f);
	addChild(resumeGameBtn);


	exitGameBtn = Sprite::create("send/cancel-button.png");
	exitGameBtn->setAnchorPoint(Vec2::ZERO);
	exitGameBtn->setScale(win_size.height / 7 / exitGameBtn->getContentSize().height);
	exitGameBtn->setPosition(win_size.width * 0.7f - exitGameBtn->getBoundingBox().size.width, win_size.height * 0.4f);
	addChild(exitGameBtn);
	
	return true;
}


bool Dialog::onTouchBegan(Touch * touch, Event * event)
{
	if (exitGameBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		Director::getInstance()->replaceScene(StartScene::createScene());
	}

	if (resumeGameBtn->getBoundingBox().containsPoint(touch->getLocation())) {

		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
		this->setVisible(false);
		GameScene* gameScene = (GameScene*) this->getParent()->getChildByTag(TAG_GAME);

		gameScene->resumeGame();
	}

	if (goSettingBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		GameScene* gameScene = (GameScene*) this->getParent()->getChildByTag(TAG_GAME);
		gameScene->isChangeControl = true;
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
		this->setVisible(false);
		
		Director::getInstance()->pushScene(ControlSettingScene::createScene());
	}

	return false;
}




