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


	posGenEnemy = Point(INT_MAX, INT_MAX);

	follow = Node::create();
	follow->setPosition(origin + visibleSize / 2);
	addChild(follow);


	camera = Follow::create(follow);
	runAction(camera);

	indexOfCurrentMap = 1;
	createBackground();
	createPool();
	originOfLastMap = Point(0, 0);
	createMap(tmxNextMap, originOfLastMap, layNextMap);

	createSoldier(Point(SCREEN_SIZE.width * 0.15f, SCREEN_SIZE.height * 0.75f));


	auto listener = new CollisionListener();
	world->SetContactListener(listener);

	/*
	this->schedule([&](float dt) {
		if (posGenEnemy != Point(INT_MAX,INT_MAX)) {
			//genEnemy();
			if ((soldier->getPositionX() + SCREEN_SIZE.width / 2) > posGenEnemy.x) {
				posGenEnemy = Point(INT_MAX, INT_MAX);
			}
		}

	}, 1.0f, "timer");*/

	this->scheduleUpdate();

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

	//dynamicEnenmy->updateEnemy(dt);
	/*for (int i = 0; i < dEnemyPool->count(); i++) {
		((DynamicHumanEnemy*)dEnemyPool->getObjectAtIndex(i))->updateEnemy(dt);;
	}*/

	updateSoldier(dt);
	controlSneakyJoystick();
	controlSneakyButton();


	loadNextMap();
	if (follow->getPositionX() <= soldier->getPositionX())
		follow->setPositionX(soldier->getPositionX());


	/*if (posGenEnemy == Point(INT_MAX, INT_MAX)) {
		checkGenEnemy();
	}*/

	background->updatePosition();
}

void GameScene::updateSoldier(float dt)
{
	soldier->updateSoldier(dt);
	if (soldier->getPosition().y < 0) {
		soldier->facingRight = true;
		soldier->cur_state = JUMPING;
		soldier->body->SetLinearVelocity(b2Vec2(0, 0));
		soldier->body->SetTransform(b2Vec2((follow->getPositionX() - SCREEN_SIZE.width * 0.35f) / PTM_RATIO,
			SCREEN_SIZE.height / PTM_RATIO), soldier->body->GetAngle());
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
		auto enemy = DynamicHumanEnemy::create(scaleOfMap*0.123f);
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
	enemy->body->SetTransform(b2Vec2(posGenEnemy.x / PTM_RATIO, posGenEnemy.y / PTM_RATIO), 0);
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
	soldier = Soldier::create("soldier/soldier.json", "soldier/soldier.atlas", SCREEN_SIZE.height / 11.0f / 182.0f);
	soldier->listener();
	GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("soldier/soldierShape.plist", soldier->sizeSoldier.height / 251.0f);
	soldier->setPosition(pos);
	soldier->initPhysic(world, soldier->getPosition());
	soldier->body->SetFixedRotation(true);						// pretent body from ratating
	addChild(soldier, ZORDER_SOLDIER);
}

void GameScene::createInfiniteNode()
{
	background = InfiniteParallaxNode::create();
	auto bg1_1 = Sprite::create("bg-1.jpg");
	bg1_1->setScaleX(SCREEN_SIZE.width / bg1_1->getContentSize().width);
	bg1_1->setScaleY(SCREEN_SIZE.height / bg1_1->getContentSize().height);
	bg1_1->setAnchorPoint(Point(0,0.5));
	auto bg1_2 = Sprite::create("bg-1.jpg");
	bg1_2->setScaleX(SCREEN_SIZE.width / bg1_2->getContentSize().width);
	bg1_2->setScaleY(SCREEN_SIZE.height / bg1_2->getContentSize().height);
	bg1_2->setAnchorPoint(Point(0, 0.5));
	//bg1->setPosition(SCREEN_SIZE / 2);
	
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


	background->addChild(bg1_1, 0, Vec2(0.1, 1), Vec2(0,0));
	background->addChild(bg1_2, 0, Vec2(0.1, 1), Vec2(bg1_1->getBoundingBox().size.width,0));
	background->addChild(bg2_1, 0, Vec2(0.5, 1), Vec2(0,0));
	background->addChild(bg2_2, 0, Vec2(0.5, 1), Vec2(bg2_1->getBoundingBox().size.width, 0));
	background->addChild(bg3_1, 0, Vec2(1, 1), Vec2(0, 0));
	background->addChild(bg3_2, 0, Vec2(1, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	background->setPosition(Point(0,SCREEN_SIZE.height/2));
	background->setAnchorPoint(Point(0,0.5f));
	this->addChild(background, Z_BACKGROUND);
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
	//buildMoveEnemy(currentMap, scaleOfMap);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::loadNextMap()
{
	if ((soldier->getPosition().x > (originOfLastMap.x + SCREEN_SIZE.width) && indexOfCurrentMap < 18)) {
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
		soldier->move(follow->getPosition());
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT;
	}
	else if (degree >= 30.0f && degree < 70.0f) {
		soldier->move(follow->getPosition());
		soldier->facingRight = true;
		if (soldier->onGround)
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
		soldier->move(follow->getPosition());
		soldier->facingRight = false;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_UP;
	}
	else if (degree >= 150.0f && degree < 210.0f) {
		soldier->move(follow->getPosition());
		soldier->facingRight = false;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT;
	}
	else if (degree >= 210.0f && degree < 250.0f) {
		soldier->move(follow->getPosition());
		soldier->facingRight = false;
		if (soldier->onGround)
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
		soldier->move(follow->getPosition());
		soldier->facingRight = true;
		if (soldier->onGround)
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

