#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "B2Skeleton.h"

USING_NS_CC;

using namespace spine;

class Enemy : public B2Skeleton
{
public:

	Enemy(string jsonFile, string atlasFile, float scale);
	static Enemy* create(string jsonFile, string atlasFile, float scale);
	Size sizeEnemy;

	virtual void move();
	virtual void shoot();
	virtual void updateEnemy(float dt);
};

#endif // __ENEMY_H__
