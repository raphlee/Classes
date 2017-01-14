#ifndef __ITEM_H__
#define __ITEM_H__

#include "B2Sprite.h"

enum TYPE
{
	TANK,
	HELICOPTER,
	PLANE,
	HEALTH,
	MULT_BULLET,
	FAST_BULLET,
	ORBIT_BULLET
};

class Item : public B2Sprite
{
public:

	static Item* create(string frameName, TYPE type);

	TYPE type;
	bool isTaken;
	Size size;

	// SCREEN_SIZE
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	void initPhysic(b2World *world, Point pos, b2BodyType type);
	void update(float dt);
};

#endif // __ITEM_H__