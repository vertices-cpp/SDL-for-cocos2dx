#include "Texture2D.h"
#include "SDLView.h"

//#include "Texture2DManage.h"

//#define TRANSPRENT_BLACK

OG_BEGIN

Texture2D::Texture2D() :mTexture(nullptr), _contentSize(Size::ZERO), mPitch(0), mPixels(nullptr)
{
}


Texture2D::~Texture2D()
{
	free();
}

void Texture2D::free()
{
	if (mTexture != nullptr)
		SDL_DestroyTexture(mTexture);
}
void Texture2D::setRGBA(Color4B color)
{
	//调制纹理rgb
	SDL_SetTextureColorMod(mTexture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(mTexture, color.a);
}

void Texture2D::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//调制纹理rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}
void Texture2D::SetTextureAlphaMod(Uint8 a) {
	SDL_SetTextureAlphaMod(mTexture, a);
}

void Texture2D::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture,blending);
}


void Texture2D::loadFont(TTF_Font* ttf,const char *text,Color4B color)
{
	free();

	SDL_Color r = { color.r,color.g,color.b,color.a };

	SDL_Surface *load = TTF_RenderUTF8_Blended(ttf,text,r);

	assert(load != NULL);

	SDL_Surface *surfaceFormat = SDL_ConvertSurfaceFormat(load, SDL_PIXELFORMAT_RGBA8888, 0);

	mTexture = SDL_CreateTexture(SDLView::getInstance()->getRender(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		surfaceFormat->w, surfaceFormat->h);

	SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

	SDL_LockTexture(mTexture, &surfaceFormat->clip_rect, &mPixels, &mPitch);

	memcpy(mPixels, surfaceFormat->pixels, mPitch * surfaceFormat->h);

	_contentSize = Size((float)surfaceFormat->w, (float)surfaceFormat->h);

#ifdef TRANSPRENT_BLACK
	Uint32 *pixels = (Uint32*)mPixels;
	Uint32 colorKey = SDL_MapRGB(surfaceFormat->format, 0, 0, 0);
	Uint32 transParent = SDL_MapRGBA(surfaceFormat->format, 0, 0, 0, 0);

	Uint32 pixelCount = mPitch / 4 * surfaceFormat->h;

	for (Uint32 i = 0; i < pixelCount; ++i)
	{
		if (pixels[i] == colorKey)
			pixels[i] = transParent;
	}

#endif

	SDL_UnlockTexture(mTexture);

	SDL_FreeSurface(surfaceFormat);
	SDL_FreeSurface(load);
}


void Texture2D::loadFrom(const char * fileName)
{
	free();

	SDL_Surface *load = IMG_Load(fileName);

	assert(load != NULL);

	SDL_Surface *surfaceFormat = SDL_ConvertSurfaceFormat(load, SDL_PIXELFORMAT_RGBA8888, 0);

	mTexture = SDL_CreateTexture(SDLView::getInstance()->getRender(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		surfaceFormat->w, surfaceFormat->h);

	SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

	SDL_LockTexture(mTexture, &surfaceFormat->clip_rect, &mPixels, &mPitch);

	memcpy(mPixels, surfaceFormat->pixels, mPitch * surfaceFormat->h);

	_contentSize = Size((float)surfaceFormat->w, (float)surfaceFormat->h);

#ifdef TRANSPRENT_BLACK
	Uint32 *pixels = (Uint32*)mPixels;
	Uint32 colorKey = SDL_MapRGB(surfaceFormat->format, 0, 0, 0);
	Uint32 transParent = SDL_MapRGBA(surfaceFormat->format, 0, 0, 0, 0);

	Uint32 pixelCount = mPitch / 4 * surfaceFormat->h;

	for (Uint32 i = 0; i < pixelCount; ++i)
	{
		if (pixels[i] == colorKey)
			pixels[i] = transParent;
	}

#endif

	SDL_UnlockTexture(mTexture);

	SDL_FreeSurface(surfaceFormat);
	SDL_FreeSurface(load);
}

void Texture2D::loadMemData(unsigned char * data, int len)
{
	free();

	SDL_RWops *memp = SDL_RWFromMem((void*)data, len);
	SDL_Surface *load = IMG_Load_RW(memp,0);

	assert(load != NULL);

	SDL_Surface *surfaceFormat = SDL_ConvertSurfaceFormat(load, SDL_PIXELFORMAT_RGBA8888, 0);

	mTexture = SDL_CreateTexture(SDLView::getInstance()->getRender(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		surfaceFormat->w, surfaceFormat->h);

	SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

	SDL_LockTexture(mTexture, &surfaceFormat->clip_rect, &mPixels, &mPitch);

	memcpy(mPixels, surfaceFormat->pixels, mPitch * surfaceFormat->h);

	_contentSize = Size((float)surfaceFormat->w, (float)surfaceFormat->h );

#ifdef TRANSPRENT_BLACK
	Uint32 *pixels = (Uint32*)mPixels;
	Uint32 colorKey = SDL_MapRGB(surfaceFormat->format, 0, 0, 0);
	Uint32 transParent = SDL_MapRGBA(surfaceFormat->format, 0, 0, 0, 0);

	Uint32 pixelCount = mPitch / 4 * surfaceFormat->h;

	for (Uint32 i = 0; i < pixelCount; ++i)
	{
		if (pixels[i] == colorKey)
			pixels[i] = transParent;
	}

#endif

	SDL_UnlockTexture(mTexture);

	SDL_FreeSurface(surfaceFormat);
	SDL_FreeSurface(load);
}

void Texture2D::createTexture(std::weak_ptr<Texture2D> texture, const Rect & rect)
{
	auto getTexture = texture.lock();

	float w = rect.size.width, h = rect.size.height;

	SDL_Texture *tex = SDL_GetRenderTarget(SDLView::getInstance()->getRender());

	mTexture = SDL_CreateTexture(SDLView::getInstance()->getRender(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)w, (int)h);
	SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(SDLView::getInstance()->getRender(), mTexture);
	//创建目标纹理
	Rect dest = { 0,0,w,h };
 	getTexture->render(rect, dest, Vec2::ZERO,
 		rect.size/2, Color4B( 255,255,255,255 ), false, false);
// 	SDL_Rect rrr = { 0,0,w,h };
// 	SDL_Rect ddd = { 0,0,w,h };
//	SDL_RenderCopyEx(SDLView::getInstance()->getRender(), getTexture.get()->getTexture(),  &rrr, &ddd, 0, 0,SDL_FLIP_NONE);
	_contentSize.width = w;
	_contentSize.height = h;
	

	SDL_SetRenderTarget(SDLView::getInstance()->getRender(), NULL);
}

// void Texture2D::render(int x, int y,SDL_Rect *clip,double degress ,SDL_Point *pointer,SDL_RendererFlip flip)
// {
// 	SDL_Rect renderQuad = { x,y,(int)_contentSize.width,(int)_contentSize.height };
// 	if (clip != nullptr)
// 	{
// 		renderQuad.w = clip->w;
// 		renderQuad.h = clip->h;
// 	}
// 
// 	SDL_RenderCopyEx(SDLView::getInstance()->getRender(), mTexture, clip, &renderQuad, degress, pointer, flip);
// }
// 
// void Texture2D::render(SDL_Rect *dest, SDL_Rect *clip, double degress,SDL_Point *pointer,SDL_RendererFlip flip)
// {
// 
// 	SDL_RenderCopyEx(SDLView::getInstance()->getRender(), mTexture, clip, dest, degress, pointer, flip);
// }
// 
// 
// void Texture2D::render(const Rect& clip, const Rect &dest, double degress, const Vec2 &pointer, bool flipX, bool flipY)
// {
// 	
// 		SDL_RendererFlip  flip = (SDL_RendererFlip)((flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (flipY ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));
// 
// 
// 		SDL_Rect src = { (int)clip.origin.x,
// 			(int)clip.origin.y,
// 			(int)clip.size.width,
// 			(int)clip.size.height };
// 
// 		SDL_Rect dst = { (int)dest.origin.x,
// 			(int)dest.origin.y,
// 			(int)dest.size.width,
// 			(int)dest.size.height };
// 
// 		//it->second->setBlendMode(SDL_BLENDMODE_INVALID);
// 		SDL_Point point = { (int)pointer.x,(int)pointer.y };
// 		SDL_RenderCopyEx(SDLView::getInstance()->getRender(), mTexture, nullptr, &dst, degress, &point, flip);
// 	
// }


void Texture2D::render(const Rect& clip, const Rect &dest, Vec2 rotate, const Vec2 &pointer, const Color4B &color, bool flipX, bool flipY)
{
	quad.TextureCvRenderer(mTexture, _contentSize, clip, dest, SDLView::getInstance()->getScale(), rotate, pointer, color,flipX, flipY);
}

OG_END
