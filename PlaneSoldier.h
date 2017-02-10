#ifndef __PLANE_SOLDIER_H__
#define __PLANE_SOLDIER_H__

#include "Soldier.h"
#include "BombOfSoldier.h"

class PlaneSoldier : public Soldier
{
public:
	
	PlaneSoldier(string jsonFile, string atlasFile, float scale);
	static PlaneSoldier* create(string jsonFile, string atlasFile, float scale);

	void initPhysic(b2World * world, Point pos);

	void die(Point posOfCammera);

	int indexBomb;
	int canDrop = 1;

	//void idle();
	void idleShoot();
	void createBombPool();
	void createBomb();
	void shoot(float radian);
	void dropLittleBoy();
	Point getGunLoc(string boneName);

	void updateHero(float dt);
};
#endif // __PLANE_SOLDIER_H__