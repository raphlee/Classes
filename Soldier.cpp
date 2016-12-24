#include "Soldier.h"


Soldier::Soldier(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
}

Soldier * Soldier::create(string jsonFile, string atlasFile, float scale)
{
	Soldier* soldier = new Soldier(jsonFile, atlasFile, scale);
	soldier->setTag(SOLDIER);
	soldier->update(0.0f);
	soldier->sizeSoldier = soldier->getBoundingBox().size;
	soldier->setTimeScale(1.5f);
	soldier->health = 5;
	soldier->jump_vel = soldier->SCREEN_SIZE.height * (4.0f / 3.0f) / PTM_RATIO;
	soldier->move_vel = soldier->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	soldier->cur_state = IDLE_SHOOT;
	soldier->facingRight = true;


	return soldier;
}



void Soldier::updateSoldier(float dt)
{
	this->setPositionX(body->GetPosition().x * PTM_RATIO);
	this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeSoldier.height / 2.0f);


	switch (cur_state)
	{
	case JUMPING:
		jumping();
		break;
	case IDLE_SHOOT:
		idleShoot();
		break;
	case IDLE_SHOOT_UP:
		idleShootUp();
		break;
	case LYING_SHOOT:
		lyingShoot();
		break;
	case RUNNING_SHOOT:
		runningShoot();
		break;
	case RUNNING_SHOOT_DOWN:
		runningShootDown();
		break;
	case RUNNING_SHOOT_UP:
		runningShootUp();
		break;
	case DIE:
		break;
	}

}


void Soldier::move(Point bgPos)
{
	auto cur_velY = body->GetLinearVelocity().y;

	if (facingRight) {
		this->setScaleX(1);
		body->SetLinearVelocity(b2Vec2(move_vel, cur_velY));
	}
	else {
		this->setScaleX(-1);
		if(bgPos.x - getPosition().x < SCREEN_SIZE.width / 2)
			body->SetLinearVelocity(b2Vec2(-move_vel, cur_velY));
		else
			body->SetLinearVelocity(b2Vec2(0, cur_velY));
	}


}

void Soldier::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "jumping") == 0 && loopCount == 6) {
			cur_state = IDLE_SHOOT;
		}
	});
}

void Soldier::initPhysic(b2World * world, Point pos)
{
	auto origin = Director::getInstance()->getVisibleOrigin();

	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set((pos.x + origin.x) / PTM_RATIO, (pos.y + origin.y) / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;

	body = world->CreateBody(&bodyDef);
	GB2ShapeCache::sharedGB2ShapeCache()->addFixturesToBody(body, "soldier");
}

void Soldier::idleShoot()
{
	if (pre_state != cur_state) {
		
		if (pre_state == LYING_SHOOT) {
			body->SetTransform(body->GetPosition(), 0);
		}

		clearTracks();
		addAnimation(0, "standing-shoot", true);
		setToSetupPose();
		pre_state = IDLE_SHOOT;
	}
}

void Soldier::idleShootUp()
{
	
	if (pre_state != cur_state) {
		if (pre_state == LYING_SHOOT) {
			body->SetTransform(body->GetPosition(), 0);
		}

		clearTracks();

		addAnimation(0, "standing-shoot-up", true);
		setToSetupPose();
		pre_state = IDLE_SHOOT_UP;
	}
}

void Soldier::jumping()
{
	if (pre_state != cur_state) {
		if (pre_state == LYING_SHOOT) {
			body->SetTransform(body->GetPosition(), 0);
		}

		log("Pre - %i", pre_state);
		clearTracks();
		addAnimation(0, "jumping", true);
		this->setToSetupPose();

		pre_state = JUMPING;
	}
}

void Soldier::lyingShoot()
{
	if (pre_state != cur_state) {
		body->SetTransform(body->GetPosition(), -PI/ 2);

		clearTracks();
		addAnimation(0, "lie-shoot", true);
		setToSetupPose();
		pre_state = LYING_SHOOT;
	}
}

void Soldier::runningShoot()
{
	if (pre_state != cur_state) {
		if (pre_state == LYING_SHOOT) {
			body->SetTransform(body->GetPosition(), 0);
		}

		clearTracks();
		addAnimation(0, "running-shoot", true);
		setToSetupPose();
		pre_state = RUNNING_SHOOT;
	}
}

void Soldier::runningShootUp()
{
	if (pre_state != cur_state) {
		if (pre_state == LYING_SHOOT) {
			body->SetTransform(body->GetPosition(), 0);
		}

		clearTracks();
		addAnimation(0, "running-shoot-up", true);
		setToSetupPose();
		pre_state = RUNNING_SHOOT_UP;
	}
}

void Soldier::runningShootDown()
{
	if (pre_state != cur_state) {
		if (pre_state == LYING_SHOOT) {
			body->SetTransform(body->GetPosition(), 0);
		}

		clearTracks();
		addAnimation(0, "running-shoot-down", true);
		setToSetupPose();
		pre_state = RUNNING_SHOOT_DOWN;
	}
}
