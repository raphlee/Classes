#ifndef __BULLET_OF_HERO_H__
#define __BULLET_OF_HERO_H__

#include "Bullet.h"

enum Type {
	normal,
	circle
};

class BulletOfHero : public Bullet
{
public:
	float alpha;//goc quay circle
	float radian;//goc ban dan
	Type type;
	BulletOfHero();
	~BulletOfHero();


	static BulletOfHero* create(float scale);
	void update(float dt);

};

#endif // __B2_SPRITE_H__