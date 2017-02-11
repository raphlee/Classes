#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "DynamicHumanEnemy.h"
#include "MiniFort.h"
#include "Fort.h"
#include "HelicopterShootEnemy.h"
#include "HelicopterBoomEnemy.h"


USING_NS_CC;

Hud *hud;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	hud = Hud::create();
	hud->setAnchorPoint(Vec2(0.0f, 0.0f));
	hud->setPosition(0.0f, 0.0f);

	auto layer = GameScene::create();


	// add layer as a child to scene
	scene->addChild(layer);
	scene->addChild(hud);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// world
	world = new b2World(b2Vec2(0, -visibleSize.height * 8.0f / 3.0f / PTM_RATIO));

	// draw debug
	/*debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
	world->SetDebugDraw(debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugDraw->SetFlags(flags);*/

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);


	posGenDEnemy = Point(INT_MAX, INT_MAX);

	follow = Node::create();
	follow->setPosition(origin + visibleSize / 2);
	addChild(follow);

	camera = Follow::create(follow);
	runAction(camera);

	auto indexMap = UserDefault::getInstance()->sharedUserDefault()->getIntegerForKey(KEY_CHOICE);

	indexOfCurrentMap = indexMap;
	//indexOfCurrentMap = 2;
	createBackground();
	createPool();
	originOfLastMap = Point(0, 0);
	createMap(tmxNextMap, originOfLastMap, layNextMap);

	createSoldier(Point(SCREEN_SIZE.width * 0.5f, SCREEN_SIZE.height * 0.75f));

	auto world_listener = new CollisionListener();
	world->SetContactListener(world_listener);

	this->schedule([&](float dt) {
		if (posGenDEnemy != Point(INT_MAX, INT_MAX)) {
			if (controlGenMoveEnemy < 3) {
				genDEnemy();
			}
			controlGenMoveEnemy++;
			if (controlGenMoveEnemy >= 10) {
				controlGenMoveEnemy = 0;
			}
			if ((soldier->getPositionX() + SCREEN_SIZE.width / 2) > posGenDEnemy.x) {
				posGenDEnemy = Point(INT_MAX, INT_MAX);
				controlGenMoveEnemy = 0;
			}
		}
	}, 0.5f, "timer");

	/*choiceControl = UserDefault::sharedUserDefault()->getIntegerForKey(KEY_SELECTION);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/

	this->scheduleUpdate();
	return true;
}

void GameScene::update(float dt)
{
	int positionIterations = 3;		// position
	int velocityIterations = 8;		// velocity

	world->Step(dt, velocityIterations, positionIterations);

	updateSoldier(dt);
	updateStandMan(dt);
	//updateAutoGun(dt);

	for (int i = 0; i < dEnemyPool->count(); i++) {
		((DynamicHumanEnemy*)dEnemyPool->getObjectAtIndex(i))->updateEnemy(dt, follow->getPosition());
	}


	switchItem(dt);

	for (int i = 0; i < existedBullet.size(); i++) {
		auto bullet = (BulletOfHero*)existedBullet[i];
		bullet->update(dt);
		if (abs(bullet->getPositionX() - follow->getPositionX()) > SCREEN_SIZE.width / 2) {
			listIndexExist.insert(i);
		}
	}

	if (listIndexExist.size() != 0 && listIndexExist.size() == existedBullet.size()) {
		for (auto i : existedBullet) {
			if(i->body != nullptr)
				world->DestroyBody(i->body);
			i->setVisible(false);
			removeChildByTag(i->getTag());
		}

		existedBullet.clear();
		listIndexExist.clear();
	}

	for (int i = 0; i < soldier->bulletPool->count(); i++) {
		auto bullet = ((BulletOfHero*)soldier->bulletPool->getObjectAtIndex(i));
		bullet->update(dt);
		if (abs(bullet->getPositionX() - follow->getPositionX()) > SCREEN_SIZE.width / 2) {
			bullet->isDie = true;
		}

		if (bullet->isDie) {
			if (bullet->body != nullptr) {
				//log("Destroy Bullet");
				world->DestroyBody(bullet->body);
				bullet->body = nullptr;
			}
			
			bullet->setVisible(false);
			bullet->isDie = false;
		}
	}

	if (soldier->bombPool != nullptr) {
		for (int i = 0; i < soldier->bombPool->count(); i++) {
			auto bomb = ((BombOfSoldier*)soldier->bombPool->getObjectAtIndex(i));
			bomb->update(dt);
			if (bomb->getPositionY() < 0) {
				bomb->isDie = true;
			}
			if (bomb->isDie) {
				if (bomb->body != nullptr) {
					//log("Destroy Bullet");
					world->DestroyBody(bomb->body);
				}
				bomb->body = nullptr;
				bomb->setVisible(false);
				bomb->isDie = false;
			}
		}
	}


	//if (choiceControl == 0) {
	controlSneakyJoystick();
	controlSneakyButtonJump();
	controlSneakyButtonFire();
	//}
	//else
		//controlButtonMove();

	loadNextMap();
	freePassedMap();
	if (follow->getPositionX() <= soldier->getPositionX())
		follow->setPositionX(soldier->getPositionX());


	if (posGenDEnemy == Point(INT_MAX, INT_MAX)) {
		checkGenDEnemy();
	}

	background->updatePosition();

}

void GameScene::updateSoldier(float dt)
{
	if (soldier != nullptr)
		soldier->updateHero(dt);
	else
		return;

	if ((follow->getPositionX() - soldier->getPositionX()) > SCREEN_SIZE.width / 2 - soldier->sizeSoldier.width / 2) {
		soldier->isGetOriginX = true;
	}
	else {
		soldier->isGetOriginX = false;
	}

	if (soldier->getPosition().y < 0) {
		soldier->isNoDie = 0;
		soldier->cur_state = DIE;
	}

	if (soldier->cur_state == State::DIE) {
		soldier->health--;
		soldier->die(follow->getPosition());
	}

	switch (soldier->health)
	{
	case 5: {
		if (!hud->life_5->isVisible()) {
			hud->life_5->setVisible(true);
			hud->life_4->setVisible(true);
			hud->life_3->setVisible(true);
			hud->life_2->setVisible(true);
			hud->life_1->setVisible(true);
		}
			
		break;
	}
	case 4: {
		if (hud->life_5->isVisible())
			hud->life_5->setVisible(false);
		if (!hud->life_4->isVisible())
			hud->life_4->setVisible(true);
		break;
	}
	case 3: {
		if (hud->life_4->isVisible())
			hud->life_4->setVisible(false);
		if (!hud->life_3->isVisible())
			hud->life_3->setVisible(true);
		break;
	}
	case 2: {
		if (hud->life_3->isVisible())
			hud->life_3->setVisible(false);
		if (!hud->life_2->isVisible())
			hud->life_2->setVisible(true);
		break;
	}
	case 1: {
		if (hud->life_2->isVisible())
			hud->life_2->setVisible(false);
		if (!hud->life_1->isVisible())
			hud->life_1->setVisible(true);
		break;
	}
	case 0: {
		if (hud->life_1->isVisible())
			hud->life_1->setVisible(false);
		break;
	}
	default:
		//log("End game");
		break;
	}
}

void GameScene::updateStandMan(float dt)
{
	if (layCurrentMap != nullptr) {
		auto listObj = layCurrentMap->getChildren();
		for (auto e : listObj) {
			if (e->getTag() > 100) {
				auto tmp = (StaticHumanEnemy*)e;
				tmp->updateBullet(follow->getPosition());
				if (!tmp->checkOutScreen(follow->getPosition())) {
					tmp->updateEnemy(dt, follow->getPosition(), soldier->getPosition());
				}
				else {
					if (tmp->getPosition().x + tmp->getParent()->getPosition().x < follow->getPosition().x) {
						if (tmp->body != nullptr)
							tmp->die();
					}

				}
			}
		}
	}

	if (layNextMap != nullptr) {
		auto listObj = layNextMap->getChildren();
		for (auto e : listObj) {
			if (e->getTag() > 100) {
				auto tmp = (StaticHumanEnemy*)e;
				tmp->updateBullet(follow->getPosition());
				if (!tmp->checkOutScreen(follow->getPosition())) {
					tmp->updateEnemy(dt, follow->getPosition(), soldier->getPosition());
				}
				else {
					if (tmp->getPosition().x + tmp->getParent()->getPosition().x < follow->getPosition().x) {
						if (tmp->body != nullptr)
							tmp->die();
					}
				}
			}
		}
	}
}



void GameScene::removeOlderSoldier()
{
	for (int i = 0; i < soldier->bulletPool->count(); i++) {
		auto bullet = (BulletOfHero*)soldier->bulletPool->getObjectAtIndex(i);
		if (abs(bullet->getPositionX() - follow->getPositionX()) < SCREEN_SIZE.width / 2) {
			existedBullet.push_back((BulletOfHero*)soldier->bulletPool->getObjectAtIndex(i));
		}
	}

	soldier->bulletPool->removeAllObjects();
	world->DestroyBody(soldier->body);

	removeChildByTag(TAG_SOLDIER);
	soldier = nullptr;
}

void GameScene::transformTank(Point pos)
{
	removeOlderSoldier();

	if (soldier == nullptr) {
		soldier = TankSoldier::create("tank/tank.json", "tank/tank.atlas", SCREEN_SIZE.height / 8.8f / 113.0f);
		soldier->setPosition(pos);
		addChild(soldier, ZORDER_SOLDIER);
		soldier->initPhysic(world, soldier->getPosition());
		soldier->body->SetFixedRotation(true);
		soldier->createPool();
		soldier->blinkTrans();
	}

}

void GameScene::transformHelicopter(Point pos)
{
	removeOlderSoldier();

	if (soldier == nullptr) {
		soldier = HelicopterSoldier::create("enemy-helicopter/helicopter.json", "enemy-helicopter/helicopter.atlas", SCREEN_SIZE.height / 12.7f / 80.0f);
		soldier->setPosition(pos);
		addChild(soldier, ZORDER_SOLDIER);
		soldier->initPhysic(world, soldier->getPosition());
		soldier->body->SetFixedRotation(true);
		soldier->createPool();
		soldier->setScaleX(-1);
		soldier->idleShoot();
		soldier->blinkTrans();
	}
}

void GameScene::transformPlane(Point pos)
{
	removeOlderSoldier();

	if (soldier == nullptr) {
		soldier = PlaneSoldier::create("plane/plane.json", "plane/plane.atlas", SCREEN_SIZE.height / 11.0f / 80.0f);
		soldier->setPosition(pos);
		addChild(soldier, ZORDER_SOLDIER);
		soldier->initPhysic(world, soldier->getPosition());
		soldier->body->SetFixedRotation(true);
		soldier->createPool();
		soldier->createBombPool();
		//soldier->idle();
		soldier->blinkTrans();
	}
}

void GameScene::switchItem(float dt)
{
	for (auto i : items) {
		
		if (i->isTaken) {
			switch (i->type)
			{
			case TYPE::TANK: {
				if (hud->btnJump->getParent()->isVisible()) {
					hud->btnJump->getParent()->setVisible(false);
				}
				transformTank(i->getPosition() + i->getParent()->getPosition());
				break;
			}
			case TYPE::HEALTH: {
				if(soldier->health < 5)
					soldier->health++;
				break;
			}
			case TYPE::HELICOPTER: {
				if (hud->btnJump->getParent()->isVisible()) {
					hud->btnJump->getParent()->setVisible(false);
				}
				transformHelicopter(i->getPosition() + i->getParent()->getPosition());
				
				break;
			}
			case TYPE::FAST_BULLET: {
				soldier->bulletType = BulletType::Fast;
				break;
			}
			case TYPE::MULT_BULLET: {
				soldier->bulletType = BulletType::Super;
				break;
			}
			case TYPE::ORBIT_BULLET: {
				soldier->bulletType = BulletType::Circle;
				break;
			}
			case TYPE::PLANE: {
				if (hud->btnJump->getParent()->isVisible()) {
					hud->btnJump->getParent()->setVisible(false);
				}
				transformPlane(i->getPosition() + i->getParent()->getPosition());
				break;
			}
			default:
				break;
			}

			i->isTaken = false;
			world->DestroyBody(i->body);
			i->removeFromParentAndCleanup(true);
		}
		else
			i->update(dt);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createPool()
{
	// pool of dynamic enemy
	dEnemyPool = CCArray::createWithCapacity(8);
	dEnemyPool->retain();
	for (int i = 0; i < 8; i++) {
		auto enemy = DynamicHumanEnemy::create(SCREEN_SIZE.height / 11.0f / 242.0f);
		enemy->setPosition(INT_MAX, INT_MAX);
		//enemy->initCirclePhysic(world, Point(INT_MAX, INT_MAX));
		enemy->body = nullptr;
		this->addChild(enemy, ZORDER_ENEMY);
		dEnemyPool->addObject(enemy);
	}

	indexDEnemy = 0;

	//heroBulletPool = CCArray::createWithCapacity(MAX_BULLET_HERO_POOL);
	//heroBulletPool->retain();
	//for (int i = 0; i < MAX_BULLET_HERO_POOL; i++) {
	//	auto bullet = BulletOfHero::create();
	//	bullet->setPosition(INT_MAX, INT_MAX);
	//	bullet->initPhysic(world, Point(INT_MAX, INT_MAX));
	//	//bullet->body->SetType(b2_staticBody);
	//	this->addChild(bullet);
	//	heroBulletPool->addObject(bullet);
	//}

	//indexHeroBulletPool = 0;
}



void GameScene::genDEnemy()
{
	auto enemy = (DynamicHumanEnemy*)dEnemyPool->objectAtIndex(indexDEnemy);
	enemy->health = 1;
	enemy->setPosition(posGenDEnemy);
	//enemy->body->SetTransform(b2Vec2(posGenDEnemy.x / PTM_RATIO, posGenDEnemy.y / PTM_RATIO), 0);
	//enemy->body->SetType(b2_dynamicBody);
	if (enemy->body != nullptr) {
		world->DestroyBody(enemy->body);
	}
	enemy->initCirclePhysic(world, posGenDEnemy);
	enemy->body->SetFixedRotation(false);
	auto fixture = enemy->body->GetFixtureList();
	fixture->SetFriction(0);
	indexDEnemy++;
	if (indexDEnemy == dEnemyPool->count()) {
		indexDEnemy = 0;
	}
}

//void GameScene::genSEnemy()
//{
//	auto enemy = (StaticHumanEnemy*)sEnemyPool->objectAtIndex(indexSEnemy);
//	enemy->body->SetTransform(b2Vec2(posGenSEnemy.x / PTM_RATIO, posGenSEnemy.y / PTM_RATIO), 0);
//	enemy->body->SetType(b2_dynamicBody);
//
//	if (indexSEnemy + 1 == sEnemyPool->count()) {
//		indexSEnemy = 0;
//	}
//	else {
//		indexSEnemy++;
//	}
//	posGenSEnemy = Point(INT_MAX, INT_MAX);
//}

void GameScene::checkGenDEnemy()
{
	if (tmxCurrentMap != nullptr) {
		auto groupOfGenemy1 = tmxCurrentMap->getObjectGroup("moveEnemy");

		for (auto e : groupOfGenemy1->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
			Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
			origin = layCurrentMap->getPosition() + origin + sizeOfBound;


			if (origin.x - follow->getPosition().x > SCREEN_SIZE.width / 2 && origin.x - follow->getPosition().x < SCREEN_SIZE.width) {
				posGenDEnemy = origin;
				return;
			}
		}
	}
	if (tmxNextMap != nullptr) {
		auto groupOfGenemy2 = tmxNextMap->getObjectGroup("moveEnemy");
		for (auto e : groupOfGenemy2->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
			Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
			origin = layNextMap->getPosition() + origin + sizeOfBound;


			if (origin.x - follow->getPosition().x > SCREEN_SIZE.width / 2 && origin.x - follow->getPosition().x < SCREEN_SIZE.width) {
				posGenDEnemy = origin;
				return;
			}
		}
	}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createSoldier(Point pos)
{
	soldier = Soldier::create("soldier/soldier.json", "soldier/soldier.atlas", SCREEN_SIZE.height / 11.0f / 182.0f);
	soldier->listener();
	GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("soldier/soldierShape.plist", soldier->sizeSoldier.height / 251.0f);
	soldier->setPosition(pos);
	addChild(soldier, ZORDER_SOLDIER);
	// ko duoc thay doi thu tu cau lenh nay
	soldier->initPhysic(world, soldier->getPosition());
	soldier->body->SetFixedRotation(true);
	soldier->createPool();
}

void GameScene::createInfiniteNode()
{
	background = InfiniteParallaxNode::create();

	auto bg1_1 = Sprite::create("bg-1.jpg");
	//auto bg1_1 = Sprite::create("bg-4.png");
	bg1_1->setScaleX(SCREEN_SIZE.width / bg1_1->getContentSize().width);
	bg1_1->setScaleY(SCREEN_SIZE.height / bg1_1->getContentSize().height);
	bg1_1->setAnchorPoint(Point(0, 0.5f));

	auto bg1_2 = Sprite::create("bg-1.jpg");
	//auto bg1_2 = Sprite::create("bg-4.png");
	bg1_2->setScaleX(SCREEN_SIZE.width / bg1_2->getContentSize().width);
	bg1_2->setScaleY(SCREEN_SIZE.height / bg1_2->getContentSize().height);
	bg1_2->setAnchorPoint(Point(0, 0.5f));

	auto bg2_1 = Sprite::create("bg-2.png");
	bg2_1->setScaleX(SCREEN_SIZE.width / bg2_1->getContentSize().width);
	bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
	bg2_1->setAnchorPoint(Point(0, 0.5f));

	auto bg2_2 = Sprite::create("bg-2.png");
	bg2_2->setScaleX(SCREEN_SIZE.width / bg2_2->getContentSize().width);
	bg2_2->setScaleY(SCREEN_SIZE.height / bg2_2->getContentSize().height);
	bg2_2->setAnchorPoint(Point(0, 0.5f));

	auto bg3_1 = Sprite::create("bg-3.png");
	bg3_1->setScaleX(SCREEN_SIZE.width / bg3_1->getContentSize().width);
	bg3_1->setScaleY(SCREEN_SIZE.height / bg3_1->getContentSize().height);
	bg3_1->setAnchorPoint(Point(0, 0.5f));

	auto bg3_2 = Sprite::create("bg-3.png");
	bg3_2->setScaleX(SCREEN_SIZE.width / bg3_2->getContentSize().width);
	bg3_2->setScaleY(SCREEN_SIZE.height / bg3_2->getContentSize().height);
	bg3_2->setAnchorPoint(Point(0, 0.5f));


	background->addChild(bg1_1, 0, Vec2(0.1f, 1), Vec2(0, 0));
	background->addChild(bg1_2, 0, Vec2(0.1f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));
	background->addChild(bg2_1, 0, Vec2(0.5f, 1), Vec2(0, 0));
	background->addChild(bg2_2, 0, Vec2(0.5f, 1), Vec2(bg2_1->getBoundingBox().size.width, 0));
	background->addChild(bg3_1, 0, Vec2(1, 1), Vec2(0, 0));
	background->addChild(bg3_2, 0, Vec2(1, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	background->setPosition(Point(0, SCREEN_SIZE.height / 2));
	background->setAnchorPoint(Point(0, 0.5f));
	this->addChild(background, ZORDER_BG_1);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createBackground()
{
	createInfiniteNode();

	tmxNextMap = TMXTiledMap::create("map" + StringUtils::toString(indexOfCurrentMap) + ".tmx");
	tmxNextMap->setAnchorPoint(Vec2(0, 0));

	scaleOfMap = SCREEN_SIZE.height / tmxNextMap->getContentSize().height;
	tmxNextMap->setScale(scaleOfMap);
	layNextMap = Layer::create();
	layNextMap->setAnchorPoint(Point(0, 0));
	layNextMap->setContentSize(tmxNextMap->getContentSize()*scaleOfMap);
	this->setPosition(originOfLastMap);
	this->addChild(layNextMap);
	layNextMap->addChild(tmxNextMap);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createMap(TMXTiledMap *map, Point origin, Layer *layer)
{
	buildFloor(map, layer, scaleOfMap);
	buildLadderUp(map, layer, scaleOfMap);
	buildLadderDown(map, layer, scaleOfMap);

	buildStandEnemy(map, layer, scaleOfMap);
	buildAutoGun(map, layer, scaleOfMap);

	buildMiniFort(map, layer, scaleOfMap);
	buildFort(map, layer, scaleOfMap);
	buildTankEnemy(map, layer, scaleOfMap);
	buildHelicopterShoot(map, layer, scaleOfMap);
	buildHelicopterBoom(map, layer, scaleOfMap);


	buildItem(map, layer, scaleOfMap, "item_tank", "item-up-tank.png", TYPE::TANK);
	buildItem(map, layer, scaleOfMap, "item_helicopter", "item-up-helicopter.png", TYPE::HELICOPTER);
	buildItem(map, layer, scaleOfMap, "item_plane", "item-up-plane.png", TYPE::PLANE);
	buildItem(map, layer, scaleOfMap, "rewardM", "item-speed-bullet.png", TYPE::FAST_BULLET);
	buildItem(map, layer, scaleOfMap, "rewardF", "item-swirl-bullet.png", TYPE::ORBIT_BULLET);
	buildItem(map, layer, scaleOfMap, "rewardS", "item-spread-bullet.png", TYPE::MULT_BULLET);
	buildItem(map, layer, scaleOfMap, "item_health", "item-life.png", TYPE::HEALTH);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::loadNextMap()
{
	if ((soldier->getPosition().x > (originOfLastMap.x + tmxNextMap->getBoundingBox().size.width - SCREEN_SIZE.width) && indexOfCurrentMap < 17)) {

		Point originOfNextmap = Point(originOfLastMap.x + tmxNextMap->getContentSize().width*scaleOfMap, 0);

		//freePassedMap(originOfLastMap);

		tmxCurrentMap = tmxNextMap;
		layCurrentMap = layNextMap;
		layNextMap = Layer::create();
		layNextMap->setPosition(originOfNextmap);
		this->addChild(layNextMap);
		string nameOfNextMap = "map" + StringUtils::toString(indexOfCurrentMap + 1) + ".tmx";
		tmxNextMap = TMXTiledMap::create(nameOfNextMap);
		tmxNextMap->setVisible(false);
		layNextMap->addChild(tmxNextMap, -100);
		layNextMap->setContentSize(tmxNextMap->getContentSize()*scaleOfMap);

		createMap(tmxNextMap, originOfNextmap, layNextMap);

		indexOfCurrentMap++;
		originOfLastMap = originOfNextmap;
		log("next map: %d", indexOfCurrentMap);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::freePassedMap()
{
	if ((soldier->getPosition().x > (originOfLastMap.x + SCREEN_SIZE.width) && indexOfCurrentMap < 17)) {
		if (layCurrentMap != nullptr) {
			vector <b2Body*> toDestroy;

			for (auto body = world->GetBodyList(); body; body = body->GetNext()) {
				log("POS BODY: %f", body->GetPosition().x*PTM_RATIO);
				log("POS ORIGIN: %f", layNextMap->getPositionX());
				if (body->GetPosition().x*PTM_RATIO < layNextMap->getPositionX()) {
					toDestroy.push_back(body);
				}
			}
			for (auto a : toDestroy) {
				log("Destroy layout");
				world->DestroyBody(a);
			}
			layCurrentMap->removeAllChildrenWithCleanup(true);
			this->removeChild(layCurrentMap, true);
			//layCurrentMap->removeFromParentAndCleanup(true);
			layCurrentMap = nullptr;
			tmxCurrentMap = nullptr;
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::buildFloor(TMXTiledMap *map, Layer* layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto groupFloor = map->getObjectGroup("floor");
	for (auto e : groupFloor->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		Point pos = Point(origin.x + sizeOfBound.width / 2,
			origin.y + sizeOfBound.height / 2 + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		auto floor = Floor::create();
		floor->setAnchorPoint(Vec2(0.5, 1));
		floor->setScaleX(sizeOfBound.width / floor->getContentSize().width);
		floor->setScaleY(SCREEN_SIZE.height / 30 / floor->getContentSize().height);
		floor->setPosition(pos);

		floor->initPhysic(world, pos + originThisMap, b2_staticBody);
		layer->addChild(floor);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::buildLadderUp(TMXTiledMap *map, Layer* layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto groupLadderUp = map->getObjectGroup("ladder");
	for (auto e : groupLadderUp->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto floor = Floor::create();
		floor->setAnchorPoint(Vec2(0.5, 0));
		floor->setScaleX(Vec2(sizeOfBound).length() / floor->getContentSize().width);
		floor->setScaleY(SCREEN_SIZE.height / 30 / floor->getContentSize().height);

		Point pos = Point(origin.x + sizeOfBound.width / 2,
			origin.y + (sizeOfBound.height - floor->getBoundingBox().size.height) / 2 + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		floor->initPhysic(world, pos + originThisMap, b2_staticBody);
		floor->createLadderUp(sizeOfBound, pos);

		floor->setPosition(pos);
		layer->addChild(floor);

	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::buildLadderDown(TMXTiledMap *map, Layer* layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto groupLadderDown = map->getObjectGroup("ladder2");
	for (auto e : groupLadderDown->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto floor = Floor::create();
		floor->setAnchorPoint(Vec2(0.5, 1));
		floor->setScaleX(Vec2(sizeOfBound).length() / floor->getContentSize().width);
		floor->setScaleY(SCREEN_SIZE.height / 30 / floor->getContentSize().height);

		Point pos = Point(origin.x + sizeOfBound.width / 2,
			origin.y + (sizeOfBound.height - floor->getBoundingBox().size.height) / 2 + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		floor->initPhysic(world, pos + originThisMap, b2_staticBody);
		floor->createLadderDown(sizeOfBound, pos);

		floor->setPosition(pos);
		layer->addChild(floor);

	}
}

void GameScene::buildStandEnemy(TMXTiledMap * map, Layer * layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup("stand_man");
	for (auto e : group->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto standMan = StaticHumanEnemy::create(SCREEN_SIZE.height / 11.0f / 242.0f);

		Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);
		standMan->initCirclePhysic(world, pos + originThisMap);
		//and set it back
		//standMan->setTag(TAG_STANDMAN);
		standMan->setPosition(pos);

		layer->addChild(standMan, 3);
		standMan->createPool(MAX_BULLET_SOLDIER_ENEMY_POOL);
	}
}

void GameScene::buildAutoGun(TMXTiledMap * map, Layer * layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup("hide_man");
	for (auto e : group->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto gun = AutoGun::create(SCREEN_SIZE.height / 11.0f / 131.0f);

		Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		gun->initCirclePhysic(world, pos + originThisMap);
		gun->changeBodyBitMask(BITMASK_SOLDIER);
		//and set it back
		gun->setPosition(pos);
		layer->addChild(gun, 3);
		gun->createPool(MAX_BULLET_AUTOGUN_POOL);

	}
}

void GameScene::buildMiniFort(TMXTiledMap * map, Layer * layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup("minifort");
	for (auto e : group->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto gun = MiniFort::create(SCREEN_SIZE.height / 8.0f / 111.0f);

		Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		gun->initCirclePhysic(world, pos + originThisMap);
		//gun->changeBodyBitMask(BITMASK_SOLDIER);
		//and set it back
		gun->setPosition(pos);
		layer->addChild(gun, 3);
		gun->createPool(MAX_BULLET_FORT_MINI_POOL);

	}
}

void GameScene::buildFort(TMXTiledMap * map, Layer * layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup("bigfort");
	for (auto e : group->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto gun = Fort::create(SCREEN_SIZE.height / 8.0f / 111.0f);

		Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		gun->initCirclePhysic(world, pos + originThisMap);
		//gun->changeBodyBitMask(BITMASK_SOLDIER);
		//and set it back
		gun->setPosition(pos);
		layer->addChild(gun, 3);
		gun->createPool(MAX_BULLET_FORT_POOL);

	}
}

void GameScene::buildTankEnemy(TMXTiledMap * map, Layer * layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup("tank_move");
	if (group != nullptr)
		for (auto e : group->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
			//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

			auto gun = TankEnemy::create(SCREEN_SIZE.height / 9.0f / 113.0f, TankType::STUPID);

			Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

			gun->initCirclePhysic(world, pos + originThisMap);
			//gun->changeBodyBitMask(BITMASK_SOLDIER);
			//and set it back
			gun->setPosition(pos);
			layer->addChild(gun, 3);
			gun->createPool(MAX_BULLET_TANK_POOL);
		}

	auto group2 = map->getObjectGroup("tank");
	if (group2 != nullptr)
		for (auto e : group2->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
			//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

			auto gun = TankEnemy::create(SCREEN_SIZE.height / 9.0f / 113.0f, TankType::NORMAL);

			Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

			gun->initCirclePhysic(world, pos + originThisMap);
			//gun->changeBodyBitMask(BITMASK_SOLDIER);
			//and set it back
			gun->setPosition(pos);
			layer->addChild(gun, 3);
			gun->createPool(MAX_BULLET_TANK_POOL);
		}

	auto group3 = map->getObjectGroup("tank2");
	if (group3 != nullptr)
		for (auto e : group3->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
			//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

			auto gun = TankEnemy::create(SCREEN_SIZE.height / 9.0f / 113.0f, TankType::SMART);

			Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

			gun->initCirclePhysic(world, pos + originThisMap);
			//gun->changeBodyBitMask(BITMASK_SOLDIER);
			//and set it back
			gun->setPosition(pos);
			layer->addChild(gun, 3);
			gun->createPool(MAX_BULLET_TANK_POOL);
		}
}

void GameScene::buildHelicopterShoot(TMXTiledMap * map, Layer * layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup("plane");
	if (group != nullptr)
		for (auto e : group->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
			//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

			auto gun = HelicopterShootEnemy::create(SCREEN_SIZE.height / 13.0f / 80.0f, HelicopterType::SHOOT_HORIZONTAL);

			Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

			gun->initCirclePhysic(world, pos + originThisMap);
			gun->body->SetGravityScale(0);
			gun->body->GetFixtureList()->SetSensor(true);
			//gun->changeBodyBitMask(BITMASK_SOLDIER);
			//and set it back
			//gun->setTag(TAG_ENEMY_TANK);
			gun->setPosition(pos);
			layer->addChild(gun, 3);
			gun->createPool(MAX_BULLET_HELICOPTER_POOL);
		}

	auto group2 = map->getObjectGroup("plane2");
	if (group2 != nullptr)
		for (auto e : group2->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
			//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

			auto gun = HelicopterShootEnemy::create(SCREEN_SIZE.height / 13.0f / 80.0f, HelicopterType::SHOOT_VERTICAL);

			Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

			gun->initCirclePhysic(world, pos + originThisMap);
			gun->body->SetGravityScale(0);
			gun->body->GetFixtureList()->SetSensor(true);
			//gun->changeBodyBitMask(BITMASK_SOLDIER);
			//and set it back
			gun->setPosition(pos);
			layer->addChild(gun, 3);
			gun->createPool(MAX_BULLET_HELICOPTER_POOL);
		}

}

void GameScene::buildHelicopterBoom(TMXTiledMap * map, Layer * layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup("plane_bomb");
	if (group != nullptr)
		for (auto e : group->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
			//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

			auto gun = HelicopterBoomEnemy::create(SCREEN_SIZE.height / 13.0f / 80.0f, HelicopterBoomType::SIMPLE);

			Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

			gun->initCirclePhysic(world, pos + originThisMap);
			gun->body->SetGravityScale(0);
			gun->body->GetFixtureList()->SetSensor(true);
			//gun->changeBodyBitMask(BITMASK_SOLDIER);
			//and set it back
			//gun->setTag(TAG_ENEMY_TANK);
			gun->setPosition(pos);
			layer->addChild(gun, 3);
			gun->createPool(TAG_ENEMY_HELICOPTER_BOOM);
		}

	auto group2 = map->getObjectGroup("plane_bomb_back");
	if (group2 != nullptr)
		for (auto e : group2->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
			//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

			auto gun = HelicopterShootEnemy::create(SCREEN_SIZE.height / 13.0f / 80.0f, HelicopterType::SHOOT_SMART);

			Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

			gun->initCirclePhysic(world, pos + originThisMap);
			gun->body->SetGravityScale(0);
			gun->body->GetFixtureList()->SetSensor(true);
			//gun->changeBodyBitMask(BITMASK_SOLDIER);
			//and set it back
			gun->setPosition(pos);
			layer->addChild(gun, 3);
			gun->createPool(MAX_BULLET_HELICOPTER_POOL);
		}
}

void GameScene::buildItem(TMXTiledMap * map, Layer * layer, float scale, string nameTile, string frameName, TYPE type)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup(nameTile);

	if (group == nullptr)
		return;

	for (auto e : group->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);

		auto item = Item::create(frameName, type);
		item->setScale(SCREEN_SIZE.height / 8.0f / item->getContentSize().height);
		item->size = item->getBoundingBox().size;
		Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);
		item->setPosition(pos);
		item->initPhysic(world, pos + originThisMap, b2_dynamicBody);
		layer->addChild(item, ZORDER_ENEMY);

		items.push_back(item);
	}

}

/************************************************************************/
/* Control joystick                                                     */
/************************************************************************/
void GameScene::controlSneakyJoystick()
{
	float degree = hud->joystick->getDegrees();
	auto joystickVel = hud->joystick->getVelocity();
	if (soldier->isOnTheAir) {
		if(soldier->cur_state != IDLE)
			soldier->cur_state = IDLE;
		soldier->moveFollow(joystickVel);
		return;
	}

	if (hud->joystick->getVelocity() == Vec2::ZERO) {
		//
		if (soldier->onGround) {
			soldier->facingRight ? soldier->angle = 0 : soldier->angle = PI;
			if (soldier->body->GetLinearVelocity().x != 0.0f)
				soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			soldier->cur_state = IDLE;

		}
	}
	else if (degree < 30.0f || degree >= 330.0f) {
		soldier->move();
		soldier->angle = 0;  // goc 0
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT;
	}
	else if (degree >= 30.0f && degree < 70.0f) {
		soldier->move();
		soldier->angle = PI / 6;
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_UP;
	}
	else if (degree >= 70.0f && degree < 110.0f) {
		soldier->angle = PI / 2;
		if (soldier->body->GetLinearVelocity().x != 0.0f)
			soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		if (soldier->onGround)
			soldier->cur_state = IDLE_SHOOT_UP;
	}
	else if (degree >= 110.0f && degree < 150.0f) {

		soldier->move();
		soldier->angle = PI * 5 / 6;
		soldier->facingRight = false;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_UP;
	}
	else if (degree >= 150.0f && degree < 210.0f) {
		soldier->move();
		soldier->angle = PI;
		soldier->facingRight = false;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT;
	}
	else if (degree >= 210.0f && degree < 250.0f) {
		soldier->move();
		soldier->angle = PI * 5 / 4;
		soldier->facingRight = false;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_DOWN;
	}
	else if (degree >= 250.0f && degree < 290.0f) {
		if (soldier->body->GetLinearVelocity().x != 0.0f)
			soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		if (soldier->facingRight) {
			soldier->angle = 0;
		}
		else {
			soldier->angle = PI;
		}

		if(soldier->onGround)
			soldier->cur_state = LYING_SHOOT;
	}
	else if (degree >= 290.0f && degree < 330.0f) {
		soldier->move();
		soldier->angle = -PI / 4;
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_DOWN;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::controlSneakyButtonJump()
{
	if (hud->btnJump->getParent()->isVisible() && hud->btnJump->getIsActive() && soldier->onGround) {
		soldier->cur_state = JUMPING;
		soldier->onGround = false;

		soldier->body->SetLinearVelocity(b2Vec2(0.0f, soldier->jump_vel));
	}
}

void GameScene::controlSneakyButtonFire()
{
	if (hud->btnFire->getIsActive()) {
		if (soldier->cur_state == IDLE) {
			soldier->cur_state = IDLE_SHOOT;
		}
			
		soldier->shoot(soldier->angle);
	}
}


// 20/12
//void GameScene::controlButtonMove()
//{
//	bool conflict = hud->btnLeft->isPress & hud->btnRight->isPress;
//	if (conflict) {
//		if (soldier->body->GetLinearVelocity().x != 0.0f)
//			soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
//
//		soldier->cur_state = IDLE_SHOOT;
//		return;
//	}
//
//	if (hud->btnLeft->isDoubleTap && soldier->onGround) {
//		hud->btnLeft->isDoubleTap = false;
//		soldier->facingRight = false;
//		soldier->cur_state = JUMPING;
//		soldier->onGround = false;
//		soldier->body->SetLinearVelocity(b2Vec2(-soldier->move_vel, soldier->jump_vel));
//	}
//
//	else if (hud->btnRight->isDoubleTap && soldier->onGround) {
//		hud->btnRight->isDoubleTap = false;
//		soldier->facingRight = true;
//		soldier->cur_state = JUMPING;
//		soldier->onGround = false;
//		soldier->body->SetLinearVelocity(b2Vec2(soldier->move_vel, soldier->jump_vel));
//	}
//
//	if (hud->btnLeft->isPress && soldier->onGround) {
//		soldier->angle = PI;
//		soldier->facingRight = false;
//		soldier->move();
//		if (soldier->onGround && !isTouchScreen)
//			soldier->cur_state = RUNNING_SHOOT;
//	}
//	else if (hud->btnRight->isPress && soldier->onGround) {
//		soldier->angle = 0;
//		soldier->facingRight = true;
//		soldier->move();
//		if (soldier->onGround && !isTouchScreen)
//			soldier->cur_state = RUNNING_SHOOT;
//	}
//
//	else if (soldier->onGround && !isTouchScreen) {
//		if (soldier->body->GetLinearVelocity().x != 0.0f)
//			soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
//		soldier->cur_state = IDLE_SHOOT;
//		if (soldier->facingRight) {
//			soldier->angle = 0;
//		}
//		else {
//			soldier->angle = PI;
//		}
//
//	}
//
//}

// xac dinh angle de update ani
//void GameScene::identifyAngle(Point location)
//{
//	auto originX = follow->getPositionX() - SCREEN_SIZE.width / 2;
//
//	auto soldierPos = soldier->getPosition();
//	float vX = soldierPos.x - (location.x + originX);
//	float vY = soldierPos.y - location.y;
//
//	float edgeXY = sqrt(vX * vX + vY * vY);
//
//	float cosOrigin = vX / edgeXY;
//	float sinOrigin = vY / edgeXY;
//
//
//	if (sinOrigin >= 0.0f) {
//		if (cosOrigin >= 0.0f) {	// goc phan tu thu 3
//			soldier->facingRight = false;
//			soldier->setScaleX(-1);
//			if (sinOrigin < 0.383f) {
//				soldier->angle = PI;
//				if (soldier->body->GetLinearVelocity().x == 0.0f)
//					soldier->cur_state = IDLE_SHOOT;
//				else
//					soldier->cur_state = RUNNING_SHOOT;
//			}
//			else {
//				soldier->angle = -PI * 5 / 6;
//				soldier->cur_state = RUNNING_SHOOT_DOWN;
//			}
//		}
//		else {	// goc phan tu thu 4
//			soldier->facingRight = true;
//			soldier->setScaleX(1);
//			if (sinOrigin < 0.383f) {
//				soldier->angle = 0;
//				if (soldier->body->GetLinearVelocity().x == 0.0f)
//					soldier->cur_state = IDLE_SHOOT;
//				else
//					soldier->cur_state = RUNNING_SHOOT;
//			}
//			else {
//				soldier->angle = -PI / 6;
//				soldier->cur_state = RUNNING_SHOOT_DOWN;
//			}
//		}
//	}
//	else {
//		if (cosOrigin >= 0.0f) {   // goc phan tu thu 2
//			soldier->facingRight = false;
//			soldier->setScaleX(-1);
//			if (sinOrigin > -0.383f) {
//				soldier->angle = PI;
//				if (soldier->body->GetLinearVelocity().x == 0.0f)
//					soldier->cur_state = IDLE_SHOOT;
//				else
//					soldier->cur_state = RUNNING_SHOOT;
//			}
//			else {
//				soldier->angle = PI * 5 / 6;
//				soldier->cur_state = RUNNING_SHOOT_UP;
//			}
//		}
//		else {						// goc phan tu thu 1
//			soldier->facingRight = true;
//			soldier->setScaleX(1);
//			if (sinOrigin > -0.383f) {
//				soldier->angle = 0;
//				if (soldier->body->GetLinearVelocity().x == 0.0f)
//					soldier->cur_state = IDLE_SHOOT;
//				else
//					soldier->cur_state = RUNNING_SHOOT;
//			}
//			else {
//				soldier->angle = PI / 6;
//				soldier->cur_state = RUNNING_SHOOT_UP;
//			}
//		}
//	}
//}



void GameScene::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
	//
	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	//
	Layer::draw(renderer, transform, flags);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	_modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(GameScene::onDraw, this);
	renderer->addCommand(&_customCommand);

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GameScene::onDraw()
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");

	auto oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
	world->DrawDebugData();
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}

//bool GameScene::onTouchBegan(Touch * touch, Event * unused_event)
//{
//	auto location = touch->getLocation();
//	convertToNodeSpace(location);
//
//	isTouchScreen = true;
//
//	if (choiceControl == 1) {
//		identifyAngle(location);
//		return true;
//	}
//	else {
//		/*if (soldier->onGround && location.x > SCREEN_SIZE.width / 2) {
//			soldier->body->SetLinearVelocity(b2Vec2(0.0f, soldier->jump_vel));
//
//			soldier->onGround = false;
//
//			soldier->cur_state = JUMPING;
//		}*/
//
//		return false;
//	}
//}
//
//void GameScene::onTouchMoved(Touch * touch, Event * unused_event)
//{
//	auto location = touch->getLocation();
//	convertToNodeSpace(location);
//	identifyAngle(location);
//}
//
//void GameScene::onTouchEnded(Touch * touch, Event * unused_event)
//{
//	isTouchScreen = false;
//}
//
