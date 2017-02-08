#ifndef __FORT_H__
#define __FORT_H__

#include "MiniFort.h"
#include "BulletOfEnemy.h"

USING_NS_CC;

using namespace spine;

class Fort : public MiniFort
{
public:

	Fort(string jsonFile, string atlasFile, float scale);
	static Fort* create(float scale);
	void shoot(float angle);
	void getHit();
	void updateEnemy(float dt, Point cameraPoint, Point posOfHero);
	//void updateBullet();
	
};

#endif // __ENEMY_H__
