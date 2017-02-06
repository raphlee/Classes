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
	if(body != nullptr)
		this->setPosition(Point(this->body->GetPosition().x*PTM_RATIO- this->getParent()->getPositionX(), 
			this->body->GetPosition().y*PTM_RATIO));
	
}

bool BulletOfEnemy::checkOutOfScreen(Point camera)
{
	// chi quan tam nhung thang dang bay
	if (this->body != nullptr) {
		if ((fabs(this->body->GetPosition().x*PTM_RATIO - camera.x) - SCREEN_SIZE.width) > 0
			|| (fabs(this->body->GetPosition().y*PTM_RATIO - camera.y) - SCREEN_SIZE.height) > 0) {
			return true;
		}
	}
	return false;
}
