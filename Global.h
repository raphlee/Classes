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
#define Y_INCREMENT_RATIO				100
#define KEYJOYSTICK_X					"KEYJSX"
#define KEYJOYSTICK_Y					"KEYJSY"
#define KEYBTNFIRE_X					"KEYFIREX"
#define KEYBTNFIRE_Y					"KEYFIREY"
#define KEYBTNJUMP_X					"KEYJUMPX"
#define KEYBTNJUMP_Y					"KEYJUMPY"
#define KEY_CHOICE						"GAMEPLAY"
#define KEYONGAME						"KEYONGAME"
#define KEYADS							"KEYADS"


// bitmask
#define BITMASK_SOLDIER					0x0001
#define BITMASK_ENEMY					0x0002
#define BITMASK_FLOOR					0x0004
#define BITMASK_BULLET_HERO				0x0008
#define BITMASK_BULLET_ENEMY			0x0010
#define BITMASK_ITEM					0x0020
#define BITMASK_BOMB_HERO				0x0040
#define BITMASK_BLINK					0x0080


// tag
#define TAG_SOLDIER							50
#define TAG_FLOOR							51
#define TAG_BULLET_HERO						52
#define TAG_BULLET_ENEMY					53
#define TAG_ITEM							54
#define TAG_BOMB							55
#define TAG_BLINK							56

#define TAG_ENEMY_SOLDIER					101
#define TAG_ENEMY_AUTOGUN					102
#define TAG_ENEMY_FORTMINI					103
#define TAG_ENEMY_FORT						104
#define TAG_ENEMY_TANK						105
#define TAG_ENEMY_HELICOPTER_SHOOT			106
#define TAG_ENEMY_HELICOPTER_BOOM			107




// z-order
#define ZORDER_BG_1						0
#define ZORDER_BG_2						1
#define ZORDER_BG_3						2
#define ZORDER_BULLET					3
#define ZORDER_ENEMY					6
#define ZORDER_SOLDIER					7
#define ZORDER_ITEM						11



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

#define MAX_BULLET_AUTOGUN_POOL				2
#define MAX_BULLET_FORT_MINI_POOL			3
#define MAX_BULLET_FORT_POOL				6
#define MAX_BULLET_TANK_POOL				3
#define MAX_BULLET_HELICOPTER_POOL			2

#define Z_BACKGROUND						0
#define Z_HERO								1

#endif // __GLOBAL_H__
