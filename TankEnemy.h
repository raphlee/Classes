#ifndef __TANK_ENEMY_H__
#define __TANK_ENEMY_H__

#include "StaticHumanEnemy.h"
#include "BulletOfEnemy.h"

USING_NS_CC;

using namespace spine;

enum TankType {
	STUPID,
	NORMAL,
	SMART
};

class TankEnemy : public StaticHumanEnemy
{
public:
	TankType type;

	TankEnemy(string jsonFile, string atlasFile, float scale);
	static TankEnemy* create(float scale, TankType type);
	void move(Point posOfSoldier);
	void getHit();
	void shoot(Point posOfHero);
	void die();
	void updateEnemy(float dt, Point cameraPoint, Point posOfHero);
//	void updateBullet(Point cameraPoint);
//	void createPool(int count);
//	void setAngle(float radian);
	virtual bool checkOutScreen(Point posCamera);
};

#endif // __ENEMY_H__
