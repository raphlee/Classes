#include "HelicopterShootEnemy.h"

HelicopterShootEnemy::HelicopterShootEnemy(string jsonFile, string atlasFile, float scale) : StaticHumanEnemy(jsonFile, atlasFile, scale)
{
}




HelicopterShootEnemy * HelicopterShootEnemy::create(float scale, HelicopterType type)
{
	HelicopterShootEnemy *e = new HelicopterShootEnemy("enemy-helicopter/helicopter.json", "enemy-helicopter/helicopter.atlas", scale);
	e->setTag(TAG_ENEMY_HELICOPTER_SHOOT);
	e->type = type;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setAnimation(0, "flying", false);
	e->isDie = false;
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(1);
	e->facingRight = false;
	//e->setTag(TAG_ENEMY);
	//e->setVisible(0);
	e->indexBullet = -1;
	return e;
	
}

void HelicopterShootEnemy::move(Point posOfSoldier)
{
	
		if (body != nullptr)
			this->body->SetLinearVelocity(b2Vec2(-move_vel*this->getScaleX(), this->body->GetLinearVelocity().y));
	
}

void HelicopterShootEnemy::shoot(Point posOfHero)
{
	switch (type) {
	case HelicopterType::SHOOT_VERTICAL:
	{
		posOfHero = posOfHero - this->getParent()->getPosition();
		auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
		bullet->isDie = false;
		auto pos = this->body->GetPosition();
		bullet->initPhysic(this->body->GetWorld(), pos);
		bullet->setVisible(true);
		bullet->body->SetGravityScale(1);

		indexBullet++;
		if (indexBullet == MAX_BULLET_HELICOPTER_POOL) {
			indexBullet = 0;
		}
		//bullet->setAngel(PI);
		break;
	}

	case HelicopterType::SHOOT_HORIZONTAL:
	{
		posOfHero = posOfHero - this->getParent()->getPosition();
		auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
		bullet->isDie = false;
		auto pos = this->body->GetPosition();
		bullet->initPhysic(this->body->GetWorld(), pos);
		bullet->setVisible(true);
		//bullet->body->SetGravityScale(1);

		indexBullet++;
		if (indexBullet == MAX_BULLET_HELICOPTER_POOL) {
			indexBullet = 0;
		}
		bullet->setAngel(PI);
		break;
	}
	}
}



void HelicopterShootEnemy::die()
{
	Enemy::die();
	auto world = this->body->GetWorld();
	if (world->IsLocked()) return;
	world->DestroyBody(body);
	this->body = nullptr;
	//this->setVisible(false);
	this->clearTracks();
	this->setAnimation(0, "destroy", false);
	auto callFunc = CallFunc::create([&]() {
		this->setVisible(false);//removeFromParentAndCleanup(true);
	});
	

	this->runAction(Spawn::createWithTwoActions(Sequence::create(DelayTime::create(0.5f), callFunc, nullptr), Spawn::create(ScaleTo::create(0.5, 0), RotateTo::create(0.5, 360), MoveBy::create(0.5f,Vec2(-SCREEN_SIZE.width/4,-SCREEN_SIZE.height/4)),nullptr)));
}

void HelicopterShootEnemy::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	StaticHumanEnemy::updateEnemy(dt, cameraPoint, posOfHero);
	move(posOfHero);
}



