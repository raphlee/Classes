﻿#include "CollisionListener.h"
#include "Enemy.h"
#include "Bullet.h"

CollisionListener::CollisionListener() {

}

CollisionListener::~CollisionListener()
{

}

void CollisionListener::BeginContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	// dùng để tính toán các vị trí contact
	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];


	B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
	B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();

	if ((sA->getTag() == TAG_SOLDIER && sB->getTag() == TAG_FLOOR) ||
		(sB->getTag() == TAG_SOLDIER && sA->getTag() == TAG_FLOOR)
		) {


		auto soldier = sA->getTag() == TAG_SOLDIER ? (Soldier *)sA : (Soldier *)sB;

		if (sA->getTag() != TAG_SOLDIER) {
			auto dentaX = fabs(collidePoint.x - bodyB->GetPosition().x);
			auto radius = (soldier->getBoundingBox().size.width / PTM_RATIO) / 2;
			if (bodyB->GetPosition().y < collidePoint.y || dentaX > radius / 2) {
				contact->SetEnabled(false);
				bodyB->GetFixtureList()->SetSensor(true);
			}
			else {
				soldier->onGround = true;
				log("On ground");
			}
		}
		else {
			auto dentaX = fabs(collidePoint.x - bodyA->GetPosition().x);
			auto radius = (soldier->getBoundingBox().size.width / PTM_RATIO) / 2;
			if (bodyA->GetPosition().y < collidePoint.y || dentaX > radius / 2) {
				contact->SetEnabled(false);
				bodyA->GetFixtureList()->SetSensor(true);
			}
			else {
				soldier->onGround = true;
				log("On ground");
			}
		}

	}
	// neu nguoi va cham enemy 
	else if ((sA->getTag() == TAG_SOLDIER && (sB->getTag() > 100)) ||
		(sB->getTag() == TAG_SOLDIER && (sA->getTag() > 100))) {
		auto soldier = sA->getTag() == TAG_SOLDIER ? (Soldier *)sA : (Soldier *)sB;
		if (soldier->isNoDie >= 0) {
			soldier->cur_state = State::DIE;
		}
		//contact->SetEnabled(false);
	}

	// neu nguoi va cham dan enemy
	else if ((sA->getTag() == TAG_SOLDIER && (sB->getTag() == TAG_BULLET_ENEMY)) ||
		(sB->getTag() == TAG_SOLDIER && (sA->getTag() == TAG_BULLET_ENEMY))) {
		auto soldier = sA->getTag() == TAG_SOLDIER ? (Soldier *)sA : (Soldier *)sB;
		auto bullet = sA->getTag() == TAG_BULLET_ENEMY ? (Bullet *)sA : (Bullet *)sB;
		if (soldier->isNoDie >= 0) {
			soldier->cur_state = State::DIE;
			bullet->isDie = true;
		}

	}

	// neu enemy va cham dan cua hero
	else if ((sA->getTag() == TAG_BULLET_HERO && (sB->getTag() > 100)) ||
		(sB->getTag() == TAG_BULLET_HERO && (sA->getTag() > 100))) {
		auto enemy = sA->getTag() == TAG_ENEMY_SOLDIER ? (Enemy *)sA : (Enemy *)sB;
		auto bullet = sA->getTag() == TAG_BULLET_HERO ? (Bullet *)sA : (Bullet *)sB;
		enemy->isDie = true;
		bullet->isDie = true;
	}

}


// fix here to collision with enemy
void CollisionListener::EndContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
	B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();

	if ((sA->getTag() == TAG_SOLDIER && sB->getTag() == TAG_FLOOR) ||
		(sB->getTag() == TAG_SOLDIER && sA->getTag() == TAG_FLOOR)
		) {
		bodyA->GetFixtureList()->SetSensor(false);
		bodyB->GetFixtureList()->SetSensor(false);
	}
}



