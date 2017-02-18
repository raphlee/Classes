﻿#include "CollisionListener.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Item.h"
#include "BulletOfEnemy.h"
#include "BombOfEnemy.h"

#include "Soldier.h"
#include "Item.h"
#include "BombOfSoldier.h"
#include "BombOfEnemy.h"

CollisionListener::CollisionListener() {

}

CollisionListener::~CollisionListener()
{

}

void CollisionListener::BeginContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	//// dùng để tính toán các vị trí contact
	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];
	//worldManifold.
	//log("sau");


	B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
	B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();


	if ((sA->getTag() == TAG_SOLDIER && sB->getTag() == TAG_FLOOR) ||
		(sB->getTag() == TAG_SOLDIER && sA->getTag() == TAG_FLOOR)
		) {
		auto soldier = sA->getTag() == TAG_SOLDIER ? (Soldier *)sA : (Soldier *)sB;

		if (sA->getTag() != TAG_SOLDIER) {
			auto dentaX = fabs(collidePoint.x - bodyB->GetPosition().x);
			auto radius = (soldier->sizeSoldier.width / PTM_RATIO) / 2;
			if (bodyB->GetPosition().y < collidePoint.y || dentaX > radius / 2) {

			}
			else {
				soldier->onGround = true;
			}
		}
		else {
			auto dentaX = fabs(collidePoint.x - bodyA->GetPosition().x);
			auto radius = (soldier->sizeSoldier.width / PTM_RATIO) / 2;
			if (bodyA->GetPosition().y < collidePoint.y || dentaX > radius / 2) {
			
			}
			else {
				soldier->onGround = true;
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
	}

	// neu nguoi va cham dan enemy
	else if ((sA->getTag() == TAG_SOLDIER && sB->getTag() == TAG_BULLET_ENEMY) ||
			(sB->getTag() == TAG_SOLDIER && sA->getTag() == TAG_BULLET_ENEMY)) {
		auto soldier = sA->getTag() == TAG_SOLDIER ? (Soldier *)sA : (Soldier *)sB;
		auto bullet = sA->getTag() == TAG_BULLET_ENEMY ? (Bullet *)sA : (Bullet *)sB;
		if (soldier->isNoDie >= 0) {
			soldier->cur_state = State::DIE;
			bullet->explosion();
			bullet->isDie = true;
		}
	}

	// neu nguoi va cham item
	else if ((sA->getTag() == TAG_SOLDIER && (sB->getTag() == TAG_ITEM)) ||
		(sB->getTag() == TAG_SOLDIER && (sA->getTag() == TAG_ITEM))) {
		auto item = sA->getTag() == TAG_ITEM ? (Item *)sA : (Item *)sB;
		item->isTaken = true;
	}

	// neu nguoi va cham item khi dang blink
	else if ((sA->getTag() == TAG_BLINK && (sB->getTag() == TAG_ITEM)) ||
		(sB->getTag() == TAG_BLINK && (sA->getTag() == TAG_ITEM))) {
		auto item = sA->getTag() == TAG_ITEM ? (Item *)sA : (Item *)sB;
		item->isTaken = true;
	}

	// neu enemy va cham dan cua hero
	else if ((sA->getTag() == TAG_BULLET_HERO && (sB->getTag() > 100)) ||
		(sB->getTag() == TAG_BULLET_HERO && (sA->getTag() > 100))) {

		auto enemy = sA->getTag() > 100 ? (Enemy *)sA : (Enemy *)sB;
		auto bullet = sA->getTag() == TAG_BULLET_HERO ? (BulletOfHero *)sA : (BulletOfHero *)sB;
		bullet->explosion();
		bullet->isDie = true;
		
		enemy->health -= bullet->damage;
		if(enemy->getTag() != TAG_ENEMY_TANK_STUPID)
			enemy->getHit();
		if (enemy->health <= 0)
			enemy->isDie = true;
	}

	// neu enemy va cham bom cua hero
	else if ((sA->getTag() == TAG_BOMB && (sB->getTag() > 100)) ||
		(sB->getTag() == TAG_BOMB && (sA->getTag() > 100))) {
		auto enemy = sA->getTag() > 100 ? (Enemy *)sA : (Enemy *)sB;
		auto bomb = sA->getTag() == TAG_BOMB ? (BombOfSoldier *)sA : (BombOfSoldier *)sB;
		bomb->explosion();
		bomb->isDie = true;
		
		enemy->isDie = true;
	}


	else if ((sA->getTag() == TAG_BOMB && (sB->getTag() == TAG_FLOOR)) ||
		(sB->getTag() == TAG_BOMB && (sA->getTag() == TAG_FLOOR))) {
		auto bomb = sA->getTag() == TAG_BOMB ? (BombOfSoldier *)sA : (BombOfSoldier *)sB;
		bomb->explosion();
		bomb->isDie = true;
	}


	else if ((sA->getTag() == TAG_BOMB_ENEMY && (sB->getTag() == TAG_FLOOR)) ||
		(sB->getTag() == TAG_BOMB_ENEMY && (sA->getTag() == TAG_FLOOR))) {
		auto bomb = sA->getTag() == TAG_BOMB_ENEMY ? (BombOfEnemy *)sA : (BombOfEnemy *)sB;
		bomb->explosion();
		bomb->isDie = true;

	}

	else if ((sA->getTag() == TAG_BOMB_ENEMY && (sB->getTag() == TAG_SOLDIER)) ||
		(sB->getTag() == TAG_BOMB_ENEMY && (sA->getTag() == TAG_SOLDIER))) {
		auto soldier = sA->getTag() == TAG_SOLDIER ? (Soldier*)sA : (Soldier*)sB;
		auto bomb = sA->getTag() == TAG_BOMB_ENEMY ? (BombOfEnemy *)sA : (BombOfEnemy *)sB;
		if (soldier->isNoDie >= 0) {
			soldier->cur_state = State::DIE;
		}

		bomb->explosion();
		bomb->isDie = true;
	}

}


// fix here to collision with enemy
//void CollisionListener::EndContact(b2Contact * contact)
//{
//	b2Body *bodyA = contact->GetFixtureA()->GetBody();
//	b2Body *bodyB = contact->GetFixtureB()->GetBody();
//
//	B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
//	B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
//
//	if ((sA->getTag() == TAG_SOLDIER && sB->getTag() == TAG_FLOOR) ||
//		(sB->getTag() == TAG_SOLDIER && sA->getTag() == TAG_FLOOR)
//		) {
//		bodyA->GetFixtureList()->SetSensor(false);
//		bodyB->GetFixtureList()->SetSensor(false);
//	}
//}

void CollisionListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
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
			}
		}
		else {
			auto dentaX = fabs(collidePoint.x - bodyA->GetPosition().x);
			auto radius = (soldier->getBoundingBox().size.width / PTM_RATIO) / 2;
			if (bodyA->GetPosition().y < collidePoint.y || dentaX > radius / 2) {
				contact->SetEnabled(false);
			}
		}

	}
}

