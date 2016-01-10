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

// ------------------------------------------------
// 20160110 - Item 
CHARACTERid Item_ID;
ACTIONid Item_Idle, Item_PoseID;

bool Item_ShowFlag = FALSE;
// ------------------------------------------------

VIEWPORTid vID;                 // the major viewport
SCENEid sID;                    // the 3D scene
OBJECTid cID, tID;              // the main camera and the terrain for terrain following
OBJECTid dID;					//#dummy object for control character dir##
OBJECTid winText[2];
CHARACTERid actorID, enemyID, scoutID;            // the major character
ACTIONid idleID, runID, curPoseID, ultimateAttack, enemyIdle, damageL, damageH, dieID, enemyPosID; // two actions
ACTIONid normalAttackID[4], heavyAttackID[3], enemyAttackID, enemyRunID;
ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;
OBJECTid nID;
BOOL4 killed = FALSE;
GAMEFX_SYSTEMid gFXID[20];// 0 is lyubu 1 is donzo 3 is robber bigger than 4 is ball
std::map<ACTIONid, int>actionLength, hitTest, actionDamage, hitTime, attackRange; // hitTest is the Interval for being hitted
vector<float> speed;
vector <CHARACTERid> ballPool;
vector <CHARACTERid> ballId;
float scoreBoard[2] = { -3070.0f, -4300.0f };
float edgeBoard[2] = { 3180.0f, 3900.0f };
int scoreActor = 0, scoreEnemy = 0;
int firstPlayer = 0;
int GameState = 1;
// Media
MEDIAid mmID;
AUDIOid backgroundSoundID, endGameSoundID;
AUDIOid footStepSoundID, weapon1SoundID, weapon2SoundID;
AUDIOid lyubuDeathSoundID, donzoDeathSoundID;
AUDIOid hitWallSoundId, summonSoundId;
FnAudio backgroundSound, endGameSound;
FnAudio footStepSound, weapon1Sound, weapon2Sound;
FnAudio lyubuDeathSound, donzoDeathSound;
FnAudio hitWallSound, summonSound;

// the centre of the rotation
// float centre[3];
float dist = 1000.0f, height = 75.0f, rotatedistancefactor = 0.017453f;
// some globals
int frame = 0, direction = 1, enemyDirection = 1; // the global variable for direction
int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;
float cameraDis = 1000.0f, elevation = 0.0f, collision[3] = { 0.0f }, collisionDis, oldcollisionDis;
int test_forward, test_right;
bool toward_wall, away_wall;
int lyubuAction = 0, attackCombo = 0, donzoAction = 0;
int enemyhp = 100;
OBJECTid scoreDigit[4];
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
void splitBall(unsigned int pos);
int oldScroll, newScroll; //for test Mouse Scroll argument
float dirScroll; //for test Mouse Scroll argument
				 //camera follow function
void followCam();
void adjustCam();
void enemyFollow(float*);
float distBetweenTwoPoints(float*, float*);
void moveBall(int skip);
void score(char name[], unsigned int score);
void collisionItem(unsigned int pos, CHARACTERid itemID);
void collisionCharacter(unsigned int pos, CHARACTERid actorID);
void collisionEdge(unsigned int pos, int x, int y);
void deleteBall(unsigned int pos);
void addBall(float fDir[3], float pos[3]);
void ShowScore();
void intitialBoard();
void Restart(BYTE code, BOOL4 value);

void createAudio(void) {
	backgroundSoundID = FyCreateAudio();
	backgroundSound.ID(backgroundSoundID);
	backgroundSound.Load("background");
	//backgroundSound.Play(LOOP);
	backgroundSound.SetVolume(0.1);

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

	hitWallSoundId = FyCreateAudio();
	hitWallSound.ID(hitWallSoundId);
	hitWallSound.Load("hitwall");

	summonSoundId = FyCreateAudio();
	summonSound.ID(summonSoundId);
	summonSound.Load("summon");
}

// ------------------------------------------------
// 20160110 - Item 
void DisplayItem(FnScene scene, float* uDir, float* fDir, FnCharacter Item, BOOL4 beOK) {
	Item_ID = scene.LoadCharacter("Robber02");
	Item.ID(Item_ID);

	Item.SetDirection(fDir, uDir);
	Item.SetTerrainRoom(terrainRoomID, 10.0f);

	float Item_Position[3];
	Item_Position[0] = (float)(rand() % 520) + 3281;
	Item_Position[1] = (float)(rand() % 1030) - 4200;
	Item_Position[2] = 1000.f;

	//char msgbuf[1000];

	beOK = Item.PutOnTerrain(Item_Position);
	//sprintf(msgbuf, "beOK : %d\n", beOK);
	//OutputDebugString(_T(msgbuf));

	Item_Idle = Item.GetBodyAction(NULL, "CombatIdle");

	Item_PoseID = Item_Idle;
	Item.SetCurrentAction(NULL, 0, Item_PoseID);
	Item.Play(START, 0.0f, FALSE, TRUE);
}
// ------------------------------------------------

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
	FySetGameFXPath("Data\\FX0");
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
	actor.SetDirection(fDir, uDir);
	infile >> enemypos[0] >> enemypos[1] >> enemypos[2];
	infile >> fDir[0] >> fDir[1] >> fDir[2];
	enemy.SetDirection(fDir, uDir);
	/*pos[0] = 3487.873; pos[1] = -3639.062f; pos[2] = 1000.0f;
	enemypos[0] = 3540.0f; enemypos[1] = -4300.0f; enemypos[2] = 1000.0f;
	scoutpos[0] = 3540.0f; scoutpos[1] = -3070.0f; scoutpos[2] = 1000.0f;*/
	//fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;

	actor.SetTerrainRoom(terrainRoomID, 10.0f);
	enemy.SetTerrainRoom(terrainRoomID, 10.0f);
	char msgbuf[1000];
	beOK = enemy.PutOnTerrain(enemypos);
	sprintf(msgbuf, "beok : %d\n", beOK);
	//OutputDebugString(_T(msgbuf));
	beOK = actor.PutOnTerrain(pos);
	sprintf(msgbuf, "beok : %d\n", beOK);
	//OutputDebugString(_T(msgbuf));

	// ------------------------------------------------
	// 20160110 - Item
	FnCharacter Item;
	DisplayItem(scene, uDir, fDir, Item, beOK);
	// ------------------------------------------------

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
	enemyAttackID = enemy.GetBodyAction(NULL, "AttackL1");
	enemyRunID = enemy.GetBodyAction(NULL, "Run");
	sprintf(msgbuf, "enemyRun Id  %d\n", enemyRunID);
	//OutputDebugString(_T(msgbuf));
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
	actionLength[enemyAttackID] = 85 - 50;
	actionLength[enemyRunID] = 240 - 210;
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

	//initialize GameFx stuff
	for (int i = 0; i < 10; i++) {
		gFXID[i] = FAILED_ID;
	}

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
	FyDefineHotKey(FY_Y, Restart, FALSE);
	FyDefineHotKey(FY_N, Restart, FALSE);
	//FyDefineHotKey(FY_D, Attack, FALSE);

	// define some mouse functions
	FyBindMouseFunction(RIGHT_MOUSE, InitElevation, ElevationCam, NULL, NULL);
	FyBindMouseWheelFunction(DistanceCam);
	intitialBoard();
	// bind timers, frame rate = 30 fps
	FyBindTimer(0, 30.0f, GameAI, TRUE);
	FyBindTimer(1, 30.0f, RenderIt, TRUE);
	FyBindTimer(2, 30.0f, moveBall, TRUE);
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

	// ------------------------------------------------
	// 20160110 - Item 
	FnCharacter Item;
	Item.ID(Item_ID);
	Item.Play(LOOP, (float)skip, FALSE, TRUE);
	// ------------------------------------------------

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
	// play game FX
	for (int i = 0; i < 10; i++) {
		if (gFXID[i] != FAILED_ID) {
			FnGameFXSystem gxS(gFXID[i]);
			BOOL4 beOK = gxS.Play((float)skip, ONCE);
			if (!beOK) {
				FnScene scene(sID);
				scene.DeleteGameFXSystem(gFXID[i]);
				gFXID[i] = FAILED_ID;
			}
		}
	}
	if (enemyDirection != 1 && enemyDirection != 7) {
		//OutputDebugString(_T("I found the stuff !! 0w0 \n"));
		if (enemyDirection == 3) {
			enemy.TurnRight(-90.0f);
			enemy.SetCurrentAction(NULL, 0, enemyRunID);
			enemyPosID = enemyRunID;
			donzoAction = actionLength[enemyRunID];
			enemyDirection = 6;
		}
		else if (enemyDirection == 2) {
			enemy.TurnRight(90.0f);
			enemy.SetCurrentAction(NULL, 0, enemyRunID);
			enemyPosID = enemyRunID;
			donzoAction = actionLength[enemyRunID];
			enemyDirection = 4;
		}
		enemy.MoveForward(dist, FALSE, FALSE, 0.0f, FALSE);
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
	vp.RenderSprites(sID, FALSE, TRUE);
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

	// ------------------------------------------------
	// 20160110 - Item 
	FnCharacter Item;
	Item.ID(Item_ID);

	float Pos_Dissapear[3], Pos_Appear[3];
	Pos_Dissapear[0] = 0.0f; Pos_Dissapear[1] = 0.0f; Pos_Dissapear[2] = 0.0f;
	Item.GetPosition(Pos_Appear);
	Pos_Appear[2] = 1000.0f;
	//Pos_Appear[0] = 3540.0f; Pos_Appear[1] = -3685.0f; Pos_Appear[2] = 1000.0f;
	Pos_Appear[0] = (float)(rand() % 520) + 3281;
	Pos_Appear[1] = (float)(rand() % 1030) - 4200;

	if (Item_ShowFlag == FALSE) {
		// Item.SetPosition( Pos_Dissapear );
		Item.Show(FALSE, FALSE, FALSE, FALSE);
		Item.SetPosition(Pos_Dissapear);

		if (frame == 100 || frame == 400) { Item_ShowFlag = TRUE; }
	}
	else if (Item_ShowFlag == TRUE) {
		// Item.SetPosition( Pos_Appear );
		Item.Show(TRUE, TRUE, TRUE, TRUE);
		float tmppos[3];
		Item.GetPosition(tmppos);
		if (tmppos[0] == 0.0f)
		{
			Item.SetTerrainRoom(terrainRoomID, 10.0f);
			Item.PutOnTerrain(Pos_Appear);
			char msgbuf[1000];
			float tmp[3];
			Item.GetPosition(tmp);
			sprintf(msgbuf, "%f %f %f\n", tmp[0], tmp[1], tmp[2]);
			//OutputDebugString(_T(msgbuf));

			summonSound.Play(ONCE);

			FnScene scene;
			scene.ID(sID);
			if (gFXID[3] != NULL) {
				scene.DeleteGameFXSystem(gFXID[3]);
			}
			gFXID[3] = scene.CreateGameFXSystem();
			FnGameFXSystem gxS(gFXID[3]);
			BOOL4 beOK = gxS.Load("SpellHome_01", TRUE);
			if (beOK) {
				gxS.SetPlayLocation(tmp);
			}
		}
		if (frame == 200 || frame == 600) { Item_ShowFlag = FALSE; }
	}

	// ------------------------------------------------

	//Do the attack action control
	if (donzoAction != 0) {
		donzoAction -= 1;
		if (donzoAction <= 0) {
			if (enemyPosID == dieID) {
				enemy.Show(FALSE, FALSE, FALSE, FALSE);
				donzoDeathSound.Play(ONCE);
			}
			else if (enemyPosID == enemyRunID) {
				char msgbuf[1000];
				sprintf(msgbuf, "I'm going to reset the action\n");
				//OutputDebugString(_T(msgbuf));
				if (enemyDirection == 6) {
					enemy.TurnRight(90.0f);
				}
				else if (enemyDirection == 4) {
					enemy.TurnRight(-90.0f);
				}
				enemyDirection = 1;
				enemyPosID = enemyIdle;
				enemy.SetCurrentAction(NULL, 0, enemyPosID);
			}
			else if (enemyPosID == enemyAttackID) {
				enemyDirection = 1;
				enemy.SetCurrentAction(NULL, 0, enemyIdle);
				enemyPosID = enemyIdle;
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
	ShowScore();
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
				footStepSound.Play(ONCE);
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
			FnScene scene;
			scene.ID(sID);
			if (gFXID[0] != NULL) {
				scene.DeleteGameFXSystem(gFXID[0]);
			}
			gFXID[0] = scene.CreateGameFXSystem();
			FnGameFXSystem gxS(gFXID[0]);
			BOOL4 beOK = gxS.Load("Lyubu_atk01", TRUE);
			if (beOK) {
				float pos[3];
				actor.GetPosition(pos);
				gxS.SetPlayLocation(pos);
			}
			if (firstPlayer == 0 && ballId.size() == 0) {
				float pos[3], fDir[3], uDir[3];
				actor.GetPosition(pos);
				actor.GetDirection(fDir, uDir);
				pos[1] -= 10.0f;
				addBall(fDir, pos);
				FnObject ball;
				ball.ID(ballId.at(ballId.size() - 1));
				srand(time(NULL));
				float degree = (float)(rand() % 60) + 1.0f;
				ball.TurnRight(degree);
			}
		}
		else if (code == FY_S && (actor.GetCurrentAction(NULL) == runID || actor.GetCurrentAction(NULL) == idleID)) {
			curPoseID = normalAttackID[0];
			actor.SetCurrentAction(0, NULL, curPoseID);
			actor.Play(START, 0.0f, FALSE, TRUE);
			lyubuAction = actionLength[curPoseID];

			weapon2Sound.Play(ONCE);
			FnScene scene;
			scene.ID(sID);
			if (gFXID[0] != NULL) {
				scene.DeleteGameFXSystem(gFXID[0]);
			}
			gFXID[0] = scene.CreateGameFXSystem();
			FnGameFXSystem gxS(gFXID[0]);
			BOOL4 beOK = gxS.Load("Lyubu_skill01", TRUE);
			if (beOK) {
				float pos[3];
				actor.GetPosition(pos);
				gxS.SetPlayLocation(pos);
			}
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
			endGameSound.Play(ONCE);
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
	float posEnemy[3], posBall[3];
	enemy.GetPosition(posEnemy);

	FnObject ball;
	int minPos = 0, selectId = -1;
	if (ballId.size() == 0 || enemyDirection != 1) return;
	for (unsigned int i = 0; i<ballId.size(); i++) {
		ball.ID(ballId[i]);
		ball.GetPosition(posBall);
		// ball.GetDirection(fDir, uDir);
		// find the nearest ball
		if (minPos > posBall[1]) {
			minPos = posBall[1];
			selectId = i;
		}
	}
	ball.ID(ballId[selectId]);
	ball.GetPosition(posBall);
	float dist = distBetweenTwoPoints(posEnemy, posBall);
	if (dist <= 240) { // attack
		enemyPosID = enemyAttackID;
		donzoAction = actionLength[enemyAttackID];
		enemy.SetCurrentAction(NULL, 0, enemyPosID);
		enemy.Play(START, 0.0f, FALSE, TRUE);
		enemyDirection = 7;
	}
	else { // move
		int forward = 0, right = 0;
		float fDir[3], uDir[3];
		char msgbuf[1000];
		dist = FYABS(posEnemy[0] - posBall[0]);
		enemy.GetDirection(fDir, uDir);
		if (dist > 120)
		{
			if (posEnemy[0] < posBall[0]) { // right
				enemyDirection = 2; // East
									/*enemy.TurnRight(90.0f);
									enemyPosID = enemyRunID;
									enemy.SetCurrentAction(NULL, 0, enemyPosID);
									donzoAction = actionLength[enemyRunID];*/
									//OutputDebugString(_T("I'm going to move Right\n"));
			}
			else if (posEnemy[0] > posBall[0]) { // left
				enemyDirection = 3; // West
									/*enemy.TurnRight(-90.0f);
									enemyPosID = enemyRunID;
									enemy.SetCurrentAction(NULL, 0, enemyPosID);
									donzoAction = actionLength[enemyRunID];*/
									//OutputDebugString(_T("I'm going to move Left\n"));
			}
			else { // stop
				enemyDirection = 1; // North
									//OutputDebugString(_T("I'm not going to move\n"));
			}
		}
	}
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
	donzoDeathSound.Play(ONCE);
}

void addBall(float fDir[3], float pos[3]) {
	FnScene scene(sID);
	char msgbuf[1000];
	FySetCharacterPath("Data\\Characters");
	OBJECTid id = scene.CreateObject(OBJECT);
	FnObject ball;
	ball.ID(id);
	ball.Load("teapot.cw3");
	//sprintf(msgbuf, "%d going to load ball %d\n",sID, id);
	//OutputDebugString(_T(msgbuf));
	float uDir[3] = { 0.0f, 0.0f, 1.0f };
	ball.SetTerrainRoom(terrainRoomID, 10.0f);
	pos[2] = 1000.0f;
	//sprintf(msgbuf, "going to put ball at at %f %f %f\n", pos[0], pos[1], pos[2]);
	//OutputDebugString(_T(msgbuf));
	ball.PutOnTerrain(pos);
	ball.SetDirection(fDir, uDir);
	float tmp[3];
	ball.GetPosition(tmp);
	//sprintf(msgbuf,"ball at %f %f %f\n", tmp[0], tmp[1], tmp[2]);
	//OutputDebugString(_T(msgbuf));
	ballId.push_back(id);
	speed.push_back(10.0f);
}

void deleteBall(unsigned int pos) {
	FnScene scene;
	scene.ID(sID);
	//char msgbuf[1000];
	//sprintf(msgbuf, "Going to delete ball at pos %d\n", pos);
	//OutputDebugString(_T(msgbuf));
	scene.DeleteObject(ballId[pos]);
	ballId.erase(ballId.begin() + pos);
	speed.erase(speed.begin() + pos);
}

void collisionEdge(unsigned int pos, int x, int y) {
	FnObject ball(ballId[pos]);
	float xDir[3] = { 1.0f, 0.0f, 0.0f }, fDir[3], uDir[3];
	ball.GetDirection(fDir, uDir);
	float dot = 2 * FyDot(xDir, fDir);
	for (int i = 0; i<3; i++)
		fDir[i] -= dot * xDir[i];
	ball.SetDirection(fDir, uDir);

	hitWallSound.Play(ONCE);
	FnScene scene;
	scene.ID(sID);
	if (gFXID[4 + pos] != NULL) {
		scene.DeleteGameFXSystem(gFXID[4 + pos]);
	}
	gFXID[4 + pos] = scene.CreateGameFXSystem();
	FnGameFXSystem gxS(gFXID[4 + pos]);
	BOOL4 beOK = gxS.Load("Blow_01e", TRUE);
	if (beOK) {
		float pos[3];
		ball.GetPosition(pos);
		gxS.SetPlayLocation(pos);
	}


}

void collisionCharacter(unsigned int pos, CHARACTERid characterID) {
	FnCharacter character;
	character.ID(characterID);
	int hit = 0;
	if (characterID == enemyID) {
		if (character.GetCurrentAction(NULL) == enemyAttackID) {
			hit = 1;
		}
	}
	else if (characterID == actorID) {
		FnObject ball(ballId[pos]);
		float pos[3];
		ball.GetPosition(pos);
		if (character.GetCurrentAction(NULL) == normalAttackID[2]) {
			hit = 2;
		}
		else if (character.GetCurrentAction(NULL) == normalAttackID[0] && pos[1] >= scoreBoard[0] - 80) {
			hit = 1;
		}
	}
	if (hit > 0) {
		float fDir[3], uDir[3];
		character.GetDirection(fDir, uDir);
		FnObject ball(ballId[pos]);
		ball.SetDirection(fDir, uDir);
		float degree = (float)(rand() % 60) + 1.0f;
		ball.TurnRight(degree);
		hitWallSound.Play(ONCE);
		FnScene scene;
		scene.ID(sID);
		if (gFXID[4 + pos] != NULL) {
			scene.DeleteGameFXSystem(gFXID[4 + pos]);
		}
		gFXID[4 + pos] = scene.CreateGameFXSystem();
		FnGameFXSystem gxS(gFXID[4 + pos]);
		BOOL4 beOK = gxS.Load("Tower_atk01_e", TRUE);
		if (beOK) {
			float pos[3];
			ball.GetPosition(pos);
			gxS.SetPlayLocation(pos);
		}
	}
}
void collisionItem(unsigned int pos) {
	if (Item_ShowFlag == true) {
		FnObject ball(ballId[pos]);
		FnCharacter robber(Item_ID);
		float xDir[3] = { 1.0f, 0.0f, 0.0f };
		float fDir[3], uDir[3] = { 0.0f, 0.0f, 1.0f };
		float posI[3], posB[3];
		robber.GetPosition(posI);
		ball.GetPosition(posB);
		//避免z軸影響
		posI[2] = 0.0f;
		posB[2] = 0.0f;

		float dis = FyDistance(posI, posB);
		//取得道具??
		if (dis < 80.0f)
		{
			hitWallSound.Play(ONCE);
			FnScene scene;
			scene.ID(sID);
			if (gFXID[4 + pos] != NULL) {
				scene.DeleteGameFXSystem(gFXID[4 + pos]);
			}
			gFXID[4 + pos] = scene.CreateGameFXSystem();
			FnGameFXSystem gxS(gFXID[4 + pos]);
			BOOL4 beOK = gxS.Load("Tower_atk01_e", TRUE);
			if (beOK) {
				float pos[3];
				ball.GetPosition(pos);
				gxS.SetPlayLocation(pos);
			}
			Item_ShowFlag = false;
			splitBall(pos);
		}
	}
}

void score(char name[], unsigned int position) {
	if (!strcmp(name, "Lyubu"))
		scoreEnemy += 1;
	else
		scoreActor += 1;
	deleteBall(position);
}
void moveBall(int skip) {
	for (unsigned int i = 0; i<ballId.size(); i++) {
		FnObject ball;
		ball.ID(ballId[i]);
		ball.MoveForward(speed[i], FALSE, FALSE, 0.0f, FALSE);
		float pos[3], fDir[3], uDir[3];
		ball.GetPosition(pos);
		ball.GetDirection(fDir, uDir);
		//條件判斷 邊界
		if (pos[0] <= edgeBoard[0] && fDir[0]<0)
			collisionEdge(i, edgeBoard[0], 0);
		else if (pos[0] >= edgeBoard[1] && fDir[0]>0)
			collisionEdge(i, edgeBoard[0], 0);
		//條件判斷 腳色
		if (pos[1] >= scoreBoard[0] - 120)
			collisionCharacter(i, actorID);
		else if (pos[1] <= scoreBoard[1] + 120)
			collisionCharacter(i, enemyID);
		//條件判斷 得分區
		if (pos[1] >= scoreBoard[0]) {
			//collisionEdge(i, scoreBoard[0], 0);
			score("Lyubu", i);
		}
		else if (pos[1] <= scoreBoard[1]) {
			//collisionEdge(i, scoreBoard[1], 0);
			score("Donzo", i);
		}
		collisionItem(i);
	}
}

void intitialBoard() {
	FnScene scene(sID);
	for (int i = 0; i < 4; i++)
		scoreDigit[i] = scene.CreateObject(SPRITE);
	for (int i = 0; i < 2; i++)
		winText[i] = scene.CreateObject(SPRITE);

}

void ShowScore() {
	if (GameState == 1) {
		FySetTexturePath("Data\\textures");
		FnScene scene(sID);
		int displayNA[2] = { scoreActor / 10, scoreActor % 10 };
		int displayNE[2] = { scoreEnemy / 10, scoreEnemy % 10 };
		char picture[2][10][10] = { { "0.png", "1.png", "2.png", "3.png", "4.png",
			"5.png", "6.png","7.png","8.png","9.png" },{ "g0.png", "g1.png", "g2.png",
			"g3.png", "g4.png", "g5.png", "g6.png","g7.png","g8.png","g9.png" } };
		FnSprite sp1(scoreDigit[0]), sp2(scoreDigit[1]);
		FnSprite sp3(scoreDigit[2]), sp4(scoreDigit[3]);
		sp1.SetSize(50, 100);
		sp2.SetSize(50, 100);
		sp3.SetSize(50, 100);
		sp4.SetSize(50, 100);
		sp1.SetImage(picture[0][displayNA[0]], 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
		sp2.SetImage(picture[0][displayNA[1]], 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
		sp3.SetImage(picture[1][displayNE[0]], 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
		sp4.SetImage(picture[1][displayNE[1]], 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
		sp1.SetPosition(50, 680, 0);
		sp2.SetPosition(100, 680, 0);
		sp3.SetPosition(850, 680, 0);
		sp4.SetPosition(900, 680, 0);
		sp1.Show(TRUE);
		sp2.Show(TRUE);
		sp3.Show(TRUE);
		sp4.Show(TRUE);
		for (int i = 0; i < 2; i++) {
			FnSprite text(winText[i]);
			text.Show(FALSE);
		}
		if (ballId.size() == 0 && (scoreActor >= 3 || scoreEnemy >= 3))
			GameState = 0;
	}
	else {
		char picture[3][20] = { "LyuBuWin.png", "DonZoWin.png", "guide.png" };
		FnSprite win1(winText[0]), win2(winText[1]);
		if (scoreActor>scoreEnemy)
			win1.SetImage(picture[0], 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
		else if (scoreActor<scoreEnemy)
			win1.SetImage(picture[1], 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
		win2.SetImage(picture[2], 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
		win1.SetPosition(260, 350, 0);
		win2.SetPosition(375, 250, 0);
		win1.SetSize(500, 200);
		win2.SetSize(283, 100);
		win1.Show(TRUE);
		win2.Show(TRUE);
	}

}

void Restart(BYTE code, BOOL4 value) {
	if (value && (GameState == 0)) {
		if (code == FY_Y) {
			GameState = 1;
			scoreActor = 0;
			scoreEnemy = 0;
			for (int i = 0; i < ballId.size(); i++) {
				deleteBall(i);
			}
		}
		else if (code == FY_N) {
			endGameSound.Play(ONCE);
			FyQuitFlyWin32();
		}
	}
}

void splitBall(unsigned int pos) {
	if (ballId.size() < 10)
	{
		OutputDebugString(_T("Going to add balls 0w0\n"));
		FnScene scene(sID);
		FnObject ball(ballId[pos]);
		OBJECTid newId1, newId2;

		newId1 = scene.CreateObject(OBJECT);
		newId2 = scene.CreateObject(OBJECT);
		FnObject newBall1(newId1), newBall2(newId2);
		FySetCharacterPath("Data\\Characters");
		newBall1.Load("teapot.cw3");
		newBall2.Load("teapot.cw3");
		float fDir[3], uDir[3], bpos[3];
		ball.GetDirection(fDir, uDir);
		ball.GetPosition(bpos);
		newBall1.SetTerrainRoom(terrainRoomID, 10.0f);
		newBall2.SetTerrainRoom(terrainRoomID, 10.0f);

		newBall1.SetPosition(bpos);
		newBall2.SetPosition(bpos);

		newBall1.SetDirection(fDir, uDir);
		newBall1.TurnRight(45.0f);
		newBall2.SetDirection(fDir, uDir);
		newBall2.TurnRight(-45.0f);

		ballId.push_back(newId1);
		ballId.push_back(newId2);

		speed.push_back(10.0f);
		speed.push_back(10.0f);
	}
}