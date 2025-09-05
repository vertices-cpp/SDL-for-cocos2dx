#ifndef _INPUT_H_
#define _INPUT_H_


#include "OGDirector.h"
#include "OGKeyCode.h"

OG_BEGIN
namespace {
	static unsigned int g_indexBitsUsed = 0;
	static Touch* g_touches[EventTouch::MAX_TOUCHES] = { nullptr };
	static int getUnUsedIndex()
	{
		int i;
		int temp = g_indexBitsUsed;

		for (i = 0; i < EventTouch::MAX_TOUCHES; i++) {
			if (!(temp & 0x00000001)) {
				g_indexBitsUsed |= (1 << i);
				return i;
			}

			temp >>= 1;
		}

		// all bits are used
		return -1;
	}
	static std::vector<Touch*> getAllTouchesVector()
	{
		std::vector<Touch*> ret;
		int i;
		int temp = g_indexBitsUsed;

		for (i = 0; i < EventTouch::MAX_TOUCHES; i++) {
			if (temp & 0x00000001) {
				ret.push_back(g_touches[i]);
			}
			temp >>= 1;
		}
		return ret;
	}

	static void removeUsedIndexBit(int index)
	{
		if (index < 0 || index >= EventTouch::MAX_TOUCHES)
		{
			return;
		}

		unsigned int temp = 1 << index;
		temp = ~temp;
		g_indexBitsUsed &= temp;
	}
}

class Input {
	float _mouseX;
	float _mouseY;

	const Uint8 *mKeyState;
public:
	Instance(Input);
	Input() {

		KeyDown(); 

		g_keyCodeMap.clear();
		for (auto& item : g_keyCodeStructArray)
		{
			g_keyCodeMap[item.SDLKeyCode] = item.keyCode;
		}
		for (auto& item : g_MouseKeyCodeStructArray)
		{
			g_MouseKeyCodeMap[item.SDLKeyCode] = item.mouseKeyCode;
		}

		for (auto& item : g_MouseButtonCodeStructArray)
		{
			g_MouseButtonCodeMap[item.SDLKeyCode] = item.mouseButtonCode;
		}

	}
	void KeyDown() { mKeyState = SDL_GetKeyboardState(nullptr); }

	void listen() {
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				Director::getInstance()->Quit();
			}
			else if (e.type == SDL_WINDOWEVENT)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					SDLView::getInstance()->upWindowSize();
					break;
				}
			}
			else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			{
				if (e.type == SDL_KEYDOWN && AllKeyMap[e.key.keysym.scancode] != true)
				{
					KeyDown();
					EventKeyboard event(g_keyCodeMap[e.key.keysym.scancode], 1);
					Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

					AllKeyMap[e.key.keysym.scancode] = true;
				}
				else if (e.type == SDL_KEYUP)
				{
					KeyDown();
					EventKeyboard event(g_keyCodeMap[e.key.keysym.scancode], 0);
					Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

					AllKeyMap[e.key.keysym.scancode] = false;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEMOTION ||
				e.type == SDL_MOUSEWHEEL)
			{
				if (e.type == SDL_MOUSEBUTTONDOWN && AllKeyMap[e.button.button] != true)
				{
					EventMouse event(EventMouse::MouseEventType::MOUSE_DOWN);
					event.setCursorPosition((float)e.button.x /getFrameZoomFactor().x,
						(float)e.button.y /getFrameZoomFactor().y);

					event.setMouseButton(static_cast<EventMouse::MouseButton>(g_MouseButtonCodeMap[e.button.button]));
					Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

					AllKeyMap[e.button.button] = true;


				}
				else if (e.type == SDL_MOUSEBUTTONUP)
				{
					EventMouse event(EventMouse::MouseEventType::MOUSE_UP);

					event.setCursorPosition((float)e.button.x /getFrameZoomFactor().x,
						(float)e.button.y /getFrameZoomFactor().y);

					event.setMouseButton(static_cast<EventMouse::MouseButton>(g_MouseButtonCodeMap[e.button.button]));

					Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

					AllKeyMap[e.button.button] = false;


				}
				else if (e.type == SDL_MOUSEMOTION)
				{

					_mouseX = (float)e.motion.x/ this->getFrameZoomFactor().x;
					_mouseY = (float)e.motion.y / this->getFrameZoomFactor().y;


					// 					if (_isInRetinaMonitor)
					// 					{
					// 						if (_retinaFactor == 1)
					// 						{
					// 							_mouseX *= 2;
					// 							_mouseY *= 2;
					// 						}
					// 					}

					// 					if (_captured)
					// 					{
					// 						int64_t id = 0;
					// 						this->handleTouchesMove(1, &id, &_mouseX, &_mouseY);
					// 					}

										//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
					// 					float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
					// 					float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

					EventMouse event(EventMouse::MouseEventType::MOUSE_MOVE);
					// Set current button
					if (AllKeyMap[SDL_BUTTON_LEFT] == true)
					{
						event.setMouseButton(static_cast<EventMouse::MouseButton>(EventMouse::MouseButton::BUTTON_LEFT));
					}
					else if (AllKeyMap[SDL_BUTTON_RIGHT] == true)
					{
						event.setMouseButton(static_cast<EventMouse::MouseButton>(EventMouse::MouseButton::BUTTON_RIGHT));
					}
					else if (AllKeyMap[SDL_BUTTON_MIDDLE] == true)
					{
						event.setMouseButton(static_cast<EventMouse::MouseButton>(EventMouse::MouseButton::BUTTON_MIDDLE));
					}
					event.setCursorPosition(_mouseX, _mouseY);
					Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);


				}
				else if (e.type == SDL_MOUSEWHEEL)
				{
					EventMouse event(EventMouse::MouseEventType::MOUSE_SCROLL);
					//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
					float cursorX = e.wheel.x / getFrameZoomFactor().x;
					float cursorY = e.wheel.y / getFrameZoomFactor().y;

					event.setScrollData((float)cursorX, -(float)cursorY);
					event.setCursorPosition(cursorX, cursorY);
					Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);


				}
			}
			else if (e.type == SDL_FINGERDOWN || e.type == SDL_FINGERUP || e.type == SDL_FINGERMOTION)
			{
				int64_t id = e.tfinger.fingerId;
				auto logicSize = SDLView::getInstance()->getRenderSize();
				float x = e.tfinger.x *logicSize.width ;
				float y = e.tfinger.y *logicSize.height;




				switch (e.type)
				{
				case SDL_FINGERDOWN:
				{
					int unusedIndex = 0;
					EventTouch touchEvent;


						auto iter = g_touchIdReorderMap.find(id);

						// it is a new touch
						if (iter == g_touchIdReorderMap.end())
						{
							unusedIndex = getUnUsedIndex();

							// The touches is more than MAX_TOUCHES ?
							if (unusedIndex == -1) {
							 
								continue;
							}

							Touch* touch = g_touches[unusedIndex] = new (std::nothrow) Touch();
							touch->setTouchInfo(unusedIndex, x ,y);

							g_touchIdReorderMap.emplace(id, unusedIndex);
							touchEvent._touches.push_back(touch);
						}
					

					if (touchEvent._touches.size() == 0)
					{
						return;
					}

					touchEvent._eventCode = EventTouch::EventCode::BEGAN;
					auto dispatcher = Director::getInstance()->getEventDispatcher();
					dispatcher->dispatchEvent(&touchEvent);
				}
					break;
				case SDL_FINGERUP:
				{

					EventTouch touchEvent;


					auto iter = g_touchIdReorderMap.find(id);
					if (iter == g_touchIdReorderMap.end())
					{
						
						continue;
					}

					/* Add to the set to send to the director */
					Touch* touch = g_touches[iter->second];
					if (touch)
					{

						touch->setTouchInfo(iter->second, x ,y);

						touchEvent._touches.push_back(touch);

						g_touches[iter->second] = nullptr;
						removeUsedIndexBit(iter->second);

						g_touchIdReorderMap.erase(id);
					}
					else
					{

						return;
					}



					if (touchEvent._touches.size() == 0)
					{

						return;
					}

					touchEvent._eventCode = EventTouch::EventCode::ENDED;
					auto dispatcher = Director::getInstance()->getEventDispatcher();
					dispatcher->dispatchEvent(&touchEvent);

					for (auto& touch : touchEvent._touches)
					{
						// release the touch object.
						touch->release();
					}
				}
				break;
				case SDL_FINGERMOTION:
				{
					float force = 0.0f;
					float maxForce = 0.0f;
					EventTouch touchEvent;



					auto iter = g_touchIdReorderMap.find(id);
					if (iter == g_touchIdReorderMap.end())
					{

						continue;
					}


					Touch* touch = g_touches[iter->second];
					if (touch)
					{
						touch->setTouchInfo(iter->second, x, y, force, maxForce);

						touchEvent._touches.push_back(touch);
					}
					else
					{
						// It is error, should return.

						return;
					}


					if (touchEvent._touches.size() == 0)
					{

						return;
					}

					touchEvent._eventCode = EventTouch::EventCode::MOVED;
					auto dispatcher = Director::getInstance()->getEventDispatcher();
					dispatcher->dispatchEvent(&touchEvent);

				}
					break;
				default:
					break;
				}
			}

		}

	}

	Vec2 getFrameZoomFactor()
	{
		return Director::getInstance()->getContentScaleFactor();
	}
	bool GetKeyState(SDL_KeyCode key) {
		
		return mKeyState[key] == 1;
			
	}

	static std::map<int64_t, int> g_touchIdReorderMap;
};


std::map<int64_t, int> Input::g_touchIdReorderMap;

OG_END

#endif
