#include "Utility.h"


void Utility::showBoomBoom(Node *father, Point posToShow)
{
	auto boom = Sprite::createWithSpriteFrameName("explosion-1.png");
	boom->setPosition(posToShow);
	father->addChild(boom);

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(6);
	for (int i = 1; i < 7; i++) {
		string s = "explosion-" + StringUtils::toString(i) + ".png";
		animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(s));
	}

	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.5f);
	Animate* animate = Animate::create(animation);
	boom->runAction(animate);

	auto removeBoom = CallFunc::create([&]() {
		father->removeChild(boom, true);
	});
	father->runAction(Sequence::create(DelayTime::create(0.6f),removeBoom, nullptr));
	

}
