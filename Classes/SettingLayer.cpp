#include "SettingLayer.h"
#include "ClassicialGameScene.h"
#include "MenuScene.h"

SettingLayer::SettingLayer()
	:m_pMenu(NULL),
	m_contentPadding(0),
	m_contentPaddingTop(0),
	m_callback(NULL),
	m_callbackListener(NULL),
	m_sfBackGround(NULL),
	m_s9BackGround(NULL),
	m_ltContentText(NULL),
	m_ltTitle(NULL)
{}

SettingLayer::~SettingLayer()
{
	CC_SAFE_RELEASE(m_pMenu);
	CC_SAFE_RELEASE(m_sfBackGround);
	CC_SAFE_RELEASE(m_s9BackGround);
	CC_SAFE_RELEASE(m_ltContentText);
	CC_SAFE_RELEASE(m_ltTitle);
}


bool SettingLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}

	Menu* menu = Menu::create();
	menu->setPosition({ 0,0 });
	setMenuButton(menu);

	Util::mycocos::banEventPass(this);

	setColor(Color3B::GRAY);
	setOpacity(128);

	return true;
}

SettingLayer* SettingLayer::create(const char* backgroundImage, Size contentSize)
{
	SettingLayer* layer = SettingLayer::create();
	layer->setSprite9BackGround(ui::Scale9Sprite::create(backgroundImage));
	layer->m_dialogContentSize = contentSize;

	return layer;
}

void SettingLayer::setTitle(const char* title, int fontSize)
{
	Label* label = Label::createWithSystemFont(title, "Arial", fontSize);
	label->setColor(Color3B::RED);
	setLabelTitle(label);
}

void SettingLayer::setContentText(const char* text, int padding, int paddingTop)
{
	Label* label = Label::createWithSystemFont(text, "Arial", 40);
	label->setColor(Color3B(247, 147, 30));
	label->enableShadow();
	label->enableOutline(Color4B::WHITE, 4);
	setLabelContentText(label);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}

void SettingLayer::setCallbackFunc(Ref* target, SEL_CallFuncN callfun)
{
	m_callbackListener = target;
	m_callback = callfun;
}

bool SettingLayer::addButton(const char* normalImage, const char* selectedImage, int tag)
{
	Size winSize = Director::getInstance()->getWinSize();
	Point center_point = Point(winSize.width / 2, winSize.height / 2);
	auto menuImage = MenuItemImage::create(
		normalImage,
		selectedImage,
		CC_CALLBACK_1(SettingLayer::buttonCallBack, this));

	menuImage->setTag(tag);
	menuImage->setPosition(center_point);
	getMenuButton()->addChild(menuImage);

	return true;
}

void SettingLayer::buttonCallBack(Ref* pSender)
{
	Node* node = dynamic_cast<Node*>(pSender);
	log("touch tag: %d", node->getTag());
	if (m_callback && m_callbackListener)
	{
		(m_callbackListener->*m_callback)(node);
	}

	for each(auto child in this->getChildren())
	{
		child->runAction(FadeOut::create(0.5f)); 
	}

	scheduleOnce(schedule_selector(SettingLayer::closeSettingTask), 0.5f);
}

void SettingLayer::closeSettingTask(float)
{
	this->removeFromParentAndCleanup(true);
}

void SettingLayer::onEnter()
{
	log("PopSetting onEnter");
	LayerColor::onEnter();
	Size winSize = Director::getInstance()->getWinSize();
	Point center = Point(winSize.width / 2, winSize.height / 2);
	ui::Scale9Sprite* background = getSprite9BackGround();

	background->setContentSize(m_dialogContentSize);//指定对话框大小
	background->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(background, 0, 0);

	Action* popupActions = Sequence::create(ScaleTo::create(0.0f, 0.0f),
		ScaleTo::create(0.06f, 1.05f),
		ScaleTo::create(0.08f, 0.95f),
		ScaleTo::create(0.08f, 1.0f),
		CallFunc::create(CC_CALLBACK_0(SettingLayer::backgroundFinish, this))
		, NULL);

	background->runAction(popupActions);
}

void SettingLayer::backgroundFinish()
{
	Size winSize = Director::getInstance()->getWinSize();
	Point pCenter = Point(winSize.width / 2, winSize.height / 2);

	this->addChild(getMenuButton());

#pragma region chechBox music, 4
	auto checkBox_musicMute = ui::CheckBox::create(PICTURE_MUSIC_NORMAL_PATH, PICTURE_MUSIC_MUTE_PATH);
	checkBox_musicMute->setTag(4);
	checkBox_musicMute->addEventListener([=](Ref* pSender, ui::CheckBox::EventType type) {
		switch (type)
		{
		case cocos2d::ui::CheckBox::EventType::SELECTED:
		{
			Util::mycocos::audioWrapper.setBackVolm(0.0f);
			User::data.isBGMMute = true;
		}
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		{
			Util::mycocos::audioWrapper.setBackVolm(1.0*User::data.BGMvolume / 100);
			User::data.isBGMMute = false;
		}
			break;
		default:
			break;
		}
	});
	checkBox_musicMute->setPosition({ 550, 470 });
	if (User::data.isBGMMute)
	{
		checkBox_musicMute->setSelected(true);
		Util::mycocos::audioWrapper.setBackVolm(0.0f);
	}
	else
	{
		checkBox_musicMute->setSelected(false);
		Util::mycocos::audioWrapper.setBackVolm(1.0*User::data.BGMvolume / 100);
	}
	this->addChild(checkBox_musicMute, 1);
#pragma endregion

#pragma region checkBox effect, 5
	checkBox_effectMute = ui::CheckBox::create(PICTURE_EFFECT_NORMAL_PATH, PICTURE_EFFECT_MUTE_PATH);
	checkBox_effectMute->setTag(5);
	checkBox_effectMute->addEventListener([=](Ref* pSender, ui::CheckBox::EventType type) {
		switch (type)
		{
		case cocos2d::ui::CheckBox::EventType::SELECTED:
		{
			Util::mycocos::audioWrapper.setEffcVolm(0.0f);
			User::data.isEffectMuse = true;
		}
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		{
			Util::mycocos::audioWrapper.setEffcVolm(1.0*User::data.Effectvolume / 100);
			User::data.isEffectMuse = false;
		}
			break;
		default:
			break;
		}
	});
	checkBox_effectMute->setPosition({ 850,470 });
	if (User::data.isEffectMuse)
	{
		checkBox_effectMute->setSelected(true);
		Util::mycocos::audioWrapper.setEffcVolm(0.0f);
	}
	else
	{
		checkBox_effectMute->setSelected(false);
		Util::mycocos::audioWrapper.setEffcVolm(1.0*User::data.Effectvolume / 100);
	}
	this->addChild(checkBox_effectMute, 1);
#pragma endregion

#pragma region slider_sound_mainBGM, 6
	slider_sound_mainBGM = ui::Slider::create();
	slider_sound_mainBGM->loadBarTexture(PICTURE_SLIDERBAR_BACK_PATH);
	slider_sound_mainBGM->loadSlidBallTextures(PICTURE_SLIDERNODE_NORMAL_PATH, PICTURE_SLIDERNODE_PRESS_PATH, PICTURE_SLIDERNODE_DISABLE_PATH);
	slider_sound_mainBGM->loadProgressBarTexture(PICTURE_SLIDERBAR_PRESS_PATH);
	slider_sound_mainBGM->setPercent(100);
	slider_sound_mainBGM->setMaxPercent(100);
	slider_sound_mainBGM->setTag(6);
	slider_sound_mainBGM->addEventListener([&](Ref* pSender, ui::Slider::EventType type) {
		switch (type)
		{
		case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
		{
			ui::Slider* slider = dynamic_cast<ui::Slider*>(pSender);
			int percent = slider->getPercent();
			if (User::data.isBGMMute) {
				User::data.BGMvolume = percent;
				break;
			}
			User::data.BGMvolume = percent;
			Util::mycocos::audioWrapper.setBackVolm(1.0* percent / 100.0);
			
			log("slider percent = %d", percent);
		}
			break;
		case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN:
			break;
		case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_UP:
			break;
		case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_CANCEL:
			break;
		default:
			break;
		}
	});
	slider_sound_mainBGM->setPosition({ 740, 420 });
	slider_sound_mainBGM->setPercent(User::data.BGMvolume);
	this->addChild(slider_sound_mainBGM, 1);
#pragma endregion

#pragma region slider_sound_effectBGM, 7
	slider_sound_effectBGM = ui::Slider::create();
	slider_sound_effectBGM->loadBarTexture(PICTURE_SLIDERBAR_BACK_PATH);
	slider_sound_effectBGM->loadSlidBallTextures(PICTURE_SLIDERNODE_NORMAL_PATH, PICTURE_SLIDERNODE_PRESS_PATH, PICTURE_SLIDERNODE_DISABLE_PATH);
	slider_sound_effectBGM->loadProgressBarTexture(PICTURE_SLIDERBAR_PRESS_PATH);
	slider_sound_effectBGM->setPercent(100);
	slider_sound_effectBGM->setMaxPercent(100);
	slider_sound_effectBGM->setTag(7);
	slider_sound_effectBGM->addEventListener([&](Ref* pSender, ui::Slider::EventType type) {
		switch (type)
		{
		case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
		{
			ui::Slider* slider = dynamic_cast<ui::Slider*>(pSender);
			int percent = slider->getPercent();
			if (User::data.isEffectMuse) {
				User::data.Effectvolume = percent;
				break;
			}
			Util::mycocos::audioWrapper.setEffcVolm(1.0* percent / 100.0);
			User::data.Effectvolume = percent;
			log("slider percent = %d", percent);
		}
			break;
		case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN:
			break;
		case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_UP:
			break;
		case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_CANCEL:
			break;
		default:
			break;
		}
	});
	slider_sound_effectBGM->setPosition({ 740,380 });
	slider_sound_effectBGM->setPercent(User::data.Effectvolume);
	this->addChild(slider_sound_effectBGM, 1);
#pragma endregion

#pragma region RBG player picture, 8
	{
		RBG_player_picture = ui::RadioButtonGroup::create();
		RBG_player_picture->setTag(8);
		this->addChild(RBG_player_picture, 5);
	}
#pragma endregion

#pragma region RadPlayerPic[4], 11-14
	{
		for (int i = 0; i < 4; i++)
		{
			RadPlayerPic[i] = ui::RadioButton::create(PICTURE_RADIO_BLUE_PATH, PICTURE_RADIO_GREEN_PATH);
			RadPlayerPic[i]->setPosition({ static_cast<float>(590 + i * 100), 240 });
			RadPlayerPic[i]->setTag(11 + i);
			RadPlayerPic[i]->addEventListener([=](Ref* pSender, ui::RadioButton::EventType type) {
				switch (type)
				{
				case cocos2d::ui::RadioButton::EventType::SELECTED:
				{
					auto rb = dynamic_cast<ui::RadioButton*>(pSender);
					User::data.themeID = rb->getTag() - 11;
					auto scene = dynamic_cast<MenuScene*>(Director::getInstance()->getRunningScene());
					if(scene){
						scene->changeCharacter(User::data.themeID);
					}
				}
					break;
				case cocos2d::ui::RadioButton::EventType::UNSELECTED:
					break;
				default:
					break;
				}
				
			});
			this->addChild(RadPlayerPic[i], 5);
			RBG_player_picture->addRadioButton(RadPlayerPic[i]);
		}
		RBG_player_picture->setSelectedButton(User::data.themeID);
	}
#pragma endregion

#pragma region RBG main BGM, 9
	{
		RBG_main_BGM = ui::RadioButtonGroup::create();
		RBG_main_BGM->setTag(9);
		this->addChild(RBG_main_BGM, 5);
	}
#pragma endregion

#pragma region RadMainBGM[4], 21-24
	{
		for (int i = 0; i < 4; i++)
		{
			RadMainBGM[i] = ui::RadioButton::create(PICTURE_RADIO_BLUE_PATH, PICTURE_RADIO_GREEN_PATH);
			RadMainBGM[i]->setPosition({ static_cast<float>(590 + i * 100), 320 });
			RadMainBGM[i]->setTag(21 + i);
			RadMainBGM[i]->addEventListener([=](Ref* pSender, ui::RadioButton::EventType type) {
				if (type != cocos2d::ui::RadioButton::EventType::SELECTED)return;
				auto rb = dynamic_cast<ui::RadioButton*>(pSender);
				int bgmid = rb->getTag() - 21;
				if(User::data.BGMID != bgmid){
					User::data.BGMID = bgmid;
					if(User::inGame){
						Util::mycocos::audioWrapper.playBack(Util::mycocos::audioWrapper.BGM_LIST[User::data.BGMID]);
					}
				}

			});
			this->addChild(RadMainBGM[i], 5);
			RBG_main_BGM->addRadioButton(RadMainBGM[i]);
		}
		RBG_main_BGM->setSelectedButton(User::data.BGMID);
	}
#pragma endregion

	// float btnWidth = m_dialogContentSize.width / (getMenuButton()->getChildrenCount() + 1);
	float btnWidth = 50.0;
	Vector<Node*> vector = getMenuButton()->getChildren();
	Ref* pObj = NULL;
	int i = 0;
	log("%f", m_dialogContentSize.width);
	for (Node*pObj : vector)
	{
		Node* node = dynamic_cast<Node*>(pObj);
		node->setPosition(Point(winSize.width / 2 + m_dialogContentSize.width / 2 - btnWidth * (i + 1),
			winSize.height / 2 + m_dialogContentSize.height / 3));
		i++;
	}

	/*if (getLabelTitle())
	{
		getLabelTitle()->setPosition(pCenter.x + 0, pCenter.y + m_dialogContentSize.height / 2 - 35.0f);
		this->addChild(getLabelTitle());
	}

	if (getLabelContentText())
	{
		Label* ltf = getLabelContentText();
		ltf->setPosition(winSize.width / 2, winSize.height / 2);
		ltf->setDimensions(m_dialogContentSize.width - m_contentPadding * 2, m_dialogContentSize.height - m_contentPaddingTop);
		ltf->setHorizontalAlignment(TextHAlignment::LEFT);
		this->addChild(ltf);
	}*/
}

void SettingLayer::onExit()
{
	log("PopSetting onExit");
	LayerColor::onExit();
}
