#include "MiniFort.h"

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
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	//e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(-1);
	e->facingRight = false;
	e->setAnimation(0, "hit", false);
	//e->setTag(TAG_ENEMY);
	//e->setVisible(0);
	e->indexBullet = -1;
	return e;
}

void MiniFort::shoot(Point posOfHero)
{
	posOfHero = posOfHero - this->getParent()->getPosition();
	auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
	bullet->isDie = false;
	//bullet->body->SetTransform(this->body->GetPosition(), 0);
	//
	this->clearTracks();
	this->setAnimation(0, "hit", false);

	bullet->initPhysic(this->body->GetWorld(), this->body->GetPosition());
	bullet->setVisible(true);

	indexBullet++;
	if (indexBullet == MAX_BULLET_SOLDIER_ENEMY_POOL) {
		indexBullet = 0;
	}
	
		auto thisToHero = posOfHero - this->getPosition();
		thisToHero = thisToHero*(SCREEN_SIZE.width / 3 / thisToHero.length());
		bullet->body->SetLinearVelocity(b2Vec2(thisToHero.x/PTM_RATIO,thisToHero.y/PTM_RATIO));
	
}


void MiniFort::die()
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

	this->runAction((Sequence::create(DelayTime::create(0.5f), callFunc, nullptr)));

}




bool MiniFort::checkOutScreen(Point posCamera)
{
	return StaticHumanEnemy::checkOutScreen(posCamera);
}