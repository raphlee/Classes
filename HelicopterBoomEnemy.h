#ifndef __HELICOPTER_BOOM_ENEMY_H__
#define __HELICOPTER_BOOM_ENEMY_H__

#include "StaticHumanEnemy.h"
#include "BulletOfEnemy.h"
//#include "Utility.h"

USING_NS_CC;

using namespace spine;

enum HelicopterBoomType {
	STUPID, // plane boom 
	SMART//plane boom back
};

class HelicopterBoomEnemy : public StaticHumanEnemy
{
public:
	HelicopterBoomType type;

	HelicopterBoomEnemy(string jsonFile, string atlasFile, float scale);
	static HelicopterBoomEnemy* create(float scale, HelicopterBoomType type);
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
