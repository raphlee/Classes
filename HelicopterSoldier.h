#ifndef __HELICOPTER_SOLDIER_H__
#define __HELICOPTER_SOLDIER_H__

#include "Soldier.h"

class HelicopterSoldier : public Soldier
{
public:
	
	HelicopterSoldier(string jsonFile, string atlasFile, float scale);
	static HelicopterSoldier* create(string jsonFile, string atlasFile, float scale);

	void initPhysic(b2World * world, Point pos);

	void die(Point posOfCammera);

	void idle();
	void idleShoot();
	void shoot(float radian);

	void updateHero(float dt);
};
#endif // __HELICOPTER_SOLDIER_H__