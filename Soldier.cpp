#include "Soldier.h"


Soldier::Soldier(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
	canShoot = INT_MAX;
}

Soldier * Soldier::create(string jsonFile, string atlasFile, float scale)
{
	Soldier* soldier = new Soldier(jsonFile, atlasFile, scale);
	soldier->setTag(TAG_SOLDIER);
	soldier->update(0.0f);
	soldier->sizeSoldier = soldier->getBoundingBox().size;
	soldier->setTimeScale(1.5f);
	soldier->health = 5;
	soldier->jump_vel = soldier->SCREEN_SIZE.height * (4.0f / 3.0f) / PTM_RATIO;
	soldier->move_vel = soldier->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	soldier->cur_state = IDLE_SHOOT;
	soldier->facingRight = true;
	soldier->canShoot = 1;
	soldier->angle = 0;
	soldier->isNoDie = -240;
	return soldier;
}



void Soldier::updateSoldier(float dt)
{
	if (isNoDie < 0) { 
		isNoDie++; 
		if (isNoDie >= 0) {
			changeBodyBitMask(BITMASK_SOLDIER);
		}
	}
	this->setPositionX(body->GetPosition().x * PTM_RATIO);
	this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeSoldier.height / 2.0f);
	//log("angle: %f", angle);
	this->shoot(angle);

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

void Soldier::changeBodyBitMask(uint16 mask)
{
	auto fixture = this->body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();

	filter.categoryBits = mask;
	//and set it back
	fixture->SetFilterData(filter);
}

Point Soldier::getGunLocation()
{
	auto gun = findBone("shoot");
	auto pos = Vec2(this->getScaleX()*gun->worldX, gun->worldY);
	//pos = convertToWorldSpace(pos);
	//float scale = character->getScale();
	pos = pos + this->getPosition();
	return pos;
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

void Soldier::die(Point posOfCammera)
{
	if (isNoDie >= 0) {
		this->facingRight = true;
		this->setScaleX(1);
		this->cur_state = JUMPING;
		this->body->SetLinearVelocity(b2Vec2(0, 0));
		this->body->SetTransform(b2Vec2((posOfCammera.x - SCREEN_SIZE.width * 0.35f) / PTM_RATIO,
			SCREEN_SIZE.height / PTM_RATIO), this->body->GetAngle());

		this->isNoDie = -240;
		this->changeBodyBitMask(BITMASK_ENEMY);
		auto blink = CCBlink::create(1, 4);
		auto sequence = Sequence::create(blink, blink, blink, blink, nullptr);
		this->runAction(sequence);
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

void Soldier::createPool()
{
	bulletPool=CCArray::createWithCapacity(MAX_BULLET_HERO_POOL);
	bulletPool->retain();
	indexBullet = 0;
	for (int i = 0; i < MAX_BULLET_HERO_POOL; i++) {
		auto bullet = BulletOfHero::create(this->getScale());
		bullet->setPosition(INT_MAX, INT_MAX);
		bullet->body = nullptr;
		//bullet->fixtureDef.filter.categoryBits = BITMASK_BULLET_HERO;
		//bullet->fixtureDef.filter.maskBits = BITMASK_ENEMY|BITMASK_ITEM_;

		//bullet->initPhysic(world, Point(INT_MAX, INT_MAX));
		//bullet->body->SetType(b2_staticBody);
		this->getParent()->addChild(bullet);
		bulletPool->addObject(bullet);
	}

	indexBullet = 0;
}


void Soldier::shoot(float radian)
{
	if (canShoot < INT_MAX) {
		if (!canShoot&& bulletPool != nullptr) {
			auto bullet = (BulletOfHero*)bulletPool->getObjectAtIndex(indexBullet);
			//bullet->body->SetTransform(this->body->GetPosition()+b2Vec2(this->getBoundingBox().size.width/PTM_RATIO,0), 0);
			//bullet->setPosition(this->getPosition() + Point(this->getBoundingBox().size.width / PTM_RATIO, this->getBoundingBox().size.height/2));
			bullet->setPosition(this->getGunLocation());
			bullet->fixtureDef.filter.categoryBits = BITMASK_BULLET_HERO;
			bullet->fixtureDef.filter.maskBits = BITMASK_ENEMY|BITMASK_ITEM_;
			bullet->initPhysic(this->body->GetWorld(), bullet->getPosition());

			bullet->setAngel(radian);
			indexBullet++;
			if (indexBullet == MAX_BULLET_HERO_POOL) {
				indexBullet = 0;
			}
		}
		canShoot++;
		if (canShoot == 30) {
			canShoot = 0;
		}
	}
}