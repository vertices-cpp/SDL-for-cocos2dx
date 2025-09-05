 #ifndef _EVENT_DISPATCHER_H_
 #define _EVENT_DISPATCHER_H_
 

 #include "OGPlatformMacros.h"
#include "Vec2.h"
#include "OGRef.h"
#include "OGTouch.h"

#include <unordered_map>

#define EVENT_COME_TO_FOREGROUND    "event_come_to_foreground"

// The renderer[android:GLSurfaceView.Renderer  WP8:Cocos2dRenderer] was recreated.
// This message is used for reloading resources before renderer is recreated on Android/WP8.
// This message is posted in cocos/platform/android/javaactivity.cpp and cocos\platform\wp8-xaml\cpp\Cocos2dRenderer.cpp.
#define EVENT_RENDERER_RECREATED    "event_renderer_recreated"

// The application will come to background.
// This message is used for doing something before coming to background, such as save RenderTexture.
// This message is posted in cocos/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp and cocos\platform\wp8-xaml\cpp\Cocos2dRenderer.cpp.

#define EVENT_COME_TO_BACKGROUND    "event_come_to_background"
 
OG_BEGIN

class Node;
class EventListener;
 class EventDispatcher;
 class EventListenerKeyboard;
 

 //--------------------------------------------Event------------------------------
 class Event :public Ref
 {
 public:
	 /** Type Event type.*/
	 enum Type
	 {
		 UNKNOWN,
		 KEYBOARD,
		 MOUSE,
		 FOCUS,
		 CUSTOM,
		 TOUCH
	 };

	 Event(Type type) :_type(type)
		 , _isStopped(false)
		 , _currentTarget(nullptr)
	 {
	 }

	 virtual ~Event()
	 {
	 }
	 Type getType() const {
		 return _type;
	 }


	 Node* getCurrentTarget() {
		 return _currentTarget;
	 }

	 void setCurrentTarget(Node* target) {
		 _currentTarget = target;
	 }
	 bool isStopped() const { return _isStopped; }
	 bool _isStopped;
	 Type _type;
	 Node* _currentTarget;

	 friend class EventDispatcher;
 };
 
 
 
 class  EventKeyboard :public Event
 {
 public:
	 /**
	  * KeyCode The key (code).
	  * @js NA
	  */
	 enum class KeyCode
	 {
		 KEY_NONE,
		 KEY_PAUSE,
		 KEY_SCROLL_LOCK,
		 KEY_PRINT,
		 KEY_SYSREQ,
		 KEY_BREAK,
		 KEY_ESCAPE,
		 KEY_BACK = KEY_ESCAPE,
		 KEY_BACKSPACE,
		 KEY_TAB,
		 KEY_BACK_TAB,
		 KEY_RETURN,
		 KEY_CAPS_LOCK,
		 KEY_SHIFT,
		 KEY_LEFT_SHIFT = KEY_SHIFT,
		 KEY_RIGHT_SHIFT,
		 KEY_CTRL,
		 KEY_LEFT_CTRL = KEY_CTRL,
		 KEY_RIGHT_CTRL,
		 KEY_ALT,
		 KEY_LEFT_ALT = KEY_ALT,
		 KEY_RIGHT_ALT,
		 KEY_MENU,
		 KEY_HYPER,
		 KEY_INSERT,
		 KEY_HOME,
		 KEY_PG_UP,
		 KEY_DELETE,
		 KEY_END,
		 KEY_PG_DOWN,
		 KEY_LEFT_ARROW,
		 KEY_RIGHT_ARROW,
		 KEY_UP_ARROW,
		 KEY_DOWN_ARROW,
		 KEY_NUM_LOCK,
		 KEY_KP_PLUS,
		 KEY_KP_MINUS,
		 KEY_KP_MULTIPLY,
		 KEY_KP_DIVIDE,
		 KEY_KP_ENTER,
		 KEY_KP_HOME,
		 KEY_KP_UP,
		 KEY_KP_PG_UP,
		 KEY_KP_LEFT,
		 KEY_KP_FIVE,
		 KEY_KP_RIGHT,
		 KEY_KP_END,
		 KEY_KP_DOWN,
		 KEY_KP_PG_DOWN,
		 KEY_KP_INSERT,
		 KEY_KP_DELETE,
		 KEY_F1,
		 KEY_F2,
		 KEY_F3,
		 KEY_F4,
		 KEY_F5,
		 KEY_F6,
		 KEY_F7,
		 KEY_F8,
		 KEY_F9,
		 KEY_F10,
		 KEY_F11,
		 KEY_F12,
		 KEY_SPACE,
		 KEY_EXCLAM,
		 KEY_QUOTE,
		 KEY_NUMBER,
		 KEY_DOLLAR,
		 KEY_PERCENT,
		 KEY_CIRCUMFLEX,
		 KEY_AMPERSAND,
		 KEY_APOSTROPHE,
		 KEY_LEFT_PARENTHESIS,
		 KEY_RIGHT_PARENTHESIS,
		 KEY_ASTERISK,
		 KEY_PLUS,
		 KEY_COMMA,
		 KEY_MINUS,
		 KEY_PERIOD,
		 KEY_SLASH,
		 KEY_0,
		 KEY_1,
		 KEY_2,
		 KEY_3,
		 KEY_4,
		 KEY_5,
		 KEY_6,
		 KEY_7,
		 KEY_8,
		 KEY_9,
		 KEY_COLON,
		 KEY_SEMICOLON,
		 KEY_LESS_THAN,
		 KEY_EQUAL,
		 KEY_GREATER_THAN,
		 KEY_QUESTION,
		 KEY_AT,
		 KEY_CAPITAL_A,
		 KEY_CAPITAL_B,
		 KEY_CAPITAL_C,
		 KEY_CAPITAL_D,
		 KEY_CAPITAL_E,
		 KEY_CAPITAL_F,
		 KEY_CAPITAL_G,
		 KEY_CAPITAL_H,
		 KEY_CAPITAL_I,
		 KEY_CAPITAL_J,
		 KEY_CAPITAL_K,
		 KEY_CAPITAL_L,
		 KEY_CAPITAL_M,
		 KEY_CAPITAL_N,
		 KEY_CAPITAL_O,
		 KEY_CAPITAL_P,
		 KEY_CAPITAL_Q,
		 KEY_CAPITAL_R,
		 KEY_CAPITAL_S,
		 KEY_CAPITAL_T,
		 KEY_CAPITAL_U,
		 KEY_CAPITAL_V,
		 KEY_CAPITAL_W,
		 KEY_CAPITAL_X,
		 KEY_CAPITAL_Y,
		 KEY_CAPITAL_Z,
		 KEY_LEFT_BRACKET,
		 KEY_BACK_SLASH,
		 KEY_RIGHT_BRACKET,
		 KEY_UNDERSCORE,
		 KEY_GRAVE,
		 KEY_A,
		 KEY_B,
		 KEY_C,
		 KEY_D,
		 KEY_E,
		 KEY_F,
		 KEY_G,
		 KEY_H,
		 KEY_I,
		 KEY_J,
		 KEY_K,
		 KEY_L,
		 KEY_M,
		 KEY_N,
		 KEY_O,
		 KEY_P,
		 KEY_Q,
		 KEY_R,
		 KEY_S,
		 KEY_T,
		 KEY_U,
		 KEY_V,
		 KEY_W,
		 KEY_X,
		 KEY_Y,
		 KEY_Z,
		 KEY_LEFT_BRACE,
		 KEY_BAR,
		 KEY_RIGHT_BRACE,
		 KEY_TILDE,
		 KEY_EURO,
		 KEY_POUND,
		 KEY_YEN,
		 KEY_MIDDLE_DOT,
		 KEY_SEARCH,
		 KEY_DPAD_LEFT,
		 KEY_DPAD_RIGHT,
		 KEY_DPAD_UP,
		 KEY_DPAD_DOWN,
		 KEY_DPAD_CENTER,
		 KEY_ENTER,
		 KEY_PLAY,
		 KEY_KP_PERIOD	//添加小键盘的英文句话
	 };

	 /** Constructor.
	  *
	  * @param keyCode A given keycode.
	  * @param isPressed True if the key is pressed.
	  * @js ctor
	  */
	 EventKeyboard(KeyCode keyCode, bool isPressed);

 private:
	 KeyCode _keyCode;
	 bool _isPressed;

	 friend class EventListenerKeyboard;
 };

 class EventMouse : public Event
 {
 public:
	 /**
	 * MouseEventType Different types of MouseEvent.
	 * @js NA
	 */
	 enum class MouseEventType
	 {
		 MOUSE_NONE,
		 MOUSE_DOWN,
		 MOUSE_UP,
		 MOUSE_MOVE,
		 MOUSE_SCROLL,
	 };

	 enum class MouseButton
	 {
		 BUTTON_UNSET = -1,
		 BUTTON_LEFT = 0,
		 BUTTON_RIGHT = 1,
		 BUTTON_MIDDLE = 2,
		 BUTTON_4 = 3,
		 BUTTON_5 = 4,
		 BUTTON_6 = 5,
		 BUTTON_7 = 6,
		 BUTTON_8 = 7
	 };

	 EventMouse(MouseEventType mouseEventCode);

	 void setScrollData(float scrollX, float scrollY) { _scrollX = scrollX; _scrollY = scrollY; }

	 float getScrollX() const { return _scrollX; }

	 float getScrollY() const { return _scrollY; }

	 void setCursorPosition(float x, float y) {
		 _x = x;
		 _y = y;
		 _prevPoint = _point;
		 _point.x = x;
		 _point.y = y;
		 if (!_startPointCaptured)
		 {
			 _startPoint = _point;
			 _startPointCaptured = true;
		 }
	 }

	 void setMouseButton(MouseButton button) { _mouseButton = button; }
	 

	 MouseButton getMouseButton() const { return _mouseButton; }

	 float getCursorX() const { return _x; }
	
	 float getCursorY() const { return _y; }

	 Vec2  getLocation() const
	 {
		 return  (_point);
	 }
	 Vec2  getPreviousLocation() const
	 {
		 return  (_prevPoint);
	 }
	 Vec2 getDelta() const{ return getLocation() - getPreviousLocation(); }
	 
 private:
	 MouseEventType _mouseEventType;
	 MouseButton _mouseButton;
	 float _x;
	 float _y;
	 float _scrollX;
	 float _scrollY;

	 bool _startPointCaptured;
	 Vec2 _startPoint;
	 Vec2 _point;
	 Vec2 _prevPoint;

	 friend class EventListenerMouse;
 };
 
 //--------------------------------------------EventCustom------------------------------
 class  EventCustom : public Event
 {
 public:
	 /** Constructor.
	  *
	  * @param eventName A given name of the custom event.
	  * @js ctor
	  */
	 EventCustom(const std::string& eventName);

	 /** Sets user data.
	  *
	  * @param data The user data pointer, it's a void*.
	  */
	 void setUserData(void* data) { _userData = data; }

	 /** Gets user data.
	  *
	  * @return The user data pointer, it's a void*.
	  */
	 void* getUserData() const { return _userData; }

	 /** Gets event name.
	  *
	  * @return The name of the event.
	  */
	 const std::string& getEventName() const { return _eventName; }
 protected:
	 void* _userData;       ///< User data
	 std::string _eventName;
 };


 //--------------------------------------EventTouch------------------------------------



 class   EventTouch : public Event
 {
 public:
	 static const int MAX_TOUCHES = 15;

	 /** EventCode Touch event code.*/
	 enum class EventCode
	 {
		 BEGAN,
		 MOVED,
		 ENDED,
		 CANCELLED
	 };

	 /**
	  * Constructor.
	  * @js NA
	  */
	 EventTouch();

	 /** Get event code.
	  *
	  * @return The code of the event.
	  */
	 EventCode getEventCode() const { return _eventCode; }

	 /** Get the touches.
	  *
	  * @return The touches of the event.
	  */
	 const std::vector<Touch*>& getTouches() const { return _touches; }


	 /** Set the event code.
	  *
	  * @param eventCode A given EventCode.
	  */
	 void setEventCode(EventCode eventCode) { _eventCode = eventCode; };
	 /** Set the touches
	  *
	  * @param touches A given touches vector.
	  */
	 void setTouches(const std::vector<Touch*>& touches) { _touches = touches; };


 private:
	 EventCode _eventCode;
	 std::vector<Touch*> _touches;

	 friend class Input;
 };

 //--------------------------------------------EventListener------------------------------

 class EventListener:public Ref
 {
 public:
	 /** Type Event type.*/
	 enum class Type
	 {
		 UNKNOWN,
		 KEYBOARD,
		 MOUSE,
		 TOUCH_ONE_BY_ONE,
		 TOUCH_ALL_AT_ONCE,
		 CUSTOM
		 // 		ACCELERATION,
		 // 		FOCUS,
		 // 		GAME_CONTROLLER,
		 // 		CUSTOM
	 };

	 typedef std::string ListenerID;

	 bool init(Type t, const ListenerID& listenerID, const std::function<void(Event*)>& callback);


 public:

	 EventListener() {}
	 virtual ~EventListener() {}

	 virtual bool checkAvailable() = 0;
	 virtual EventListener* clone() = 0;

	 void setEnabled(bool enabled) { _isEnabled = enabled; }

	 bool isEnabled() const {
		 return _isEnabled; 
	 }
	 void setPaused(bool paused) { _paused = paused; }
	 bool isPaused() const { return _paused; }
	 /** Marks the listener was registered by EventDispatcher */
	 void setRegistered(bool registered) { _isRegistered = registered; }

	 /** Checks whether the listener was registered by EventDispatcher */
	 bool isRegistered() const { return _isRegistered; }


	 Type getType() const { return _type; }

	 const ListenerID& getListenerID()const {
		 return _listenerID;
	 }

	 void setFixedPriority(int fixedPriority) { _fixedPriority = fixedPriority; }
	 int getFixedPriority() const { return _fixedPriority; }

	 void setAssociatedNode(Node* node) { _node = node; }

	 Node* getAssociatedNode() const { return _node; }

	 std::function<void(Event*)> _onEvent;   /// Event callback function

	 Type _type;                             /// Event listener type
	 ListenerID _listenerID;                 /// Event listener ID
	 bool _isRegistered;                     /// Whether the listener has been added to dispatcher.

	 int   _fixedPriority;   // The higher the number, the higher the priority, 0 is for scene graph base priority.
	 Node* _node;            // scene graph based priority
	 bool _paused;           // Whether the listener is paused
	 bool _isEnabled;        // Whether the listener is enabled
	 friend class EventDispatcher;
 };
 
 
 
 class  EventListenerKeyboard : public EventListener
 {
 public:
 	static const std::string LISTENER_ID;
 
 	CREATE_FUNC(EventListenerKeyboard);
 
 	bool init();
 	
	virtual EventListenerKeyboard* clone() override;
	virtual bool checkAvailable() override;
 
 	//包含两个事件回调的调用
 	std::function<void(EventKeyboard::KeyCode, Event*)> onKeyPressed;
 	std::function<void(EventKeyboard::KeyCode, Event*)> onKeyReleased;
 
 	EventListenerKeyboard()
 		: onKeyPressed(nullptr)
 		, onKeyReleased(nullptr)
 	{
 	}
 
 };

 class EventListenerMouse : public EventListener
 {
 public:
	 static const std::string LISTENER_ID;

	 CREATE_FUNC(EventListenerMouse);

	 /// Overrides
	 virtual EventListenerMouse* clone() override;
	 virtual bool checkAvailable() override;

	 std::function<void(EventMouse* event)> onMouseDown;
	 std::function<void(EventMouse* event)> onMouseUp;
	 std::function<void(EventMouse* event)> onMouseMove;
	 std::function<void(EventMouse* event)> onMouseScroll;

	 EventListenerMouse();
	 bool init();
 };

 //------------------------------------------EventListenerCustom------------------------------------------

 class   EventListenerCustom : public EventListener
 {
 public:
	 /** Creates an event listener with type and callback.
	  * @param eventName The type of the event.
	  * @param callback The callback function when the specified event was emitted.
	  * @return An autoreleased EventListenerCustom object.
	  */
	 static EventListenerCustom* create(const std::string& eventName, const std::function<void(EventCustom*)>& callback);

	 /// Overrides
	 virtual bool checkAvailable() override;
	 virtual EventListenerCustom* clone() override;

 
	 /** Constructor */
	 EventListenerCustom();

	 /** Initializes event with type and callback function */
	 bool init(const ListenerID& listenerId, const std::function<void(EventCustom*)>& callback);

 protected:
	 std::function<void(EventCustom*)> _onCustomEvent;
	  
 };


 //--------------------------------------EventListenerTouchOneByOne------------------------------------
 class   EventListenerTouchOneByOne : public EventListener
 {
 public:
	 static const std::string LISTENER_ID;

	 /** Create a one by one touch event listener.
	  */
	 static EventListenerTouchOneByOne* create();

	 /**
	  * Destructor.
	  * @js NA
	  */
	 virtual ~EventListenerTouchOneByOne();

	 /** Whether or not to swall touches.
	  *
	  * @param needSwallow True if needs to swall touches.
	  */
	 void setSwallowTouches(bool needSwallow);
	 /** Is swall touches or not.
	  *
	  * @return True if needs to swall touches.
	  */
	 bool isSwallowTouches();

	 /// s
	 virtual EventListenerTouchOneByOne* clone();
	 virtual bool checkAvailable();
	 //

 public:

	 typedef std::function<bool(Touch*, Event*)> ogTouchBeganCallback;
	 typedef std::function<void(Touch*, Event*)> ogTouchCallback;

	 ogTouchBeganCallback onTouchBegan;
	 ogTouchCallback onTouchMoved;
	 ogTouchCallback onTouchEnded;
	 ogTouchCallback onTouchCancelled;


	 EventListenerTouchOneByOne();
	 bool init();

 private:
	 std::vector<Touch*> _claimedTouches;
	 bool _needSwallow;

	 friend class EventDispatcher;
 };
 //--------------------------------------EventListenerTouchAllAtOnce------------------------------------
 /** @class EventListenerTouchAllAtOnce
  * @brief Multiple touches event listener.
  */
 class   EventListenerTouchAllAtOnce : public EventListener
 {
 public:
	 static const std::string LISTENER_ID;

	 /** Create a all at once event listener.
	  *
	  * @return An autoreleased EventListenerTouchAllAtOnce object.
	  */
	 static EventListenerTouchAllAtOnce* create();
	 /** Destructor.
	  * @js NA
	  */
	 virtual ~EventListenerTouchAllAtOnce();

	 /// s
	 virtual EventListenerTouchAllAtOnce* clone();
	 virtual bool checkAvailable();
	 //
 public:

	 typedef std::function<void(const std::vector<Touch*>&, Event*)> ogTouchesCallback;

	 ogTouchesCallback onTouchesBegan;
	 ogTouchesCallback onTouchesMoved;
	 ogTouchesCallback onTouchesEnded;
	 ogTouchesCallback onTouchesCancelled;


	 EventListenerTouchAllAtOnce();
	 bool init();
 private:

	 friend class EventDispatcher;
 };

  //------------------------------------------EventDispatcher------------------------------------------

//  class EventDispatcher:public Ref {
// 
// 	 //------------------------------------------_uEventNode------------------------------------------
// 
// 	 typedef struct _uEventNode
// 	 {
// 		 struct _uEventNode   *prev, *next;
// 		 EventListener * target;
// 	 } uEventNode, *uPEventNode;
// 
// 	 //------------------------------------------EventListenerVector------------------------------------------
// 
// 	 class EventListenerVector
// 	 {
// 	 public:
// 		 uPEventNode _sceneGraphListeners;
// 	 public:
// 		 EventListenerVector();
// 		 ~EventListenerVector();
// 
// 		 void clearSceneGraphListeners();
// 
// 
// 		 bool empty() const { return _sceneGraphListeners == nullptr; }
// 
// 		 void push_back(EventListener* target);
// 
// 		 
// 
// 		 void removeEvent(EventListener * target);
// 		 void removeEvent(Node * target);
// 
// 		 uPEventNode findNode(Node * target);
// 
// 		 uPEventNode getSceneGraphPriorityListeners() const { return _sceneGraphListeners; }
// 
// 	 private:
// 
// 	 };
// 
//  public:
// 	 EventDispatcher() {}
// 	 ~EventDispatcher() {
// 		 for (auto &i : _listenerMap)
// 		 {
// 			 delete i.second;
// 		 }
// 		 _listenerMap.clear();
// 	 }
// 	 std::unordered_map<EventListener::ListenerID, EventListenerVector*> getListMap() { return _listenerMap; }
// 
// 	
// 	 void addEventListenerWithSceneGraphPriority(EventListener* listener, Node* node);
// 	 EventListenerCustom* addCustomEventListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback);
// 
// 	 void addEventListener(EventListener * listener);
// 	 void removeEventListener(EventListener * listener);
// 	 void removeEventListenersForTarget(Node * target);
// 
// // 	 void EventDispatcher::removeCustomEventListeners(const std::string& customEventName,
// // 		 std::function<void(Event *)> callback);
// // 	 void  removeEventListenersForListenerID(const EventListener::ListenerID& listenerID,
// // 		 std::function<void(Event *)> callback);
// 
// 	 void  removeCustomEventListeners(const std::string& customEventName);
// 	 void  removeEventListenersForListenerID(const EventListener::ListenerID& listenerID);
// 
// 	 void dispatchEventToListeners(EventListenerVector* listeners, const std::function<void(EventListener*)>& onEvent);
// 
// 	 void dispatchTouchEventToListeners(EventListenerVector* listeners, const std::function<void(EventListener*)>& onEvent);
// 
// 	 void dispatchEvent(Event * event);
// 
// 	 void dispatchCustomEvent(const std::string & eventName, void * optionalUserData = nullptr);
// 
// 	 //包含事件的基类成员
// 	 std::unordered_map<EventListener::ListenerID, EventListenerVector*> _listenerMap;
// 
//  };
 
 
 //------------------------------------------EventDispatcher------------------------------------------
 
 class   EventDispatcher : public Ref
 {
 public:
	 // Adds event listener.
	 void addEventListenerWithSceneGraphPriority(EventListener* listener, Node* node); 
	 void addEventListenerWithFixedPriority(EventListener* listener, int fixedPriority); 
	 EventListenerCustom* addCustomEventListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback); 
	 void removeEventListener(EventListener* listener); 
	 void removeEventListenersForType(EventListener::Type listenerType); 
	 void removeEventListenersForTarget(Node* target, bool recursive = false); 
	 void removeCustomEventListeners(const std::string& customEventName); 
	 void removeAllEventListeners(); 
	 void pauseEventListenersForTarget(Node* target, bool recursive = false); 
	 void resumeEventListenersForTarget(Node* target, bool recursive = false); 
	 void setPriority(EventListener* listener, int fixedPriority); 
	 void setEnabled(bool isEnabled); 
	 bool isEnabled() const; 
	 void dispatchEvent(Event* event); 
	 void dispatchCustomEvent(const std::string &eventName, void *optionalUserData = nullptr); 
	 bool hasEventListener(const EventListener::ListenerID& listenerID) const;

	 /////////////////////////////////////////////

	 /** Constructor of EventDispatcher.
	  */
	 EventDispatcher();
	 /** Destructor of EventDispatcher.
	  */
	 ~EventDispatcher();

#if CC_NODE_DEBUG_VERIFY_EVENT_LISTENERS && COCOS2D_DEBUG > 0

	 /**
	  * To help track down event listener issues in debug builds.
	  * Verifies that the node has no event listeners associated with it when destroyed.
	  */
	 void debugCheckNodeHasNoEventListenersOnDestruction(Node* node);

#endif

 protected:
	 friend class Node;

	 /** Sets the dirty flag for a node. */
	 void setDirtyForNode(Node* node);

	 /**
	  *  The vector to store event listeners with scene graph based priority and fixed priority.
	  */
	 class EventListenerVector
	 {
	 public:
		 EventListenerVector();
		 ~EventListenerVector();
		 size_t size() const;
		 bool empty() const;

		 void push_back(EventListener* item);
		 void clearSceneGraphListeners();
		 void clearFixedListeners();
		 void clear();

		 std::vector<EventListener*>* getFixedPriorityListeners() const { return _fixedListeners; }
		 std::vector<EventListener*>* getSceneGraphPriorityListeners() const { return _sceneGraphListeners; }
		 long getGt0Index() const { return _gt0Index; }
		 void setGt0Index(long index) { _gt0Index = index; }
	 private:
		 std::vector<EventListener*>* _fixedListeners;
		 std::vector<EventListener*>* _sceneGraphListeners;
		 long _gt0Index;
	 };

	 /** Adds an event listener with item
	  *  @note if it is dispatching event, the added operation will be delayed to the end of current dispatch
	  *  @see forceAddEventListener
	  */
	 void addEventListener(EventListener* listener);

	 /** Force adding an event listener
	  *  @note force add an event listener which will ignore whether it's in dispatching.
	  *  @see addEventListener
	  */
	 void forceAddEventListener(EventListener* listener);

	 /** Gets event the listener list for the event listener type. */
	 EventListenerVector* getListeners(const EventListener::ListenerID& listenerID) const;

	 /** Update dirty flag */
	 void updateDirtyFlagForSceneGraph();

	 /** Removes all listeners with the same event listener ID */
	 void removeEventListenersForListenerID(const EventListener::ListenerID& listenerID);

	 /** Sort event listener */
	void sortEventListeners(const EventListener::ListenerID& listenerID);
	
		 /** Sorts the listeners of specified type by scene graph priority */
		 void sortEventListenersOfSceneGraphPriority(const EventListener::ListenerID& listenerID, Node* rootNode);

	 /** Sorts the listeners of specified type by fixed priority */
	 void sortEventListenersOfFixedPriority(const EventListener::ListenerID& listenerID);

	 /** Updates all listeners
	  *  1) Removes all listener items that have been marked as 'removed' when dispatching event.
	  *  2) Adds all listener items that have been marked as 'added' when dispatching event.
	  */
	 void updateListeners(Event* event);

	 /** Touch event needs to be processed different with other events since it needs support ALL_AT_ONCE and ONE_BY_NONE mode. */
	 void dispatchTouchEvent(EventTouch* event);

	 /** Associates node with event listener */
	 void associateNodeAndEventListener(Node* node, EventListener* listener);

	 /** Dissociates node with event listener */
	 void dissociateNodeAndEventListener(Node* node, EventListener* listener);

	 /** Dispatches event to listeners with a specified listener type */
	 void dispatchEventToListeners(EventListenerVector* listeners, const std::function<bool(EventListener*)>& onEvent);

	 /** Special version dispatchEventToListeners for touch/mouse event.
	  *
	  *  Touch/mouse event process flow different with common event,
	  *      for scene graph node listeners, touch event process flow should
	  *      order by viewport/camera first, because the touch location convert
	  *      to 3D world space is different by different camera.
	  *  When listener process touch event, can get current camera by Camera::getVisitingCamera().
	  */
	 void dispatchTouchEventToListeners(EventListenerVector* listeners, const std::function<bool(EventListener*)>& onEvent);

	 void releaseListener(EventListener* listener);

	 /// Priority dirty flag
	 enum class DirtyFlag
	 {
		 NONE = 0,
		 FIXED_PRIORITY = 1 << 0,
		 SCENE_GRAPH_PRIORITY = 1 << 1,
		 ALL = FIXED_PRIORITY | SCENE_GRAPH_PRIORITY
	 };

	 /** Sets the dirty flag for a specified listener ID */
	 void setDirty(const EventListener::ListenerID& listenerID, DirtyFlag flag);

	 /** Walks though scene graph to get the draw order for each node, it's called before sorting event listener with scene graph priority */
	 void visitTarget(Node* node, bool isRootNode);

	 /** Remove all listeners in _toRemoveListeners list and cleanup */
	 void cleanToRemovedListeners();

	 /** Listeners map */
	 std::unordered_map<EventListener::ListenerID, EventListenerVector*> _listenerMap;

	 /** The map of dirty flag */
	 std::unordered_map<EventListener::ListenerID, DirtyFlag> _priorityDirtyFlagMap;

	 /** The map of node and event listeners */
	 std::unordered_map<Node*, std::vector<EventListener*>*> _nodeListenersMap;

	 /** The map of node and its event priority */
	 std::unordered_map<Node*, int> _nodePriorityMap;

	 /** key: Global Z Order, value: Sorted Nodes */
	 std::unordered_map<float, std::vector<Node*>> _globalZOrderNodeMap;

	 /** The listeners to be added after dispatching event */
	 std::vector<EventListener*> _toAddedListeners;

	 /** The listeners to be removed after dispatching event */
	 std::vector<EventListener*> _toRemovedListeners;

	 /** The nodes were associated with scene graph based priority listeners */
	 std::set<Node*> _dirtyNodes;

	 /** Whether the dispatcher is dispatching event */
	 int _inDispatch;

	 /** Whether to enable dispatching event */
	 bool _isEnabled;

	 int _nodePriorityIndex;

	 std::set<std::string> _internalCustomListenerIDs;
 };

 OG_END
 
 #endif