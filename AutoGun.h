#ifndef __AUTOGUN_H__
#define __AUTOGUN_H__

#include "Enemy.h"

USING_NS_CC;

using namespace spine;

class AutoGun : public Enemy
{
public:
	int checkCanShoot;

	AutoGun(string jsonFile, string atlasFile, float scale);
	static AutoGun* create(float scale);
	void shoot();
	void die();
	void updateEnemy(float dt, Point cameraPoint);
protected:
	bool checkOutScreen(Point posCamera);
};

#endif // __ENEMY_H__
