#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "B2Skeleton.h"
#include "GB2ShapeCache-x.h"
#include "BulletOfHero.h"

USING_NS_CC;

using namespace spine;

enum State {
	IDLE_SHOOT,
	IDLE_SHOOT_UP,
	JUMPING,

	LYING_SHOOT,

	RUNNING_SHOOT,
	RUNNING_SHOOT_DOWN,
	RUNNING_SHOOT_UP,

	DIE
};


class Soldier : public B2Skeleton
{
public:
	int canShoot;
	int isNoDie;
	float angle; // goc de bem dan
	CCArray *bulletPool;
	int indexBullet;

	Soldier(string jsonFile, string atlasFile, float scale);
	static Soldier* create(string jsonFile, string atlasFile, float scale);
	bool onGround = true;
	
	Size sizeSoldier;
	float jump_vel;

	State cur_state;
	State pre_state;

	void move(Point bgPos);
	void die(Point posOfCammera);

	void listener();

	void initPhysic(b2World *world, Point pos);

	void idleShoot();
	void idleShootUp();
	void jumping();
	void lyingShoot();
	void runningShoot();
	void runningShootUp();
	void runningShootDown();
	void createPool();
	void shoot(float radian);
	void updateSoldier(float dt);
	//void changeBodyBitMask(uint16 mask);
	Point getGunLocation();
};
#endif // __SOLDIER_H__