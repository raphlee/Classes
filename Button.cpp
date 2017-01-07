#include "Button.h"

Button::Button() {

}

Button * Button::create(string file_name, Point pos)
{
	Button *mNode = new Button();
	mNode->initWithFile(file_name);
	mNode->setPosition(pos);
	mNode->addEvents();
	return mNode;
}


// add listener to sprite
void Button::addEvents()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);		// preventing other listener from using it

	listener->onTouchBegan = [&](Touch *mTouch, Event *mEvent)
	{
		auto p = mTouch->getLocation();
		convertToNodeSpace(p);
		Rect rect = this->getBoundingBox();
		
		
		if (rect.containsPoint(p))
		{	
			++tapCount;
			touch = *mTouch;

			if (tapCount == 1) {
				this->schedule(schedule_selector(Button::singleTap), timeInterval, 1, 0);	// repeat 1, delay 0
				
			}
			else {
				this->unschedule(schedule_selector(Button::singleTap));
				tapCount = 0;
				isDoubleTap = true;
			}

			isPress = true;
					
			return true;		// continue
		}

		return false;
	};

	listener->onTouchMoved = [&](Touch *mTouch, Event *mEvent)
	{
		auto p = mTouch->getLocation();
		convertToNodeSpace(p);
		Rect rect = this->getBoundingBox();

		if (! rect.containsPoint(p) && !isDoubleTap) {
			isPress = false;
		}
	};

	listener->onTouchEnded = [&](Touch *mTouch, Event *mEvent) {
		isPress = false;

	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Button::singleTap(float dt)
{
	this->unschedule(schedule_selector(Button::singleTap));
	tapCount = 0;
}
