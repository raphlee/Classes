#ifndef __TANK_SOLDIER_H__
#define __TANK_SOLDIER_H__

#include "Soldier.h"

class TankSoldier : public Soldier
{
public:
	
	TankSoldier(string jsonFile, string atlasFile, float scale);
	static TankSoldier* create(string jsonFile, string atlasFile, float scale);

	void initPhysic(b2World * world, Point pos);

	void die(Point posOfCammera);

//	void idle();
	void idleShoot();
	void idleShootUp();
	void jumping();
	void lyingShoot();
	void runningShoot();
	void runningShootUp();
	void runningShootDown();

	Point getGunLocation();
};
#endif // __TANK_SOLDIER_H__