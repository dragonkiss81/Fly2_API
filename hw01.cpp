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
#include <vector>
using namespace std;

#define SHORTDIST 400
#define LONGDIST 500
#define HEIGHTBOUND 500
#define HEIGHTOFFSET 70
#define ACTORPROBE 5
#define CAMERAPROBE 5
#define MAXCAMANGLE -0.990
#define RUNSPEED 15.0f
#define CAMERASPEED 10.0f
#define CALLIBRATION 1.0f
#define SMOOTHINESS 1
#define NUM_OF_BADGUYS 10

VIEWPORTid vID;                 // the major viewport
SCENEid sID;                    // the 3D scene
OBJECTid cID, tID;              // the main camera and the terrain for terrain following
CHARACTERid actorID;            // the major character
ACTIONid idleID, runID, curPoseID;
ACTIONid NormalAttack1ID, NormalAttack2ID, HeavyAttack1ID;
ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;

// Blood System Class declaration
class ACTNUM {
public:
	ACTNUM();
	CHARACTERid actorID;
	int blood_total;
	int blood_remain;
	int mana_total;
	int mana_remain;
}; vector<ACTNUM> badguyID(NUM_OF_BADGUYS);

ACTNUM::ACTNUM() {
	blood_total = 1024;
	blood_remain = 1024;
	mana_total = 1024;
	mana_remain = 1024;
}

// some globals
int frame = 0;
int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;
int dirCount = 0, dirState = 0;

// memorize status
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

void BadGuyGen(FnScene scene, int num_of_actor, char* actor_name, float* pos, float* fDir, float* uDir)
{
	FnCharacter cur_actor;
	ACTIONid cur_actor_idleID, cur_actor_curPoseID; // two actions
	badguyID[num_of_actor].actorID = scene.LoadCharacter(actor_name);

	cur_actor.ID(badguyID[num_of_actor].actorID);
	cur_actor.SetDirection(fDir, uDir);

	cur_actor.SetTerrainRoom(terrainRoomID, 10.0f);
	cur_actor.PutOnTerrain(pos);

	// Get two character actions pre-defined at Lyubu2
	cur_actor_idleID = cur_actor.GetBodyAction(NULL, "CombatIdle");

	// set the character to idle action
	cur_actor_curPoseID = cur_actor_idleID;
	cur_actor.SetCurrentAction(NULL, 0, cur_actor_curPoseID);
	cur_actor.Play(ONCE, 0.0f, FALSE, TRUE);
}

void BadGuyAction(int num_of_actor, char* act, int damage_num)
{
	FnCharacter cur_actor;
	ACTIONid cur_actor_idleID, cur_actor_curPoseID; // two actions
	cur_actor.ID(badguyID[num_of_actor].actorID);

	badguyID[num_of_actor].blood_remain = badguyID[num_of_actor].blood_remain - damage_num;

	if (badguyID[num_of_actor].blood_remain > 0)
		cur_actor_curPoseID = cur_actor.GetBodyAction(NULL, act);
	else
	{
		float fDir[3], uDir[3];
		fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
		uDir[0] = 1.0f; uDir[1] = 1.0f; uDir[2] = 0.0f;
		cur_actor.SetDirection(fDir, uDir);
		cur_actor_curPoseID = cur_actor.GetBodyAction(NULL, "Die");
	}
	cur_actor.SetCurrentAction(NULL, 0, cur_actor_curPoseID);
}

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
	NormalAttack1ID = actor.GetBodyAction(NULL, "NormalAttack1");
	NormalAttack2ID = actor.GetBodyAction(NULL, "NormalAttack2");
	HeavyAttack1ID = actor.GetBodyAction(NULL, "HeavyAttack1");

	// set the character to idle action
	curPoseID = idleID;
	actor.SetCurrentAction(NULL, 0, curPoseID);
	actor.Play(START, 0.0f, FALSE, TRUE);

	// translate the camera
	cID = scene.CreateObject(CAMERA);
	FnCamera camera;
	camera.ID(cID);
	camera.SetNearPlane(5.0f);
	camera.SetFarPlane(100000.0f);

	//hw3 : ¡§Donzo2¡¨ and ¡§Robber02 initialization
	float temp_pos[3];
	float temp_fDir[3];

	for (int i = 0; i < NUM_OF_BADGUYS; i++)
	{
		temp_pos[0] = pos[0] + 30 * (rand()%8);
		temp_pos[1] = pos[1] + 30 * (rand()%8);
		temp_pos[2] = pos[2] + 30 * (rand()%8);
		temp_fDir[0] = -1.0f; temp_fDir[1] = -1.0f; temp_fDir[2] = 1.0f;

		BadGuyGen(scene, i, "Robber02", temp_pos, temp_fDir, uDir);
	}

	// hw2 initial : set camera position
	actor.GetPosition(pos);
	pos[0] = pos[0] - SHORTDIST * fDir[0];
	pos[1] = pos[1] - SHORTDIST * fDir[1];
	pos[2] = pos[2] + HEIGHTOFFSET;

	fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
	uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;

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

	// set Hotkeys
	FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);  // escape for quiting the game
	FyDefineHotKey(FY_UP, Movement, FALSE);      // Up for moving forward
	FyDefineHotKey(FY_RIGHT, Movement, FALSE);   // Right for turning right
	FyDefineHotKey(FY_LEFT, Movement, FALSE);    // Left for turning left
	FyDefineHotKey(FY_DOWN, Movement, FALSE);
	FyDefineHotKey(FY_Q, Movement, FALSE);
	FyDefineHotKey(FY_W, Movement, FALSE);
	FyDefineHotKey(FY_E, Movement, FALSE);

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
	// play character pose
	FnCharacter actor;
	actor.ID(actorID);
	//actor.Play(LOOP, (float)skip, FALSE, TRUE);

	if (curPoseID == runID || curPoseID == idleID)
	{
		actor.Play(LOOP, (float)skip, FALSE, TRUE);
	}
	else if ( ! actor.Play(ONCE, (float)skip, FALSE, TRUE))
	{
		
		actor.SetCurrentAction(NULL, 0, idleID, 10.0f);
		actor.Play(LOOP, (float)skip, FALSE, TRUE);
	}

	FnCharacter cur_actor;
	for (int i = 0; i < NUM_OF_BADGUYS; i++)
	{
		cur_actor.ID(badguyID[i].actorID);

		if (curPoseID == runID || curPoseID == idleID)
		{
			cur_actor.Play(LOOP, (float)skip, FALSE, TRUE);
		}
		else if (!cur_actor.Play(ONCE, (float)skip, FALSE, TRUE) && badguyID[i].blood_remain > 0 )
		{

			ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, "CombatIdle");
			cur_actor.SetCurrentAction(NULL, 0, CombatIdleID, 10.0f);
			cur_actor.Play(LOOP, (float)skip, FALSE, TRUE);
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

	FnObject terrain;
	terrain.ID(tID);

	float dir[3], origin[3], actorfDir[3], actoruDir[3];// origin is the actor's position      
	actor.GetPosition(origin);
	actor.GetDirection(actorfDir, actoruDir);
	origin[2] = origin[2] + HEIGHTOFFSET; // need an offset to probe the hit
	dir[0] = actorfDir[0] / ACTORPROBE;
	dir[1] = actorfDir[1] / ACTORPROBE;
	dir[2] = -1.0f;

	//////////////////////////////////////////
	// Homework #01 part 1
	//////////////////////////////////////////

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
		actorfDir[0] = camerafDir[1];
		actorfDir[1] = -camerafDir[0];
		actor.SetDirection(actorfDir, actoruDir);
		if (terrain.HitTest(origin, dir) > 0)
		{
			//actor.MoveRight(CALLIBRATION, FALSE, FALSE, 0, FALSE); // for hw1
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
			//actor.MoveRight(CALLIBRATION, FALSE, FALSE, 0, FALSE); // for hw1
			actor.MoveForward(RUNSPEED / 2, FALSE, FALSE, 0, FALSE);
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
	sprintf(debug, "debug: %d %d %d", badguyID[0].blood_remain, badguyID[1].blood_remain, badguyID[2].blood_remain);

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
	//////////////////////////////////////////
	// Homework #01 part 2
	//////////////////////////////////////////
	// note : only hotkey input can trigger this function

	FnCharacter actor;
	actor.ID(actorID);


	// 2. use a global idle_count variable to memorize the action status.
	//    (to prevent the "slide" actor bug)

	if (value)
	{
		idle_count++;

		if (FyCheckHotKeyStatus(FY_Q) && dirCount % SMOOTHINESS == 0)
		{
			dirCount = 0 - dirState;
			dirState = 0;
			status = 1;
		}

		if (FyCheckHotKeyStatus(FY_W) && dirCount % SMOOTHINESS == 0)
			status = 2;
		if (FyCheckHotKeyStatus(FY_E) && dirCount % SMOOTHINESS == 0)
			status = 3;

		if (FyCheckHotKeyStatus(FY_UP) && dirCount % SMOOTHINESS == 0)
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
		status = 0;
	}

	// 3. the actor is idle when idle_count equal to zero.   

	float actorPos[3];
	actor.GetPosition(actorPos);

	float badguyPos[3], badguyfDir[3], badguyuDir[3];
	FnCharacter badguy;

	if (idle_count > 0)
	{

		if (status == 1)
		{
			curPoseID = NormalAttack1ID;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < 50)
				{
					BadGuyAction(i, "Damage1", 100);
				}
			}
		}

		else if (status == 2)
		{
			curPoseID = NormalAttack2ID;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < 70)
				{
					BadGuyAction(i, "Damage2", 200);
				}
			}
		}
		else if (status == 3)
		{
			curPoseID = HeavyAttack1ID;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < 70)
				{
					BadGuyAction(i, "Damage2", 400);
				}
			}
		}
		else
		{
			curPoseID = runID;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				BadGuyAction(i, "CombatIdle", 0);
			}
		}

		actor.SetCurrentAction(NULL, 0, curPoseID, 10.0f);
		//actor.Play(ONCE, 0.0f, FALSE, TRUE);
	}
	else
	{
		curPoseID = idleID;
		actor.SetCurrentAction(NULL, 0, curPoseID, 10.0f);
		//actor.Play(START, 0.0f, FALSE, TRUE);

		for (int i = 0; i < NUM_OF_BADGUYS; i++)
		{
			BadGuyAction(i, "CombatIdle", 0);
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
