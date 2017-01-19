#include "AutoGun.h"
//#include "Global.h"

AutoGun::AutoGun(string jsonFile, string atlasFile, float scale) : StaticHumanEnemy(jsonFile, atlasFile, scale)
{
	checkCanShoot = 0;
}



AutoGun * AutoGun::create(float scale)
{
	AutoGun *e = new AutoGun("autogun/autogun.json", "autogun/autogun.atlas", scale);
	e->setTag(TAG_ENEMY_AUTOGUN);
	e->isDie = false;
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	//e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(-1);
	e->facingRight = false;
	//e->setAnimation(0, "standing-shoot", false);
	//e->setTag(TAG_ENEMY);
	e->setVisible(0);
	e->indexBullet = -1;
	return e;
}

void AutoGun::shoot()
{
	auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
	bullet->isDie = false;
	this->clearTracks();
	this->setAnimation(0, "shooting", false);
	//bullet->body->SetTransform(this->body->GetPosition(), 0);
	//

	bullet->initPhysic(this->body->GetWorld(), this->body->GetPosition());
	//bullet->initPhysic(world, this->body->GetPosition());
	bullet->setVisible(true);

	indexBullet++;
	if (indexBullet == MAX_BULLET_AUTOGUN_POOL) {
		indexBullet = 0;
	}
	bullet->setAngel(PI);
}


void AutoGun::die()
{
	Enemy::die();
	isDie = false;
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


void AutoGun::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	if (!checkOutScreen(cameraPoint)  && body !=nullptr) {
		if (!this->isVisible()) {
			this->setVisible(true);
			this->setAnimation(0, "raising", false);
			auto call = CallFunc::create([&]() {
				this->changeBodyBitMask(BITMASK_ENEMY);
			});
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), call));
		}
		if (body != nullptr) {
			this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
			this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getParent()->getPositionY() - sizeEnemy.height / 2);
		}
		checkCanShoot++;
		if (checkCanShoot >= 30 && checkCanShoot <= 40 && checkCanShoot%10 == 0 ) {
			
			shoot();
		}
		if (checkCanShoot == 240) {
			checkCanShoot = 0;
		}
	}
	if (isDie) {
		this->die();
		//isDie = false;
	}
	//updateBullet(cameraPoint);
}

// private function
//20/12
bool AutoGun::checkOutScreen(Point posCamera)
{
	auto screenSize = Director::getInstance()->getVisibleSize();
	// truong hop dac biet khong gian check nho honss
	if (fabs((this->getPosition().x + this->getParent()->getPosition().x)- posCamera.x)  > screenSize.width / 2) {
		return true;
	}
	return false;
}





