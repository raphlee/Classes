#include "BulletOfEnemy.h"



BulletOfEnemy::BulletOfEnemy()
{
}

BulletOfEnemy::~BulletOfEnemy()
{
}

BulletOfEnemy * BulletOfEnemy::create(float scale)
{
	BulletOfEnemy *bullet = new BulletOfEnemy();
	bullet->initWithSpriteFrameName("enemy-bullet.png");
	bullet->setTag(TAG_BULLET_ENEMY);
	bullet->isDie = false;
	return bullet;
}

void BulletOfEnemy::update(float dt)
{
	//Bullet::update(dt);
	this->setPosition(Point(this->body->GetPosition().x*PTM_RATIO - this->getParent()->getPositionX(), 
							this->body->GetPosition().y*PTM_RATIO));
	if (isDie) {
		this->body->SetTransform(b2Vec2(INT_MAX, INT_MAX), 0);
		isDie = false;
	}
}
