#ifndef _SDL_VIEW_H_
#define _SDL_VIEW_H_

#include "header.h"
//#include "PlatformMacros.h"
#include "Vec2.h"
#include "OGGeometry.h"

OG_BEGIN

class SDLView
{
	SDL_Window *win;
	SDL_Renderer *ren;
	bool _init_state;
	
	Size _screenSize, _renderLogicSize;
	Vec2 _scale;
public:
	SDLView();
	~SDLView();

	Instance(SDLView);

	bool get_init_state()const { return _init_state; }
	void init(const char *title ="SDL tutorial", float x = SDL_WINDOWPOS_UNDEFINED, float y = SDL_WINDOWPOS_UNDEFINED,
		float w = 640 , float h = 480);


	Vec2 getScale() { return _scale; }
	Vec2 setScale(Vec2 scale) { return _scale=scale; }
	Size getWindowSize() {
		return _screenSize;
	}
	Size getRenderSize() {
		return _renderLogicSize;
	}
	void SetWindowSize(Size win_size) {
		if (win_size == _screenSize)
			return;
		
		_screenSize = win_size;
		_scale = _screenSize / _renderLogicSize;
		
		SDL_SetWindowSize(win, (int)_screenSize.width, (int)_screenSize.height);
		//SDL_RenderSetLogicalSize(ren, (int)_renderLogicSize.width, (int)_renderLogicSize.height);
		refresh();
	}
	void SetRenderLogicSize(Size ren_size) {
		if (_renderLogicSize == ren_size)
			return;
	
		_renderLogicSize = ren_size;
		_scale = _screenSize / _renderLogicSize;

		SDL_SetWindowSize(win, (int)_screenSize.width, (int)_screenSize.height);
		//SDL_RenderSetLogicalSize(ren, (int)_renderLogicSize.width, (int)_renderLogicSize.height);
		refresh();
	}
	void upWindowSize() {
		int width =0, height = 0;
		SDL_GetWindowSize(win, &width, &height);

		_screenSize = Size((float)width,(float)height);
		_scale = _screenSize / _renderLogicSize;

		refresh();
	}

	SDL_Window* getWin()const { return win; }
	SDL_Renderer* getRender()const { return ren; }

	void clear() {
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);
	}
	void refresh() {

		//auto ret = SDL_GetPreferredLocales();

		SDL_RenderPresent(ren);
	}
};

OG_END

#endif