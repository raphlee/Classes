#ifndef __STATIC_HUMAN_ENEMY_H__
#define __STATIC_HUMAN_ENEMY_H__

#include "Enemy.h"
#include "Utility.h"

USING_NS_CC;

using namespace spine;

class StaticHumanEnemy : public Enemy
{
public:
	int checkCanShoot;
	CCArray *bulletPool;
	int indexBullet;
	StaticHumanEnemy(string jsonFile, string atlasFile, float scale);
	static StaticHumanEnemy* create(float scale);
	virtual void shoot(Point posOfHero);
	void die();
	void updateEnemy(float dt, Point cameraPoint, Point posOfHero);
	void updateBullet(Point cameraPoint);
	void createPool(int count);
//	void setAngle(float radian);
	bool checkOutScreen(Point posCamera);
	//void resetEnemy();
};

#endif // __ENEMY_H__
