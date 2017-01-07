#ifndef __BUTTON_LEFT_H__
#define __BUTTON_LEFT_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;

class Button : public Sprite
{
public:
	Button();
	static Button* create(string file_name, Point pos);

	void addEvents();

	bool isPress = false;
	bool isDoubleTap = false;



	void singleTap(float dt);
private:
	float timeInterval = 0.33f;
	Touch touch;
	int tapCount = 0;
};

#endif // __BUTTON_LEFT_H__
