#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "DynamicHumanEnemy.h"

USING_NS_CC;

Hud *hud;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	hud = Hud::create();

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
	posGenEnemy = Point(INT_MAX, INT_MAX);
	// world
	world = new b2World(b2Vec2(0, -visibleSize.height * 8.0f / 3.0f / PTM_RATIO));

	// draw debug
	debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
	world->SetDebugDraw(debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	// flags += b2Draw::e_aabbBit;
	// flags += b2Draw::e_pairBit;
	// flags += b2Draw::e_centerOfMassBit;
	debugDraw->SetFlags(flags);

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);


	createBackground();
	createPool();
	

	indexOfCurrentMap = 1;
	originOfLastMap = Point(0, 0);
	createMap(tmxNextMap, originOfLastMap, layNextMap);

	// test dynamic human enemy


	createSoldier(Point(SCREEN_SIZE.width * 0.1f, SCREEN_SIZE.height * 0.5f));

	auto listener = new CollisionListener();
	world->SetContactListener(listener);

	this->scheduleUpdate();

	this->schedule([&](float dt) {
		if (posGenEnemy != Point(INT_MAX,INT_MAX)) {
			genEnemy();
			if ((soldier->getPosition().x + SCREEN_SIZE.width / 2) > posGenEnemy.x) {
				posGenEnemy = Point(INT_MAX, INT_MAX);
			}
		}
		
	}, 1.0f, "timer");

	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::update(float dt)
{
	int positionIterations = 3;		// position
	int velocityIterations = 8;		// velocity

	world->Step(dt, velocityIterations, positionIterations);
	soldier->updateSoldier(dt);
	//dynamicEnenmy->updateEnemy(dt);
	for (int i = 0; i < dEnemyPool->count(); i++) {
		((DynamicHumanEnemy*)dEnemyPool->getObjectAtIndex(i))->updateEnemy(dt);;
	}



	controlSneakyJoystick();
	controlSneakyButton();

	for (auto e : listEnemy) {
		e->updateEnemy(dt);
	}


	loadNextMap();

	//if (bg->getPosition().x < soldier->getPosition().x)
	bg_1A->setPosition(soldier->body->GetPosition().x * PTM_RATIO, SCREEN_SIZE.height / 2);

	if (posGenEnemy == Point(INT_MAX,INT_MAX)) {
		checkGenEnemy();
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createPool()
{
	dEnemyPool = CCArray::createWithCapacity(8);
	dEnemyPool->retain();
	for (int i = 0; i < 8; i++) {
		auto enemy = DynamicHumanEnemy::create(scaleOfMap*0.1f);
		enemy->setPosition(INT_MAX, INT_MAX);
		enemy->initCirclePhysic(world, Point(INT_MAX, INT_MAX));
		enemy->body->SetType(b2_staticBody);
		this->addChild(enemy);
		//auto  enemy = DynamicHumanEnemy::create(scaleOfMap*0.1f);
		dEnemyPool->addObject(enemy);
	}
	indexDEnemy = 0;
}

void GameScene::genEnemy()
{
	log("gennemy");
	log("point genenemy: %f, %f", posGenEnemy.x, posGenEnemy.y);
	auto enemy = (DynamicHumanEnemy*)dEnemyPool->objectAtIndex(indexDEnemy);
	enemy->body-> SetTransform(b2Vec2(posGenEnemy.x/PTM_RATIO,posGenEnemy.y/PTM_RATIO),0);
	enemy->body->SetType(b2_dynamicBody);
	indexDEnemy++;
	if (indexDEnemy == dEnemyPool->count()) {
		indexDEnemy = 0;
	}
	
}

void GameScene::checkGenEnemy()
{
	if (tmxCurrentMap != nullptr) {
		auto groupOfGenemy1 = tmxCurrentMap->getObjectGroup("moveEnemy");

		for (auto e : groupOfGenemy1->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
			origin = layCurrentMap->getPosition() + origin;
			Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);

			if (origin.x - soldier->getPosition().x > 0 && origin.x - soldier->getPosition().x < SCREEN_SIZE.width / 2) {
				posGenEnemy = origin + sizeOfBound;
				return;
			}
		}
	}
	if (tmxNextMap != nullptr) {
		auto groupOfGenemy2 = tmxNextMap->getObjectGroup("moveEnemy");
		for (auto e : groupOfGenemy2->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
			origin = layNextMap->getPosition() + origin;
			Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);

			if (origin.x - soldier->getPosition().x > 0 && origin.x - soldier->getPosition().x < SCREEN_SIZE.width / 2) {
				posGenEnemy = origin + sizeOfBound;
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
	//soldier = Soldier::create("soldier/soldier.json", "soldier/soldier.atlas", SCREEN_SIZE.height / 8.0f / 182.0f);
	soldier = Soldier::create("soldier/soldier.json", "soldier/soldier.atlas", scaleOfMap*0.15f);
	soldier->setPosition(pos);
	soldier->initCirclePhysic(world, soldier->getPosition());
	soldier->body->SetFixedRotation(true);						// pretent body from ratating
	addChild(soldier, ZORDER_SOLDIER);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createBackground()
{
	bg_1A = Sprite::create("bg-4.png");
	bg_1A->setScaleX(SCREEN_SIZE.width / bg_1A->getContentSize().width);
	bg_1A->setScaleY(SCREEN_SIZE.height / bg_1A->getContentSize().height);
	bg_1A->setPosition(SCREEN_SIZE / 2);
	addChild(bg_1A);

	/*bg_1B = Sprite::create("bg-1.jpg");
	bg_1B->setScaleX(SCREEN_SIZE.width / bg_1B->getContentSize().width);
	bg_1B->setScaleY(SCREEN_SIZE.height / bg_1B->getContentSize().height);
	bg_1B->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	addChild(bg_1B);

	bg_2A = Sprite::create("bg-2.png");
	bg_2A->setScaleX(SCREEN_SIZE.width / bg_2A->getContentSize().width);
	bg_2A->setScaleY(SCREEN_SIZE.height / bg_2A->getContentSize().height);
	bg_2A->setPosition(SCREEN_SIZE / 2);
	addChild(bg_2A);

	bg_2B = Sprite::create("bg-2.png");
	bg_2B->setScaleX(SCREEN_SIZE.width / bg_2B->getContentSize().width);
	bg_2B->setScaleY(SCREEN_SIZE.height / bg_2B->getContentSize().height);
	bg_2B->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	addChild(bg_2B);


	bg_3A = Sprite::create("bg-3.png");
	bg_3A->setScaleX(SCREEN_SIZE.width / bg_3A->getContentSize().width);
	bg_3A->setScaleY(SCREEN_SIZE.height / bg_3A->getContentSize().height);
	bg_3A->setPosition(SCREEN_SIZE / 2);
	addChild(bg_3A);

	bg_3B = Sprite::create("bg-3.png");
	bg_3B->setScaleX(SCREEN_SIZE.width / bg_3B->getContentSize().width);
	bg_3B->setScaleY(SCREEN_SIZE.height / bg_3B->getContentSize().height);
	bg_3B->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	addChild(bg_3B);*/

	camera = Follow::create(bg_1A);
	runAction(camera);

	tmxNextMap = TMXTiledMap::create("map1.tmx");
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
void GameScene::moveBackground()
{
	if (bg_1A->getPosition().x + bg_1A->getBoundingBox().size.width / 2 <= 0) {
		bg_1A->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	}

	if (bg_1B->getPosition().x + bg_1B->getBoundingBox().size.width / 2 <= 0) {
		bg_1B->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	}

	if (bg_2A->getPosition().x + bg_2A->getBoundingBox().size.width / 2 <= 0) {
		bg_2A->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	}

	if (bg_2B->getPosition().x + bg_2B->getBoundingBox().size.width / 2 <= 0) {
		bg_2B->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	}

	if (bg_3A->getPosition().x + bg_3A->getBoundingBox().size.width / 2 <= 0) {
		bg_3A->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	}

	if (bg_3B->getPosition().x + bg_3B->getBoundingBox().size.width / 2 <= 0) {
		bg_3B->setPosition(SCREEN_SIZE.width * 1.5f, SCREEN_SIZE.height / 2);
	}


}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createMap(TMXTiledMap *map, Point origin, Layer *layer)
{
	buildFloor(map, layer, scaleOfMap);
	buildLadderUp(map, layer, scaleOfMap);
	buildLadderDown(map, layer, scaleOfMap);
	//buildMoveEnemy(currentMap, scaleOfMap);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::loadNextMap()
{
	if ((soldier->getPosition().x > (originOfLastMap.x + SCREEN_SIZE.width))) {
		Point originOfNextmap = Point(originOfLastMap.x + tmxNextMap->getContentSize().width*scaleOfMap, 0);

		freePassedMap(originOfLastMap);

		tmxCurrentMap = tmxNextMap;
		layCurrentMap = layNextMap;
		layNextMap = Layer::create();
		layNextMap->setPosition(originOfNextmap);
		this->addChild(layNextMap);
		string nameOfNextMap = "map" + StringUtils::toString(indexOfCurrentMap + 1) + ".tmx";
		tmxNextMap = TMXTiledMap::create(nameOfNextMap);
		layNextMap->addChild(tmxNextMap);
		layNextMap->setContentSize(tmxNextMap->getContentSize()*scaleOfMap);
		createMap(tmxNextMap, originOfNextmap, layNextMap);
		originOfLastMap = originOfNextmap;
		indexOfCurrentMap++;
		log("next map: %d", indexOfCurrentMap);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::freePassedMap(Point originOfLastMap)
{
	if (layCurrentMap != nullptr) {
		vector <b2Body*> toDestroy;

		for (auto body = world->GetBodyList(); body; body = body->GetNext()) {
			log("POS BODY: %f", body->GetPosition().x*PTM_RATIO);
			log("POS ORIGIN: %f", originOfLastMap.x);
			if (body->GetPosition().x*PTM_RATIO < originOfLastMap.x) {
				toDestroy.push_back(body);
			}
		}
		for (auto a : toDestroy) {
			world->DestroyBody(a);
		}

		this->removeChild(layCurrentMap, true);
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
		Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y + sizeOfBound.height / 2);


		auto floor = Floor::create();
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
		floor->setScaleX(Vec2(sizeOfBound).length() / floor->getContentSize().width);
		floor->setScaleY(SCREEN_SIZE.height / 30 / floor->getContentSize().height);

		Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y + (sizeOfBound.height - floor->getBoundingBox().size.height) / 2);

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
		floor->setScaleX(Vec2(sizeOfBound).length() / floor->getContentSize().width);
		floor->setScaleY(SCREEN_SIZE.height / 30 / floor->getContentSize().height);

		Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y + (sizeOfBound.height - floor->getBoundingBox().size.height) / 2);

		floor->initPhysic(world, pos + originThisMap, b2_staticBody);
		floor->createLadderDown(sizeOfBound, pos);

		floor->setPosition(pos);
		layer->addChild(floor);

	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//void GameScene::buildMoveEnemy(TMXTiledMap * map, float scale)
//{
//	auto groupMoveEnemy = map->getObjectGroup("moveEnemy");
//	for (auto e : groupMoveEnemy->getObjects()) {
//		auto mObject = e.asValueMap();
//		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
//		Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);
//
//		Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y + sizeOfBound.height / 2);
//
//		Enemy *e = Enemy::create("enemy-soldier/soldier.json", "enemy-soldier/soldier.atlas", SCREEN_SIZE.height / 8.0f / 242.0f);
//		e->setPosition(pos);
//		e->initCirclePhysic(world, e->getPosition());
//		e->type = Type::MOVE;
//		e->body->SetFixedRotation(true);						// pretent body from ratating
//		e->addAnimation(0, "running", true);
//		addChild(e);
//
//		listEnemy.push_back(e);
//	}
//}

/************************************************************************/
/* Control joystick                                                     */
/************************************************************************/
void GameScene::controlSneakyJoystick()
{
	float degree = hud->joystick->getDegrees();
	if (hud->joystick->getVelocity() == Vec2::ZERO) {
		if (soldier->onGround) {
			if (soldier->body->GetLinearVelocity().x != 0.0f)
				soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			soldier->facingRight = true;
			soldier->cur_state = IDLE_SHOOT;
		}
	}
	else if (degree < 30.0f || degree >= 330.0f) {
		soldier->move(bg_1A->getPosition());
		soldier->facingRight = true;

		soldier->cur_state = RUNNING_SHOOT;
	}
	else if (degree >= 30.0f && degree < 70.0f) {
		soldier->move(bg_1A->getPosition());
		soldier->facingRight = true;
		soldier->cur_state = RUNNING_SHOOT_UP;
	}
	else if (degree >= 70.0f && degree < 110.0f) {
		if (soldier->body->GetLinearVelocity().x != 0.0f)
			soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = IDLE_SHOOT_UP;
	}
	else if (degree >= 110.0f && degree < 150.0f) {
		soldier->move(bg_1A->getPosition());
		soldier->facingRight = false;
		soldier->cur_state = RUNNING_SHOOT_UP;
	}
	else if (degree >= 150.0f && degree < 210.0f) {
		soldier->move(bg_1A->getPosition());
		soldier->facingRight = false;
		soldier->cur_state = RUNNING_SHOOT;
	}
	else if (degree >= 210.0f && degree < 250.0f) {
		soldier->move(bg_1A->getPosition());
		soldier->facingRight = false;
		soldier->cur_state = RUNNING_SHOOT_DOWN;
	}
	else if (degree >= 250.0f && degree < 290.0f) {
		if (soldier->body->GetLinearVelocity().x != 0.0f)
			soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = LYING_SHOOT;
	}
	else if (degree >= 290.0f && degree < 330.0f) {
		soldier->move(bg_1A->getPosition());
		soldier->facingRight = true;
		soldier->cur_state = RUNNING_SHOOT_DOWN;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::controlSneakyButton()
{
	if (hud->btnJump->getIsActive() && soldier->onGround) {
		soldier->cur_state = JUMPING;
		soldier->onGround = false;

		soldier->body->SetLinearVelocity(b2Vec2(0.0f, soldier->jump_vel));
	}
}

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

