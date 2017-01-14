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
	bool isFirstShoot;
	int isNoDie;
	float angle; // goc de ban dan

	CCArray *bulletPool;
	int indexBullet;

	Soldier(string jsonFile, string atlasFile, float scale);
	static Soldier* create(string jsonFile, string atlasFile, float scale);
	bool onGround = true;
	bool isGetOriginX = false;
	bool isTransform = false;
	
	Size sizeSoldier;
	float jump_vel;

	State cur_state;
	State pre_state;

	virtual void move();
	virtual void die(Point posOfCammera);

	virtual void listener();

	virtual void initPhysic(b2World *world, Point pos);

	virtual void idleShoot();
	virtual void idleShootUp();
	virtual void jumping();
	virtual void lyingShoot();
	virtual void runningShoot();
	virtual void runningShootUp();
	virtual void runningShootDown();
	virtual void createPool();
	virtual void shoot(float radian);
	virtual void createBullet(float radian);
	
	void updateHero(float dt);
	//virtual void changeBodyBitMask(uint16 mask);
	virtual Point getGunLocation();
};
#endif // __SOLDIER_H__