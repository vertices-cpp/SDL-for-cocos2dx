#ifndef _ACTION_H_
#define _ACTION_H_

#include "OGPlatformMacros.h"
#include "Vec2.h"

#include "ulist.h"
#include "OGRef.h"

#include "OGEventDispatcher.h"
#include "OGSpriteFrame.h"


OG_BEGIN

class Node;
class Animation;

//--------------------------------------Action------------------------------------
class Action:public Ref
{
public:
	/** Default tag used for all the actions. */
	static const int INVALID_TAG = -1;


	virtual Action* reverse() const
	{

		return nullptr;
	}

	/** Return true if the action has finished.
	 *
	 * @return Is true if the action has finished.
	 */
	virtual bool isDone() const { return true; }


	virtual void startWithTarget(Node *target) { _target = target; }

	virtual void stop() { _target = nullptr; }

	virtual Action* clone() const
	{
		return nullptr;
	}
	virtual void step(float dt) {}
	virtual void update(float time) {}

	Node* getTarget() const { return _target; }
	void setTarget(Node *target) { _target = target; }


	int getTag() const { return _tag; }
	/** Changes the tag that is used to identify the action easily.
	 *
	 * @param tag Used to identify the action easily.
	 */
	void setTag(int tag) { _tag = tag; }
	/** Returns a flag field that is used to group the actions easily.
	 *
	 * @return A tag.
	 */
	unsigned int getFlags() const { return _flags; }
	/** Changes the flag field that is used to group the actions easily.
	 *
	 * @param flags Used to group the actions easily.
	 */
	void setFlags(unsigned int flags) { _flags = flags; }

	Action() : _target(nullptr)
		, _tag(Action::INVALID_TAG)
		, _flags(0) {}
	virtual ~Action() {}

protected:
	
	Node    *_target;
	/** The action tag. An identifier of the action. */
	int     _tag;
	/** The action flag field. To categorize action into certain groups.*/
	unsigned int _flags;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(Action);
};




//--------------------------------------FiniteTimeAction------------------------------------

class FiniteTimeAction : public Action
{
public:
	/** Get duration in seconds of the action.
	 *
	 * @return The duration in seconds of the action.
	 */
	float getDuration() const { return _duration; }
	/** Set duration in seconds of the action.
	 *
	 * @param duration In seconds of the action.
	 */
	void setDuration(float duration) { _duration = duration; }

	//
	// Overrides
	//
	virtual FiniteTimeAction* reverse() const override
	{

		return nullptr;
	}
	virtual FiniteTimeAction* clone() const override
	{
		return nullptr;
	}
	FiniteTimeAction()
		: _duration(0)
	{}
	virtual ~FiniteTimeAction() {}

protected:
	//! Duration in seconds.
	float _duration;
private:
	OG_DISALLOW_COPY_AND_ASSIGN(FiniteTimeAction);
};
//--------------------------------------ActionInterval------------------------------------

class  ActionInterval : public FiniteTimeAction
{
public:
	
	float getElapsed() { return _elapsed; }

	/** initializes the action */
	bool initWithDuration(float d);

	virtual bool isDone() const override;

	virtual void step(float dt) override;
	virtual void startWithTarget(Node *target) override;
	virtual ActionInterval* reverse() const override
	{
		return nullptr;
	}
	virtual ActionInterval *clone() const override
	{
		return nullptr;
	}
protected:
	float _elapsed;
	bool _firstTick;
	bool _done;

};
//--------------------------------------ExtraAction------------------------------------
class ExtraAction : public FiniteTimeAction
{
public:
	static ExtraAction* create();
	virtual ExtraAction* clone() const;
	virtual ExtraAction* reverse() const;
	virtual void update(float time);
	virtual void step(float dt);
};


//--------------------------------------ActionInstant------------------------------------

class   ActionInstant : public FiniteTimeAction
{
public:
	//
	// Overrides
	//
	virtual ActionInstant* clone() const override
	{
	 
		return nullptr;
	}

	virtual ActionInstant * reverse() const override
	{
 
		return nullptr;
	}

	virtual void startWithTarget(Node *target) override;

	virtual bool isDone() const override;
	/**
	 * @param dt In seconds.
	 */
	virtual void step(float dt) override;
	/**
	 * @param time In seconds.
	 */
	virtual void update(float time) override;

private:
	bool _done;
};
//--------------------------------------Sequence------------------------------------

class  Sequence : public ActionInterval
{
public:
	 
	static Sequence* create(FiniteTimeAction *action1, ...) ;
 
	static Sequence* create(const Vector<FiniteTimeAction*>& arrayOfActions);
	 
	static Sequence* createWithVariableList(FiniteTimeAction *action1, va_list args);
 
	static Sequence* createWithTwoActions(FiniteTimeAction *actionOne, FiniteTimeAction *actionTwo);

	 
	virtual Sequence* clone() const override;
	virtual Sequence* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	virtual void stop() override;
	virtual bool isDone() const override;
	/**
	 * @param t In seconds.
	 */
	virtual void update(float t) override;
	 
	Sequence();
	virtual ~Sequence();

	/** initializes the action */
	bool initWithTwoActions(FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo);
	bool init(const Vector<FiniteTimeAction*>& arrayOfActions);

protected:
	FiniteTimeAction *_actions[2];
	float _split;
	int _last;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(Sequence);
};

//--------------------------------------Repeat------------------------------------

class Repeat : public ActionInterval
{
public:
	/** Creates a Repeat action. Times is an unsigned integer between 1 and pow(2,30).
	 *
	 * @param action The action needs to repeat.
	 * @param times The repeat times.
	 * @return An autoreleased Repeat object.
	 */
	static Repeat* create(FiniteTimeAction *action, unsigned int times);

	/** Sets the inner action.
	 *
	 * @param action The inner action.
	 */
	void setInnerAction(FiniteTimeAction *action)
	{
		if (_innerAction != action)
		{
			OG_SAFE_RETAIN(action);
			OG_SAFE_RELEASE(_innerAction);
			_innerAction = action;
		}
	}

	/** Gets the inner action.
	 *
	 * @return The inner action.
	 */
	FiniteTimeAction* getInnerAction()
	{
		return _innerAction;
	}

	//
	// Overrides
	//
	virtual Repeat* clone() const override;
	virtual Repeat* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	virtual void stop() override;
	/**
	 * @param dt In seconds.
	 */
	virtual void update(float dt) override;
	virtual bool isDone() const override;
	 
	Repeat() {}
	virtual ~Repeat();

	/** initializes a Repeat action. Times is an unsigned integer between 1 and pow(2,30) */
	bool initWithAction(FiniteTimeAction *pAction, unsigned int times);

protected:
	unsigned int _times;
	unsigned int _total;
	float _nextDt;
	bool _actionInstant;
	/** Inner action */
	FiniteTimeAction *_innerAction;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(Repeat);
};


//--------------------------------------RepeatForever------------------------------------
class   RepeatForever : public ActionInterval
{
public:
 
	static RepeatForever* create(ActionInterval *action);
 
	void setInnerAction(ActionInterval *action)
	{
		if (_innerAction != action)
		{
			OG_SAFE_RELEASE(_innerAction);
			_innerAction = action;
			OG_SAFE_RETAIN(_innerAction);
		}
	}

	/** Gets the inner action.
	 *
	 * @return The inner action.
	 */
	ActionInterval* getInnerAction()
	{
		return _innerAction;
	}

	//
	// Overrides
	//
	virtual RepeatForever* clone() const override;
	virtual RepeatForever* reverse() const override;
	virtual void startWithTarget(Node* target) override;
	/**
	 * @param dt In seconds.
	 */
	virtual void step(float dt) override;
	virtual bool isDone() const override;
	 
	RepeatForever()
		: _innerAction(nullptr)
	{}
	virtual ~RepeatForever();

	/** initializes the action */
	bool initWithAction(ActionInterval *action);

protected:
	/** Inner action */
	ActionInterval *_innerAction;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(RepeatForever);
};
//--------------------------------------Spaw------------------------------------
class   Spawn : public ActionInterval
{
public:
	 
	static Spawn* create(FiniteTimeAction *action1, ...)  ;

	 
	static Spawn* createWithVariableList(FiniteTimeAction *action1, va_list args);

	 
	static Spawn* create(const Vector<FiniteTimeAction*>& arrayOfActions);

	 
	static Spawn* createWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2);

	
	bool init(const Vector<FiniteTimeAction*>& arrayOfActions);
	bool initWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2);
	//
	// Overrides
	//
	virtual Spawn* clone() const override;
	virtual Spawn* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	virtual void stop() override;
	/**
	 * @param time In seconds.
	 */
	virtual void update(float time) override;
	 
	Spawn();
	virtual ~Spawn();

	

protected:
	FiniteTimeAction *_one;
	FiniteTimeAction *_two;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(Spawn);
};

//--------------------------------------move------------------------------------

class MoveTo : public ActionInterval
{
public:
	
	static MoveTo* create(float duration, const Vec2 &);
	
	virtual void startWithTarget(Node *target) override;
	MoveTo*  reverse() const;
	virtual MoveTo* clone() const override;
	void update(float dt);

	MoveTo() {}
	virtual ~MoveTo() {
	}

	bool initWithDuration(float duration, const Vec2&);
	

protected:
	Vec2 _positionDelta;
	Vec2 _startPosition;
	Vec2 _previousPosition;

	Vec2 _endPosition;

private:
	MoveTo(const MoveTo &) = delete;
	MoveTo &operator =(const MoveTo &) = delete;

};


//----------------------------------------------CircularMoveTo----------------------------------------------

class CircularMoveTo :public ActionInterval {

public:
	CircularMoveTo() :
		_cur_degrees(0.0f),
		_radius(0.0f),
		_dest_degrees(0.0f),
		_degrees(0.0f) {}
	~CircularMoveTo() {
		//	cout << "~CircularMoveTo" << endl;
	}
	static CircularMoveTo* create(float duration, float degrees = 0.0, const Vec2 &center = Vec2::ZERO);
	bool initWithDuration(float duration, float degrees, const Vec2 &center);


	virtual void startWithTarget(Node *target) override;
	virtual CircularMoveTo* clone() const override;
	void calcRadius(Node * target);

	void update(float);
	
	float _cur_degrees;
	float _old_degrees;

	float _radius;

	float _dest_degrees;

	float _degrees;

	Vec2 _positionDelta;
	Vec2 _startPosition;
	Vec2 _previousPosition;
	Vec2 _center;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(CircularMoveTo);
};

//----------------------------------------------JumpTo----------------------------------------------
class  JumpBy : public ActionInterval
{
public:
	/**
	 * Creates the action.
	 * @param duration Duration time, in seconds.
	 * @param position The jumping distance.
	 * @param height The jumping height.
	 * @param jumps The jumping times.
	 * @return An autoreleased JumpBy object.
	 */
	static JumpBy* create(float duration, const Vec2& position, float height, int jumps);

	//
	// Overrides
	//
	virtual JumpBy* clone() const override;
	virtual JumpBy* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	/**
	 * @param time In seconds.
	 */
	virtual void update(float time) override;
	 
	JumpBy() {}
	virtual ~JumpBy() {}

	/**
	 * initializes the action
	 * @param duration in seconds
	 */
	bool initWithDuration(float duration, const Vec2& position, float height, int jumps);

protected:
	Vec2           _startPosition;
	Vec2           _delta;
	float           _height;
	int             _jumps;
	Vec2           _previousPos;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(JumpBy);
};

  class JumpTo : public JumpBy
  {
  public:
  
  	static JumpTo* create(float duration, const Vec2& position, float height, int jumps);
  	bool initWithDuration(float duration, const Vec2& position, float height, int jumps);
  	//
  	// Override
  	//
  	virtual void startWithTarget(Node *target) override;
  	virtual JumpTo* clone() const override;
  	virtual JumpTo* reverse() const override;
  	
  	JumpTo() {}
  	virtual ~JumpTo() {}
  
  	/**
  	 * initializes the action
  	 * @param duration In seconds.
  	 */
//  	void update(float);
  
  protected:
  
//   	Vec2           _startPosition;
//   	Vec2           _delta;
//   	float           _height;
//   	int             _jumps;
//   	Vec2           _previousPos;
  
  	Vec2 _endPosition;
  
  private:
  	OG_DISALLOW_COPY_AND_ASSIGN(JumpTo);
  };



//----------------------------------------------BezierTo----------------------------------------------


typedef struct _ogBezierConfig {
	//! end position of the bezier
	Vec2 endPosition;
	//! Bezier control point 1
	Vec2 controlPoint_1;
	//! Bezier control point 2
	Vec2 controlPoint_2;

} ogBezierConfig;
// 
// class  BezierTo : public ActionInterval
// {
// protected:
// 	Vec2 _previousPosition;
// 	Vec2 _startPosition;
// 	std::vector<Vec2> _orPoint;
// public:
// 	template<typename...Args>
// 	static BezierTo* create(float t, Args &&...args)
// 	{
// 		BezierTo *bezierTo = new (std::nothrow) BezierTo();
// 
// 		if (bezierTo && bezierTo->initWithDuration(t, std::forward<Args>(args)...))
// 		{
// 			return bezierTo;
// 		}
// 
// 		delete bezierTo;
// 		return nullptr;
// 
// 	}
// 	
// 	template<typename...Args>
// 	bool initWithDuration(float t, Args &&...args)
// 	{
// 		if (ActionInterval::initWithDuration(t))
// 		{
// 			//(orPoint.push_back(args), ...);//17°æ±¾
// 
// 			for (auto &i : { args... })
// 			{
// 				_orPoint.push_back(i);
// 			}
// 			return true;
// 		}
// 
// 		return false;
// 	}
// 
// // 	static BezierTo* create(float t, std::vector<Vec2>& orPoint)
// // 	{
// // 		BezierTo *bezierTo = new (std::nothrow) BezierTo();
// // 
// // 		if (bezierTo && bezierTo->initWithDurationVec(t, orPoint))
// // 		{
// // 			return bezierTo;
// // 		}
// // 
// // 		delete bezierTo;
// // 		return nullptr;
// // 
// // 	}
// // 	bool initWithDurationVec(float t, std::vector<Vec2>& orPoint)
// // 	{
// // 		if (ActionInterval::initWithDuration(t))
// // 		{
// // 			//(orPoint.push_back(args), ...);//17°æ±¾
// // 
// // 			_orPoint = orPoint;
// // 			return true;
// // 		}
// // 
// // 		return false;
// // 	}
// 
// 	virtual void startWithTarget(Node *target) override;
// 	virtual BezierTo* clone() const override;
// 	BezierTo() {}
// 	virtual ~BezierTo() {}
// 	
// 	void update(float);
// 
// 	
// private:
// 	OG_DISALLOW_COPY_AND_ASSIGN(BezierTo);
// };


class   BezierTo : public ActionInterval
{
public:

	static BezierTo* create(float t, const ogBezierConfig& c);

	virtual void startWithTarget(Node *target) override;
	virtual BezierTo* clone() const override;
	virtual BezierTo* reverse() const override;

	void update(float time);

	BezierTo() {}
	virtual ~BezierTo() {}
	/**
	 * @param t In seconds.
	 */
	bool initWithDuration(float t, const ogBezierConfig &c);

protected:

	ogBezierConfig _config;
	Vec2 _startPosition;
	Vec2 _previousPosition;

	ogBezierConfig _toConfig;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(BezierTo);
};


//----------------------------------------------Blink----------------------------------------------

class  Blink : public ActionInterval
{
public:
	 
	static Blink* create(float duration, int blinks);
	bool initWithDuration(float duration, int blinks);
	 
	
	virtual void startWithTarget(Node *target) override;
	virtual void stop() override;
	virtual Blink* clone() const override;
	Blink() {}
	virtual ~Blink() {}
 
	virtual void update(float time) override;

protected:
	int _times;
	bool _originalState;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(Blink);
};

class   FadeTo : public ActionInterval
{
public:
	/**
	 * Creates an action with duration and opacity.
	 * @param duration Duration time, in seconds.
	 * @param opacity A certain opacity, the range is from 0 to 255.
	 * @return An autoreleased FadeTo object.
	 */
	static FadeTo* create(float duration, uint8_t opacity);

	//
	// Overrides
	//
	virtual FadeTo* clone() const override;
	virtual FadeTo* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	/**
	 * @param time In seconds.
	 */
	virtual void update(float time) override;

	public:
	FadeTo() {}
	virtual ~FadeTo() {}

	/**
	 * initializes the action with duration and opacity
	 * @param duration in seconds
	 */
	bool initWithDuration(float duration, uint8_t opacity);

protected:
	uint8_t _toOpacity;
	uint8_t _fromOpacity;
	friend class FadeOut;
	friend class FadeIn;
private:
	OG_DISALLOW_COPY_AND_ASSIGN(FadeTo);
};

/** @class FadeIn
 * @brief Fades In an object that implements the RGBAProtocol protocol. It modifies the opacity from 0 to 255.
 The "reverse" of this action is FadeOut
 */
class   FadeIn : public FadeTo
{
public:
	/**
	 * Creates the action.
	 * @param d Duration time, in seconds.
	 * @return An autoreleased FadeIn object.
	 */
	static FadeIn* create(float d);

	//
	// Overrides
	//
	virtual void startWithTarget(Node *target) override;
	virtual FadeIn* clone() const override;
	virtual FadeTo* reverse() const override;

	/**
	 * @js NA
	 */
	void setReverseAction(FadeTo* ac);

	public:
	FadeIn() : _reverseAction(nullptr) {}
	virtual ~FadeIn() {}

private:
	OG_DISALLOW_COPY_AND_ASSIGN(FadeIn);
	FadeTo* _reverseAction;
};

/** @class FadeOut
 * @brief Fades Out an object that implements the RGBAProtocol protocol. It modifies the opacity from 255 to 0.
 The "reverse" of this action is FadeIn
*/
class   FadeOut : public FadeTo
{
public:
	/**
	 * Creates the action.
	 * @param d Duration time, in seconds.
	 */
	static FadeOut* create(float d);

	//
	// Overrides
	//
	virtual void startWithTarget(Node *target) override;
	virtual FadeOut* clone() const  override;
	virtual FadeTo* reverse() const override;

	/**
	 * @js NA
	 */
	void setReverseAction(FadeTo* ac);

	public:
	FadeOut() : _reverseAction(nullptr) {}
	virtual ~FadeOut() {}
private:
	OG_DISALLOW_COPY_AND_ASSIGN(FadeOut);
	FadeTo* _reverseAction;
};
//----------------------------------------------DelayTime----------------------------------------------


class DelayTime : public ActionInterval
{
public:
	 
	static DelayTime* create(float d);

	virtual void update(float time) override;

	DelayTime * reverse() const;

	
	virtual DelayTime* clone() const override;
 
	DelayTime() {}
	virtual ~DelayTime() {}

private:
	OG_DISALLOW_COPY_AND_ASSIGN(DelayTime);
};

//----------------------------------------------ReverseTime----------------------------------------------
class   ReverseTime : public ActionInterval
{
public:
	/** Creates the action.
	 *
	 * @param action a certain action.
	 * @return An autoreleased ReverseTime object.
	 */
	static ReverseTime* create(FiniteTimeAction *action);
	bool initWithAction(FiniteTimeAction *action);
	//
	// Overrides
	//

	virtual void startWithTarget(Node *target) override;
	virtual void stop() override;
	virtual ReverseTime* clone() const override;
	/**
	 * @param time In seconds.
	 */
	virtual void update(float time) override;

 
	ReverseTime():_other(nullptr) {}
	virtual ~ReverseTime() { OG_SAFE_RELEASE(_other); }

	/** initializes the action */
	

protected:
	FiniteTimeAction *_other;

private:
	OG_DISALLOW_COPY_AND_ASSIGN(ReverseTime);
};

//----------------------------------------------Animate----------------------------------------------
class   Animate : public ActionInterval
{
public:
 
	static Animate* create(Animation *animation);
 
	void setAnimation(Animation* animation);
 
	Animation* getAnimation() { return _animation; }
	const Animation* getAnimation() const { return _animation; }
 
	int getCurrentFrameIndex() { return _currFrameIndex; }
	//
	// Overrides
	//
	virtual Animate* clone() const/* override*/;
	virtual Animate* reverse() const /*override*/;
	virtual void startWithTarget(Node *target)/* override*/;
	virtual void stop()/* override*/;
	/**
	 * @param t In seconds.
	 */
	virtual void update(float t)/* override*/;
	 
	Animate();
	virtual ~Animate();

	/** initializes the action with an Animation and will restore the original frame when the animation is over */
	bool initWithAnimation(Animation *animation);

protected:
	std::vector<float>* _splitTimes = new std::vector<float>;
	int             _nextFrame = 0;
	SpriteFrame*    _origFrame = nullptr;
	int _currFrameIndex = 0;
	unsigned int    _executedLoops = 0;

	Animation*      _animation = nullptr;

	EventCustom*    _frameDisplayedEvent = nullptr;
	AnimationFrame::DisplayedEventInfo _frameDisplayedEventInfo;
private:
	OG_DISALLOW_COPY_AND_ASSIGN(Animate);
};

//----------------------------------------------ReverseTime----------------------------------------------


OG_END

#endif
