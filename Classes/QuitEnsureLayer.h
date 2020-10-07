#ifndef __QUIT_ENSURE_LAYER_H_
#define __QUIT_ENSURE_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include <ScoreLayer.h>
#include  "util.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

using std::string;

class QuitEnsureLayer :public LayerColor {

public:

	static const int TYPE_CLASSICAL = 0;
	static const int TYPE_SPECIAL = 1;
	static const int TYPE_LEVEL = 2;

	virtual bool init();
	CREATE_FUNC(QuitEnsureLayer);

private:

	void initComponents();

	void showLayer();

#pragma region CONST_STRING

	const string PICTURE_BACKGROUND_PATH = "../Resources/texture/GameScene/waringScene.png";

	const string PICTURE_BTN_YES_0_PATH = "../Resources/texture/common/yes_0.png";
	const string PICTURE_BTN_YES_1_PATH = "../Resources/texture/common/yes_1.png";

	const string PICTURE_BTN_NO_0_PATH = "../Resources/texture/common/no_0.png";
	const string PICTURE_BTN_NO_1_PATH = "../Resources/texture/common/no_1.png";

#pragma endregion

#pragma region NORMAL_NODE
	ImageView* PicBack;
	Button* BtnYes;
	Button* BtnNo;
#pragma endregion

#pragma region CALL_BACK

#pragma endregion

};

#endif // ! __QUIT_ENSURE_LAYER_H_
