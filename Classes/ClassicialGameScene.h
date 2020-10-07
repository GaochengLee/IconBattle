#ifndef __CLASSICIALGAMESCENE_SCENE_H__
#define __CLASSICIALGAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SettingLayer.h"
#include "QuitEnsureLayer.h"
#include "EndGameLayer.h"
#include "ScoreLayer.h"
#include "user.h"
#include "myGame.h"
#include "PNDelegate.h"
#include <queue>
#include <vector>
#include <set>
#include <string>

using std::string;
using std::pair;
using std::queue;
using std::vector;
using std::set;
using std::swap;
using namespace cocos2d::ui;
using namespace myGame;

class ClassicialGameScene : public cocos2d::Scene
{
public:
	bool init() override;
	void onEnter() override;
	void onExit() override;
	CREATE_FUNC(ClassicialGameScene);
	static void runCommand(ClassicialGameScene* s, string command) ;
private:

	static const int BOARD_SIZE = 10;
	static const int SWAP_COUNT = 25;
	
	bool boardLocked = true;

	int nowScore = 0;
	int leftSwapTime = 30;

	pii selectedBlock = {-1,-1};
	blockInfo boardInfo[BOARD_SIZE * 2][BOARD_SIZE];
	void initComponents();
	void settingSaveCallBack(Node* pNode);
	void backButtonCallBack(Node* pNode);

	bool boardOperatable() { return !(boardLocked); };

	//�±�ת��Ļ����(����)
	pii getPosition(pii);
	//��Ļ����ת�±�(����)
	pii getIndex(pii);
	//������ĳ���꣬����Ϊĳ,��������Ϊĳ�ľ���
	Actor* createActor(int,int,pii);

	//��ȡ�������б�pair<posion,pair<spontype,sponid>> ����������Ҫ������������block�����second
	KillGroupList getKillList();
	//�ж��Ƿ������
	bool getKillable();
	//��ȡ��ʾ�б�
	validOperateList getHitList();

	//���Խ���������,������ʵ�ʶ������ַ���FailSwap/RealSwap
	void trySwap(pii, pii);
	

	//��ʼ������
	void initBoard();
	//ˢ�°��棬�ϰ�BFS��䣬�°�-1
	void refreshBoard();

	//��������
	void doDrop();
	//�������������ⷽ��
	void doKill(KillGroupList);
	//[UNLOCK]������Ч��������
	void doFailSwap(pii,pii);
	//���н����������ص���activeDoneCallback
	void doRealSwap(pii,pii);
	//[UNLOCK]�������������������������������������dokill.<promise>[doKill->doDrop->activeDoneCallback]
	void activeDoneCallback();

	//��piiλ��չʾ��������
	void showScore(int,pii);
	//��ӷ���
	void addScore(int);
	//ˢ����ʾ
	void updateSwapTime();

	//�������
	void showThunder_H(int);
	//�������
	void showThunder_V(int);
	//3*3����
	void showFire(pii);
	//SUPER
	void showMelt(pii, int);
	//circle
	void showCircle(pii);


#pragma region SettingScene
	const string PICTURE_SAVE_BUTTON_NORMAL_PATH = "../Resources/texture/SettingLayer/button_right_normal.png";
	const string PICTURE_SAVE_BUTTON_CLICKED_PATH = "../Resources/texture/SettingLayer/button_right_clicked.png";
	const string PICTURE_CANCEL_BUTTON_NORMAL_PATH = "../Resources/texture/SettingLayer/button_false_normal.png";
	const string PICTURE_CANCEL_BUTTON_CLICKED_PATH = "../Resources/texture/SettingLayer/button_false_clicked.png";
	const string PICTURE_SETTING_SCENE_BACKGROUND_PATH = "../Resources/texture/SettingLayer/setting_scene.png";
#pragma endregion

#pragma region CONST_STRING
	//bar
	const string PICTURE_BAR_PATH = "../Resources/texture/GameScene/bar.png";
	// button back
	const string PICTURE_BUTTON_BACK_NORMAL_PATH = "../Resources/texture/GameScene/button_back_normal.png";
	const string PICTURE_BUTTON_BACK_CLICKED_PATH = "../Resources/texture/GameScene/button_back_clicked.png";
	// button back waring
	const string PICTURE_BACK_SCENE_PATH = "../Resources/texture/GameScene/waringScene.png";
	// button setting
	const string PICTURE_BUTTON_SETTING_1_PATH = "../Resources/texture/GameScene/button_setting_normal.png";
	const string PICTURE_BUTTON_SETTING_2_PATH = "../Resources/texture/GameScene/button_setting_clicked.png";
	// button rank list
	const string PICTURE_BUTTON_RANK_LIST_NORMAL_PATH = "../Resources/texture/GameScene/button_rank_list_normal.png";
	const string PICTURE_BUTTON_RANK_LIST_CLICKED_PATH = "../Resources/texture/GameScene/button_rank_list_clicked.png";
	// button prompt
	const string PICTURE_BUTTON_PROMPT_NORMAL_PATH = "../Resources/texture/GameScene/button_prompt_normal.png";
	const string PICTURE_BUTTON_PROMPT_CLICKED_PATH = "../Resources/texture/GameScene/button_prompt_clicked.png";
	// slider bar score
	const string PICTURE_SLIDERBAR_BACK_PATH = "../Resources/texture/GameScene/score_back.png";
	const string PICTURE_SLIDERBAR_PRESS_PATH = "../Resources/texture/GameScene/score_pressBar.png";
	const string PICTURE_SLIDERNODE_NORMAL_PATH = "../Resources/texture/GameScene/sliderNode_normal.png";
	const string PICTURE_SLIDERNODE_PRESS_PATH = "../Resources/texture/GameScene/sliderNode_press.png";
	const string PICTURE_SLIDERNODE_DISABLE_PATH = "../Resources/texture/GameScene/sliderNode_disable.png";
	// edit box bac
	const string PICTURE_EDITBOX_BACKGROUND_PATH = "../Resources/texture/MenuScene/player_name_background.png";
	

	//board
	const string PICTURE_BOARD_PATH = "../Resources/texture/GameScene/board_new.png";
	//cover
	const string PICTURE_COVER_PATH = "../Resources/texture/GameScene/game_background_new.png";

	//actors
	const string PLIST_ACTOR_PATH = "../Resources/texture/GameScene/gems.plist";
	const string PPNG_ACTOR_PATH = "../Resources/texture/GameScene/gems.png";
	const string PLIST_ACTOR_ADDITION_PATH = "../Resources/texture/GameScene/super_gems.plist";
	const string PPNG_ACTOR_ADDITION_PATH = "../Resources/texture/GameScene/super_gems.png";

	const string PICTURE_ACTOR_HIGHLIGHT = "../Resources/texture/GameScene/highlight.png";

	// Special effects
	const string SPECIAL_EFFECT_THUNDER = "../Resources/texture/GameScene/thunder.plist";
	const string SPECIAL_EFFECT_FIRE = "../Resources/texture/GameScene/fire_effect.plist";
	const string SPECIAL_EFFECT_MELT_N[7] = {
		"../Resources/texture/GameScene/super_0.plist",
		"../Resources/texture/GameScene/super_1.plist",
		"../Resources/texture/GameScene/super_2.plist",
		"../Resources/texture/GameScene/super_3.plist",
		"../Resources/texture/GameScene/super_4.plist",
		"../Resources/texture/GameScene/super_5.plist",
		"../Resources/texture/GameScene/super_6.plist"
	};
	const string SPECIAL_EFFECT_CIRCLE = "../Resources/texture/GameScene/circle.plist";

	const string AUDIO_FIRE = "../Resources/sound/fire.mp3";
	const string AUDIO_THUNDER = "../Resources/sound/thunder.mp3";

#pragma endregion

#pragma region NORMAL_NODE
	ImageView* PicBarBack;
	ImageView* PicBoard;
	ImageView* PicCover;
	Button* BtnBarBack;
	Slider* SliderScore;
	Label* LabSwap;
	Label* LabScore;
	Button* BtnSetting;
	Button* BtnRankList;
	Button* BtnPrompt;
	//ui::EditBox* EditBoxPlayerName;
	ui::EditBox* EditBoxConsole;
	ImageView* PicActorHighLight;
	Label* LabTitle;
#pragma endregion

#pragma region CALL_BACK
	void SettingCallBack(Ref*);
	void RankCallBack(Ref*);
	void PromptCallBack(Ref*);
#pragma endregion


};


#endif // __CLASSICIALGAMESCENE_SCENE_H__
