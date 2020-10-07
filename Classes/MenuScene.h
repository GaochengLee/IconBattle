#ifndef __MENUSCENE_SCENE_H__
#define __MENUSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "LevelScene.h"
#include "LevelGameScene.h"
#include "SpecialGameScene.h"
#include "ClassicialGameScene.h"
#include "SettingLayer.h"
#include "user.h"
#include "util.h"
#include "PNDelegate.h"

using std::string;

class MenuScene : public cocos2d::Scene{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(MenuScene);

	void changeCharacter(int);

private:

	bool inArae[3] = {};

#pragma region SettingScene
	//const string PICTURE_SAVE_BUTTON_NORMAL_PATH = "../Resources/texture/SettingLayer/button_right_normal.png";
	//const string PICTURE_SAVE_BUTTON_CLICKED_PATH = "../Resources/texture/SettingLayer/button_right_clicked.png";
	const string PICTURE_CANCEL_BUTTON_NORMAL_PATH = "../Resources/texture/SettingLayer/button_false_normal.png";
	const string PICTURE_CANCEL_BUTTON_CLICKED_PATH = "../Resources/texture/SettingLayer/button_false_clicked.png";
	const string PICTURE_SETTING_SCENE_BACKGROUND_PATH = "../Resources/texture/SettingLayer/setting_scene.png";
#pragma endregion
	
#pragma region MenuScene
	const string PICTURE_MENU_BACKGROUND_PATH = "../Resources/texture/MenuScene/background.png";
	//const string PICTURE_TITLE_BAR_PATH = "../Resources/texture/MenuScene/title_bar_t0.png";
	//const string PICTURE_COIN_PATH = "../Resources/texture/MenuScene/coin.png";

	const string PICTURE_PLAYER_BOX_PATH = "../Resources/texture/MenuScene/player_info.png";

	const string PICTURE_SETTING_0_PATH = "../Resources/texture/common/setting_0.png";
	const string PICTURE_SETTING_1_PATH = "../Resources/texture/common/setting_1.png";

	const string PICTURE_PLAYER_PAINT_PATH_N[4] = {
		"../Resources/texture/MenuScene/player_icon_1.png",
		"../Resources/texture/MenuScene/player_icon_2.png",
		"../Resources/texture/MenuScene/player_icon_3.png",
		"../Resources/texture/MenuScene/player_icon_4.png"
	};
	/*
	const string PICTURE_LEVEL_GAME_PATH = "../Resources/texture/MenuScene/level_game.png";
	const string PICTURE_CLASSICAL_GAME_PATH = "../Resources/texture/MenuScene/classical_game.png";
	const string PICTURE_SPECIAL_GAME_PATH = "../Resources/texture/MenuScene/special_game.png";
	*/
	const string PICTURE_LEVEL_GAME_0_PATH = "../Resources/texture/MenuScene/level_game_t0.png";
	const string PICTURE_LEVEL_GAME_1_PATH = "../Resources/texture/MenuScene/level_game_t1.png";
	const string PICTURE_CLASSICAL_GAME_0_PATH = "../Resources/texture/MenuScene/classical_game_t0.png";
	const string PICTURE_CLASSICAL_GAME_1_PATH = "../Resources/texture/MenuScene/classical_game_t1.png";
	const string PICTURE_SPECIAL_GAME_0_PATH = "../Resources/texture/MenuScene/special_game_t0.png";
	const string PICTURE_SPECIAL_GAME_1_PATH = "../Resources/texture/MenuScene/special_game_t1.png";

	const string PICTURE_HEAD_PATH = "../Resources/texture/MenuScene/user.png";

	const string SPECIAL_EFFECT_MELT_N[7] = {
		"../Resources/texture/MenuScene/super_0.plist",
		"../Resources/texture/MenuScene/super_1.plist",
		"../Resources/texture/MenuScene/super_2.plist",
		"../Resources/texture/MenuScene/super_3.plist",
		"../Resources/texture/MenuScene/super_4.plist",
		"../Resources/texture/MenuScene/super_5.plist"
	};
	const string SPECIAL_EFFECT_THUNDER = "../Resources/texture/MenuScene/thunderb.plist";
	const string SPECIAL_EFFECT_CIRCLE = "../Resources/texture/MenuScene/circle.plist";

	const string MUSIC_BACKGROUND_PATH = "../Resources/sound/menu_background_ice.mp3";
#pragma endregion

	ParticleSystemQuad* effFire_A[2];
	ParticleSystemQuad* effFire_B[6];
	ParticleSystemQuad* effFire_C;
	ImageView* imageView_player;

	void initComponents();

	

	void playerImageViewCallBack(Ref* pSender);
	void levelSceneButtonCallBack(Ref* pSender);
	void classicialSceneButtonCallBack(Ref* pSender);
	void specialSceneButtonCallBack(Ref* pSender);
	void settingButtonCallBack(Ref* pSender);
	void settingSceneButtonCallBack(Node* pNode);

	void MoveinBtnLevel();
	void MoveoutBtnLevel();
	void MoveinBtnClassical();
	void MoveoutBtnClassical();
	void MoveinBtnSpecial();
	void MoveoutBtnSpecial();

};

#endif // __MENUSCENE_SCENE_H__
