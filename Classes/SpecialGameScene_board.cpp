#include "SpecialGameScene.h"

USING_NS_CC;
//BUG: 结算界面直接关闭窗口，触发断点，初步估计是重复回收节点。
void SpecialGameScene::trySwap(pii blocka, pii blockb) {
	CCLOG("[LOCK] Try Swap (%d,%d),(%d,%d)", blocka.first, blocka.second, blockb.first, blockb.second);
	boardLocked = true;
	leftSwapTime--;
	if (boardInfo[blocka.first][blocka.second].func == FUNC_SUPER && boardInfo[blockb.first][blockb.second].func == FUNC_SUPER) {
		pii center = blocka;
		ActorInfoList tCrossActor;
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (boardInfo[i][j].func == FUNC_SUPER) {
					boardInfo[i][j].func = FUNC_NO;
					showMelt(getPosition({ i,j }), 6);
				}
				else {
					showMelt(getPosition({ i,j }), boardInfo[i][j].type);
				}
				tCrossActor.push_back(ActorInfo(i, j));
			}
		}
		doKill({ { center,5000,tCrossActor,{} } });
		updateSwapTime();
		return;
	}
	else if (boardInfo[blocka.first][blocka.second].func == FUNC_SUPER) {
		boardInfo[blocka.first][blocka.second].type = boardInfo[blockb.first][blockb.second].type;
		doKill({ { blocka,233,{blocka},{} } });
		updateSwapTime();
		return;
	}
	else if (boardInfo[blockb.first][blockb.second].func == FUNC_SUPER) {
		boardInfo[blockb.first][blockb.second].type = boardInfo[blocka.first][blocka.second].type;
		doKill({ { blockb,233,{blockb},{} } });
		updateSwapTime();
		return;
	}


	swap(boardInfo[blocka.first][blocka.second], boardInfo[blockb.first][blockb.second]);
	auto rst = getKillable();
	swap(boardInfo[blocka.first][blocka.second], boardInfo[blockb.first][blockb.second]);

	if (!rst) {
		doFailSwap(blocka, blockb);
		leftSwapTime++;
	}
	else {
		doRealSwap(blocka, blockb);
	}
	updateSwapTime();
}

void SpecialGameScene::initBoard() {
	nowScoreA[0] = nowScoreA[1] = 0;
	leftSwapTime = SWAP_COUNT;
	updateScore();
	updateSwapTime();
	refreshBoard();
}

void SpecialGameScene::refreshBoard() {
	for (int i = 0; i < 2 * BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (boardInfo[i][j].type != -1) {
				removeChild(boardInfo[i][j].actor);
			}
			boardInfo[i][j] = blockInfo();
		}
	}
	for (int i = BOARD_SIZE; i < 2 * BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			int banx = ((i >= BOARD_SIZE + 2 && boardInfo[i - 1][j].type == boardInfo[i - 2][j].type) ? (boardInfo[i - 2][j].type) : (-1));
			int bany = ((j >= 2 && boardInfo[i][j - 1].type == boardInfo[i][j - 2].type) ? (boardInfo[i][j - 2].type) : (-1));
			int typ = rand() % 6;
			while (typ == banx || typ == bany)typ = rand() % 6;
			boardInfo[i][j] = { typ, -1 ,createActor(typ,-1,getPosition({i,j })) };
		}
	}
}

void SpecialGameScene::doDrop() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (boardInfo[i][j].type == -1) {
				int ti = i;
				while (boardInfo[ti][j].type == -1) {
					ti++;
					assert(ti < 2 * BOARD_SIZE);
				}
				pii ep = getPosition({ i,j });
				boardInfo[ti][j].actor->dropTo(ep);
				swap(boardInfo[i][j], boardInfo[ti][j]);
			}
		}
	}
	runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), CCCallFunc::create([&]() {activeDoneCallback(); })));
}

void SpecialGameScene::doKill(KillGroupList killList) {

	//移除过程中，检测到的被触发的特殊型宝石暂存于此
	ActorInfoList specialActors;

	//1:移除过程
	for (KillGroup killGroup : killList) {
		//1.1展示分数
		//showScore(killGroup.markValue, killGroup.markPoint);
		//1.2移除所有得分块
		for (ActorInfo toDelActorInfo : killGroup.killInfo) {
			//delete
			pii pos = toDelActorInfo.pos;
			assert(boardInfo[pos.first][pos.second].type != -1);
			if (boardInfo[pos.first][pos.second].func != -1) {
				specialActors.push_back({ pos,boardInfo[pos.first][pos.second].type ,boardInfo[pos.first][pos.second].func });
			}

			if(boardInfo[pos.first][pos.second].type == 1 || boardInfo[pos.first][pos.second].type == 4 && boardInfo[pos.first][pos.second].func != FUNC_SUPER){
				showScoreA((boardInfo[pos.first][pos.second].type == 1) ? 0 : 1, 1, getPosition(pos));
			}

			boardInfo[pos.first][pos.second].actor->selfClose();
			boardInfo[pos.first][pos.second] = blockInfo();
			//placeDropper
			int ti = BOARD_SIZE;
			while (boardInfo[ti][pos.second].type != -1) {
				ti++;
				assert(ti < 2 * BOARD_SIZE);
			}
			int type = rand() % 6;
			auto droper = createActor(type, -1, getPosition({ ti,pos.second }));
			boardInfo[ti][pos.second] = blockInfo(type, -1, droper);
		}
		//1.3添加奖励特殊方块
		for (ActorInfo toAddActorInfo : killGroup.newInfo) {
			//create
			pii pos = toAddActorInfo.pos;
			assert(boardInfo[pos.first][pos.second].type == -1);
			auto newActor = createActor(toAddActorInfo.type, toAddActorInfo.func, getPosition(pos));
			boardInfo[pos.first][pos.second] = blockInfo(toAddActorInfo.type, toAddActorInfo.func, newActor);
			//deleteDropper
			int ti = 2 * BOARD_SIZE - 1;
			while (boardInfo[ti][pos.second].type == -1) {
				ti--;
				assert(ti >= BOARD_SIZE);
			}
			removeChild(boardInfo[ti][pos.second].actor);
			boardInfo[ti][pos.second] = blockInfo();
		}
	}

	//处理特殊方块的消除
	if (specialActors.empty()) {
		runAction(Sequence::createWithTwoActions(DelayTime::create(.3f), CCCallFunc::create([&]() {doDrop(); })));
	}
	else {
		KillGroupList causedKillList;
		set<pii> vis;

		for (auto specialActor : specialActors) {
			KillGroup nowKill = { specialActor.pos,114,{},{} };
			switch (specialActor.func) {
			case FUNC_H_1: {
				int i = specialActor.pos.first;
				showThunder_H(getPosition({ i,0 }).second);
				for (int j = 0; j < BOARD_SIZE; j++) {
					if (boardInfo[i][j].type == -1 || vis.count({ i,j }))continue;
					nowKill.killInfo.push_back(ActorInfo(i, j));
					if (boardInfo[i][j].func == FUNC_SUPER)boardInfo[i][j].type = specialActor.type;
					vis.insert({ i,j });
				}
				break;
			}
			case FUNC_V_1: {
				int j = specialActor.pos.second;
				showThunder_V(getPosition({ 0,j }).first);
				for (int i = 0; i < BOARD_SIZE; i++) {
					if (boardInfo[i][j].type == -1 || vis.count({ i,j }))continue;
					nowKill.killInfo.push_back(ActorInfo(i, j));
					if (boardInfo[i][j].func == FUNC_SUPER)boardInfo[i][j].type = specialActor.type;
					vis.insert({ i,j });
				}
				break;
			}
			case FUNC_H_3: {
				for (int i = specialActor.pos.first - 1; i <= specialActor.pos.first + 1; i++) {
					if (i<0 || i>BOARD_SIZE)continue;
					showThunder_H(getPosition({ i,0 }).second);
					for (int j = 0; j < BOARD_SIZE; j++) {
						if (boardInfo[i][j].type == -1 || vis.count({ i,j }))continue;
						nowKill.killInfo.push_back(ActorInfo(i, j));
						if (boardInfo[i][j].func == FUNC_SUPER)boardInfo[i][j].type = specialActor.type;
						vis.insert({ i,j });
					}
				}
				break;
			}
			case FUNC_V_3: {
				for (int j = specialActor.pos.second - 1; j <= specialActor.pos.second + 1; j++) {
					if (j<0 || j>BOARD_SIZE)continue;
					showThunder_V(getPosition({ 0,j }).first);
					for (int i = 0; i < BOARD_SIZE; i++) {
						if (boardInfo[i][j].type == -1 || vis.count({ i,j }))continue;
						nowKill.killInfo.push_back(ActorInfo(i, j));
						if (boardInfo[i][j].func == FUNC_SUPER)boardInfo[i][j].type = specialActor.type;
						vis.insert({ i,j });
					}
				}
				break;
			}
			case FUNC_FIRE: {
				showFire(getPosition(specialActor.pos));
				for (int i = specialActor.pos.first - 1; i <= specialActor.pos.first + 1; i++) {
					for (int j = specialActor.pos.second - 1; j <= specialActor.pos.second + 1; j++) {
						if (i < 0 || i >= BOARD_SIZE)continue;
						if (j < 0 || j >= BOARD_SIZE)continue;
						if (boardInfo[i][j].type == -1 || vis.count({ i,j }))continue;
						nowKill.killInfo.push_back(ActorInfo(i, j));
						if (boardInfo[i][j].func == FUNC_SUPER)boardInfo[i][j].type = specialActor.type;
						vis.insert({ i,j });
					}
				}
				break;
			}
			case FUNC_SUPER: {
				int toDelColor = specialActor.type;
				pii center = specialActor.pos;
				boardInfo[center.first][center.second].func = -1;

				for (int i = 0; i < BOARD_SIZE; i++) {
					for (int j = 0; j < BOARD_SIZE; j++) {
						if (boardInfo[i][j].type == toDelColor && boardInfo[i][j].func != FUNC_SUPER && !vis.count({ i,j })) {
							nowKill.killInfo.push_back(ActorInfo(i, j));
							showMelt(getPosition({ i,j }), toDelColor);
							vis.insert({ i,j });
						}
					}
				}
				break;
			}
			default: assert(false);
			}
			causedKillList.push_back(nowKill);
		}

		runAction(Sequence::createWithTwoActions(DelayTime::create(.2f), CCCallFunc::create([&, causedKillList]() {doKill(causedKillList); })));

	}

}

void SpecialGameScene::doFailSwap(pii blocka, pii blockb) {
	pii posa = getPosition(blocka);
	pii posb = getPosition(blockb);

	boardInfo[blocka.first][blocka.second].actor->moveToThenBack(posb);
	boardInfo[blockb.first][blockb.second].actor->moveToThenBack(posa);

	runAction(Sequence::createWithTwoActions(DelayTime::create(.65f), CCCallFunc::create([&]() {activeDoneCallback(); })));
}

void SpecialGameScene::doRealSwap(pii blocka, pii blockb) {
	pii posa = getPosition(blocka);
	pii posb = getPosition(blockb);

	boardInfo[blocka.first][blocka.second].actor->moveTo(posb);
	boardInfo[blockb.first][blockb.second].actor->moveTo(posa);

	swap(boardInfo[blocka.first][blocka.second], boardInfo[blockb.first][blockb.second]);

	runAction(Sequence::createWithTwoActions(DelayTime::create(.4f), CCCallFunc::create([&]() {activeDoneCallback(); })));
}

void SpecialGameScene::activeDoneCallback() {
	auto soveList = getKillList();
	if (soveList.empty()) {
		if (leftSwapTime == 0 || std::abs(nowScoreA[0] - nowScoreA[1]) >= 10) {
			auto backScene = EndGameLayer::create(EndGameLayer::TYPE_SPECIAL, 1, nowScoreA[0] + nowScoreA[1]);
			backScene->setRestartCallBack([&, backScene](Ref*) {
				removeChild(backScene);
				initBoard();
				doDrop();
			});
			this->addChild(backScene, 100);
		}
		else if (getHitList().size()) {
			boardLocked = false;
			CCLOG("[Unlock] Active done,Board Unlocked.");
		}
		else {
			CCLOG("No valid operate.");
			refreshBoard();
			doDrop();
		}
	}
	else {
		CCLOG("Block to kill found, call doKill().");
		doKill(soveList);
	}
}
/*
void SpecialGameScene::showScore(int val, pii pos) {
	auto LabelScore = Label::createWithSystemFont(std::to_string(val), "Arial", 36);
	pos = getPosition(pos);
	LabelScore->setPosition(pos.first, pos.second);
	LabelScore->enableShadow(Color4B::ORANGE);
	LabelScore->runAction(
		Sequence::create(
			Spawn::create(
				MoveBy::create(1.0f, { 40,40 }),
				FadeOut::create(1.0f),
				NULL),
			CallFunc::create([&, LabelScore]() {removeChild(LabelScore); }),
			NULL
		));
	addChild(LabelScore, 5);

	addScore(val);
}

void SpecialGameScene::addScore(int val) {
	CCLOG("Add score: %d", val);
	nowScore += val;
	//TODO：两个分数条的分数改变
	//SliderScore->setPercent(nowScore);
	EditBoxScore->setText(std::to_string(nowScore).c_str());
}
*/
void SpecialGameScene::showThunder_H(int h) {
	auto q = ParticleSystemQuad::create(SPECIAL_EFFECT_THUNDER);
	q->setPosition({ 530 + 640 / 2,static_cast<float>(h) });
	q->setScale(0.8f);
	q->setRotation(90);
	q->setAutoRemoveOnFinish(true);
	addChild(q, 400);
	Util::mycocos::audioWrapper.playEffc(AUDIO_THUNDER);
}

void SpecialGameScene::showThunder_V(int v) {
	auto q = ParticleSystemQuad::create(SPECIAL_EFFECT_THUNDER);
	q->setPosition({ static_cast<float>(v),360 });
	q->setScale(0.8f);
	q->setAutoRemoveOnFinish(true);
	addChild(q, 400);
	Util::mycocos::audioWrapper.playEffc(AUDIO_THUNDER);
}

void SpecialGameScene::showFire(pii p) {
	auto q = ParticleSystemQuad::create(SPECIAL_EFFECT_FIRE);
	q->setPosition({ static_cast<float>(p.first),static_cast<float>(p.second) });
	q->setScale(0.8f);
	q->setAutoRemoveOnFinish(true);
	addChild(q, 400);
	Util::mycocos::audioWrapper.playEffc(AUDIO_FIRE);
}

void SpecialGameScene::showMelt(pii pos, int type) {
	auto q = ParticleSystemQuad::create(SPECIAL_EFFECT_MELT_N[type]);
	q->setPosition({ static_cast<float>(pos.first),static_cast<float>(pos.second) });
	q->setScale(0.6f);
	q->setAutoRemoveOnFinish(true);
	addChild(q, 400);
	//Util::mycocos::audioWrapper.playEffc(AUDIO_FIRE);
}

void SpecialGameScene::showCircle(pii pos) {
	auto q = ParticleSystemQuad::create(SPECIAL_EFFECT_CIRCLE);
	q->setPosition({ static_cast<float>(pos.first),static_cast<float>(pos.second) });
	q->setScale(0.4f);
	q->setAutoRemoveOnFinish(true);
	addChild(q, 400);
}

pii SpecialGameScene::getPosition(pii ind) {
	return pii(530 + 64 * ind.second, 50 + 64 * ind.first);
}

pii SpecialGameScene::getIndex(pii pos) {
	if (pos.first < 500 || pos.first >= 500 + 640)return { -1,-1 };
	if (pos.second < 20 || pos.second >= 20 + 640)return { -1,-1 };
	return pii((pos.second - 20) / 64, (pos.first - 500) / 64);
}

Actor* SpecialGameScene::createActor(int typ, int spv, pii pos) {
	Actor* rtn = Actor::create(typ, spv, pos);
	addChild(rtn);
	return rtn;
}

KillGroupList SpecialGameScene::getKillList() {
	KillGroupList rtn;
	set<pii> vis;

	//check cross;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			int ilen = 1, jlen = 1;
			ActorInfoList tCrossActor;
			tCrossActor.push_back(ActorInfo(i, j));
			for (int ip = i + 1; ip < BOARD_SIZE && boardInfo[ip][j].type == boardInfo[i][j].type && !vis.count({ ip,j }) && boardInfo[ip][j].func != FUNC_SUPER; ip++) { ilen++; tCrossActor.push_back(ActorInfo(ip, j)); }
			for (int ip = i - 1; ip >= 0 && boardInfo[ip][j].type == boardInfo[i][j].type && !vis.count({ ip,j }) && boardInfo[ip][j].func != FUNC_SUPER; ip--) { ilen++; tCrossActor.push_back(ActorInfo(ip, j)); }
			for (int jp = j + 1; jp < BOARD_SIZE && boardInfo[i][jp].type == boardInfo[i][j].type && !vis.count({ i,jp }) && boardInfo[i][jp].func != FUNC_SUPER; jp++) { jlen++; tCrossActor.push_back(ActorInfo(i, jp)); }
			for (int jp = j - 1; jp >= 0 && boardInfo[i][jp].type == boardInfo[i][j].type && !vis.count({ i,jp }) && boardInfo[i][jp].func != FUNC_SUPER; jp--) { jlen++; tCrossActor.push_back(ActorInfo(i, jp)); }
			if (ilen == 3 && jlen == 3) {
				rtn.push_back({ {i,j},500, tCrossActor ,{ActorInfo(i,j,boardInfo[i][j].type,FUNC_FIRE)} });
			}
			else if (ilen >= 3 && jlen >= 3) {
				rtn.push_back({ {i,j},1000, tCrossActor ,{ActorInfo(i,j,boardInfo[i][j].type,FUNC_SUPER)} });
			}
			if (ilen >= 3 && jlen >= 3) {
				for (auto elm : tCrossActor) {
					vis.insert(elm.pos);
				}
			}
		}
	}

	// check 6 link;
	//砍了，具体看info
	//BUG: 6消不能正确识别。
	//check 5 link;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i > 1 && i < BOARD_SIZE - 2)
				&& (boardInfo[i - 1][j].type == boardInfo[i][j].type && boardInfo[i][j].type == boardInfo[i + 1][j].type && boardInfo[i][j].type == boardInfo[i + 2][j].type && boardInfo[i][j].type == boardInfo[i - 2][j].type)
				&& (boardInfo[i - 1][j].func != FUNC_SUPER && boardInfo[i][j].func != FUNC_SUPER && boardInfo[i + 1][j].func != FUNC_SUPER && boardInfo[i + 2][j].func != FUNC_SUPER && boardInfo[i - 2][j].func != FUNC_SUPER)
				&& (!vis.count({ i - 1,j }) && !vis.count({ i,j }) && !vis.count({ i + 1,j }) && !vis.count({ i + 2,j }) && !vis.count({ i - 2,j }))) {
				vis.insert({ i - 1,j }); vis.insert({ i,j }); vis.insert({ i + 1,j }); vis.insert({ i + 2,j }); vis.insert({ i - 2,j });
				rtn.push_back({ {i,j},200,{ ActorInfo(i - 1,j),ActorInfo(i,j),ActorInfo(i + 1,j),ActorInfo(i + 2,j),ActorInfo(i - 2,j) },{ActorInfo(i,j,boardInfo[i][j].type,FUNC_H_3)} });
			}
			if ((j > 1 && j < BOARD_SIZE - 2)
				&& (boardInfo[i][j - 1].type == boardInfo[i][j].type && boardInfo[i][j].type == boardInfo[i][j + 1].type && boardInfo[i][j].type == boardInfo[i][j + 2].type && boardInfo[i][j].type == boardInfo[i][j - 2].type)
				&& (boardInfo[i][j - 1].func != FUNC_SUPER && boardInfo[i][j].func != FUNC_SUPER && boardInfo[i][j + 1].func != FUNC_SUPER && boardInfo[i][j + 2].func != FUNC_SUPER && boardInfo[i][j - 2].func != FUNC_SUPER)
				&& (!vis.count({ i,j - 1 }) && !vis.count({ i,j }) && !vis.count({ i,j + 1 }) && !vis.count({ i,j + 2 }) && !vis.count({ i,j - 2 }))) {
				vis.insert({ i,j - 1 }); vis.insert({ i,j }); vis.insert({ i ,j + 1 }); vis.insert({ i ,j + 2 }); vis.insert({ i ,j - 2 });
				rtn.push_back({ {i,j},200,{ ActorInfo(i,j - 1),ActorInfo(i,j),ActorInfo(i,j + 1),ActorInfo(i,j + 2),ActorInfo(i,j - 2) },{ActorInfo(i,j,boardInfo[i][j].type,FUNC_V_3)} });
			}
		}
	}

	// check 4 link;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i > 0 && i < BOARD_SIZE - 2)
				&& (boardInfo[i - 1][j].type == boardInfo[i][j].type && boardInfo[i][j].type == boardInfo[i + 1][j].type && boardInfo[i][j].type == boardInfo[i + 2][j].type)
				&& (boardInfo[i - 1][j].func != FUNC_SUPER && boardInfo[i][j].func != FUNC_SUPER && boardInfo[i + 1][j].func != FUNC_SUPER && boardInfo[i + 2][j].func != FUNC_SUPER)
				&& (!vis.count({ i - 1,j }) && !vis.count({ i,j }) && !vis.count({ i + 1,j }) && !vis.count({ i + 2,j }))) {
				vis.insert({ i - 1,j }); vis.insert({ i,j }); vis.insert({ i + 1,j }); vis.insert({ i + 2,j });
				rtn.push_back({ {i,j},200,{ ActorInfo(i - 1,j),ActorInfo(i,j),ActorInfo(i + 1,j),ActorInfo(i + 2,j) },{ActorInfo(i,j,boardInfo[i][j].type,FUNC_H_1)} });
			}
			if ((j > 0 && j < BOARD_SIZE - 2)
				&& (boardInfo[i][j - 1].type == boardInfo[i][j].type && boardInfo[i][j].type == boardInfo[i][j + 1].type && boardInfo[i][j].type == boardInfo[i][j + 2].type)
				&& (boardInfo[i][j - 1].func != FUNC_SUPER && boardInfo[i][j].func != FUNC_SUPER && boardInfo[i][j + 1].func != FUNC_SUPER && boardInfo[i][j + 2].func != FUNC_SUPER)
				&& (!vis.count({ i,j - 1 }) && !vis.count({ i,j }) && !vis.count({ i,j + 1 }) && !vis.count({ i,j + 2 }))) {
				vis.insert({ i,j - 1 }); vis.insert({ i,j }); vis.insert({ i ,j + 1 }); vis.insert({ i ,j + 2 });
				rtn.push_back({ {i,j},200,{ ActorInfo(i,j - 1),ActorInfo(i,j),ActorInfo(i,j + 1),ActorInfo(i,j + 2) },{ActorInfo(i,j,boardInfo[i][j].type,FUNC_V_1)} });
			}
		}
	}

	//check 3 link;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i > 0 && i < BOARD_SIZE - 1)
				&& (boardInfo[i - 1][j].type == boardInfo[i][j].type && boardInfo[i][j].type == boardInfo[i + 1][j].type)
				&& (boardInfo[i - 1][j].func != FUNC_SUPER && boardInfo[i][j].func != FUNC_SUPER && boardInfo[i + 1][j].func != FUNC_SUPER)
				&& (!vis.count({ i - 1,j }) && !vis.count({ i,j }) && !vis.count({ i + 1,j }))) {
				vis.insert({ i - 1,j }); vis.insert({ i,j }); vis.insert({ i + 1,j });
				rtn.push_back({ {i,j},100,{ ActorInfo(i - 1,j),ActorInfo(i,j),ActorInfo(i + 1,j) },{} });
			}
			if ((j > 0 && j < BOARD_SIZE - 1)
				&& (boardInfo[i][j - 1].type == boardInfo[i][j].type && boardInfo[i][j].type == boardInfo[i][j + 1].type)
				&& (boardInfo[i][j - 1].func != FUNC_SUPER && boardInfo[i][j].func != FUNC_SUPER && boardInfo[i][j + 1].func != FUNC_SUPER)
				&& (!vis.count({ i,j - 1 }) && !vis.count({ i,j }) && !vis.count({ i,j + 1 }))) {
				vis.insert({ i,j - 1 }); vis.insert({ i,j }); vis.insert({ i ,j + 1 });
				rtn.push_back({ {i,j},100,{ ActorInfo(i,j - 1),ActorInfo(i,j),ActorInfo(i,j + 1) },{} });
			}
		}
	}

	return rtn;
}

bool SpecialGameScene::getKillable() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i > 0 && i < BOARD_SIZE - 1)
				&& (boardInfo[i - 1][j].type == boardInfo[i][j].type && boardInfo[i][j].type == boardInfo[i + 1][j].type)
				&& (boardInfo[i - 1][j].func != FUNC_SUPER && boardInfo[i][j].func != FUNC_SUPER && boardInfo[i + 1][j].func != FUNC_SUPER)
				) {
				return true;
			}
			if ((j > 0 && j < BOARD_SIZE - 1)
				&& (boardInfo[i][j - 1].type == boardInfo[i][j].type && boardInfo[i][j].type == boardInfo[i][j + 1].type)
				&& (boardInfo[i][j - 1].func != FUNC_SUPER && boardInfo[i][j].func != FUNC_SUPER && boardInfo[i][j + 1].func != FUNC_SUPER)
				) {
				return true;
			}
		}
	}
	return false;
}

validOperateList SpecialGameScene::getHitList() {
	validOperateList rtn;
	auto beginTime = clock();

	/*
	// 130ms
	for (int i = 1; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if(boardInfo[i][j].func == FUNC_SUPER || boardInfo[i - 1][j].func == FUNC_SUPER){
				rtn.push_back({ {i,j} ,{i - 1,j} });
				continue;
			}
			swap(boardInfo[i][j], boardInfo[i-1][j]);
			if (getKillList().size())rtn.push_back({ {i,j} ,{i - 1,j} });
			swap(boardInfo[i][j], boardInfo[i - 1][j]);
		}
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 1; j < BOARD_SIZE; j++) {
			if (boardInfo[i][j].func == FUNC_SUPER || boardInfo[i][j - 1].func == FUNC_SUPER) {
				rtn.push_back({ {i,j} ,{i,j - 1} });
				continue;
			}
			swap(boardInfo[i][j], boardInfo[i][j-1]);
			if (getKillList().size())rtn.push_back({ {i,j} ,{i,j-1} });
			swap(boardInfo[i][j], boardInfo[i][j-1]);
		}
	}
	*/

	//1ms
	for (int i = 1; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (boardInfo[i][j].func == FUNC_SUPER || boardInfo[i - 1][j].func == FUNC_SUPER) {
				rtn.push_back({ {i,j} ,{i - 1,j} });
				continue;
			}
			swap(boardInfo[i][j], boardInfo[i - 1][j]);
			if (getKillable())rtn.push_back({ {i,j} ,{i - 1,j} });
			swap(boardInfo[i][j], boardInfo[i - 1][j]);
		}
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 1; j < BOARD_SIZE; j++) {
			if (boardInfo[i][j].func == FUNC_SUPER || boardInfo[i][j - 1].func == FUNC_SUPER) {
				rtn.push_back({ {i,j} ,{i,j - 1} });
				continue;
			}
			swap(boardInfo[i][j], boardInfo[i][j - 1]);
			if (getKillable())rtn.push_back({ {i,j} ,{i,j - 1} });
			swap(boardInfo[i][j], boardInfo[i][j - 1]);
		}
	}

	auto endTime = clock();

	CCLOG("Hit calc time: %dms.", endTime - beginTime);

	return rtn;
}

void SpecialGameScene::updateSwapTime() {
	LabSwap->setString(std::to_string(leftSwapTime));
}

void SpecialGameScene::showScoreA(int type, int val, pii pos){
	auto LabelScore = Label::createWithSystemFont(std::to_string(val), "Arial", 36);
	pos = getPosition(pos);
	LabelScore->setPosition(pos.first, pos.second);
	LabelScore->setTextColor(type ? Color4B::BLUE : Color4B::RED);
	LabelScore->enableShadow(Color4B::ORANGE);
	LabelScore->runAction(
		Sequence::create(
			Spawn::create(
				MoveBy::create(1.0f, { 40,40 }),
				FadeOut::create(1.0f),
				NULL),
			CallFunc::create([&, LabelScore]() {removeChild(LabelScore); }),
			NULL
		));
	addChild(LabelScore, 5);
	addScoreA(type,val);
}

void SpecialGameScene::addScoreA(int type, int val){
	nowScoreA[type] += val;
	updateScore();
}

void SpecialGameScene::updateScore(){
	SliderBlueScore->setPercent(nowScoreA[0]);
	SliderRedScore->setPercent(nowScoreA[1]);
	LabScore[0]->setString(std::to_string(nowScoreA[0]));
	LabScore[1]->setString(std::to_string(nowScoreA[1]));
}

void SpecialGameScene::settingSaveCallBack(Node* pNode) {
	if (pNode->getTag() == 1)
		User::writeOut();
}

void SpecialGameScene::backButtonCallBack(Node* pNode) {
	if (pNode->getTag() == 1)
		Director::getInstance()->popScene();
}

