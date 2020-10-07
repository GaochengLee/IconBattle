#ifndef _USER_H_
#define _USER_H_

#include <fstream>
#include <map>
#include <string>
#include "myGame.h"

using std::fstream;
using std::ios;
using std::map;
using std::string;

struct User {

	struct recordLine;
	struct Data; 

	static Data data;
	static map<string, int> para;
	static std::string gameTitle;
	static bool inGame;

	//初始化
	static void init();
	//检测存档合法性 0非法 1合法
	static bool check();
	//读入存档，没有则初始化 0非法存档 1成功读取/初始化
	static bool readIn();
	//写出存档,请在每次修改设置后写出 0失败 1成功
	static bool writeOut();
	//尝试插入记录
	static void insertRecord(int, recordLine);
	static void insertRecord(int, int);
};

struct User::recordLine {
	char userName[50];
	int mark;
};

struct User::Data {
	char userName[50];
	int coins;
	int levelState[4][5];
	int themeID;
	int BGMID;
	bool fullScreen;

	bool isBGMMute;
	bool isEffectMuse;
	int BGMvolume;
	int Effectvolume;

	recordLine record[2][10];
};



#endif 