/*==============================================================
  This is a demo program for Fly2 System

  Hello !

  (C)2004-2012 Chuan-Chang Wang, All Rights Reserved
  Created : 0303, 2004, C. Wang

  Last Updated : 0904, 2012, C.Wang
 ===============================================================*/
#include "FlyWin32.h"

int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

VIEWPORTid vID;
SCENEid sID;
OBJECTid nID, cID, lID;

SCENEid sID2;                // the 2D scene
OBJECTid spID0 = FAILED_ID;  // the sprite

void QuitGame(BYTE, BOOL4);
void GameAI(int);

void InitPivot(int, int);
void PivotCam(int, int);
void InitMove(int, int);
void MoveCam(int, int);
void InitZoom(int, int);
void ZoomCam(int, int);

/*------------------
  the main program
  C.Wang 0904, 2012
 -------------------*/
void FyMain(int argc, char **argv)
{
   // create a new world
   FyStartFlyWin32("Hello Fly2 !", 0, 0, 800, 600, FALSE);

   FySetModelPath("Data\\Models");
   FySetTexturePath("Data\\textures");
   FySetShaderPath("Data\\shaders");

   // create a viewport
   vID = FyCreateViewport(0, 0, 800, 600);
   FnViewport vp;
   vp.ID(vID);
   vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);

   // create a 3D scene & the 3D entities
   sID = FyCreateScene(1);

   FnScene scene;
   scene.ID(sID);
   nID = scene.CreateObject(OBJECT);
   cID = scene.CreateObject(CAMERA);
   lID = scene.CreateObject(LIGHT);

   // load the teapot model
   FnObject model;
   model.ID(nID);
   model.Load("teapot");

   // translate the camera
   FnCamera camera;
   camera.ID(cID);
   camera.Rotate(X_AXIS, 90.0f, LOCAL);
   camera.Translate(0.0f, 10.0f, 100.0f, LOCAL);

   // translate the light
   FnLight light;
   light.ID(lID);
   light.SetName("MainLight");
   light.Translate(-50.0f, -50.0f, 50.0f, GLOBAL);

   // create a 2D scene for sprite rendering which will be rendered on the top of 3D
   sID2 = FyCreateScene(1);
   scene.ID(sID2);
   scene.SetSpriteWorldSize(800, 600);         // 2D scene size in pixels

   FnSprite sp;

   spID0 = scene.CreateObject(SPRITE);
   sp.ID(spID0);
   sp.SetSize(200, 200);
   sp.SetImage("Lyubu_talk", 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
   sp.SetPosition(100, 100, 0);

   // set Hotkeys
   FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);

   // define some mouse functions
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

   // bind a timer, frame rate = 30 fps
   FyBindTimer(0, 30.0f, GameAI, TRUE);

   // invoke the system
   FyInvokeFly(TRUE);
}


/*-------------------------------------------------------------------------------
  timer callback function which will be invoked by Fly2 System every 1/30 second
  C.Wang 0308, 2004
 --------------------------------------------------------------------------------*/
void GameAI(int skip)
{
   // render the 3D scene
   FnViewport vp;
   vp.ID(vID);
   vp.Render3D(cID, TRUE, TRUE);

   // render the sprites
   vp.RenderSprites(sID2, FALSE, TRUE);  // no clear the background but clear the z buffer

   FySwapBuffers();
}


/*------------------
  quit the program
  C.Wang 0904, 2012
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
  C.Wang 0329, 2004
 ------------------------------------*/
void InitPivot(int x, int y)
{
   oldX = x;
   oldY = y;
}


/*------------------
  pivot the camera
  C.Wang 0329, 2004
 -------------------*/
void PivotCam(int x, int y)
{
   FnObject model;

   if (x != oldX) {
      model.ID(cID);
      model.Rotate(Z_AXIS, (float) (x - oldX), GLOBAL);
      oldX = x;
   }

   if (y != oldY) {
      model.ID(cID);
      model.Rotate(X_AXIS, (float) (y - oldY), GLOBAL);
      oldY = y;
   }
}


/*----------------------------------
  initialize the move of the camera
  C.Wang 0329, 2004
 -----------------------------------*/
void InitMove(int x, int y)
{
   oldXM = x;
   oldYM = y;
}


/*------------------
  move the camera
  C.Wang 0329, 2004
 -------------------*/
void MoveCam(int x, int y)
{
   if (x != oldXM) {
      FnObject model;

      model.ID(cID);
      model.Translate((float)(x - oldXM)*0.1f, 0.0f, 0.0f, LOCAL);
      oldXM = x;
   }
   if (y != oldYM) {
      FnObject model;

      model.ID(cID);
      model.Translate(0.0f, (float)(oldYM - y)*0.1f, 0.0f, LOCAL);
      oldYM = y;
   }
}


/*----------------------------------
  initialize the zoom of the camera
  C.Wang 0329, 2004
 -----------------------------------*/
void InitZoom(int x, int y)
{
   oldXMM = x;
   oldYMM = y;
}


/*------------------
  zoom the camera
  C.Wang 0329, 2004
 -------------------*/
void ZoomCam(int x, int y)
{
   if (x != oldXMM || y != oldYMM) {
      FnObject model;

      model.ID(cID);
      model.Translate(0.0f, 0.0f, (float)(x - oldXMM), LOCAL);
      oldXMM = x;
      oldYMM = y;
   }
}