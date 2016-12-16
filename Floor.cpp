#include "Floor.h"




Floor * Floor::create()
{
	Floor *floor = new Floor();
	floor->initWithFile("box/lid.png");
	floor->setTag(FLOOR);
	return floor;
}

void Floor::createLadderUp(Size sizeOfBox, Point pos)
{
	rotate = (PI / 2 - atan2f(sizeOfBox.height, sizeOfBox.width));
	body->SetTransform(body->GetPosition(), PI / 2 - rotate);

	this->setRotation(90 + (rotate / PI) * 180);
}

void Floor::createLadderDown(Size sizeOfBox, Point pos)
{
	rotate = (PI - atan2f(sizeOfBox.height, sizeOfBox.width));
	body->SetTransform(body->GetPosition(), rotate);

	this->setRotation(180 - (rotate / PI) * 180);
}


