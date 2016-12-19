#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "B2Skeleton.h"
#include "GB2ShapeCache-x.h"

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

	Soldier(string jsonFile, string atlasFile, float scale);
	static Soldier* create(string jsonFile, string atlasFile, float scale);
	bool onGround = true;
	
	Size sizeSoldier;
	float jump_vel;

	State cur_state;
	State pre_state;

	void move(Point bgPos);

	void initPhysic(b2World *world, Point pos);

	void idleShoot();
	void idleShootUp();
	void jumping();
	void lyingShoot();
	void runningShoot();
	void runningShootUp();
	void runningShootDown();

	void updateSoldier(float dt);
};

#endif // __SOLDIER_H__
