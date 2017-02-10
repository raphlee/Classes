#include "BombOfSoldier.h"

BombOfSoldier::BombOfSoldier()
{
}

BombOfSoldier::~BombOfSoldier()
{
}

BombOfSoldier * BombOfSoldier::create()
{
	BombOfSoldier *bomb = new BombOfSoldier();
	bomb->initWithSpriteFrameName("bomb.png");
	bomb->setTag(TAG_BOMB);
	bomb->setScale(bomb->SCREEN_SIZE.height / 30.0f / bomb->getContentSize().height);
	return bomb;
}

void BombOfSoldier::initPhysic(b2World * world, Point pos)
{
	auto size = this->getContentSize();

	b2PolygonShape shape;
	shape.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;
	fixtureDef.filter.categoryBits = BITMASK_BOMB_HERO;
	fixtureDef.filter.maskBits = BITMASK_ENEMY | BITMASK_FLOOR;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

