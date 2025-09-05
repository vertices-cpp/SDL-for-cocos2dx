#include "OGScene.h"
#include "OGDirector.h"

OG_BEGIN

Scene::Scene()
{
}


Scene::~Scene()
{

}



Scene* Scene::create(const std::string &fileName)
{
	Scene *ret = new (std::nothrow) Scene();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		OG_SAFE_DELETE(ret);
		return nullptr;
	}
}


bool Scene::init() {
	auto size = Director::getInstance()->getWinSize();
	setContentSize(size);
	setPosition(Vec2::ZERO);

	return true;
}




void Scene::draw()
{

}

OG_END
