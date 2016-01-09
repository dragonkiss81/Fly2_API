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

#include "FlyWin32.h"
#include "FyMedia.h"
#include "game_design.h"
#include <vector>

using namespace std;

// ACTOR
vector<ACTNUM> badguyID(NUM_OF_BADGUYS);
vector<ACTNUM> BossID(NUM_OF_BOSS);
ACTNUM LyubuID;
ProduceBadguys generator[NUM_OF_GENERATOR];
float genSpot[NUM_OF_GENERATOR][2];

// VIEW
VIEWPORTid vID;                 // the major viewport
SCENEid sID;                    // the 3D scene
OBJECTid cID, tID;              // the main camera and the terrain for terrain following

// ACTION
CHARACTERid actorID;            // the major character
ACTIONid idleID, runID, curPoseID;
ACTIONid NormalAttack1ID, NormalAttack2ID, HeavyAttack1ID;
ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;

//FRAME
int frame = 0;
int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

// 2D SCENE
SCENEid sID2;                
FnSprite sp;
FnSprite sp_hpFrame, sp_headLyu, sp_wordLyu, sp_hpBlood, sp_hpMana, sp_deadEnd, sp_trueEnd;
OBJECTid spID0 = FAILED_ID;
OBJECTid spID_hpFrame = FAILED_ID, spID_headLyu = FAILED_ID, spID_wordLyu = FAILED_ID, spID_hpBlood = FAILED_ID, spID_hpMana = FAILED_ID, spID_deadEnd = FAILED_ID, spID_trueEnd = FAILED_ID;
bool missionWindowStatus = FALSE;
int picture_count = 1;

// FX
GAMEFX_SYSTEMid gFXID = FAILED_ID;
OBJECTid dummyID = FAILED_ID;

// MUSIC
MEDIAid BackMusic_ID;
MEDIAid hit_ID, BADHIT_ID;
MEDIAid damage_ID, DIE_ID;
MEDIAid RunMusuc_ID;


// Turning Frame Counters
int dirCount = 0, dirState = 0;
int idle_count = 0;
int status = 0;

// hotkey callbacks
void QuitGame(BYTE, BOOL4);
void Movement(BYTE, BOOL4);

// timer callbacks
void GameAI(int);
void RenderIt(int);

// mouse callbacks
void InitPivot(int, int);
void PivotCam(int, int);
void InitMove(int, int);
void MoveCam(int, int);
void InitZoom(int, int);
void ZoomCam(int, int);


/*------------------
the main program
C.Wang 1010, 2014
-------------------*/
void FyMain(int argc, char **argv)
{
	// create a new world
	BOOL4 beOK = FyStartFlyWin32("NTU@2015 Homework #02 - Use Fly2", 0, 0, 1024, 768, FALSE);

	// setup the data searching paths
	FySetShaderPath("Data\\NTU6\\Shaders");
	FySetModelPath("Data\\NTU6\\Scenes");
	FySetTexturePath("Data\\NTU6\\Scenes\\Textures");
	FySetScenePath("Data\\NTU6\\Scenes");
	FySetGameFXPath("Data\\NTU6\\FX");
	FyBeginMedia("Data\\NTU6\\Media", 2);
	BackMusic_ID = FyCreateMediaPlayer("background.mp3", 0, 0, 800, 600);   //­I´º­µ¼Ö

	// include background music
	FnMedia mP;
	mP.Object(BackMusic_ID);
	mP.SetVolume(1.0f);
	mP.Play(LOOP);

	// create a viewport
	vID = FyCreateViewport(0, 0, 1024, 768);
	FnViewport vp;
	vp.ID(vID);

	// create a 3D scene
	sID = FyCreateScene(10);
	FnScene scene;
	scene.ID(sID);

	// load the scene
	scene.Load("gameScene02");
	scene.SetAmbientLights(1.0f, 1.0f, 1.0f, 0.6f, 0.6f, 0.6f);

	// load the terrain
	tID = scene.CreateObject(OBJECT);
	FnObject terrain;
	terrain.ID(tID);
	BOOL beOK1 = terrain.Load("terrain");
	terrain.Show(FALSE);

	// set terrain environment
	terrainRoomID = scene.CreateRoom(SIMPLE_ROOM, 10);
	FnRoom room;
	room.ID(terrainRoomID);
	room.AddObject(tID);

	// load the character
	FySetModelPath("Data\\NTU6\\Characters");
	FySetTexturePath("Data\\NTU6\\Characters");
	FySetCharacterPath("Data\\NTU6\\Characters");
	actorID = scene.LoadCharacter("Lyubu2");

	// put the character on terrain
	float pos[3], fDir[3], uDir[3];
	SetValues(pos, 3569.0f, -3208.0f, 1000.0f);
	SetValues(fDir, 1.0f, 1.0f, 0.0f);
	SetValues(uDir, 0.0f, 0.0f, 1.0f);

	ActorGen(scene, terrainRoomID, LyubuID, "Lyubu2", "Idle", pos, fDir, uDir);
	actorID = LyubuID.actorID;

	FnCharacter actor;
	actor.ID(actorID);

	// Get two character actions pre-defined at Lyubu2
	idleID = actor.GetBodyAction(NULL, "Idle");
	runID = actor.GetBodyAction(NULL, "Run");
	NormalAttack1ID = actor.GetBodyAction(NULL, "NormalAttack1");
	NormalAttack2ID = actor.GetBodyAction(NULL, "NormalAttack2");
	HeavyAttack1ID = actor.GetBodyAction(NULL, "HeavyAttack1");

	// translate the camera
	cID = scene.CreateObject(CAMERA);
	FnCamera camera;
	camera.ID(cID);
	camera.SetNearPlane(5.0f);
	camera.SetFarPlane(100000.0f);

	// Donzo and Robber02 position initialization
	float temp_pos[3];
	float temp_fDir[3];

	/*
	for (int i = 0; i < NUM_OF_BADGUYS; i++)
	{
	temp_pos[0] = pos[0] + 30 * (rand()%8);
	temp_pos[1] = pos[1] + 30 * (rand()%8);
	temp_pos[2] = pos[2] + 30 * (rand()%8);
	temp_fDir[0] = -1.0f; temp_fDir[1] = -1.0f; temp_fDir[2] = 1.0f;

	ActorGen(scene, terrainRoomID, badguyID[i], "Robber02", "CombatIdle", temp_pos, temp_fDir, uDir);
	}
	*/

	for (int i = 0; i < NUM_OF_BOSS; i++)
	{
		//temp_pos[0] = 130.636;  //pos[0] + 30 * (rand()%8);
		//temp_pos[1] = 2707.003; //pos[1] + 30 * (rand()%8);
		//temp_pos[2] = 70.897;   //pos[2] + 30 * (rand()%8);

		temp_pos[0] = pos[0] + 30 * (rand()%8);
		temp_pos[1] = pos[1] + 30 * (rand()%8);
		temp_pos[2] = pos[2] + 30 * (rand()%8);

		SetValues(temp_fDir, -1.0f, -1.0f, 1.0f);

		ActorGen(scene, terrainRoomID, BossID[i], "Donzo2", "CombatIdle", temp_pos, temp_fDir, uDir);
		BossID[i].idtype = DONZO;
	}

	// set camera position
	actor.GetPosition(pos);

	pos[0] = pos[0] - SHORTDIST * fDir[0];
	pos[1] = pos[1] - SHORTDIST * fDir[1];
	pos[2] = pos[2] + HEIGHTOFFSET;

	SetValues(fDir, 1.0f, 1.0f, 0.0f);
	SetValues(uDir, 0.0f, 0.0f, 1.0f);

	camera.SetPosition(pos);
	camera.SetDirection(fDir, uDir); // the same as actor

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

	// create a 2D scene for sprite rendering which will be rendered on the top of 3D
	sID2 = FyCreateScene(1);
	scene.ID(sID2);
	scene.SetSpriteWorldSize(800, 600);         // 2D scene size in pixels

	spID0 = scene.CreateObject(SPRITE);
	sp.ID(spID0);
	showPicture(sp, "startTalk_1.png", 780, 180, 10, 10); //showPicture parameter : FnSprite ,imageName, size, position

	spID_hpFrame = scene.CreateObject(SPRITE);
	sp_hpFrame.ID(spID_hpFrame);
	showPicture(sp_hpFrame, "hp_line.png", 400, 100, 400, 480);

	spID_hpBlood = scene.CreateObject(SPRITE);
	sp_hpBlood.ID(spID_hpBlood);

	spID_hpMana = scene.CreateObject(SPRITE);
	sp_hpMana.ID(spID_hpMana);

	spID_headLyu = scene.CreateObject(SPRITE);
	sp_headLyu.ID(spID_headLyu);
	showPicture(sp_headLyu, "headLyu.png", 80, 80, 475, 520);

	spID_wordLyu = scene.CreateObject(SPRITE);
	sp_wordLyu.ID(spID_wordLyu);
	showPicture(sp_wordLyu, "wordLyu.png", 140, 40, 550, 540);

	spID_deadEnd = scene.CreateObject(SPRITE);
	sp_deadEnd.ID(spID_deadEnd);

	spID_trueEnd = scene.CreateObject(SPRITE);
	sp_trueEnd.ID(spID_trueEnd);

	// set Hotkeys
	FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);  // escape for quiting the game
	FyDefineHotKey(FY_UP, Movement, FALSE);      // Up for moving forward
	FyDefineHotKey(FY_RIGHT, Movement, FALSE);   // Right for turning right
	FyDefineHotKey(FY_LEFT, Movement, FALSE);    // Left for turning left
	FyDefineHotKey(FY_DOWN, Movement, FALSE);
	FyDefineHotKey(FY_Q, Movement, FALSE);
	FyDefineHotKey(FY_W, Movement, FALSE);
	FyDefineHotKey(FY_E, Movement, FALSE);
	FyDefineHotKey(FY_A, Movement, FALSE);       // Turn left with camera rotation
	FyDefineHotKey(FY_D, Movement, FALSE);		 // Turn right with camera rotation
	FyDefineHotKey(FY_S, Movement, FALSE);
	FyDefineHotKey(FY_T, Movement, FALSE);   //Talk button
	FyDefineHotKey(FY_M, Movement, FALSE);   //Mission button
	FyDefineHotKey(FY_I, Movement, FALSE);   //Mission button

	// define some mouse functions
	FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
	FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
	FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

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
	FnCharacter actor;
	FnCharacter cur_actor;
	FnObject terrain;
	terrain.ID(tID);
	FnScene scene;
	scene.ID(sID);
	LyubuID.actorID = actorID;

	float actorPos[3];
	vector<char*> FX_FileName;

	actor.ID(actorID);
	actor.GetPosition(actorPos);

	if (curPoseID == runID || curPoseID == idleID)
		actor.Play(LOOP, (float)skip, FALSE, TRUE);
	else if (actor.Play(ONCE, (float)skip, FALSE, TRUE) == 0)
	{
		curPoseID = idleID;
		actor.SetCurrentAction(NULL, 0, curPoseID, 10.0f);
	}

	if (curPoseID == idleID)					// Automatic mana recovery
	{
		if (LyubuID.mana_remain < LyubuID.mana_total)
			LyubuID.mana_remain += 2;
		if (LyubuID.blood_remain < LyubuID.blood_total)
			LyubuID.blood_remain += 1;
	}


	for (int i = 0; i < NUM_OF_BADGUYS; i++)
	{
		cur_actor.ID(badguyID[i].actorID);

		if (badguyID[i].alive)
		{
			bool checkMove = MoveToTargetLocation(badguyID[i], LyubuID, badguyID, terrain);

			if (cur_actor.Play(ONCE, (float)skip, FALSE, TRUE) == 0)
			{
				if (checkMove)
				{
					ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, "Run");
					cur_actor.SetCurrentAction(NULL, 0, CombatIdleID);
				}
				else
				{
					ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, get_monster_atk(badguyID[i]));
					//ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, "NormalAttack2");
					cur_actor.SetCurrentAction(NULL, 0, CombatIdleID);
					LyubuID.blood_remain = LyubuID.blood_remain - GUY_ATTACK * badguyID[i].attack / 10;
				
					FX_FileName.clear();
					FX_FileName.push_back("HitForRobber");
					GenFX(sID, gFXID, dummyID, actorPos, FX_FileName);

					FnMedia BHit;
					BADHIT_ID = FyCreateMediaPlayer("katana-clash3.mp3", 0, 0, 800, 600);
					BHit.Object(BADHIT_ID);
					BHit.SetVolume(1.0f);
					BHit.Play(ONCE);
					
				}
			}
		}
	}

	for (int i = 0; i < NUM_OF_BOSS; i++)
	{
		cur_actor.ID(BossID[i].actorID);
		ACTIONid cur_action = cur_actor.GetCurrentAction(NULL, 0);
		ACTIONid DieID = cur_actor.GetBodyAction(NULL, "Die");

		if (cur_action == DieID)
			cur_actor.Play(ONCE, (float)skip, FALSE, TRUE);
		else if (BossID[i].blood_remain > 0)
		{
			bool checkMove = MoveToTargetLocation(BossID[i], LyubuID, BossID, terrain);

			if (cur_actor.Play(ONCE, (float)skip, FALSE, TRUE) == 0)
			{
				if (checkMove)
				{
					ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, "Run");
					cur_actor.SetCurrentAction(NULL, 0, CombatIdleID);
				}
				else
				{

					if (checkMove)
					{
						ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, "Run");
						cur_actor.SetCurrentAction(NULL, 0, CombatIdleID);
					}
					else
					{
						ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, get_monster_atk(BossID[i]));
						cur_actor.SetCurrentAction(NULL, 0, CombatIdleID);
						LyubuID.blood_remain = LyubuID.blood_remain - GUY_HEAVY_ATTACK;

						FX_FileName.clear();
						FX_FileName.push_back("HitForRobber");
						GenFX(sID, gFXID, dummyID, actorPos, FX_FileName);

						FnMedia BHit;
						BADHIT_ID = FyCreateMediaPlayer("sword-clash1.mp3", 0, 0, 800, 600);
						BHit.Object(BADHIT_ID);
						BHit.SetVolume(1.0f);
						BHit.Play(ONCE);
						
					}
				}
			}
		}
	}


	// Camera's position and direction as a standard for character's location setting
	FnCamera camera;
	camera.ID(cID);
	float cameraPos[3], camerafDir[3], camerauDir[3];
	camera.GetPosition(cameraPos); // not used
	camera.GetDirection(camerafDir, camerauDir);

	float camVertical = camerafDir[2];
	camerafDir[2] = 0; // Guaranteed up direction be 1
	camerauDir[0] = 0; camerauDir[1] = 0; camerauDir[2] = 1;

	float dir[3], origin[3], actorfDir[3], actoruDir[3];// origin is the actor's position      
	actor.GetPosition(origin);
	actor.GetDirection(actorfDir, actoruDir);

	//Generator location 
	genSpot[0][0] = 2905.724;
	genSpot[0][1] = -2856.196;
	genSpot[1][0] = 729.288;
	genSpot[1][1] = -2472.88;
	genSpot[2][0] = -2372;
	genSpot[2][1] = -2700;
	genSpot[3][0] = -1918;
	genSpot[3][1] = 699;
	genSpot[4][0] = 299;
	genSpot[4][1] = 1912;
	genSpot[5][0] = -352;
	genSpot[5][1] = 2296;
	genSpot[6][0] = -700;
	genSpot[6][1] = -1640;

	// Bad guys generator
	for (int i = 0; i < NUM_OF_GENERATOR; i++)
	{
		if (!InArea(origin, genSpot[i], generator[i].range))
		{
			generator[i].in = 0;
			generator[i].double_in = 0;
		}
		if (generator[i].in && InArea(origin, genSpot[i], generator[i].range))
		{
			generator[i].double_in = 1;
		}
		if (InArea(origin, genSpot[i], generator[i].range))
		{
			generator[i].in = 1;
		}

		if (generator[i].in == 1 && generator[i].double_in == 0)
			generator[i].produce(origin, actoruDir, scene, i, NUM_OF_GENERATOR, LyubuID.level);
	}

	origin[2] = origin[2] + HEIGHTOFFSET; // need an offset to probe the hit
	dir[0] = actorfDir[0] / ACTORPROBE;
	dir[1] = actorfDir[1] / ACTORPROBE;
	dir[2] = -1.0f;


	if (dirCount % SMOOTHINESS != 0)
	{
		if ((dirCount + SMOOTHINESS) % SMOOTHINESS < SMOOTHINESS / 2 + 1)
		{
			dirCount--;
			actor.TurnRight(360 / SMOOTHINESS);
		}
		else
		{
			dirCount++;
			actor.TurnRight(360 - 360 / SMOOTHINESS);
		}
	}

	if ((FyCheckHotKeyStatus(FY_UP) || FyCheckHotKeyStatus(FY_S)) && dirCount % SMOOTHINESS == 0)
	{
		actor.SetDirection(camerafDir, camerauDir);
		if (terrain.HitTest(origin, dir) > 0)
			actor.MoveForward(RUNSPEED, FALSE, FALSE, 0, FALSE);
	}
	if (FyCheckHotKeyStatus(FY_D) && dirCount % SMOOTHINESS == 0)
		actor.TurnRight(TURNSPEED);
	if (FyCheckHotKeyStatus(FY_A) && dirCount % SMOOTHINESS == 0)
		actor.TurnRight(360 - TURNSPEED);
	else if (FyCheckHotKeyStatus(FY_RIGHT) && dirCount % SMOOTHINESS == 0)
	{
		actorfDir[0] = camerafDir[1];
		actorfDir[1] = -camerafDir[0];
		actor.SetDirection(actorfDir, actoruDir);
		if (terrain.HitTest(origin, dir) > 0)
		{
			//actor.MoveRight(CALLIBRATION, FALSE, FALSE, 0, FALSE); 
			actor.MoveForward(RUNSPEED / 2, FALSE, FALSE, 0, FALSE);
		}
	}
	else if (FyCheckHotKeyStatus(FY_LEFT) && dirCount % SMOOTHINESS == 0)
	{
		actorfDir[0] = -camerafDir[1];
		actorfDir[1] = camerafDir[0];
		actor.SetDirection(actorfDir, actoruDir);
		if (terrain.HitTest(origin, dir) > 0)
		{
			//actor.MoveRight(CALLIBRATION, FALSE, FALSE, 0, FALSE); 
			actor.MoveForward(RUNSPEED / 2, FALSE, FALSE, 0, FALSE);
		}
	}
	else if (FyCheckHotKeyStatus(FY_DOWN) && dirCount % SMOOTHINESS == 0)
	{
		actor.SetDirection(camerafDir, camerauDir);
		actor.TurnRight(180.0f);
		if (terrain.HitTest(origin, dir) > 0 && camVertical > MAXCAMANGLE)
			actor.MoveForward(RUNSPEED, FALSE, FALSE, 0, FALSE);
	}


	// FX
	if (gFXID != FAILED_ID) {
		FnGameFXSystem gxS(gFXID);
		BOOL4 beOK = gxS.Play((float)skip, ONCE);
		if (!beOK) {
			FnScene scene(sID);
			scene.DeleteGameFXSystem(gFXID);
			gFXID = FAILED_ID;
		}
	}
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

	// render the sprites
	showPicture(sp_hpBlood, "hp_blood.png", (int)250 * ((double)LyubuID.blood_remain / LyubuID.blood_total), 8, 500, 514);
	showPicture(sp_hpMana, "hp_mana.png", (int)250 * ((double)LyubuID.mana_remain / LyubuID.mana_total), 4, 500, 510);

	//if (LyubuID.blood_remain <= 0)
	//	showPicture(sp_deadEnd, "dead_end.png", 800, 600, 0, 0);

	//if (BossID[0].blood_remain <= 0)
	//	showPicture(sp_trueEnd, "true_end.png", 800, 600, 0, 0);

	vp.RenderSprites(sID2, FALSE, TRUE);  // no clear the background but clear the z buffer
	FySwapBuffers();

	// get camera's data
	FnObject terrain;
	terrain.ID(tID);

	float pos[3], fDir[3], uDir[3];
	FnCamera camera;
	camera.ID(cID);

	float actorPos[3], actorfDir[3], actoruDir[3];
	FnCharacter actor;
	actor.ID(actorID);

	actor.GetPosition(actorPos);
	actor.GetDirection(actorfDir, actoruDir);

	camera.GetPosition(pos);
	camera.GetDirection(fDir, uDir);

	// Initialze probing ray direction
	float dir[3], charDir[3];

	dir[0] = -fDir[0] / CAMERAPROBE;
	dir[1] = -fDir[1] / CAMERAPROBE;
	dir[2] = -1;

	charDir[0] = actorfDir[0] / ACTORPROBE;
	charDir[1] = actorfDir[1] / ACTORPROBE;
	charDir[2] = -1;

	if ((FyCheckHotKeyStatus(FY_A) || FyCheckHotKeyStatus(FY_D)) && dirCount % SMOOTHINESS == 0)
	{
		int tempHeight = pos[2];
		pos[2] = actorPos[2];
		int theDistance = FyDistance(pos, actorPos);

		pos[0] = actorPos[0] - theDistance * actorfDir[0];
		pos[1] = actorPos[1] - theDistance * actorfDir[1];
		pos[2] = tempHeight;

		if (pos[2] > actorPos[2] + HEIGHTOFFSET)
		{
			fDir[0] = actorPos[0] - pos[0];
			fDir[1] = actorPos[1] - pos[1];
			fDir[2] = -pos[2];

			uDir[0] = fDir[0] / abs(fDir[0] * fDir[1]);
			uDir[1] = fDir[1] / abs(fDir[0] * fDir[1]);
			uDir[2] = -abs(uDir[0] * fDir[0] + uDir[1] * fDir[1]) / fDir[2];
		}
		else
		{
			fDir[0] = actorfDir[0];
			fDir[1] = actorfDir[1];
			fDir[2] = 0;

			uDir[0] = 0;
			uDir[1] = 0;
			uDir[2] = 1;
		}
	}
	if ((FyCheckHotKeyStatus(FY_UP) || FyCheckHotKeyStatus(FY_S)) && dirCount % SMOOTHINESS == 0)
	{
		if (pos[2] > actorPos[2] + HEIGHTOFFSET)
		{
			pos[2] = actorPos[2];
			pos[2] = actorPos[2] + HEIGHTBOUND + HEIGHTOFFSET - FyDistance(pos, actorPos);

			fDir[0] = actorPos[0] - pos[0];
			fDir[1] = actorPos[1] - pos[1];
			fDir[2] = -pos[2];

			uDir[0] = fDir[0] / abs(fDir[0] * fDir[1]);
			uDir[1] = fDir[1] / abs(fDir[0] * fDir[1]);
			uDir[2] = -abs(uDir[0] * fDir[0] + uDir[1] * fDir[1]) / fDir[2];
		}
		else  // Reset the up direction 
		{
			pos[2] = actorPos[2] + HEIGHTOFFSET;
			fDir[0] = actorfDir[0];		fDir[1] = actorfDir[1];		fDir[2] = 0;
			uDir[0] = 0;	            uDir[1] = 0;	            uDir[2] = 1;
		}

		if (FyDistance(pos, actorPos) > LONGDIST)
		{
			pos[0] = actorPos[0] - LONGDIST * actorfDir[0];
			pos[1] = actorPos[1] - LONGDIST * actorfDir[1];
			pos[2] = actorPos[2] + HEIGHTOFFSET;

			fDir[0] = actorfDir[0];
			fDir[1] = actorfDir[1];
			fDir[2] = -0.0f;

			uDir[0] = actoruDir[0];
			uDir[1] = actoruDir[1];
			uDir[2] = actoruDir[2];

		}
	}

	if (FyCheckHotKeyStatus(FY_RIGHT) && dirCount % SMOOTHINESS == 0)
	{
		actorPos[2] += HEIGHTOFFSET;
		if (terrain.HitTest(actorPos, charDir) <= 0)
		{
			camera.MoveRight(-CAMERASPEED, FALSE, FALSE, 0, FALSE);
			camera.GetPosition(pos);
		}

		fDir[0] = actorPos[0] - pos[0];
		fDir[1] = actorPos[1] - pos[1];
		fDir[2] = 0.0f;
	}
	if (FyCheckHotKeyStatus(FY_LEFT) && dirCount % SMOOTHINESS == 0)
	{
		actorPos[2] += HEIGHTOFFSET;
		if (terrain.HitTest(actorPos, charDir) <= 0)
		{
			camera.MoveRight(CAMERASPEED, FALSE, FALSE, 0, FALSE);
			camera.GetPosition(pos);
		}

		fDir[0] = actorPos[0] - pos[0];
		fDir[1] = actorPos[1] - pos[1];
		fDir[2] = 0.0f;
	}
	if (FyCheckHotKeyStatus(FY_DOWN) && dirCount % SMOOTHINESS == 0)
	{
		if (FyDistance(pos, actorPos) < SHORTDIST)
		{
			if (terrain.HitTest(pos, dir) > 0)
			{
				pos[0] = actorPos[0] + SHORTDIST * actorfDir[0];
				pos[1] = actorPos[1] + SHORTDIST * actorfDir[1];
				pos[2] = actorPos[2] + HEIGHTOFFSET;

				fDir[0] = -actorfDir[0];
				fDir[1] = -actorfDir[1];
				fDir[2] = 0.0f;

				uDir[0] = actoruDir[0];
				uDir[1] = actoruDir[1];
				uDir[2] = actoruDir[2];
			}
			else
			{
				// go back and hit Obstacle : camera look down at the actor

				pos[2] = actorPos[2];
				pos[2] = actorPos[2] + HEIGHTBOUND + HEIGHTOFFSET - FyDistance(pos, actorPos);

				fDir[0] = actorPos[0] - pos[0];
				fDir[1] = actorPos[1] - pos[1];
				fDir[2] = -pos[2];

				uDir[0] = fDir[0] / abs(fDir[0] * fDir[1]);
				uDir[1] = fDir[1] / abs(fDir[0] * fDir[1]);
				uDir[2] = -abs(uDir[0] * fDir[0] + uDir[1] * fDir[1]) / fDir[2];
			}
		}
	}


	if (dirCount % SMOOTHINESS == 0)
	{
		camera.SetPosition(pos);
		camera.SetDirection(fDir, uDir);
	}
	// show frame rate
	static char string[128];
	if (frame == 0) {
		FyTimerReset(0);
	}

	if (frame / 10 * 10 == frame) {
		float curTime;

		curTime = FyTimerCheckTime(0);
		sprintf(string, "Fps: %6.2f", frame / curTime);
	}

	frame += skip;
	if (frame >= 1000) {
		frame = 0;
	}

	FnText text;
	text.ID(textID);

	text.Begin(vID);
	text.Write(string, 20, 20, 255, 0, 0);

	char posS[256], fDirS[256], uDirS[256], debug[256];
	sprintf(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
	sprintf(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
	sprintf(uDirS, "HP  %d / %d   MP %d / %d   Attack %d", LyubuID.blood_remain, LyubuID.blood_total, LyubuID.mana_remain, LyubuID.mana_total, LyubuID.attack);
	sprintf(debug, "Level %d   Exp %d   Next %d", LyubuID.level, LyubuID.exp_cur, LyubuID.exp_total);

	text.Write(posS, 20, 35, 255, 255, 0);
	text.Write(fDirS, 20, 50, 255, 255, 0);
	text.Write(uDirS, 20, 65, 255, 255, 0);
	text.Write(debug, 20, 90, 255, 255, 0);

	text.End();

	// swap buffer
	FySwapBuffers();
}


/*------------------
movement control
-------------------*/
void Movement(BYTE code, BOOL4 value)
{
	// only hotkey input can trigger this function

	FnCharacter actor;
	FnCharacter badguy;
	vector<char*> FX_FileName;

	float actorPos[3], actorfDir[3], actoruDir[3];
	float badguyPos[3], badguyfDir[3], badguyuDir[3];

	actor.ID(actorID);
	actor.GetPosition(actorPos);
	actor.GetDirection(actorfDir, actoruDir);

	// use a global idle_count variable to memorize the action status.

	if (value)
	{
		idle_count++;

		if (FyCheckHotKeyStatus(FY_Q) && dirCount % SMOOTHINESS == 0 && LyubuID.mana_remain > 0)
		{
			actorPos[0] += actorfDir[0] * HATKOFFSET;
			actorPos[1] += actorfDir[1] * HATKOFFSET;

			LyubuID.mana_remain = LyubuID.mana_remain - HATKDAMAGE * HATKDAMAGE / 100;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < HATKRANGE && badguyID[i].blood_remain > 0)
				{
					
					//FnMedia dam;
					//damage_ID = FyCreateMediaPlayer("damage6.mp3", 0, 0, 800, 600);
					//dam.Object(damage_ID);
					//dam.Play(LOOP);
					//dam.SetVolume(1.0f);
					
					
					if (!ActAction(badguyID[i], get_monster_act(badguyID[i]), HATKDAMAGE * LyubuID.attack / 20))
					{
						FnMedia Die;
						DIE_ID = FyCreateMediaPlayer("samurai_shouting1.mp3", 0, 0, 800, 600);
						Die.Object(DIE_ID);
						Die.Play(ONCE);
						Die.SetVolume(1.0f);
						
						LyubuID.exp_cur += badguyID[i].exp_cur;
						chk_levelup(LyubuID);
					}
				}

				//FX_FileName.clear();
				//FX_FileName.push_back("LyubuDamege");
				//GenFX(sID, gFXID, dummyID, badguyPos, FX_FileName);
			}

			for (int i = 0; i < NUM_OF_BOSS; i++)
			{
				badguy.ID(BossID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < HATKRANGE && BossID[i].blood_remain > 0)
				{
					FnMedia dam;
					damage_ID = FyCreateMediaPlayer("damage6.mp3", 0, 0, 800, 600);
					dam.Object(damage_ID);
					dam.Play(LOOP);
					dam.SetVolume(1.0f);
					
					ActAction(BossID[i], get_monster_act(BossID[i]), HATKDAMAGE * LyubuID.attack / 20);
				
					FX_FileName.clear();
					FX_FileName.push_back("LyubuDamege");
					GenFX(sID, gFXID, dummyID, badguyPos, FX_FileName);
				}
			}


		}

		if (FyCheckHotKeyStatus(FY_W) && dirCount % SMOOTHINESS == 0/* && LyubuID.mana_remain > 0*/)
		{

			actorPos[0] += actorfDir[0] * NATK1OFFSET;
			actorPos[1] += actorfDir[1] * NATK1OFFSET;

			LyubuID.mana_remain = LyubuID.mana_remain - NATK1DAMAGE * NATK1RANGE / 100;

			LyubuID.exp_cur += 50;                            // Debugging
			chk_levelup(LyubuID);							  // Debugging

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < NATK1RANGE && badguyID[i].blood_remain > 0)
				{
					//FnMedia dam;
					//damage_ID = FyCreateMediaPlayer("damage6.mp3", 0, 0, 800, 600);
					//dam.Object(damage_ID);
					//dam.Play(LOOP);
					//dam.SetVolume(1.0f);
					
					if (!ActAction(badguyID[i], get_monster_act(badguyID[i]), NATK1DAMAGE * LyubuID.attack / 20))
					{
						FnMedia Die;
						DIE_ID = FyCreateMediaPlayer("samurai_shouting1.mp3", 0, 0, 800, 600);
						Die.Object(DIE_ID);
						Die.Play(ONCE);
						Die.SetVolume(1.0f);
						
						LyubuID.exp_cur += badguyID[i].exp_cur;
						chk_levelup(LyubuID);
					}
				}

				//FX_FileName.clear();
				//FX_FileName.push_back("MonGotHit");
				//GenFX(sID, gFXID, dummyID, badguyPos, FX_FileName);
			}

			for (int i = 0; i < NUM_OF_BOSS; i++)
			{
				badguy.ID(BossID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < NATK1RANGE && BossID[i].blood_remain > 0)
				{
					FnMedia dam;
					damage_ID = FyCreateMediaPlayer("damage6.mp3", 0, 0, 800, 600);
					dam.Object(damage_ID);
					dam.Play(LOOP);
					dam.SetVolume(1.0f);
					
					ActAction(BossID[i], get_monster_act(BossID[i]), NATK1DAMAGE * LyubuID.attack / 20);
				
					FX_FileName.clear();
					FX_FileName.push_back("MonGotHit2G02");
					GenFX(sID, gFXID, dummyID, badguyPos, FX_FileName);
				}
			}

		}

		if (FyCheckHotKeyStatus(FY_E) && dirCount % SMOOTHINESS == 0 && LyubuID.mana_remain > 0)
		{
			actorPos[0] += actorfDir[0] * NATK2OFFSET;
			actorPos[1] += actorfDir[1] * NATK2OFFSET;


			LyubuID.mana_remain = LyubuID.mana_remain - NATK2DAMAGE * NATK2DAMAGE / 100;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < NATK2RANGE && badguyID[i].blood_remain > 0)
				{
					//FnMedia dam;
					//damage_ID = FyCreateMediaPlayer("damage6.mp3", 0, 0, 800, 600);
					//dam.Object(damage_ID);
					//dam.Play(LOOP);
					//dam.SetVolume(1.0f);
					
					if (!ActAction(badguyID[i], get_monster_act(badguyID[i]), NATK2DAMAGE * LyubuID.attack / 20))
					{
						FnMedia Die;
						DIE_ID = FyCreateMediaPlayer("samurai_shouting1.mp3", 0, 0, 800, 600);
						Die.Object(DIE_ID);
						Die.Play(ONCE);
						Die.SetVolume(1.0f);
						
						LyubuID.exp_cur += badguyID[i].exp_cur;
						chk_levelup(LyubuID);
					}
				}

				//FX_FileName.clear();
				//FX_FileName.push_back("Lyubu_atk01 -X");
				//GenFX(sID, gFXID, dummyID, badguyPos, FX_FileName);
			}

			for (int i = 0; i < NUM_OF_BOSS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < NATK2RANGE && BossID[i].blood_remain > 0)
				{
					FnMedia dam;
					damage_ID = FyCreateMediaPlayer("damage6.mp3", 0, 0, 800, 600);
					dam.Object(damage_ID);
					dam.Play(LOOP);
					dam.SetVolume(1.0f);
					
					ActAction(BossID[i], get_monster_act(BossID[i]), NATK2DAMAGE);
					
					FX_FileName.clear();
					FX_FileName.push_back("Lyubu_atk01 -X");
					GenFX(sID, gFXID, dummyID, badguyPos, FX_FileName);
				}
			}


		}
		if ((FyCheckHotKeyStatus(FY_UP) || FyCheckHotKeyStatus(FY_S)) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = 0 - dirState;
			dirState = 0;
		}
		if (FyCheckHotKeyStatus(FY_RIGHT) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = SMOOTHINESS / 4 - dirState;
			dirState = SMOOTHINESS / 4;
		}
		if (FyCheckHotKeyStatus(FY_LEFT) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = SMOOTHINESS * 3 / 4 - dirState;
			dirState = SMOOTHINESS * 3 / 4;
		}
		if (FyCheckHotKeyStatus(FY_DOWN) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = SMOOTHINESS / 2 - dirState;
			dirState = SMOOTHINESS / 2;
		}
	}
	else
	{
		idle_count--;
	}

	// 3. the actor is idle when idle_count is equal to zero.   


	if (idle_count > 0)
	{

		if (FyCheckHotKeyStatus(FY_Q) && dirCount % SMOOTHINESS == 0 && LyubuID.mana_remain > 0)
		{
			curPoseID = NormalAttack1ID;
	
			FnMedia hit;
			hit_ID = FyCreateMediaPlayer("sword-slash4.mp3", 0, 0, 800, 600);
			hit.Object(hit_ID);
			hit.Play(LOOP);
			hit.SetVolume(1.0f);

			FX_FileName.clear();
			FX_FileName.push_back("AttacKBasic");
			FX_FileName.push_back("Lyubu_skill01");
			FX_FileName.push_back("LyubuDamege");
			GenFX(sID, gFXID, dummyID, actorPos, FX_FileName);
		}
		else if (FyCheckHotKeyStatus(FY_W) && dirCount % SMOOTHINESS == 0 && LyubuID.mana_remain > 0)
		{
			curPoseID = NormalAttack2ID;

			FnMedia hit;
			hit_ID = FyCreateMediaPlayer("sword-slash4.mp3", 0, 0, 800, 600);
			hit.Object(hit_ID);
			hit.Play(LOOP);
			hit.SetVolume(1.0f);

			FX_FileName.clear();
			FX_FileName.push_back("AttacKBasic");
			FX_FileName.push_back("Lyubu_skill02");
			FX_FileName.push_back("Lyubu_atk01");
			FX_FileName.push_back("MonGotHit_1");
			GenFX(sID, gFXID, dummyID, actorPos, FX_FileName);
		}
		else if (FyCheckHotKeyStatus(FY_E) && dirCount % SMOOTHINESS == 0 && LyubuID.mana_remain > 0)
		{
			curPoseID = HeavyAttack1ID;

			FnMedia hit;
			hit_ID = FyCreateMediaPlayer("sword-slash4.mp3", 0, 0, 800, 600);
			hit.Object(hit_ID);
			hit.Play(LOOP);
			hit.SetVolume(1.0f);

			FX_FileName.clear();
			FX_FileName.push_back("AttacKBasic");
			FX_FileName.push_back("Lyubu_skill03");
			FX_FileName.push_back("MonGotHit2G04");
			//FX_FileName.push_back("Lyubu_atk01 - X");
			GenFX(sID, gFXID, dummyID, actorPos, FX_FileName);
		}
		else if (!FyCheckHotKeyStatus(FY_T) && !FyCheckHotKeyStatus(FY_M) &&
			!FyCheckHotKeyStatus(FY_Q) && !FyCheckHotKeyStatus(FY_W) &&
			!FyCheckHotKeyStatus(FY_E))
		{
			curPoseID = runID;

			FnMedia runMedia;
			RunMusuc_ID = FyCreateMediaPlayer("running2.mp3", 0, 0, 800, 600);
			runMedia.Object(RunMusuc_ID);
			runMedia.Play(ONCE);
			runMedia.SetVolume(1.0f);

			FX_FileName.clear();
			FX_FileName.push_back("RunFX");
			GenFX(sID, gFXID, dummyID, actorPos, FX_FileName);
		}
		else
		{
			curPoseID = idleID;
		}

		actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
	}
	else if (curPoseID == runID)
	{
		curPoseID = idleID;
		actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
	}








	if (FyCheckHotKeyStatus(FY_T) && picture_count <= 8)
	{
		char str_buf[16];
		itoa(picture_count, str_buf, 10);
		string pic_num(str_buf);

		string picture = "startTalk_" + pic_num + ".png";
		string talk = "startTalk_" + pic_num + ".mp3";
		char* chr = strdup(picture.c_str());
		char* chr2 = strdup(talk.c_str());


		showPicture(sp, chr, 780, 180, 10, 10); //showPicture parameter : FnSprite ,imageName, size, position

		FnMedia runMedia;
		RunMusuc_ID = FyCreateMediaPlayer(chr2, 0, 0, 800, 600);
		runMedia.Object(RunMusuc_ID);
		runMedia.SetVolume(10.0f);
		runMedia.Play(ONCE);
		


		free(chr);
		picture_count++;
	}
	else if (FyCheckHotKeyStatus(FY_T) && picture_count == 9) {
		showPicture(sp, "mission_transparent.png", 580, 580, 110, 15);
		picture_count++;
	}
	else if (FyCheckHotKeyStatus(FY_T) && picture_count > 9) {
		showPicture(sp, "", 0, 0, 0, 0);
	}




	if (FyCheckHotKeyStatus(FY_M) && missionWindowStatus == FALSE  && picture_count > 9)
	{
		showPicture(sp, "mission_transparent.png", 580, 580, 110, 15);
		missionWindowStatus = TRUE;

	}
	else if (FyCheckHotKeyStatus(FY_M) && missionWindowStatus == TRUE  && picture_count > 9)
	{
		showPicture(sp, "", 0, 0, 0, 0);
		missionWindowStatus = FALSE;
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
			FyQuitFlyWin32();
		}
	}
}

/*-----------------------------------
initialize the pivot of the camera
C.Wang 0329, 2005
------------------------------------*/
void InitPivot(int x, int y)
{
	oldX = x;
	oldY = y;
	frame = 0;
}


/*------------------
pivot the camera
C.Wang 0329, 2005
-------------------*/
void PivotCam(int x, int y)
{
	FnObject model;

	if (x != oldX) {
		model.ID(cID);
		model.Rotate(Z_AXIS, (float)(x - oldX), GLOBAL);
		oldX = x;
	}

	if (y != oldY) {
		model.ID(cID);
		model.Rotate(X_AXIS, (float)(y - oldY), GLOBAL);
		oldY = y;
	}
}


/*----------------------------------
initialize the move of the camera
C.Wang 0329, 2005
-----------------------------------*/
void InitMove(int x, int y)
{
	oldXM = x;
	oldYM = y;
	frame = 0;
}


/*------------------
move the camera
C.Wang 0329, 2005
-------------------*/
void MoveCam(int x, int y)
{
	if (x != oldXM) {
		FnObject model;

		model.ID(cID);
		model.Translate((float)(x - oldXM)*2.0f, 0.0f, 0.0f, LOCAL);
		oldXM = x;
	}
	if (y != oldYM) {
		FnObject model;

		model.ID(cID);
		model.Translate(0.0f, (float)(oldYM - y)*2.0f, 0.0f, LOCAL);
		oldYM = y;
	}
}


/*----------------------------------
initialize the zoom of the camera
C.Wang 0329, 2005
-----------------------------------*/
void InitZoom(int x, int y)
{
	oldXMM = x;
	oldYMM = y;
	frame = 0;
}


/*------------------
zoom the camera
C.Wang 0329, 2005
-------------------*/
void ZoomCam(int x, int y)
{
	if (x != oldXMM || y != oldYMM) {
		FnObject model;

		model.ID(cID);
		model.Translate(0.0f, 0.0f, (float)(x - oldXMM)*10.0f, LOCAL);
		oldXMM = x;
		oldYMM = y;
	}
}


void ProduceBadguys::produce(float* pos, float* uDir, FnScene scene, int generatorNUM, int totalgenNUM, int level)
{
	float temp_pos[3];
	float temp_fDir[3];
	int populationMAX = NUM_OF_BADGUYS / totalgenNUM;

	for (int i = generatorNUM * populationMAX; i < (generatorNUM + 1) * populationMAX; i++)
	{
		if (!badguyID[i].alive)
		{
			temp_pos[0] = pos[0] + 30 * (rand() % 8);
			temp_pos[1] = pos[1] + 30 * (rand() % 8);
			temp_pos[2] = pos[2] + 30 * (rand() % 8);
			temp_fDir[0] = -1.0f; temp_fDir[1] = -1.0f; temp_fDir[2] = 1.0f;

			badguyID[i].reset();
			badguylevelup(level, badguyID[i]);
			if (level < 3) {              //     Level settings just for debug
				badguyID[i].idtype = ROBBER;
				ActorGen(scene, terrainRoomID, badguyID[i], "Robber02", "CombatIdle", temp_pos, temp_fDir, uDir);
			}
			else if (level < 4)
			{
				badguyID[i].idtype = AMA001;
				ActorGen(scene, terrainRoomID, badguyID[i], "AMA001", "CombatIdle", temp_pos, temp_fDir, uDir);
			}
			else if (level < 5)
			{
				badguyID[i].idtype = AMA002;
				ActorGen(scene, terrainRoomID, badguyID[i], "AMA002", "CombatIdle", temp_pos, temp_fDir, uDir);
			}
			else if (level < 6)
			{
				badguyID[i].idtype = AMA003;
				ActorGen(scene, terrainRoomID, badguyID[i], "AMA003", "CombatIdle", temp_pos, temp_fDir, uDir);
			}
			else if (level < 7)
			{
				badguyID[i].idtype = AMA004;
				ActorGen(scene, terrainRoomID, badguyID[i], "AMA004", "CombatIdle", temp_pos, temp_fDir, uDir);
			}
			else if (level < 8)
			{
				badguyID[i].idtype = AMA005;
				ActorGen(scene, terrainRoomID, badguyID[i], "AMA005", "CombatIdle", temp_pos, temp_fDir, uDir);
			}
			else
			{
				badguyID[i].idtype = AMA006;
				ActorGen(scene, terrainRoomID, badguyID[i], "AMA006", "CombatIdle", temp_pos, temp_fDir, uDir);
			}
			break;
		}
	}
}