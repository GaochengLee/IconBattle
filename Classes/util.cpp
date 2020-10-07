#include "util.h"

//XSleep====================================================================
struct XSleep_Structure
{
	int duration;
	HANDLE eventHandle;
};

DWORD WINAPI XSleepThread(LPVOID pWaitTime) {
	XSleep_Structure* sleep = static_cast<XSleep_Structure*>(pWaitTime);

	Sleep(sleep->duration);
	SetEvent(sleep->eventHandle);

	return 0;
}

void Util::ui::XSleep(int nWaitInMSecs) {

	XSleep_Structure sleep;

	sleep.duration = nWaitInMSecs;
	sleep.eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD threadId;
	CreateThread(NULL, 0, &XSleepThread, &sleep, 0, &threadId);

	MSG msg;

	while (::WaitForSingleObject(sleep.eventHandle, 0) == WAIT_TIMEOUT) {
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	CloseHandle(sleep.eventHandle);
}

//AudioWrapper====================================================================
Util::mycocos::AudioWrapper Util::mycocos::audioWrapper;
void Util::mycocos::AudioWrapper::setBackVolm(float v){
	assert(v >=0.0 && v <= 1.0);
	backVolm = v;
	if (backTaskID != AudioEngine::INVALID_AUDIO_ID) {
		AudioEngine::setVolume(backTaskID, v);
	}
}

float Util::mycocos::AudioWrapper::getBackVolm() {
	return backVolm;
}

void Util::mycocos::AudioWrapper::setEffcVolm(float v){
	assert(v >= 0.0 && v <= 1.0);
	effcVolm = v;
}

float Util::mycocos::AudioWrapper::getEffcVolm(){
	return effcVolm;
}

void Util::mycocos::AudioWrapper::playBack(std::string s) {
	if (backTaskID != AudioEngine::INVALID_AUDIO_ID ) {
		AudioEngine::stop(backTaskID);
	}
	backTaskID = AudioEngine::play2d(s.c_str(), true, User::data.isBGMMute ? 0 : backVolm);
}

void Util::mycocos::AudioWrapper::playEffc(std::string s){
	AudioEngine::play2d(s.c_str(), false, User::data.isEffectMuse ? 0 : effcVolm);
}

void Util::mycocos::AudioWrapper::pauseBack() {
	AudioEngine::pause(backTaskID);
}

void Util::mycocos::AudioWrapper::resumeBack() {
	AudioEngine::resume(backTaskID);
}

void Util::mycocos::banEventPass(cocos2d::Node* me) {
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](cocos2d::Touch*, cocos2d::Event*)->bool {return true; };
	listener->onTouchMoved = [](cocos2d::Touch*, cocos2d::Event*) {};
	listener->onTouchEnded = [](cocos2d::Touch*, cocos2d::Event*) {};

	auto listenerb = cocos2d::EventListenerMouse::create();
	listenerb->onMouseMove = [](cocos2d::EventMouse* event) {event->stopPropagation(); };
	listenerb->onMouseDown = [](cocos2d::EventMouse* event) {event->stopPropagation(); };

	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, me);
	dispatcher->addEventListenerWithSceneGraphPriority(listenerb, me);
}
