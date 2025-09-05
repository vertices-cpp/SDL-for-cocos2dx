#include "HelloWorld.h"
#include<iostream>
#include <string>
using namespace std;
ParticleExample *particle;
//Node *particle;
Scene* HelloWorld::createScene() {
	return HelloWorld::create();
}
// 
// TextMessage* HelloWorld::textAddStroke(const char* string_text, const char* fontName, float fontSize, const Color3B &colorA, const Color3B &colorB, float lineWidth)
// {
// 	/* ����Label */
// 	TextMessage* center = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	center->setColor(colorA);
// 
// 	/* ���Label �� */
// 	TextMessage* up = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	up->setColor(colorB);
// 	up->setPosition(Vec2(center->getPosition().x, center->getPosition().y - lineWidth));
// 	center->addChild(up, -1);
// 
// 	/* ���Label �� */
// 	TextMessage* down = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	down->setColor(colorB);
// 	down->setPosition(Vec2(center->getPosition().x, center->getPosition().y + lineWidth));
// 	center->addChild(down, -1);
// 
// 	/* ���Label �� */
// 	TextMessage* left = TextMessage::createWithTTF(string_text, fontName, fontSize);
// 	left->setPosition(Vec2(center->getPosition().x - lineWidth, center->getPosition().y));
// 	left->setColor(colorB);
// 	center->addChild(left, -1);
// 
// 	/* ���Label �� */
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

	jtm = JsonTiledMap::create("Resources/game_map/level01_01.tmj");
	//jtm = JsonTiledMap::create("Resources/map1.tmj");
	auto sp = Sprite::create("Resources/objects.png");
	sp->setPosition(Vec2(0, 0));
	addChild(sp);
	auto sp1 = Sprite::create("Resources/objects.png");
	addChild(sp1);
	sp1->setPosition(Vec2(0, 224));
// 	sp1->setFlipX(true);
 	sp1->setFlipY(true);
	//jtm->setColor(Color3B(20, 20, 20));
	//jtm->setOpacity(20);
	 
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
	
	addChild(jtm,2);

// 	auto lay = jtm->getLayer("Layer1");
// 	//lay->setLayerScale(Vec2(1, 1));
// 	sp = lay->getTileAt(Vec2(0, 11));
// // 	auto lay = jtm->getLayer("B2");
// // 	
// // 	sp = lay->getTileAt(Vec2(4, 13));
// 
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
// 	//jtm->setFlipX(true);
// 
// 	AudioEngine::play2d("Resources/effect1.wav");
// 
	
  	particle = OG::ParticleExample::create();
  	particle->setStyle(ParticleExample::METEOR);
	addChild(particle);
  
	particle->setLocalZOrder(3);
 	auto mouseEvent = EventListenerMouse::create();
 
// 	mouseEvent->onMouseScroll = [&](EventMouse *evnet)
// 	{
// 		auto curLayer = ((JsonTiledMap*)evnet->getCurrentTarget());// ->getLayer("Layer1");
// 		auto scale = curLayer->getMapScale();
// 		if (evnet->getCursorY() > 0)
// 		{
// 			scale.x+=0.01;
// 			scale.y += 0.01;
// 			curLayer->setMapScale(scale);
// 		//	AudioEngine::stop(u8"Resources/��˴��� - ȫ�ǰ�.wav");
// 			AudioEngine::closeAll();
// 		}
// 		if (evnet->getCursorY() < 0)
// 		{
// 			scale.x-=0.01;
// 			scale.y-=0.01;
// 			curLayer->setMapScale(scale);
// 			AudioEngine::play2d(u8"Resources/��˴��� - ȫ�ǰ�.wav");
// 
// 		}
// 		
// 	};
// 
 	mouseEvent->onMouseMove = [&](EventMouse *evnet)
 	{
 		auto curLayer = ((JsonTiledMap*)evnet->getCurrentTarget());// ->getLayer("Layer1");
 		
  		Vec2 mousePos = evnet->getLocation();
  		particle->setPosition(mousePos);
 	};
// 
 	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEvent, particle);
// 
// 	auto sp1 = Sprite::create("Resources/002.jpg");
// 	addChild(sp1);
// 	auto mouseEvent1 = EventListenerMouse::create();
// 	mouseEvent1->onMouseScroll = [&](EventMouse *evnet)
// 	{
// 		Node* sprite = (evnet->getCurrentTarget());// ->getLayer("Layer1");
//  
// 		if (evnet->getCursorY() > 0)
// 		{
// // 			scale.x += 0.01;
// // 			scale.y += 0.01;
// // 			curLayer->setMapScale(scale);
// 			//	AudioEngine::stop(u8"Resources/��˴��� - ȫ�ǰ�.wav");
// 			AudioEngine::closeAll();
// 		}
// 		if (evnet->getCursorY() < 0)
// 		{
// // 			scale.x -= 0.01;
// // 			scale.y -= 0.01;
// // 			curLayer->setMapScale(scale);
// 			AudioEngine::play2d(u8"Resources/��˴��� - ȫ�ǰ�.wav");
// 
// 		}
// 
// 	};
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEvent1, sp1);
// 
// 	
// 
// 	auto J1 = JumpBy::create(1.9, Vec2(80, 0), 230, 1);
// 	
// 	auto J2 = JumpBy::create(1.9, Vec2(-80, 0), 230, 1);
// 	
// 	auto se = Sequence::create(J1,J2, nullptr);
// 	 rr = RepeatForever::create(se);
// 	jtm->runAction(rr);
//  
// 	auto func = [&] (EventCustom* event) {
// 		if(rr!=NULL)
// 		jtm->stopAction(rr);//��ͣ�¼�
// 	};
//  	auto J1event = EventListenerCustom::create("1",func);
//  	_eventDispatcher->addEventListenerWithFixedPriority(J1event, 1);
// 	//_eventDispatcher->removeEventListener(J1event);
// 
// 	
// 	
// 	auto listener1 = EventListenerKeyboard::create();
// 	listener1->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
// 	{
// 
// 		EventCustom event1 = EventCustom("1");
// 		this->_eventDispatcher->dispatchEvent(&event1);
// 
// 		switch (keyCode)
// 		{
// 		case EventKeyboard::KeyCode::KEY_A:
// 		{
// 			auto currentPos = sp->getPosition();
// 			currentPos.x -= 10;
// 			sp->setPosition(currentPos);
// 		}
// 		break;
// 		case EventKeyboard::KeyCode::KEY_D:
// 		{	auto currentPos = sp->getPosition();
// 		currentPos.x += 10;
// 		sp->setPosition(currentPos); }
// 		break;
// 
// 		case EventKeyboard::KeyCode::KEY_W:
// 		{
// 			auto currentPos = sp->getPosition();
// 			currentPos.y -= 10;
// 			sp->setPosition(currentPos);
// 		}
// 		break;
// 		case EventKeyboard::KeyCode::KEY_S:
// 		{	auto currentPos = sp->getPosition();
// 		currentPos.y += 10;
// 		sp->setPosition(currentPos);
// 		}
// 		break;
// 		}
// 
// 	};
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, jtm);
// 
 	this->setUpdate();
	auto t = FadeOut::create(0.5);
	auto t1 = FadeIn::create(0.5);
	auto jump2 = JumpBy::create(1, Vec2(80,0), 60, 2);

	sp1->runAction(RepeatForever::create(Sequence::create( jump2, nullptr)));

	return true;
}
const char *layerArray[] = { "ani1","ani2","ani3" };
int cnt = 0;

static int curLayer = 0;
void HelloWorld::update(float delta)
{
	cnt++;
	if (cnt > 3)
	{
		cnt = 0;
		jtm->getLayer(layerArray[curLayer])->setVisible(false);
		curLayer++;
		if (curLayer >= 3)
			curLayer = 0;
		jtm->getLayer(layerArray[curLayer])->setVisible(true);

	}
}
