#ifndef __WELCOMESCENE_H_
#define __WELCOMESCENE_H_

#include "cocos2d.h"
#include "SplashScene.h"

class welcomeScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(welcomeScene);
};

#endif // __WELCOMESCENE_H_