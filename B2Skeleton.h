#ifndef __B2_SKELETON_H__
#define __B2_SKELETON_H__

#include "Global.h"
#include <spine/spine-cocos2dx.h>

using namespace spine;

class B2Skeleton : public SkeletonAnimation
{
public:

	B2Skeleton(string jsonFile, string atlasFile, float scale);

	static B2Skeleton* create(string jsonFile, string atlasFile, float scale);

	// SCREEN_SIZE
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	b2Body *body;
	//------
	b2BodyDef bodyDef;	// defination of body like: shape,...// first
	b2PolygonShape box_shape;
	b2CircleShape circle_shape;
	b2FixtureDef fixtureDef;

	int health;
	float move_vel;
	bool facingRight;

	void initBoxPhysic(b2World *world, Point pos);
	void initCirclePhysic(b2World *world, Point pos);

};

#endif // __B2_SKELETON_H__