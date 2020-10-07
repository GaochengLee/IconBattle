#ifndef __SETTING_LAYER_H_
#define __SETTING_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "util.h"
#include "user.h"

USING_NS_CC;
USING_NS_CC_EXT;
using std::string;

class SettingLayer : public LayerColor
{
public:
	SettingLayer();
	~SettingLayer();

	virtual bool init();
	CREATE_FUNC(SettingLayer);

	static SettingLayer *create(const char* backgroundImage, Size contentSize);

	void setTitle(const char* title, int fontSize = 20);
	void setContentText(const char* text, int padding = 50, int paddingTop = 50);
	void setCallbackFunc(Ref* target, SEL_CallFuncN callfun);

	/*bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);*/
	bool addButton(const char* normalImage, const char* selectedImage, int tag = 0);

	virtual void onEnter();
	virtual void onExit();
	void backgroundFinish();

private:

#pragma region picture
	const string PICTURE_MUSIC_NORMAL_PATH = "../Resources/texture/SettingLayer/button_music_normal.png";
	const string PICTURE_MUSIC_MUTE_PATH = "../Resources/texture/SettingLayer/button_music_mute.png";
	const string PICTURE_EFFECT_NORMAL_PATH = "../Resources/texture/SettingLayer/button_effect_normal.png";
	const string PICTURE_EFFECT_MUTE_PATH = "../Resources/texture/SettingLayer/button_effect_mute.png";
	const string PICTURE_SLIDERBAR_BACK_PATH = "../Resources/texture/SettingLayer/slider_back.png";
	const string PICTURE_SLIDERBAR_PRESS_PATH = "../Resources/texture/SettingLayer/slider_pressBar.png";
	const string PICTURE_SLIDERNODE_NORMAL_PATH = "../Resources/texture/SettingLayer/sliderNode_normal.png";
	const string PICTURE_SLIDERNODE_PRESS_PATH = "../Resources/texture/SettingLayer/sliderNode_press.png";
	const string PICTURE_SLIDERNODE_DISABLE_PATH = "../Resources/texture/SettingLayer/sliderNode_disable.png";
	const string PICTURE_RADIO_BLUE_PATH = "../Resources/stone_blue.png";
	const string PICTURE_RADIO_GREEN_PATH = "../Resources/stone_green.png";
#pragma endregion

#pragma region music
	const string MUSIC_BACKGROUND_PATH = "../Resources/sound/menu_background_ice.mp3";
#pragma endregion

#pragma region NORMAL_NODE
	ui::CheckBox* checkBox_musicMute;
	ui::CheckBox* checkBox_effectMute;
	ui::Slider* slider_sound_mainBGM;
	ui::Slider* slider_sound_effectBGM;

	ui::RadioButtonGroup* RBG_player_picture;
	ui::RadioButton* RadPlayerPic[4];
	ui::RadioButtonGroup* RBG_main_BGM;
	ui::RadioButton* RadMainBGM[4];
#pragma endregion

	void buttonCallBack(Ref* pSender);
	void closeSettingTask(float);

	int m_contentPadding;
	int m_contentPaddingTop;
	Size m_dialogContentSize;

	Ref* m_callbackListener;
	SEL_CallFuncN m_callback;

	
	//定义了CCMenu*类型变量m_pMenu, 并且直接定义默认的set/get方法
	CC_SYNTHESIZE_RETAIN(Menu*, m_pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, m_sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(ui::Scale9Sprite*, m_s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(Label*, m_ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(Label*, m_ltContentText, LabelContentText);
};

#endif // ! __SETTING_LAYER_H_
