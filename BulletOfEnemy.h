#ifndef __BULLET_OF_ENEMY_H__
#define __BULLET_OF_ENEMY_H__

#include "Bullet.h"


class BulletOfEnemy : public Bullet
{
public:
	//bool isDie;
	BulletOfEnemy();
	~BulletOfEnemy();


	static BulletOfEnemy* create(float scale);

	// SCREEN_SIZE
	void update(float dt);
	bool checkOutOfScreen(Point camera);

};

#endif