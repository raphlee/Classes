#ifndef __HELICOPTER_SHOOT_ENEMY_H__
#define __HELICOPTER_SHOOT_ENEMY_H__

#include "StaticHumanEnemy.h"
#include "BulletOfEnemy.h"
//#include "Utility.h"

USING_NS_CC;

using namespace spine;

enum HelicopterType {
	SHOOT_VERTICAL, // plane2
	SHOOT_HORIZONTAL,//plane
	SHOOT_SMART
};

class HelicopterShootEnemy : public StaticHumanEnemy
{
public:
	HelicopterType type;

	HelicopterShootEnemy(string jsonFile, string atlasFile, float scale);
	static HelicopterShootEnemy* create(float scale, HelicopterType type);
	void move(Point posOfSoldier);
	virtual void shoot(Point posOfHero);
	virtual void die();
	virtual void updateEnemy(float dt, Point cameraPoint, Point posOfHero);
//	virtual void updateBullet(Point cameraPoint);
//	virtual void createPool(int count);
	//	void setAngle(float radian);
	//virtual bool checkOutScreen(Point posCamera);
};

#endif // __ENEMY_H__
