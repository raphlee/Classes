#include "HelicopterSoldier.h"


HelicopterSoldier::HelicopterSoldier(string jsonFile, string atlasFile, float scale) : Soldier(jsonFile, atlasFile, scale)
{

}

HelicopterSoldier * HelicopterSoldier::create(string jsonFile, string atlasFile, float scale)
{
	HelicopterSoldier* h = new HelicopterSoldier(jsonFile, atlasFile, scale);
	h->setTag(TAG_SOLDIER);
	h->update(0.0f);
	h->sizeSoldier = h->getBoundingBox().size;
	h->setTimeScale(1.5f);
	h->health = 10000;
	h->move_vel = h->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	h->canShoot = 1;

	h->cur_state = IDLE_SHOOT;
	h->isOnTheAir = true;

	h->angle = 0;
	h->isNoDie = -180;		// time to respawn
	return h;
}


void HelicopterSoldier::initPhysic(b2World * world, Point pos)
{
	initCirclePhysic(world, pos);
	body->SetGravityScale(0);		// prevent helicopter going down
	fixtureDef.filter.categoryBits = BITMASK_SOLDIER;
	fixtureDef.filter.maskBits = BITMASK_ENEMY | BITMASK_BULLET_ENEMY | BITMASK_FLOOR;
}

void HelicopterSoldier::die(Point posOfCammera)
{
	if (isNoDie >= 0) {
		isNoDie = -180;
		cur_state = IDLE_SHOOT;
		changeBodyBitMask(BITMASK_ENEMY);
		auto blink = CCBlink::create(1, 3);
		auto visible = CallFunc::create([=] {
			this->setVisible(true);
		});
		auto sequence = Sequence::create(blink, blink, blink, visible, nullptr);
		this->runAction(sequence);
	}
}

void HelicopterSoldier::idleShoot()
{
	if (pre_state != cur_state) {

		clearTracks();
		addAnimation(0, "flying", true);
		setToSetupPose();
		pre_state = IDLE_SHOOT;
	}
}

void HelicopterSoldier::shoot(float radian)
{
	if (canShoot < INT_MAX) {
		if (isFirstShoot) {
			createBullet(radian, Point(getPositionX() + sizeSoldier.height * 0.54f, getPositionY() + sizeSoldier.height * 0.5f));
			createBullet(radian, Point(getPositionX() + sizeSoldier.height * 0.8f, getPositionY() + sizeSoldier.height * 0.35f));
			createBullet(radian, Point(getPositionX() + sizeSoldier.height * 0.4f, getPositionY() + sizeSoldier.height * 0.18f));
			isFirstShoot = false;
			canShoot = 1;
		}

		if (!canShoot && bulletPool != nullptr) {
			createBullet(radian, Point(getPositionX() + sizeSoldier.height * 0.5f, getPositionY() + sizeSoldier.height * 0.4f));
			createBullet(radian, Point(getPositionX() + sizeSoldier.height * 0.73f, getPositionY() + sizeSoldier.height * 0.25f));
			createBullet(radian, Point(getPositionX() + sizeSoldier.height * 0.51f, getPositionY() + sizeSoldier.height * 0.16f));
		}

		canShoot++;
		if (canShoot == 30) {
			canShoot = 0;
		}
	}
}

void HelicopterSoldier::updateHero(float dt)
{
	if (isNoDie < 0) {
		isNoDie++;
		if (isNoDie >= 0) {
			changeBodyBitMask(BITMASK_SOLDIER);
		}
	}

	this->setPositionX(body->GetPosition().x * PTM_RATIO);
	this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeSoldier.height / 2.0f);

	//shoot(0);
}

