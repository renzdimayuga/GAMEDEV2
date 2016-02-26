#pragma once

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);
	void update(float) override;
private:
	cocos2d::Label* label;
	cocos2d::Sprite* bg;
	cocos2d::Sprite* ball;
	cocos2d::Sprite* paddle;
	cocos2d::Sprite* block[8][5];
	cocos2d::PhysicsWorld *sceneWorld;
	void SetPhysicsWorld(cocos2d::PhysicsWorld *world) {
		sceneWorld = world;
	}
	bool onContactBegin(cocos2d::PhysicsContact &contact);
};