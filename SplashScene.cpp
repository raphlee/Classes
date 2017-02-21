#include "SplashScene.h"
#include "SimpleAudioEngine.h"


Scene* SplashScene::createScene()
{

    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add Splash sprite
	splashSprite = Sprite::create("pipo.png");
	splashSprite->setScaleX(SCREEN_SIZE.width / splashSprite->getContentSize().width);
	splashSprite->setScaleY(SCREEN_SIZE.height / splashSprite->getContentSize().height);
	splashSprite->setPosition(origin + visibleSize / 2);
	addChild(splashSprite);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map1.plist");
	Director::getInstance()->getTextureCache()->addImage("box/lid.png");

	experimental::AudioEngine::preload("sound/boss_explosion.mp3");
	experimental::AudioEngine::preload("sound/cannon shoot.mp3");		// touch me
	experimental::AudioEngine::preload("sound/enemy bullet.mp3");			// attack
	experimental::AudioEngine::preload("sound/enemy_bomb.mp3");		// on me
	experimental::AudioEngine::preload("sound/enemy_bomb_explosion.mp3");
	experimental::AudioEngine::preload("sound/F bullet.mp3");
	experimental::AudioEngine::preload("sound/FormatFactoryplane_bomb.mp3");
	experimental::AudioEngine::preload("sound/get item.mp3");
	experimental::AudioEngine::preload("sound/helicopter.mp3");
	experimental::AudioEngine::preload("sound/machine gun.mp3");
	experimental::AudioEngine::preload("sound/missle.mp3");
	experimental::AudioEngine::preload("sound/normal bullet.mp3");
	experimental::AudioEngine::preload("sound/plane_bomb.mp3");
	experimental::AudioEngine::preload("sound/plane_drop.mp3");
	experimental::AudioEngine::preload("sound/S Bullet.mp3");
	experimental::AudioEngine::preload("sound/sound_lose.mp3");
	experimental::AudioEngine::preload("sound/tank move.mp3");
	experimental::AudioEngine::preload("sound/tank shoot .mp3");
	experimental::AudioEngine::preload("sound/tank_explosion.mp3");
	experimental::AudioEngine::preload("sound/theme_music.mp3");
	experimental::AudioEngine::preload("sound/transform.mp3");
	experimental::AudioEngine::preload("sound/transform_2.mp3");
	experimental::AudioEngine::preload("sound/Win.mp3");

	this->schedule([&](float dt) {
		timer += 1;

		if (timer >= 3) {
			unschedule("KeyMeNow");
			Director::getInstance()->replaceScene(TransitionFade::create(0.67f, StartScene::createScene()));
		}
		
	}, 1.0f, "KeyMeNow");

    return true;
}



