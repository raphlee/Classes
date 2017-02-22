#include "Dialog.h"
#include "GameScene.h"
#include "StartScene.h"
#include "ControlSettingScene.h"
#include "SimpleAudioEngine.h"

#ifdef SDKBOX_ENABLED
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"
#include "PluginAdMob/PluginAdMob.h"
#endif

bool Dialog::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//this->setVisible(false);

	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	// SCREEN_SIZE
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();

	if (!isLoseTheGame) {
		background = Sprite::create("send/dialog.png");
		background->setScale(win_size.width / 2 / background->getContentSize().width);
		background->setPosition(origin + win_size / 2);
		addChild(background);


		goSettingBtn = Sprite::create("send/btn-setting.png");
		goSettingBtn->setAnchorPoint(Vec2::ZERO);
		goSettingBtn->setScale(win_size.height / 7 / goSettingBtn->getContentSize().height);
		goSettingBtn->setPosition(origin.x + win_size.width * 0.3f, origin.y + win_size.height * 0.4f);
		addChild(goSettingBtn);


		resumeGameBtn = Sprite::create("send/play-button.png");
		resumeGameBtn->setAnchorPoint(Vec2(0.5f, 0));
		resumeGameBtn->setScale(win_size.height / 7 / resumeGameBtn->getContentSize().height);
		resumeGameBtn->setPosition(origin.x + win_size.width * 0.5f, origin.y + win_size.height * 0.4f);
		addChild(resumeGameBtn);


		exitGameBtn = Sprite::create("send/cancel-button.png");
		exitGameBtn->setAnchorPoint(Vec2::ZERO);
		exitGameBtn->setScale(win_size.height / 7 / exitGameBtn->getContentSize().height);
		exitGameBtn->setPosition(origin.x + win_size.width * 0.7f - exitGameBtn->getBoundingBox().size.width,
			origin.y + win_size.height * 0.4f);
		addChild(exitGameBtn);
	}
	else {
		background = Sprite::create("send/retry.png");
		background->setScale(win_size.width / 3 / background->getContentSize().width);
		background->setPosition(origin + win_size / 2);
		addChild(background);


		resumeGameBtn = Sprite::create("send/retry-button.png");
		resumeGameBtn->setScale(win_size.height / 7 / resumeGameBtn->getContentSize().height);
		resumeGameBtn->setPosition(origin.x + win_size.width * 0.43f, origin.y + win_size.height * 0.45f);
		addChild(resumeGameBtn);


		exitGameBtn = Sprite::create("send/cancel-button.png");
		exitGameBtn->setScale(win_size.height / 7 / exitGameBtn->getContentSize().height);
		exitGameBtn->setPosition(origin.x + win_size.width * 0.57f, origin.y + win_size.height * 0.45f);
		addChild(exitGameBtn);
	}



	return true;
}

Dialog* Dialog::create(bool isLoseTheGame)
{
	Dialog* dialog = new Dialog();
	dialog->isLoseTheGame = isLoseTheGame;
	if (dialog && dialog->init())
	{
		dialog->autorelease();
		return dialog;
	}
	else
	{
		delete dialog;
		dialog = nullptr;
		return nullptr;
	}
}


bool Dialog::onTouchBegan(Touch * touch, Event * event)
{
	if (exitGameBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		log("AHHIHIH");
		Director::getInstance()->replaceScene(StartScene::createScene());
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show("gameover");
#endif
	}

	if (resumeGameBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		
		GameScene* gameScene = (GameScene*) this->getParent()->getChildByTag(TAG_GAME);

		if (!isLoseTheGame) {
			gameScene->resumeGame();
			Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
		}
		else {
#ifdef SDKBOX_ENABLED
			if (sdkbox::PluginAdMob::isAvailable("gameover")) {
				Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
				gameScene->retryGame();
				sdkbox::PluginAdMob::show("gameover");
			}
			else {
				resumeGameBtn->runAction(Sequence::create(ScaleTo::create(0.4f, 1.3f), ScaleTo::create(0.4f, 1), nullptr));
			}
#endif
//#ifndef SDKBOX_ENABLED
//			gameScene->retryGame();
//#endif
		}
	}

	else if (!isLoseTheGame && goSettingBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		GameScene* gameScene = (GameScene*) this->getParent()->getChildByTag(TAG_GAME);
		gameScene->isChangeControl = true;

		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
		this->removeFromParentAndCleanup(true);

		Director::getInstance()->pushScene(ControlSettingScene::createScene());
	}

	return false;
}




