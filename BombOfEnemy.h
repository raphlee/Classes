#ifndef __BOOM_OF_ENEMY_H__
#define __BOOM_OF_ENEMY_H__

#include "BulletOfEnemy.h"
#include "Global.h"


class BombOfEnemy : public BulletOfEnemy
{
public:
	//bool isDie;
	BombOfEnemy();
	~BombOfEnemy();

	static BombOfEnemy* create();
	void initPhysic(b2World *world, b2Vec2 pos);
	// SCREEN_SIZE
	

};

#endif