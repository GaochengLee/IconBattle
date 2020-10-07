#ifndef __SPLASHSCENE_SCENE_H__
#define __SPLASHSCENE_SCENE_H__

#include "cocos2d.h"
#include "MenuScene.h"
#include "SpecialGameScene.h"
#include "util.h"
#include "user.h"

using std::queue;
using std::pair;
using std::string;

using cocos2d::Sprite;
using cocos2d::Sequence;

class SplashScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual void onEnter();
	virtual bool init();
	CREATE_FUNC(SplashScene);

private:
	const string PICTURE_A_PATH = "../Resources/texture/SplashScene/welcome_page_1.png";
	const string PICTURE_B_PATH = "../Resources/texture/SplashScene/welcome_page_2.png";
	const string PICTURE_SPLASH_BACK_PATH = "../Resources/texture/SplashScene/background.png";
	const string PICTURE_TITLE_PATH = "../Resources/texture/SplashScene/title.png";

	const string PICTURE_SETTING_0_PATH = "../Resources/texture/common/setting_0.png";
	const string PICTURE_SETTING_1_PATH = "../Resources/texture/common/setting_1.png";
	
	const string PICTURE_CANCEL_BUTTON_NORMAL_PATH = "../Resources/texture/SettingLayer/button_false_normal.png";
	const string PICTURE_CANCEL_BUTTON_CLICKED_PATH = "../Resources/texture/SettingLayer/button_false_clicked.png";

	const string PICTURE_SETTING_BG_PATH = "../Resources/texture/SettingLayer/setting_scene.png";

#pragma region BGM
	const string MUSIC_WELCOME_PATH = "../Resources/sound/voice_welcomeback.mp3";
#pragma endregion

	void initComponents();
	void mainTask(float);
	void showPictureA();
	void showPictureB();
	void showSplash();

	void tsakFlashLable(float);

	void startButtonCallBack(Ref* pSender);
	void settingButtonCallBack(Ref* pSender);
	void settingSceneButtonCallBack(Node* pNode);

	bool movieDone = false;
};

#endif // __SPLASHSCENE_SCENE_H__
