#ifndef __B2_SPRITE_H__
#define __B2_SPRITE_H__

#include "Global.h"
#include "spine/spine-cocos2dx.h"

using namespace spine;

class B2Sprite : public Sprite
{
public:
	B2Sprite();
	~B2Sprite();


	static B2Sprite* create();

	// SCREEN_SIZE
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	b2Body *body;
	b2FixtureDef fixtureDef;

	void initPhysic(b2World *world, Point pos, b2BodyType type);
	void update(float dt);

};

#endif // __B2_SPRITE_H__