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
#include "game_design.h"
using namespace std;

vector<ACTNUM> badguyID(NUM_OF_BADGUYS);
vector<ACTNUM> BossID(NUM_OF_BOSS);
ACTNUM LyubuID;
ProduceBadguys generator[NUM_OF_GENERATOR];
float genSpot[NUM_OF_GENERATOR][2];

/* ------------- Original Code ----------------*/

VIEWPORTid vID;                 // the major viewport
SCENEid sID;                    // the 3D scene
OBJECTid cID, tID;              // the main camera and the terrain for terrain following
CHARACTERid actorID;            // the major character
ACTIONid idleID, runID, curPoseID;
ACTIONid NormalAttack1ID, NormalAttack2ID, HeavyAttack1ID;
ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;
// Globals
int frame = 0;
int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

/*---------------------------------------------*/

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

	//hw3 : !§Donzo2!‥ and !§Robber02 initialization
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
		temp_pos[0] = pos[0] + 30 * (rand()%8);
		temp_pos[1] = pos[1] + 30 * (rand()%8);
		temp_pos[2] = pos[2] + 30 * (rand()%8);
		SetValues(temp_fDir, -1.0f, -1.0f, 1.0f);

		ActorGen(scene, terrainRoomID, BossID[i], "Donzo2", "CombatIdle", temp_pos, temp_fDir, uDir);
	}
	

	// hw2 initial : set camera position
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
	FnCharacter cur_actor;
	FnObject terrain;
	terrain.ID(tID);
	FnScene scene;
	scene.ID(sID);
	LyubuID.actorID = actorID;

	actor.ID(actorID);

	if (curPoseID == runID || curPoseID == idleID)
		actor.Play(LOOP, (float)skip, FALSE, TRUE);
	else if ( actor.Play(ONCE, (float)skip, FALSE, TRUE) == 0)
		actor.SetCurrentAction(NULL, 0, idleID, 10.0f);


	for (int i = 0; i < NUM_OF_BADGUYS; i++)
	{
		cur_actor.ID(badguyID[i].actorID);

		if (badguyID[i].blood_remain > 0)
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
					ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, "NormalAttack2");
					cur_actor.SetCurrentAction(NULL, 0, CombatIdleID);
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
					ACTIONid CombatIdleID = cur_actor.GetBodyAction(NULL, "HeavyAttack");
					cur_actor.SetCurrentAction(NULL, 0, CombatIdleID);
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
			generator[i].produce(origin, actoruDir, scene, i, NUM_OF_GENERATOR);
	}

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

	if ((FyCheckHotKeyStatus(FY_A) || FyCheckHotKeyStatus(FY_D)) && dirCount % SMOOTHINESS == 0)
	{
		int tempHeight = pos[2];
		pos[2] = actorPos[2];
		int theDistance = FyDistance(pos, actorPos);

		pos[0] = actorPos[0] - theDistance * actorfDir[0];
		pos[1] = actorPos[1] - theDistance * actorfDir[1];
		pos[2] = tempHeight;

		fDir[0] = actorfDir[0];
		fDir[1] = actorfDir[1];

		uDir[0] = fDir[0] / abs(fDir[0] * fDir[1]);
		uDir[1] = fDir[1] / abs(fDir[0] * fDir[1]);
		uDir[2] = -abs(uDir[0] * fDir[0] + uDir[1] * fDir[1]) / fDir[2];
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
	sprintf(debug, "debug: %d %d %d", LyubuID.level, LyubuID.exp_cur, LyubuID.blood_remain);

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
	FnCharacter badguy;

	float actorPos[3], actorfDir[3], actoruDir[3];
	float badguyPos[3], badguyfDir[3], badguyuDir[3];

	actor.ID(actorID);
	actor.GetPosition(actorPos);
	actor.GetDirection(actorfDir, actoruDir);

	// 2. use a global idle_count variable to memorize the action status.
	//    (to prevent the "slide" actor bug)

	if (value)
	{
		idle_count++;

		if (FyCheckHotKeyStatus(FY_Q) && dirCount % SMOOTHINESS == 0)
		{
			actorPos[0] += actorfDir[0] * HATKOFFSET;
			actorPos[1] += actorfDir[1] * HATKOFFSET;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < HATKRANGE && badguyID[i].blood_remain > 0)
				{
					if (!ActAction(badguyID[i], "Damage1", HATKDAMAGE))
					{
						LyubuID.exp_cur += 50;
						if (LyubuID.exp_cur >= LyubuID.exp_total){
							LyubuID.level++;
							LyubuID.exp_cur -= LyubuID.exp_total;
						}
					}
				}
			}

			for (int i = 0; i < NUM_OF_BOSS; i++)
			{
				badguy.ID(BossID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < HATKRANGE && BossID[i].blood_remain > 0)
				{
					ActAction(BossID[i], "Donzo", HATKDAMAGE);
				}
			}

		}

		if (FyCheckHotKeyStatus(FY_W) && dirCount % SMOOTHINESS == 0)
		{
			actorPos[0] += actorfDir[0] * NATK1OFFSET;
			actorPos[1] += actorfDir[1] * NATK1OFFSET;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < NATK1RANGE && badguyID[i].blood_remain > 0)
				{
					if (!ActAction(badguyID[i], "Damage2", NATK1DAMAGE))
					{
						LyubuID.exp_cur += 50;
						if (LyubuID.exp_cur >= LyubuID.exp_total){
							LyubuID.level++;
							LyubuID.exp_cur -= LyubuID.exp_total;
						}
					}
				}
			}

			for (int i = 0; i < NUM_OF_BOSS; i++)
			{
				badguy.ID(BossID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < NATK1RANGE && BossID[i].blood_remain > 0)
				{
					ActAction(BossID[i], "Donzo", NATK1DAMAGE);
				}
			}

		}

		if (FyCheckHotKeyStatus(FY_E) && dirCount % SMOOTHINESS == 0)
		{
			actorPos[0] += actorfDir[0] * NATK2OFFSET;
			actorPos[1] += actorfDir[1] * NATK2OFFSET;

			for (int i = 0; i < NUM_OF_BADGUYS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < NATK2RANGE && badguyID[i].blood_remain > 0)
				{
					if (!ActAction(badguyID[i], "Damage2", NATK2DAMAGE))
					{
						LyubuID.exp_cur += 50;
						if (LyubuID.exp_cur >= LyubuID.exp_total){
							LyubuID.level++;
							LyubuID.exp_cur -= LyubuID.exp_total;
						}
					}
				}
			}

			for (int i = 0; i < NUM_OF_BOSS; i++)
			{
				badguy.ID(badguyID[i].actorID);
				badguy.GetPosition(badguyPos);

				if (FyDistance(badguyPos, actorPos) < NATK2RANGE && BossID[i].blood_remain > 0)
				{
					ActAction(BossID[i], "Donzo", NATK2DAMAGE);
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

		if (FyCheckHotKeyStatus(FY_Q) && dirCount % SMOOTHINESS == 0)
		{
			curPoseID = NormalAttack1ID;
		}
		else if (FyCheckHotKeyStatus(FY_W) && dirCount % SMOOTHINESS == 0)
		{
			curPoseID = NormalAttack2ID;
		}
		else if (FyCheckHotKeyStatus(FY_E) && dirCount % SMOOTHINESS == 0)
		{
			curPoseID = HeavyAttack1ID;
		}
		else
		{
			curPoseID = runID;
		}
		
		actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
	}
	else if (curPoseID == runID)
	{
		curPoseID = idleID;
		actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
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


void ProduceBadguys::produce(float* pos, float* uDir, FnScene scene, int generatorNUM, int totalgenNUM)
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
			ActorGen(scene, terrainRoomID, badguyID[i], "Robber02", "CombatIdle", temp_pos, temp_fDir, uDir);
			break;
		}
	}
}