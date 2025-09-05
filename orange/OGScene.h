#ifndef _SCENE_H_
#define _SCENE_H_


#include "OGNode.h"


OG_BEGIN


class Scene: public Node
{

public:
	Scene();
	~Scene();

//	friend class Node;
// 
// 	using Node::addNode;

	static Scene * create(const std::string & fileName);

	bool init();


	void draw();



};

OG_END

#endif  //_SCENE_H_
