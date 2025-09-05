#ifndef _TEXTURE_2D_H_
#define _TEXTURE_2D_H_

//#include "PlatformMacros.h"
#include "SDLView.h"
#include "OGGeometry.h"
#include "OGTypes.h"
#include "OGMathBase.h"
#include <algorithm>

OG_BEGIN

template<class _Ty = float>  
	void og_swap(_Ty& _Left, _Ty& _Right)
{	 
	_Ty _Tmp =  _Left ;
	_Left =  _Right ;
	_Right =  _Tmp;
}
 
struct  Quad {
	SDL_Vertex m[4];
	const int index[6] = { 0,1,2,1,2,3 };

public:
	Quad() {}
	//包含纹理，原图大小，当前源/裁剪大小，将渲染目标大小，缩放值，旋转，旋转参照点，颜色，翻转
	void TextureCvRenderer(SDL_Texture* texture,const  Size &contentSize,
		const  Rect &clip, const  Rect& dest, const  Vec2 &Scale, const  Vec2& rotate, const  Vec2 &pointer, const Color4B& color, float flipX, float flipY)
	{
		auto len = sizeof(m);
		memset(m, 0,len);
		//裁剪位置
		float uvX = clip.origin.x / contentSize.width;
		float uvY = clip.origin.y / contentSize.height;
		float uvW = (clip.origin.x + clip.size.width) / contentSize.width;
		float uvH = (clip.origin.y + clip.size.height) / contentSize.height;
 //----------------------
 //|			20           |
 //|20|------------- |20 |
 //|  |              |   |
 //|  |              |   |
 //|	   ---------------   |
 //|          20         |
 // ----------------------

		m[0] = {//左上
			{ dest.origin.x,dest.origin.y }
			,{color.r,color.g,color.b,color.a}
			,{uvX,uvY}
		};
		m[1] = {//左下
				{dest.origin.x ,dest.origin.y + dest.size.height}
				,{color.r,color.g,color.b,color.a}
				,{uvX,uvH}
		};
		m[2] = {//右上
			{dest.origin.x + dest.size.width ,dest.origin.y}
			,{color.r,color.g,color.b,color.a}
			,{uvW,uvY}
		};
		m[3] = {//右下
			{dest.origin.x + dest.size.width ,dest.origin.y + dest.size.height}
			,{color.r,color.g,color.b,color.a}
			,{uvW,uvH}
		};
		//翻转
		if (flipX)
		{
			og_swap(m[0].tex_coord.x, m[2].tex_coord.x);
 			og_swap(m[0].tex_coord.y, m[2].tex_coord.y);
 			og_swap(m[1].tex_coord.x, m[3].tex_coord.x);
			og_swap(m[1].tex_coord.y, m[3].tex_coord.y);
		}
		if (flipY)
		{
			og_swap(m[0].tex_coord.x, m[1].tex_coord.x);
			og_swap(m[0].tex_coord.y, m[1].tex_coord.y);
			og_swap(m[2].tex_coord.x, m[3].tex_coord.x);
			og_swap(m[2].tex_coord.y, m[3].tex_coord.y);
		}


		//计算旋转
		// 0.01745329252f = 3.14/180
		float radiansX = -(rotate.x * 0.01745329252f);
		float radiansY = -(rotate.y * 0.01745329252f);
		float rotate_cosX = cosf(radiansX);
		float rotate_sinX = sinf(radiansX);
		float rotate_cosY = cosf(radiansY);
		float rotate_sinY = sinf(radiansY);

		//x = x * cos(θ) - y * sin(θ);
		// y = x * sin(θ) + y * cos(θ);

  		m[0].position = {
  					pointer.x + (m[0].position.x - pointer.x) * rotate_cosY - (m[0].position.y - pointer.y) * rotate_sinX,
  					pointer.y + (m[0].position.x - pointer.x) * rotate_sinY + (m[0].position.y - pointer.y) * rotate_cosX
  		};
  
  		m[1].position = {
  					pointer.x + (m[1].position.x - pointer.x) * rotate_cosY - (m[1].position.y - pointer.y) * rotate_sinX,
  					pointer.y + (m[1].position.x - pointer.x) * rotate_sinY + (m[1].position.y - pointer.y) * rotate_cosX
  		};
  		m[2].position = {
  					pointer.x + (m[2].position.x - pointer.x) * rotate_cosY - (m[2].position.y - pointer.y) * rotate_sinX,
  					pointer.y + (m[2].position.x - pointer.x) * rotate_sinY + (m[2].position.y - pointer.y) * rotate_cosX
  		};
  		m[3].position = {
  					pointer.x + (m[3].position.x - pointer.x) * rotate_cosY - (m[3].position.y - pointer.y) * rotate_sinX,
  					pointer.y + (m[3].position.x - pointer.x) * rotate_sinY + (m[3].position.y - pointer.y) * rotate_cosX
  		};
  
  		//设置缩放
//   		m[0].position = {
//   					m[0].position.x * Scale.x + dest.origin.x * (Scale.x) ,
//   				m[0].position.y * Scale.y + dest.origin.y * (Scale.y)
//   		};
//   
//   		m[1].position = {
//   					m[1].position.x * Scale.x + dest.origin.x * (Scale.x) ,
//   				m[1].position.y * Scale.y + dest.origin.y * (Scale.y)
//   		};
//   		m[2].position = {
//   					m[2].position.x * Scale.x + dest.origin.x * (Scale.x) ,
//   				m[2].position.y * Scale.y + dest.origin.y * (Scale.y)
//   		};
//   		m[3].position = {
//   					m[3].position.x * Scale.x + dest.origin.x * (Scale.x) ,
//   				m[3].position.y * Scale.y + dest.origin.y * (Scale.y)
//   		};

// 		m[0].position = {
// 					m[0].position.x   + dest.origin.x   ,
// 				m[0].position.y   + dest.origin.y  
// 		};
// 
// 		m[1].position = {
// 					m[1].position.x   + dest.origin.x   ,
// 				m[1].position.y   + dest.origin.y 
// 		};
// 		m[2].position = {
// 					m[2].position.x   + dest.origin.x  ,
// 				m[2].position.y   + dest.origin.y  
// 		};
// 		m[3].position = {
// 					m[3].position.x   + dest.origin.x  ,
// 				m[3].position.y   + dest.origin.y 
// 		};

		SDL_RenderGeometry(SDLView::getInstance()->getRender(), texture, m, 4, index, 6);
	}

};

static Quad quad;

class Texture2D
{
	SDL_Texture *mTexture;
	Size _contentSize;
	int mPitch;
	void* mPixels;
public:
	Texture2D();
	~Texture2D();
	void free();
	void setRGBA(Color4B color);
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void SetTextureAlphaMod(Uint8 a);
	void setBlendMode(SDL_BlendMode blending = SDL_BLENDMODE_BLEND);

	void loadFont(TTF_Font * ttf, const char * text, Color4B color);

	SDL_Texture* getTexture() { return mTexture; }
	Size getContentSize() { return _contentSize; }
	void loadFrom(const char *fileName);
	void loadMemData(unsigned char * data,int len);

	void createTexture(std::weak_ptr<Texture2D> texture,const Rect &r);

// 	void render(int x, int y, SDL_Rect * clip = nullptr, double degress =0.0, SDL_Point * pointer=nullptr, SDL_RendererFlip flip=SDL_FLIP_NONE);
// 
// 	void render(SDL_Rect * dest, SDL_Rect * clip =nullptr, double degress = 0.0, SDL_Point * pointer =nullptr, SDL_RendererFlip flip =SDL_FLIP_NONE);
// 
// 	void render(const Rect & clip, const Rect & dest, double degress, const Vec2 & pointer, bool flipX, bool flipY);

	void render(const Rect & clip, const Rect & dest, Vec2 rotate, const Vec2 & pointer, const Color4B &color, bool flipX, bool flipY );

private:
	OG_DISALLOW_COPY_AND_ASSIGN(Texture2D);
};


OG_END

#endif