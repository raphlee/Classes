#ifndef __MINIFORT_H__
#define __MINIFORT_H__

#include "StaticHumanEnemy.h"
#include "BulletOfEnemy.h"

USING_NS_CC;

using namespace spine;

class MiniFort : public StaticHumanEnemy
{
public:

	MiniFort(string jsonFile, string atlasFile, float scale);
	static MiniFort* create(float scale);
	void shoot(Point posOfHero);
	bool checkOutScreen(Point posCamera);
	void die();
	//void updateBullet();
	
};

#endif // __ENEMY_H__
