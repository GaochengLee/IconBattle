#ifndef _UTIL_H_
#define _UTIL_H_

#include "cocos2d.h"
#include "cocos-ext.h"

#include "audio/include/AudioEngine.h"

#include "user.h"

using namespace cocos2d::experimental;


struct Util {

	struct string {
		
	};

	struct ui {
		static void XSleep(int nWaitInMSecs);
	};

	struct mycocos {
		class AudioWrapper;
		static AudioWrapper audioWrapper;
		static void banEventPass(cocos2d::Node*);
	};
};

class Util::mycocos::AudioWrapper {
private:
	float backVolm = 1.0;
	float effcVolm = 1.0;
	int backTaskID = AudioEngine::INVALID_AUDIO_ID;
public:
	std::string BGM_LIST[4] = {
		"../Resources/sound/game1.mp3",
		"../Resources/sound/game2.mp3",
		"../Resources/sound/game3.mp3",
		"../Resources/sound/game4.mp3",
	};
	void init() {};
	void setBackVolm(float);
	float getBackVolm();
	void setEffcVolm(float);
	float getEffcVolm();
	void playBack(std::string);
	void playEffc(std::string);
	void pauseBack();
	void resumeBack();
};


#endif 