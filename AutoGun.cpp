#include "AutoGun.h"

AutoGun::AutoGun(string jsonFile, string atlasFile, float scale) : Enemy(jsonFile, atlasFile, scale)
{
	checkCanShoot = 0;
}



AutoGun * AutoGun::create(float scale)
{
	AutoGun *e = new AutoGun("autogun/autogun.json", "autogun/autogun.atlas", scale);
	e->setTag(TAG_ENEMY);
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(-1);
	e->facingRight = false;
	e->setAnimation(0,"shooting",true);
	e->isDie= false;
	return e;
}

void AutoGun::shoot()
{
	
}


void AutoGun::die()
{
	this->body->SetTransform(b2Vec2(INT_MAX, INT_MAX), 0);
}


void AutoGun::updateEnemy(float dt, Point cameraPoint)
{
	if (!checkOutScreen(cameraPoint)) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getParent()->getPositionY() - sizeEnemy.height / 2);
		checkCanShoot++;
		if (checkCanShoot == 30) {
			checkCanShoot = 0;
		}
		shoot();
	}
	if (isDie) {
		this->die();
		isDie = false;
	}
}

// private function
//20/12
bool AutoGun::checkOutScreen(Point posCamera)
{
	auto screenSize = Director::getInstance()->getVisibleSize();
	if (fabs((this->getPosition().y + this->getParent()->getPosition().y)- posCamera.y)  > screenSize.width/2) {
		return true;
	}
	return false;
}





