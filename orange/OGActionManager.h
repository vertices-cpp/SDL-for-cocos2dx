#ifndef _ACTION_MANAGER_H_
#define _ACTION_MANAGER_H_

#include "OGPlatformMacros.h"
#include "OGRef.h"


OG_BEGIN

class Action;

struct _hashElement;

// class Node;
// struct _actionNode;
// class Action;

class ActionManager :public Ref
{
protected:
	struct _actionNode    *_targets;
	void deleteNodeElement(_actionNode* element);
public:
	ActionManager();
	virtual ~ActionManager();

	virtual void addAction(Action *action, Node *target, bool paused);

	virtual void removeAllActions();

	virtual void removeAllActionsFromTarget(Node *target);

	virtual void removeAction(Action *action);

	virtual void pauseTarget(Node *target);

	virtual void resumeTarget(Node *target);

	virtual void pauseAllRunningActions();
	virtual void resumeTargets();
	virtual void update(float dt);

};


OG_END

#endif