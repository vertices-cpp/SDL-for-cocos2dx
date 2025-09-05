#ifndef _DIRECTOR_H_
#define _DIRECTOR_H_

#include "SDLView.h"
//#include "Texture2DManage.h"

#include "OGScene.h"


OG_BEGIN

class Node;
class Update;
class ActionManager;
class EventDispatcher;
class PoolManager;

class  Director 
{
public:
	static SDLView *_sdlview;
	static PoolManager * _poolMananger;
	//static Texture2DManage *_textureCache;

	Update *_update_ptr;
	ActionManager *_actionManager;
	EventDispatcher *_eventDispatcher;

	Scene *_curScene;
	Scene *_nextScene;

	bool _paused;
	bool _quit;
//	float _contentScaleFactor;


public:
	Instance(Director);

	Director();
	~Director();

	Scene *getRunningScene() { return _curScene; }
	Size getWinSize()const {
		return _sdlview->getWindowSize();
	}
	Size getVisibleSize()const {
		return _sdlview->getRenderSize();
	}
// 	Texture2DManage* getTextureCache()
// 	{
// 		return _textureCache;
// 	}
	SDLView* getSDLView() { return _sdlview; }
	Update* getUpdatePtr() const { return _update_ptr; }
	ActionManager* getActionManager() const { return _actionManager; }
	EventDispatcher* getEventDispatcher() const { return _eventDispatcher; }

	void setEventDispatcher(EventDispatcher* dispatcher) {
		if (_eventDispatcher != dispatcher)
		{
			_eventDispatcher = dispatcher;
		}
	}
	//设置全局缩放
	Vec2 getContentScaleFactor()const { return _sdlview->getScale(); }
	void setContentScaleFactor(const Vec2& scale) {  _sdlview->setScale(scale); }
// 	Size setContentScaleFactor(Size scaleFactor) {
// 		if (_sdlview->getRenderSize() != scaleFactor)
// 			_sdlview->SetRenderLogicSize() = scaleFactor;
// 	}
	

	void free();

	void setScene(Scene *scene);

	void init();

	bool isPaused() { return _paused; }
	void pause();
	void resume();

	bool EventHandle();

	void Quit() { _quit = true; }
	void mainLoop();

	friend class Node;
};



OG_END

#endif
