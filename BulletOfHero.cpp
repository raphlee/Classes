#include "BulletOfHero.h"



BulletOfHero::BulletOfHero()
{
}

BulletOfHero::~BulletOfHero()
{
}

BulletOfHero * BulletOfHero::create(float scale)
{
	BulletOfHero *bullet = new BulletOfHero();
	bullet->initWithSpriteFrameName("soldier-bullet.png");
	bullet->setTag(TAG_BULLET_HERO);
	return bullet;
}

void BulletOfHero::update(float dt)
{
	Bullet::update(dt);
}

//void BulletOfHero::setAngel(float radian)
//{
//	if (body != nullptr) {
//		auto vecx = VELOCITY_FOR_BULLET_HERO*cosf(radian);
//		auto vecy = VELOCITY_FOR_BULLET_HERO*sinf(radian);
//		body->SetLinearVelocity(b2Vec2(vecx, vecy));
//	}
//}
