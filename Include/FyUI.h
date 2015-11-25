/*========================================================================
  The Fly2 User Interface Header - Using FnSprite

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0804, 2012 by Kevin C. Wang

  Last Updated : 0320, 2015
 =========================================================================*/
#if !defined(_THE_FLY2_UI_H_INCLUDED_)
#define _THE_FLY2_UI_H_INCLUDED_

#include "FlyWin32.h"

#define NO_CHANGE                         0

// UI types
#define PLATE_UI                          1
#define BUTTON_UI                         2
#define RADIO_BUTTON_UI                   3
#define TYPEIN_UI                         4
#define TAB_UI                            5
#define COMBO_UI                          6
#define MESSAGE_BOX_UI                    7

// radio button types
#define RADIO_BUTTON                      0
#define CHECK_BOX                         1

// border types
#define NO_BORDER                         1
#define SINGLE_BORDER                     2
#define SHADOW_BORDER                     3

#define NORMAL_MESSAGE                    1
#define RED_MESSAGE                       2


typedef unsigned int UISYSTEMid;
typedef unsigned int UIid;


class FnUISystem : public FnFunction
{
public:
   FnUISystem(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnUISystem() {};

   BOOL4 Load(char *fileName);
   void Render(BOOL4 beClear);
   void SetBackgroundColor(float r, float g, float b, float a = 1.0f);
   UIid CreatePlate(char *name, int ox, int oy, int w, int h, int layer, float r, float g, float b, float a = 1.0f);
   UIid CreateButton(char *name, int ox, int oy, int w, int h, int layer, float r, float g, float b, float a = 1.0f);
   UIid CreateRadioButton(char *name, int ox, int oy, int w, int h, int layer, float r, float g, float b, float a = 1.0f, int groupID = FY_NONE, DWORD ct = 0, float cr = 0.0f, float cg = 0.7f, float cb = 0.0f);
   UIid CreateTypein(char *name, int ox, int oy, int w, int h, int layer, float r, float g, float b, float a = 1.0f);
   UIid CreateTab(char *name, int ox, int oy, int w, int h, int layer, float r, float g, float b, float a = 1.0f, BOOL4 beUp = FALSE, BOOL4 beDown = FALSE);
   UIid CreateCombo(char *name, int ox, int oy, int w, int h, int layer, float r, float g, float b, float a = 1.0f, int offset = 50, int ww = 100, int len = 10, BOOL4 beS = TRUE, int maxMessage = 64);

   void PopUp(BOOL4 beP);
   void BePopUp(BOOL4 bePopup);

   BOOL4 MousePress(int x, int y, UIid * = NULL);
   BOOL4 MouseRelease(int x, int y, UIid * = NULL);

   VIEWPORTid GetViewport();
   void MoveSize(int ox, int oy, int w, int h);
};


class FnUiPlate : public FnFunction
{
public:
   FnUiPlate(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnUiPlate() {};

   void SetCaption(DWORD type, int ox, int oy, char *caption, float r, float g, float b, float a = 1.0f);
   void ReCaption(char *caption);
   void SetBorder(DWORD type, float r, float g, float b, float a = 1.0f);

   void CreateUIMessageSlots(int num);
   BOOL4 SetUIMessage(int id, DWORD type, int ox, int oy, char *value, float r, float g, float b);

   void SetParent(UIid pID);
   void SetLayer(int z);
   void Lines(int *vi, int nP, float r, float g, float b);

   void Enable(BOOL4 beD);
   BOOL4 IsAble();
   void Show(BOOL4 beShow);

   void UseChineseFontSet(BOOL4 beU);
   DWORD GetType();

   OBJECTid GetObjectID();

   void MoveSize(int ox, int oy, int w, int h);
   void Move(int ox, int oy);
};


class FnUiButton : public FnUiPlate
{
public:
   FnUiButton(DWORD m = FAILED_ID) : FnUiPlate(m) {};
  ~FnUiButton() {};

   void BindCallback(void (*fun)(UIid, BOOL4));
};


class FnUiTab : public FnUiPlate
{
public:
   FnUiTab(DWORD m = FAILED_ID) : FnUiPlate(m) {};
  ~FnUiTab() {};

   void Open(BOOL4 beOpen);
   void SetCaption(char *caption, float r, float g, float b, float a = 1.0f);
   void NextTab(UIid);

   void BindCallback(void (*fun)(UIid, BOOL4));
};



class FnUiRadioButton : public FnUiButton
{
public:
   FnUiRadioButton(DWORD m = FAILED_ID) : FnUiButton(m) {};
  ~FnUiRadioButton() {};

   void SetGroup(int id);
   void ChangeState(BOOL4 beP);
   BOOL4 GetState();
};


class FnUiTypein : public FnUiPlate
{
public:
   FnUiTypein(DWORD m = FAILED_ID) : FnUiPlate(m) {};
  ~FnUiTypein() {};

   void SetTypein(DWORD type, int ox, int oy, int length);
   void SetTypeinColor(float r, float g, float b, float a  = 1.0f);
   void SetTypeinFontColor(float r, float g, float b, float a  = 1.0f);

   void PasteTypeinValue(char *value);
   void GetTypeinValue(char *value);
   void BindCallbacks(void (*fun)(UIid, char *), void (*funT)(UIid, char *) = NULL);
};


class FnUiCombo : public FnUiPlate
{
public:
   FnUiCombo(DWORD m = FAILED_ID) : FnUiPlate(m) {};
  ~FnUiCombo() {};

   void PasteComboValue(char *value);
   void GetComboValue(char *value);
   void InsertMessage(int id, char *value);
   void ClearMessages();
   void BindCallbacks(void (*funT)(UIid, char *), void (*funMB)(UIid, char *), void (*funT2)(UIid, char *) = NULL);
   void PopupMessageBox(BOOL4 beO);
   int GetMessageNumber();
   char *GetMessageString(int iOne);
   void SetMessageFlag(char *value, int flag);
   void SetMessageFlag(int id, int flag);
};


extern UISYSTEMid FyBeginUISystem(int ox, int oy, int w, int h);
extern void FyEndUISystem(UISYSTEMid uID);
extern void FyRenderAllUISystem(UISYSTEMid *uID, int numUI);
extern BOOL4 FyCheckAllLeftMousePress(UISYSTEMid *uID, int numUI, int x, int y, UIid *uiID = NULL, int *inX = NULL, int *inY = NULL);
extern BOOL4 FyCheckAllLeftMouseDoubleClick(UISYSTEMid *uID, int numUI, int x, int y, UIid *uiID = NULL);
extern BOOL4 FyCheckAllLeftMouseDrag(UISYSTEMid *uID, int numUI, int x, int y, UIid *uiID = NULL, int *inX = NULL, int *inY = NULL);
extern BOOL4 FyCheckAllLeftMouseRelease(UISYSTEMid *uID, int numUI, int x, int y, UIid *uiID = NULL);
extern void FyStandardPopupQuit(UIid, BOOL4);
extern BOOL4 FyFindFile(char *d, char *f, BOOL4 beNotClose);
extern void FyBackupMouseMoveFunctionForUI(void (*fun0)(int, int));


#endif _THE_FLY2_UI_H_INCLUDED_