#ifndef __BOMB_OF_SOLDIER_H__
#define __BOMB_OF_SOLDIER_H__


#include "Bullet.h"

class BombOfSoldier : public Bullet
{
public:
	BombOfSoldier();
	~BombOfSoldier();


	bool isAdded = false;

	static BombOfSoldier* create();
	void initPhysic(b2World *world, Point pos);
};

#endif // __BOMB_OF_SOLDIER_H__