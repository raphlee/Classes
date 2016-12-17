#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__


#include "Hud.h"
#include "GLES-Render.h"
#include "Soldier.h"
#include "Enemy.h"
#include "Floor.h"
#include "CollisionListener.h"

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
	Node* follow;

	TMXTiledMap *tmxCurrentMap;
	TMXTiledMap *tmxNextMap;
	Point originOfLastMap;
	Layer *layCurrentMap;
	Layer *layNextMap;
	float scaleOfMap;
	int indexOfCurrentMap;

	Soldier *soldier;
	vector<Enemy*> listEnemy;

	CCArray *pool;


private:


	void update(float dt);
	void updateSoldier(float dt);

	void createPool();
	void createSoldier(Point pos);
	void createBackground();
	void moveBackground();

	void createMap(TMXTiledMap *map, Point origin, Layer *layer);

	void loadNextMap();
	void freePassedMap(Point originOfLastMap);

	void buildFloor(TMXTiledMap *map, Layer* layer, float scale);
	void buildLadderUp(TMXTiledMap *map, Layer* layer, float scale);
	void buildLadderDown(TMXTiledMap *map, Layer* layer, float scale);
	void buildMoveEnemy(TMXTiledMap *map, float scale);
 



	void controlSneakyJoystick();
	void controlSneakyButton();

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
