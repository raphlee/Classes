#include "CollisionListener.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Item.h"

#include "Soldier.h"
#include "Item.h"
#include "BombOfSoldier.h"

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

	// neu nguoi va cham item
	else if ((sA->getTag() == TAG_SOLDIER && (sB->getTag() == TAG_ITEM)) ||
		(sB->getTag() == TAG_SOLDIER && (sA->getTag() == TAG_ITEM))) {
		auto item = sA->getTag() == TAG_ITEM ? (Item *)sA : (Item *)sB;
		item->isTaken = true;
		log("TAKEN");
	}

	// neu enemy va cham dan cua hero
	else if ((sA->getTag() == TAG_BULLET_HERO && (sB->getTag() > 100)) ||
		(sB->getTag() == TAG_BULLET_HERO && (sA->getTag() > 100))) {
		auto enemy = sA->getTag() == TAG_ENEMY_SOLDIER ? (Enemy *)sA : (Enemy *)sB;
		auto bullet = sA->getTag() == TAG_BULLET_HERO ? (Bullet *)sA : (Bullet *)sB;
		log("%i", enemy->health);
		bullet->isDie = true;

		enemy->health -= 1;
		if(enemy->health <= 0)
			enemy->isDie = true;
		
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



