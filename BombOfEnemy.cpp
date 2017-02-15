#include "BombOfEnemy.h"



BombOfEnemy::BombOfEnemy()
{
	boom = nullptr;
}

BombOfEnemy::~BombOfEnemy()
{
}

BombOfEnemy * BombOfEnemy::create()
{
	BombOfEnemy *bullet = new BombOfEnemy();
	bullet->initWithSpriteFrameName("bomb.png");
	bullet->setTag(TAG_BOMB_ENEMY);
	bullet->isDie = false;
	bullet->setScale(bullet->SCREEN_SIZE.height / 30.0f / bullet->getContentSize().height);
	return bullet;
}

void BombOfEnemy::initPhysic(b2World * world, b2Vec2 pos)
{
	auto size = this->getContentSize();

	b2PolygonShape shape;
	shape.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;
	fixtureDef.filter.categoryBits = BITMASK_BOMB_ENEMY;
	fixtureDef.filter.maskBits = BITMASK_SOLDIER | BITMASK_FLOOR;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x, pos.y);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

