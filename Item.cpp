#include "Item.h"

Item * Item::create(string frameName, TYPE type)
{
	Item *i = new Item();
	i->initWithSpriteFrameName(frameName);
	i->setTag(TAG_ITEM);
	i->type = type;
	i->isTaken = false;
	return i;
}

void Item::initPhysic(b2World * world, Point pos, b2BodyType type)
{
	//------
	b2BodyDef bodyDef;	// defination of body like: shape,...// first
	b2CircleShape shape;
	
	shape.m_radius = getBoundingBox().size.width / 2 / PTM_RATIO;

	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.3f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_ITEM;
	fixtureDef.filter.maskBits = BITMASK_SOLDIER | BITMASK_FLOOR | BITMASK_BLINK;

	bodyDef.type = type;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void Item::update(float dt)
{
	if (body != nullptr && this->getParent() != nullptr) {
		setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		setPositionY(body->GetPosition().y * PTM_RATIO - this->getParent()->getPositionY());
	}
}

