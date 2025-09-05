#include "OGActionManager.h"

#include "ulist.h"
#include "OGNode.h"
#include "OGUpdate.h"
#include "OGAction.h"

OG_BEGIN


typedef struct _actionNode
{
	struct _actionNode * prev, *next;
	Node                *target;
	Action              *currentAction;
	bool                paused;

} actionNode, *PactionNode;


ActionManager::ActionManager()
	: _targets(nullptr)
{

}

ActionManager::~ActionManager()
{
	
	removeAllActions();
}

// protected

void ActionManager::deleteNodeElement(_actionNode* element)
{
	DL_DELETE(_targets, element);
	element->target->release();
	element->currentAction->release();
	delete element;
}



// pause / resume

void ActionManager::pauseTarget(Node *target)
{
	PactionNode element = nullptr;
	OG_FIND_NODE_TARGET(_targets, target, element);
	if (element)
	{
		element->paused = true;
	}
}

void ActionManager::resumeTarget(Node *target)
{
	PactionNode element = nullptr;
	OG_FIND_NODE_TARGET(_targets, target, element);
	if (element)
	{
		element->paused = false;
	}
}

void ActionManager::pauseAllRunningActions()
{
	for (PactionNode elem = _targets; elem != nullptr; elem = elem->next)
	{
		elem->paused = true;
	}
}

void ActionManager::resumeTargets()
{
	for (PactionNode elem = _targets;elem!=nullptr;elem=elem->next)
	{
		elem->paused = false;
	}
}


// run

void ActionManager::addAction(Action *action, Node *target, bool paused)
{

	if (action == nullptr || target == nullptr)
		return;

// 	uPAnNode element = nullptr;
// 	OG_FIND_NODE_TARGET(_targets, target, element);
// 
// 	if (element != nullptr)
// 		return;

	PactionNode NewNode = new _actionNode;
	NewNode->target = target;
	target->retain();

	NewNode->paused = paused;
	NewNode->currentAction = action;
	action->retain();

	DL_APPEND(_targets, NewNode);

	action->startWithTarget(target);
}

// remove

void ActionManager::removeAllActions()
{
	for (PactionNode element = _targets; element != nullptr; )
	{
		auto target = element->target;
		
		removeAllActionsFromTarget(target);
	}
}

void ActionManager::removeAllActionsFromTarget(Node *target)
{
	// explicit null handling
	if (target == nullptr)
	{
		return;
	}

	PactionNode element = nullptr;
	OG_FIND_NODE_TARGET(_targets, target, element);
	if (element)
	{
		deleteNodeElement(element);
	}
}

void ActionManager::removeAction(Action *action)
{
	// explicit null handling
	if (action == nullptr)
	{
		return;
	}

	PactionNode element = nullptr;

	//assert(action->getTarget(),"target is NULL");

	OG_FIND_NODE_ACTION(_targets, action, element);

	if (element)
	{
		deleteNodeElement(element);
	}
}


// main loop
void ActionManager::update(float dt)
{
	PactionNode _currentTarget = _targets, tmp = nullptr;
	while ((_currentTarget) && (tmp = _currentTarget->next, 1)) {


		if (_currentTarget->currentAction != nullptr &&  !_currentTarget->paused)
		{
			_currentTarget->currentAction->step(dt);

			if (_currentTarget->currentAction->isDone())
			{
				_currentTarget->currentAction->stop();
				removeAction(_currentTarget->currentAction);
			}
		}
		_currentTarget = tmp;
	}
}





OG_END