#ifndef __SCORE_LAYER_H_
#define __SCORE_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "user.h"
#include  "util.h"
#include <vector>
#include <algorithm>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using std::string;
using std::vector;

struct PlayerRecord {
	ImageView* BacPlayer;
	ImageView* BacIcon;
	int playerScore;
	string playerName;
};

class ScoreLayer: public LayerColor {

public:
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;

	CREATE_FUNC(ScoreLayer);
	static ScoreLayer* create(int type);
	/*名字 排名 图标编号 分数*/
	void addPlayerRecord(string name, int record, int icon, int ranking);

private:
	/*
	 * 0：not show the index and id
	 * 1：show the index and id
	 */
	int type;

	void initComponents();

	void showLayer();

#pragma region NODE
	ImageView* PicBack;
	Button* BtnClose;
	vector<PlayerRecord*> playerGroup;
#pragma endregion

#pragma region CONST_STRING

	const string PICTURE_BACKGROUND_PATH = "../Resources/texture/ScoreLayer/rankList.png";

	const string BUTTON_CLOSE_0_PATH = "../Resources/texture/ScoreLayer/button_false_normal.png";
	const string BUTTON_CLOSE_1_PATH = "../Resources/texture/ScoreLayer/button_false_clicked.png";

	// player record
	const string PIC_PLAYER_RECORD_PATH = "../Resources/texture/ScoreLayer/player_record_temp.png";

	// player icon
	const string PIC_PLAYER_ICON_PATH[4] = {
		"../Resources/texture/common/play_icon_1.png",
		"../Resources/texture/common/play_icon_2.png",
		"../Resources/texture/common/play_icon_3.png",
		"../Resources/texture/common/play_icon_4.png"
	};

#pragma endregion
};

#endif // !__SCORE_LAYER_H_
