#pragma once
#include "stdafx.h"
using namespace std;

class ArenaController{
private:
	int firstPlayer;
	int mapid;
	int score1;
	int score2;
	VIEWPORTid vid1, vid2;
	CHARACTERid ch1, ch2;
	SCENEid sid;
	Map map;
	OBJECTid tID;
	ROOMid terrainRoomID = FAILED_ID;
	char Datapathstr[200];
	CharacterController character_Control;

public:
	ArenaController(string sceneName, string Datapath, int goal, int mapid, int width, int height, string player1, string player2, int powerupAmount);
	void createScene(string sceneName, string Datapath, int mapid, int width, int height);
	void arenaLoadCharacter(string player1, string player2);
	void loadPowerupItem(int amount);
};