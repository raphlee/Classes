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
#define KEY_SELECTION					"KEYSELECTION"
#define KEYJOYSTICK_X					"KEYJSX"
#define KEYJOYSTICK_Y					"KEYJSY"
#define KEYBTNFIRE_X					"KEYFIREX"
#define KEYBTNFIRE_Y					"KEYFIREY"
#define KEYBTNJUMP_X					"KEYJUMPX"
#define KEYBTNJUMP_Y					"KEYJUMPY"
#define KEYONGAME						"KEYONGAME"
#define KEYADS							"KEYADS"


// bitmask
#define BITMASK_SOLDIER					0x0001
#define BITMASK_ENEMY					0x0002
#define BITMASK_FLOOR					0x0004
#define BITMASK_BULLET_HERO				0x0008
#define BITMASK_BULLET_ENEMY			0x0010
#define BITMASK_ITEM					0x0020


// tag
#define TAG_SOLDIER							101
#define TAG_FLOOR							102
#define TAG_ENEMY							103
#define TAG_BULLET_HERO						104
#define TAG_BULLET_ENEMY					105
#define TAG_STANDMAN						106
#define TAG_AUTOGUN							107
#define TAG_ITEM							108
#define TAG_BOMB_HERO						109




// z-order
#define ZORDER_BG_1						0
#define ZORDER_BG_2						1
#define ZORDER_BG_3						2
#define ZORDER_BULLET					3
#define ZORDER_SHADOW					4
#define ZORDER_VORTEX					5
#define ZORDER_ENEMY					6
#define ZORDER_SOLDIER					7
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


#define PTM_RATIO							32
#define MAX_DYNAMIC_HUMAN_ENEMY				8
#define MAX_BULLET_HERO_POOL				30
#define MAX_BOMB_HERO_POOL					7
#define MAX_BULLET_SOLDIER_ENEMY_POOL		3



// arr to identify freq item
const int ARR[] = {0, 0, 0, 1, 0, 0, 1, 0, 0, 1};


#endif // __GLOBAL_H__
