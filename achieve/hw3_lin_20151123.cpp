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

#define SHORTDIST 500
#define LONGDIST 1000 
#define HEIGHTBOUND 500
#define HEIGHTOFFSET 70
#define ACTORPROBE 5
#define CAMERAPROBE 5
#define MAXCAMANGLE -0.990
#define RUNSPEED 10.0f
#define CAMERASPEED 5.0f
#define CALLIBRATION 1.0f
#define SMOOTHINESS 16
#define NATK1OFFSET 100
#define NATK1RANGE 100
#define NATK2RANGE 150
#define HATKOFFSET 100
#define HATKRANGE 100
#define NATKDAMAGE1 7
#define NATKDAMAGE2 5
#define HATKDAMAGE 10

VIEWPORTid vID;                 // the major viewport
SCENEid sID;                    // the 3D scene
OBJECTid cID, tID;              // the main camera and the terrain for terrain following
CHARACTERid actorID, actorID_Don, actorID_Rob;            // the major character
ACTIONid idleID, idleID_Don, idleID_Rob, runID, runID_Don, runID_Rob, curPoseID, curPoseID_Don, curPoseID_Rob; // two actions
ACTIONid Natt1ID, Natt2ID, Hatt1ID;
ACTIONid Damage1ID_Don, Damage2ID_Don, DieID_Don;
ACTIONid Damage1ID_Rob, Damage2ID_Rob, DieID_Rob;
ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;

// some globals
int frame = 0;
int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;
int dirCount = 0, dirState = 0;
// memorize idle/run status
int idle_count = 0;
int life_Lyu = 100, life_Don = 150, life_Rob = 20;

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
	actorID_Don = scene.LoadCharacter("Donzo2");
	actorID_Rob = scene.LoadCharacter("Robber02");

	// put the character on terrain
	float pos[3], fDir[3], uDir[3];
	FnCharacter actor;
	actor.ID(actorID);
	pos[0] = 3569.0f; pos[1] = -3208.0f; pos[2] = 1000.0f;
	fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
	uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;
	actor.SetDirection(fDir, uDir);

	actor.SetTerrainRoom(terrainRoomID, 10.0f);
	beOK = actor.PutOnTerrain(pos);

	// Get two character actions pre-defined at Lyubu2
	idleID = actor.GetBodyAction(NULL, "Idle");
	runID = actor.GetBodyAction(NULL, "Run");
	Natt1ID = actor.GetBodyAction(NULL, "NormalAttack1");
	Natt2ID = actor.GetBodyAction(NULL, "NormalAttack2");
	Hatt1ID = actor.GetBodyAction(NULL, "HeavyAttack1");
	// set the character to idle action
	//curPoseID = idleID;
	curPoseID = idleID;
	actor.SetCurrentAction(NULL, 0, curPoseID, 10.0f);
	actor.Play(START, 0.0f, FALSE, TRUE);

	// translate the camera
	cID = scene.CreateObject(CAMERA);
	FnCamera camera;
	camera.ID(cID);
	camera.SetNearPlane(5.0f);
	camera.SetFarPlane(100000.0f);

	// set camera initial position and orientation
	//pos[0] = 4315.783f; pos[1] = -3199.686f; pos[2] = 93.046f;
	//fDir[0] = -0.983f; fDir[1] = -0.143f; fDir[2] = -0.119f;
	//uDir[0] = -0.116f; uDir[1] = -0.031f; uDir[2] = 0.993f;

	// Donzo---------------------------------------------------
	FnCharacter actor_Don;
	actor_Don.ID(actorID_Don);
	pos[0] = 3400.0f; pos[1] = -3008.0f; pos[2] = 1000.0f;
	fDir[0] = -1.0f; fDir[1] = -1.0f;
	actor_Don.SetDirection(fDir, uDir);

	actor_Don.SetTerrainRoom(terrainRoomID, 10.0f);
	beOK = actor_Don.PutOnTerrain(pos);

	// Get two character actions pre-defined at Lyubu2
	idleID_Don = actor_Don.GetBodyAction(NULL, "CombatIdle");
	runID_Don = actor_Don.GetBodyAction(NULL, "Run");
	Damage1ID_Don = actor_Don.GetBodyAction(NULL, "Damage1");
	Damage2ID_Don = actor_Don.GetBodyAction(NULL, "Damage2");
	DieID_Don = actor_Don.GetBodyAction(NULL, "Die");

	// set the character to idle action
	//curPoseID = idleID;
	curPoseID_Don = idleID_Don;
	actor_Don.SetCurrentAction(NULL, 0, curPoseID_Don, 10.0f);
	actor_Don.Play(START, 0.0f, FALSE, TRUE);

	// --------------------------------------------------------
	// Robber--------------------------------------------------

	FnCharacter actor_Rob;
	actor_Rob.ID(actorID_Rob);
	pos[0] = 3600.0f; pos[1] = -3200.0f; pos[2] = 1000.0f;
	fDir[0] = -1.0f; fDir[1] = -1.0f;
	actor_Rob.SetDirection(fDir, uDir);

	actor_Rob.SetTerrainRoom(terrainRoomID, 10.0f);
	beOK = actor_Rob.PutOnTerrain(pos);

	// Get two character actions pre-defined at Lyubu2
	idleID_Rob = actor_Rob.GetBodyAction(NULL, "CombatIdle");
	runID_Rob = actor_Rob.GetBodyAction(NULL, "Run");
	Damage1ID_Rob = actor_Rob.GetBodyAction(NULL, "Damage1");
	Damage2ID_Rob = actor_Rob.GetBodyAction(NULL, "Damage2");
	DieID_Rob = actor_Rob.GetBodyAction(NULL, "Die");

	// set the character to idle action
	//curPoseID = idleID;
	curPoseID_Rob = idleID_Rob;
	actor_Rob.SetCurrentAction(NULL, 0, curPoseID_Rob, 10.0f);
	actor_Rob.Play(START, 0.0f, FALSE, TRUE);

	// --------------------------------------------------------

	//------------------------HW2 init-----------------------//
	actor.GetPosition(pos);
	actor.GetDirection(fDir, uDir);

	pos[0] = pos[0] - SHORTDIST * fDir[0];
	pos[1] = pos[1] - SHORTDIST * fDir[1];
	pos[2] = pos[2] + HEIGHTOFFSET;

	fDir[0] = 1.0f;
	fDir[1] = 1.0f;
	fDir[2] = 0.0f;

	uDir[0] = 0.0f;
	uDir[1] = 0.0f;
	uDir[2] = 1.0f;


	//------------------------HW2 end------------------------//
	camera.SetPosition(pos);
	camera.SetDirection(fDir, uDir);

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
	FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);  // escape for quiting the game
	FyDefineHotKey(FY_UP, Movement, FALSE);      // Up for moving forward
	FyDefineHotKey(FY_RIGHT, Movement, FALSE);   // Right for turning right
	FyDefineHotKey(FY_LEFT, Movement, FALSE);    // Left for turning left
	// HW2
	FyDefineHotKey(FY_DOWN, Movement, FALSE);
	// HW3
	FyDefineHotKey(FY_A, Movement, FALSE);
	FyDefineHotKey(FY_D, Movement, FALSE);
	FyDefineHotKey(FY_F, Movement, FALSE);

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
	FnCharacter actor_Don;
	FnCharacter actor_Rob;

	// play character pose
	actor.ID(actorID);
	actor_Don.ID(actorID_Don);
	actor_Rob.ID(actorID_Rob);
	if (curPoseID == runID || curPoseID == idleID)
	{
		actor.Play(LOOP, (float)skip, FALSE, TRUE);
	}
	else if (actor.Play(ONCE, (float)skip, FALSE, TRUE) == 0)
	{
		curPoseID = idleID;
		actor.SetCurrentAction(NULL, 0, curPoseID, 10.0f);
	}
	actor_Don.Play(ONCE, (float)skip, FALSE, TRUE);
	actor_Rob.Play(ONCE, (float)skip, FALSE, TRUE);

	// Camera's position and direction as a standard for character's location setting
	FnCamera camera;
	camera.ID(cID);
	float cameraPos[3], camerafDir[3], camerauDir[3];
	camera.GetPosition(cameraPos); // not used
	camera.GetDirection(camerafDir, camerauDir);

	float camVertical = camerafDir[2];
	camerafDir[2] = 0; // Guaranteed up direction be 1
	camerauDir[0] = 0; camerauDir[1] = 0; camerauDir[2] = 1;

	FnObject terrain;
	terrain.ID(tID);
	float dir[3], origin[3], fDir[3], uDir[3];// origin is the actor's position      

	actor.GetPosition(origin);
	actor.GetDirection(fDir, uDir);

	origin[2] = origin[2] + HEIGHTOFFSET; // need an offset to probe the hit
	dir[0] = fDir[0] / ACTORPROBE;
	dir[1] = fDir[1] / ACTORPROBE;
	dir[2] = -1.0f;

	//////////////////////////////////////////
	// Homework #01 part 1
	//////////////////////////////////////////

	// check the hotkey status and make the actor move

	if (dirCount % SMOOTHINESS != 0)
	{
		if ((dirCount + SMOOTHINESS) % SMOOTHINESS < SMOOTHINESS/2 + 1)
		{
			dirCount--;
			actor.TurnRight(360/SMOOTHINESS);
		}
		else
		{
			dirCount++;
			actor.TurnRight(360 - 360/SMOOTHINESS);
		}
	}

	if (FyCheckHotKeyStatus(FY_UP) && dirCount % SMOOTHINESS == 0)
	{
		actor.SetDirection(camerafDir, camerauDir);
		if (terrain.HitTest(origin, dir) > 0)
		{
			actor.MoveForward(RUNSPEED, FALSE, FALSE, 0, FALSE);
		}
	}
	else if (FyCheckHotKeyStatus(FY_RIGHT) && dirCount % SMOOTHINESS == 0)
	{
		fDir[0] = camerafDir[1];
		fDir[1] = -camerafDir[0];
		actor.SetDirection(fDir, uDir);
		if (terrain.HitTest(origin, dir) > 0)
		{
			//actor.MoveRight(CALLIBRATION, FALSE, FALSE, 0, FALSE);
			actor.MoveForward(RUNSPEED, FALSE, FALSE, 0, FALSE);
		}
	}
	else if (FyCheckHotKeyStatus(FY_LEFT) && dirCount % SMOOTHINESS == 0)
	{
		fDir[0] = -camerafDir[1];
		fDir[1] = camerafDir[0];
		actor.SetDirection(fDir, uDir);
		if (terrain.HitTest(origin, dir) > 0)
		{
			//actor.MoveRight(CALLIBRATION, FALSE, FALSE, 0, FALSE);
			actor.MoveForward(RUNSPEED, FALSE, FALSE, 0, FALSE);
		}
	}
	else if (FyCheckHotKeyStatus(FY_DOWN) && dirCount % SMOOTHINESS == 0)
	{
		actor.SetDirection(camerafDir, camerauDir);
		actor.TurnRight(180.0f);
		if (terrain.HitTest(origin, dir) > 0 && camVertical > MAXCAMANGLE)
		{
			actor.MoveForward(RUNSPEED, FALSE, FALSE, 0, FALSE);
		}
	}

	/*

	[note] Movement

	control the character Move forward / move right / Move up
	-- Turn right / turn left / tilt
	-- We need to define the orientation of an object or a character :
	position
	-- Its facing direction
	-- Its up direction
	keyboard
	-- FyDefineHotKey(FY_UP, Movement, FALSE);       // Up for moving forward
	-- FyDefineHotKey(FY_RIGHT, Movement, FALSE);    // Right for turning right
	-- FyDefineHotKey(FY_LEFT, Movement, FALSE);     // Left for turning left

	*/


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
	camera.ID(cID);

	FnObject terrain;
	terrain.ID(tID);

	float pos[3], fDir[3], uDir[3];

	//-----------------HW2 added-----------------------//

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

	if (FyCheckHotKeyStatus(FY_UP) && dirCount % SMOOTHINESS == 0)
	{
		if (/*terrain.HitTest(pos, dir) < 0 && */pos[2] > actorPos[2] + HEIGHTOFFSET)
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
			uDir[0] = 0;	uDir[1] = 0;	uDir[2] = 1;
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
		fDir[2] = 0;
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
		fDir[2] = 0;
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

	//-----------------HW2 end-------------------------//
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
	sprintf(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);
	//sprintf(debug, "numerical: %d ", turn);

	text.Write(posS, 20, 35, 255, 255, 0);
	text.Write(fDirS, 20, 50, 255, 255, 0);
	text.Write(uDirS, 20, 65, 255, 255, 0);
	//text.Write(debug, 20, 90, 255, 255, 0);

	text.End();

	// swap buffer
	FySwapBuffers();
}


/*------------------
movement control
-------------------*/
void Movement(BYTE code, BOOL4 value)
{
	//////////////////////////////////////////
	// Homework #01 part 2
	//////////////////////////////////////////

	// note : only hotkey input can trigger this function

	// 1. get the actor
	FnCharacter actor, actor_Don, actor_Rob;
	actor.ID(actorID);
	actor_Don.ID(actorID_Don);
	actor_Rob.ID(actorID_Rob);
	float pos[3], fDir[3], uDir[3], pos2[3];

	// 2. use a global idle_count variable to memorize the action status.
	//    (to prevent the "slide" actor bug)
	if (value)
	{
		idle_count++;
		if (FyCheckHotKeyStatus(FY_A) && dirCount % SMOOTHINESS == 0)
		{
			actor.GetPosition(pos);
			actor.GetDirection(fDir, uDir);
			pos[0] += fDir[0] * HATKOFFSET;
			pos[1] += fDir[1] * HATKOFFSET;
			actor_Don.GetPosition(pos2);
			if (FyDistance(pos, pos2) < HATKRANGE && life_Don > 0)
			{
				life_Don -= HATKDAMAGE;
				curPoseID_Don = runID_Don;
				if (life_Don <= 0)
				{
					curPoseID_Don = DieID_Don;
				}
				actor_Don.SetCurrentAction(NULL, 0, curPoseID_Don, 10.0f);
			}
			actor_Rob.GetPosition(pos2);
			if (FyDistance(pos, pos2) < HATKRANGE && life_Rob > 0)
			{
				life_Rob -= HATKDAMAGE;
				curPoseID_Rob = Damage1ID_Rob;
				if (life_Rob <= 0)
				{
					curPoseID_Rob = DieID_Rob;
				}
				actor_Rob.SetCurrentAction(NULL, 0, curPoseID_Rob, 10.0f);
			}
		}
		if (FyCheckHotKeyStatus(FY_D) && dirCount % SMOOTHINESS == 0)
		{
			actor.GetPosition(pos);
			actor.GetDirection(fDir, uDir);
			pos[0] += fDir[0] * NATK1OFFSET;
			pos[1] += fDir[1] * NATK1OFFSET;
			actor_Don.GetPosition(pos2);
			if (FyDistance(pos, pos2) < NATK1RANGE && life_Don > 0)
			{
				life_Don -= NATKDAMAGE1;
				curPoseID_Don = runID_Don;
				if (life_Don <= 0)
				{
					curPoseID_Don = DieID_Don;
				}
				actor_Don.SetCurrentAction(NULL, 0, curPoseID_Don, 10.0f);
			}
			actor_Rob.GetPosition(pos2);
			if (FyDistance(pos, pos2) < NATK1RANGE && life_Rob > 0)
			{
				life_Rob -= NATKDAMAGE1;
				curPoseID_Rob = Damage1ID_Rob;
				if (life_Rob <= 0)
				{
					curPoseID_Rob = DieID_Rob;
				}
				actor_Rob.SetCurrentAction(NULL, 0, curPoseID_Rob, 10.0f);
			}
		}
		if (FyCheckHotKeyStatus(FY_F) && dirCount % SMOOTHINESS == 0)
		{
			actor.GetPosition(pos);
			actor.GetDirection(fDir, uDir);
			actor_Don.GetPosition(pos2);
			if (FyDistance(pos, pos2) < NATK2RANGE && life_Don > 0)
			{
				life_Don -= NATKDAMAGE2;
				curPoseID_Don = runID_Don;
				if (life_Don <= 0)
				{
					curPoseID_Don = DieID_Don;
				}
				actor_Don.SetCurrentAction(NULL, 0, curPoseID_Don, 10.0f);
			}
			actor_Rob.GetPosition(pos2);
			if (FyDistance(pos, pos2) < NATK2RANGE && life_Rob > 0)
			{
				life_Rob -= NATKDAMAGE2;
				curPoseID_Rob = Damage1ID_Rob;
				if (life_Rob <= 0)
				{
					curPoseID_Rob = DieID_Rob;
				}
				actor_Rob.SetCurrentAction(NULL, 0, curPoseID_Rob, 10.0f);
			}
		}
		if (FyCheckHotKeyStatus(FY_UP) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = 0 - dirState;
			dirState = 0;
		}
		if (FyCheckHotKeyStatus(FY_RIGHT) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = SMOOTHINESS /4 - dirState;
			dirState = SMOOTHINESS /4;
		}
		if (FyCheckHotKeyStatus(FY_LEFT) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = SMOOTHINESS * 3/4 - dirState;
			dirState = SMOOTHINESS * 3/4;
		}
		if (FyCheckHotKeyStatus(FY_DOWN) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = SMOOTHINESS /2 - dirState;
			dirState = SMOOTHINESS /2;
		}
	}
	else
		idle_count--;



	// 3. the actor is idle when idle_count equal to zero.   
	if (idle_count > 0)
	{
		if (FyCheckHotKeyStatus(FY_A) && dirCount % SMOOTHINESS == 0)
		{
			curPoseID = Hatt1ID;
			
		}
		else if (FyCheckHotKeyStatus(FY_D) && dirCount % SMOOTHINESS == 0)
		{
			curPoseID = Natt1ID;
			
		}
		else if (FyCheckHotKeyStatus(FY_F) && dirCount % SMOOTHINESS == 0)
		{
			curPoseID = Natt2ID;
		}
		else
		{
			curPoseID = runID;
		}
		actor.SetCurrentAction(NULL, 0, curPoseID, 10.0f);
		actor.Play(ONCE, 0.0f, FALSE, TRUE);
	}
	else if (curPoseID == runID)
	{
		curPoseID = idleID;
		actor.SetCurrentAction(NULL, 0, curPoseID, 10.0f);
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