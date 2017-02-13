#include "AudioManager.h"
#include "Global.h"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}

void AudioManager::playSound(string keysound)
{
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEYSOUND,true);
	if (checkSound) {
		experimental::AudioEngine::play2d(keysound);
	}
}
