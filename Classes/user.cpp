#include "user.h"

User::Data User::data;
bool User::inGame;
map<string, int> User::para;
std::string User::gameTitle;

void User::init() {
	strcpy_s(data.userName, 50, "Player");
	data.coins = 0;
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 5;j++) {
			data.levelState[i][j] = (j?-1:0);
		}
	}
	data.themeID = 0;
	data.BGMID = 0;
	data.fullScreen = false;

	data.isBGMMute = false;
	data.isEffectMuse = false;
	data.BGMvolume = 100;
	data.Effectvolume = 100;

	for (int i = 0; i < 2;i++) {
		for (int j = 0; j < 10;j++) {
			strcpy_s(data.record[i][j].userName, 50, "Empty");
			data.record[i][j].mark = -1;
		}
	}

}

bool User::check() {
	{
		bool nameOK = false;
		for (int i = 0; i < 50; i++) {
			if (data.userName[i] == 0)nameOK = true;
		}
		if (!nameOK)return false;
	}
	if (data.coins < 0)return false;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			if (data.levelState[i][j] < -1 || data.levelState[i][j] > 3)return false;
		}
	}
	if (data.themeID < 0 || data.themeID > 233)return false;
	if (data.BGMID < 0 || data.BGMID > 233)return false;
	if (data.BGMvolume < 0 || data.BGMvolume > 100) return false;
	if (data.Effectvolume < 0 || data.Effectvolume > 100) return false;

	return true;
}

bool User::readIn() {
	fstream fs("user.dat", ios::in | ios::binary);
	if (!fs) {
		init();
		writeOut();
		return true;
	}
	fs.read(reinterpret_cast<char*>(&data), sizeof(Data));
	bool rtn = fs.bad();
	fs.close();
	return rtn && check();
}

bool User::writeOut() {
	fstream fs("user.dat", ios::out | ios::binary);
	if (!fs)return false;
	fs.write(reinterpret_cast<char*>(&data), sizeof(Data));
	bool rtn = fs.bad();
	fs.close();
	return rtn;
}

void User::insertRecord(int pos, recordLine line) {
	int insp = -1;
	for (int i = 0; i < 10;i++) {
		if(data.record[pos][i].mark < line.mark){
			insp = i;
			break;
		}
	}
	if(insp != -1){
		for(int i=9;i>insp;i--){
			data.record[pos][i] = data.record[pos][i-1];
		}
		data.record[pos][insp] = line;
	}
}

void User::insertRecord(int pos, int val) {
	recordLine rline;
	strcpy_s(rline.userName,50, data.userName);
	rline.mark = val;
	insertRecord(pos, rline);
}
