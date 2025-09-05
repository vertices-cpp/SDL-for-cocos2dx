#include "HelloWorld.h"
#include<iostream>
#include <string>
using namespace std;
ParticleExample *particle;

Scene* HelloWorld::createScene() {
	return HelloWorld::create();
}
// 
// TextMessage* HelloWorld::textAddStroke(const char* string_text, const char* fontName, float fontSize, const Color3B &colorA, const Color3B &colorB, float lineWidth)
// {
// 	/* ÕýÎÄLabel */
// 	TextMessage* center = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	center->setColor(colorA);
// 
// 	/* Ãè±ßLabel ÉÏ */
// 	TextMessage* up = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	up->setColor(colorB);
// 	up->setPosition(Vec2(center->getPosition().x, center->getPosition().y - lineWidth));
// 	center->addChild(up, -1);
// 
// 	/* Ãè±ßLabel ÏÂ */
// 	TextMessage* down = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	down->setColor(colorB);
// 	down->setPosition(Vec2(center->getPosition().x, center->getPosition().y + lineWidth));
// 	center->addChild(down, -1);
// 
// 	/* Ãè±ßLabel ×ó */
// 	TextMessage* left = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	left->setPosition(Vec2(center->getPosition().x - lineWidth, center->getPosition().y));
// 	left->setColor(colorB);
// 	center->addChild(left, -1);
// 
// 	/* Ãè±ßLabel ÓÒ */
// 	TextMessage* right = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	right->setColor(colorB);
// 	right->setPosition(Vec2(center->getPosition().x + lineWidth, center->getPosition().y));
// 	center->addChild(right, -1);
// 
// 	return center;
// }

RepeatForever *rr;

JsonTiledMap *jtm;
Sprite * sp = NULL;


bool HelloWorld::init()
{
	Scene::init();

	SDLView::getInstance()->setScale(Vec2(1, 1));
	Size visibleSize_ = Director::getInstance()->getVisibleSize();

	//jtm = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
	jtm = JsonTiledMap::create("Resources/map1.tmj");
// 	auto jtm1 = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
// 	auto jtm2 = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
// 	auto jtm3 = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
// 	auto jtm4 = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
// 	auto jtm5 = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
// 	auto jtm6 = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
// 	auto jtm7 = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
// 	auto jtm8 = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
// 	addChild(jtm1);
// 	addChild(jtm2);
// 	addChild(jtm3);
// 	addChild(jtm5);
// 	addChild(jtm4);
// 	addChild(jtm6);
// 	addChild(jtm7);
// 	addChild(jtm8);
 //	jtm = JsonTiledMap::create("Resources/map1.tmj");
	auto pos = jtm->getPointer();
	pos = visibleSize_ / 2;
	pos.x = 0;
	jtm->setPosition(pos);
	
	addChild(jtm);

	//auto lay = jtm->getLayer("Layer1");
	//lay->setLayerScale(Vec2(1, 1));
	//sp = lay->getTileAt(Vec2(0, 11));
	auto lay = jtm->getLayer("B2");
	
	sp = lay->getTileAt(Vec2(4, 13));

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = [&](const std::vector<Touch*>& touches, Event  *event)
	{
		auto touch = touches[0];

		auto diff = touch->getDelta();
		diff.x = (float)diff.x, diff.y = (float)diff.y;
		 	auto node = event->getCurrentTarget();// getChildByTag(1);
		 	auto currentPos = node->getPosition();
		 	node->setPosition(currentPos + diff);
       
		
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, jtm);
	//jtm->setFlipX(true);

	AudioEngine::play2d("Resources/02.wav");

// 	particle = OG::ParticleExample::create();
// 	particle->setStyle(ParticleExample::METEOR);
// 	addChild(particle);

	auto mouseEvent = EventListenerMouse::create();
	mouseEvent->onMouseScroll = [&](EventMouse *evnet)
	{
		auto curLayer = ((JsonTiledMap*)evnet->getCurrentTarget());// ->getLayer("Layer1");
		auto scale = curLayer->getMapScale();
		if (evnet->getCursorY() > 0)
		{
			scale.x+=0.01;
			scale.y += 0.01;
			curLayer->setMapScale(scale);
		//	AudioEngine::stop(u8"Resources/·ï»Ë´«Ææ - È«ÊÇ°®.wav");
			AudioEngine::closeAll();
		}
		if (evnet->getCursorY() < 0)
		{
			scale.x-=0.01;
			scale.y-=0.01;
			curLayer->setMapScale(scale);
		//	AudioEngine::play2d(u8"Resources/·ï»Ë´«Ææ - È«ÊÇ°®.wav");

		}
		
	};

	mouseEvent->onMouseMove = [&](EventMouse *evnet)
	{
		auto curLayer = ((JsonTiledMap*)evnet->getCurrentTarget());// ->getLayer("Layer1");
		
// 		Vec2 mousePos = evnet->getLocation();
// 		particle->setPosition(mousePos);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEvent, jtm);


	

	auto J1 = JumpBy::create(1.9, Vec2(80, 0), 230, 1);
	
	auto J2 = JumpBy::create(1.9, Vec2(-80, 0), 230, 1);
	 

	auto se = Sequence::create(J1,J2, nullptr);
	 rr = RepeatForever::create(se);
	jtm->runAction(rr);
 
	auto func = [&] (EventCustom* event) {
		if(rr!=NULL)
		jtm->stopAction(rr);//ÔÝÍ£ÊÂ¼þ
	};
 	auto J1event = EventListenerCustom::create("1",func);
 	_eventDispatcher->addEventListenerWithFixedPriority(J1event, 1);
	//_eventDispatcher->removeEventListener(J1event);

	
	
	auto listener1 = EventListenerKeyboard::create();
	listener1->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{

		EventCustom event1 = EventCustom("1");
		this->_eventDispatcher->dispatchEvent(&event1);

		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_A:
		{
			auto currentPos = sp->getPosition();
			currentPos.x -= 10;
			sp->setPosition(currentPos);
		}
		break;
		case EventKeyboard::KeyCode::KEY_D:
		{	auto currentPos = sp->getPosition();
		currentPos.x += 10;
		sp->setPosition(currentPos); }
		break;

		case EventKeyboard::KeyCode::KEY_W:
		{
			auto currentPos = sp->getPosition();
			currentPos.y -= 10;
			sp->setPosition(currentPos);
		}
		break;
		case EventKeyboard::KeyCode::KEY_S:
		{	auto currentPos = sp->getPosition();
		currentPos.y += 10;
		sp->setPosition(currentPos);
		}
		break;
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, jtm);

	this->setUpdate();

	return true;
}


static int curLayer = 0;
void HelloWorld::update(float delta)
{
 
}
