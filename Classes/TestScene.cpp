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

	/* ����һ�����鲢����ӵ������� */
	auto testSprite = Sprite::create("test.png");
	testSprite->setPosition(Vec2(300, 300));
	this->addChild(testSprite);
	
	return true;
}
