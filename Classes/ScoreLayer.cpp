#include "ScoreLayer.h"

bool ScoreLayer::init() {
	if (!LayerColor::init()) {
		return false;
	}

	setColor(Color3B::GRAY);
	setOpacity(128);
	

	Util::mycocos::banEventPass(this);

	return true;
}

void ScoreLayer::onEnter() {
	LayerColor::onEnter();
	showLayer();
}

void ScoreLayer::onExit() {
	LayerColor::onExit();
}


ScoreLayer* ScoreLayer::create(int type) {
	ScoreLayer* layer = create();
	layer->type = type;
	layer->initComponents();
	return layer;
}

void ScoreLayer::initComponents() {

#pragma region PicBack,100
	PicBack = ImageView::create(PICTURE_BACKGROUND_PATH);
	PicBack->setTag(100);
	PicBack->setScale(0.8f);
	PicBack->setPosition({ 640,360 });
	addChild(PicBack, 0);
#pragma endregion

#pragma region BtnClose,101
	BtnClose = Button::create(BUTTON_CLOSE_0_PATH, BUTTON_CLOSE_1_PATH);
	BtnClose->setTag(101);
	BtnClose->setAnchorPoint({ 0,0 });
	BtnClose->setPosition({ 800,600 });
	BtnClose->setScale(0.4f);
	BtnClose->addClickEventListener([&](Ref*) {
		removeFromParentAndCleanup(true);
	});
	addChild(BtnClose, 1);
#pragma endregion


	string temp;
	for (int i = 0; i < 10; i++) {
		temp = User::data.record[type][i].userName;
		int t = User::data.record[type][i].mark;
		if(t >= 0)
			addPlayerRecord(temp, 1, 0, User::data.record[type][i].mark);
	}

	//addPlayerRecord("t1", 111, 0, 111);
	//addPlayerRecord("t2", 111, 1, 222);
	//addPlayerRecord("t3", 111, 2, 333);
	//addPlayerRecord("t4", 111, 3, 444);

}

void ScoreLayer::addPlayerRecord(string name, int record, int icon, int ranking) {

	if (icon < 0) icon = 0;
	if (icon > 3) icon = 3;

	PlayerRecord* pr = new PlayerRecord;
	pr->BacPlayer = ImageView::create(PIC_PLAYER_RECORD_PATH);
	pr->BacIcon = ImageView::create(PIC_PLAYER_ICON_PATH[icon]);
	pr->playerName = name;
	pr->playerScore = ranking;

	playerGroup.push_back(pr);

	for(unsigned int i = 0; i < playerGroup.size() - 1; i++)
		for (unsigned int j = i + 1; j < playerGroup.size(); j++) {
			if (playerGroup[j]->playerScore > playerGroup[i]->playerScore) {
				PlayerRecord* temp = playerGroup[i];
				playerGroup[i] = playerGroup[j];
				playerGroup[j] = temp;
			}
		}


	if (playerGroup.size() > 4)
		playerGroup.pop_back();


	removeAllChildrenWithCleanup(true);
	addChild(PicBack, 0);
	addChild(BtnClose, 0);

	for (unsigned int i = 0; i < playerGroup.size(); i++) {
		playerGroup[i]->BacPlayer->setPosition({ static_cast<float>(640), static_cast<float>(510 - i * (109 + 10)) });
		playerGroup[i]->BacPlayer->setScale(0.8f);

		playerGroup[i]->BacIcon->setScale(0.316f);
		playerGroup[i]->BacIcon->setPosition({ static_cast<float>(525), static_cast<float>(515 - i * (109 + 10)) });
		addChild(playerGroup[i]->BacIcon, 2);

		auto LabelName = Label::create(playerGroup[i]->playerName, "fonts/Marker Felt.ttf", 20);
		LabelName->setTextColor(Color4B::BLACK);
		LabelName->enableOutline(Color4B::WHITE, 2);
		LabelName->setPosition({ static_cast<float>(670),static_cast<float>(540 - i*(109 + 10)) });
		addChild(LabelName, 2);

		auto LabelScore = Label::create(std::to_string(playerGroup[i]->playerScore), "fonts/Marker Felt.ttf", 30);
		LabelScore->setTextColor(Color4B::BLACK);
		LabelScore->enableOutline(Color4B::WHITE, 2);
		LabelScore->setPosition({ static_cast<float>(680), static_cast<float>(500 - i * (109 + 10)) });
		addChild(LabelScore, 2);

		addChild(playerGroup[i]->BacPlayer, 1);
	}
}

void ScoreLayer::showLayer() {
	Sequence* animA = Sequence::create(
		ScaleTo::create(0.0f, 0.0f),
		ScaleTo::create(0.06f, 0.85f),
		ScaleTo::create(0.08f, 0.75f),
		ScaleTo::create(0.08f, 0.8f),
		NULL
	);

	BtnClose->runAction(Sequence::create(
		ScaleTo::create(0.0f, 0.0f),
		ScaleTo::create(0.06f, 0.45f),
		ScaleTo::create(0.08f, 0.35f),
		ScaleTo::create(0.08f, 0.4f),
		NULL
	));
	PicBack->runAction(animA->clone());

	for (unsigned int i = 0; i < playerGroup.size(); i++) {
		playerGroup[i]->BacPlayer->runAction(animA->clone());
		
		playerGroup[i]->BacIcon->runAction(Sequence::create(
			ScaleTo::create(0.0f, 0.0f),
			ScaleTo::create(0.06f, 0.336f),
			ScaleTo::create(0.08f, 0.286f),
			ScaleTo::create(0.08f, 0.316f),
			NULL
		));
	}

}
