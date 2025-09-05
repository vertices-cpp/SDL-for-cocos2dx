#ifndef _HELLO_WORLD_H_
#define _HELLO_WORLD_H_

#include "OG_Engine.h"

USING_OG;

class HelloWorld :public Scene
{
	//EventListenerKeyboard *listener;
  
public:
	static Scene * createScene();
	
	HelloWorld() {}
	~HelloWorld() {}

	
	
	

	bool init();

	void update(float delta);


	CREATE_FUNC(HelloWorld);
};

#endif
