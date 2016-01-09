#include "stdafx.h"
#include "ArenaController.h"
using namespace std;

ArenaController::ArenaController(string sceneName, string Datapath, int goal, int mapid, int width, int height, string player1, string player2, int powerupAmount){
	createScene(sceneName, Datapath, mapid, width, height);
	arenaLoadCharacter(player1,player2);
	loadPowerupItem(powerupAmount);
}

void ArenaController::createScene(string sceneName, string Datapath, int mapid, int width, int height) {
	// load the scene
	char sceneNamestr[200];
	strncpy(Datapathstr, Datapath.c_str(), sizeof(Datapathstr));
	Datapathstr[sizeof(Datapathstr) - 1] = 0;
	FySetModelPath(strcat(Datapathstr, "\\Scenes"));
	FySetTexturePath(strcat(Datapathstr, "\\Scenes\\Textures"));
	FySetScenePath(strcat(Datapathstr, "\\Scenes"));
	vid1 = FyCreateViewport(0, 0, width, height);
	FnViewport vp1;
	vp1.ID(vid1);
	sid = FyCreateScene(100);
	FnScene scene;
	scene.ID(sid);
	strncpy(sceneNamestr, sceneName.c_str(), sizeof(sceneNamestr));
	sceneNamestr[sizeof(sceneNamestr) - 1] = 0;
	scene.Load(sceneNamestr);
	scene.SetAmbientLights(1.0f, 1.0f, 1.0f, 0.6f, 0.6f, 0.6f); //Sky Light(1.0f), Ground Light (0.6f)
	tID = scene.CreateObject(OBJECT);
	FnObject terrain;
	terrain.ID(tID);
	BOOL beOK1 = terrain.Load("terrain");
	terrain.Show(FALSE); // Can comment out this line of code to show the path which user can walk
	terrainRoomID = scene.CreateRoom(SIMPLE_ROOM, 10);
	FnRoom room;
	room.ID(terrainRoomID);
	room.AddObject(tID);
	//finish loading the scene, let's load the map
	char mapfile[300];
	sprintf(mapfile,"%s\\map\\map%d.txt", Datapathstr, mapid);
	string mapstring(mapfile);
	map.constructMap(mapstring);
}

void ArenaController::arenaLoadCharacter(string player1, string player2) {
	FnScene scene;
	scene.ID(sid);
	char player1c[50], player2c[50];
	strtoChar(player1c,player1);
	ch1 = scene.LoadCharacter(player1c);
	strtoChar(player2c, player2);
	ch2 = scene.LoadCharacter(player2c);
	Character character1, character2;
	character1.ID(ch1);
	character2.ID(ch2);
	character_Control.addCharacter(character1, ch1);
	character_Control.addCharacter(character2, ch2);
	character1.initializeAction();
	character2.initializeAction();
}

void ArenaController::loadPowerupItem(int amount) {
	
}