#include "B2Sprite.h"

B2Sprite::B2Sprite()
{
}

B2Sprite::~B2Sprite()
{
}

B2Sprite * B2Sprite::create()
{
	return nullptr;
}


void B2Sprite::initPhysic(b2World *world, Point pos, b2BodyType type)
{
	//------
	b2BodyDef bodyDef;	// defination of body like: shape,...// first
	b2PolygonShape shape;

	auto size = this->getBoundingBox().size;
	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	bodyDef.type = type;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

}

void B2Sprite::update(float dt)
{
	this->setPositionX(body->GetPosition().x * PTM_RATIO);
	this->setPositionY(body->GetPosition().y * PTM_RATIO);
	this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));	// rotate
}



