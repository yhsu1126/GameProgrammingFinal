/*==============================================================
character movement testing using Fly2

- Load a scene
- Generate a terrain object
- Load a character
- Control a character to move
- Change poses

(C)2012-2015 Chuan-Chang Wang, All Rights Reserved
Created : 0802, 2012

Last Updated : 1004, 2015, Kevin C. Wang
===============================================================*/
#include "stdafx.h"
using namespace std;

VIEWPORTid vID;                 // the major viewport
SCENEid sID;                    // the 3D scene
OBJECTid cID, tID;              // the main camera and the terrain for terrain following
OBJECTid dID;					//#dummy object for control character dir##
CHARACTERid actorID, enemyID, scoutID;            // the major character
ACTIONid idleID, runID, curPoseID, ultimateAttack, enemyIdle, damageL, damageH, dieID, enemyPosID; // two actions
ACTIONid normalAttackID[4], heavyAttackID[3];
ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;
BOOL4 killed = FALSE;
std::map<ACTIONid, int>actionLength, hitTest, actionDamage, hitTime, attackRange; // hitTest is the Interval for being hitted

// Media
MEDIAid mmID;
AUDIOid backgroundSoundID, endGameSoundID;
AUDIOid footStepSoundID, weapon1SoundID, weapon2SoundID;
AUDIOid lyubuDeathSoundID, donzoDeathSoundID;
FnAudio backgroundSound, endGameSound;
FnAudio footStepSound, weapon1Sound, weapon2Sound;
FnAudio lyubuDeathSound, donzoDeathSound;

// the centre of the rotation
// float centre[3];
float dist = 1000.0f, height = 75.0f, rotatedistancefactor = 0.017453f;
// some globals
int frame = 0, direction = 1; // the global variable for direction
int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;
float cameraDis = 1000.0f, elevation = 0.0f, collision[3] = { 0.0f }, collisionDis, oldcollisionDis;
int test_forward, test_right;
bool toward_wall, away_wall;
int lyubuAction = 0, attackCombo = 0, donzoAction = 0;
int enemyhp = 100;
// hotkey callbacks
void QuitGame(BYTE, BOOL4);
void Movement(BYTE, BOOL4); // The main stuff to do at here
void Attack(BYTE, BOOL4); // the function to control the attack movement
void die(); // just kill the enemy and make sure that he disappeared

// timer callbacks
// mouse callbacks
void InitElevation(int, int);
void ElevationCam(int, int);
void DistanceCam(int, int, float);
int oldScroll, newScroll; //for test Mouse Scroll argument
float dirScroll; //for test Mouse Scroll argument
//camera follow function
void followCam();
void adjustCam();
void enemyFollow(float*);
float distBetweenTwoPoints(float*, float*);

void createAudio( void ) {
	backgroundSoundID = FyCreateAudio();
	backgroundSound.ID( backgroundSoundID );
	backgroundSound.Load( "background" );
	backgroundSound.Play( LOOP );
	backgroundSound.SetVolume( 0.1 );
	
	endGameSoundID = FyCreateAudio();
	endGameSound.ID(endGameSoundID);
	endGameSound.Load("endGameSound");
	
	footStepSoundID = FyCreateAudio();
	footStepSound.ID(footStepSoundID);
	footStepSound.Load("footStep");
	
	weapon1SoundID = FyCreateAudio();
	weapon1Sound.ID(weapon1SoundID);
	weapon1Sound.Load("weapon1");
	
	weapon2SoundID = FyCreateAudio();
	weapon2Sound.ID(weapon2SoundID);
	weapon2Sound.Load("weapon2");
	
	lyubuDeathSoundID = FyCreateAudio();
	lyubuDeathSound.ID(lyubuDeathSoundID);
	lyubuDeathSound.Load("lyubuDeath");
	
	donzoDeathSoundID = FyCreateAudio();
	donzoDeathSound.ID(donzoDeathSoundID);
	donzoDeathSound.Load("donzoDeath");
}
/*------------------
the main program
C.Wang 1010, 2014
-------------------*/
void FyMain(int argc, char **argv)
{
	// create a new world
	BOOL4 beOK = FyStartFlyWin32("DADA urara", 0, 0, 1024, 768, FALSE);
	// setup the data searching paths
	FySetShaderPath("Data\\Shaders");
	FySetModelPath("Data\\Scenes");
	FySetTexturePath("Data\\Scenes\\Textures");
	FySetScenePath("Data\\Scenes");
	FySetAudioPath("Data\\Media");
	FyBeginMedia("Data\\Media", 2);
	
	// Load Media
	createAudio();
	
	// create a viewport
	vID = FyCreateViewport(0, 0, 1024, 768);
	FnViewport vp;
	vp.ID(vID);

	// create a 3D scene
	sID = FyCreateScene(100);
	FnScene scene;
	scene.ID(sID);

	// load the scene
	scene.Load("gameScene02");
	//scene.Load("terrain");
	scene.SetAmbientLights(1.0f, 1.0f, 1.0f, 0.6f, 0.6f, 0.6f); //Sky Light(1.0f), Ground Light (0.6f)

	// load the terrain
	tID = scene.CreateObject(OBJECT);
	FnObject terrain;
	terrain.ID(tID);
	BOOL beOK1 = terrain.Load("terrain");
	terrain.Show(FALSE); // Can comment out this line of code to show the path which user can walk

	// set terrain environment
	terrainRoomID = scene.CreateRoom(SIMPLE_ROOM, 10);
	FnRoom room;
	room.ID(terrainRoomID);
	room.AddObject(tID);
	// load the character
	FySetModelPath("Data\\Characters");
	FySetTexturePath("Data\\Characters");
	FySetCharacterPath("Data\\Characters");
	actorID = scene.LoadCharacter("Lyubu2");
	enemyID = scene.LoadCharacter("Donzo2");

	// put the character on terrain
	float pos[3], fDir[3], uDir[3], enemypos[3], scoutpos[3];
	FnCharacter actor, enemy;
	actor.ID(actorID);
	enemy.ID(enemyID);

	fstream infile;
	infile.open("Data\\Scenes\\Map\\map0.txt", ios::in);
	infile >> pos[0] >> pos[1] >> pos[2];
	infile >> fDir[0] >> fDir[1] >> fDir[2];
	uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;
	infile >> enemypos[0] >> enemypos[1] >> enemypos[2];
	/*pos[0] = 3487.873; pos[1] = -3639.062f; pos[2] = 1000.0f;
	enemypos[0] = 3540.0f; enemypos[1] = -4300.0f; enemypos[2] = 1000.0f;
	scoutpos[0] = 3540.0f; scoutpos[1] = -3070.0f; scoutpos[2] = 1000.0f;*/
	fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
	actor.SetDirection(fDir, uDir);
	enemy.SetDirection(fDir, uDir);

	actor.SetTerrainRoom(terrainRoomID, 10.0f);
	enemy.SetTerrainRoom(terrainRoomID, 10.0f);
	char msgbuf[1000];
	beOK = enemy.PutOnTerrain(enemypos);
	sprintf(msgbuf, "beok : %d\n", beOK);
	OutputDebugString(_T(msgbuf));
	beOK = actor.PutOnTerrain(pos);
	sprintf(msgbuf, "beok : %d\n", beOK);
	OutputDebugString(_T(msgbuf));

	// Get character actions pre-defined at Lyubu2
	idleID = actor.GetBodyAction(NULL, "Idle");
	runID = actor.GetBodyAction(NULL, "Run");
	normalAttackID[0] = actor.GetBodyAction(NULL, "NormalAttack1");
	normalAttackID[1] = actor.GetBodyAction(NULL, "NormalAttack2");
	normalAttackID[2] = actor.GetBodyAction(NULL, "NormalAttack3");
	normalAttackID[3] = actor.GetBodyAction(NULL, "NormalAttack4");
	heavyAttackID[0] = actor.GetBodyAction(NULL, "HeavyAttack1");
	heavyAttackID[1] = actor.GetBodyAction(NULL, "HeavyAttack2");
	heavyAttackID[2] = actor.GetBodyAction(NULL, "HeavyAttack3");
	ultimateAttack = actor.GetBodyAction(NULL, "UltimateAttack");
	damageL = enemy.GetBodyAction(NULL, "DamageL");
	damageH = enemy.GetBodyAction(NULL, "DamageH");
	enemyIdle = enemy.GetBodyAction(NULL, "Idle");
	dieID = enemy.GetBodyAction(NULL, "Die");
	// insert the length of these games into a HashMap for tracking
	actionLength[normalAttackID[0]] = 145 - 121;
	actionLength[normalAttackID[1]] = 218 - 171;
	actionLength[normalAttackID[2]] = 276 - 231;
	actionLength[normalAttackID[3]] = 339 - 291;
	actionLength[heavyAttackID[0]] = 423 - 351;
	actionLength[heavyAttackID[1]] = 489 - 431;
	actionLength[heavyAttackID[2]] = 557 - 501;
	actionLength[ultimateAttack] = 1091 - 971;
	//insert the hitTest of these action
	hitTest[normalAttackID[0]] = 17;
	hitTest[normalAttackID[1]] = 23;
	hitTest[normalAttackID[2]] = 17;
	hitTest[normalAttackID[3]] = 24;
	hitTest[heavyAttackID[0]] = 36;
	hitTest[heavyAttackID[1]] = 29;
	hitTest[heavyAttackID[2]] = 29;
	hitTest[ultimateAttack] = 12;
	//add the damage
	actionDamage[normalAttackID[0]] = 1;
	actionDamage[normalAttackID[1]] = 1;
	actionDamage[normalAttackID[2]] = 1;
	actionDamage[normalAttackID[3]] = 1;
	actionDamage[heavyAttackID[0]] = 2;
	actionDamage[heavyAttackID[1]] = 2;
	actionDamage[heavyAttackID[2]] = 2;
	actionDamage[ultimateAttack] = 2;
	//set the attack Range
	attackRange[normalAttackID[0]] = 115;
	attackRange[normalAttackID[1]] = 115;
	attackRange[normalAttackID[2]] = 115;
	attackRange[normalAttackID[3]] = 115;
	attackRange[heavyAttackID[0]] = 115;
	attackRange[heavyAttackID[1]] = 115;
	attackRange[heavyAttackID[2]] = 115;
	attackRange[ultimateAttack] = 115;
	// enemy actor
	actionLength[damageL] = 160 - 135;
	actionLength[damageH] = 200 - 165;
	actionLength[dieID] = 550 - 380;

	// set the character to idle action
	curPoseID = idleID;
	actor.SetCurrentAction(NULL, 0, curPoseID);
	actor.Play(START, 0.0f, FALSE, TRUE);
	actor.TurnRight(90.0f);
	actor.GetDirection(fDir, uDir);
	enemyPosID = enemyIdle;
	enemy.SetCurrentAction(NULL, 0, enemyPosID);
	enemy.Play(START, 0.0f, FALSE, TRUE);

	// translate the camera
	cID = scene.CreateObject(CAMERA);
	FnCamera camera;
	camera.ID(cID);
	camera.SetNearPlane(5.0f);
	camera.SetFarPlane(100000.0f);
	camera.SetParent(actorID);
	// set dummy position and direction to actor
	dID = scene.CreateObject(OBJECT);
	FnObject dummy;
	dummy.SetParent(actorID);
	float tem[2][3];
	actor.GetPosition(tem[0]);
	dummy.SetPosition(tem[0]);
	actor.GetDirection(tem[0], tem[1]);
	dummy.SetDirection(tem[0], tem[1]);

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

	// create a text object for displaying messages on screen
	textID = FyCreateText("Trebuchet MS", 18, FALSE, FALSE);

	// set Hotkeys
	FyDefineHotKey(VK_ESCAPE, QuitGame, FALSE);  // escape for quiting the game
	//FyDefineHotKey(VK_UP, Movement, FALSE);      // Up for moving forward
	FyDefineHotKey(VK_RIGHT, Movement, FALSE);   // Right for turning right
	FyDefineHotKey(VK_LEFT, Movement, FALSE);    // Left for turning left
	//FyDefineHotKey(VK_DOWN, Movement, FALSE);
	FyDefineHotKey(FY_A, Attack, FALSE);
	FyDefineHotKey(FY_S, Attack, FALSE);
	//FyDefineHotKey(FY_D, Attack, FALSE);

	// define some mouse functions
	FyBindMouseFunction(RIGHT_MOUSE, InitElevation, ElevationCam, NULL, NULL);
	FyBindMouseWheelFunction(DistanceCam);
	// bind timers, frame rate = 30 fps
	FyBindTimer(0, 30.0f, GameAI, TRUE);
	FyBindTimer(1, 30.0f, RenderIt, TRUE);
	// invoke the system
	FyInvokeFly(TRUE);
}


/*-------------------------------------------------------------
30fps timer callback in fixed frame rate for major game loop
--------------------------------------------------------------*/
void GameAI(int skip)
{
	FnCharacter character, enemy;
	FnObject dummy;
	character.ID(actorID);
	dummy.ID(dID);
	enemy.ID(enemyID);
	// play character pose

	character.Play(LOOP, (float)skip, FALSE, TRUE);
	enemy.Play(LOOP, (float)skip, FALSE, TRUE);

	/****************
	process key conflict
	forward: 1=forward 0=stand -1=backward
	right: 1=turn right 0=stand -1=turn left
	direction : 1 = same direction as camera -1 = opposite direction of camera
	*****************/
	int forward = 0, right = 0;
	if (FyCheckHotKeyStatus(VK_RIGHT))
		right += 1;
	if (FyCheckHotKeyStatus(VK_LEFT))
		right -= 1;
	test_forward = forward;
	test_right = right;
	/****************
	direction means the character is facing the camera or backing the camera
	we use left or right to turn the character left or right
	down will turn the character to the camera and up will move forward
	****************/
	float dist = 6.0f;
	float fDir[3], uDir[3];
	// 
	if (forward != 0 || right != 0)
	{
		dummy.GetDirection(fDir, uDir);
		if (direction <= 0) {
			for (int i = 0; i < 3; i++) {
				fDir[i] = -1 * fDir[i];
			}
		}
		character.SetDirection(fDir, uDir);
	}
	if (right>0)
	{
		//direction>0 ? dummy.TurnRight(2.0f*(float)direction) : dummy.TurnRight(-2.0f*(float)direction);
		//character.TurnRight(2.0f);
		character.TurnRight(90.0f);
		direction = 2;
	}
	else if (right<0)
	{
		//direction>0 ? dummy.TurnRight(-2.0f*(float)direction) : dummy.TurnRight(2.0f*(float)direction);
		//character.TurnRight(-2.0f);
		character.TurnRight(-90.0f);
		direction = 3;

	}
	if (right != 0 && (character.GetCurrentAction(NULL) == runID || character.GetCurrentAction(NULL) == ultimateAttack)) {
		character.MoveForward(dist, TRUE, FALSE, 0.0f, TRUE);
	}
	else {
		if (direction == 3) {
			character.TurnRight(90.0f);
		}
		else if (direction == 2) {
			character.TurnRight(-90.0f);
		}
		direction = 1;
	}
	float pos[3];
	character.GetPosition(pos);
	dummy.SetPosition(pos);
	followCam();
	adjustCam();
	enemyFollow(pos);
}


/*----------------------
perform the rendering
C.Wang 0720, 2006
-----------------------*/
void RenderIt(int skip)
{
	FnViewport vp;

	// render the whole scene
	vp.ID(vID);
	vp.Render3D(cID, TRUE, TRUE);

	// get camera's data
	FnCamera camera;
	FnCharacter character, enemy;
	character.ID(actorID);
	enemy.ID(enemyID);

	camera.ID(cID);

	float pos[3], fDir[3], uDir[3], enemypos[3], actorPos[3];
	camera.GetPosition(pos);
	character.GetPosition(actorPos);
	camera.GetDirection(fDir, uDir);
	enemy.GetPosition(enemypos);



	// show frame rate
	static char string[128];
	if (frame == 0) {
		FyTimerReset(0);
	}

	if (frame / 10 * 10 == frame) {
		float curTime;

		curTime = FyTimerCheckTime(0);
		//sprintf(string, "Fps: %6.2f", frame / curTime);
	}

	frame += skip;
	if (frame >= 1000) {
		frame = 0;
	}
	//Do the attack action control
	if (donzoAction != 0) {
		donzoAction -= 1;
		if (donzoAction <= 0) {
			if (enemyPosID == dieID) {
				enemy.Show(FALSE, FALSE, FALSE, FALSE);
				donzoDeathSound.Play( ONCE );
			}
			else {
				enemy.SetCurrentAction(NULL, 0, enemyIdle);
				enemyPosID = enemyIdle;
			}
		}
	}
	if (lyubuAction != 0) {
		lyubuAction -= 1;
		if (lyubuAction <= 0) {
			character.SetCurrentAction(NULL, 0, idleID);
			curPoseID = idleID;
			// footStepSound.Stop();
		}
	}
	//Do the hit test control
	/*if (lyubuAction > 0 && lyubuAction%hitTest[curPoseID] == 0 && distBetweenTwoPoints(enemypos, actorPos) <= 115.0f) {
	//hit
	enemyPosID = actionDamage[curPoseID] > 1 ? damageH : damageL;
	enemy.SetCurrentAction(NULL, 0, enemyPosID);
	donzoAction = actionLength[enemyPosID];
	enemyhp = enemyhp - actionDamage[curPoseID];
	if (enemyhp <= 0) {
	die();
	}
	}*/
	// if hp is lower then zero kill the stuff

	// swap buffer
	FySwapBuffers();
}


/*------------------
movement control
-------------------*/
void Movement(BYTE code, BOOL4 value)
{
	// Homework #01 part 2
	FnCamera camera;
	FnCharacter actor;
	actor.ID(actorID);
	camera.ID(cID);
	if (!value) {
		if (curPoseID == runID) {
			if (!FyCheckHotKeyStatus(VK_LEFT) && !FyCheckHotKeyStatus(VK_RIGHT)) {
				curPoseID = idleID;
				actor.SetCurrentAction(NULL, 0, curPoseID);
				actor.Play(START, 0.0f, FALSE, TRUE);
			}
		}

	}
	else {
		// If any key is pressed
		if (code == VK_LEFT || code == VK_RIGHT) {
			if (curPoseID == idleID) {
				curPoseID = runID;
				actor.SetCurrentAction(0, NULL, curPoseID);
				actor.Play(START, 0.0f, FALSE, TRUE);
				footStepSound.Play( ONCE );
			}
		}
	}
}
/*-----------------------
Attack Control
-----------------------*/
void Attack(BYTE code, BOOL4 value) {
	FnCharacter actor;
	actor.ID(actorID);
	if (value) {
		if (code == FY_A && (actor.GetCurrentAction(NULL) == runID || actor.GetCurrentAction(NULL) == idleID)) {
			curPoseID = normalAttackID[2];
			actor.SetCurrentAction(0, NULL, curPoseID);
			actor.Play(START, 0.0f, FALSE, TRUE);
			lyubuAction = actionLength[curPoseID];
			
			weapon1Sound.Play(ONCE);
		}
		else if (code == FY_S && (actor.GetCurrentAction(NULL) == runID || actor.GetCurrentAction(NULL) == idleID)) {
			curPoseID = heavyAttackID[0];
			actor.SetCurrentAction(0, NULL, curPoseID);
			actor.Play(START, 0.0f, FALSE, TRUE);
			lyubuAction = actionLength[curPoseID];
			
			weapon2Sound.Play(ONCE);
		}
	}
}

/*------------------
quit the demo
C.Wang 0327, 2005
-------------------*/
void QuitGame(BYTE code, BOOL4 value)
{
	if (code == FY_ESCAPE) {
		if (value) {
			endGameSound.Play( ONCE );
			FyQuitFlyWin32();
		}
	}
}


/*----------------------------------
initialize the zoom of the camera
C.Wang 0329, 2005
-----------------------------------*/

void InitElevation(int x, int y)
{
	oldXMM = x;
	oldYMM = y;
	frame = 0;
}


/*------------------
zoom the camera
C.Wang 0329, 2005
-------------------*/
void ElevationCam(int x, int y)
{
	if (y != oldYMM) {
		elevation = elevation + y - oldYMM;
		oldXMM = x;
		oldYMM = y;
	}
	if (elevation >= 90.0f)
		elevation = 89.0f;
	if (elevation < 0.0f)
		elevation = 0.0f;
}

void DistanceCam(int oldS, int newS, float dir) {
	//show debug message
	oldScroll = oldS;
	newScroll = newS;
	dirScroll = dir;
	//change cameraDis 
	if (dir>0) //scroll up
		cameraDis -= (oldS - newS) / 10;
	else if (dir<0) //scroll down
		cameraDis += (oldS - newS) / 10;
	//for limit range
	if (cameraDis<100)
		cameraDis = 100;
	//if (cameraDis>2000)
	//	cameraDis = 2000;

}
void followCam()
{
	//Camera configure ,elevation describe in degree

	FnCamera camera;
	FnObject dummy;
	camera.ID(cID);
	dummy.ID(dID);

	float camerapos[3];
	float pos[3], fDir[3], uDir[3];
	float horDistance = cameraDis*cos(elevation*PI / 180.0);
	float verDistance = cameraDis*sin(elevation*PI / 180.0);
	dummy.GetPosition(pos);
	dummy.GetDirection(fDir, uDir);
	//set camera position
	camerapos[0] = pos[0] - fDir[0] * horDistance;
	camerapos[1] = pos[1] - fDir[1] * horDistance;
	camerapos[2] = verDistance;
	//set camera direction
	fDir[0] = pos[0] - camerapos[0];
	fDir[1] = pos[1] - camerapos[1];
	fDir[2] = -camerapos[2];
	unitVector(fDir, 3);
	//set camera up
	if (fDir[2] == 0) {
		uDir[0] = 0;
		uDir[1] = 0;
		uDir[2] = 1;
	}
	else {
		uDir[0] = fDir[0];
		uDir[1] = fDir[1];
		uDir[2] = -(fDir[0] * fDir[0] + fDir[1] * fDir[1]) / fDir[2];
	}
	unitVector(uDir, 3);
	//fix the camerapos high
	camerapos[2] += 60.0f;
	camera.SetPosition(camerapos);
	camera.SetDirection(fDir, uDir);
}

void adjustCam()
{
	if (collisionDis > 0) {
		oldcollisionDis = collisionDis;
	}
	//collision detect
	FnObject terrain, dummy, camera;
	float cameraPos[3], dummyPos[3], direction[3];
	camera.ID(cID);
	dummy.ID(dID);
	terrain.ID(tID);
	camera.GetPosition(cameraPos);
	dummy.GetPosition(dummyPos);
	// get the direction from dummy to the camera
	for (int i = 0; i < 3; i++) {
		direction[i] = cameraPos[i] - dummyPos[i];
	}
	if (terrain.HitTest(dummyPos, direction, collision)>0) {
		/*collision[0] = 0.0;
		collision[1] = 0.0;
		collision[2] = 0.0;
		collisionDis = 0.0;
		oldcollisionDis = -1.0;*/
		collisionDis = distBetweenTwoPoints(dummyPos, collision);
		if (collisionDis < cameraDis) {
			// something block the camera
			//move the camera up
			if (collisionDis != oldcollisionDis && elevation <= 89.0 && elevation >= 0.0f && test_forward != 1.0f) {
				elevation = elevation + 1.0f;
			}
		}
	}
	else {
		/*collisionDis = distBetweenTwoPoints(dummyPos, collision);
		if(collisionDis < cameraDis) {
		// something block the camera
		//move the camera up
		if(collisionDis != oldcollisionDis && elevation <=89.0 && elevation >=0.0f) {
		elevation = elevation + 1.0f;
		}
		}*/
		if (elevation >= 15.0f && test_forward != -1.0f) {
			elevation = elevation - 1.0f;
		}
	}
}

void enemyFollow(float *pos) {
	FnCharacter enemy;
	enemy.ID(enemyID);
	float mypos[3], fDir[3], uDir[3];
	enemy.GetPosition(mypos);
	enemy.GetDirection(fDir, uDir);
	for (int i = 0; i < 3; i++) {
		fDir[i] = pos[i] - mypos[i];
	}
	unitVector(fDir, 3);
	enemy.SetDirection(fDir, uDir);
}
float distBetweenTwoPoints(float *a, float *b) {
	return sqrt((a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1]) + (a[2] - b[2])*(a[2] - b[2]));
}

void die() {
	FnCharacter enemy;
	enemy.ID(enemyID);
	enemy.SetCurrentAction(NULL, 0, dieID, FALSE, TRUE);
	enemyPosID = dieID;
	donzoAction = actionLength[enemyPosID];
	donzoDeathSound.Play( ONCE );
}
