#ifndef __BULLET_OF_HERO_H__
#define __BULLET_OF_HERO_H__

#include "Bullet.h"


class BulletOfHero : public Bullet
{
public:
	BulletOfHero();
	~BulletOfHero();


	static BulletOfHero* create(float scale);

};

#endif // __B2_SPRITE_H__