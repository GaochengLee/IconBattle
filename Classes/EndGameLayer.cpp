#include "EndGameLayer.h"

bool EndGameLayer::init() {
	if (!LayerColor::init()) {
		return false;
	}

	setColor(Color3B::GRAY);
	setOpacity(128);
	
	Util::mycocos::banEventPass(this);

	return true;
}

void EndGameLayer::doRecord(){
	User::data.coins += gold;
	switch(type){
		case TYPE_CLASSICAL: {
			User::insertRecord(TYPE_CLASSICAL, score);
			break;
		}
		case TYPE_SPECIAL: {
			User::insertRecord(TYPE_SPECIAL, score);
			break;
		}
		case TYPE_LEVEL: {
			int world_id = User::para["LEVEL_GO_WORLD"];
			int level_id = User::para["LEVEL_GO_LEVEL"];
			assert(world_id>=0 && world_id <4);
			assert(level_id >= 0 && level_id < 5);
			int nowScore = User::data.levelState[world_id][level_id];
			nowScore = std::max(nowScore,star);
			User::data.levelState[world_id][level_id] = nowScore;
			if(level_id != 5 - 1 && star){
				User::data.levelState[world_id][level_id + 1] = std::max(User::data.levelState[world_id][level_id + 1], 0);
			}
			break;
		}
	default:assert(false);
	}
}

EndGameLayer* EndGameLayer::create(int type,int gold,int score,int star) {
	EndGameLayer* layer = create();
	layer->type = type;
	layer->gold = gold;
	layer->score = score;
	layer->star = star;
	layer->initComponents();
	layer->doRecord();
	return layer;
}

void EndGameLayer::onEnter() {
	LayerColor::onEnter();
	showLayer();
}


void EndGameLayer::setRestartCallBack(const ui::Widget::ccWidgetClickCallback& callback){
	BtnRestart->addClickEventListener(callback);
}

void EndGameLayer::initComponents(){
#pragma region PicBack, 100
	{
		PicBack = ImageView::create(PICTURE_BACKGROUND_PATH);
		PicBack->setPosition({ 640,360 });
		PicBack->setTag(100);
		addChild(PicBack, 0);
	}
#pragma endregion

#pragma region BtnBack, 101
	{
		BtnBack = Button::create(PICTURE_BTN_BACK_0_PATH, PICTURE_BTN_BACK_1_PATH);
		BtnBack->setPosition({ 530,130 });
		BtnBack->setTag(101);
		BtnBack->setScale(0.5f);
		addChild(BtnBack, 1);
		BtnBack->addClickEventListener([&](Ref*) {Director::getInstance()->popScene(); });
	}
#pragma endregion
#pragma region BtnRank, 102
	if(type != TYPE_LEVEL){
		BtnRank = Button::create(PICTURE_BTN_RANK_0_PATH, PICTURE_BTN_RANK_1_PATH);
		BtnRank->setPosition({ 640,130 });
		BtnRank->setTag(102);
		BtnRank->setScale(0.5f);
		addChild(BtnRank, 1);
		BtnRank->addClickEventListener([&](Ref*){
			auto rank = ScoreLayer::create(type);
			addChild(rank, 100);
		});
	}
#pragma endregion
#pragma region BtnRestart, 103
	{
		BtnRestart = Button::create(PICTURE_BTN_RESTART_0_PATH, PICTURE_BTN_RESTART_1_PATH);
		BtnRestart->setPosition({ 750,130 });
		BtnRestart->setTag(103);
		BtnRestart->setScale(0.5f);
		addChild(BtnRestart, 1);
	}
#pragma endregion

#pragma region LabMoney, 111
	{
		LabMoney = Label::create("0", "fonts/Marker Felt.ttf", 30);
		LabMoney->setPosition(715, 320);
		LabMoney->setTextColor({ 255,255,255,255 });
		LabMoney->enableOutline({ 0,0,0,255 }, 3);
		LabMoney->setTag(111);
		addChild(LabMoney, 1);
	}
#pragma endregion
#pragma region LabScore, 112
	{
		LabScore = Label::create("0", "fonts/Marker Felt.ttf", 30);
		LabScore->setPosition(715, 237);
		LabScore->setTextColor({ 255,255,255,255 });
		LabScore->enableOutline({ 0,0,0,255 }, 3);
		LabScore->setTag(112);
		addChild(LabScore, 1);
	}
#pragma endregion

#pragma region PicStar[0-2], 121-123
	{
		Vec2 pos[] = { {538,421} ,{638,455},{737,421} };
		float scale[] = {0.53f,0.72f,0.53f};
		for (int p = 0; p < 3; p++) {
			PicStars[p] = ImageView::create(PICTURE_STAR_PATH);
			PicStars[p]->setPosition(pos[p]);
			PicStars[p]->setOpacity(0.0f);
			PicStars[p]->setScale(scale[p]);
			PicStars[p]->setTag(121+p);
			addChild(PicStars[p], 1);
		}
	}
#pragma endregion

}

void EndGameLayer::showScore() {
	const float SINGLE_DELAY = 0.05;
	float totTime = 0.0f;
	for (int i = 1; i <= 10;i++) {
		int nowv = score * i / 10;
		runAction(Sequence::createWithTwoActions(DelayTime::create(totTime), CallFunc::create([&,nowv](){
			LabScore->setString(std::to_string(nowv));
		})));
		totTime += SINGLE_DELAY;
	}
	runAction(Sequence::createWithTwoActions(DelayTime::create(totTime + 0.25f), CallFunc::create(CC_CALLBACK_0(EndGameLayer::showMoney, this))));
}

void EndGameLayer::showMoney() {
	const float SINGLE_DELAY = 0.05;
	float totTime = 0.0f;
	for (int i = 1; i <= gold; i++) {
		runAction(Sequence::createWithTwoActions(DelayTime::create(totTime), CallFunc::create([&, i]() {
			LabMoney->setString(std::to_string(i));
			})));
		totTime += SINGLE_DELAY;
	}
	runAction(Sequence::createWithTwoActions(DelayTime::create(totTime + 0.25f), CallFunc::create(CC_CALLBACK_0(EndGameLayer::showStar, this))));
}

void EndGameLayer::showStar() {
	const float SINGLE_DELAY = 0.30;
	float totTime = 0.0f;
	for (int i = 0; i < star; i++) {
		PicStars[i]->runAction(Sequence::create(
			DelayTime::create(totTime),
			Spawn::create(
				RotateTo::create(0.3f, 720.0f),
				FadeIn::create(0.3f),
				NULL),
			NULL
		));
		totTime += SINGLE_DELAY;
	}
	//runAction(Sequence::createWithTwoActions(DelayTime::create(totTime + 0.25f), CallFunc::create(CC_CALLBACK_0(EndGameLayer::showStar, this))));
}

void EndGameLayer::showLayer(){
	Sequence* animA = Sequence::create(
		ScaleTo::create(0.0f, 0.0f),
		ScaleTo::create(0.06f, 1.05f),
		ScaleTo::create(0.08f, 0.95f),
		ScaleTo::create(0.08f, 1.0f),
		NULL
	);
	Sequence* animB = Sequence::create(
		ScaleTo::create(0.0f, 0.0f),
		ScaleTo::create(0.06f, 0.53f),
		ScaleTo::create(0.08f, 0.47f),
		ScaleTo::create(0.08f, 0.53f),
		NULL
	);
	PicBack->runAction(animA->clone());
	LabMoney->runAction(animA->clone());
	LabScore->runAction(animA->clone());
	BtnBack->runAction(animB->clone());
	if(BtnRank)BtnRank->runAction(animB->clone());
	BtnRestart->runAction(animB->clone());
	runAction(Sequence::createWithTwoActions(DelayTime::create(0.22f + 0.25f), CallFunc::create(CC_CALLBACK_0(EndGameLayer::showScore, this))));
}
