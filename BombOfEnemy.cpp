#include "BombOfEnemy.h"



BombOfEnemy::BombOfEnemy()
{
	boom = nullptr;
}

BombOfEnemy::~BombOfEnemy()
{
}

BombOfEnemy * BombOfEnemy::create()
{
	BombOfEnemy *bullet = new BombOfEnemy();
	bullet->initWithSpriteFrameName("bomb.png");
	bullet->setTag(TAG_BOMB_ENEMY);
	bullet->isDie = false;
	bullet->setScale(bullet->SCREEN_SIZE.height / 30.0f / bullet->getContentSize().height);
	return bullet;
}

