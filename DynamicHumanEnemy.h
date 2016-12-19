#ifndef __DYNAMIC_HUMAN_ENEMY_H__
#define __DYNAMIC_HUMAN_ENEMY_H__

#include "Enemy.h"

USING_NS_CC;

using namespace spine;

class DynamicHumanEnemy : public Enemy
{
public:

	DynamicHumanEnemy(string jsonFile, string atlasFile, float scale);
	static DynamicHumanEnemy* create(float scale);

	void move();
	void updateEnemy(float dt);
};

#endif // __ENEMY_H__
