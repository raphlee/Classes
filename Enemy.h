#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "B2Skeleton.h"

USING_NS_CC;

using namespace spine;


enum Type {
	MOVE,
	STAND
};


class Enemy : public B2Skeleton
{
public:

	Enemy(string jsonFile, string atlasFile, float scale);
	static Enemy* create(string jsonFile, string atlasFile, float scale);
	Type type;

	Size sizeEnemy;

	void move();
	void shoot();
	void shootUp();
	void shootUpLeft();
	void shootUpRight();
	void updateEnemy(float dt);
};

#endif // __ENEMY_H__
