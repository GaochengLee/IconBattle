#include "SplashScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;

Scene* SplashScene::createScene() {
	return SplashScene::create();
}

void SplashScene::onEnter() {
	Scene::onEnter();

	//Util::mycocos::audioWrapper.playEffc(MUSIC_WELCOME_PATH);
}

bool SplashScene::init(){

	if (!Scene::init()){
		return false;
	}

	initComponents();
	User::readIn();

	Util::mycocos::audioWrapper.setBackVolm(User::data.BGMvolume * 1.0 / 100);
	Util::mycocos::audioWrapper.setEffcVolm(User::data.Effectvolume * 1.0 / 100);

	scheduleOnce(schedule_selector(SplashScene::mainTask), 0.0f);

	return true;
}

void SplashScene::mainTask(float) {
	showPictureA();
}

void SplashScene::tsakFlashLable(float) {
	getChildByTag(11)->runAction(Sequence::create(
		FadeIn::create(0.75),
		FadeOut::create(0.75),
		NULL));
}

void SplashScene::initComponents() {
	
	auto picA = Sprite::create(PICTURE_A_PATH);
	picA->setPosition(640, 360);
	picA->setCascadeOpacityEnabled(true);
	picA->setOpacity(0.0);
	addChild(picA);
	picA->setTag(1);

	auto picB = Sprite::create(PICTURE_B_PATH);
	picB->setPosition(640, 360);
	picB->setCascadeOpacityEnabled(true);
	picB->setOpacity(0.0);
	addChild(picB);
	picB->setTag(2);
	
	auto picSplashBack = ImageView::create(PICTURE_SPLASH_BACK_PATH);
	picSplashBack->setPosition({ 640,360 });
	picSplashBack->setCascadeOpacityEnabled(true);
	picSplashBack->setOpacity(1.0);
	picSplashBack->setTag(10);
	picSplashBack->addClickEventListener(CC_CALLBACK_1(SplashScene::startButtonCallBack, this));
	addChild(picSplashBack, 1);

	auto labelStart = Label::create("CLICK TO STRAT", "fonts/Marker Felt.ttf", 48);
	labelStart->setPosition(640,150);
	labelStart->setTextColor({ 255,255,255,255 });
	labelStart->enableOutline({ 0,0,0,255 }, 3);
	labelStart->setTag(11);
	labelStart->setOpacity(0.0);
	addChild(labelStart, 2);

	auto buttonSetting = Button::create(PICTURE_SETTING_0_PATH, PICTURE_SETTING_1_PATH);
	buttonSetting->setScale(0.25);
	buttonSetting->setAnchorPoint(Vec2(1, 0));
	buttonSetting->setPosition(Vec2(1280, 0));
	buttonSetting->setTag(12);
	buttonSetting->setOpacity(0.0);
	buttonSetting->addClickEventListener(CC_CALLBACK_1(SplashScene::settingButtonCallBack, this));
	addChild(buttonSetting, 2);

	auto picTitle = Sprite::create(PICTURE_TITLE_PATH);
	picTitle->setPosition(640  - 30, 260);
	picTitle->setCascadeOpacityEnabled(true);
	picTitle->setOpacity(0.0);
	addChild(picTitle, 2);
	picTitle->setTag(13);
	
}

void SplashScene::showPictureA() {
	getChildByTag(1)->runAction(Sequence::create(
		FadeIn::create(2.0),
		FadeOut::create(2.0),
		CallFunc::create(CC_CALLBACK_0(SplashScene::showPictureB, this)),
		NULL));
}

void SplashScene::showPictureB() {
	getChildByTag(2)->runAction(Sequence::create(
		FadeIn::create(2.0),
		FadeOut::create(2.0),
		CallFunc::create(CC_CALLBACK_0(SplashScene::showSplash, this)),
		NULL));
}

void SplashScene::showSplash() {
	getChildByTag(10)->runAction(Sequence::create(
		FadeIn::create(2.0),
		NULL));
	schedule(schedule_selector(SplashScene::tsakFlashLable), 1.5f);

	removeChildByTag(1);
	removeChildByTag(2);
	((ImageView*)getChildByTag(10))->setTouchEnabled(true);

	getChildByTag(12)->runAction(Sequence::create(
		FadeIn::create(2.0f),
		NULL));

	getChildByTag(13)->runAction(Spawn::create(
		MoveTo::create(2.0f, Vec2(640 - 30, 460)),
		FadeIn::create(2.0f),
		NULL
	));
}

void SplashScene::startButtonCallBack(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MenuScene::create()));
}

void SplashScene::settingButtonCallBack(Ref* pSender)
{
	auto setting = SettingLayer::create(PICTURE_SETTING_BG_PATH.c_str(), Size(816, 682));

	setting->addButton(PICTURE_CANCEL_BUTTON_NORMAL_PATH.c_str(), PICTURE_CANCEL_BUTTON_CLICKED_PATH.c_str(), 1);
	setting->setCallbackFunc(this, callfuncN_selector(SplashScene::settingSceneButtonCallBack));
	this->addChild(setting, 5);
}

void SplashScene::settingSceneButtonCallBack(Node* pNode)
{
	if (pNode->getTag() == 1)
	{
		User::writeOut();
	}
}
