#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"

class TestScene : public cocos2d::Scene
{
public:
	/* 静态创建一个场景对象 */
	static cocos2d::Scene* createScene();
	/* 初始化工作 */
	virtual bool init();

	/*使用宏 CREATE_FUNC 定义 create 函数 */
	// implement the "static create()" method manually
	CREATE_FUNC(TestScene);
};

#endif