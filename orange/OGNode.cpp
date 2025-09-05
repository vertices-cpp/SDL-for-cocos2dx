#include "OGNode.h"

#include "OGDirector.h"

#include "OGUpdate.h"
#include "OGActionManager.h"
#include "OGEventDispatcher.h"

#include "OGAction.h"
//#include "OGTypes.h"


OG_BEGIN

//Director* Node::_director = nullptr;
int Node::__attachedNodeCount = 0;

Node::Node() :
	_running(false)
	, _visible(true)
	,_realColor(Color4B::WHITE)
	, _displayedColor(Color4B::WHITE)
	//, _modiflyColor(false)
	, _rotate(Vec2::ZERO)
	, _pointer(Vec2::ZERO)
	, _render_position(Vec2::ZERO)
	, _contentSize(Size::ZERO)
	, _parent(nullptr)
	, _tag(Node::INVALID_TAG)
	, _flipX(false)
	, _flipY(false)
	, _localZOrder(0)
	, _globalZOrder(0)
	, _reorderChildDirty(false)
	, _contentChange(true)
	, _contentSizeDirty(true)
	,
	_originalDestPosition(Rect::ZERO)
	
{
	_director = Director::getInstance();
	_actionManager = _director->getActionManager();
	_actionManager->retain();

	_update_ptr = _director->getUpdatePtr();
	_update_ptr->retain();
	
   _eventDispatcher = _director->getEventDispatcher();
   _eventDispatcher->retain();

}

Node * Node::create()
{
	Node * ret = new (std::nothrow) Node();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		OG_SAFE_DELETE(ret);
	}
	return ret;
}

Node::~Node()
{
	for (auto& child : _children)
	{
		child->_parent = nullptr;
	}

	stopAllActions();
	//unscheduleAllCallbacks();

	

	OG_SAFE_RELEASE_NULL(_actionManager);

//	OG_SAFE_RELEASE_NULL(_scheduler);
	setNotUpdate();
	OG_SAFE_RELEASE_NULL(_update_ptr);

	_eventDispatcher->removeEventListenersForTarget(this);

	OG_SAFE_RELEASE(_eventDispatcher);

}
bool Node::init()
{
	return true;
}

void Node::cleanup()
{
 

	// actions
	this->stopAllActions();
	// timers
	//this->unscheduleAllCallbacks();
	setNotUpdate();
	 

	for (const auto &child : _children)
		child->cleanup();
}

Vec2 Node::getRotation() const
{
	 
	return _rotate;
}


void Node::setRotation(float rotation)
{
	if (_rotate.x == rotation)
		return;

	_rotate = { rotation,rotation };

	_contentChange = true;
}

float Node::getRotationSkewX() const
{
	return _rotate.x;
}

float Node::getRotationSkewY() const
{
	return _rotate.y;
}

void Node::setRotationSkewX(float rotationX)
{
	if (_rotate.x == rotationX)
		return;

	_rotate.x = rotationX;

	_contentChange = true;
}

void Node::setRotationSkewY(float rotationX)
{
	if (_rotate.y == rotationX)
		return;

	_rotate.y= rotationX;

	_contentChange = true;

// 	_transformUpdated = _transformDirty = _inverseDirty = true;
// 
// 	updateRotationQuat();
}


const Vec2 & Node::getRenderPosition() const
{
	return _render_position;
}

void Node::setRenderPosition(const Vec2 & position)
{
	_render_position = position;
}

const Vec2& Node::getPosition() const
{
	return _position;
}

 

void Node::setPosition(const Vec2& position)
{
	setPosition(position.x  , position.y);
}

void Node::setPosition(float x, float y)
{
	if (_position.x == x && _position.y == y)
		return;

	_position.x = x;
	_position.y = y;

	_render_position = _position;
	//processParentFlags();
	_contentChange  = true;
	//_usingNormalizedPosition = false;
}
float Node::getPositionX() const
{
	return _position.x;
}

void Node::setPositionX(float x)
{
	setPosition(x, _position.y);

}

float Node::getPositionY() const
{
	return  _position.y;
}

void Node::setPositionY(float y)
{
	setPosition(_position.x, y);
}

// const Vec2& Node::getPositionNormalized() const
// {
// 	return _normalizedPosition;
// }


long Node::getChildrenCount() const
{
	return _children.size();
}

bool Node::isVisible() const
{
	return _visible;
}
void Node::setVisible(bool visible)
{
	if (visible != _visible)
	{
		_visible = visible;
		if (_visible)
			_contentChange = true;
	}
}

void  Node::setFlipX(bool flipX) {
	_flipX = flipX;
	_contentChange = true;
}
void  Node::setFlipY(bool flipY) {
	_flipY = flipY;
	_contentChange = true;
}
bool  Node::getFlipX() {
	return _flipX;

}
bool  Node::getFlipY() {
	return _flipY;
}
const Size& Node::getContentSize() const
{
	return _contentSize;
}

void Node::setContentSize(const Size & size)
{
	if (!size.equals(_contentSize))
	{
		_contentSize = size;
		_contentSizeDirty = true;
	}

}

// isRunning getter
bool Node::isRunning() const
{
	return _running;
}

/// parent setter
void Node::setParent(Node * parent)
{
	_parent = parent;
	_contentChange = true;
}

int Node::getTag() const
{
	return _tag;
}

/// tag setter
void Node::setTag(int tag)
{
	_tag = tag;
}

Node* Node::getChildByTag(int tag) const
{
//	CCASSERT(tag != Node::INVALID_TAG, "Invalid tag");

	for (const auto child : _children)
	{
		if (child && child->_tag == tag)
			return child;
	}
	return nullptr;
}



Scene* Node::getScene() const
{
	if (!_parent)
		return nullptr;

	auto sceneNode = _parent;
	while (sceneNode->_parent)
	{
		sceneNode = sceneNode->_parent;
	}

	return dynamic_cast<Scene*>(sceneNode);
}
void Node::addChild(Node *child)
{
	this->addChild(child, child->getLocalZOrder());
}
void Node::addChild(Node *child, int localZOrder)
{
	addChildHelper(child, localZOrder, INVALID_TAG);
}

void Node::addChild(Node *child, int localZOrder, int tag)
{
// 	CCASSERT(child != nullptr, "Argument must be non-nil");
// 	CCASSERT(child->_parent == nullptr, "child already added. It can't be added again");

	addChildHelper(child, localZOrder, tag);
}


void Node::addChildHelper(Node* child, int localZOrder,int tag)
{
	auto assertNotSelfChild
	([this, child]() -> bool
	{
		for (Node* parent(getParent()); parent != nullptr;
			parent = parent->getParent())
			if (parent == child)
				return false;

		return true;
	});
	(void)assertNotSelfChild;

	assert(assertNotSelfChild()/*,"A node cannot be the child of his own children"*/);

	if (_children.empty())
	{
		this->childrenAlloc();
	}

	this->insertChild(child, localZOrder);

	child->setTag(tag);
	child->setParent(this);

	//child->updateOrderOfArrival();

	if (_running)
	{
		child->onEnter();
		// prevent onEnterTransitionDidFinish to be called twice when a node is added in onEnter
// 		if (_isTransitionFinished)
// 		{
// 			child->onEnterTransitionDidFinish();
// 		}
	}

// 	if (_cascadeColorEnabled)
// 	{
// 		updateCascadeColor();
// 	}
// 
// 	if (_cascadeOpacityEnabled)
// 	{
// 		updateCascadeOpacity();
// 	}
}

void Node::insertChild(Node* child, int localZOrder)
{
	_children.pushBack(child);
	_contentChange= _reorderChildDirty = true;
	child->_setLocalZOrder(localZOrder);
}
void Node::sortAllChildren()
{
	if (_reorderChildDirty)
	{
		sortNodes(_children);
		_reorderChildDirty = false;
		_eventDispatcher->setDirtyForNode(this);
	}
}
void Node::setLocalZOrder(int z)
{
	if (getLocalZOrder() == z)
		return;

	_setLocalZOrder(z);
	if (_parent)
	{
		_parent->reorderChild(this, z);
	}
// 
 	_eventDispatcher->setDirtyForNode(this);
}
void Node::reorderChild(Node *child, int zOrder)
{
	_reorderChildDirty = true;
	child->_setLocalZOrder(zOrder);

}
void Node::_setLocalZOrder(int z)
{
	_localZOrder = z;
	_reorderChildDirty = true;
	
}

 float Node::getGlobalZOrder() const { return _globalZOrder; }
void Node::setGlobalZOrder(float globalZOrder)
{
	if (_globalZOrder != globalZOrder)
	{
		_globalZOrder = globalZOrder;
		_eventDispatcher->setDirtyForNode(this);
	}
}

void Node::onEnter()
{
	if (!_running)
	{
		++__attachedNodeCount;
	}


	for (const auto &child : _children)
		child->onEnter();

	this->resume();

	_running = true;

}

void Node::onExit()
{
	if (_running)
	{
		--__attachedNodeCount;
	}

	this->pause();

	_running = false;

	for (const auto &child : _children)
		child->onExit();

}


void Node::removeChildByTag(int tag, bool cleanup/* = true */)
{
	//CCASSERT(tag != Node::INVALID_TAG, "Invalid tag");

	Node *child = this->getChildByTag(tag);

	if (child == nullptr)
	{
	//	CCLOG("cocos2d: removeChildByTag(tag = %d): child not found!", tag);
	}
	else
	{
		this->removeChild(child, cleanup);
	}
}
void Node::removeFromParentAndCleanup(bool cleanup)
{
	if (_parent != nullptr)
	{
		_parent->removeChild(this, cleanup);
	}
}
void Node::removeChild(Node* child, bool cleanup /* = true */)
{
	// explicit nil handling
	if (_children.empty())
	{
		return;
	}

	size_t index = _children.getIndex(child);
	if (index != CC_INVALID_INDEX)
		this->detachChild(child, index, cleanup);
}
void Node::detachChild(Node *child, size_t childIndex, bool doCleanup)
{
	// IMPORTANT:
	//  -1st do onExit
	//  -2nd cleanup
	if (_running)
	{
	//	child->onExitTransitionDidStart();
		child->onExit();
	}

	// If you don't do cleanup, the child's actions will not get removed and the
	// its scheduledSelectors_ dict will not get released!
	if (doCleanup)
	{
		child->cleanup();
	}

#if CC_ENABLE_GC_FOR_NATIVE_OBJECTS
	auto sEngine = ScriptEngineManager::getInstance()->getScriptEngine();
	if (sEngine)
	{
		sEngine->releaseScriptObject(this, child);
	}
#endif // CC_ENABLE_GC_FOR_NATIVE_OBJECTS
	// set parent nil at the end
	child->setParent(nullptr);

	_children.erase(childIndex);
}

void Node::removeAllChildren()
{
	this->removeAllChildrenWithCleanup();
}
void Node::removeAllChildrenWithCleanup()
{
	// not using detachChild improves speed here
	for (const auto& child : _children)
	{
		// IMPORTANT:
		//  -1st do onExit
		//  -2nd cleanup
		if (_running)
		{
	 
			child->onExit();
		}
		 
			child->cleanup(); 
		// set parent nil at the end
		child->setParent(nullptr);
	}

	_children.clear();
}

void Node::detachChild(  Node *child, int childIndex)
{
	// IMPORTANT:
	//  -1st do onExit
	//  -2nd cleanup
	if (_running)
	{
		child->onExit();
	}
	child->cleanup();

	child->setParent(nullptr);
	_children.erase(childIndex);
}



void Node::childrenAlloc()
{
	_children.reserve(4);
}


void Node::setUpdate()
{
	if (_director->getUpdatePtr() != nullptr)
		_update_ptr->setUpdate(this);
}

void Node::setNotUpdate() {

	if (_director->getUpdatePtr() == nullptr)
	{
		_update_ptr = nullptr;
		return;
	}
	_update_ptr->setNullUpdate(this);
}

void Node::update(float delta)
{

}

void Node::draw()
{
	
}
uint32_t Node::processParentFlags(uint32_t parentFlags)
{
	uint32_t flags = parentFlags;
	flags |= (_contentChange ? FLAGS_ELEMENT_CHANGE : 0);
	//flags |= (_contentChange ? FLAGS_CONTENT_SIZE_DIRTY : 0);

	Vec2 tmp = Vec2::ZERO;

	if (flags && _parent != nullptr)
	{
		_render_position = _parent->_render_position + _position;
		
		auto parentDisplayedColor = _parent->getDisplayedColor();
		_displayedColor.r = _realColor.r * parentDisplayedColor.r / 255.0f;
		_displayedColor.g = _realColor.g * parentDisplayedColor.g / 255.0f;
		_displayedColor.b = _realColor.b * parentDisplayedColor.b / 255.0f;
		_displayedColor.a = _realColor.a * parentDisplayedColor.a / 255.0f;
		_rotate += _parent->getRotation();
		_visible = _parent->isVisible();
// 		_flipX = _parent->getFlipX();
// 		_flipX = _parent->getFlipY();
		//_pointer = _parent->getPointer();
	}
// 	else
// 		_render_position = _position + tmp;

	_contentChange = false;
	_contentSizeDirty = false;

	return flags;
}

// void Node::do_enumeration()
// {
// 	if (!_children.empty())
// 	{
// 	//	_render_position = _position;
// 		this->visit(flags);
// 	}
// }
void Node::visit(uint32_t parentFlags)
{
	// quick return if not visible. children won't be drawn.
	if (!_visible)
	{
		return;
	}

	int i = 0;

	uint32_t flags  = processParentFlags(parentFlags);

	if (!_children.empty())
	{
		sortAllChildren();
		// draw children zOrder < 0
		for (auto size = _children.size(); i < size; ++i)
		{
			auto node = _children.at(i);

			if (node && node->_localZOrder < 0)
				node->visit(flags);
			else
				break;
		}
// 		// self draw
// 		if (visibleByCamera)
        this->draw();

		for (auto it = _children.cbegin() + i, itCend = _children.cend(); it != itCend; ++it)
			(*it)->visit(flags);
	}
	else 
	{
		this->draw();
	}
 
	 
}

void Node::resume()
{
	//_update_ptr->resumeTarget(this);
	_actionManager->resumeTarget(this);
	_eventDispatcher->resumeEventListenersForTarget(this);
}

void Node::pause()
{
	//_update_ptr->pauseTarget(this);
	_actionManager->pauseTarget(this);
	_eventDispatcher->pauseEventListenersForTarget(this);
}
Action * Node::runAction(Action * action)
{
	_actionManager->addAction(action, this, !_running);
	return action;
}

void Node::stopAllActions()
{
	_actionManager->removeAllActionsFromTarget(this);
}
void Node::stopAction(Action* action)
{
	_actionManager->removeAction(action);
}

OG_END
