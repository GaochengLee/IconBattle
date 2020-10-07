#include "welcomeScene.h"

USING_NS_CC;

Scene* welcomeScene::createScene()
{
	return welcomeScene::create();
}

bool welcomeScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto texture = Sprite::create("welcome_page_2.png");
	texture->setPosition(640, 360);
	this->addChild(texture, 0);

	return true;
}
