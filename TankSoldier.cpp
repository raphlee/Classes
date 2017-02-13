#include "TankSoldier.h"


TankSoldier::TankSoldier(string jsonFile, string atlasFile, float scale) : Soldier(jsonFile, atlasFile, scale)
{

}

TankSoldier * TankSoldier::create(string jsonFile, string atlasFile, float scale)
{
	TankSoldier* tank = new TankSoldier(jsonFile, atlasFile, scale);
	tank->setTag(TAG_SOLDIER);
	tank->update(0.0f);
	tank->sizeSoldier = tank->getBoundingBox().size;
	tank->setTimeScale(1.5f);
	//tank->health = 5;
	tank->jump_vel = tank->SCREEN_SIZE.height * (4.0f / 3.0f) / PTM_RATIO;
	tank->move_vel = tank->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	tank->facingRight = true;
	tank->canShoot = 1;

	tank->defense = 2;
	tank->cur_state = IDLE_SHOOT;
	tank->bulletType = BulletType::Slow;

	tank->angle = 0;
	tank->isNoDie = 0;		// time to respawn
	return tank;
}


void TankSoldier::initPhysic(b2World * world, Point pos)
{
	initCirclePhysic(world, pos);
	fixtureDef.filter.categoryBits = BITMASK_SOLDIER;
	fixtureDef.filter.maskBits = BITMASK_ENEMY | BITMASK_BULLET_ENEMY | BITMASK_FLOOR;
}


Point TankSoldier::getGunLocation()
{
	auto gun = findBone("eff0");
	auto pos = Vec2(this->getScaleX()*gun->worldX, gun->worldY);
	pos = pos + this->getPosition();
	return pos;
}

void TankSoldier::die(Point posOfCammera)
{
	if (defense >= 0) {
		if (isNoDie >= 0) {
			this->isNoDie = -180;
			this->changeBodyBitMask(BITMASK_BLINK);
			auto blink = CCBlink::create(1, 3);
			auto visible = CallFunc::create([=] {
				this->setVisible(true);
			});
			auto sequence = Sequence::create(blink, blink, blink, visible, nullptr);
			this->runAction(sequence);
		}
	}
	
}

//void TankSoldier::idle()
//{
//
//}

void TankSoldier::idleShoot()
{
	if (pre_state != cur_state) {

		clearTracks();
		addAnimation(0, "standing-shoot", true);
		setToSetupPose();
		pre_state = IDLE_SHOOT;
	}
}

void TankSoldier::idleShootUp()
{
	
	if (pre_state != cur_state) {
		clearTracks();

		addAnimation(0, "standing-shoot-up", true);
		setToSetupPose();
		pre_state = IDLE_SHOOT_UP;
	}
}

void TankSoldier::jumping()
{
}

void TankSoldier::lyingShoot()
{
	if (pre_state != cur_state) {

		clearTracks();
		addAnimation(0, "standing-shoot", true);
		setToSetupPose();
		pre_state = LYING_SHOOT;
	}
}


void TankSoldier::runningShoot()
{
	if (pre_state != cur_state) {

		clearTracks();
		//addAnimation(0, "running-shoot", true);
		addAnimation(0, "moving", true);
		setToSetupPose();
		pre_state = RUNNING_SHOOT;
	}
}

void TankSoldier::runningShootUp()
{
	if (pre_state != cur_state) {

		clearTracks();
		addAnimation(0, "running-shoot-up", true);
		setToSetupPose();
		pre_state = RUNNING_SHOOT_UP;
	}
}

void TankSoldier::runningShootDown()
{
	if (pre_state != cur_state) {

		clearTracks();
		addAnimation(0, "running-shoot-down", true);
		setToSetupPose();
		pre_state = RUNNING_SHOOT_DOWN;
	}
}

