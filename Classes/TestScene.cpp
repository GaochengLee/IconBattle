#include "TestScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* TestScene::createScene()
{
	return TestScene::create();
}

bool TestScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	/* 创建一个精灵并且添加到场景中 */
	auto testSprite = Sprite::create("test.png");
	testSprite->setPosition(Vec2(300, 300));
	this->addChild(testSprite);
	
	return true;
}
