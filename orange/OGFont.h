#ifndef _OG_FONT_
#define _OG_FONT_

#include "Header.h"
#include "OGPlatformMacros.h"
#include "OGNode.h"
#include "OGDirector.h"
#include "OGTypes.h"
#include "Texture2D.h"
#include "OGNode.h"

OG_BEGIN


class TextMessage :public Node {

	TTF_Font* _font;
	std::shared_ptr<Texture2D> _texture;
	int _ttf_size;
	Rect _rect;
public:
	static TextMessage* createWithTTF(const char *ttf_name, const char *text, int ttf_size) {
		TextMessage *ret = new (std::nothrow) TextMessage();

		if (ret && ret->init(ttf_name, text, ttf_size))
		{
			ret->autorelease();
			return ret;
		}

		delete ret;
		return nullptr;
	}
	TextMessage():_font(nullptr), _texture(nullptr),_ttf_size(0),_rect(Rect::ZERO) {  }
	~TextMessage() { 
		TTF_CloseFont(_font);
		if (_texture)
			_texture = nullptr;
	}

	void setString(const char *text)
	{
		_texture->loadFont(_font, text, _realColor);
		_contentSize = _rect.size = _texture->getContentSize();
	}
// 	void SetTextureRGBA() {
// 		if (_modiflyColor)
// 		{
// 			_texture->setRGBA(_realColor);
// 			_modiflyColor = false;
// 		}
// 	}
	void draw() {
		//SetTextureRGBA();
// 		Rect dest_render_position = { _render_position * SDLView::getInstance()->getScale(),getContentSize() * SDLView::getInstance()->getScale() };
// 		_texture->render(_rect,dest_render_position,_degrees,_pointer,_flipX,_flipY);

		Rect dest_render_position = { _render_position  ,getContentSize() };
		_texture->render(_rect, dest_render_position, _rotate,
			_pointer, _displayedColor, _flipX, _flipY);
	}

private:
	bool init(const char *ttf_name, const char *text, int ttf_size)
	{
		_font = TTF_OpenFont(ttf_name, ttf_size);
		assert(_font != nullptr);
		if (_font == nullptr)
			return false;
		_ttf_size = ttf_size;
		
		Node::setColor(Color4B::WHITE);
		_texture = std::make_shared<Texture2D>();

		setString(text);
		return true;
	}
	
};


OG_END

#endif