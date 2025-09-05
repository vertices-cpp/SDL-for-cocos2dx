#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "ulist.h"
#include "OGRef.h"

OG_BEGIN


typedef std::function<void(float)> callFunc;

typedef struct _uNode
{
	struct _uNode   *prev, *next;
	void * target;
	callFunc     callback;

} uNode, *uPNode;


class Update :public Ref{

	uPNode _updates0List;
public:
	Instance(Update);

	Update() { _updates0List = nullptr; }
	~Update(){
		OG_FREE_UPDATE_CALLBACK(_updates0List);
	}
	template<typename T>
	void setUpdate(T *target)
	{

		this->bindCallBack([target](float dt) {
				target->update(dt);
		},target);

	}

	void bindCallBack(std::function<void(float)> call,void *target)
	{
		uPNode element = nullptr;
		OG_FIND_CALLBACK(_updates0List, target, element);

		if (element != nullptr)
			return;

		uPNode NewNode = new uNode;
		NewNode->target = target;
		NewNode->callback = call;

		DL_APPEND(_updates0List,NewNode);

	//	addCallback(&_updates0List, target, call);
	}
	template<typename T>
	void setNullUpdate(T *target)
	{
		if (_updates0List == nullptr)
			return;

		uPNode element = nullptr;
		OG_FIND_CALLBACK(_updates0List, target, element);

		if (element == nullptr)
			return;

		DL_DELETE(_updates0List, element);

		delete element;
		//delCallback(&_updates0List, target);
	}
	void update(float delta) {
		if (_updates0List != nullptr)
			OG_TRAVEROG_UPDATE(_updates0List, delta);
	}

	
};

OG_END

#endif