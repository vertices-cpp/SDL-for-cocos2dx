#ifndef _KEY_CODE_H
#define _KEY_CODE_H

#include "Header.h"
#include "OGPlatformMacros.h"
#include "OGEventDispatcher.h"

OG_BEGIN

struct keyCodeItem
{
	int SDLKeyCode;
	EventKeyboard::KeyCode keyCode;
};

static std::unordered_map<int, EventKeyboard::KeyCode> g_keyCodeMap;

static keyCodeItem g_keyCodeStructArray[] = {
	/* The unknown key */
	{ /*GLFW_KEY_UNKNOWN*/   SDL_SCANCODE_UNKNOWN      , EventKeyboard::KeyCode::KEY_NONE          },

	/* Printable keys */
	{ /*GLFW_KEY_SPACE*/ SDL_SCANCODE_SPACE          , EventKeyboard::KeyCode::KEY_SPACE         },
	{/* GLFW_KEY_APOSTROPHE*/ SDL_SCANCODE_APOSTROPHE     , EventKeyboard::KeyCode::KEY_APOSTROPHE    },
	{ /*GLFW_KEY_COMMA */ SDL_SCANCODE_COMMA         , EventKeyboard::KeyCode::KEY_COMMA         },
	{/* GLFW_KEY_MINUS  */  SDL_SCANCODE_MINUS       , EventKeyboard::KeyCode::KEY_MINUS         },
	{/* GLFW_KEY_PERIOD */ SDL_SCANCODE_PERIOD        , EventKeyboard::KeyCode::KEY_PERIOD        },
	{/* GLFW_KEY_SLASH*/ SDL_SCANCODE_SLASH          , EventKeyboard::KeyCode::KEY_SLASH         },
	{ SDL_SCANCODE_0               , EventKeyboard::KeyCode::KEY_0             },
	{ SDL_SCANCODE_1               , EventKeyboard::KeyCode::KEY_1             },
	{ SDL_SCANCODE_2               , EventKeyboard::KeyCode::KEY_2             },
	{SDL_SCANCODE_3               , EventKeyboard::KeyCode::KEY_3             },
	{ SDL_SCANCODE_4               , EventKeyboard::KeyCode::KEY_4             },
	{ SDL_SCANCODE_5               , EventKeyboard::KeyCode::KEY_5             },
	{ SDL_SCANCODE_6               , EventKeyboard::KeyCode::KEY_6             },
	{SDL_SCANCODE_7               , EventKeyboard::KeyCode::KEY_7             },
	{SDL_SCANCODE_8               , EventKeyboard::KeyCode::KEY_8             },
	{ SDL_SCANCODE_9               , EventKeyboard::KeyCode::KEY_9             },
	{ SDL_SCANCODE_SEMICOLON       , EventKeyboard::KeyCode::KEY_SEMICOLON     },
	{ SDL_SCANCODE_EQUALS           , EventKeyboard::KeyCode::KEY_EQUAL         },
	{ SDL_SCANCODE_A               , EventKeyboard::KeyCode::KEY_A             },
	{ SDL_SCANCODE_B               , EventKeyboard::KeyCode::KEY_B             },
	{ SDL_SCANCODE_C               , EventKeyboard::KeyCode::KEY_C             },
	{ SDL_SCANCODE_D               , EventKeyboard::KeyCode::KEY_D             },
	{SDL_SCANCODE_E               , EventKeyboard::KeyCode::KEY_E             },
	{ SDL_SCANCODE_F               , EventKeyboard::KeyCode::KEY_F             },
	{ SDL_SCANCODE_G               , EventKeyboard::KeyCode::KEY_G             },
	{ SDL_SCANCODE_H               , EventKeyboard::KeyCode::KEY_H             },
	{ SDL_SCANCODE_I               , EventKeyboard::KeyCode::KEY_I             },
	{ SDL_SCANCODE_J               , EventKeyboard::KeyCode::KEY_J             },
	{ SDL_SCANCODE_K               , EventKeyboard::KeyCode::KEY_K             },
	{ SDL_SCANCODE_L               , EventKeyboard::KeyCode::KEY_L             },
	{ SDL_SCANCODE_M               , EventKeyboard::KeyCode::KEY_M             },
	{ SDL_SCANCODE_N               , EventKeyboard::KeyCode::KEY_N             },
	{ SDL_SCANCODE_O               , EventKeyboard::KeyCode::KEY_O             },
	{SDL_SCANCODE_P               , EventKeyboard::KeyCode::KEY_P             },
	{SDL_SCANCODE_Q               , EventKeyboard::KeyCode::KEY_Q             },
	{ SDL_SCANCODE_R               , EventKeyboard::KeyCode::KEY_R             },
	{SDL_SCANCODE_S               , EventKeyboard::KeyCode::KEY_S             },
	{SDL_SCANCODE_T               , EventKeyboard::KeyCode::KEY_T             },
	{SDL_SCANCODE_U               , EventKeyboard::KeyCode::KEY_U             },
	{ SDL_SCANCODE_V               , EventKeyboard::KeyCode::KEY_V             },
	{ SDL_SCANCODE_W               , EventKeyboard::KeyCode::KEY_W             },
	{ SDL_SCANCODE_X               , EventKeyboard::KeyCode::KEY_X             },
	{ SDL_SCANCODE_Y               , EventKeyboard::KeyCode::KEY_Y             },
	{ SDL_SCANCODE_Z               , EventKeyboard::KeyCode::KEY_Z             },
	{ SDL_SCANCODE_LEFTBRACKET    , EventKeyboard::KeyCode::KEY_LEFT_BRACKET  },
	{ SDL_SCANCODE_BACKSLASH       , EventKeyboard::KeyCode::KEY_BACK_SLASH    },
	{ SDL_SCANCODE_RIGHTBRACKET   , EventKeyboard::KeyCode::KEY_RIGHT_BRACKET },
	{ SDL_SCANCODE_GRAVE    , EventKeyboard::KeyCode::KEY_GRAVE         },
	//{ GLFW_KEY_WORLD_1         , EventKeyboard::KeyCode::KEY_GRAVE         },
	//{ GLFW_KEY_WORLD_2         , EventKeyboard::KeyCode::KEY_NONE          },

	/* Function keys */
	{ SDL_SCANCODE_ESCAPE          , EventKeyboard::KeyCode::KEY_ESCAPE        },
	{ /*GLFW_KEY_ENTER*/ SDL_SCANCODE_RETURN          , EventKeyboard::KeyCode::KEY_ENTER      },
	{ SDL_SCANCODE_TAB             , EventKeyboard::KeyCode::KEY_TAB           },
	{SDL_SCANCODE_BACKSPACE       , EventKeyboard::KeyCode::KEY_BACKSPACE     },
	{SDL_SCANCODE_INSERT          , EventKeyboard::KeyCode::KEY_INSERT        },
	{ SDL_SCANCODE_DELETE          , EventKeyboard::KeyCode::KEY_DELETE        },
	{ SDL_SCANCODE_RIGHT           , EventKeyboard::KeyCode::KEY_RIGHT_ARROW   },
	{ SDL_SCANCODE_LEFT            , EventKeyboard::KeyCode::KEY_LEFT_ARROW    },
	{SDL_SCANCODE_DOWN            , EventKeyboard::KeyCode::KEY_DOWN_ARROW    },
	{ SDL_SCANCODE_UP              , EventKeyboard::KeyCode::KEY_UP_ARROW      },
	{ SDL_SCANCODE_PAGEUP         , EventKeyboard::KeyCode::KEY_PG_UP      },
	{ SDL_SCANCODE_PAGEDOWN       , EventKeyboard::KeyCode::KEY_PG_DOWN    },
	{ SDL_SCANCODE_HOME            , EventKeyboard::KeyCode::KEY_HOME       },
	{ SDL_SCANCODE_END             , EventKeyboard::KeyCode::KEY_END           },
	{ SDL_SCANCODE_CAPSLOCK       , EventKeyboard::KeyCode::KEY_CAPS_LOCK     },
	{ SDL_SCANCODE_SCROLLLOCK     , EventKeyboard::KeyCode::KEY_SCROLL_LOCK   },
	{ SDL_SCANCODE_NUMLOCKCLEAR        , EventKeyboard::KeyCode::KEY_NUM_LOCK      },
	{ SDL_SCANCODE_PRINTSCREEN    , EventKeyboard::KeyCode::KEY_PRINT         },
	{ SDL_SCANCODE_PAUSE           , EventKeyboard::KeyCode::KEY_PAUSE         },
	{ SDL_SCANCODE_F1              , EventKeyboard::KeyCode::KEY_F1            },
	{ SDL_SCANCODE_F2              , EventKeyboard::KeyCode::KEY_F2            },
	{ SDL_SCANCODE_F3              , EventKeyboard::KeyCode::KEY_F3            },
	{ SDL_SCANCODE_F4              , EventKeyboard::KeyCode::KEY_F4            },
	{ SDL_SCANCODE_F5              , EventKeyboard::KeyCode::KEY_F5            },
	{ SDL_SCANCODE_F6              , EventKeyboard::KeyCode::KEY_F6            },
	{ SDL_SCANCODE_F7              , EventKeyboard::KeyCode::KEY_F7            },
	{ SDL_SCANCODE_F8              , EventKeyboard::KeyCode::KEY_F8            },
	{ SDL_SCANCODE_F9              , EventKeyboard::KeyCode::KEY_F9            },
	{ SDL_SCANCODE_F10             , EventKeyboard::KeyCode::KEY_F10           },
	{ SDL_SCANCODE_F11             , EventKeyboard::KeyCode::KEY_F11           },
	{ SDL_SCANCODE_F12             , EventKeyboard::KeyCode::KEY_F12           },
	{ SDL_SCANCODE_F13             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F14             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F15             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F16             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F17             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F18             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F19             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F20             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F21             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F22             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F23             , EventKeyboard::KeyCode::KEY_NONE          },
	{ SDL_SCANCODE_F24             , EventKeyboard::KeyCode::KEY_NONE          },
	//	{ GLFW_KEY_F25             , EventKeyboard::KeyCode::KEY_NONE          },
		{ SDL_SCANCODE_KP_0            , EventKeyboard::KeyCode::KEY_0             },
		{ SDL_SCANCODE_KP_1            , EventKeyboard::KeyCode::KEY_1             },
		{ SDL_SCANCODE_KP_2            , EventKeyboard::KeyCode::KEY_2             },
		{ SDL_SCANCODE_KP_3            , EventKeyboard::KeyCode::KEY_3             },
		{ SDL_SCANCODE_KP_4            , EventKeyboard::KeyCode::KEY_4             },
		{ SDL_SCANCODE_KP_5            , EventKeyboard::KeyCode::KEY_5             },
		{ SDL_SCANCODE_KP_6            , EventKeyboard::KeyCode::KEY_6             },
		{ SDL_SCANCODE_KP_7            , EventKeyboard::KeyCode::KEY_7             },
		{ SDL_SCANCODE_KP_8            , EventKeyboard::KeyCode::KEY_8             },
		{ SDL_SCANCODE_KP_9            , EventKeyboard::KeyCode::KEY_9             },
		{ SDL_SCANCODE_KP_DECIMAL      , EventKeyboard::KeyCode::KEY_PERIOD        },
	{ SDL_SCANCODE_KP_PERIOD      , EventKeyboard::KeyCode::KEY_KP_PERIOD },
		{ SDL_SCANCODE_KP_DIVIDE       , EventKeyboard::KeyCode::KEY_KP_DIVIDE     },
		{ SDL_SCANCODE_KP_MULTIPLY     , EventKeyboard::KeyCode::KEY_KP_MULTIPLY   },
		{ SDL_SCANCODE_KP_MINUS     , EventKeyboard::KeyCode::KEY_KP_MINUS      },
		{ SDL_SCANCODE_KP_PLUS          , EventKeyboard::KeyCode::KEY_KP_PLUS       },
		{ SDL_SCANCODE_KP_ENTER        , EventKeyboard::KeyCode::KEY_KP_ENTER      },
		{ SDL_SCANCODE_KP_EQUALS        , EventKeyboard::KeyCode::KEY_EQUAL         },
		{ SDL_SCANCODE_LSHIFT      , EventKeyboard::KeyCode::KEY_LEFT_SHIFT         },
		{ SDL_SCANCODE_LCTRL    , EventKeyboard::KeyCode::KEY_LEFT_CTRL          },
		{ SDL_SCANCODE_LALT        , EventKeyboard::KeyCode::KEY_LEFT_ALT           },
		//	{ GLFW_KEY_LEFT_SUPER      , EventKeyboard::KeyCode::KEY_HYPER         },
			{ SDL_SCANCODE_RSHIFT     , EventKeyboard::KeyCode::KEY_RIGHT_SHIFT         },
			{ SDL_SCANCODE_RCTRL   , EventKeyboard::KeyCode::KEY_RIGHT_CTRL          },
			{ SDL_SCANCODE_RALT       , EventKeyboard::KeyCode::KEY_RIGHT_ALT           },
			//	{ GLFW_KEY_RIGHT_SUPER     , EventKeyboard::KeyCode::KEY_HYPER         },
				{ SDL_SCANCODE_MENU            , EventKeyboard::KeyCode::KEY_MENU          },
				//	{ GLFW_KEY_LAST            , EventKeyboard::KeyCode::KEY_NONE          }
};


struct MouseKeyCodeItem
{
	int SDLKeyCode;
	EventMouse::MouseEventType mouseKeyCode;
};

static std::unordered_map<int, EventMouse::MouseEventType> g_MouseKeyCodeMap;


static MouseKeyCodeItem g_MouseKeyCodeStructArray[] = {
	{0,EventMouse::MouseEventType::MOUSE_NONE},
{SDL_MOUSEMOTION, EventMouse::MouseEventType::MOUSE_MOVE},
{SDL_MOUSEBUTTONDOWN,EventMouse::MouseEventType::MOUSE_DOWN},        /**< Mouse button pressed */
{SDL_MOUSEBUTTONUP,    EventMouse::MouseEventType::MOUSE_UP},      /**< Mouse button released */
{SDL_MOUSEWHEEL,  EventMouse::MouseEventType::MOUSE_SCROLL}           /**< Mouse wheel motion */
};


struct MouseButtonCodeItem
{
	int SDLKeyCode;
	EventMouse::MouseButton mouseButtonCode;
};

static std::unordered_map<int, EventMouse::MouseButton> g_MouseButtonCodeMap;


static MouseButtonCodeItem g_MouseButtonCodeStructArray[] = {
	{0,EventMouse::MouseButton::BUTTON_UNSET},
{SDL_BUTTON_LEFT, EventMouse::MouseButton::BUTTON_LEFT},
{SDL_BUTTON_RIGHT,EventMouse::MouseButton::BUTTON_RIGHT},        /**< Mouse button pressed */
{SDL_BUTTON_MIDDLE,    EventMouse::MouseButton::BUTTON_MIDDLE}      /**< Mouse button released */
		/**< Mouse wheel motion */
};

static bool AllKeyMap[512];

static const char* KeyStr[]={
"KEY_NONE",
"KEY_PAUSE",
"KEY_SCROLL_LOCK",
"KEY_PRINT",
"KEY_SYSREQ",
"KEY_BREAK",
"KEY_ESCAPE",
//"KEY_BACK = KEY_ESCAPE",
"KEY_BACKSPACE",
"KEY_TAB",
"KEY_BACK_TAB",
"KEY_RETURN",
"KEY_CAPS_LOCK",
//"KEY_SHIFT",
"KEY_LEFT_SHIFT = KEY_SHIFT",
"KEY_RIGHT_SHIFT",
//"KEY_CTRL",
"KEY_LEFT_CTRL = KEY_CTRL",
"KEY_RIGHT_CTRL",
//"KEY_ALT",
"KEY_LEFT_ALT = KEY_ALT",
"KEY_RIGHT_ALT",
"KEY_MENU",
"KEY_HYPER",
"KEY_INSERT",
"KEY_HOME",
"KEY_PG_UP",
"KEY_DELETE",
"KEY_END",
"KEY_PG_DOWN",
"KEY_LEFT_ARROW",
"KEY_RIGHT_ARROW",
"KEY_UP_ARROW",
"KEY_DOWN_ARROW",
"KEY_NUM_LOCK",
"KEY_KP_PLUS",
"KEY_KP_MINUS",
"KEY_KP_MULTIPLY",
"KEY_KP_DIVIDE",
"KEY_KP_ENTER",
"KEY_KP_HOME",
"KEY_KP_UP",
"KEY_KP_PG_UP",
"KEY_KP_LEFT",
"KEY_KP_FIVE",
"KEY_KP_RIGHT",
"KEY_KP_END",
"KEY_KP_DOWN",
"KEY_KP_PG_DOWN",
"KEY_KP_INSERT",
"KEY_KP_DELETE",
"KEY_F1",
"KEY_F2",
"KEY_F3",
"KEY_F4",
"KEY_F5",
"KEY_F6",
"KEY_F7",
"KEY_F8",
"KEY_F9",
"KEY_F10",
"KEY_F11",
"KEY_F12",
"KEY_SPACE",
"KEY_EXCLAM",
"KEY_QUOTE",
"KEY_NUMBER",
"KEY_DOLLAR",
"KEY_PERCENT",
"KEY_CIRCUMFLEX",
"KEY_AMPERSAND",
"KEY_APOSTROPHE",
"KEY_LEFT_PARENTHESIS",
"KEY_RIGHT_PARENTHESIS",
"KEY_ASTERISK",
"KEY_PLUS",
"KEY_COMMA",
"KEY_MINUS",
"KEY_PERIOD",

"KEY_SLASH",
"KEY_0",
"KEY_1",
"KEY_2",
"KEY_3",
"KEY_4",
"KEY_5",
"KEY_6",
"KEY_7",
"KEY_8",
"KEY_9",
"KEY_COLON",
"KEY_SEMICOLON",
"KEY_LESS_THAN",
"KEY_EQUAL",
"KEY_GREATER_THAN",
"KEY_QUESTION",
"KEY_AT",
"KEY_CAPITAL_A",
"KEY_CAPITAL_B",
"KEY_CAPITAL_C",
"KEY_CAPITAL_D",
"KEY_CAPITAL_E",
"KEY_CAPITAL_F",
"KEY_CAPITAL_G",
"KEY_CAPITAL_H",
"KEY_CAPITAL_I",
"KEY_CAPITAL_J",
"KEY_CAPITAL_K",
"KEY_CAPITAL_L",
"KEY_CAPITAL_M",
"KEY_CAPITAL_N",
"KEY_CAPITAL_O",
"KEY_CAPITAL_P",
"KEY_CAPITAL_Q",
"KEY_CAPITAL_R",
"KEY_CAPITAL_S",
"KEY_CAPITAL_T",
"KEY_CAPITAL_U",
"KEY_CAPITAL_V",
"KEY_CAPITAL_W",
"KEY_CAPITAL_X",
"KEY_CAPITAL_Y",
"KEY_CAPITAL_Z",
"KEY_LEFT_BRACKET",
"KEY_BACK_SLASH",
"KEY_RIGHT_BRACKET",
"KEY_UNDERSCORE",
"KEY_GRAVE",
"KEY_A",
"KEY_B",
"KEY_C",
"KEY_D",
"KEY_E",
"KEY_F",
"KEY_G",
"KEY_H",
"KEY_I",
"KEY_J",
"KEY_K",
"KEY_L",
"KEY_M",
"KEY_N",
"KEY_O",
"KEY_P",
"KEY_Q",
"KEY_R",
"KEY_S",
"KEY_T",
"KEY_U",
"KEY_V",
"KEY_W",
"KEY_X",
"KEY_Y",
"KEY_Z",
"KEY_LEFT_BRACE",
"KEY_BAR",
"KEY_RIGHT_BRACE",
"KEY_TILDE",
"KEY_EURO",
"KEY_POUND",
"KEY_YEN",
"KEY_MIDDLE_DOT",
"KEY_SEARCH",
"KEY_DPAD_LEFT",
"KEY_DPAD_RIGHT",
"KEY_DPAD_UP",
"KEY_DPAD_DOWN",
"KEY_DPAD_CENTER",
"KEY_ENTER",
"KEY_PLAY",
"KEY_KP_PERIOD"////���С���̵�Ӣ�ľ仰
};


static const char* GetKeyName(EventKeyboard::KeyCode KeyCode)
{
	return KeyStr[(int)KeyCode];
}

OG_END


#endif