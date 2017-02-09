#include "PlaneSoldier.h"


PlaneSoldier::PlaneSoldier(string jsonFile, string atlasFile, float scale) : Soldier(jsonFile, atlasFile, scale)
{

}

PlaneSoldier * PlaneSoldier::create(string jsonFile, string atlasFile, float scale)
{
	PlaneSoldier* plane = new PlaneSoldier(jsonFile, atlasFile, scale);
	plane->setTag(TAG_SOLDIER);
	plane->update(0.0f);
	plane->sizeSoldier = plane->getBoundingBox().size;
	plane->setTimeScale(1.5f);
	plane->health = 5;
	plane->move_vel = plane->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	plane->canShoot = 1;

	plane->cur_state = IDLE_SHOOT;
	plane->isOnTheAir = true;

	plane->angle = 0;
	plane->isNoDie = -180;		// time to respawn
	return plane;
}


void PlaneSoldier::initPhysic(b2World * world, Point pos)
{
	b2PolygonShape box_shape;
	box_shape.SetAsBox(sizeSoldier.width / 2.24f / PTM_RATIO, sizeSoldier.height / 5.1f / PTM_RATIO);

	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &box_shape;
	fixtureDef.filter.categoryBits = BITMASK_SOLDIER;
	fixtureDef.filter.maskBits = BITMASK_ENEMY | BITMASK_BULLET_ENEMY | BITMASK_FLOOR | BITMASK_ITEM;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	body->SetGravityScale(0);
}

void PlaneSoldier::die(Point posOfCammera)
{
	if (isNoDie >= 0) {
		this->cur_state = IDLE_SHOOT;

		this->isNoDie = -180;
		this->changeBodyBitMask(BITMASK_ENEMY);
		auto blink = CCBlink::create(1, 3);
		auto visible = CallFunc::create([=] {
			this->setVisible(true);
		});
		auto sequence = Sequence::create(blink, blink, blink, visible, nullptr);
		this->runAction(sequence);;
	}
}

void PlaneSoldier::idle()
{
	if (pre_state != cur_state) {

	}
}

void PlaneSoldier::idleShoot()
{
	clearTracks();
	addAnimation(0, "flying-shoot", true);
	setToSetupPose();
}

void PlaneSoldier::createBombPool()
{
	bombPool = CCArray::createWithCapacity(MAX_BOMB_HERO_POOL);
	bombPool->retain();
	for (int i = 0; i < MAX_BOMB_HERO_POOL; i++) {
		auto bomb = BombOfSoldier::create();
		bomb->body = nullptr;
		bombPool->addObject(bomb);
	}
	indexBomb = 0;
}

void PlaneSoldier::createBomb()
{
	auto bomb = (BombOfSoldier*)bombPool->getObjectAtIndex(indexBomb);
	bomb->setPosition(getPositionX(), getPositionY() - sizeSoldier.height / 2);
	bomb->setVisible(true);
	bomb->initPhysic(this->body->GetWorld(), bomb->getPosition());

	if (!bomb->isAdded) {
		this->getParent()->addChild(bomb, ZORDER_BULLET);
		bomb->isAdded = true;
	}


	indexBomb++;
	if (indexBomb == MAX_BOMB_HERO_POOL) {
		indexBomb = 0;
	}
}

void PlaneSoldier::shoot(float radian)
{
	if (canShoot < INT_MAX) {
		if (!canShoot) {
			if (!canShoot && bulletPool != nullptr) {
				createBullet(radian, getGunLoc("bshoot"));
				createBullet(radian, getGunLoc("fshoot"));
			}

			canShoot = 1;
		}
	}
}

void PlaneSoldier::dropLittleBoy()
{
	canDrop++;
	if (canDrop >= 120 && bombPool != nullptr) {
		createBomb();
		canDrop = 0;
	}
}

Point PlaneSoldier::getGunLoc(string boneName)
{
	auto gun = findBone(boneName);
	auto pos = Vec2(this->getScaleX()*gun->worldX, gun->worldY);
	pos = pos + this->getPosition();
	return pos;
}


void PlaneSoldier::updateHero(float dt)
{
	if (isNoDie < 0) {
		isNoDie++;
		if (isNoDie >= 0) {
			changeBodyBitMask(BITMASK_SOLDIER);
		}
	}

	this->setPositionX(body->GetPosition().x * PTM_RATIO);
	this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeSoldier.height / 3.2f);

	dropLittleBoy();

	// update canshoot
	if (canShoot) {
		canShoot++;
		if (canShoot == 20) {
			canShoot = 0;
		}

	}
}

