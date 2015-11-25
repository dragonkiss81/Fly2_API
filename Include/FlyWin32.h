/*==============================================================
  Fly 2 Win32 Platform - Header

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0327, 2012 by Chuan-Chang Wang

  Last Updated : 0206, 2015
 ===============================================================*/
#if !defined(_FLY2_WIN32_H_INCLUDED_)
#define _FLY2_WIN32_H_INCLUDED_


// include Fly 2 headers
#include "Fly.h"


// mouse buttons
#define LEFT_MOUSE             0x00
#define RIGHT_MOUSE            0x01
#define MIDDLE_MOUSE           0x02


typedef void (*MOUSECALLBACKptr)(int, int);

// functions
extern void FyMain(int argc, char *argv[]);
extern void FyWin32GetDesktopSize(int *w, int *h);
extern BOOL4 FyStartFlyWin32(char *caption, int ox, int oy, int w, int h, BOOL4 beFullScreen, 
                             BOOL4 beWindowFrame = TRUE, int aaLevel = 0,
                             int numMB = 0, float *blockLen = NULL, BOOL4 beResize = FALSE,
                             BOOL4 beUseThread = TRUE, float tmpM = 400.0f, float tmpMB = 200.0f);
extern void FyResizeFlyWin32World(int w, int h, BOOL4 beFillScreen, BOOL4 beWin32Resize = TRUE);
extern void FyQuitFlyWin32();
extern void FyQueryWindowFrameSize(int *w, int *h);
extern void FyInvokeFly(BOOL4 beTimer = TRUE);
extern void FyBindWindowResizeFunction(void (*fun0)(int, int, BOOL4));
extern void FyBindQuitFunction(void (*callBack)());
extern void FyDefineHotKey(BYTE keyCode, void (*callBack)(BYTE, BOOL4), BOOL4 beR = FALSE);
extern BOOL4 FyCheckHotKeyStatus(BYTE keyCode);
extern void FyBindInputFunction(void (*callBack)(WORD));
extern void *FyGetInputFunction();
extern void FyBindMouseFunction(DWORD mButton, void (*callBackPressed)(int, int), void (*callBackDrag)(int, int),
                                void (*callBackReleased)(int, int), void (*callBackDoubleClick)(int, int));
extern void FyBindMouseMoveFunction(void (*callBack)(int, int));
extern MOUSECALLBACKptr FyGetMouseMoveFunction();
extern void FyBindMouseWheelFunction(void (*callBack)(int, int, float));
extern void FyBindDropFileFunction(void (*callBack)(char *));
extern void (*FYGetWin32Renderer())();
extern void FyBindBatchWin32Renderer(void (*callBack)(), BOOL4 beUpdate);
extern void FyStartWin32Timer(BOOL4 beStart, BOOL4 beBackUp = TRUE);
extern BOOL4 FyGetWin32TimerStatus();
extern void FySetTimerPause(BOOL4 beON);

extern int FySubmitBatchJob(LPTHREAD_START_ROUTINE pFunc, LPVOID pData);
extern BOOL4 FyTerminateBatchJob(int id);
extern HWND FyGetWindowsHandle();

#endif //_FLY2_WIN32_H_INCLUDED_