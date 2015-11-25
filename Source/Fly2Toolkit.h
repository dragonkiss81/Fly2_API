/*========================================================================
  Fly2 Toolkit's Header

  (C)2012-2014 BlackSmith Technology, All Rights Reserved
  Created : 0807, 2012 by Kevin C. Wang

  Last Updated : 0520, 2014
 =========================================================================*/
#if !defined(_FLY2_TOOLKIT_I_H_INCLUDED_)
#define _FLY2_TOOLKIT_I_H_INCLUDED_


#define MAX_UI_SYSTEMS                64
#define MAX_COMMAND_SLOTS             256
#define MAX_STRING_CHARACTERS         512
#define MAX_GLOBALS                   2048
#define MAX_LOCALS                    512
#define MAX_SCRIPTS                   16
#define MAX_LOOP_STACKS               16
#define MAX_SCENE_BUTTONS             10
#define MAX_ACTIONS                   1024
#define MAX_MATERIAL_DATA             10
#define MAX_BLENDSHAPES               40

#define SYSTEM_TOOL                   0
#define MODEL_TOOL                    1
#define CHARACTER_TOOL                2
#define FX_TOOL                       3
#define SCENE_TOOL                    4
#define FACE_TOOL                     5

#define STRING_VARIABLE               0
#define FLOAT_VARIABLE                1
#define INTEGER_VARIABLE              2
#define BOOLEAN_VARIABLE              3

#define LOOP_TIMER                    0
#define RENDER_TIMER                  1
#define BLINK_TIMER                   2

#define BUTTON_WIDTH                  80
#define LONG_BUTTON_WIDTH             120
#define SHORT_BUTTON_WIDTH            30

#define KEYFRAME_POSITION             1
#define KEYFRAME_ROTATION             2
#define KEYFRAME_PLATE_SIZE           3
#define KEYFRAME_PLATE_COLOR          4
#define KEYFRAME_VISIBILITY           5


#define TOOL_MODE                     1
#define PLAY_MODE                     2

#define FT_MASK_NONE                  0      // Face Tool Mask Operation Mode: None
#define FT_MASK_ADD                   1      // Face Tool Mask Operation Mode: +
#define FT_MASK_SUB                   2      // Face Tool Mask Operation Mode: -


#define BLOOM_EFFECT                  0
#define HDR_EFFECT                    1


// index of gizmoIDs[], gizmoMIDs[] for Gizmo
//#define X_AXIS                      0      // compatible for Fly2 X axis
//#define Y_AXIS                      1      // compatible for Fly2 Y axis
//#define Z_AXIS                      2      // compatible for Fly2 Z axis
#define XY_PLANE                      3
#define YZ_PLANE                      4
#define ZX_PLANE                      5
#define MAXIMUM_PARTICLES             102400


struct TBGizmo
{
   BOOL4      isShow;               // Show/Hide Flag
   OBJECTid   baseID;               // Base Object ID of this Gizmo
   BOOL4      beDrag;               // Flag for Dragging this Gizmo
   float      curSlop[2];           // Current Choosed Gizmo Line Slop ({x,y} in Screen Coord.)
   int        iCurGizmo;            // Current Choosed Gizmo Element index
   int        numGizmoObj;          // gizmoIDs[] num
   OBJECTid   gizmoIDs[6];          // Objects ID of this Gizmo used
                                    // -- For Translate: X-axis, Y-axis, Z-axis, XY-plane, YZ-plane, ZX-plane
                                    // -- For Rotate:    X-axis, Y-axis, Z-axis
   MATERIALid gizmoMIDs[12];        // Material ID of this Gizmo used responding to gizmoIDs[]
                                    // -- 2 Materials per Gizmo Object
   float      colorX[3];            // X-axis Color
   float      colorY[3];            // Y-axis Color
   float      colorZ[3];            // Z-axis Color
   float      colorChoose[3];       // Choosed Color
   int        idxGizmoObjV[6];      // Start Index of gizmoObjVex[] of one Gizmo Object
   int        numGizmoObjV[6];      // Line Number of one Gizmo Object
   int        typeGizmoObjV[6];     // Line Type of one Gizmo Object
   float      gizmoObjVex[300];     // verteices of Gizmo Object lines (3 float per 1 vertex)
   float      radiusBig;            // Big   Radius
   float      radiusSmall;          // Small Radius
};


struct TBMaterial
{
   float ambient[3];
   float diffuse[3];
   float specular[3];
   float emissive[3];
   float power;
   char shaderPath[256];
   char shaderFile[256];
   char effectName[256];

   char texturePath[256];
   int  numMatTexture;
   char texture0[256];
   char texture1[256];
   char texture2[256];
   char texture3[256];
   char texture4[256];
   char texture5[256];

   int numMatData;
   int dataID[MAX_MATERIAL_DATA];
   int dataLen[MAX_MATERIAL_DATA];
   float data[MAX_MATERIAL_DATA*3];
};


// object data structure used in cwViewer
class TBObject : public FnObject
{
private:
   CHARACTERid usedBy;
   int type;
   int boneID;
   BOOL4 beGeometry;

public:
   TBObject() { usedBy = FAILED_ID; boneID = FY_NONE; type = FY_NONE; beGeometry = TRUE; };
  ~TBObject() {};

   void UsedBy(CHARACTERid aID) { usedBy = aID; };
   CHARACTERid UsedBy() { return usedBy; };
   void Data(int t, int b) { type = t; boneID = b; };
   int Type() { return type; }
   int BoneID() { return boneID; };
   void BeGeometry(BOOL4 beGeo) { beGeometry = beGeo; };
   BOOL4 IsGeometry() { return beGeometry; };
};


// character data structure used in cwViewer
class TBCharacter : public FnCharacter
{
private:
   int curFacialExpressionKey;

public:
   TBCharacter() { curFacialExpressionKey = FY_NONE; };
  ~TBCharacter() {};

   int CurrentFacialExpressionKey() { return curFacialExpressionKey; };
   void CurrentFacialExpressionKey(int key) { curFacialExpressionKey = key; };
};


// Game FX data used in cwViewer
class TBGameFXSystem : public FnGameFXSystem
{
private:

public:
   TBGameFXSystem() {};
  ~TBGameFXSystem() {};
};


// scene data structure used in cwViewer
class TBScene : public FnScene
{
private:
   OBJECTid camRefID;
   OBJECTid camID;
   OBJECTid mainLightID;
   OBJECTid auxLightID;

   UIid sceneButtonID;

   MATERIALid curMatID;

   float curFrame;

   int iCurObject;
   int numObject;
   int maxObject;
   TBObject **objList;

   int iCurActor;
   int numActor;
   int maxActor;
   TBCharacter **actorList;

   int iCurFXS;
   int numFXS;
   int maxFXS;
   TBGameFXSystem **fxsList;

public:
   TBScene();
  ~TBScene();

   inline int GetCharacterNumber() { return numActor; };
   inline int GetObjectNumber() { return numObject; };
   void AssignCamera(OBJECTid rID, OBJECTid cID);
   void AssignLights(OBJECTid mainID, OBJECTid auxID);
   void GetLights(OBJECTid *mainID, OBJECTid *auxID);
   void InsertObject(OBJECTid oID, CHARACTERid aID, int type, int id, BOOL4 beGeo = TRUE);
   CHARACTERid RemoveObject(OBJECTid oID, BOOL4 beD, BOOL4 beProcess = TRUE);
   TBObject *GetTBObject(OBJECTid oID);
   TBObject *GetCurrentTBObject();
   void SetCurrentObject(OBJECTid oID);
   OBJECTid GetCurrentObject();
   OBJECTid FindObject(char *name);
   OBJECTid GetObjectID(int iOne);
   MATERIALid GetCurrentMaterial() { return curMatID; };
   void SetCurrentMaterial(MATERIALid mID) { curMatID = mID; };
   BOOL4 NextCurrentObject();
   BOOL4 PreviousCurrentObject();
   void InsertCharacter(CHARACTERid aID);
   void RemoveCharacter(CHARACTERid oID, BOOL4 beD);
   void SetCurrentCharacter(CHARACTERid aID);
   TBCharacter *GetCurrentTBCharacter();
   CHARACTERid GetCurrentCharacter();
   CHARACTERid GetCharacterByName(char *name);
   int GetEffectCharacterNumber();
   char *GetCharacterName(int id);

   void New();
   void ResetPlay(BOOL4 beBase);
   float Play(float skip, BOOL4 beBase, DWORD mode);

   int GetGameFXSystemNumber() { return numFXS; };
   float PauseFXPlaying();

   int CheckEffectGameFXSystemNumber();
   TBGameFXSystem *GetTBGameFXSystem(int iOne);
   TBGameFXSystem *GetTBGameFXSystem(char *name);
   TBGameFXSystem *GetCurrentTBGameFXSystem();
   void SetCurrentGameFXSystem(GAMEFX_SYSTEMid gxID);
   void InsertGameFXSystem(GAMEFX_SYSTEMid gxID);
   void RemoveGameFXSystem(GAMEFX_SYSTEMid gxID, BOOL4 beD = TRUE);

   void FindCenter(float *pos, float *radius);

   float GetCurrentFrame();
   void SetCurrentFrame(float f, BOOL4 beBased);

   int GetCameraNumber();
   void GetCameras(OBJECTid *camList, int bufSize);

   OBJECTid CheckCurObjectInCurCharacter();
};


struct TBFaceMaskForQ3D
{
   int numMaskFaceVert;
   int *maskVert;
   int numMaskFace;
   int *maskFace;
   int numCornerVert;
   int *cornerVert;
   int numBlendFace;
   int *blendFace;
};


#endif _FLY2_TOOLKIT_I_H_INCLUDED_
