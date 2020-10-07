#include "ClassicialGameScene.h"

USING_NS_CC;

bool ClassicialGameScene::init() {
	if (!Scene::init()) {
		return false;
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_ACTOR_PATH, PPNG_ACTOR_PATH);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_ACTOR_ADDITION_PATH, PPNG_ACTOR_ADDITION_PATH);
	initComponents();
	
	//addScore(0);
	//updateSwapTime();
	initBoard();
	doDrop();

	return true;
}

void ClassicialGameScene::onEnter() {
	Scene::onEnter();
	User::inGame = true;
	
	Util::mycocos::audioWrapper.playBack(Util::mycocos::audioWrapper.BGM_LIST[User::data.BGMID]);
	LabTitle->setString(User::gameTitle);
}

void ClassicialGameScene::onExit() {
	Scene::onExit();
	User::inGame = false;
}

void ClassicialGameScene::runCommand(ClassicialGameScene* s, string command) {
	CCLOG("RunCommand: %s",command.c_str());
	std::stringstream ss(command);

	string op;
	vector<int> nums(10,-999);
	ss >> op;
	for (int i = 0; i < 10; i++)ss >> nums[i];

	if (op == "change") {
		if (nums[0] < 0 || nums[0] >= BOARD_SIZE)return;
		if (nums[1] < 0 || nums[1] >= BOARD_SIZE)return;
		if (nums[2] < 0 || nums[2] >= 6)return;
		if (nums[3] < -1 || nums[3] >= 6)return;
		s->removeChild(s->boardInfo[nums[0]][nums[1]].actor);
		s->boardInfo[nums[0]][nums[1]] = { nums[2],nums[3],s->createActor(nums[2], nums[3], s->getPosition({ nums[0],nums[1] })) };
		return;
	}

	if (op == "kill") {
		if (nums[0] < 0 || nums[0] >= BOARD_SIZE)return;
		if (nums[1] < 0 || nums[1] >= BOARD_SIZE)return;
		s->boardLocked = true;
		if (s->boardInfo[nums[0]][nums[1]].func == FUNC_SUPER) s->boardInfo[nums[0]][nums[1]].type = 0;
		s->doKill({{{nums[0],nums[1]},0,{ActorInfo(nums[0],nums[1])},{}}});
		return;
	}

	if(op == "refresh"){
		s->boardLocked = true;
		s->refreshBoard();
		s->doDrop();
	}

	if (op == "setscore") {
		if (nums[0] < 0 || nums[0] >= 9999999)return;
		s->nowScore = nums[0];
		s->addScore(0);
	}

	if (op == "setswap") {
		if (nums[0] < 0 || nums[0] >= 9999999)return;
		s->leftSwapTime = nums[0];
		s->updateSwapTime();
	}

}

void ClassicialGameScene::initComponents() {

#pragma region PicBarBack, 100
	{
		PicBarBack = ImageView::create(PICTURE_BAR_PATH);
		PicBarBack->setPosition({ 0,720 });
		PicBarBack->setAnchorPoint({ 0,1 });
		PicBarBack->ignoreContentAdaptWithSize(false);
		PicBarBack->setContentSize({ 1280,50 });
		PicBarBack->setTag(100);
		addChild(PicBarBack, 10);
	}
#pragma endregion
#pragma region BtnBarBack, 101
	{
		BtnBarBack = Button::create(PICTURE_BUTTON_BACK_NORMAL_PATH, PICTURE_BUTTON_BACK_CLICKED_PATH);
		BtnBarBack->setPosition({ 0,720 });
		BtnBarBack->setAnchorPoint({ 0,1 });
		BtnBarBack->setTag(101);
		addChild(BtnBarBack, 11);
		BtnBarBack->addClickEventListener([&](Ref*) {
			auto backScene = QuitEnsureLayer::create();
			this->addChild(backScene, 100);
			});
	}
#pragma endregion

#pragma region LabTitle, 102
	{
		LabTitle = Label::create("0", "fonts/Marker Felt.ttf", 30);
		LabTitle->setPosition(640, 700 - 3);
		LabTitle->setTextColor({ 255,255,255,255 });
		LabTitle->enableOutline({ 0,0,0,255 }, 3);
		LabTitle->setTag(160);
		addChild(LabTitle, 10);
	}
#pragma endregion

#pragma region SliderScore, 150
	{
		SliderScore = Slider::create();
		SliderScore->loadBarTexture(PICTURE_SLIDERBAR_BACK_PATH);
		SliderScore->loadProgressBarTexture(PICTURE_SLIDERBAR_PRESS_PATH);
		SliderScore->setPosition({180 + 10, 450 + 10});
		SliderScore->setRotation(-90);
		SliderScore->setTag(150);
		SliderScore->setMaxPercent(50000);
		SliderScore->setTouchEnabled(false);
		addChild(SliderScore, 5);
	}
#pragma endregion

#pragma region LabSwap, 160
	{
		LabSwap = Label::create("0", "fonts/Marker Felt.ttf", 30);
		LabSwap->setPosition(180 + 23,192);
		LabSwap->setTextColor({ 255,255,255,255 });
		LabSwap->enableOutline({ 0,0,0,255 }, 3);
		LabSwap->setTag(160);
		addChild(LabSwap, 5);
	}
#pragma endregion

#pragma region LabScore, 170
	{
		LabScore = Label::create("0", "fonts/Marker Felt.ttf", 30);
		LabScore->setPosition(180 + 23, 190 + 79);
		LabScore->setTextColor({ 255,255,255,255 });
		LabScore->enableOutline({ 0,0,0,255 }, 3);
		LabScore->setTag(170);
		addChild(LabScore, 5);
	}
#pragma endregion

#pragma region BtnSetting, 201
	{
		BtnSetting = Button::create(PICTURE_BUTTON_SETTING_1_PATH, PICTURE_BUTTON_SETTING_2_PATH );
		BtnSetting->setAnchorPoint({ 0,0 });
		BtnSetting->setPosition({ 60 + 7,70 + 8 });
		BtnSetting->setTag(201);
		addChild(BtnSetting, 5);
		BtnSetting->ignoreContentAdaptWithSize(false);
		BtnSetting->setContentSize({ 50,50 });

		BtnSetting->addClickEventListener(CC_CALLBACK_1(ClassicialGameScene::SettingCallBack,this));
	}
#pragma endregion
#pragma region BtnRankList, 202
	{
		BtnRankList = Button::create(PICTURE_BUTTON_RANK_LIST_NORMAL_PATH, PICTURE_BUTTON_RANK_LIST_CLICKED_PATH);
		BtnRankList->setAnchorPoint({ 0,0 });
		BtnRankList->setPosition({ 150 + 7,70 + 8 });
		BtnRankList->setTag(202);
		addChild(BtnRankList, 5);

		BtnRankList->addClickEventListener(CC_CALLBACK_1(ClassicialGameScene::RankCallBack, this));
	}
#pragma endregion
#pragma region BtnPrompt, 203
	{
		BtnPrompt = Button::create(PICTURE_BUTTON_PROMPT_NORMAL_PATH, PICTURE_BUTTON_PROMPT_CLICKED_PATH);
		BtnPrompt->setAnchorPoint({ 0,0 });
		BtnPrompt->setPosition({ 240 + 7,70 + 8 });
		BtnPrompt->setTag(203);
		addChild(BtnPrompt, 5);

		BtnPrompt->addClickEventListener(CC_CALLBACK_1(ClassicialGameScene::PromptCallBack, this));
	}
#pragma endregion

#pragma region PicBoard, 300
	{
		PicBoard = ImageView::create(PICTURE_BOARD_PATH);
		PicBoard->setPosition({ 640,360 });
		PicBoard->setTag(300);
		addChild(PicBoard, -1);
	}
#pragma endregion
#pragma region PicCover, 301
	{
		PicCover = ImageView::create(PICTURE_COVER_PATH);
		PicCover->setPosition({ 640,360 });
		PicCover->setTag(301);
		addChild(PicCover, 1);
	}
#pragma endregion

#pragma region PicActorHighLight, 310
	{
		PicActorHighLight = ImageView::create(PICTURE_ACTOR_HIGHLIGHT);
		PicActorHighLight->setPosition({ 9999,9999 });
		PicActorHighLight->setTag(310);
		addChild(PicActorHighLight, 1);
	}
#pragma endregion

#pragma region EditBoxConsole, 401
	{
		class tmpDelegate : public cocos2d::ui::EditBoxDelegate {
			ClassicialGameScene* fa;
			virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) {};
			virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox){
				editBox->setPosition({ 0,0 });
			};
			virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text){
				string str = "";
				for(auto elm:text){
					if(elm == '\n'){
						runCommand((ClassicialGameScene*)editBox->getParent(), str);
						editBox->setText("");
					}
					str += elm;
				}
			};
			virtual void editBoxReturn(cocos2d::ui::EditBox* editBox){};
		};
		
		EditBoxConsole = ui::EditBox::create(Size(1280, 50), PICTURE_EDITBOX_BACKGROUND_PATH, Widget::TextureResType::LOCAL);
		EditBoxConsole->setAnchorPoint({ 0,1 });
		EditBoxConsole->setPosition({ 0,0 });
		EditBoxConsole->setFontColor(Color3B::GRAY);
		EditBoxConsole->setFontSize(16);
		EditBoxConsole->setText("");
		EditBoxConsole->setMaxLength(999);
		EditBoxConsole->setInputMode(ui::EditBox::InputMode::ANY);
		EditBoxConsole->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
		EditBoxConsole->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		EditBoxConsole->setDelegate(new tmpDelegate);
		EditBoxConsole->setTag(401);
		this->addChild(EditBoxConsole, 2000);
	}
#pragma endregion

#pragma region mouseListener
	{
		auto mouseListen = EventListenerMouse::create();

		mouseListen->onMouseDown = [&](Event* event) {
			PicActorHighLight->setPosition({ 9999,9999 });
			if (!boardOperatable())return;
			EventMouse* e = (EventMouse*)event;
			if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
				float cursorX = e->getCursorX();
				float cursorY = e->getCursorY();

				pii nowClick = getIndex(pii(cursorX, cursorY));
				if (selectedBlock == pii(-1,-1)) {
					selectedBlock = nowClick;
				} else if (
					(selectedBlock == pii(nowClick.first + 1, nowClick.second)) ||
					(selectedBlock == pii(nowClick.first - 1, nowClick.second)) ||
					(selectedBlock == pii(nowClick.first, nowClick.second + 1)) ||
					(selectedBlock == pii(nowClick.first, nowClick.second - 1))
					) {
					trySwap(selectedBlock, nowClick);
					selectedBlock = { -1,-1 };
				} else {
					selectedBlock = nowClick;
				}
			}
		};

		/*
		mouseListen->onMouseUp = [=](Event* event) {
			//selectedBlock = { -1,-1 };
			CCLOG("select cleand.");
		};
		*/


		mouseListen->onMouseMove = [=](Event* event) {
			const static int DELTA = 48;
			EventMouse* e = (EventMouse*)event;
			float cursorX = e->getCursorX();
			float cursorY = e->getCursorY();
			if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT && selectedBlock != pii(-1,-1)) {
				if (!boardOperatable())return;
				if (cursorX - getPosition(selectedBlock).first > DELTA && selectedBlock.second != BOARD_SIZE -1) {
					trySwap(selectedBlock, pii(selectedBlock.first, selectedBlock.second+1));
					selectedBlock = { -1,-1 };
				} else if(cursorX - getPosition(selectedBlock).first < -DELTA && selectedBlock.second != 0) {
					trySwap(selectedBlock, pii(selectedBlock.first, selectedBlock.second-1));
					selectedBlock = { -1,-1 };
				} else if(cursorY - getPosition(selectedBlock).second > DELTA && selectedBlock.first != BOARD_SIZE-1) {
					trySwap(selectedBlock, pii(selectedBlock.first+1, selectedBlock.second));
					selectedBlock = { -1,-1 };
				} else if(cursorY - getPosition(selectedBlock).second < -DELTA && selectedBlock.first != 0) {
					trySwap(selectedBlock, pii(selectedBlock.first-1, selectedBlock.second));
					selectedBlock = { -1,-1 };
				}
			} else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_UNSET) {
				if (!boardOperatable()) {
					PicActorHighLight->setPosition({9999,9999});
					return;
				}
				pii nowStayBlock = getIndex(pii(cursorX, cursorY));
				if (nowStayBlock != pii(-1, -1)) {
					pii standerPosition = getPosition(nowStayBlock);
					PicActorHighLight->setPosition(ccp(standerPosition.first,standerPosition.second ));
				} else {
					PicActorHighLight->setPosition({ 9999,9999 });
				}
			}
		};

		PicCover->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListen,PicCover);
	}
#pragma endregion
#pragma region keyListener
	{
		auto listenerKey = EventListenerKeyboard::create();
		listenerKey->onKeyPressed = ([=](EventKeyboard::KeyCode code, Event* event){
			if(code == EventKeyboard::KeyCode::KEY_GRAVE){
				EditBoxConsole->setPosition({ 0, 25 });
				EditBoxConsole->openKeyboard();
			}
		});

		getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKey, this);
	}
#pragma endregion
}

void ClassicialGameScene::SettingCallBack(Ref*) {
	auto setting = SettingLayer::create(PICTURE_SETTING_SCENE_BACKGROUND_PATH.c_str(), Size(816, 682));
	setting->addButton(PICTURE_CANCEL_BUTTON_NORMAL_PATH.c_str(), PICTURE_CANCEL_BUTTON_CLICKED_PATH.c_str(), 1);
	setting->setCallbackFunc(this, callfuncN_selector(ClassicialGameScene::settingSaveCallBack));
	this->addChild(setting, 100);
}

void ClassicialGameScene::RankCallBack(Ref*) {
	auto rank = ScoreLayer::create(0);
	addChild(rank, 100);
}

void ClassicialGameScene::PromptCallBack(Ref*) {
	if (!boardOperatable())return;

	validOperateList hint = getHitList();
	int pos = rand() % hint.size();

	showCircle(getPosition(hint[pos].first));
	showCircle(getPosition(hint[pos].second));

}