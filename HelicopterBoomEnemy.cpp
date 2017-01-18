#include "HelicopterBoomEnemy.h"

HelicopterBoomEnemy::HelicopterBoomEnemy(string jsonFile, string atlasFile, float scale) : StaticHumanEnemy(jsonFile, atlasFile, scale)
{
}




HelicopterBoomEnemy * HelicopterBoomEnemy::create(float scale, HelicopterBoomType type)
{
	HelicopterBoomEnemy *e = new HelicopterBoomEnemy("tank/tank.json", "tank/tank.atlas", scale);
	e->setTag(TAG_ENEMY_HELICOPTER_SHOOT);
	e->type = type;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setAnimation(0, "flying", false);
	e->isDie = false;
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	//e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(1);
	e->facingRight = false;
	//e->setTag(TAG_ENEMY);
	//e->setVisible(0);
	e->indexBullet = -1;
	return e;
	
}

void HelicopterBoomEnemy::move(Point posOfSoldier)
{
	
		if (body != nullptr)
			this->body->SetLinearVelocity(b2Vec2(-move_vel, this->body->GetLinearVelocity().y));
	
}

void HelicopterBoomEnemy::shoot(Point posOfHero)
{
	switch (type) {
	case HelicopterBoomType::STUPID:
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

	case HelicopterBoomType::SMART:
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
		this->setVisible(false);//removeFromParentAndCleanup(true);
	});

	this->runAction((Sequence::create(DelayTime::create(0.5f), callFunc, nullptr)));
}

void HelicopterBoomEnemy::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	StaticHumanEnemy::updateEnemy(dt, cameraPoint, posOfHero);
	move(posOfHero);
}



