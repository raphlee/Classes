#ifndef __AUTOGUN_H__
#define __AUTOGUN_H__

#include "StaticHumanEnemy.h"
#include "BulletOfEnemy.h"

USING_NS_CC;

using namespace spine;

class AutoGun : public StaticHumanEnemy
{
public:

	AutoGun(string jsonFile, string atlasFile, float scale);
	static AutoGun* create(float scale);
	void getHit();
	void shoot();
	void die();
	void updateEnemy(float dt, Point cameraPoint, Point posOfHero);
//protected:
	bool checkOutScreen(Point posCamera);
};

#endif // __ENEMY_H__
