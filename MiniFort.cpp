#include "MiniFort.h"
#include "AudioEngine.h"

MiniFort::MiniFort(string jsonFile, string atlasFile, float scale) : StaticHumanEnemy(jsonFile, atlasFile, scale)
{
	//checkCanShoot = 100;
}



MiniFort * MiniFort::create(float scale)
{
	MiniFort *e = new MiniFort("fortmini/enemy-gun.json", "fortmini/enemy-gun.atlas", scale);
	e->setTag(TAG_ENEMY_FORTMINI);
	e->isDie = false;
	e->update(0.0f);
	e->health = 2;
	e->sizeEnemy = e->getBoundingBox().size;
	e->setScaleX(-1);
	e->facingRight = false;
	e->setAnimation(0, "hit", false);
	e->indexBullet = -1;
	return e;
}

void MiniFort::getHit()
{
	clearTracks();
	addAnimation(0, "hit", false);
	setToSetupPose();
}

void MiniFort::shoot(Point posOfHero)
{
	/*auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEYSOUND);
	if (checkSound) {
		experimental::AudioEngine::play2d(SOUND_CANON_SHOOT);
	}*/
	AudioManager::playSound(SOUND_CANON_SHOOT);
	posOfHero = posOfHero - this->getParent()->getPosition();
	auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
	bullet->isDie = false;
	//bullet->body->SetTransform(this->body->GetPosition(), 0);
	//
	//this->clearTracks();
	//this->setAnimation(0, "hit", false);
	auto thisToHero = posOfHero - this->getPosition();
	auto unitVecto = thisToHero / thisToHero.length();
	thisToHero = thisToHero*(SCREEN_SIZE.width / 3 / thisToHero.length());
	auto vecOfBarrel = (barrel->getBoundingBox().size.width*unitVecto)/PTM_RATIO;

	bullet->initPhysic(this->body->GetWorld(), this->body->GetPosition()+b2Vec2(vecOfBarrel.x,vecOfBarrel.y));
	bullet->setVisible(true);
	//barrel->setRotation(thisToHero.getAngle());
	bullet->body->SetLinearVelocity(b2Vec2(thisToHero.x / PTM_RATIO, thisToHero.y / PTM_RATIO));

	indexBullet++;
	if (indexBullet == MAX_BULLET_SOLDIER_ENEMY_POOL) {
		indexBullet = 0;
	}

	
	

}


void MiniFort::die()
{
	//Enemy::die();
	barrel->removeFromParentAndCleanup(true);
	auto world = this->body->GetWorld();
	if (world->IsLocked()) return;
	world->DestroyBody(body);
	this->body = nullptr;
	this->clearTracks();
	this->setAnimation(0, "destroy", false);
	auto callFunc = CallFunc::create([&]() {
		this->setVisible(false);//removeFromParentAndCleanup(true);
	});

	this->runAction((Sequence::create(DelayTime::create(0.5f), callFunc, nullptr)));

}

void MiniFort::createBarrel()
{
	barrel = Sprite::createWithSpriteFrameName("enemy-gun-barrel.png");
	barrel->setScale((SCREEN_SIZE.height/45.0f)/barrel->getContentSize().height);
	barrel->setAnchorPoint(Point(1, 0.5f));
	barrel->setPosition(this->getGunLocation());
	this->getParent()->addChild(barrel);
}

Vec2 MiniFort::getGunLocation()
{
	auto gun = findBone("top");
	auto pos = Vec2(this->getScaleX()*gun->worldX, gun->worldY);
	pos = pos + this->getPosition();
	return pos;
}

void MiniFort::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	StaticHumanEnemy::updateEnemy(dt, cameraPoint, posOfHero);
	if (!isDie) {
		posOfHero = posOfHero - this->getParent()->getPosition();
		auto thisToHero = posOfHero - this->getPosition();
		thisToHero.x = -thisToHero.x;
		log("Angle: %f", thisToHero.getAngle());
		barrel->setPosition(this->getGunLocation());
		barrel->setRotation((thisToHero.getAngle() / PI) * 180);
	}
}




bool MiniFort::checkOutScreen(Point posCamera)
{
	return StaticHumanEnemy::checkOutScreen(posCamera);
}