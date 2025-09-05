#include "OGSpriteFrame.h"
#include "OGDirector.h"



OG_BEGIN
//--------------------------------------SpriteFrame------------------------------------

SpriteFrame* SpriteFrame::create(const std::string& filename, const Rect& rect)
{
	SpriteFrame *spriteFrame = new (std::nothrow) SpriteFrame();
	spriteFrame->initWithTextureFilename(filename, rect);
	spriteFrame->autorelease();

	return spriteFrame;
}


bool SpriteFrame::initWithTextureFilename(const std::string& filename, const Rect& rect/*, bool rotated, const Vec2& offset, const Size& originalSize*/)
{
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();// Director::getInstance()->getTextureCache()->addImage(filename);
	texture->loadFrom(filename.c_str());

	//if (FileUtils::getInstance()->isFileExist(filename)) 
	if (texture != nullptr)
	{
		_texture = nullptr;
		_textureFilename = filename;

		if (_rect.equals(Rect::ZERO))
			_rect.size = texture->getContentSize();
		else
			_rect = rect;

		setTexture(texture);

		return true;
	}
	return false;
}

SpriteFrame* SpriteFrame::createWithTexture(std::shared_ptr<Texture2D> texture, const Rect& rect)
{
	SpriteFrame *spriteFrame = new (std::nothrow) SpriteFrame();
	spriteFrame->initWithTexture(texture, rect);

	return spriteFrame;
}


bool SpriteFrame::initWithTexture(std::shared_ptr<Texture2D> texture, const Rect& rect )
{
	_texture = texture;
 
	_rect = rect;

	return true;
}


SpriteFrame* SpriteFrame::clone() const
{
	// no copy constructor	
	SpriteFrame *copy = new (std::nothrow) SpriteFrame();
	copy->initWithTexture(_texture, _rect/*, _rotated, _offsetInPixels, _originalSizeInPixels*/);
 
	return copy;
}

void SpriteFrame::setRect(const Rect& rect)
{
	_rect = rect;
	 
}
 

void SpriteFrame::setTexture(std::shared_ptr<Texture2D> texture)
{
	if (_texture != texture && _texture.get()!= texture.get()) {
		OG_SAFE_NULL(_texture);
		//   OG_SAFE_RETAIN(texture);
		_texture = texture;
	}
}

std::shared_ptr<Texture2D> SpriteFrame::getTexture()
{
	if (_texture && _texture.get()) {
		return _texture;
	}


	if (!_textureFilename.empty()) {

		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->loadFrom(_textureFilename.c_str());

		//return Director::getInstance()->getTextureCache()->addImage(_textureFilename);
	}
	// no texture or texture filename
	return nullptr;
}
 

 //--------------------------------------AnimationFrame------------------------------------


AnimationFrame* AnimationFrame::create(SpriteFrame* spriteFrame, float delayUnits, const ValueMap& userInfo)
{
	auto ret = new (std::nothrow) AnimationFrame();
	if (ret && ret->initWithSpriteFrame(spriteFrame, delayUnits, userInfo))
	{
		ret->autorelease();
	}
	else
	{
		OG_SAFE_DELETE(ret);
	}
	return ret;
}

AnimationFrame::AnimationFrame()
	: _spriteFrame(nullptr)
	, _delayUnits(0.0f)
{

}

bool AnimationFrame::initWithSpriteFrame(SpriteFrame* spriteFrame, float delayUnits, const ValueMap& userInfo)
{
	setSpriteFrame(spriteFrame);
	setDelayUnits(delayUnits);
	setUserInfo(userInfo);

	return true;
}

AnimationFrame::~AnimationFrame()
{
 
	OG_SAFE_RELEASE(_spriteFrame);
}

AnimationFrame* AnimationFrame::clone() const
{
	// no copy constructor
	auto frame = new (std::nothrow) AnimationFrame();
	frame->initWithSpriteFrame(_spriteFrame->clone(),
		_delayUnits,
		_userInfo);

	frame->autorelease();
	return frame;
}
//--------------------------------------Animation------------------------------------
// implementation of Animation

Animation* Animation::create()
{
	Animation *animation = new (std::nothrow) Animation();
	animation->init();
	animation->autorelease();

	return animation;
}

Animation* Animation::createWithSpriteFrames(const Vector<SpriteFrame*>& frames, float delay/* = 0.0f*/, unsigned int loops/* = 1*/)
{
	Animation *animation = new (std::nothrow) Animation();
	animation->initWithSpriteFrames(frames, delay, loops);
	animation->autorelease();

	return animation;
}

Animation* Animation::create(const Vector<AnimationFrame*>& arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops /* = 1 */)
{
	Animation *animation = new (std::nothrow) Animation();
	animation->initWithAnimationFrames(arrayOfAnimationFrameNames, delayPerUnit, loops);
	animation->autorelease();

	return animation;
}

bool Animation::init()
{
	_loops = 1;
	_delayPerUnit = 0.0f;

	return true;
}

bool Animation::initWithSpriteFrames(const Vector<SpriteFrame*>& frames, float delay/* = 0.0f*/, unsigned int loops/* = 1*/)
{
	_delayPerUnit = delay;
	_loops = loops;

	for (auto& spriteFrame : frames)
	{
		auto animFrame = AnimationFrame::create(spriteFrame, 1, ValueMap());
		_frames.pushBack(animFrame);
		_totalDelayUnits++;
	}

	return true;
}

bool Animation::initWithAnimationFrames(const Vector<AnimationFrame*>& arrayOfAnimationFrames, float delayPerUnit, unsigned int loops)
{
	_delayPerUnit = delayPerUnit;
	_loops = loops;

	setFrames(arrayOfAnimationFrames);

	for (auto& animFrame : _frames)
	{
		_totalDelayUnits += animFrame->getDelayUnits();
	}
	return true;
}

Animation::Animation()
	: _totalDelayUnits(0.0f)
	, _delayPerUnit(0.0f)
	, _duration(0.0f)
	, _restoreOriginalFrame(false)
	, _loops(0)
{

}

Animation::~Animation()
{

}

void Animation::addSpriteFrame(SpriteFrame* spriteFrame,float delay)
{
	AnimationFrame *animFrame = AnimationFrame::create(spriteFrame, 1.0f, ValueMap());
	_frames.pushBack(animFrame);

	// update duration
	_totalDelayUnits++;

	if (!_delayPerUnit)
		_delayPerUnit = delay;
}

void Animation::addFrame(const std::string &filename,const Rect &rect, float delay)
{
	AnimationFrame *animFrame = AnimationFrame::create(SpriteFrame::create(filename,rect), 1.0f, ValueMap());
	_frames.pushBack(animFrame);

	// update duration
	_totalDelayUnits++;

	if (!_delayPerUnit)
		_delayPerUnit = delay;
}

void Animation::addFrame(const std::string &filename, float delay)
{
	addFrame(filename, Rect::ZERO, delay);
}

void Animation::addAnimationFrame(AnimationFrame* animFrame)
{
//	AnimationFrame *animFrame = AnimationFrame::create(spriteFrame, 1.0f, ValueMap());
	assert(animFrame != nullptr);

	_frames.pushBack(animFrame);

	// update duration
	_totalDelayUnits++;
}

void Animation::addSpriteFrameWithFile(const std::string& filename)
{
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();// Director::getInstance()->getTextureCache()->addImage(filename);
	texture->loadFrom(filename.c_str());

	Rect rect = Rect::ZERO;
	rect.size = texture->getContentSize();
	SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
	addSpriteFrame(frame);
}

void Animation::addSpriteFrameWithTexture(std::shared_ptr<Texture2D> texture, const Rect& rect)
{
	SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
	addSpriteFrame(frame);
}

float Animation::getDuration() const
{
	return _totalDelayUnits * _delayPerUnit;
}

Animation* Animation::clone() const
{
	// no copy constructor    
	auto a = new (std::nothrow) Animation();
	a->initWithAnimationFrames(_frames, _delayPerUnit, _loops);
	a->setRestoreOriginalFrame(_restoreOriginalFrame);
 
	return a;
}




OG_END
