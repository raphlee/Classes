#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include "cocos2d.h"
#include <string>
#include "AudioEngine.h"

USING_NS_CC;
using namespace std;
//using namespace experimental;

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	static void playSound(string keysound);
};
#endif // __TANK_SOLDIER_H__