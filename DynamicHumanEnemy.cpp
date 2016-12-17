#include "DynamicHumanEnemy.h"

DynamicHumanEnemy::DynamicHumanEnemy(string jsonFile, string atlasFile, float scale) : Enemy(jsonFile, atlasFile, scale)
{
}



DynamicHumanEnemy * DynamicHumanEnemy::create(float scale)
{
	DynamicHumanEnemy *e = new DynamicHumanEnemy("enemy-soldier/soldier.json", "enemy-soldier/soldier.atlas", scale);
	//e->setTag(ENEMY);
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

//void DynamicHumanEnemy::shoot()
//{
//}


void DynamicHumanEnemy::updateEnemy(float dt)
{
	this->setPositionX(body->GetPosition().x * PTM_RATIO);
	this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeEnemy.height / 2);
	move();
	shoot();
}
