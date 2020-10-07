#ifndef __END_GAME_LAYER_H_
#define __END_GAME_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include <ScoreLayer.h>
#include  "util.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

using std::string;

class EndGameLayer :public LayerColor {

public:

	static const int TYPE_CLASSICAL = 0;
	static const int TYPE_SPECIAL = 1;
	static const int TYPE_LEVEL = 2;

	virtual bool init();
	CREATE_FUNC(EndGameLayer);
	static EndGameLayer* create(int,int,int,int = 3);
	virtual void onEnter();

	void setRestartCallBack(const ui::Widget::ccWidgetClickCallback& callback);

private:

	int gold;
	int score;
	int star;
	int type;

	void initComponents();
	void doRecord();

	void showScore();
	void showMoney();
	void showStar();
	void showLayer();

#pragma region CONST_STRING

	const string PICTURE_BACKGROUND_PATH = "../Resources/texture/EndGameLayer/back.png";

	const string PICTURE_BTN_BACK_0_PATH = "../Resources/texture/common/return_0.png";
	const string PICTURE_BTN_BACK_1_PATH = "../Resources/texture/common/return_1.png";

	const string PICTURE_BTN_RANK_0_PATH = "../Resources/texture/common/rank_0.png";
	const string PICTURE_BTN_RANK_1_PATH = "../Resources/texture/common/rank_1.png";

	const string PICTURE_BTN_RESTART_0_PATH = "../Resources/texture/common/restart_0.png";
	const string PICTURE_BTN_RESTART_1_PATH = "../Resources/texture/common/restart_1.png";

	const string PICTURE_STAR_PATH = "../Resources/texture/common/star.png";


#pragma endregion

#pragma region NORMAL_NODE
	ImageView* PicBack;
	Button* BtnBack;
	Button* BtnRank;
	Button* BtnRestart;
	Label* LabMoney;
	Label* LabScore;
	ImageView* PicStars[3];
#pragma endregion

#pragma region CALL_BACK

#pragma endregion

};

#endif // ! __END_GAME_LAYER_H_
