#include "Fort.h"
#include "AudioEngine.h"

Fort::Fort(string jsonFile, string atlasFile, float scale) : MiniFort(jsonFile, atlasFile, scale)
{
	checkCanShoot = 75;
}



Fort* Fort::create(float scale)
{
	Fort *e = new Fort("fort/enemy-gun.json", "fort/enemy-gun.atlas", scale);
	e->setTag(TAG_ENEMY_FORT);
	e->isDie = false;
	e->update(0.0f);
	e->health = 2;
	e->sizeEnemy = e->getBoundingBox().size;
	e->setScaleX(-1);
	e->facingRight = false;
	e->setAnimation(0, "standing-shoot", false);
	e->indexBullet = -1;
	return e;
}

void Fort::shoot(float angle)
{
	/*auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEYSOUND);
	if (checkSound) {
		experimental::AudioEngine::play2d(SOUND_BIG_FORT_SHOOT);
	}*/

	AudioManager::playSound(SOUND_BIG_FORT_SHOOT);
	auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
	bullet->isDie = false;
	//bullet->body->SetTransform(this->body->GetPosition(), 0);
	//
	this->clearTracks();
	this->setAnimation(0, "hit", false);

	bullet->initPhysic(this->body->GetWorld(), this->body->GetPosition());
	bullet->setVisible(true);

	indexBullet++;
	if (indexBullet == MAX_BULLET_FORT_POOL) {
		indexBullet = 0;
	}
	
	bullet->setAngel(angle);
}

void Fort::getHit()
{
	clearTracks();
	addAnimation(0, "hit", false);
	setToSetupPose();
}

void Fort::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	if (!isDie) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getParent()->getPositionY() - sizeEnemy.height / 2);;
		
		if (checkCanShoot % 10 == 0  && checkCanShoot > 119) {
			
			shoot(2*PI*(float)checkCanShoot/60);
			if (checkCanShoot == 180) {
				checkCanShoot = 0;
			}
		}
		checkCanShoot++;
	}
	else {
		if (this->body != nullptr) {
			this->die();
		}
	}
}


