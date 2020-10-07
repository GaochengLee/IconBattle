#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

Scene* MenuScene::createScene() {
	return MenuScene::create();
}

bool MenuScene::init(){

	if (!Scene::init()) {
		return false;
	}

	initComponents();
	// testParticle();
	return true;
}

void MenuScene::onEnter() {
	Scene::onEnter();

	auto coinLab = static_cast<Label*>(getChildByTag(1001));
	auto playerLab = static_cast<TextField*>(getChildByTag(1002));
	auto starLab = static_cast<Label*>(getChildByTag(1003));

	int starCnt = 0;

	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 5;j++) {
			starCnt += ((User::data.levelState[i][j] == -1) ? 0 : User::data.levelState[i][j]);
		}
	}

	coinLab->setString(std::to_string(User::data.coins));
	playerLab->setText(string(User::data.userName));
	starLab->setString(std::to_string(starCnt));
	changeCharacter(User::data.themeID);

	Util::mycocos::audioWrapper.playBack(MUSIC_BACKGROUND_PATH);
}

void MenuScene::initComponents() {
	// todo:
	auto vbox_main = VBox::create(Size(1280, 720));

	auto bg_menu = ImageView::create(PICTURE_MENU_BACKGROUND_PATH);
	bg_menu->ignoreContentAdaptWithSize(false);
	bg_menu->setContentSize(Size(1280, 720));
	bg_menu->setTouchEnabled(false);
	bg_menu->setAnchorPoint(Vec2(0, 0));
	this->addChild(bg_menu, -2);

	auto picTitle = ImageView::create(PICTURE_PLAYER_BOX_PATH);
	picTitle->setScale(0.75);
	picTitle->setAnchorPoint(Vec2(0, 1));
	picTitle->setPosition(Vec2(0, 720));
	this->addChild(picTitle, -1);
	
	auto hbox_head = HBox::create(Size(1280, 60));
	hbox_head->setPosition(Vec2(0, 660));

	auto picHead = ImageView::create(PICTURE_HEAD_PATH);
	picHead->setScale(0.32);
	picHead->setPosition({ 52,675 });
	addChild(picHead,5);


	auto textField = TextField::create("", "Arial", 30);
	//textField->setString(User::data.userName);
	textField->setTextColor(Color4B::BLACK);
	textField->setAnchorPoint({ 0,1 });
	textField->setMaxLengthEnabled(true);
	textField->setMaxLength(7);
	textField->setCursorEnabled(true);
	textField->setTag(1002);
	textField->addEventListener([&](Ref* pSender, ui::TextField::EventType type) {
		if (type != cocos2d::ui::TextField::EventType::DETACH_WITH_IME) return;
		auto text = dynamic_cast<ui::TextField*>(pSender);
		strcpy_s(User::data.userName, text->getString().c_str());
		User::writeOut();
	});

	textField->setPosition({ 105,673 });
	addChild(textField, 5);

	
	auto text_coin = Label::create("", "fonts/Marker Felt.ttf", 20);
	text_coin->setPosition(185, 698);
	text_coin->setTextColor({ 255,255,255,255 });
	text_coin->enableOutline({ 0,0,0,255 }, 3);
	text_coin->setTag(1001);
	addChild(text_coin,5);

	auto button_setting = Button::create(PICTURE_SETTING_0_PATH, PICTURE_SETTING_1_PATH);
	button_setting->addClickEventListener(CC_CALLBACK_1(MenuScene::settingButtonCallBack, this));
	button_setting->setScale(0.25);
	button_setting->setAnchorPoint({ 1,1 });
	button_setting->setPosition({ 1280, 720 });
	this->addChild(button_setting, 2);
	vbox_main->addChild(hbox_head);

	
	auto hbox_bottom = HBox::create(Size(1280, 600));

	imageView_player = ImageView::create(PICTURE_PLAYER_PAINT_PATH_N[0]);
	
	imageView_player->setTouchEnabled(true);
	imageView_player->addClickEventListener(CC_CALLBACK_1(MenuScene::playerImageViewCallBack, this));
	auto para_imageView_player = LinearLayoutParameter::create();
	para_imageView_player->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	para_imageView_player->setMargin(Margin(40, 100, 20, 50));
	imageView_player->setLayoutParameter(para_imageView_player);

	hbox_bottom->addChild(imageView_player);

	auto button_levelGame = Button::create(PICTURE_LEVEL_GAME_0_PATH, PICTURE_LEVEL_GAME_1_PATH);
	button_levelGame->addClickEventListener(CC_CALLBACK_1(MenuScene::levelSceneButtonCallBack, this));

	auto para_buttonLevel = LinearLayoutParameter::create();
	para_buttonLevel->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	para_buttonLevel->setMargin(Margin(0, 50, 20, 20));
	button_levelGame->setLayoutParameter(para_buttonLevel);
	hbox_bottom->addChild(button_levelGame);

	auto button_classicialGame = Button::create(PICTURE_CLASSICAL_GAME_0_PATH, PICTURE_CLASSICAL_GAME_1_PATH);
	button_classicialGame->addClickEventListener(CC_CALLBACK_1(MenuScene::classicialSceneButtonCallBack, this));

	auto para_buttonClassicial = LinearLayoutParameter::create();
	para_buttonClassicial->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	para_buttonClassicial->setMargin(Margin(0, 50, 20, 20));
	button_classicialGame->setLayoutParameter(para_buttonClassicial);
	hbox_bottom->addChild(button_classicialGame);

	auto button_specialGame = Button::create(PICTURE_SPECIAL_GAME_0_PATH, PICTURE_SPECIAL_GAME_1_PATH);
	button_specialGame->addClickEventListener(CC_CALLBACK_1(MenuScene::specialSceneButtonCallBack, this));

	auto para_buttonSpecial = LinearLayoutParameter::create();
	para_buttonSpecial->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	para_buttonSpecial->setMargin(Margin(0, 50, 20, 20));
	button_specialGame->setLayoutParameter(para_buttonSpecial);
	hbox_bottom->addChild(button_specialGame);

	vbox_main->addChild(hbox_bottom);

	auto text_version = Text::create("Version: ", "Arial", 24);
	auto para_textVersion = LinearLayoutParameter::create();
	para_textVersion->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
	para_textVersion->setMargin(Margin(300, 10, 10, 10));
	text_version->setLayoutParameter(para_textVersion);
	text_version->setVisible(false);
	//¿³ÁË
	vbox_main->addChild(text_version);

	this->addChild(vbox_main);

	auto text_star = Label::create("", "fonts/Marker Felt.ttf", 20);
	text_star->setPosition(500, 175);
	text_star->setTextColor({ 255,255,255,255 });
	text_star->enableOutline({ 0,0,0,255 }, 3);
	text_star->setTag(1003);
	addChild(text_star, 10);

	auto levelMouseListener = EventListenerMouse::create();
	levelMouseListener->onMouseMove = [&](EventMouse* event) {
		static Rect rect(360, 110, 270, 500);
		bool nowState = rect.containsPoint(event->getLocation());
		if (nowState != inArae[0]) {
			inArae[0] = nowState;
			nowState ? MoveinBtnLevel() : MoveoutBtnLevel();
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(levelMouseListener, button_levelGame);

	auto classicalMouseListener = EventListenerMouse::create();
	classicalMouseListener->onMouseMove = [&](EventMouse* event) {
		static Rect rect(650, 110, 270, 500);
		bool nowState = rect.containsPoint(event->getLocation());
		if (nowState != inArae[1]) {
			inArae[1] = nowState;
			nowState ? MoveinBtnClassical() : MoveoutBtnClassical();
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(classicalMouseListener, button_classicialGame);

	auto specialMouseListener = EventListenerMouse::create();
	specialMouseListener->onMouseMove = [&](EventMouse* event) {
		CCLOG("%f,%f",event->getCursorX(),event->getCursorY());
		static Rect rect(940, 110, 270, 500);
		bool nowState = rect.containsPoint(event->getLocation());
		if (nowState != inArae[2]) {
			inArae[2] = nowState;
			nowState ? MoveinBtnSpecial() : MoveoutBtnSpecial();
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(specialMouseListener, button_specialGame);

	for (int i = 0; i < 2; i++) {
		effFire_A[i] = ParticleSystemQuad::create(SPECIAL_EFFECT_CIRCLE);
		effFire_A[i]->setPosition({ static_cast<float>(420 + 150 * i),static_cast<float>(275 + 100 * i)});
		effFire_A[i]->setScale(0.4f);
		effFire_A[i]->stop();
		addChild(effFire_A[i], 400);
	}

	

	for (int i = 0; i < 6;i++) {
		effFire_B[i] = ParticleSystemQuad::create(SPECIAL_EFFECT_MELT_N[i]);
		effFire_B[i]->setPosition({ static_cast<float>(698 + 83 *(i%3)),static_cast<float>(326 - 100 * (i/3)) });
		effFire_B[i]->setScale(0.6f);
		effFire_B[i]->stop();
		addChild(effFire_B[i], 400);
	}

	effFire_C = ParticleSystemQuad::create(SPECIAL_EFFECT_THUNDER);
	effFire_C->setScale(0.4f); 
	effFire_C->setPosition(698 + 374,382 - 122);
	effFire_C->setRotation(90);
	effFire_C->stop();
	addChild(effFire_C, 400);

}

void MenuScene::playerImageViewCallBack(Ref* pSender){
	log("ImageView clicked");
}

void MenuScene::levelSceneButtonCallBack(Ref* pSender) {
	User::gameTitle = "Level Game";
	Director::getInstance()->pushScene(TransitionFadeTR::create(1.0f, LevelScene::create()));
}

void MenuScene::classicialSceneButtonCallBack(Ref* pSender){
	User::gameTitle = "Classicial Game";
	Director::getInstance()->pushScene(TransitionFadeTR::create(1.0f, ClassicialGameScene::create()));
}

void MenuScene::specialSceneButtonCallBack(Ref* pSender)
{
	User::gameTitle = "Special Game";
	Director::getInstance()->pushScene(TransitionFadeTR::create(1.0f, SpecialGameScene::create()));
}

void MenuScene::settingButtonCallBack(Ref* pSender)
{
	auto setting = SettingLayer::create(PICTURE_SETTING_SCENE_BACKGROUND_PATH.c_str(), Size(816, 682));
	setting->addButton(PICTURE_CANCEL_BUTTON_NORMAL_PATH.c_str(), PICTURE_CANCEL_BUTTON_CLICKED_PATH.c_str(), 1);
	setting->setCallbackFunc(this, callfuncN_selector(MenuScene::settingSceneButtonCallBack));
	this->addChild(setting, 10);
}

void MenuScene::settingSceneButtonCallBack(Node* pNode)
{
	if (pNode->getTag() == 1)
	{
		User::writeOut();
	}
}

void MenuScene::changeCharacter(int type) {
	imageView_player->loadTexture(PICTURE_PLAYER_PAINT_PATH_N[type]);
}

void MenuScene::MoveinBtnLevel(){
	for (int i = 0; i < 2; i++) {
		effFire_A[i]->start();
	}
}

void MenuScene::MoveoutBtnLevel(){
	for (int i = 0; i < 2; i++) {
		effFire_A[i]->stop();
	}
}

void MenuScene::MoveinBtnClassical(){
	for(int i=0;i<6;i++){
		effFire_B[i]->start();
	}
}

void MenuScene::MoveoutBtnClassical(){
	for (int i = 0; i < 6; i++) {
		effFire_B[i]->stop();
	}
}

void MenuScene::MoveinBtnSpecial(){
	effFire_C->start();
}

void MenuScene::MoveoutBtnSpecial(){
	effFire_C->stop();
}
