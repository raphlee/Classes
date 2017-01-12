#ifndef __DYNAMIC_HUMAN_ENEMY_H__
#define __DYNAMIC_HUMAN_ENEMY_H__

#include "Enemy.h"
#include "Utility.h"

USING_NS_CC;

using namespace spine;

class DynamicHumanEnemy : public Enemy
{
public:

	DynamicHumanEnemy(string jsonFile, string atlasFile, float scale);
	static DynamicHumanEnemy* create(float scale);
	void move();
	void die();
	void updateEnemy(float dt, Point cameraPoint);
//protected:
	bool checkOutScreen(Point posCamera);
	void resetEnemy();
};

#endif // __ENEMY_H__
