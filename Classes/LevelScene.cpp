#include "LevelScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

bool LevelScene::init(){

	if (!Scene::init()){
		return false;
	}

	initComponents();
	changeToWorld(0);

	return true;
}

void LevelScene::onEnter() {
	Scene::onEnter();

	Util::mycocos::audioWrapper.playBack(MUSIC_BGM);
	
	changeToWorld(nowWorld);
}

void LevelScene::initComponents() {

#pragma region PicBarBack, 100 
	{
		PicBarBack = ImageView::create(PICTURE_BAR_PATH);
		PicBarBack->setPosition({ 0,720 });
		PicBarBack->setAnchorPoint({ 0,1 });
		PicBarBack->ignoreContentAdaptWithSize(false);
		PicBarBack->setSize({ 1280,50 });
		PicBarBack->setZOrder(10);
		PicBarBack->setTag(100);
		addChild(PicBarBack, 10);
	}
#pragma endregion
#pragma region BtnBarBack, 101
	{
		BtnBarBack = Button::create(PICTURE_BAR_RETURN_0_PATH, PICTURE_BAR_RETURN_1_PATH);
		BtnBarBack->setScale(0.25f);
		BtnBarBack->setPosition({ -10,720 + 10 });
		BtnBarBack->setAnchorPoint({ 0,1 });
		BtnBarBack->setTag(101);
		addChild(BtnBarBack,11);
		BtnBarBack->addClickEventListener([&](Ref*) {Director::getInstance()->popScene(); });
	}
#pragma endregion
#pragma region RBGWorld, 110
	{
		RBGWorld = RadioButtonGroup::create();
		RBGWorld->setTag(110);
		addChild(RBGWorld, 11);
	}
#pragma endregion
#pragma region RadWorld[4], 111-114
	{
		for (int i = 0; i < 4; i++) {
			RadWorld[i] = RadioButton::create(PICTURE_BAR_WORLD_N_1_PATH[i], PICTURE_BAR_WORLD_N_2_PATH[i]);
			RadWorld[i]->setPosition({ static_cast<float>(760 + i * 130) ,720 });
			RadWorld[i]->setAnchorPoint({ 0,1 });
			RadWorld[i]->setTag(111+i);
			RadWorld[i]->addEventListener([=](Ref*, RadioButton::EventType t) {
				if (t == RadioButton::EventType::UNSELECTED)return;
				Util::mycocos::audioWrapper.playEffc(VOICE_SEL);
				//SimpleAudioEngine::getInstance()->playEffect(VOICE_SEL.c_str());
				changeToWorld(i);
				});
			addChild(RadWorld[i], 11);
			RBGWorld->addRadioButton(RadWorld[i]);
		}
	}
#pragma endregion
#pragma region BtnLevel[5], 201-205
	{
		for (int i = 0; i < 5; i++) {
			BtnLevel[i] = Button::create(PICTURE_BTN_LEVEL_1_PATH, PICTURE_BTN_LEVEL_2_PATH, PICTURE_BTN_LEVEL_3_PATH);
			BtnLevel[i]->setPosition({ static_cast<float>(220 + 200 * i), static_cast<float>(250 + 300 * (i % 2)) });
			BtnLevel[i]->setTag(201 + i);
			BtnLevel[i]->setScale(0.4);
			BtnLevel[i]->addClickEventListener([=](Ref*) {
				enterLevel(i);
				});
			addChild(BtnLevel[i], 1);
		}
	}
#pragma endregion
#pragma region LabLevel[5], 211-215
	{
		for (int i = 0; i < 5; i++) {
			LabLevel[i] = Label::create(std::to_string(i + 1), "fonts/Marker Felt.ttf", 48);
			LabLevel[i]->setPosition({ static_cast<float>(220 + 200 * i), static_cast<float>(280 + 300 * (i % 2)) });
			LabLevel[i]->setTextColor({ 255,255,255,255 });
			LabLevel[i]->enableOutline({ 0,0,0,255 }, 3);
			LabLevel[i]->setTag(211 + i);
			addChild(LabLevel[i], 2);
		}
	}
#pragma endregion
#pragma region PicStar[5][3], NULL
	{
		for (int i = 0; i < 5; i++) {
			int dx[] = { -34,0,34 };
			int dy[] = { -93,-81,-93 };
			float scale[] = {0.18,0.25,0.18};
			for (int j = 0; j < 3;j++) {
				PicStar[i][j] = ImageView::create(PICTURE_STAR_PATH);
				PicStar[i][j]->setPosition({ static_cast<float>(220 + 200 * i + dx[j]), static_cast<float>(280 + 300 * (i % 2) + dy[j]) });
				PicStar[i][j]->setScale(scale[j]);
				addChild(PicStar[i][j], 3);
			}
		}
	}
#pragma endregion
#pragma region PicWorldBack, 211-214
	{
		for (int i = 0; i < 4; i++) {
			PicWorldBack[i] = ImageView::create(PICTURE_WORLD_N_BACK_PATH[i]);
			PicWorldBack[i]->setPosition({ 640,360 });
			PicWorldBack[i]->setTag(211+i);
			addChild(PicWorldBack[i], 0);
		}
	}
#pragma endregion
}

void LevelScene::changeToWorld(int worldID){
	//if (worldID == nowWorld)return;
	//TODO: change to worldID
	CCLOG("LevelScene: Change world to %d",worldID);
	nowWorld = worldID;
	for (int i = 0; i < 5;i++) {
		BtnLevel[i]->setEnabled(User::data.levelState[nowWorld][i] != -1);
		for (int j = 0; j < 3;j++) {
			PicStar[i][j]->setVisible(User::data.levelState[nowWorld][i] > j);
		}
	}
	for (int i = 0; i < 4;i++) {
		PicWorldBack[i]->setVisible(i==worldID);
	}
}

void LevelScene::enterLevel(int levelId) {
	CCLOG("LevelScene: Enter Level (%d,%d) s(%d,%d)", nowWorld + 1 , levelId + 1, nowWorld,levelId);
	User::para["LEVEL_GO_LEVEL"] = levelId;
	User::para["LEVEL_GO_WORLD"] = nowWorld;
	User::gameTitle = "Level Game " + std::to_string(nowWorld + 1) + "-" + std::to_string(levelId + 1);

	Director::getInstance()->pushScene(LevelGameScene::create());
}
