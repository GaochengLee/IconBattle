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

	//��ʼ��
	static void init();
	//���浵�Ϸ��� 0�Ƿ� 1�Ϸ�
	static bool check();
	//����浵��û�����ʼ�� 0�Ƿ��浵 1�ɹ���ȡ/��ʼ��
	static bool readIn();
	//д���浵,����ÿ���޸����ú�д�� 0ʧ�� 1�ɹ�
	static bool writeOut();
	//���Բ����¼
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