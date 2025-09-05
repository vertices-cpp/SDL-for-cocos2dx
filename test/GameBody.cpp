#include "GameBody.h"

#include "HelloWorld.h"

GameBody::GameBody()
{
}


GameBody::~GameBody()
{
}


GameBody& GameBody::init()
{
	auto _sdlview = SDLView::getInstance();

	_sdlview->init("test",20,20,1000,1000);

	auto director = Director::getInstance();
	director->init();

	auto scene = HelloWorld::createScene();
	director->setScene(scene);

	return *this;
}

GameBody & GameBody::run()
{
	Director::getInstance()->mainLoop();
	return *this;
}

