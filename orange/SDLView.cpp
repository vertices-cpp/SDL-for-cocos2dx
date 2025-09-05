#include "SDLView.h"

OG_BEGIN

SDLView::SDLView():_init_state(false),_scale(1.0f,1.0f)
{
	SDL_Init(SDL_INIT_VIDEO);

#if defined(_WIN32)||defined(_WIN64)

	SDL_SetHint(/*SDL_HINT_RENDER_SCALE_QUALITY |*/ SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

#elif defined(__ANDROID__)

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

#endif
	


}


SDLView::~SDLView()
{
	
	if (ren != nullptr)
		SDL_DestroyRenderer(ren);

	if (win != nullptr)
		SDL_DestroyWindow(win);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}

void SDLView::init(const char * title, float x, float y, float w, float h)
{
	_init_state = true;

	win = SDL_CreateWindow(title, (int)x, (int)y, (int)w, (int)h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
// 	int cx, cy;
// 	SDL_GetWindowPosition(win, &cx, &cy);

	_screenSize = Size(w,h);
	_renderLogicSize = _screenSize;
}


OG_END
