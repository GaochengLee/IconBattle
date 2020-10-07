#ifndef __LEVELSCENE_SCENE_H__
#define __LEVELSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SettingLayer.h"
#include "user.h"
#include "util.h"
#include "LevelGameScene.h"

using std::string;
using namespace cocos2d::ui;

class LevelScene : public cocos2d::Scene
{

public:

	virtual bool init();
	virtual void onEnter();

	CREATE_FUNC(LevelScene);

private:

#pragma region CONST_STRING
	//bar
	const string PICTURE_BAR_PATH = "../Resources/texture/LevelScene/bar.png";
	//bar-return
	const string PICTURE_BAR_RETURN_0_PATH = "../Resources/texture/common/return_0.png";
	const string PICTURE_BAR_RETURN_1_PATH = "../Resources/texture/common/return_1.png";
	//bar-world[4]
	const string PICTURE_BAR_WORLD_N_1_PATH[4] = {
		"../Resources/texture/LevelScene/worlda1.png",
		"../Resources/texture/LevelScene/worldb1.png",
		"../Resources/texture/LevelScene/worldc1.png",
		"../Resources/texture/LevelScene/worldd1.png"
	};
	const string PICTURE_BAR_WORLD_N_2_PATH[4] = {
	"../Resources/texture/LevelScene/worlda2.png",
	"../Resources/texture/LevelScene/worldb2.png",
	"../Resources/texture/LevelScene/worldc2.png",
	"../Resources/texture/LevelScene/worldd2.png"
	};
	//btn-level[5]
	const string PICTURE_BTN_LEVEL_1_PATH = "../Resources/texture/LevelScene/btnLevel1.png";
	const string PICTURE_BTN_LEVEL_2_PATH = "../Resources/texture/LevelScene/btnLevel2.png";
	const string PICTURE_BTN_LEVEL_3_PATH = "../Resources/texture/LevelScene/btnLevel3.png";
	//world-back[4]
	const string PICTURE_WORLD_N_BACK_PATH[4] = {
		"../Resources/texture/LevelScene/worldaBak.png",
		"../Resources/texture/LevelScene/worldbBak.png",
		"../Resources/texture/LevelScene/worldcBak.png",
		"../Resources/texture/LevelScene/worlddBak.png"
	};

	const string PICTURE_STAR_PATH = "../Resources/texture/common/star.png";

	const string MUSIC_BGM = "../Resources/sound/music_level.mp3";
	const string VOICE_BGM = "../Resources/sound/splash.mp3";
	const string VOICE_SEL = "../Resources/sound/passby.mp3";
#pragma endregion

#pragma region NORMAL_NODE
	ImageView* PicBarBack;
	Button* BtnBarBack;
	RadioButtonGroup* RBGWorld;
	RadioButton* RadWorld[4];
	ImageView* PicWorldBack[4];
	Button* BtnLevel[5];
	Label* LabLevel[5];
	ImageView* PicStar[5][3];
#pragma endregion

#pragma region CALL_BACK

#pragma endregion

	int nowWorld  = -1;

	void initComponents();
	void changeToWorld(int);
	void enterLevel(int);
};

#endif // __LEVELSCENE_SCENE_H__
