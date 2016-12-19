#include "CollisionListener.h"

CollisionListener::CollisionListener() {

}

CollisionListener::~CollisionListener()
{

}

void CollisionListener::BeginContact(b2Contact * contact)
{

}


// fix here to collision with enemy
void CollisionListener::EndContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	bodyA->GetFixtureList()->SetSensor(false);
	bodyB->GetFixtureList()->SetSensor(false);

	B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
	B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();

	if ((sA->getTag() == SOLDIER && sB->getTag() == FLOOR) ||
		(sB->getTag() == SOLDIER && sA->getTag() == FLOOR)
		) {
		auto soldier = sA->getTag() == SOLDIER ? (Soldier *)sA : (Soldier *)sB;
		soldier->onGround = false;
	}

}

void CollisionListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	// oldManifold dùng để truy cập raw contact mainfold

	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	// dùng để tính toán các vị trí contact
	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];


	B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
	B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();

	if ((sA->getTag() == SOLDIER && sB->getTag() == FLOOR) ||
		(sB->getTag() == SOLDIER && sA->getTag() == FLOOR)
		) {

		
		auto soldier = sA->getTag() == SOLDIER ? (Soldier *)sA : (Soldier *)sB;

		if (sA->getTag() != SOLDIER) {
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
}


