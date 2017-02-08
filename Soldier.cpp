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
	soldier->pre_state = IDLE_SHOOT;
	soldier->cur_state = JUMPING;
	soldier->facingRight = true;
	soldier->canShoot = 1;
	soldier->angle = 0;
	soldier->bulletType = BulletType::Slow;
	soldier->isNoDie = -180;
	return soldier;
}



void Soldier::updateHero(float dt)
{
	if (isNoDie < 0) {
		isNoDie++;
		if (isNoDie >= 0) {
			changeBodyBitMask(BITMASK_SOLDIER);
		}
	}

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

	// update canshoot
	if (canShoot) {
		canShoot++;
		if (canShoot == 20) {
			canShoot = 0;
		}
		if (bulletType == BulletType::Fast) {
			if (canShoot > 10) {
				canShoot = 0;
			}
		}

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
	pos = pos + this->getPosition();
	return pos;
}


void Soldier::move()
{
	auto cur_velY = body->GetLinearVelocity().y;

	if (facingRight) {
		this->setScaleX(1);
		body->SetLinearVelocity(b2Vec2(move_vel, cur_velY));
	}
	else {
		this->setScaleX(-1);
		body->SetLinearVelocity(b2Vec2(-move_vel, cur_velY));
		if (isGetOriginX) {
			body->SetLinearVelocity(b2Vec2(0, cur_velY));
		}
	}


}

void Soldier::moveFollow(Point joystickVel)
{
	if (getPositionY() + sizeSoldier.height > SCREEN_SIZE.height && isGetOriginX) {
		if (joystickVel.x < 0 && joystickVel.y > 0)
			body->SetLinearVelocity(b2Vec2(0, 0));
		else if (joystickVel.x < 0) {
			body->SetLinearVelocity(b2Vec2(0, joystickVel.y * move_vel));
		}
		else if (joystickVel.y > 0) {
			body->SetLinearVelocity(b2Vec2(joystickVel.x * move_vel, 0));
		}
	}
	else if (getPositionY() + sizeSoldier.height > SCREEN_SIZE.height) {
		if (joystickVel.y > 0)
			body->SetLinearVelocity(b2Vec2(joystickVel.x * move_vel, 0));
		else
			body->SetLinearVelocity(b2Vec2(joystickVel.x * move_vel, joystickVel.y * move_vel));
	}
	else if (isGetOriginX) {
		if (joystickVel.x < 0)
			body->SetLinearVelocity(b2Vec2(0, joystickVel.y * move_vel));
		else
			body->SetLinearVelocity(b2Vec2(joystickVel.x * move_vel, joystickVel.y * move_vel));

	}
	else
		body->SetLinearVelocity(b2Vec2(joystickVel.x * move_vel, joystickVel.y * move_vel));
}

void Soldier::blinkTrans()
{
	auto blink = CCBlink::create(1, 3);
	auto sequence = Sequence::create(blink, blink, blink, nullptr);
	this->runAction(sequence);
}

void Soldier::die(Point posOfCammera)
{
	if (isNoDie >= 0) {
		facingRight = true;
		setScaleX(1);
		cur_state = JUMPING;
		onGround = false;
		body->SetLinearVelocity(b2Vec2(0, 0));
		body->SetTransform(b2Vec2((posOfCammera.x - SCREEN_SIZE.width * 0.35f) / PTM_RATIO,
			SCREEN_SIZE.height / PTM_RATIO), this->body->GetAngle());


		this->bulletType = BulletType::Slow;

		isNoDie = -180;
		changeBodyBitMask(BITMASK_BLINK);
		auto blink = CCBlink::create(1, 3);
		auto visible = CallFunc::create([=] {
			this->setVisible(true);
		});
		auto sequence = Sequence::create(blink, blink, blink, visible, nullptr);
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
	b2BodyDef bodyDef;

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

		clearTracks();
		addAnimation(0, "jumping", true);
		setToSetupPose();

		pre_state = JUMPING;
	}
}

void Soldier::lyingShoot()
{
	if (pre_state != cur_state) {
		body->SetTransform(body->GetPosition(), -PI / 2);

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
	bulletPool = CCArray::createWithCapacity(MAX_BULLET_HERO_POOL);
	bulletPool->retain();
	for (int i = 0; i < MAX_BULLET_HERO_POOL; i++) {
		auto bullet = BulletOfHero::create(this->getScale());
		bullet->body = nullptr;
		bulletPool->addObject(bullet);
	}

	indexBullet = 0;
}

void Soldier::createBombPool()
{
}


void Soldier::shoot(float radian)
{
	if (canShoot < INT_MAX) {
		if (!canShoot) {
			switch (bulletType)
			{
			case BulletType::Circle:
			case BulletType::Slow: {


				if (!canShoot && bulletPool != nullptr) {
					createBullet(radian, getGunLocation());

				}
				break;
			}
			case BulletType::Fast: {

				if (!(canShoot % 10) && bulletPool != nullptr) {
					createBullet(radian, getGunLocation());

				}

				break;
			}

			case BulletType::Super: {
				
				if (!canShoot && bulletPool != nullptr) {
					createBullet(radian, getGunLocation());
					createBullet(radian - PI / 10, getGunLocation());
					createBullet(radian + PI / 10, getGunLocation());
					createBullet(radian - PI / 5, getGunLocation());
					createBullet(radian + PI / 5, getGunLocation());

				}


				break;
			}
			default: {
				break;
			}
			}
			canShoot = 1;
		}
	}
}

void Soldier::createBullet(float radian, Point posGun)
{
	auto bullet = (BulletOfHero*)bulletPool->getObjectAtIndex(indexBullet);
	
	if (bullet->body != nullptr) {
		auto world = bullet->body->GetWorld();
		world->DestroyBody(bullet->body);
	}
	bullet->setVisible(true);
	bullet->setPosition(posGun);
	bullet->fixtureDef.filter.categoryBits = BITMASK_BULLET_HERO;
	bullet->fixtureDef.filter.maskBits = BITMASK_ENEMY;
	bullet->initPhysic(this->body->GetWorld(), bullet->getPosition());

	if (!bullet->isAdded) {
		this->getParent()->addChild(bullet, ZORDER_BULLET);
		bullet->isAdded = true;
	}

	bullet->setAngel(radian);
	indexBullet++;
	if (indexBullet == MAX_BULLET_HERO_POOL) {
		indexBullet = 0;
	}
	if (this->bulletType == BulletType::Circle) {
		bullet->type = Type::circle;
		bullet->alpha = 3 * PI / 2;
		bullet->radian = radian;
	}
	else {
		bullet->type = Type::normal;
	}

}

