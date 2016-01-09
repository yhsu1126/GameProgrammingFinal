#pragma once
#include "stdafx.h"
using namespace std;

void ArenaController::startGame(string sceneName, string Datapath, int goal, int mapid, int width, int height, string player1, string player2, int powerupAmount){
	createScene(sceneName, Datapath, mapid, width, height);
	arenaLoadCharacter(player1,player2);
	setupCamera();
	OutputDebugString(_T("Finish doing the job 0w0\n"));
	//loadPowerupItem(powerupAmount);
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
	//set light
	float mainLightPos[3] = { -4579.0, -714.0, 15530.0 };
	float mainLightFDir[3] = { 0.276, 0.0, -0.961 };
	float mainLightUDir[3] = { 0.961, 0.026, 0.276 };
	
	FnLight lgt;
	lgt.ID(scene.CreateObject(LIGHT));
	lgt.Translate(mainLightPos[0], mainLightPos[1], mainLightPos[2], REPLACE);
	lgt.SetDirection(mainLightFDir, mainLightUDir);
	lgt.SetLightType(PARALLEL_LIGHT);
	lgt.SetColor(1.0f, 1.0f, 1.0f);
	lgt.SetName("MainLight");
	lgt.SetIntensity(0.4f);
	//finish loading the scene, let's load the map
	char mapfile[300];
	sprintf(mapfile,"%s\\map\\map%d.txt", Datapathstr, mapid);
	string mapstring(mapfile);
	map.constructMap(mapstring);
}

void ArenaController::arenaLoadCharacter(string player1, string player2) {
	//initialize the action the character have and create the character
	FnScene scene;
	scene.ID(sid);
	char player1c[50], player2c[50];
	strtoChar(player1c, player1);
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
	//put the character on the map and do the default action
	character1.idle();
	character2.idle();
	float pos1[3], pos2[3], fDir1[3], fDir2[3], uDir[3];
	map.getPos1(pos1);
	map.getfDir1(fDir1);
	map.getPos2(pos2);
	map.getfDir2(fDir2);
	map.getuDir(uDir);
	character1.SetTerrainRoom(terrainRoomID, 10.0f);
	character2.SetTerrainRoom(terrainRoomID, 10.0f);
	character1.PutOnTerrain(pos1);
	character2.PutOnTerrain(pos2);
	character1.SetDirection(fDir1, uDir);
	character2.SetDirection(fDir2, uDir);
	character_Control.startPlayAction();
}

void ArenaController::setupCamera() {
	FnScene scene;
	scene.ID(sid);
	cID = scene.CreateObject(CAMERA);
	FnCamera camera;
	camera.ID(cID);
	camera.SetNearPlane(5.0f);
	camera.SetFarPlane(100000.0f);
	float cameraPos[3], camerafDir[3], camerauDir[3];
	float horDistance = cameraDis*cos(elevation*PI / 180.0);
	float verDistance = cameraDis*sin(elevation*PI / 180.0);
	Character mainPlayer;
	mainPlayer.ID(ch1);
	float mainPos[3], mainuDir[3], mainfDir[3];
	mainPlayer.GetPosition(mainPos);
	mainPlayer.GetDirection(mainfDir, mainuDir);
	//set camera position
	cameraPos[0] = mainPos[0] - mainfDir[0] * horDistance;
	cameraPos[1] = mainPos[1] - mainfDir[1] * horDistance;
	cameraPos[2] = verDistance;
	//set camera direction
	mainfDir[0] = mainPos[0] - cameraPos[0];
	mainfDir[1] = mainPos[1] - cameraPos[1];
	mainfDir[2] = -cameraPos[2];
	unitVector(mainfDir, 3);
	//set camera up
	if (mainfDir[2] == 0) {
		mainuDir[0] = 0;
		mainuDir[1] = 0;
		mainuDir[2] = 1;
	}
	else {
		mainuDir[0] = mainfDir[0];
		mainuDir[1] = mainfDir[1];
		mainuDir[2] = -(mainfDir[0] * mainfDir[0] + mainfDir[1] * mainfDir[1]) / mainfDir[2];
	}
	//fix the camerapos high
	cameraPos[2] += 60.0f;
	camera.SetPosition(cameraPos);
	camera.SetDirection(camerafDir, camerauDir);
}

void ArenaController::renderArena(int skip) {
	FnViewport vp;
	// render the whole scene
	vp.ID(vid1);
	vp.Render3D(cID, TRUE, TRUE);
	FySwapBuffers();
}

void ArenaController::loadPowerupItem(int amount) {
	
}