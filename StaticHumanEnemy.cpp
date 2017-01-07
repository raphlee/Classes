#include "StaticHumanEnemy.h"
#include "BulletOfEnemy.h"

StaticHumanEnemy::StaticHumanEnemy(string jsonFile, string atlasFile, float scale) : Enemy(jsonFile, atlasFile, scale)
{
	checkCanShoot = 100;
}



StaticHumanEnemy * StaticHumanEnemy::create(float scale)
{
	StaticHumanEnemy *e = new StaticHumanEnemy("enemy-soldier/soldier.json", "enemy-soldier/soldier.atlas", scale);
	e->setTag(TAG_ENEMY);
	e->isDie = false;
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(-1);
	e->facingRight = false;
	e->setAnimation(0, "standing-shoot", false);
	//e->setTag(TAG_ENEMY);
	e->indexBullet = -1;
	return e;
}

void StaticHumanEnemy::shoot(Point posOfHero)
{
	posOfHero = posOfHero - this->getParent()->getPosition();
	auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
	bullet->isDie = false;
	bullet->body->SetTransform(this->body->GetPosition(), 0);
	//

	indexBullet++;
	if (indexBullet == MAX_BULLET_SOLDIER_ENEMY_POOL) {
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
				this->setAnimation(0, "standing-shoot", false);
				bullet->setAngel(PI);
			}
			else {
				this->setScaleX(1);
				this->clearTracks();
				this->setAnimation(0, "standing-shoot", false);
				bullet->setAngel(0);
			}
		}
		else if (fabs(tmp) >= 0.5f && fabs(tmp) <= 2.0f) {
			if (thisToHero.x < 0) {
				if (thisToHero.y < 0) {
					this->setScaleX(-1);
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-45down", false);
					bullet->setAngel(PI * 5 / 4);
				}
				else {
					this->setScaleX(-1);
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-45up", false);
					bullet->setAngel(PI * 3 / 4);
				}
			}
			else {
				if (thisToHero.y < 0) {
					this->setScaleX(1);
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-45down", false);
					bullet->setAngel(-PI / 4);
				}
				else {
					this->setScaleX(1);
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-45up", false);
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
			else {
				this->clearTracks();
				this->setAnimation(0, "standing-shoot-down", false);
				bullet->setAngel(-PI / 2);
			}
		}
		/*else if (tmp < 0 && fabs(tmp) > 1 / 2 && fabs(tmp) < 2) {
			this->setScaleX(-1);
			this->clearTracks();
			this->setAnimation(0, "standing-shoot-45up", false);
			checkCanShoot = false;
		}
		else if(fabs(tmp) > 2){
			this->clearTracks();
			this->setAnimation(0, "standing-shoot-up", false);
			checkCanShoot = false;
		}
		else if (tmp > 0 && fabs(tmp) > 1 / 2 && fabs(tmp) < 2) {
			this->setScaleX(1);
			this->clearTracks();
			this->setAnimation(0, "standing-shoot-45up", false);
			checkCanShoot = false;
		}*/
	}

}


void StaticHumanEnemy::die()
{
	this->body->SetTransform(b2Vec2(INT_MAX, INT_MAX), 0);
	//this->updateEnemy();
	auto world = this->body->GetWorld();
	world->DestroyBody(body);
	this->body = nullptr;
	for (int i = 0; i < bulletPool->count(); i++) {
		auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(i);
		auto world = bullet->body->GetWorld();
		world->DestroyBody(bullet->body);
		bullet->setVisible(false);
		//bullet->setPosition(INT_MAX, INT_MAX);
		//bullet->release();
	}
	this->bulletPool->removeAllObjects();
	this->bulletPool->release();
	this->setVisible(false);
	//this->setPosition(INT_MAX, INT_MAX);
}


void StaticHumanEnemy::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	
	if (!isDie) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getParent()->getPositionY() - sizeEnemy.height / 2);;
		checkCanShoot++;
		if (checkCanShoot == 120) {
			checkCanShoot = 0;
			shoot(posOfHero);
		}
		if (this->indexBullet>=0) {
			for (int i = 0; i < bulletPool->count(); i++) {
				auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(i);
				if (bullet->isDie) {
					bullet->body->SetTransform(b2Vec2(INT_MAX, INT_MAX), 0);
					bullet->isDie = false;
				}
				bullet->update(dt);
			}
		}
	}
	else {
		if (this->body != nullptr) {
			this->die();
		}
	}
}
/*else {
	this->die();
}
}*/


void StaticHumanEnemy::createPool()
{
	bulletPool = CCArray::createWithCapacity(MAX_BULLET_SOLDIER_ENEMY_POOL);
	bulletPool->retain();
	for (int i = 0; i < MAX_BULLET_SOLDIER_ENEMY_POOL; i++) {
		auto bullet = BulletOfEnemy::create(1);
		bullet->setPosition(INT_MAX, INT_MAX);
		this->getParent()->addChild(bullet);
		bullet->fixtureDef.filter.categoryBits = BITMASK_BULLET_ENEMY;
		bullet->fixtureDef.filter.maskBits = BITMASK_SOLDIER;
		bullet->initPhysic(this->body->GetWorld(), bullet->getPosition());
		bulletPool->addObject(bullet);
		
	}
	indexBullet = 0;
}


// private function
//20/12
bool StaticHumanEnemy::checkOutScreen(Point posCamera)
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

	if ((fabs((this->getPosition().x + this->getParent()->getPosition().x) - posCamera.x) > screenSize.width / 2) || (this->getPosition().y < 0)) {
		return true;
	}
	return false;
}

void StaticHumanEnemy::resetEnemy()
{
	this->body->SetTransform(b2Vec2(INT_MAX / PTM_RATIO, INT_MIN / PTM_RATIO), 0);
	this->body->SetType(b2_staticBody);
}




