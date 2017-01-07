#include "DynamicHumanEnemy.h"

DynamicHumanEnemy::DynamicHumanEnemy(string jsonFile, string atlasFile, float scale) : Enemy(jsonFile, atlasFile, scale)
{
}



DynamicHumanEnemy * DynamicHumanEnemy::create(float scale)
{
	DynamicHumanEnemy *e = new DynamicHumanEnemy("enemy-soldier/soldier.json", "enemy-soldier/soldier.atlas", scale);
	e->setTag(TAG_ENEMY);
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(-1);
	e->facingRight = false;
	e->setAnimation(0,"running",true);
	return e;
}

void DynamicHumanEnemy::move()
{
	if(body != nullptr)
	this->body->SetLinearVelocity(b2Vec2(-move_vel,this->body->GetLinearVelocity().y));
}

void DynamicHumanEnemy::die()
{
	this->body->SetTransform(b2Vec2(INT_MAX, INT_MAX), 0);
}


void DynamicHumanEnemy::updateEnemy(float dt, Point cameraPoint)
{
	Enemy::updateEnemy(dt);
	//this->setPositionX(body->GetPosition().x * PTM_RATIO);
	//this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeEnemy.height / 2);
	move();
	shoot();
	if (checkOutScreen(cameraPoint)) resetEnemy();
	if (isDie) {
		resetEnemy();
		isDie = false;
	}
}

// private function
//20/12
bool DynamicHumanEnemy::checkOutScreen(Point posCamera)
{
	auto screenSize = Director::getInstance()->getVisibleSize();
	if (this->getPosition().y < 0 || this->getPosition().x < (posCamera.x - screenSize.width / 2)) {
		return true;
	}
	return false;
}

void DynamicHumanEnemy::resetEnemy()
{
	this->body->SetTransform(b2Vec2(INT_MAX / PTM_RATIO, INT_MIN / PTM_RATIO), 0);
	this->body->SetType(b2_staticBody);
}




