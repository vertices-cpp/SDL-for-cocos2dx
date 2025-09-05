#ifndef _NODE_H_
#define _NODE_H_


#include "OGGeometry.h"
#include "OGRef.h"
#include "OGVector.h"
#include "Vec2.h"
#include "OGTypes.h"
//#include "OGType.hpp"

OG_BEGIN

//class Scene;
class Director;
class Update;
class ActionManager;
class EventDispatcher;

class Action;
class Scene;
struct Color4B;


class Node:public Ref
{

protected:
	
	Director* _director;

	Update *_update_ptr;

	ActionManager *_actionManager;  ///< a pointer to ActionManager singleton, which is used to handle all the actions

	EventDispatcher* _eventDispatcher;  ///< event dispatcher used to dispatch all kinds of events
	int _tag;
	bool _running;                  ///< is running

	bool _visible;                  ///< is this node visible

	Color4B _realColor;
	Color4B _displayedColor;
	//bool _modiflyColor;
	Vec2 _rotate;
	Vec2 _pointer;
	bool _flipX, _flipY;

	Vec2 _position;
	Size _contentSize;              ///< untransformed size of the node

	Vector<Node*> _children;
	Node *_parent;

	bool _reorderChildDirty;
	int _localZOrder;
	float _globalZOrder;
	//¾ØÐÎ
// 	V2F_C4C_QUAD _transform = { 0 };
// 	int indices[6] = { 0 };
//	bool _transformDirty;


	static int __attachedNodeCount;

	Rect _originalDestPosition;
public:

	//void setOriginalDestPos(const Vec2 &originalDestPosition) { _originalDestPosition = originalDestPosition; }
	void setOriginalDestPosition(const Rect &originalDestPosition) { _originalDestPosition = originalDestPosition; }
	Rect getOriginalDestPosition()const { return _originalDestPosition; }

	enum {
		FLAGS_ELEMENT_CHANGE = (1 << 0),
		FLAGS_CONTENT_SIZE_DIRTY = (1 << 1),
	//	FLAGS_RENDER_AS_3D = (1 << 3),

		FLAGS_DIRTY_MASK = (FLAGS_ELEMENT_CHANGE | FLAGS_CONTENT_SIZE_DIRTY),
	};

	static const int INVALID_TAG = -1;

	bool _contentChange;
	bool _contentSizeDirty;
	Vec2  _render_position;

	Node();
	Node * create();
	bool init();
	virtual ~Node();

	void cleanup();
	void detachChild(Node * child, size_t childIndex, bool doCleanup);
	void addChildHelper(Node * child, int localZOrder, int tag);
	void insertChild(Node * child, int localZOrder);
	void detachChild(Node * child, int childIndex);
 
	void childrenAlloc();

	virtual Vec2 getRotation() const;


	virtual void setRotation(float rotation);

	virtual float getRotationSkewX() const;

	virtual float getRotationSkewY() const;

	virtual void setRotationSkewX(float rotationX);

	virtual void setRotationSkewY(float rotationX);
	virtual Vec2 getPointer() { return _pointer; }
	virtual void setPointer(const Vec2 &point) {
		_pointer = point;
	}

	
	virtual  const Vec2 & getRenderPosition() const;
	virtual void setRenderPosition(const Vec2 & position);
	virtual  const Vec2 & getPosition() const;
 
	virtual void setPosition(const Vec2 & position);
	 
	virtual void setPosition(float x, float y);

	virtual Color4B getColor() { return _realColor; }
	virtual Color4B getDisplayedColor() { return _displayedColor; }
	virtual void setColor(Color4B color) { 
		_displayedColor = _realColor = color; _contentChange = true;
	}
	virtual void setColor(Color3B color) {
		_displayedColor = _realColor = color; _contentChange = true;
	}
	
	uint8_t getOpacity() {
		return   _realColor.a;
	}
	void  setOpacity(uint8_t opacity)
	{
		_displayedColor.a = _realColor.a = opacity;
		_contentChange = _reorderChildDirty = true;
	//	updateCascadeOpacity();
	}

	virtual float getPositionX() const;
	virtual void setPositionX(float x);
	virtual float getPositionY() const;
	virtual void setPositionY(float y);
	
	virtual long getChildrenCount() const;

	virtual bool isVisible() const;

	virtual void setVisible(bool visible);

	virtual void setFlipX(bool flipX);

	virtual void setFlipY(bool flipY);

	virtual bool getFlipX();

	virtual bool getFlipY();

	

// 	virtual bool isFlippedX() const { return _flipX; }
// 	virtual void setFlippedX(bool flippedX) { _flipX = flippedX; }
// 	virtual bool isFlippedY() const { return _flipY; }
// 	virtual void setFlippedY(bool flippedY) { _flipY = flippedY; }
	
 	virtual void setParent(Node * parent);
	virtual Node* getParent() { 
		return _parent;
	}
	virtual const Node* getParent() const {
		return _parent;
	}

	virtual const Size & getContentSize() const;

	virtual void setContentSize(const Size & size);

	virtual bool isRunning() const;
	virtual Vector<Node*>& getChildren() { return _children; }
	
 

	virtual int getTag() const;
	virtual void setTag(int tag);

	virtual Node * getChildByTag(int tag) const;

	virtual Scene * getScene() const;

	virtual void addChild(Node * child);
	virtual void addChild(Node * child, int localZOrder);
	virtual void addChild(Node * child, int localZOrder, int tag);

	
	virtual void sortAllChildren();
	virtual void setLocalZOrder(int z);
	virtual float getGlobalZOrder() const;
	virtual void setGlobalZOrder(float globalZOrder);
	virtual void reorderChild(Node * child, int zOrder);
	virtual int getLocalZOrder() const { return _localZOrder; }
private:
	virtual void _setLocalZOrder(int z);
	
public:
	virtual void onEnter();
	virtual void onExit();

	virtual void removeFromParentAndCleanup(bool cleanup);
	virtual void removeChild(Node * child, bool cleanup = true);

	virtual void removeChildByTag(int tag, bool cleanup = true);

	

	 

	

	 virtual void removeAllChildren();

	 virtual void removeAllChildrenWithCleanup();
	
	

	

	 virtual void setUpdate();
	 virtual void setNotUpdate();



	virtual void update(float delta);
	virtual void draw();
	//void processParentFlags();
	uint32_t processParentFlags(uint32_t parentFlags);
	//void do_enumeration();
	//void processParentFlags();
//	void visit();
	virtual void visit(uint32_t parentFlags);
	virtual void resume();
 
	virtual void pause();

	virtual Action * runAction(Action* action);
	void stopAllActions();

	void stopAction(Action * action);


	
	template<typename _T> inline
		static void sortNodes(orange::Vector<_T*>& nodes)
	{
		static_assert(std::is_base_of<Node, _T>::value, "Node::sortNodes: Only accept derived of Node!");
//#if defined(_M_X64) || defined(_WIN64) || defined(__LP64__) || defined(_LP64) || defined(__x86_64) || defined(__arm64__) || defined(__aarch64__)
		std::sort(std::begin(nodes), std::end(nodes), [](_T* n1, _T* n2) {
			return (n1->_localZOrder < n2->_localZOrder);
		});
// #else
// 		std::sort(std::begin(nodes), std::end(nodes), [](_T* n1, _T* n2) {
// 			return (n1->_localZOrder == n2->_localZOrder && n1->_orderOfArrival < n2->_orderOfArrival) || n1->_localZOrder < n2->_localZOrder;
// 		});
//#endif
	}

private:

		Node(const Node &) = delete;
		Node &operator =(const Node &) = delete;
	
};

OG_END


#endif