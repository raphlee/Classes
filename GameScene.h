#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__


#include "Hud.h"
#include "GLES-Render.h"
#include "Soldier.h"
#include "TankSoldier.h"
#include "HelicopterSoldier.h"
#include "PlaneSoldier.h"

#include "Enemy.h"
#include "Floor.h"
#include "Item.h"
#include "CollisionListener.h"
#include "DynamicHumanEnemy.h"
#include "StaticHumanEnemy.h"
#include "AutoGun.h"
#include "TankEnemy.h"
#include "InfiniteParallaxNode.h"


class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	GLESDebugDraw *debugDraw;
	Mat4 _modelViewMV;
	CustomCommand _customCommand;
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	b2World *world;

	Follow *camera;
	Node* follow;
	InfiniteParallaxNode *background;

	TMXTiledMap *tmxCurrentMap;
	TMXTiledMap *tmxNextMap;
	Point originOfLastMap;
	Layer *layCurrentMap;
	Layer *layNextMap;
	float scaleOfMap;
	int indexOfCurrentMap;

	Soldier *soldier;
	CCArray *dEnemyPool;
	int indexDEnemy;
	Point posGenDEnemy;

	set<int> listIndexExist;
	vector<BulletOfHero *> existedBullet;
	vector<Item*> items;

	//bool isTouchScreen = false;

public:
	// main loop in game
	void update(float dt);
	void updateSoldier(float dt);
	void updateStandMan(float dt);
	/*void updateAutoGun(float dt);
	void updateMiniFort(float dt);
	void updateFort(float dt);*/

	void removeOlderSoldier();
	void transformTank(Point pos);
	void transformHelicopter(Point pos);
	void transformPlane(Point pos);
	void switchItem(float dt);

	// process background / map for game
	void createInfiniteNode();
	void createBackground();
	void createMap(TMXTiledMap *map, Point origin, Layer *layer);
	void loadNextMap();
	void freePassedMap();

	void buildFloor(TMXTiledMap *map, Layer* layer, float scale);
	void buildLadderUp(TMXTiledMap *map, Layer* layer, float scale);
	void buildLadderDown(TMXTiledMap *map, Layer* layer, float scale);
	void buildStandEnemy(TMXTiledMap *map, Layer* layer, float scale);
	void buildAutoGun(TMXTiledMap *map, Layer* layer, float scale);
	void buildMiniFort(TMXTiledMap *map, Layer* layer, float scale);
	void buildFort(TMXTiledMap *map, Layer* layer, float scale);
	void buildTankEnemy(TMXTiledMap *map, Layer* layer, float scale);
	void buildHelicopterShoot(TMXTiledMap *map, Layer* layer, float scale);
	void buildHelicopterBoom(TMXTiledMap *map, Layer* layer, float scale);
	void buildItem(TMXTiledMap *map, Layer* layer, float scale, string nameTile, string frameName, TYPE type);


	// process hero for game
	void createSoldier(Point pos);

	// process enemy for game
	void createPool();
	//void genEnemy();			// call in schedule
	//void checkGenEnemy();		// call in update
	
	void genDEnemy();
	void checkGenDEnemy();


	// process joystick
	void controlSneakyJoystick();
	void controlSneakyButtonJump();
	void controlSneakyButtonFire();

	//void controlButtonMove();

	//void identifyAngle(Point location);

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw();

	//bool onTouchBegan(Touch *touch, Event *unused_event);
	//void onTouchMoved(Touch *touch, Event *unused_event);
	//void onTouchEnded(Touch *touch, Event *unused_event);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
