#include "HelicopterBoomEnemy.h"
#include "AudioEngine.h"

HelicopterBoomEnemy::HelicopterBoomEnemy(string jsonFile, string atlasFile, float scale) : StaticHumanEnemy(jsonFile, atlasFile, scale)
{
}

HelicopterBoomEnemy * HelicopterBoomEnemy::create(float scale, HelicopterBoomType type)
{
	HelicopterBoomEnemy *e = new HelicopterBoomEnemy("enemy-helicopter/helicopter.json", "enemy-helicopter/helicopter.atlas", scale);
	e->setTag(TAG_ENEMY_HELICOPTER_BOOM);
	e->type = type;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 8.0f;
	e->setAnimation(0, "flying", true);
	e->isDie = false;
	e->update(0.0f);
	e->health = 2;
	e->sizeEnemy = e->getBoundingBox().size;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(1);
	e->facingRight = false;
	//e->setTag(TAG_ENEMY);
	//e->setVisible(0);
	e->indexBullet = -1;
	return e;

}

void HelicopterBoomEnemy::move(Point posOfSoldier)
{

	if (body != nullptr) {
		if (type == HelicopterBoomType::SIMPLE) {
			this->body->SetLinearVelocity(b2Vec2(-move_vel, this->body->GetLinearVelocity().y));
		}
		else {
			if (this->checkCanShoot == 60) {
				auto vec = posOfSoldier - (this->getPosition() + this->getParent()->getPosition());
				if (vec.x <= 0) {
					move_vel = -SCREEN_SIZE.width / 5 / PTM_RATIO;
					//this->body->SetLinearVelocity(b2Vec2(-SCREEN_SIZE.width / 5 / PTM_RATIO, this->body->GetLinearVelocity().y));
				}
				else {
					move_vel = SCREEN_SIZE.width / 5 / PTM_RATIO;
					//this->body->SetLinearVelocity(b2Vec2(SCREEN_SIZE.width / 5 / PTM_RATIO, this->body->GetLinearVelocity().y));
				}
				this->body->SetLinearVelocity(b2Vec2(move_vel, this->body->GetLinearVelocity().y));
			}
		}
	}

}

void HelicopterBoomEnemy::shoot(Point posOfHero)
{
	/*auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEYSOUND);
	if (checkSound) {
		experimental::AudioEngine::play2d(SOUND_HELICOPTER);
	}*/
	AudioManager::playSound(SOUND_HELICOPTER);
	posOfHero = posOfHero - this->getParent()->getPosition();
	auto bullet = (BombOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
	bullet->isDie = false;
	auto pos = this->body->GetPosition();
	bullet->initPhysic(this->body->GetWorld(), pos);
	bullet->setVisible(true);
	//bullet->body->SetGravityScale(1);

	indexBullet++;
	if (indexBullet == MAX_BULLET_HELICOPTER_POOL) {
		indexBullet = 0;
	}
	//bullet->setAngel(PI);


}



void HelicopterBoomEnemy::die()
{
	auto world = this->body->GetWorld();
	if (world->IsLocked()) return;
	world->DestroyBody(body);
	this->body = nullptr;
	//this->setVisible(false);
	this->clearTracks();
	this->setAnimation(0, "destroy", false);
	auto callFunc = CallFunc::create([&]() {
		this->setVisible(false);
		//removeFromParentAndCleanup(true);
	});

	this->runAction((Sequence::create(DelayTime::create(0.5f), callFunc, nullptr)));
}

void HelicopterBoomEnemy::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	StaticHumanEnemy::updateEnemy(dt, cameraPoint, posOfHero);
	move(posOfHero);
}

void HelicopterBoomEnemy::createPool(int count)
{
	bulletPool = CCArray::createWithCapacity(count);
	bulletPool->retain();
	for (int i = 0; i < count; i++) {
		auto bullet = BombOfEnemy::create();
		bullet->setPosition(INT_MAX, INT_MAX);
		//this->getParent()->addChild(bullet);
		this->getParent()->addChild(bullet, ZORDER_BULLET);
		//bullet->initPhysic(this->body->GetWorld(), bullet->getPosition());
		bullet->body = nullptr;
		bulletPool->addObject(bullet);

	}
	indexBullet = 0;
}



