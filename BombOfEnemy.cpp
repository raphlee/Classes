#include "BombOfEnemy.h"



BombOfEnemy::BombOfEnemy()
{
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
	bullet->setScale((Director::getInstance()->getVisibleSize().height / 30) / bullet->getContentSize().height);
	return bullet;
}

