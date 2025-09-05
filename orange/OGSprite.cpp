#include "OGSprite.h"
#include "Texture2D.h"
#include "OGDirector.h"
#include "OGTypes.h"

#include "OGSpriteFrame.h"

OG_BEGIN

Sprite::Sprite(): _rect(Rect::ZERO), _offsetPosition(Vec2::ZERO)
{
	bfunc = BlendFunc::DISABLE;
	_texture = nullptr;
}


Sprite::~Sprite()
{
	free();
	//_filename.clear();
//	OG_SAFE_RELEASE(_spriteFrame);
}

void Sprite::free()
{
	if (_texture != nullptr)
	{
		//_director->getTextureCache()->removeTexture(_texture);
		_texture = nullptr;
	}

}

Sprite* Sprite::create(const std::string &filename,const Rect & rect)
{
	assert(filename.empty() != true);

	Sprite *sprite = new (std::nothrow) Sprite();
	if (sprite && sprite->initWithFile(filename, rect))
	{
		sprite->autorelease();
		return sprite;
	}
	OG_SAFE_DELETE(sprite);
	return nullptr;

}

bool Sprite::initWithFile(const std::string &filename, const Rect & rect)
{

	_filename = filename;

	//std::shared_ptr<Texture2D> texture = _director->getTextureCache()->addImage(filename);

	auto texture = std::make_shared<Texture2D>();
	texture->loadFrom(filename.c_str());


	if (texture != nullptr )
	{
		if (rect.equals(Rect::ZERO))
		{
			_rect.size = texture->getContentSize();

		}
		else
		{
			_rect = rect;
		}


		if (_texture != texture)
		{
			free();
			_texture = texture;
			//添加渲染模式
			_texture->setBlendMode((SDL_BlendMode)bfunc);
		}


		setTextureRect(_rect);
		return true;
	}
	
	return false;
}

void Sprite::setTexture(const std::string &filename, const Rect & rect)
{
	this->initWithFile(filename, rect);
}
void Sprite::setTexture(std::shared_ptr<Texture2D> &texture)
{
	//this->initWithFile(filename, rect);
}

void Sprite::setTextureRect(const Rect& rect)
{
//	_rectRotated = rotated;
	Node::setPosition(Vec2::ZERO);
	Node::setContentSize(rect.size);
	Node::setPointer(_rect.size / 2);
	_rect = rect;
//	Node::_render_position = Node::_position;
	Node::setContentSize(getContentSize() * OG_CONTENT_SCALE_FACTOR());
	
// 	updateStretchFactor();
// 	updatePoly();
}

void Sprite::setSpriteFrame(SpriteFrame *spriteFrame)
{
	// retain the sprite frame
// 	if (_spriteFrame != spriteFrame)
// 	{
// 		OG_SAFE_RELEASE(_spriteFrame);
// 		_spriteFrame = spriteFrame;
//  
// 	}
	if (_filename != spriteFrame->getFileName())
	{
		_filename = spriteFrame->getFileName();
	}
//	_unflippedOffsetPositionFromCenter = spriteFrame->getOffset();

	std::shared_ptr<Texture2D> texture = spriteFrame->getTexture();
	// update texture before updating texture rect
	if (texture.get() != _texture.get())
	{
		if (_texture == nullptr)
		{
			initWithFile(_filename);
		}
		else
		{
			_texture = texture;
			//添加渲染模式
			_texture->setBlendMode((SDL_BlendMode)bfunc);
		}
	}

	// update rect
 
	setTextureRect(spriteFrame->getRect() );

}

Sprite* Sprite::createWithTexture(std::weak_ptr<Texture2D> texture, const Rect& rect, const Vec2& rotated)
{
	Sprite *sprite = new (std::nothrow) Sprite();
	if (sprite && sprite->initWithTexture(texture, rect, rotated))
	{
		sprite->autorelease();
		return sprite;
	}
	OG_SAFE_DELETE(sprite);
	return nullptr;
}
bool Sprite::initWithTexture(std::weak_ptr<Texture2D> texture, const Rect& rect, const Vec2& rotated)
{
	bool result = false;
	if (Node::init())
	{

		//	setDirty(false);
		
		_rotate = rotated;
		_flipX = _flipY = false;

		// default transform anchor: center
	//	setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		// zwoptex default values
// 		_offsetPosition.setZero();
// 
// 		// clean the Quad
// 		memset(&_quad, 0, sizeof(_quad));
// 
// 		// Atlas: Color
// 		_quad.bl.colors = Color4B::WHITE;
// 		_quad.br.colors = Color4B::WHITE;
// 		_quad.tl.colors = Color4B::WHITE;
// 		_quad.tr.colors = Color4B::WHITE;

		// update texture (calls updateBlendFunc)
		setTexture(texture,rect);
		_rect = { 0,0,rect.size.width,rect.size.height };
		_contentSize = rect.size;
		//添加渲染模式
		_texture->setBlendMode((SDL_BlendMode)bfunc);
	//	setTextureRect(rect, rotated, rect.size);

		// by default use "Self Render".
		// if the sprite is added to a batchnode, then it will automatically switch to "batchnode Render"
	//	setBatchNode(nullptr);
		result = true;
	}

// 	_recursiveDirty = true;
// 	setDirty(true);

	return result;
}

void Sprite::setTexture(std::weak_ptr<Texture2D> texture,  const Rect& rect) {
	_texture = std::make_shared<Texture2D>();
	_texture->createTexture(texture, rect);
	//添加渲染模式
	_texture->setBlendMode((SDL_BlendMode)bfunc);
}


// void Sprite::SetTextureRGBA() {
// 	if (_modiflyColor)
// 	{
// 		_texture->setRGBA(_realColor);
// 		_modiflyColor = false;
// 	}
// }
void Sprite::draw()
{
//	SetTextureRGBA();

	Rect dest_render_position = { _render_position  ,getContentSize() };


	_texture->render( _rect, dest_render_position, _rotate,
		_pointer, _displayedColor,_flipX, _flipY);
}

OG_END
