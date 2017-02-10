#include "BulletOfHero.h"



BulletOfHero::BulletOfHero()
{
}

BulletOfHero::~BulletOfHero()
{
}

BulletOfHero * BulletOfHero::create()
{
	BulletOfHero *bullet = new BulletOfHero();
	bullet->initWithSpriteFrameName("soldier-bullet.png");
	bullet->setTag(TAG_BULLET_HERO);
	bullet->alpha = PI;
	bullet->type = Type::normal;
	bullet->setScale(bullet->SCREEN_SIZE.height / 20.0 / bullet->getContentSize().height);
	bullet->isDie = false;
	bullet->setScale((Director::getInstance()->getVisibleSize().height/20)/bullet->getContentSize().height);
	return bullet;
}


void BulletOfHero::update(float dt)
{
	Bullet::update(dt);
	// r = screensize.height , T = 2, 
	if (body != nullptr) {
		//auto vLenght = PI * SCREEN_SIZE.height;
		auto vLenght = SCREEN_SIZE.width/3*PI;
		auto tmp = PI / 8;
		alpha += tmp;
		if (type == Type::circle) {
			auto vx = vLenght * cos(alpha)/PTM_RATIO;
			auto vy = -vLenght * sin(alpha)/ PTM_RATIO *cos(radian);
			this->body->SetLinearVelocity(b2Vec2(vx, vy)+ b2Vec2(SCREEN_SIZE.width/2/PTM_RATIO*cosf(radian), SCREEN_SIZE.width/2  / PTM_RATIO*sinf(radian)));
		}
		if (alpha > 2 * PI) alpha = 0;
	}
}

