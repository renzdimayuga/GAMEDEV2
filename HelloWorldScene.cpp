#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	auto layer = HelloWorld::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	scene->addChild(layer);

	return scene;
}

int score = 0;
bool gameOver0 = false;
bool gameOver1 = false;

bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0,1,0), 3);
	edgeBody->setDynamic(false);

	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);

	this->addChild(edgeNode);

	bg = Sprite::create("background.png");
	ball = Sprite::create("ball.png");
	paddle = Sprite::create("paddle.png");
	label = Label::createWithSystemFont(std::to_string(score), "Arial", 40);
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	ball->setAnchorPoint(Vec2(0, 1));
	paddle->setAnchorPoint(Vec2(0, 1));
	label->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(320, 240);
	ball->setPosition(160, 240);
	paddle->setPosition(208, 48);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 5; j++) {
			block[i][j] = Sprite::create("brick.png");
			block[i][j]->setAnchorPoint(Vec2(0, 1));
			block[i][j]->setPosition(192 + 32 * i, 400 - 16 * j);
		}
	}

	auto spriteBody = PhysicsBody::createCircle(ball->getContentSize().width/2, PhysicsMaterial(0, 1, 0));
	spriteBody->setCollisionBitmask(1);
	spriteBody->setContactTestBitmask(true);
	Vect force = Vect(80, -80);
	spriteBody->applyImpulse(force);
	ball->setPhysicsBody(spriteBody);

	auto spriteBody2 = PhysicsBody::createBox(paddle->getContentSize(), PhysicsMaterial(0, 1, 0));
	spriteBody2->setDynamic(false);
	paddle->setPhysicsBody(spriteBody2);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 5; j++) {
			auto blockBody = PhysicsBody::createBox(block[i][j]->getContentSize(), PhysicsMaterial(0, 1, 0));
			blockBody->setCollisionBitmask(2);
			blockBody->setContactTestBitmask(true);
			blockBody->setDynamic(false);
			block[i][j]->setPhysicsBody(blockBody);
		}
	}

	this->addChild(bg, 0);
	this->addChild(ball, 0);
	this->addChild(paddle, 0);
	this->addChild(label, 1);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 5; j++) {
			this->addChild(block[i][j], 0);
		}
	}

	this->scheduleUpdate();

	auto eventListener = EventListenerKeyboard::create();
	auto contactListener = EventListenerPhysicsContact::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)->void {
		Vector<FiniteTimeAction*> actions;
		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			paddle->setPosition(paddle->getPosition().x - 16, paddle->getPosition().y);
			if (paddle->getPosition().x < 0) {
				paddle->setPosition(560, paddle->getPosition().y);
			}
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			paddle->setPosition(paddle->getPosition().x + 16, paddle->getPosition().y);
			if (paddle->getPosition().x > 560) {
				paddle->setPosition(0, paddle->getPosition().y);
			}
			break;
		}
	};

	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, paddle);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

bool HelloWorld::onContactBegin(cocos2d::PhysicsContact &contact) {
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();
	if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask())) {
		auto spriteA = (Sprite*)a->getNode();
		auto spriteB = (Sprite*)b->getNode();
		if (2 == b->getCollisionBitmask()) {
			this->removeChild(spriteB, true);
			score++;
		} else if (2 == a->getCollisionBitmask()) {
			this->removeChild(spriteA, true);
			score++;
		}
	}
	return true;
}

void HelloWorld::update(float delta) {
	if (gameOver0) {
		Sleep(1000);
		exit(1);
	} else if (gameOver1) {
		Sleep(1000);
		exit(1);
	} else {
		this->removeChild(label, true);
		label = Label::createWithSystemFont(std::to_string(score), "Arial", 40);
		label->setAnchorPoint(Vec2(0, 0));
		this->addChild(label, 1);
		auto pos = ball->getPosition();
		if (pos.y < 32) {
			gameOver0 = true;
			this->removeAllChildren();
			label = Label::createWithSystemFont("Game over. Your score is " + std::to_string(score) + ".", "Arial", 40);
			label->setAnchorPoint(Vec2(0, 0));
			this->addChild(label, 1);
		}
		if (score == 40) {
			gameOver1 = true;
			this->removeAllChildren();
			label = Label::createWithSystemFont("You win!", "Arial", 40);
			label->setAnchorPoint(Vec2(0, 0));
			this->addChild(label, 1);
		}
	}
}