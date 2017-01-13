#include "HelicopterEnemy.h"

HelicopterEnemy::HelicopterEnemy(string jsonFile, string atlasFile, float scale) : StaticHumanEnemy(jsonFile, atlasFile, scale)
{
}




HelicopterEnemy * HelicopterEnemy::create(float scale, HelicopterType type)
{
	HelicopterEnemy *e = new HelicopterEnemy("tank/tank.json", "tank/tank.atlas", scale);
	e->setTag(TAG_ENEMY_TANK);
	e->type = type;
	switch (type) {
	case HelicopterType::STUPID:
	{
		e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
		e->setAnimation(0, "flying", false);
		break;
	}

	case HelicopterType::SMART:
	{
		e->move_vel = e->SCREEN_SIZE.width / 5 / PTM_RATIO;;
		e->setAnimation(0, "flying", false);
		break;
	}
	}
	e->isDie = false;
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	//e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(1);
	e->facingRight = false;
	//e->setTag(TAG_ENEMY);
	//e->setVisible(0);
	e->indexBullet = -1;
	return e;
	
}

void HelicopterEnemy::move(Point posOfSoldier)
{
	switch (type) {
	case HelicopterType::STUPID:
	{
		if (body != nullptr)
			this->body->SetLinearVelocity(b2Vec2(-move_vel, this->body->GetLinearVelocity().y));
		break;
	}
	

	case HelicopterType::SMART:
	{
		if (body != nullptr) {
			//auto tmp = CCRANDOM_0_1();
			if (this->checkCanShoot == 60) {
				auto vec = posOfSoldier - (this->getPosition()+this->getParent()->getPosition());
				if (vec.x <= 0) {
					move_vel = -SCREEN_SIZE.width / 5 / PTM_RATIO;
					//this->body->SetLinearVelocity(b2Vec2(-SCREEN_SIZE.width / 5 / PTM_RATIO, this->body->GetLinearVelocity().y));
				}
				else {
					move_vel = SCREEN_SIZE.width / 5 / PTM_RATIO;
					//this->body->SetLinearVelocity(b2Vec2(SCREEN_SIZE.width / 5 / PTM_RATIO, this->body->GetLinearVelocity().y));
				}
			}
			this->body->SetLinearVelocity(b2Vec2(move_vel, this->body->GetLinearVelocity().y));
		}

		break;
	}

	}
}

void HelicopterEnemy::shoot(Point posOfHero)
{
	switch (type) {
	case HelicopterType::STUPID:
	{

		break;
	}
	
	case HelicopterType::SMART:
	{
		posOfHero = posOfHero - this->getParent()->getPosition();
		auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
		bullet->isDie = false;
		//bullet->body->SetTransform(this->body->GetPosition(), 0);
		//
		auto gun = findBone("eff0");
		auto pos = Vec2(this->getScaleX()*gun->worldX, gun->worldY);
		//pos = convertToWorldSpace(pos);
		//float scale = character->getScale();
		pos = pos + this->getPosition() + this->getParent()->getPosition();
		bullet->initPhysic(this->body->GetWorld(), b2Vec2(pos.x / PTM_RATIO, pos.y / PTM_RATIO));
		bullet->setVisible(true);
		/*bullet->initPhysic(this->body->GetWorld(), this->body->GetPosition());
		bullet->setVisible(true);*/

		indexBullet++;
		if (indexBullet == MAX_BULLET_TANK_POOL) {
			indexBullet = 0;
		}
		if (!checkCanShoot) {
			auto thisToHero = posOfHero - this->getPosition();
			auto tmp = thisToHero.y / thisToHero.x;// vecto toi hero
			log("Fabs this to hero: %f", fabs(tmp));

			if (fabs(tmp) < 0.5f) {
				if (thisToHero.x < 0) {
					this->setScaleX(-1);
					this->clearTracks();
					this->setAnimation(0, "running-shoot", false);
					bullet->setAngel(PI);
				}
				else {
					this->setScaleX(1);
					this->clearTracks();
					this->setAnimation(0, "running-shoot", false);
					bullet->setAngel(0);
				}
			}
			else if (fabs(tmp) >= 0.5f && fabs(tmp) <= 2.0f) {
				if (thisToHero.x < 0) {
					if (thisToHero.y < 0) {
						this->setScaleX(-1);
						this->clearTracks();
						this->setAnimation(0, "running-shoot-down", false);
						bullet->setAngel(PI * 5 / 4);
					}
					else {
						this->setScaleX(-1);
						this->clearTracks();
						this->setAnimation(0, "running-shoot-up", false);
						bullet->setAngel(PI * 3 / 4);
					}
				}
				else {
					if (thisToHero.y < 0) {
						this->setScaleX(1);
						this->clearTracks();
						this->setAnimation(0, "running-shoot-down", false);
						bullet->setAngel(-PI / 4);
					}
					else {
						this->setScaleX(1);
						this->clearTracks();
						this->setAnimation(0, "running-shoot-up", false);
						bullet->setAngel(PI / 4);
					}
				}
			}
			else if (fabs(tmp) > 2.0f) {
				if (thisToHero.y > 0) {
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-up", false);
					bullet->setAngel(PI / 2);
				}
			}
			break;
		}

	}
	}
}



void HelicopterEnemy::die()
{
	auto world = this->body->GetWorld();
	if (world->IsLocked()) return;
	world->DestroyBody(body);
	this->body = nullptr;
	//this->setVisible(false);
	this->clearTracks();
	this->setAnimation(0, "destroy", false);
	auto callFunc = CallFunc::create([&]() {
		this->setVisible(false);//removeFromParentAndCleanup(true);
	});

	this->runAction((Sequence::create(DelayTime::create(0.5f), callFunc, nullptr)));
}

void HelicopterEnemy::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	StaticHumanEnemy::updateEnemy(dt, cameraPoint, posOfHero);
	move(posOfHero);
}


bool HelicopterEnemy::checkOutScreen(Point posCamera)
{
	auto screenSize = Director::getInstance()->getVisibleSize();
	/*log("begin");
	log("Pos Cam: %f", posCamera.x);
	log("Pos Parent: %f", this->getParent()->getPosition().x);
	log("Pos THis: %f, %f", this->getPosition().x, this->getPosition().y);
	log("FABS: %f", fabs((this->getPosition().x + this->getParent()->getPosition().x) - posCamera.x));
	log("Screne/2: %f", screenSize.width / 2);
	log("End\n");*/

	if ((this->getPosition().y < 0)) {
		log("This fucking bug!");
	}

	if ((fabs((this->getPosition().x + this->getParent()->getPosition().x) - posCamera.x) > screenSize.width*2/3 ) || (this->getPosition().y < 0)) {
		return true;
	}
	return false;
}

/*void TankEnemy::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{

}*/


//void TankEnemy::updateEnemy(float dt, Point cameraPoint)
//{
//	Enemy::updateEnemy(dt);
//	//this->setPositionX(body->GetPosition().x * PTM_RATIO);
//	//this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeEnemy.height / 2);
//	//if(this->body->Get)
//	//if(!isDie)
//	move();
//	
//	shoot();
//	if (checkOutScreen(cameraPoint))
//		if(body!= nullptr)
//			resetEnemy();
//	if (isDie) {
//		resetEnemy();
//		isDie = false;
//	}
//}

// private function
//20/12
//bool TankEnemy::checkOutScreen(Point posCamera)
//{
//	auto screenSize = Director::getInstance()->getVisibleSize();
//	if (this->getPosition().y < 0 || this->getPosition().x < (posCamera.x - screenSize.width / 2)) {
//		return true;
//	}
//	return false;
//}

