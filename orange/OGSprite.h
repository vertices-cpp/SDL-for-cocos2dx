#ifndef _SPRITE_H_
#define _SPRITE_H_


#include "OGNode.h"


OG_BEGIN

class Texture2D;
class SpriteFrame;

class Sprite :public Node
{
	BlendFunc bfunc;
	std::string _filename;
	std::shared_ptr<Texture2D> _texture = nullptr;
	Vec2 _offsetPosition;
	Rect _rect;
	//SpriteFrame *_spriteFrame = nullptr;
public:
	
	Sprite();
	~Sprite();

	void free();

	static Sprite* create(const std::string &filename, const Rect & rect = Rect::ZERO);

	bool initWithFile(const std::string & filename, const Rect & rect = Rect::ZERO);

	void setTexture(const std::string & filename, const Rect & rect = Rect::ZERO);

	void setTexture(std::shared_ptr<Texture2D>& texture);

	void setTextureRect(const Rect & rect);

	void setSpriteFrame(SpriteFrame * spriteFrame);
	
	void  setPosition(const Vec2& pos)
	{
		
		Node::setPosition(pos);
		 
		_contentChange = true;
	}

	void  setPosition(float x, float y)
	{
		Vec2 offset = { x - _position.x , y - _position.y };
		setOffset(offset);

		Node::setPosition(x, y);
	
		_contentChange = true;
	}


	static Sprite * createWithTexture(std::weak_ptr<Texture2D> texture, const Rect & rect, const Vec2& rotated=Vec2::ZERO);
	virtual bool initWithTexture(std::weak_ptr<Texture2D> texture, const Rect & rect, const Vec2& rotated);
	void setTexture(std::weak_ptr<Texture2D> texture, const Rect& rect);

	

	void SetTextureRGBA();

	
	void draw();
	
private:
	friend class JsonLayer;
	void  setOffset(const Vec2& pos)
	{
		_offsetPosition += pos;
	}
	Vec2  getOffset()const
	{
		return _offsetPosition;
	}

	void  setPositionNotOffset(const Vec2 &pos)
	{
		//Node::setPosition(x, y);
		_position = pos;
		_contentChange = true;
	}
	Sprite(const Sprite &) = delete; 
	Sprite &operator =(const Sprite &) = delete;
};

OG_END

#endif

