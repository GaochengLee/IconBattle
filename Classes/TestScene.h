#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"

class TestScene : public cocos2d::Scene
{
public:
	/* ��̬����һ���������� */
	static cocos2d::Scene* createScene();
	/* ��ʼ������ */
	virtual bool init();

	/*ʹ�ú� CREATE_FUNC ���� create ���� */
	// implement the "static create()" method manually
	CREATE_FUNC(TestScene);
};

#endif