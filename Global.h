#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include <string>
#include <vector>

USING_NS_CC;
using namespace std;


// define
#define PI								3.1415926
#define Y_INCREMENT_RATIO				7						
#define KEY_SELECTION					"KEYME"
#define KEYBACKGROUND					"KEYBG"
#define KEYSOUND						"KEYSOUND"
#define KEYONGAME						"KEYONGAME"
#define KEYADS							"KEYADS"


// bitmask
#define SOLDIER_BITMASK					0x0001
#define ENEMY_BITMASK					0x0002
#define FLOOR_BITMASK					0x0004
#define BULLET_HERO_BITMASK				0x0008
#define BULLET_ENEMY_BITMASK			0x0010
#define ITEM_BITMASK					0x0020
#define BLANK_BITMASK					0x0040
#define ALLIGATOR_BITMASK				0x0080
#define HIPPO_BITMASK					0x0100

// tag
#define SOLDIER							101
#define FLOOR							102
#define ENEMY							103
#define BRIDGE							104
#define WAVE							105
#define LOG								106
#define OTHER_LEAF						107
#define OTHER_BROKEN_LEAF				108
#define ALLIGATOR						109


#define WATER_SHAPE						110
#define OTHER_WATER_SHAPE				111
#define SHADOW							112
#define ITEM							113
#define TREE							114
#define WOOD							115
#define VORTEX							116
#define HIPPO							117
#define BULLET							118
#define EFFECT_ON_LEAF					119

#define GAME_TAG						120



// z-order
#define ZORDER_BG_1						0
#define ZORDER_BG_2						1
#define ZORDER_BG_3						2
#define ZORDER_SOLDIER					3
#define ZORDER_SHADOW					4
#define ZORDER_VORTEX					5
#define ZORDER_ENEMY					6
#define ZORDER_LOG						7
#define ZORDER_EFFECT_ON_LEAF			8		// touch leaf
#define ZORDER_LEAF						9
#define ZORDER_WATER_EFFECT				10		// frog die
#define ZORDER_ITEM						11
#define ZORDER_BRIDGE					12
#define ZORDER_JUMP_EFFECT				13
#define ZORDER_FROG						14
#define ZORDER_DECORATION				15		// tree, wood
#define ZORDER_SCORE_PLUS				16
#define ZORDER_PERFECT					17
#define ZORDER_POONG_EFFECT				18
#define ZORDER_POONG_TEXT				19
#define ZORDER_LIGHT					20

// timing
#define TIME_BRIDGE_OCCUR				21
#define TIME_ALLIGATOR_OCCUR			41
#define TIME_HIPPO_OCCUR				50
#define TIME_VORTEX_OCCUR				90
#define TIME_TREE_OCCUR					6
#define TIME_WOOD_OCCUR					11
#define TIME_TO_UPGRADE					11


// define scale ratio
#define ALLIGATOR_RATIO_ORIGIN			2.65f
#define ALLIGATOR_RATIO					3.65f
#define HIPPO_RATIO						3.0f
#define FROG_RATIO						9.0f
#define VORTEX_RATIO					7.0f
#define LEAF_RATIO_ORIGIN				8.5f
#define LOG_RATIO						4.5f

// we need max times upgrade
// upgrade frog velocity, leaf, bridge, all.. @@

// ratio
#define PTM_RATIO							32
#define MAX_DYNAMIC_HUMAN_ENEMY				8
#define Z_BACKGROUND						0
#define Z_HERO								1

// velocity
const float VELOCITY_TO_UPGRADE_FROG	= 0.1f;

const b2Vec2 VELOCITY_TO_UPGRADE		= b2Vec2(0.0f, -0.13f);



// arr to identify freq item
const int ARR[] = {0, 0, 0, 1, 0, 0, 1, 0, 0, 1};

// arr to random type of leaf: green leaf and yellow green leaf
const int ARR_RANDOM_TYPE[] = {0, 1, 0, 0, 1};

// arr to random angle crocodile appear
const float ARR_ANGLE[] = {0, -90, 90, -60, 60};


#endif // __GLOBAL_H__
