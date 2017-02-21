#ifndef __MINIFORT_H__
#define __MINIFORT_H__

#include "StaticHumanEnemy.h"
#include "BulletOfEnemy.h"

USING_NS_CC;

using namespace spine;

class MiniFort : public StaticHumanEnemy
{
public:
	Sprite *barrel;

	MiniFort(string jsonFile, string atlasFile, float scale);
	static MiniFort* create(float scale);
	virtual void getHit();
	virtual	void shoot(Point posOfHero);
	virtual	bool checkOutScreen(Point posCamera);
	virtual void die();
	void createBarrel();
	Vec2 getGunLocation();
	void updateEnemy(float dt, Point cameraPoint, Point posOfHero);
	
};

#endif // __ENEMY_H__
