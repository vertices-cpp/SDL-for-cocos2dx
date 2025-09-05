#include "OGAction.h"
#include "OGSprite.h"

#include "OGDirector.h"

OG_BEGIN




//--------------------------------------Action------------------------------------
//--------------------------------------FiniteTimeAction------------------------------------
//--------------------------------------ActionInterval------------------------------------



bool ActionInterval::initWithDuration(float d)
{
	_duration = d;

	_elapsed = 0;
	_firstTick = true;
	_done = false;

	return true;
}

bool ActionInterval::isDone() const
{
	return _done;
}

void ActionInterval::step(float dt)
{
	if (_firstTick)
	{
		_firstTick = false;
		_elapsed = 0;
	}
	else
	{
		_elapsed += dt;
	}


	float updateDt = std::max(0.0f,                                  // needed for rewind. elapsed could be negative
		std::min(1.0f, _elapsed / _duration)
	);


	this->update(updateDt);

	_done = _elapsed >= _duration;
}

void ActionInterval::startWithTarget(Node * target)
{
	FiniteTimeAction::startWithTarget(target);
	_elapsed = 0.0f;
	_firstTick = true;
	_done = false;
	
}
//--------------------------------------ExtraAction------------------------------------
ExtraAction* ExtraAction::create()
{
	ExtraAction* ret = new (std::nothrow) ExtraAction();
	if (ret)
	{
		ret->autorelease();
	}
	return ret;
}

ExtraAction* ExtraAction::clone() const
{
	// no copy constructor
	return ExtraAction::create();
}

ExtraAction* ExtraAction::reverse() const
{
	return ExtraAction::create();
}

void ExtraAction::update(float /*time*/)
{
}

void ExtraAction::step(float /*dt*/)
{
}


//--------------------------------------ActionInstant------------------------------------
void ActionInstant::startWithTarget(Node *target)
{
	FiniteTimeAction::startWithTarget(target);
	_done = false;
}

bool ActionInstant::isDone() const
{
	return _done;
}

void ActionInstant::step(float /*dt*/)
{
	float updateDt = 1;

	update(updateDt);
}

void ActionInstant::update(float /*time*/)
{
	_done = true;
}

//--------------------------------------ActionInstant------------------------------------

//--------------------------------------Sequence------------------------------------


Sequence* Sequence::createWithTwoActions(FiniteTimeAction *actionOne, FiniteTimeAction *actionTwo)
{
	Sequence *sequence = new (std::nothrow) Sequence();
	if (sequence && sequence->initWithTwoActions(actionOne, actionTwo))
	{
		sequence->autorelease();
		return sequence;
	}

	delete sequence;
	return nullptr;
}

Sequence* Sequence::create(FiniteTimeAction *action1, ...)
{
	va_list params;
	va_start(params, action1);

	Sequence *ret = Sequence::createWithVariableList(action1, params);

	va_end(params);

	return ret;
}

Sequence* Sequence::createWithVariableList(FiniteTimeAction *action1, va_list args)
{
	FiniteTimeAction *now;
	FiniteTimeAction *prev = action1;
	bool bOneAction = true;

	while (action1)
	{
		now = va_arg(args, FiniteTimeAction*);
		if (now)
		{
			prev = createWithTwoActions(prev, now);
			bOneAction = false;
		}
		else
		{
			// If only one action is added to Sequence, make up a Sequence by adding a simplest finite time action.
			if (bOneAction)
			{
				prev = createWithTwoActions(prev, ExtraAction::create());
			}
			break;
		}
	}

	return ((Sequence*)prev);
}

Sequence* Sequence::create(const Vector<FiniteTimeAction*>& arrayOfActions)
{
	Sequence* seq = new (std::nothrow) Sequence;

	if (seq && seq->init(arrayOfActions))
	{
		seq->autorelease();
		return seq;
	}

	delete seq;
	return nullptr;
}

bool Sequence::init(const Vector<FiniteTimeAction*>& arrayOfActions)
{
	auto count = arrayOfActions.size();
	if (count == 0)
		return false;

	if (count == 1)
		return initWithTwoActions(arrayOfActions.at(0), ExtraAction::create());

	// else size > 1
	auto prev = arrayOfActions.at(0);
	for (int i = 1; i < count - 1; ++i)
	{
		prev = createWithTwoActions(prev, arrayOfActions.at(i));
	}

	return initWithTwoActions(prev, arrayOfActions.at(count - 1));
}

bool Sequence::initWithTwoActions(FiniteTimeAction *actionOne, FiniteTimeAction *actionTwo)
{
// 	CCASSERT(actionOne != nullptr, "actionOne can't be nullptr!");
// 	CCASSERT(actionTwo != nullptr, "actionTwo can't be nullptr!");
	if (actionOne == nullptr || actionTwo == nullptr)
	{
	//	log("Sequence::initWithTwoActions error: action is nullptr!!");
		return false;
	}

	float d = actionOne->getDuration() + actionTwo->getDuration();
	ActionInterval::initWithDuration(d);

	_actions[0] = actionOne;
	actionOne->retain();

	_actions[1] = actionTwo;
	actionTwo->retain();

	return true;
}

bool Sequence::isDone() const
{
	// fix issue #17884
	if (dynamic_cast<ActionInstant*>(_actions[1]))
		return (_done && _actions[1]->isDone());
	else
		return _done;
}

Sequence* Sequence::clone() const
{
	// no copy constructor
	if (_actions[0] && _actions[1])
	{
		return Sequence::create(_actions[0]->clone(), _actions[1]->clone(), nullptr);
	}
	else {
		return nullptr;
	}
}

Sequence::Sequence()
	: _split(0)
{
	_actions[0] = nullptr;
	_actions[1] = nullptr;
}

Sequence::~Sequence()
{
	OG_SAFE_RELEASE(_actions[0]);
	OG_SAFE_RELEASE(_actions[1]);
}

void Sequence::startWithTarget(Node *target)
{
	if (target == nullptr)
	{
	//	log("Sequence::startWithTarget error: target is nullptr!");
		return;
	}
	if (_actions[0] == nullptr || _actions[1] == nullptr)
	{
	//	log("Sequence::startWithTarget error: _actions[0] or _actions[1] is nullptr!");
		return;
	}
	if (_duration > FLT_EPSILON)
		// fix #14936 - FLT_EPSILON (instant action) / very fast duration (0.001) leads to wrong split, that leads to call instant action few times
		_split = _actions[0]->getDuration() > FLT_EPSILON ? _actions[0]->getDuration() / _duration : 0;

	ActionInterval::startWithTarget(target);
	_last = -1;
}

void Sequence::stop()
{
	// Issue #1305
	if (_last != -1 && _actions[_last])
	{
		_actions[_last]->stop();
	}

	ActionInterval::stop();
}

void Sequence::update(float t)
{
	int found = 0;
	float new_t = 0.0f;

	if (t < _split)
	{
		// action[0]
		found = 0;
		if (_split != 0)
			new_t = t / _split;
		else
			new_t = 1;

	}
	else
	{
		// action[1]
		found = 1;
		if (_split == 1)
			new_t = 1;
		else
			new_t = (t - _split) / (1 - _split);
	}
//	std::cout << this << "t:" << t << "  split:" << _split << " new_t" << new_t << "\n";
	if (found == 1)
	{
		if (_last == -1)
		{
			// action[0] was skipped, execute it.
			_actions[0]->startWithTarget(_target);
			
				_actions[0]->update(1.0f);
			_actions[0]->stop();
		}
		else if (_last == 0)
		{
	
			_actions[0]->update(1.0f);
			_actions[0]->stop();
		}
	}
	else if (found == 0 && _last == 1)
	{
			_actions[1]->update(0);
		_actions[1]->stop();
	}
	// Last action found and it is done.
	if (found == _last && _actions[found]->isDone())
	{
		return;
	}

	// Last action found and it is done
	if (found != _last)
	{
		_actions[found]->startWithTarget(_target);
	}
	_actions[found]->update(new_t);
	_last = found;
}

Sequence* Sequence::reverse() const
{
	if (_actions[0] && _actions[1])
		return Sequence::createWithTwoActions(_actions[1]->reverse(), _actions[0]->reverse());
	else
		return nullptr;
}



//--------------------------------------Repeat------------------------------------

Repeat* Repeat::create(FiniteTimeAction *action, unsigned int times)
{
	Repeat* repeat = new (std::nothrow) Repeat();
	if (repeat && repeat->initWithAction(action, times))
	{
		repeat->autorelease();
		return repeat;
	}

	delete repeat;
	return nullptr;
}

bool Repeat::initWithAction(FiniteTimeAction *action, unsigned int times)
{
	if (action && ActionInterval::initWithDuration(action->getDuration() * times))
	{
		_times = times;
		_innerAction = action;
		action->retain();

		_actionInstant = dynamic_cast<ActionInstant*>(action) ? true : false;
		//an instant action needs to be executed one time less in the update method since it uses startWithTarget to execute the action
		// minggo: instant action doesn't execute action in Repeat::startWithTarget(), so comment it.
//        if (_actionInstant) 
//        {
//            _times -=1;
//        }
		_total = 0;

		return true;
	}

	return false;
}

Repeat* Repeat::clone() const
{
	// no copy constructor
	return Repeat::create(_innerAction->clone(), _times);
}

Repeat::~Repeat()
{
	OG_SAFE_RELEASE(_innerAction);
}

void Repeat::startWithTarget(Node *target)
{
	_total = 0;
	_nextDt = _innerAction->getDuration() / _duration;
	ActionInterval::startWithTarget(target);
	_innerAction->startWithTarget(target);
}

void Repeat::stop()
{
	_innerAction->stop();
	ActionInterval::stop();
}

// issue #80. Instead of hooking step:, hook update: since it can be called by any 
// container action like Repeat, Sequence, Ease, etc..
void Repeat::update(float dt)
{
	if (dt >= _nextDt)
	{
		while (dt >= _nextDt && _total < _times)
		{

				_innerAction->update(1.0f);
			_total++;

			_innerAction->stop();
			_innerAction->startWithTarget(_target);
			_nextDt = _innerAction->getDuration() / _duration * (_total + 1);
		}

		// fix for issue #1288, incorrect end value of repeat
		if (std::abs(dt - 1.0f) < FLT_EPSILON && _total < _times)
		{

				_innerAction->update(1.0f);

			_total++;
		}

		// don't set an instant action back or update it, it has no use because it has no duration
		if (!_actionInstant)
		{
			if (_total == _times)
			{
				// minggo: inner action update is invoked above, don't have to invoke it here
//                if (!(sendUpdateEventToScript(1, _innerAction)))
//                    _innerAction->update(1);
				_innerAction->stop();
			}
			else
			{
				// issue #390 prevent jerk, use right update

					_innerAction->update(dt - (_nextDt - _innerAction->getDuration() / _duration));
			}
		}
	}
	else
	{
			_innerAction->update(fmodf(dt * _times, 1.0f));
	}
}

bool Repeat::isDone() const
{
	return _total == _times;
}

Repeat* Repeat::reverse() const
{
	return Repeat::create(_innerAction->reverse(), _times);
}





//--------------------------------------RepeatForever------------------------------------

RepeatForever::~RepeatForever()
{
	OG_SAFE_RELEASE(_innerAction);
}

RepeatForever *RepeatForever::create(ActionInterval *action)
{
	RepeatForever *ret = new (std::nothrow) RepeatForever();
	if (ret && ret->initWithAction(action))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

bool RepeatForever::initWithAction(ActionInterval *action)
{
	assert(action != nullptr);
	if (action == nullptr)
	{
		
		return false;
	}

	action->retain();
	_innerAction = action;

	return true;
}

RepeatForever *RepeatForever::clone() const
{
	// no copy constructor
	return RepeatForever::create(_innerAction->clone());
}

void RepeatForever::startWithTarget(Node* target)
{
	ActionInterval::startWithTarget(target);
	_innerAction->startWithTarget(target);
}

void RepeatForever::step(float dt)
{

	_innerAction->step(dt);
	// only action interval should prevent jerk, issue #17808
	if (_innerAction->isDone() && _innerAction->getDuration() > 0)
	{
		float diff = _innerAction->getElapsed() - _innerAction->getDuration();
		if (diff > _innerAction->getDuration())
			diff = fmodf(diff, _innerAction->getDuration());
		_innerAction->startWithTarget(_target);
		// to prevent jerk. cocos2d-iphone issue #390, 1247
		_innerAction->step(0.0f);
		_innerAction->step(diff);
	}
}

bool RepeatForever::isDone() const
{
	return false;
}

RepeatForever *RepeatForever::reverse() const
{
	return RepeatForever::create(_innerAction->reverse());
}
//--------------------------------------Spaw------------------------------------

Spawn* Spawn::create(FiniteTimeAction *action1, ...)
{
	va_list params;
	va_start(params, action1);

	Spawn *ret = Spawn::createWithVariableList(action1, params);

	va_end(params);

	return ret;
}

Spawn* Spawn::createWithVariableList(FiniteTimeAction *action1, va_list args)
{
	FiniteTimeAction *now;
	FiniteTimeAction *prev = action1;
	bool oneAction = true;

	while (action1)
	{
		now = va_arg(args, FiniteTimeAction*);
		if (now)
		{
			prev = createWithTwoActions(prev, now);
			oneAction = false;
		}
		else
		{
			// If only one action is added to Spawn, make up a Spawn by adding a simplest finite time action.
			if (oneAction)
			{
				prev = createWithTwoActions(prev, ExtraAction::create());
			}
			break;
		}
	}

	return ((Spawn*)prev);
}

Spawn* Spawn::create(const Vector<FiniteTimeAction*>& arrayOfActions)
{
	Spawn* ret = new (std::nothrow) Spawn;

	if (ret && ret->init(arrayOfActions))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

Spawn* Spawn::createWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2)
{
	Spawn *spawn = new (std::nothrow) Spawn();
	if (spawn && spawn->initWithTwoActions(action1, action2))
	{
		spawn->autorelease();
		return spawn;
	}

	delete spawn;
	return nullptr;
}

bool Spawn::init(const Vector<FiniteTimeAction*>& arrayOfActions)
{
	auto count = arrayOfActions.size();

	if (count == 0)
		return false;

	if (count == 1)
		return initWithTwoActions(arrayOfActions.at(0), ExtraAction::create());

	// else count > 1
	auto prev = arrayOfActions.at(0);
	for (int i = 1; i < count - 1; ++i)
	{
		prev = createWithTwoActions(prev, arrayOfActions.at(i));
	}

	return initWithTwoActions(prev, arrayOfActions.at(count - 1));
}

bool Spawn::initWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2)
{
	assert(action1 != nullptr/*, "action1 can't be nullptr!"*/);
	assert(action2 != nullptr/*, "action2 can't be nullptr!"*/);
	if (action1 == nullptr || action2 == nullptr)
	{
		//log("Spawn::initWithTwoActions error: action is nullptr!");
		return false;
	}

	bool ret = false;

	float d1 = action1->getDuration();
	float d2 = action2->getDuration();

	if (ActionInterval::initWithDuration(std::max(d1, d2)))
	{
		_one = action1;
		_two = action2;

		if (d1 > d2)
		{
			_two = Sequence::createWithTwoActions(action2, DelayTime::create(d1 - d2));
		}
		else if (d1 < d2)
		{
			_one = Sequence::createWithTwoActions(action1, DelayTime::create(d2 - d1));
		}

		_one->retain();
		_two->retain();

		ret = true;
	}

	return ret;
}

Spawn* Spawn::clone() const
{
	// no copy constructor
	if (_one && _two)
		return Spawn::createWithTwoActions(_one->clone(), _two->clone());
	else
		return nullptr;
}

Spawn::Spawn()
	: _one(nullptr)
	, _two(nullptr)
{

}

Spawn::~Spawn()
{
	OG_SAFE_RELEASE(_one);
	OG_SAFE_RELEASE(_two);
}

void Spawn::startWithTarget(Node *target)
{
	if (target == nullptr)
	{
	 
		return;
	}
	if (_one == nullptr || _two == nullptr)
	{
		 
		return;
	}

	ActionInterval::startWithTarget(target);
	_one->startWithTarget(target);
	_two->startWithTarget(target);
}

void Spawn::stop()
{
	if (_one)
		_one->stop();

	if (_two)
		_two->stop();

	ActionInterval::stop();
}

void Spawn::update(float time)
{
	if (_one)
	{
 
			_one->update(time);
	}
	if (_two)
	{
 
			_two->update(time);
	}
}

Spawn* Spawn::reverse() const
{
	if (_one && _two)
		return Spawn::createWithTwoActions(_one->reverse(), _two->reverse());

	return nullptr;
}

//--------------------------------------MoveTo------------------------------------



MoveTo* MoveTo::create(float duration, const Vec2& position)
{
	MoveTo *ret = new (std::nothrow) MoveTo();

	if (ret && ret->initWithDuration(duration, position))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}


bool MoveTo::initWithDuration(float duration, const Vec2& position)
{
	bool ret = false;

	if (ActionInterval::initWithDuration(duration))
	{
		_endPosition = position;
		ret = true;
	}

	return ret;
}


void MoveTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_previousPosition = _startPosition = target->getPosition();
	_positionDelta = _endPosition - target->getPosition();
}
MoveTo * MoveTo::reverse() const
{
	return MoveTo::create(_duration, _startPosition);
}
MoveTo* MoveTo::clone() const
{
	// no copy constructor
	return MoveTo::create(_duration, _endPosition);
}
#ifndef OG_ENABLE_STACKABLE_ACTIONS
#define OG_ENABLE_STACKABLE_ACTIONS 1
#endif

void MoveTo::update(float dt)
{
	if (_target)
	{
#if OG_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();
		Vec2 diff = currentPos - _previousPosition;
		_startPosition = _startPosition + diff;
		Vec2 newPos = _startPosition + (_positionDelta * dt);
		_target->setPosition(newPos);
		_previousPosition = newPos;
#else
		_target->setPosition(_startPosition + _positionDelta * dt);
#endif // OG_ENABLE_STACKABLE_ACTIONS

		//std::cout << _target->getPosition().x << " " << _target->getPosition().y << std::endl;
	}
}

//----------------------------------------------CircularMoveTo----------------------------------------------

CircularMoveTo * CircularMoveTo::create(float duration, float degrees, const Vec2 & center)
{
	CircularMoveTo *ret = new (std::nothrow) CircularMoveTo();

	if (ret && ret->initWithDuration(duration, degrees, center))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;

}


bool CircularMoveTo::initWithDuration(float duration, float degrees, const Vec2 & center)
{
	bool ret = false;

	if (ActionInterval::initWithDuration(duration))
	{
		_degrees = degrees;
		_center = center;
		ret = true;
	}

	return ret;

}


void CircularMoveTo::startWithTarget(Node * target)
{
	ActionInterval::startWithTarget(target);
	_previousPosition = _startPosition = target->getPosition();
	//获取自己的位置
	calcRadius(target);
	
	//获取中心的原始角度
	_old_degrees = std::atan2( _startPosition.y - _center.y, _startPosition.x - _center.x) * 180 / 3.14f;

}
CircularMoveTo* CircularMoveTo::clone() const{
	return CircularMoveTo::create(_duration, _degrees, _center);
}

void CircularMoveTo::calcRadius(Node *target)
{
	//计算自己位置与中心点的距离
	float deltaX = _center.x - _startPosition.x;
	float deltaY = _center.y - _startPosition.y;

	_radius = sqrt(deltaX * deltaX + deltaY * deltaY);
}

void CircularMoveTo::update(float dt)
{
	if (_target)
	{	
		//得到占比
		_cur_degrees = _degrees * dt;

		float _dest_degrees = _cur_degrees +_old_degrees;

	//	std::cout << dt << " ----- "<<_cur_degrees<<" ----- "<< _dest_degrees << std::endl;
#if OG_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();
		Vec2 diff = currentPos - _previousPosition;

		_startPosition = _startPosition + diff;
		calcRadius(nullptr);

		auto cdx = fmodf(_dest_degrees, 360);

		_positionDelta.x = std::cosf(cdx* (3.14f / 180.0f)) *_radius;
		_positionDelta.y = std::sinf(cdx* (3.14f / 180.0f)) *_radius;

		Vec2 newPos = _center + _positionDelta ;

		_target->setPosition(newPos);

		_previousPosition = newPos;
#else
		auto cdx = fmodf(_dest_degrees, 360);

		_positionDelta.x = std::cosf(cdx* (3.14f / 180.0f)) *_radius;
		_positionDelta.y = std::sinf(cdx* (3.14f / 180.0f)) *_radius;

		_target->setPosition(_center + _positionDelta);
#endif // OG_ENABLE_STACKABLE_ACTIONS
	}

}
//----------------------------------------------JumpBy----------------------------------------------

JumpBy* JumpBy::create(float duration, const Vec2& position, float height, int jumps)
{
	JumpBy *jumpBy = new (std::nothrow) JumpBy();
	if (jumpBy && jumpBy->initWithDuration(duration, position, height, jumps))
	{
		jumpBy->autorelease();
		return jumpBy;
	}

	delete jumpBy;
	return nullptr;
}

bool JumpBy::initWithDuration(float duration, const Vec2& position, float height, int jumps)
{
	//CCASSERT(jumps >= 0, "Number of jumps must be >= 0");
	if (jumps < 0)
	{
	//	log("JumpBy::initWithDuration error: Number of jumps must be >= 0");
		return false;
	}

	if (ActionInterval::initWithDuration(duration) && jumps >= 0)
	{
		_delta = position;
		//SDL修改方向
		_delta.y = -_delta.y; 
		_height = -height;
		_jumps = jumps;

		return true;
	}

	return false;
}

JumpBy* JumpBy::clone() const
{
	// no copy constructor
	return JumpBy::create(_duration, _delta, _height, _jumps);
}

void JumpBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_previousPos = _startPosition = target->getPosition();
}

void JumpBy::update(float t)
{
	// parabolic jump (since v0.8.2)
	if (_target)
	{
		//得到总的跳跃进度[0,1]
		float frac = fmodf(t * _jumps, 1.0f);
		//抛物线公式
		float y = _height * 4 * frac * (1 - frac);
		//把垂直方向的持续偏移量 _delta.y * t 加到 y 上，_delta.y  表示垂直方向的持续移动速度。
		y +=  _delta.y * t;
		//计算水平方向的偏移量，_delta.x  持续移动速度。
		float x = _delta.x * t;
#if OG_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();

		Vec2 diff = currentPos - _previousPos;
		_startPosition = diff + _startPosition;

		Vec2 newPos = _startPosition + Vec2(x, y);
		_target->setPosition(newPos);

		_previousPos = newPos;
#else
		_target->setPosition(_startPosition + Vec2(x, y));
#endif // !OG_ENABLE_STACKABLE_ACTIONS
	}
}

JumpBy* JumpBy::reverse() const
{
	return JumpBy::create(_duration, Vec2(-_delta.x, -_delta.y),
		_height, _jumps);
}

//----------------------------------------------JumpTo----------------------------------------------

JumpTo * JumpTo::create(float duration, const Vec2 & position, float height, int jumps)
{
	JumpTo *jumpTo = new (std::nothrow) JumpTo();

	if (jumpTo && jumpTo->initWithDuration(duration, position, height, jumps))
	{
		jumpTo->autorelease();
		return jumpTo;
	}

	delete jumpTo;
	return nullptr;;
}

bool JumpTo::initWithDuration(float duration, const Vec2 & position, float height, int jumps)
{
	if (jumps < 0)
	{
		//log("JumpTo::initWithDuration error:Number of jumps must be >= 0");
		return false;
	}

	if (ActionInterval::initWithDuration(duration) && jumps >= 0)
	{
		_endPosition = position;
		_endPosition.y = -_endPosition.y;
		//SDL修改方向
		_height = -height;
		_jumps = jumps;

		return true;
	}

	return false;
}

void JumpTo::startWithTarget(Node * target)
{


	//JumpBy
	JumpBy::startWithTarget(target);

	// 	ActionInterval::startWithTarget(target);
// 	_previousPos = _startPosition = target->getPosition();

	//JumpTo
	_delta = { _endPosition.x - _startPosition.x, _endPosition.y - _startPosition.y };
 
}
JumpTo* JumpTo::clone() const
{
	// no copy constructor
	return JumpTo::create(_duration, _endPosition, _height, _jumps);
}
JumpTo* JumpTo::reverse() const
{
// 	return JumpTo::create(_duration, Vec2(-_delta.x, -_delta.y),
// 		_height, _jumps);
	return nullptr;
}
// void JumpTo::update(float t)
// {
// 	// parabolic jump (since v0.8.2)
// 	if (_target)
// 	{
// 		float frac = fmodf(t * _jumps, 1.0f);
// 		float y = _height * 4 * frac * (1 - frac);
// 		y += _delta.y * t;
// 
// 		float x = _delta.x * t;
// #if OG_ENABLE_STACKABLE_ACTIONS
// 		Vec2 currentPos = _target->getPosition();
// 
// 		Vec2 diff = currentPos - _previousPos;
// 		_startPosition = diff + _startPosition;
// 
// 		Vec2 newPos = _startPosition + Vec2(x, y);
// 		_target->setPosition(newPos);
// 
// 		_previousPos = newPos;
// #else
// 		_target->setPosition(_startPosition + Vec2(x, y));
// #endif // !OG_ENABLE_STACKABLE_ACTIONS
// 	}
// }

//----------------------------------------------BezierTo----------------------------------------------

// 
// // 必须为double类型
// float factorial(int num) { //递归阶乘
// 	if (num <= 1) {
// 		return 1;
// 	}
// 	else {
// 		return num * factorial(num - 1);
// 	}
// }
// 
// 
// Vec2 bezierFun(Vec2 start, std::vector<Vec2> arr, float t) { //通过各控制点与占比t计算当前贝塞尔曲线上的点坐标
// 
// 	arr.insert(arr.begin(), start);
// 	Vec2 ret = { 0,0 };
// 	int n = arr.size() - 1;
// 	double temp = (1 - t);
// 	for (size_t index = 0; index < arr.size(); index++) {
// 
// 		if (!index) {
// 			ret += (arr[index] * (float)pow(temp, n - index) *  (float)pow(t, index));
// 		}
// 		else {
// 			float a = factorial(n);
// 			float b = factorial(index);
// 			float c = factorial(n - index);
// 			float nOrder = a / b / c;
// 			ret += (arr[index] * nOrder * (float)pow(temp, n - index) *(float)pow(t, index));
// 		}
// 
// 	}
// 
// 	return ret;
// }
// 
// void BezierTo::startWithTarget(Node * target)
// {
// 	ActionInterval::startWithTarget(target);
// 	_previousPosition = _startPosition = target->getPosition();
// }
// // BezierTo* BezierTo::clone() const
// // {
// // 	// no copy constructor
// // 	return BezierTo::create(_duration, _orPoint);
// // }
// 
// void BezierTo::update(float dt)
// {
// 	if (_target)
// 	{
// #if OG_ENABLE_STACKABLE_ACTIONS
// 		Vec2 currentPos = _target->getPosition();
// 		Vec2 diff = currentPos - _previousPosition;
// 		_startPosition = _startPosition + diff;
// 		Vec2 newPos = bezierFun(_startPosition, _orPoint, dt);
// 		_target->setPosition(newPos);
// 
// 		_previousPosition = newPos;
// #else
// 		Vec2 newPos = bezierFun(_startPosition, _orPoint, dt);
// 		_target->setPosition(newPos);
// #endif
// 	}
// }


static inline float bezierat(float a, float b, float c, float d, float t)
{
	return (powf(1 - t, 3) * a +
		3 * t*(powf(1 - t, 2))*b +
		3 * powf(t, 2)*(1 - t)*c +
		powf(t, 3)*d);
}


BezierTo* BezierTo::create(float t, const ogBezierConfig& c)
{
	BezierTo *bezierTo = new (std::nothrow) BezierTo();
	if (bezierTo && bezierTo->initWithDuration(t, c))
	{
		bezierTo->autorelease();
		return bezierTo;
	}

	delete bezierTo;
	return nullptr;
}

bool BezierTo::initWithDuration(float t, const ogBezierConfig &c)
{
	if (ActionInterval::initWithDuration(t))
	{
		_toConfig = c;
		return true;
	}

	return false;
}

BezierTo* BezierTo::clone() const
{
	// no copy constructor
	return BezierTo::create(_duration, _toConfig);
}

void BezierTo::startWithTarget(Node *target)
{
	startWithTarget(target);
	_previousPosition = _startPosition = target->getPosition();

	_config.controlPoint_1 = _toConfig.controlPoint_1 - _startPosition;
	_config.controlPoint_2 = _toConfig.controlPoint_2 - _startPosition;
	_config.endPosition = _toConfig.endPosition - _startPosition;
}

BezierTo* BezierTo::reverse() const
{
	assert(false);
	return nullptr;

	ogBezierConfig r;
	//回朔
	r.endPosition = -_toConfig.endPosition;
	r.controlPoint_1 = _toConfig.controlPoint_2 + (-_toConfig.endPosition);
	r.controlPoint_2 = _toConfig.controlPoint_1 + (-_toConfig.endPosition);

	BezierTo *action = BezierTo::create(_duration, r);
	return action;
}

void BezierTo::update(float time)
{
	if (_target)
	{
		float xa = 0;
		float xb = _config.controlPoint_1.x;
		float xc = _config.controlPoint_2.x;
		float xd = _config.endPosition.x;

		float ya = 0;
		float yb = _config.controlPoint_1.y;
		float yc = _config.controlPoint_2.y;
		float yd = _config.endPosition.y;

		float x = bezierat(xa, xb, xc, xd, time);
		float y = bezierat(ya, yb, yc, yd, time);

#if OG_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();
		Vec2 diff = currentPos - _previousPosition;
		_startPosition = _startPosition + diff;

		Vec2 newPos = _startPosition + Vec2(x, y);
		_target->setPosition(newPos);

		_previousPosition = newPos;
#else
		_target->setPosition(_startPosition + Vec2(x, y));
#endif // !OG_ENABLE_STACKABLE_ACTIONS
	}
}


//----------------------------------------------Blink----------------------------------------------
Blink * Blink::create(float duration, int blinks)
{

	Blink *blink = new (std::nothrow) Blink();
	if (blink && blink->initWithDuration(duration, blinks))
	{
		blink->autorelease();
		return blink;
	}

	delete blink;
	return nullptr;

}

bool Blink::initWithDuration(float duration, int blinks)
{
	 

	if (blinks < 0)
	{
	//	log("Blink::initWithDuration error:blinks should be >= 0");
		return false;
	}

	if (ActionInterval::initWithDuration(duration) && blinks >= 0)
	{
		_times = blinks;
		return true;
	}

	return false;

}

void Blink::stop()
{
	if (nullptr != _target)
		_target->setVisible(_originalState);
	ActionInterval::stop();
}


void Blink::startWithTarget(Node * target)
{
	ActionInterval::startWithTarget(target);
	_originalState = target->isVisible();
}
 
Blink* Blink::clone() const
{
	// no copy constructor
	return Blink::create(_duration, _times);
}
void Blink::update(float time)
{
	if (_target && !isDone())
	{
		float slice = 1.0f / _times;
		float m = fmodf(time, slice);
		_target->setVisible(m > slice / 2 ? true : false);
	}
}


//
// FadeIn
//

FadeIn* FadeIn::create(float d)
{
	FadeIn* action = new (std::nothrow) FadeIn();
	if (action && action->initWithDuration(d, 255))
	{
		action->autorelease();
		return action;
	}

	delete action;
	return nullptr;
}

FadeIn* FadeIn::clone() const
{
	// no copy constructor
	return FadeIn::create(_duration);
}

void FadeIn::setReverseAction( FadeTo *ac)
{
	_reverseAction = ac;
}


FadeTo* FadeIn::reverse() const
{
	auto action = FadeOut::create(_duration);
	action->setReverseAction(const_cast<FadeIn*>(this));
	return action;

}

void FadeIn::startWithTarget( Node *target)
{
	ActionInterval::startWithTarget(target);

	if (nullptr != _reverseAction)
		this->_toOpacity = this->_reverseAction->_fromOpacity;
	else
		_toOpacity = 255;

	if (target)
		_fromOpacity = target->getOpacity();
}

//
// FadeOut
//

FadeOut* FadeOut::create(float d)
{
	FadeOut* action = new (std::nothrow) FadeOut();
	if (action && action->initWithDuration(d, 0))
	{
		action->autorelease();
		return action;
	}

	delete action;
	return nullptr;
}

FadeOut* FadeOut::clone() const
{
	// no copy constructor
	return FadeOut::create(_duration);
}

void FadeOut::startWithTarget( Node *target)
{
	ActionInterval::startWithTarget(target);

	if (nullptr != _reverseAction)
		_toOpacity = _reverseAction->_fromOpacity;
	else
		_toOpacity = 0;

	if (target)
		_fromOpacity = target->getOpacity();
}

void FadeOut::setReverseAction( FadeTo *ac)
{
	_reverseAction = ac;
}


FadeTo* FadeOut::reverse() const
{
	auto action = FadeIn::create(_duration);
	action->setReverseAction(const_cast<FadeOut*>(this));
	return action;
}

//
// FadeTo
//

FadeTo* FadeTo::create(float duration, uint8_t opacity)
{
	FadeTo *fadeTo = new (std::nothrow) FadeTo();
	if (fadeTo && fadeTo->initWithDuration(duration, opacity))
	{
		fadeTo->autorelease();
		return fadeTo;
	}

	delete fadeTo;
	return nullptr;
}

bool FadeTo::initWithDuration(float duration, uint8_t opacity)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_toOpacity = opacity;
		return true;
	}

	return false;
}

FadeTo* FadeTo::clone() const
{
	// no copy constructor
	return FadeTo::create(_duration, _toOpacity);
}

FadeTo* FadeTo::reverse() const
{
//	CCASSERT(false, "reverse() not supported in FadeTo");
	return nullptr;
}

void FadeTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);

	if (target)
	{
		_fromOpacity = target->getOpacity();
	}
}

void FadeTo::update(float time)
{
	if (_target)
	{
		_target->setOpacity((uint8_t)(_fromOpacity + (_toOpacity - _fromOpacity) * time));
	}
}

//----------------------------------------------DelayTime----------------------------------------------


 
DelayTime* DelayTime::create(float d)
{
	DelayTime* action = new (std::nothrow) DelayTime();
	if (action && action->initWithDuration(d))
	{
		action->autorelease();
		return action;
	}

	delete action;
	return nullptr;
 }
DelayTime* DelayTime::clone() const
{
	// no copy constructor
	return DelayTime::create(_duration);
}
void DelayTime::update(float time) {}
DelayTime* DelayTime::reverse() const
{
	return DelayTime::create(_duration);
}
//----------------------------------------------ReverseTime----------------------------------------------


ReverseTime * ReverseTime::create(FiniteTimeAction * action)
{
	// casting to prevent warnings
	ReverseTime *reverseTime = new (std::nothrow) ReverseTime();
	if (reverseTime && reverseTime->initWithAction(action->clone()))
	{

		return reverseTime;
	}

	delete reverseTime;
	return nullptr;
}

bool ReverseTime::initWithAction(FiniteTimeAction * action)
{
// 	CCASSERT(action != nullptr, "action can't be nullptr!");
// 	CCASSERT(action != _other, "action doesn't equal to _other!");
	if (action == nullptr || action == _other)
	{
	//	log("ReverseTime::initWithAction error: action is null or action equal to _other");
		return false;
	}

	if (ActionInterval::initWithDuration(action->getDuration()))
	{
		// Don't leak if action is reused
		OG_SAFE_RELEASE(_other);

		_other = action;
		action->retain();

		return true;
	}

	return false;
}

void ReverseTime::startWithTarget(Node * target)
{
	ActionInterval::startWithTarget(target);
	_other->startWithTarget(target);
}

void ReverseTime::stop()
{
	_other->stop();
	ActionInterval::stop();
}

ReverseTime* ReverseTime::clone() const
{
	// no copy constructor
	return ReverseTime::create(_other->clone());
}

void ReverseTime::update(float time)
{
	if (_other)
	{
			_other->update(1 - time);
	}
}

//----------------------------------------------Animate----------------------------------------------

// Animate
//
Animate* Animate::create(Animation *animation)
{
	Animate *animate = new (std::nothrow) Animate();
	if (animate && animate->initWithAnimation(animation))
	{
		animate->autorelease();
		return animate;
	}

	delete animate;
	return nullptr;
}

Animate::Animate()
{
}

Animate::~Animate()
{
	OG_SAFE_RELEASE(_animation);
	//OG_SAFE_RELEASE(_origFrame);
	OG_SAFE_DELETE(_splitTimes);
	OG_SAFE_RELEASE(_frameDisplayedEvent);
}

bool Animate::initWithAnimation(Animation* animation)
{
	//CCASSERT(animation != nullptr, "Animate: argument Animation must be non-nullptr");
	if (animation == nullptr)
	{
	//	log("Animate::initWithAnimation: argument Animation must be non-nullptr");
		return false;
	}

	float singleDuration = animation->getDuration();

	if (ActionInterval::initWithDuration(singleDuration * animation->getLoops()))
	{
		_nextFrame = 0;
		setAnimation(animation);
		_origFrame = nullptr;
		_executedLoops = 0;

		_splitTimes->reserve(animation->getFrames().size());

		float accumUnitsOfTime = 0;
		float newUnitOfTimeValue = singleDuration / animation->getTotalDelayUnits();

		auto& frames = animation->getFrames();

		for (auto& frame : frames)
		{
			float value = (accumUnitsOfTime * newUnitOfTimeValue) / singleDuration;
			accumUnitsOfTime += frame->getDelayUnits();
			_splitTimes->push_back(value);
		}
		return true;
	}
	return false;
}

void Animate::setAnimation(Animation *animation)
{
	if (_animation != animation)
	{
		OG_SAFE_RETAIN(animation);
		OG_SAFE_RELEASE(_animation);
		_animation = animation;
	}
}

Animate* Animate::clone() const
{
	// no copy constructor
	return Animate::create(_animation->clone());
}

void Animate::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	Sprite *sprite = static_cast<Sprite*>(target);

//	OG_SAFE_RELEASE(_origFrame);

	if (_animation->getRestoreOriginalFrame())
	{
// 		_origFrame = sprite->getSpriteFrame();
// 		_origFrame->retain();
	}
	_nextFrame = 0;
	_executedLoops = 0;
}

void Animate::stop()
{
	if (_animation->getRestoreOriginalFrame() && _target)
	{
		//auto blend = static_cast<Sprite*>(_target)->getBlendFunc();
		//	static_cast<Sprite*>(_target)->setSpriteFrame(_origFrame);
		//	static_cast<Sprite*>(_target)->setBlendFunc(blend);
	}

	ActionInterval::stop();
}

void Animate::update(float t)
{
	// if t==1, ignore. Animation should finish with t==1
	if (t < 1.0f)
	{
		t *= _animation->getLoops();

		// new loop?  If so, reset frame counter
		unsigned int loopNumber = (unsigned int)t;
		if (loopNumber > _executedLoops)
		{
			_nextFrame = 0;
			_executedLoops++;
		}

		// new t for animations
		t = fmodf(t, 1.0f);
	}

	auto& frames = _animation->getFrames();
	int numberOfFrames = frames.size();
	SpriteFrame *frameToDisplay = nullptr;

	for (int i = _nextFrame; i < numberOfFrames; i++)
	{
		float splitTime = _splitTimes->at(i);

		if (splitTime <= t)
		{
			//auto blend = static_cast<Sprite*>(_target)->getBlendFunc();
			_currFrameIndex = i;
			AnimationFrame* frame = frames.at(_currFrameIndex);
			frameToDisplay = frame->getSpriteFrame();
			static_cast<Sprite*>(_target)->setSpriteFrame(frameToDisplay);
			//static_cast<Sprite*>(_target)->setBlendFunc(blend);

			const ValueMap& dict = frame->getUserInfo();
			if (!dict.empty())
			{
				if (_frameDisplayedEvent == nullptr)
					_frameDisplayedEvent = new (std::nothrow) EventCustom(AnimationFrameDisplayedNotification);

				_frameDisplayedEventInfo.target = _target;
				_frameDisplayedEventInfo.userInfo = &dict;
				_frameDisplayedEvent->setUserData(&_frameDisplayedEventInfo);
				Director::getInstance()->getEventDispatcher()->dispatchEvent(_frameDisplayedEvent);
			}
			_nextFrame = i + 1;
		}
		// Issue 1438. Could be more than one frame per tick, due to low frame rate or frame delta < 1/FPS
		else {
			break;
		}
	}
}

Animate* Animate::reverse() const
{
	auto& oldArray = _animation->getFrames();
	Vector<AnimationFrame*> newArray;// (oldArray.size());

	if (!oldArray.empty())
	{
		for (auto iter = oldArray.rbegin(), iterCrend = oldArray.rend(); iter != iterCrend; ++iter)
		{
			AnimationFrame* animFrame = *iter;
			if (!animFrame)
			{
				break;
			}

			newArray.pushBack(animFrame->clone());
		}
	}

	Animation *newAnim = Animation::create(newArray, _animation->getDelayPerUnit(), _animation->getLoops());
	newAnim->setRestoreOriginalFrame(_animation->getRestoreOriginalFrame());
	return Animate::create(newAnim);
}
//----------------------------------------------Animate----------------------------------------------



//----------------------------------------------Animate----------------------------------------------


OG_END

