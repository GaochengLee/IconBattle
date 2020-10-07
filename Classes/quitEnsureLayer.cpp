#include "QuitEnsureLayer.h"

bool QuitEnsureLayer::init() {
	if (!LayerColor::init()) {
		return false;
	}

	setColor(Color3B::GRAY);
	setOpacity(128);
	initComponents();
	Util::mycocos::banEventPass(this);

	return true;
}

void QuitEnsureLayer::initComponents() {
#pragma region PicBack, 100
	{
		PicBack = ImageView::create(PICTURE_BACKGROUND_PATH);
		PicBack->setPosition({ 640,360 });
		PicBack->setTag(100);
		addChild(PicBack, 0);
	}
#pragma endregion

#pragma region BtnYes, 101
	{
		BtnYes = Button::create(PICTURE_BTN_YES_0_PATH, PICTURE_BTN_YES_1_PATH);
		BtnYes->setPosition({ 530,130 });
		BtnYes->setTag(101);
		BtnYes->setScale(0.5f);
		addChild(BtnYes, 1);
		BtnYes->addClickEventListener([&](Ref*) {Director::getInstance()->popScene(); });
	}
#pragma endregion
#pragma region BtnNo, 102
	{
		BtnNo = Button::create(PICTURE_BTN_NO_0_PATH, PICTURE_BTN_NO_1_PATH);
		BtnNo->setPosition({ 750,130 });
		BtnNo->setTag(102);
		BtnNo->setScale(0.5f);
		addChild(BtnNo, 1);
		BtnNo->addClickEventListener([&](Ref*) {removeFromParent();});
	}
#pragma endregion
}

void QuitEnsureLayer::showLayer() {
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
	BtnYes->runAction(animA->clone());
	BtnNo->runAction(animA->clone());
}
