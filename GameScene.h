#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__


#include "Hud.h"
#include "GLES-Render.h"
#include "Soldier.h"
#include "Enemy.h"
#include "Floor.h"
#include "CollisionListener.h"
#include "DynamicHumanEnemy.h"

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

	Sprite *bg_1A;
	Sprite *bg_1B;
	Sprite *bg_2A;
	Sprite *bg_2B;
	Sprite *bg_3A;
	Sprite *bg_3B;

	Follow *camera;
	TMXTiledMap *tmxCurrentMap;
	TMXTiledMap *tmxNextMap;
	Point originOfLastMap;
	Layer *layCurrentMap;
	Layer *layNextMap;
	float scaleOfMap;
	int indexOfCurrentMap;

	Soldier *soldier;
	vector<Enemy*> listEnemy;

	//DynamicHumanEnemy* dynamicEnenmy;

	CCArray *dEnemyPool;
	//queue<DynamicHumanEnemy*> dEnemyPool;
	int indexDEnemy;
	Point posGenEnemy;

	


public:
	// main loop in game
	void update(float dt);

	// process background / map for game
	void createBackground();
	void moveBackground();
	void createMap(TMXTiledMap *map, Point origin, Layer *layer);
	void loadNextMap();
	void freePassedMap(Point originOfLastMap);
	void buildFloor(TMXTiledMap *map, Layer* layer, float scale);
	void buildLadderUp(TMXTiledMap *map, Layer* layer, float scale);
	void buildLadderDown(TMXTiledMap *map, Layer* layer, float scale);
	//void buildMoveEnemy(TMXTiledMap *map, float scale);

	// process hero for game
	void createSoldier(Point pos);

	// process enemy for game
	void createPool();
	void genEnemy(); // call in schedule
	void checkGenEnemy();// call in update
	
	// process joystick
	void controlSneakyJoystick();
	void controlSneakyButton();

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
