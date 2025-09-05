#include "OGDirector.h"
#include "OGInput.h"
#include "OGTimer.h"

#include "OGUpdate.h"
#include "OGActionManager.h"

#include "OGAutoreleasePool.h"

OG_BEGIN

SDLView* Director::_sdlview = SDLView::getInstance();
PoolManager * Director::_poolMananger = PoolManager::getInstance();
//Texture2DManage* Director::_textureCache = Texture2DManage::getInstance();

//Update* Director::_update_ptr = Update::getInstance();


Director::Director()
{
	
}

Director::~Director()
{
	free();
}

void Director::free() {

	OG_SAFE_RELEASE(_curScene);
	OG_SAFE_RELEASE(_update_ptr);
	OG_SAFE_RELEASE(_actionManager);
	OG_SAFE_RELEASE(_eventDispatcher);
}
void Director::setScene(Scene * scene)
{
	_nextScene = scene;
	_nextScene->retain();
}


void Director::init()
{
	if (!_sdlview->get_init_state())
		exit(-1);
	//_sdlview->init(u8"仿cocos2dx的sdl2框架",20,20,1000,1000);

	_curScene = _nextScene = nullptr;



	_update_ptr = new Update;
	
	
	_actionManager = new ActionManager;

	//绑定原始调用
	_update_ptr->setUpdate(_actionManager);

	_eventDispatcher = new EventDispatcher;
	_eventDispatcher->setEnabled(true);

	_paused = false;

	//_contentScaleFactor = 1.0f;

	_quit = false;
}


void Director::pause()
{
	if (_paused)
	{
		return;
	}
	_paused = true;
}
void Director::resume()
{
	if (!_paused)
	{
		return;
	}

	_paused = false;
	//_deltaTime = 0;
	
}


bool Director::EventHandle() {

	 Input::getInstance()->listen();
	return _quit;

}

void Director::mainLoop()
{
	auto Timer = Timer::getInstance();

	Timer->ticks();

	while(!EventHandle())
	{
		Timer->capTimeStart();

// 		SDL_SetWindowTitle(_sdlview->getWin(),
// 			(u8"仿cocos2dx的sdl2框架" + std::to_string(Timer->getFps()) + "/s").c_str());
		auto t = Timer->getFps();
		if (_nextScene != nullptr)
		{
			if (_curScene != nullptr)
			{
				_curScene->onExit();
				_curScene->release();
			}

			_curScene = _nextScene;
			_curScene->onEnter();

			_nextScene = nullptr;
			// 
			// 			curScene->init();
		}
/*		std::cout << Timer->get_delay() << std::endl;*/

		if (_paused == false)
			_update_ptr->update(Timer->get_delay());

		_sdlview->clear();


		_curScene->visit(0);
		_poolMananger->getCurrentPool()->clear();

		_sdlview->refresh();

		Timer->TimeDelay();
	}

}

OG_END


