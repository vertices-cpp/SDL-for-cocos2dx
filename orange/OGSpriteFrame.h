#ifndef _SPRITE_FRAME_H_
#define _SPRITE_FRAME_H_

#include "OGPlatformMacros.h"
#include "Vec2.h"
#include "Texture2D.h"
#include "OGValue.h"
#include "OGRef.h"
#include "OGVector.h"

//#include "Texture2D.h"

OG_BEGIN
//
//#define OG_RECT_PIXELS_TO_POINTS(__rect_in_pixels__)                                                                        \
//    SE::Rect( (__rect_in_pixels__).origin.x / OG_CONTENT_SCALE_FACTOR(), (__rect_in_pixels__).origin.y / OG_CONTENT_SCALE_FACTOR(),    \
//            (__rect_in_pixels__).size.width / OG_CONTENT_SCALE_FACTOR(), (__rect_in_pixels__).size.height / OG_CONTENT_SCALE_FACTOR() )
//#define OG_RECT_POINTS_TO_PIXELS(__rect_in_points_points__)                                                                        \
//    SE::Rect( (__rect_in_points_points__).origin.x * OG_CONTENT_SCALE_FACTOR(), (__rect_in_points_points__).origin.y * OG_CONTENT_SCALE_FACTOR(),    \
//            (__rect_in_points_points__).size.width * OG_CONTENT_SCALE_FACTOR(), (__rect_in_points_points__).size.height * OG_CONTENT_SCALE_FACTOR() )
//
//#define OG_POINT_PIXELS_TO_POINTS(__pixels__)                                                                        \
//SE::Vec2( (__pixels__).x / OG_CONTENT_SCALE_FACTOR(), (__pixels__).y / OG_CONTENT_SCALE_FACTOR())
//
//#define OG_POINT_POINTS_TO_PIXELS(__points__)                                                                        \
//SE::Vec2( (__points__).x * OG_CONTENT_SCALE_FACTOR(), (__points__).y * OG_CONTENT_SCALE_FACTOR())
//
//#define OG_SIZE_PIXELS_TO_POINTS(__size_in_pixels__)                                                                        \
//SE::Size( (__size_in_pixels__).width / OG_CONTENT_SCALE_FACTOR(), (__size_in_pixels__).height / OG_CONTENT_SCALE_FACTOR())


class Node;
//--------------------------------------SpriteFrame------------------------------------
class SpriteFrame  : public Ref
{
public:

 
	static SpriteFrame* create(const std::string& filename, const Rect& rect = Rect::ZERO);
	bool initWithTextureFilename(const std::string& filename, const Rect& rect);

	static SpriteFrame* createWithTexture(std::shared_ptr<Texture2D> texture, const Rect& rect);
	bool initWithTexture(std::shared_ptr<Texture2D> pobTexture, const Rect& rect);
  
	const Rect& getRect() const { return _rect; }
 
	void setRect(const Rect& rect);
 
	std::shared_ptr<Texture2D> getTexture();
 
	void setTexture(std::shared_ptr<Texture2D>  pobTexture);
  
	
	// Overrides
	virtual SpriteFrame *clone() const;
	std::string getFileName()const { return _textureFilename; }

	SpriteFrame():_texture(nullptr){}
	virtual ~SpriteFrame() { _texture=nullptr; }
  
 
protected:
 
	Rect _rect;
 
	std::shared_ptr<Texture2D> _texture;
	std::string  _textureFilename;
 
};

//--------------------------------------AnimationFrame------------------------------------

class  AnimationFrame : public Ref
{
public:
	/** @struct DisplayedEventInfo
	 * When the animation display,Dispatches the event of UserData.
	 */
	struct DisplayedEventInfo
	{
		Node* target;
		const ValueMap* userInfo;
	};
 
	static AnimationFrame* create(SpriteFrame* spriteFrame, float delayUnits, const ValueMap& userInfo);
	bool initWithSpriteFrame(SpriteFrame* spriteFrame, float delayUnits, const ValueMap& userInfo);

	SpriteFrame* getSpriteFrame() const { return _spriteFrame; };
	 
	void setSpriteFrame(SpriteFrame* frame)
	{
		OG_SAFE_RETAIN(frame);
 		OG_SAFE_DELETE(_spriteFrame);
		_spriteFrame = frame;
	}

	 
	float getDelayUnits() const {
		return _delayUnits;
	};

 
	void setDelayUnits(float delayUnits) { _delayUnits = delayUnits; };
 
 
	const ValueMap& getUserInfo() const { return _userInfo; };
	ValueMap& getUserInfo() { return _userInfo; };

	/** Sets user information.
	 * @param userInfo A dictionary as UserInfo.
	 */
	void setUserInfo(const ValueMap& userInfo)
	{
		_userInfo = userInfo;
	}
	// Overrides
	virtual AnimationFrame *clone() const /*override*/;
	 
	AnimationFrame();
 
	virtual ~AnimationFrame();
 

protected:

	/** SpriteFrameName to be used */
	SpriteFrame* _spriteFrame;

	/**  how many units of time the frame takes */
	float _delayUnits;
 
	ValueMap _userInfo;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(AnimationFrame);
};
//--------------------------------------Animation------------------------------------

class   Animation :public Ref
{
public:
	

	static Animation* create();

	static Animation* createWithSpriteFrames(const Vector<SpriteFrame*>& arrayOfSpriteFrameNames, float delay = 0.0f, unsigned int loops = 1);
 
	static Animation* create(const Vector<AnimationFrame*>& arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops = 1);
 

	void addSpriteFrame(SpriteFrame * spriteFrame, float delay = 0.2f);
	void addFrame(const std::string & filename, const Rect & rect = Rect::ZERO, float delay = 0.2f);
	void addFrame(const std::string & filename, float delay);
	void addAnimationFrame(AnimationFrame* animFrame);

	void addSpriteFrameWithFile(const std::string& filename);

	 
	void addSpriteFrameWithTexture(std::shared_ptr<Texture2D> pobTexture, const Rect& rect);
 
	float getTotalDelayUnits() const {
		return _totalDelayUnits;
	};

	 
	void setDelayPerUnit(float delayPerUnit) { _delayPerUnit = delayPerUnit; };
 
	float getDelayPerUnit() const { return _delayPerUnit; };
 
	float getDuration() const;
 
	const Vector<AnimationFrame*>& getFrames() const { return _frames; };
 
	void setFrames(const Vector<AnimationFrame*>& frames)
	{
		_frames = frames;
	}
 
	bool getRestoreOriginalFrame() const { return _restoreOriginalFrame; };
 
	void setRestoreOriginalFrame(bool restoreOriginalFrame) { _restoreOriginalFrame = restoreOriginalFrame; };
 
	unsigned int getLoops() const { return _loops; };
 
	void setLoops(unsigned int loops) { _loops = loops; };


	// overrides
	virtual Animation *clone() const/* override*/;
 
	Animation();
	virtual ~Animation();

	/** Initializes a Animation. */
	bool init();

	bool initWithSpriteFrames(const Vector<SpriteFrame*>& arrayOfSpriteFrameNames, float delay = 0.0f, unsigned int loops = 1);
	bool initWithAnimationFrames(const Vector<AnimationFrame*>& arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops);
protected:
	/** total Delay units of the Animation. */
	float _totalDelayUnits;

	/** Delay in seconds of the "delay unit". */
	float _delayPerUnit;

	/** duration in seconds of the whole animation. It is the result of totalDelayUnits * delayPerUnit. */
	float _duration;

	/** array of AnimationFrames. */
	Vector<AnimationFrame*> _frames;

	/** whether or not it shall restore the original frame when the animation finishes. */
	bool _restoreOriginalFrame;

	/** how many times the animation is going to loop. 0 means animation is not animated. 1, animation is executed one time, ... */
	unsigned int _loops;

private:
	Animation(const Animation &) = delete; 
		Animation &operator =(const Animation &) = delete;
};



OG_END


#endif