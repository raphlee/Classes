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
