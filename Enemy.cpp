#include "Enemy.h"

Enemy::Enemy(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
}

Enemy * Enemy::create(string jsonFile, string atlasFile, float scale)
{
	Enemy *e = new Enemy(jsonFile, atlasFile, scale);
	e->setTag(ENEMY);
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(-1);
	e->facingRight = false;
	return e;
}

void Enemy::move()
{
	auto cur_velY = body->GetLinearVelocity().y;
	body->SetLinearVelocity(b2Vec2(-move_vel, cur_velY));
}

void Enemy::shoot()
{
}

void Enemy::shootUp()
{
}

void Enemy::shootUpLeft()
{
}

void Enemy::shootUpRight()
{
}

void Enemy::updateEnemy(float dt)
{
	this->setPositionX(body->GetPosition().x * PTM_RATIO);
	this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeEnemy.height / 2);

	if (type == Type::MOVE) {
		move();
	}
	else {
		log("BBB");
	}
}