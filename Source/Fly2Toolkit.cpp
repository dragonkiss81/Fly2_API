/*==========================================================
  Fly2 Toolkit Build 10

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0807, 2012 by Kevin C. Wang

  Last Updated : 0908, 2015
 ===========================================================*/
#include "FlyWin32.h"
#include "FyUI.h"
#include "FyMemory.h"
#include "FyCommand.h"
#include "FyJob.h"
#include "FyUtil.h"

#ifdef FLY2_RENDER
#include "FyRenderer.h"
#endif

#include <commdlg.h>
#include <commctrl.h>
#include <math.h>

#include "Fly2Toolkit.h"

// global environments
int tbBuildNumber = 9;
DWORD tbControlMode = TOOL_MODE;
DWORD tbTool = SYSTEM_TOOL;
BOOL4 tbBeZUp = TRUE;
DWORD tbKeyframeType = KEYFRAME_POSITION;
int tbAALevel = 1;

int tbWorldWidth = 1200;
int tbWorldHeight = 840;
int tbMinWorldWidth = 900;
int tbMinWorldHeight = 680;

int tbMinWorkSpaceWidth = 640;
int tbMinWorkSpaceHeight = 480;

int tbTempTopWidth = 1024, tbTempTopHeight = 768;
int tbWindowFrameInWidth = 6, tbWindowFrameInHeight = 54;

#ifdef FLY2_DX9
char *tbCaption = "Fly2 Toolkit (HLSL9) (C)2012-2015, BlackSmith Technology, All Rights Reserved, Build";
#else
char *tbCaption = "Fly2 Toolkit (C)2012-2015, BlackSmith Technology, All Rights Reserved, Build";
#endif

int tbRenderGroup = 12;
BOOL4 tbBeUIPressed = FALSE;
BOOL4 tbBeHitTest = FALSE;
BOOL4 tbShowWorkSpaceCaption = TRUE;
BOOL4 tbBePlay = FALSE;
BOOL4 tbBePause = FALSE;
BOOL4 tbBePlayFacialExpression = FALSE;
float tbPlaySpeed = 1.0f;
float tbCurFrame = 0.0f;
BOOL4 tbBeBoundingBoxOn = TRUE;
BOOL4 tbShowAux = TRUE;
BOOL4 tbBeEditCam = TRUE;
int tbTestPoseStart = 0;
int tbTestPoseEnd = 0;
char tbTestMotionName[256];
float tbTestPoseFrame = 0.0f;
char tbCurrentBodyPoseName[256];
BOOL4 tbBePlayPose = FALSE;
BOOL4 tbBeBase = TRUE;
BOOL4 tbBeRestoreCurrentDir = TRUE;
int tbRenderCamera = PERSPECTIVE;
float tbOrthScaleFactor = 1.0f;
float tbOrthScaleRate = 1.03f;
float tbMasterOpacity = 1.0f;
float tbSlaveOpacity = 0.3f;
float tbGaussianWidth = 2.0f;
float tbFilterThreshold = 0.12f;

int tbShowLabelStart = 0;
int tbShowLabelEnd = 0;

DWORD tbPlayMode = LOOP;

BOOL4 tbBeSmallShuFa = FALSE;
BOOL4 tbBeTinyShuFa = FALSE;
float tbMainCamDist = 400.0f;
float tbMainCamUp = 25.0f;
int tbTris = 0;
int tbVs = 0;

int tbRenderedFrame = 0;
float tbFPS = 0.0f;
BOOL4 tbShowFPSFrame = FALSE;

int tbDefFXLife = 500;
int tbCurMaxParticles = 512;
float tbDefParticleSize = 3.0f;
float tbCurMaxForcemagnitude = 32.0f;

int tbCurFacialExpressionFrame = FY_NONE;

// Fly2 objects
SCENEid tbMainSceneID = FAILED_ID;
SCENEid tbAuxSceneID = FAILED_ID;
SCENEid tbAux2SceneID = FAILED_ID;
SCENEid tbOrthoSceneID = FAILED_ID;
SCENEid tbSquareSceneID = FAILED_ID;
OBJECTid tbMainCamID = FAILED_ID;
OBJECTid tbMainCamRefID = FAILED_ID;
OBJECTid tbMainLightID = FAILED_ID;
OBJECTid tbAuxLightID = FAILED_ID;
OBJECTid tbAuxCamID = FAILED_ID;
OBJECTid tbAux2CamID = FAILED_ID;
OBJECTid tbOrthoCamID = FAILED_ID;
OBJECTid tbAxisID = FAILED_ID;
OBJECTid tbGizmoMID = FAILED_ID;
OBJECTid tbGizmoSID = FAILED_ID;

OBJECTid tbRenderCamID = FAILED_ID;

OBJECTid tbExclusiveSaveList[64];
int tbNumExclusiveSave = 0;

VIEWPORTid tbMiniPortID = FAILED_ID;
SCENEid tbMiniSceneID = FAILED_ID;
OBJECTid tbMiniAxisID = FAILED_ID;
OBJECTid tbMiniCamID = FAILED_ID;
FnViewport tbMiniPost;
float tbMiniCamDist = 50.0f;

float tbMoveAxis[3];

OBJECTid tbTestPlaneID = FAILED_ID;
OBJECTid tbOrthoPlaneID = FAILED_ID;

FnScene tbScene, tbAuxScene, tbAux2Scene, tbOrthoScene;
FnCamera tbCamera, tbAuxCamera, tbAux2Camera, tbOrthoCamera;
FnLight tbMainLight;
FnLight tbAuxLight;
FnObject tbCameraRef;

TEXTid tbSmallEFontID = FAILED_ID;
TEXTid tbTinyEFontID = FAILED_ID;
FnText tbSmallEFont;
FnText tbTinyEFont;

int tbOldX, tbOldY;  // store mouse coordinates
float tbMoveSpeed = 0.2f, tbZoomSpeed = 1.0f, tbPivotSpeed = 1.0f;

float tbTestPlaneSize = 100.0f;

char tbWorkDir[256];
char tbHomeDir[256];

float tbTexAspect = 1.0f;

// face tool
char tbFaceModelName[256];
OBJECTid tbCurrentFaceModelID = FAILED_ID;
float tbFaceCamDist = 40.0f;
OBJECTid tbFaceOrthoID = FAILED_ID;
OBJECTid tbFaceOrthoEdgeID = FAILED_ID;
TBFaceMaskForQ3D *tbCurrentFaceMask = NULL;
OBJECTid tbCurrentFaceMaskID = FAILED_ID;
int tbMatSlotUsed = FY_NONE;

BOOL4 tbMaskOpMode = FT_MASK_NONE;
BOOL4 tbBeFTPressed = FALSE;
int tbFTStartX = 0;
int tbFTStartY = 0;
int tbFTEndX = 0;
int tbFTEndY = 0;
OBJECTid tbSquareAreaID = FAILED_ID;

// scene table
TBScene *tbCurScene;
int tbNumScene = 0;
int tbMaxScene = 0;
TBScene **tbSceneList = NULL;


// UIs
int tbNumUISys;
UISYSTEMid tbAllUISysID[MAX_UI_SYSTEMS];


// menu bar UIs
UISYSTEMid tbMenuBarID = FAILED_ID;
FnUISystem tbMenuBarUI;
int tbMenuBarOX = 0, tbMenuBarOY = 0, tbMenuBarWidth = 1200, tbMenuBarHeight = 30;

int tbOneTypeinHeight = 23;

UIid tbMenuBarPlateID = FAILED_ID;
UIid tbSystemButtonID = FAILED_ID;
UIid tbScriptSystemButtonID = FAILED_ID;
UIid tbModelSystemButtonID = FAILED_ID;
UIid tbCharacterSystemButtonID = FAILED_ID;
UIid tbFXSystemButtonID = FAILED_ID;
UIid tbBuildSceneButtonID = FAILED_ID;
UIid tbGamePlayerButtonID = FAILED_ID;
UIid tbEnvironmentSystemButtonID = FAILED_ID;
UIid tbPostProcessingSystemButtonID = FAILED_ID;
UIid tbFaceToolButtonID = FAILED_ID;
UIid tbAuxButtonID = FAILED_ID;
UIid tbOrthoButtonID = FAILED_ID;
UIid tbExitButtonID = FAILED_ID;

// main menu UIs
UISYSTEMid tbMenuID = FAILED_ID;
FnUISystem tbMenuUI;
int tbMenuOX = 0, tbMenuOY = 640, tbMenuWidth = 1200, tbMenuHeight = 120;

// side menu UIs
UISYSTEMid tbSideMenuID = FAILED_ID;
FnUISystem tbSideMenuUI;
int tbSideMenuOX = 0, tbSideMenuOY = 40, tbSideMenuWidth = 290, tbSideMenuHeight = 600;

UIid tbSideMenuPlateID = FAILED_ID;
UIid tbSideMenuSystemPlateID = FAILED_ID;
UIid tbSideMenuScriptPlateID = FAILED_ID;
UIid tbSideMenuModelPlateID = FAILED_ID;
UIid tbSideMenuCharacterPlateID = FAILED_ID;
UIid tbSideMenuFXPlateID = FAILED_ID;
UIid tbSideMenuEnvironmentPlateID = FAILED_ID;
UIid tbSideMenuPostProcessingPlateID = FAILED_ID;
UIid tbSideMenuFaceToolPlateID = FAILED_ID;

UIid tbNewButtonID = FAILED_ID;
UIid tbScriptButtonID = FAILED_ID;
UIid tbModelButtonID = FAILED_ID;
UIid tbCharacterButtonID = FAILED_ID;
UIid tbFXButtonID = FAILED_ID;
UIid tbEnvironmentButtonID = FAILED_ID;
UIid tbPostProcessingButtonID = FAILED_ID;
UIid tbFaceButtonID = FAILED_ID;

// script loader UIs
UIid tbLoadScriptButtonID = FAILED_ID;

// Model / Transform UI configuration
int  tbTransformHeight = 81;                 // Transform Tab Height
int  tbNumTransformUI  = 8;                  // tbTransformUIs[] size

// GameFX UI configuration
int tbForceTypeTextWidth         = 50;      // Force Type Text Width
int tbForceTypeWidth             = 270;     // Force Type Width
int tbForceUsedByTextWidth       = 50;      // Force UsedBy Text Width
int tbForceUsedByWidth           = 270;     // Force UsedBy Width
int tbForceMinDistTextWidth      = 80;      // Force Minimum Distance Text Width
int tbForceMinDistWidth          = 147;     // Force Minimum Distance Width
int tbAudioTabHeight             = 95;      // Audio Tab Height
int tbAudioUIBound               = 3;       // Audio UI Bound
int tbAudioTextWidth             = 40;      // Audio Name Text Width
int tbAudioNameWidth             = 200;     // Audio Name Width
int tbAudioButtonWidth           = 50;      // Audio Load Button Width
int tbAudioStartTextWidth        = 75;      // Audio Start Frame Text Width
int tbAudioStartWidth            = 130;     // Audio Start Frame Width
int tbSwordTabHeight             = 300;     // Sword Tab Height
int tbSwordUIBound               = 3;       // Sword UI Bound
int tbSwordEnableWidth           = 100;     // Sword Enable Width
int tbSwordCharNameTextWidth     = 60;      // Sword Character Name Text Width
int tbSwordCharNameWidth         = 270;     // Sword Character Name Width
int tbSwordDummy1TextWidth       = 60;      // Sword Dummy 1 Text Width
int tbSwordDummy1Width           = 270;     // Sword Dummy 1 Width
int tbSwordDummy2TextWidth       = 60;      // Sword Dummy 2 Text Width
int tbSwordDummy2Width           = 270;     // Sword Dummy 2 Width
int tbSwordTailLengthTextWidth   = 105;     // Sword Tail Length Text Width
int tbSwordTailLengthWidth       = 150;     // Sword Tail Length Width
int tbSwordTailDivisionOffset    = 70;      // Sword Tail Division Offset
int tbSwordTailDivisionTextWidth = 50;      // Sword Tail Division Text Width
int tbSwordTailDivisionWidth     = 95;      // Sword Tail Division Width
int tbSwordTailSegmentTextWidth  = 55;      // Sword Tail Segment Text Width
int tbSwordTailSegmentWidth      = 100;     // Sword Tail Segment Width
int tbSwordTextureTextWidth      = 65;      // Sword Texture Text Width
int tbSwordTextureNameWidth      = 188;     // Sword Texture Name Width
int tbSwordTextureNumWidth       = 32;      // Sword Texture Number Width
int tbSwordTextureButtonWidth    = 16;      // Sword Texture Add/Remove Button Width
int tbSwordAlphaMapTextWidth     = 65;      // Sword Texture Alpha Map Text Width
int tbSwordAlphaMapWidth         = 188;     // Sword Texture Alpha Map Width
int tbSwordAlphaMapBtnOffset     = 41;      // Sword Texture Alpha Map Add Button Offset
int tbSwordBlendModeTextWidth    = 60;      // Sword Texture Blending Mode Text Width
int tbSwordBlendModeWidth        = 218;     // Sword Texture Blending Mode Width
int tbSwordColorTextWidth        = 65;      // Sword Color Text Width
int tbSwordColorRWidth           = 110;     // Sword Color Red Width
int tbSwordColorGWidth           = 45;      // Sword Color Green Width
int tbSwordColorBWidth           = 45;      // Sword Color Blue Width
int tbSwordColorAWidth           = 45;      // Sword Color Alpha Width

// model viewer UIs
UIid tbModelFileTabID = FAILED_ID;
UIid tbLoadModelButtonID = FAILED_ID;
UIid tbSaveModelButtonID = FAILED_ID;
UIid tbTransformUIs[8];                      // Transform UI IDs Array
UIid tbModelTransformTabID = FAILED_ID;      // Transform Tab UI
UIid tbModelTFTranslateButtonID = FAILED_ID; // Transform -- Translate Button
UIid tbModelTFTranslateXID      = FAILED_ID; // Transform -- TranslateText + TranslateX UI
UIid tbModelTFTranslateYID      = FAILED_ID; // Transform --                 TranslateY UI
UIid tbModelTFTranslateZID      = FAILED_ID; // Transform --                 TranslateZ UI
UIid tbModelTFRotateButtonID = FAILED_ID;    // Transform -- Rotate Button
UIid tbModelTFRotateXID      = FAILED_ID;    // Transform -- RotateText + RotateX UI
UIid tbModelTFRotateYID      = FAILED_ID;    // Transform --              RotateY UI
UIid tbModelTFRotateZID      = FAILED_ID;    // Transform --              RotateZ
UIid tbMaterialCurrentTabID = FAILED_ID;
UIid tbRenderCurrentTabID = FAILED_ID;
UIid tbTexture0TabID = FAILED_ID;
UIid tbTexture1TabID = FAILED_ID;
UIid tbTexture2TabID = FAILED_ID;
UIid tbTexture3TabID = FAILED_ID;
UIid tbTexture4TabID = FAILED_ID;
UIid tbTexture5TabID = FAILED_ID;
UIid tbMatDataTabID = FAILED_ID;
UIid tbWireframeCurModelButtonID = FAILED_ID;
UIid tbShowCurModelButtonID = FAILED_ID;
UIid tbRGCurModelID = FAILED_ID;
UIid tbPreviousModelButtonID = FAILED_ID;
UIid tbNextModelButtonID = FAILED_ID;
UIid tbSelectMaterialButtonID = FAILED_ID;
UIid tbAlphaCurModelButtonID = FAILED_ID;

int tbMatHeight = 228;
int tbRenderHeight = 80;

// character viewer UIs
UIid tbCharacterFileTabID = FAILED_ID;
UIid tbCharacterCurrentTabID = FAILED_ID;
UIid tbPoseExploreTabID = FAILED_ID;
UIid tbLoadCharacterButtonID = FAILED_ID;
UIid tbSaveCharacterButtonID = FAILED_ID;
UIid tbExportCharacterModelButtonID = FAILED_ID;
UIid tbDeleteCharacterButtonID = FAILED_ID;
UIid tbSelectCharacterComboID = FAILED_ID;
UIid tbBakeCharacterVAID = FAILED_ID;
UIid tbSelectBodyComboID = FAILED_ID;
UIid tbSelectActionComboID = FAILED_ID;
UIid tbSelectPoseComboID = FAILED_ID;
UIid tbDeletePoseButtonID = FAILED_ID;
UIid tbPoseStartID = FAILED_ID;
UIid tbPoseEndID = FAILED_ID;
UIid tbPlayPoseButtonID = FAILED_ID;
UIid tbPoseMotionNameID = FAILED_ID;
UIid tbMakeBaseMoveButtonID = FAILED_ID;
UIid tbEditActionButtonID = FAILED_ID;
UIid tbAddActionButtonID = FAILED_ID;
UIid tbDeleteActionButtonID = FAILED_ID;
UIid tbLoadMotionButtonID = FAILED_ID;
UIid tbSetLocalScaleID = FAILED_ID;
UIid tbCalculateBBButtonID = FAILED_ID;

// game FX viewer UIs
UIid tbGameFXFileTabID = FAILED_ID;
UIid tbFXSystemCurrentTabID = FAILED_ID;
UIid tbLoadFXSystemButtonID = FAILED_ID;
UIid tbNewFXSystemButtonID = FAILED_ID;
UIid tbSaveFXSystemButtonID = FAILED_ID;
UIid tbDeleteFXSystemButtonID = FAILED_ID;
UIid tbSelectFXSystemComboID = FAILED_ID;
UIid tbNewFXButtonID = FAILED_ID;
UIid tbSelectFXComboID = FAILED_ID;
UIid tbBaseFXFXPosID[3] = { FAILED_ID, FAILED_ID, FAILED_ID };
UIid tbBaseFXFXPosEditKeyID = FAILED_ID;
UIid tbBaseFXFXRotateID[3] = { FAILED_ID, FAILED_ID, FAILED_ID };
UIid tbBaseFXFXRotateEditKeyID = FAILED_ID;
UIid tbBaseFXShowID = FAILED_ID;
UIid tbBaseFXShowEditKeyID = FAILED_ID;
UIid tbBaseLifeID = FAILED_ID;
UIid tbSetFXParentID = FAILED_ID;
UIid tbBaseLifeAutoAdjustID = FAILED_ID;
UIid tbDeleteFXButtonID = FAILED_ID;
UIid tbEditPlateFXTabID = FAILED_ID;
UIid tbPlateFXTypeID = FAILED_ID;
UIid tbPlateFXTextureNameID = FAILED_ID;
UIid tbPlateFXTextureNumberID = FAILED_ID;
UIid tbPlateFXAlphamapNameID = FAILED_ID;
UIid tbPlateFXTextureAddID = FAILED_ID;
UIid tbPlateFXAlphamapAddID = FAILED_ID;
UIid tbPlateFXTextureRemoveID = FAILED_ID;
UIid tbPlateFXBlendTypeID = FAILED_ID;
UIid tbPlateFXSizeID[2] = { FAILED_ID, FAILED_ID };
UIid tbPlateFXSizeEditKeyID = FAILED_ID;
UIid tbPlateFXColorID[4] = { FAILED_ID, FAILED_ID, FAILED_ID, FAILED_ID };
UIid tbPlateFXColorEditKeyID = FAILED_ID;
UIid tbEditParticleFXTabID = FAILED_ID;
UIid tbParticleLifeID[2] = { FAILED_ID, FAILED_ID };
UIid tbParticleBornID[2] = { FAILED_ID, FAILED_ID };
UIid tbParticleSizeID[2] = { FAILED_ID, FAILED_ID };
UIid tbParticleBlendTypeID = FAILED_ID;
UIid tbParticleVelocityID[3] = { FAILED_ID, FAILED_ID, FAILED_ID };
UIid tbParticleVelocityVarID[3] = { FAILED_ID, FAILED_ID, FAILED_ID };
UIid tbParticleTextureNameID = FAILED_ID;
UIid tbParticleTextureAddID = FAILED_ID;
UIid tbParticleTextureRemoveID = FAILED_ID;
UIid tbParticleAlphamapNameID = FAILED_ID;
UIid tbParticleAlphamapAddID = FAILED_ID;
UIid tbParticleTextureAnimID[3] = { FAILED_ID, FAILED_ID, FAILED_ID };
UIid tbEditForceFXTabID = FAILED_ID;
UIid tbForceFXTypeID = FAILED_ID;
UIid tbForceMagnitudeID = FAILED_ID;
UIid tbForceMinDistID = FAILED_ID;                    // FX -- Force -- Minimum Distance Typein
UIid tbForceUsedByID = FAILED_ID;

UIid tbEditGeometryFXTabID = FAILED_ID;
UIid tbLoadGeometryFXID = FAILED_ID;
UIid tbGeometryNameFXID = FAILED_ID;
UIid tbGeometryFXTextureNameID = FAILED_ID;
UIid tbGeometryFXTextureNumberID = FAILED_ID;
UIid tbGeometryFXTextureAddID = FAILED_ID;
UIid tbGeometryFXTextureRemoveID = FAILED_ID;
UIid tbGeometryFXBlendTypeID = FAILED_ID;
UIid tbGeometryFXAlphamapNameID = FAILED_ID;
UIid tbGeometryFXAlphamapAddID = FAILED_ID;

UIid tbEditAudioFXTabID = FAILED_ID;               // FX -- Audio Tab
UIid tbAudioFXNameID = FAILED_ID;                  // FX -- Audio File Name
UIid tbAudioFXLoadButtonID = FAILED_ID;            // FX -- Audio File Load
UIid tbAudioFXStartID = FAILED_ID;                 // FX -- Audio Start Frame

UIid tbEditSwordFXTabID = FAILED_ID;               // FX -- Sword -- Tab
UIid tbSwordFXEnableRButtonID = FAILED_ID;         // FX -- Sword -- Enable/Disable Radio Button
UIid tbSwordFXCharacterComboID = FAILED_ID;        // FX -- Sword -- Character Name
UIid tbSwordFXDummy1ComboID = FAILED_ID;           // FX -- Sword -- Dummy 1 Name
UIid tbSwordFXDummy2ComboID = FAILED_ID;           // FX -- Sword -- Dummy 2 Name
UIid tbSwordFXTailLengthTypeinID = FAILED_ID;      // FX -- Sword -- Tail Length (Blade Max Num)
UIid tbSwordFXTailSegmentTypeinID = FAILED_ID;     // FX -- Sword -- Tail Segment (Segment of each Blade)
UIid tbSwordFXTailDivisionTypeinID = FAILED_ID;    // FX -- Sword -- Tail Division (Divide of each Blade)
UIid tbSwordFXTextureNameTypeinID = FAILED_ID;     // FX -- Sword -- Texture Name
UIid tbSwordFXTextureNumTypeinID = FAILED_ID;      // FX -- Sword -- Texture Number
UIid tbSwordFXTextureAddButtonID = FAILED_ID;      // FX -- Sword -- Texture Add Button
UIid tbSwordFXTextureRemoveButtonID = FAILED_ID;   // FX -- Sword -- Texture Remove Button
UIid tbSwordFXAlphaMapNameTypeinID = FAILED_ID;    // FX -- Sword -- Alpha Map Name
UIid tbSwordFXAlphaMapAddButtonID = FAILED_ID;     // FX -- Sword -- Alpha Map Add Button
UIid tbSwordFXAlphaMapRemoveButtonID = FAILED_ID;  // FX -- Sword -- Alpha Map Remove Button
UIid tbSwordFXBlendModeComboID = FAILED_ID;        // FX -- Sword -- Blending Mode Combo Button
UIid tbSwordFXColorTypeinID[4] = { FAILED_ID, FAILED_ID, FAILED_ID, FAILED_ID };   // FX -- Sword -- Color

// Post-processing UIs
UIid tbSSAOTabID = FAILED_ID;
UIid tbHDRTabID = FAILED_ID;
UIid tbDOFTabID = FAILED_ID;
UIid tbSSAORadioID = FAILED_ID;
UIid tbSampleRangeTypeinID = FAILED_ID;
UIid tbSSAODistanceTypeinID = FAILED_ID;
UIid tbHDRRadioID = FAILED_ID;

// Face tool UIs
UIid tbSelectFaceTabID = FAILED_ID;
UIid tbFindFaceModelComboID = FAILED_ID;
UIid tbFindFaceMaterialComboID = FAILED_ID;
UIid tbLoadFaceMaskButtonID = FAILED_ID;
UIid tbSaveFaceMaskButtonID = FAILED_ID;

// facial expression tool UIs
UIid tbBlendshapeTabID = FAILED_ID;
UIid tbSelectFacialExpressionID = FAILED_ID;
UIid tbAddFacialExpressionID = FAILED_ID;
UIid tbRemoveFacialExpressionID = FAILED_ID;
UIid tbFaceFrameCountID = FAILED_ID;
UIid tbStopFacialExpressionID = FAILED_ID;
UIid tbPlayFacialExpressionID = FAILED_ID;
UIid tbSelectFacialExpressionKeyID = FAILED_ID;
UIid tbAddFacialExpressionKeyID = FAILED_ID;
UIid tbRemoveFacialExpressionKeyID = FAILED_ID;
UIid tbBlendShapeID[MAX_BLENDSHAPES] = { FAILED_ID };
UIid tbLoadFacialExpressionID = FAILED_ID;
UIid tbSaveFacialExpressionID = FAILED_ID;

int tbNumGameKeyUI = 0;
UIid tbGameKeyUI[1024];

// setup environment UIs
UIid tbWorkDirTypeinUI = FAILED_ID;
UIid tbShaderDirTypeinUI = FAILED_ID;
UIid tbModelDirTypeinUI = FAILED_ID;
UIid tbCharacterDirTypeinUI = FAILED_ID;
UIid tbTextureDirTypeinUI = FAILED_ID;

// play panel UIs
UIid tbControlPanelPlateUI = FAILED_ID;
UIid tbPlayID = FAILED_ID;
UIid tbStopID = FAILED_ID;
UIid tbPlaySpeedID = FAILED_ID;
UIid tbPlayBarID = FAILED_ID;
UIid tbCurFrameID = FAILED_ID;
UIid tbSelectPlayModeID = FAILED_ID;
UIid tbDebugTypeinID = FAILED_ID;
UIid tbSelectCameraID = FAILED_ID;

int tbNumSceneButton = 0;
UIid tbSceneButton[MAX_SCENE_BUTTONS];

// System panel UIs
UIid tbEnvironmentPlanelPlateUI = FAILED_ID;

// warning dialogue UIs
UISYSTEMid tbWarnDialogueID = FAILED_ID;
FnUISystem tbWarnUI;
int tbWarnOX = 100, tbWarnOY = 100, tbWarnWidth = 300, tbWarnHeight = 300;
UIid tbWarnMessageID = FAILED_ID;
UIid tbWarnOKID = FAILED_ID;
UIid tbWarnCancelID = FAILED_ID;

// add action dialogue UIs
UISYSTEMid tbAddActionDialogueID = FAILED_ID;
FnUISystem tbAddActionUI;
int tbAddActionW = 250, tbAddActionH = 120, tbAddActionOX = 0, tbAddActionOY = 0;
UIid tbAddActionCancelID = FAILED_ID;
UIid tbAddActionOKID = FAILED_ID;
UIid tbAddActionBackgroundID = FAILED_ID;
UIid tbAddActionNameID = FAILED_ID;
UIid tbAddActionTypeID = FAILED_ID;
UIid tbAddActionFrontPoseID = FAILED_ID;
UIid tbAddActionEndPoseID = FAILED_ID;
UIid tbAddActionDurationID = FAILED_ID;
UIid tbAddActionWeight0ID = FAILED_ID;
UIid tbAddActionWeight1ID = FAILED_ID;

// add facial expression UIs
UISYSTEMid tbAddFacialExpressionDialogueID = FAILED_ID;
FnUISystem tbAddFacialExpressionUI;
int tbAddFacialExpressionW = 250, tbAddFacialExpressionH = 120, tbAddFacialExpressionOX = 0, tbAddFacialExpressionOY = 0;
UIid tbAddFacialExpressionBackgroundID = FAILED_ID;
UIid tbAddFacialExpressionCancelID = FAILED_ID;
UIid tbAddFacialExpressionOKID = FAILED_ID;
UIid tbAddFacialExpressionNameID = FAILED_ID;
UIid tbAddFacialExpressionLengthID = FAILED_ID;


// add game FX system dialogue UIs
UISYSTEMid tbAddGameFXSystemDialogueID = FAILED_ID;
FnUISystem tbAddGameFXSystemUI;
int tbAddGameFXSystemW = 250, tbAddGameFXSystemH = 120, tbAddGameFXSystemOX = 0, tbAddGameFXSystemOY = 0;
UIid tbAddFXSystemBackgroundID = FAILED_ID;
UIid tbAddFXSystemCancelID = FAILED_ID;
UIid tbAddFXSystemOKID = FAILED_ID;
UIid tbAddFXSystemNameID = FAILED_ID;

// add game FX dialogue UIs
UISYSTEMid tbAddGameFXDialogueID = FAILED_ID;
FnUISystem tbAddGameFXUI;
int tbAddGameFXW = 250, tbAddGameFXH = 120, tbAddGameFXOX = 0, tbAddGameFXOY = 0;
UIid tbAddFXBackgroundID = FAILED_ID;
UIid tbAddFXCancelID = FAILED_ID;
UIid tbAddFXOKID = FAILED_ID;
UIid tbAddFXNameID = FAILED_ID;
UIid tbAddFXTypeID = FAILED_ID;
UIid tbFXLifeID = FAILED_ID;
UIid tbMaxParticleNumID = FAILED_ID;
UIid tbPickForceTypeID = FAILED_ID;
UIid tbPickForceMagnitudeID = FAILED_ID;

// keyframe editing dialogue UIs
UISYSTEMid tbGameFXKeyframeDialogueID = FAILED_ID;
FnUISystem tbGameFXKeyframeUI;
int tbGameFXKeyframeW = 250, tbGameFXKeyframeH = 120, tbGameFXKeyframeOX = 0, tbGameFXKeyframeOY = 0;
UIid tbGameFXKeyframeBackgroundID = FAILED_ID;
UIid tbGameFXKeyframeModifyID = FAILED_ID;
UIid tbGameFXKeyframeRemoveID = FAILED_ID;
UIid tbGameFXKeyframeCancelID = FAILED_ID;
UIid tbGameFXKeyFrameID = FAILED_ID;
UIid tbGameFXKeyDataID[4] = { FAILED_ID, FAILED_ID, FAILED_ID, FAILED_ID };

// current material editing UIs
UIid tbShaderFileID = FAILED_ID;
#ifdef FLY2_DX9
   UIid tbEffectID = FAILED_ID;
#endif
UIid tbShaderLoaderID = FAILED_ID;
UIid tbShaderRemoveID = FAILED_ID;
UIid tbAmbientRID = FAILED_ID;
UIid tbAmbientGID = FAILED_ID;
UIid tbAmbientBID = FAILED_ID;
UIid tbDiffuseRID = FAILED_ID;
UIid tbDiffuseGID = FAILED_ID;
UIid tbDiffuseBID = FAILED_ID;
UIid tbSpecularRID = FAILED_ID;
UIid tbSpecularGID = FAILED_ID;
UIid tbSpecularBID = FAILED_ID;
UIid tbEmissiveRID = FAILED_ID;
UIid tbEmissiveGID = FAILED_ID;
UIid tbEmissiveBID = FAILED_ID;
UIid tbShininessID = FAILED_ID;
UIid tbTexture0ID = FAILED_ID;
UIid tbTextureNumID = FAILED_ID;
UIid tbTexture0AddID = FAILED_ID;
UIid tbTexture0RemoveID = FAILED_ID;
UIid tbTexture1ID = FAILED_ID;
UIid tbTexture1AddID = FAILED_ID;
UIid tbTexture1RemoveID = FAILED_ID;
UIid tbTexture2ID = FAILED_ID;
UIid tbTexture2AddID = FAILED_ID;
UIid tbTexture2RemoveID = FAILED_ID;
UIid tbTexture3ID = FAILED_ID;
UIid tbTexture3AddID = FAILED_ID;
UIid tbTexture3RemoveID = FAILED_ID;
UIid tbTexture4ID = FAILED_ID;
UIid tbTexture4AddID = FAILED_ID;
UIid tbTexture4RemoveID = FAILED_ID;
UIid tbTexture5ID = FAILED_ID;
UIid tbTexture5AddID = FAILED_ID;
UIid tbTexture5RemoveID = FAILED_ID;
UIid tbUpdateMaterialID = FAILED_ID;
UIid tbCancelMaterialID = FAILED_ID;
UIid tbDeleteModelButtonID = FAILED_ID;
UIid tbFindModelComboID = FAILED_ID;
int tbMatTexutreTypeinOffset = 20;           // Texture Layer 0~5        Typein UI Offset
int tbMatTexture0TypeinWidth = 165;          // Texture Layer 0          Typein UI Width
int tbMatTextureNumWidth     = 45;           // Texture Layer 0   Number Typein UI Width
int tbMatTextureUIBound      = 5;            // Texture Layer 0~5               UI Bound
int tbMatTextureTypeinWidth  = 215;          // Texture Layer 1~5        Typein UI Width
int tbNumMatUI = 36;
UIid tbMatUI[36];
BOOL4 tbModelUpdateState = FALSE;            // Enable Status of Material Update UI in Model
int tbNumShowMatDataUI = 0;
UIid tbMatDataUI[MAX_MATERIAL_DATA*3];
int tbMatDataID[MAX_MATERIAL_DATA];
int tbMatDataLen[MAX_MATERIAL_DATA];
TBMaterial tbMatBuffer;

char tbTmpShaderPath[256];
char tbTmpTexturePath[256];

// for gizmo
float tbGizmoRadiusTBig   = 15.0f;      // Gizmo Translate Big   Radius
float tbGizmoRadiusTSmall = 5.0f;       // Gizmo Translate Small Radius
float tbGizmoRadiusR      = 15.0f;      // Gizmo Rotate          Radius
float tbGizmoBackupM12[12];             // Gizmo Backup M12

TBGizmo tbGizmoTranslate;   // Aux Tool Object of Translate
TBGizmo tbGizmoRotate;      // Aux Tool Object of Rotate

// workspace viewports
VIEWPORTid tbWorkSpaceBTID = FAILED_ID;
FnViewport tbWSBottomVP;
int tbBottomVPOX = 260, tbBottomVPOY = 40, tbBottomVPWidth = 955, tbBottomVPHeight = 600;

VIEWPORTid tbWorkSpaceID = FAILED_ID;
FnViewport tbWorkSpaceVP;
int tbWorkSpaceVPOX = 418, tbWorkSpaceVPOY = 100, tbWorkSpaceVPWidth = 800, tbWorkSpaceVPHeight = 600;


// etc
int tbResizeWidth = 1024, tbResizeHeight = 768;
BOOL4 tbBeResize = FALSE;
float tbCurResizeTime = 0.0f;
HCURSOR tbArrowCursor;
HCURSOR tbWaitCursor;
BOOL4 tbRunScript = FALSE;
char tbDebugInfo[256];
BOOL4 tbShowDeb = TRUE;
BOOL4 tbBe1stSetName = FALSE;
int tbInputInteger = 100;

// for post-processing
float tbSSAOSampleRange = 18.0f;
float tbSSAODistance = 6.0f;
BOOL4 tbBeSSAO = FALSE;
BOOL4 tbBeHDR = FALSE;
int tbHDRLoop = 8;
int tbHDRType = HDR_EFFECT;

// functions
void TbCancelMaterialEditing();
void TbPasteCurrentMaterialData(FnMaterial *mat);
void TbPasteCurrentTexture(FnMaterial *mat, int layer);
void TbExtractMaterialUIData();
void TbUpdateCurrentMaterial(FnMaterial *mat);
void TbUpdateCurrentTexture(FnMaterial *mat, int layer);
void TbOpenQuit(UIid uiID, BOOL4 bePress);
void TbOpenNew(UIid uiID, BOOL4 bePress);
void TbOpenAddAction(UIid uiID, BOOL4 bePress);
void TbOpenAddGameFX(UIid uiID, BOOL4 bePress);
void TbOpenAddGameFXSystem(UIid uiID, BOOL4 bePress);
void TbSwitchMenuBarRadios(UIid uiID, BOOL4 bePressed);
void TbUpdateCurrentBodyDataUI(FnCharacter *actor, BOOL4 beUpdateUI = TRUE);
void TbDrawBlueBox(FnObject *object, float s, float *color);
void TbMakePlay(UIid uiID, BOOL4 bePress);
void TbUpdatSelectKeysUI(FnBaseGameFX *, int iFrame);
void TbEnableGameFXKeyUI(BOOL4 beEnable);
void TbEnableModelMaterialUI(BOOL4 beEnable);
void TbShowOrthoView(UIid uiID, BOOL4 bePress);
void TbInitFaceTool(int iOne);
void TbFormTextureModel();
void TbCheckHomePath(char *path, char *fPath);
void TbRenderItOnce();
TBFaceMaskForQ3D *TbLoadQ3DFaceMask(char *fileName, int nV, int nT);
TBFaceMaskForQ3D *TbAdjustQ3DFaceMask(OBJECTid fID, int startX, int startY, int endX, int endY, int mode);
void TbAddSubFaceMaskTri(TBFaceMaskForQ3D *mask, int idxTri, int mode);
void TbAdjustFaceMaskLMPress(int x, int y);
void TbAdjustFaceMaskLMDrag(int x, int y);
void TbAdjustFaceMaskLMRelease(int x, int y);
void TbAdjustFaceMaskCreateCommand();
void TbHeapsortSingle(int num, int * ary);
void TbRemoveRepeatData(int * num, int * ary);
void TbSaveQ3DFaceMask(char *fileName, TBFaceMaskForQ3D *mask);
OBJECTid TbCreateFaceMask(OBJECTid fID, TBFaceMaskForQ3D *mask);
void TbUpdateCurrentFacialExpressionUIs();
void TbGotoFacialExpressionFrame(UIid uiID, char *value);
void TbStopPlayFacialExpression(UIid uiID, BOOL4 beP);


// gizmo functions
OBJECTid TbGetCurrentBaseObjID();
void TbSwitchGizmo(UIid uiID, BOOL4 bePress);
void TbShowGizmo(BYTE code, BOOL4 value);
void TbCancelGizmoProcess(BYTE code, BOOL4 value);
void TbGenTransformArgu(char * argu, int len);
void TbUpdateTransformCommand(UIid uiID, char *value);
void TbPasteCurrentTransformData(float * m12);
void TbUpdateTransformUIData(BOOL4 beUpdate);
void TbPasteCurrentTypeinValue(float * pos, float * rot);
void TbHitGizmo(int x, int y, BOOL4 beChoose=FALSE);
void TbHitGizmoEle(float x, float y, TBGizmo * gizmo, int idxEle, float * dis, float * slop=NULL);
void TbHitLine(float x, float y, float * line, float * dis);
void TbDragGizmo(int x, int y);
void TbReleaseGizmo();

// game FX -- audio functions
void TbGameFXKeyFrameCancelCallback(UIid uiID, BOOL4 bePress);
void TbGameFXAudioLoadBtnCallback(UIid uiID, BOOL4 bePress);
void TbGameFXAudioLoadToCommand(UIid uiID, char *value);
void TbGameFXAudioChangeStartCallback(UIid uiID, char *value);
void TbGameFXAudioChangeStartToCommand(UIid uiID, char *value);
void TbGameFXSwordSelectCharComboCallback(UIid uiID, char *value);
void TbGameFXSwordSelectCharToCommand(UIid uiID, char *value);
void TbGameFXSwordSelectDummy1ComboCallback(UIid uiID, char *value);
void TbGameFXSwordSelectDummy1ToCommand(UIid uiID, char *value);
void TbGameFXSwordSelectDummy2ComboCallback(UIid uiID, char *value);
void TbGameFXSwordSelectDummy2ToCommand(UIid uiID, char *value);
void TbGameFXSwordSelectDummy1SearchCallback(UIid uiID, char *value);
void TbGameFXSwordSelectDummy2SearchCallback(UIid uiID, char *value);
void TbGameFXSwordSelectDummySearchAlg(UIid uiID, char *value);
void TbGameFXSwordChangeParamButtonCallback(UIid uiID, BOOL4 bePress);
void TbGameFXSwordChangeParamTypeinCallback(UIid uiID, char *value);
void TbGameFXSwordChangeParamTypeinToCommand(UIid uiID, char *value);
void TbGameFXSwordTextureTypeinCallback(UIid uiID, char *value);
void TbGameFXSwordAlphamapTypeinCallback(UIid uiID, char *value);
void TbGameFXSwordAddBtnCallback(UIid uiID, BOOL4 bePress);
void TbGameFXSwordRemoveBtnCallback(UIid uiID, BOOL4 bePress);
void TbGameFXSwordChangeTextureToCommand(UIid uiID, char *value);
void TbGameFXSwordChangeAlphamapToCommand(UIid uiID, char *value);
void TbUpdateSwordFXUIs(GAMEFXid gxID = FAILED_ID);
DWORD TbGetBlendNum(char * value);                                                  // get blend type ID from string
void  TbGetBlendString(char * value, DWORD num);                                    // get blend string  from type ID
BOOL4 TbAdjustTextureFileNameFromUI(char * fileName, char ** fileTail, int nTex);   // adjust texture file name according to Texture number
void  TbAdjustTextureFileNameFromMat(char * fileName, int nTex);                    // adjust texture file name according to Texture name

void TbDebugKey(BYTE key, BOOL4 bePress);

#ifdef FLY2_RENDER
RENDERid tbAORenderID = FAILED_ID;
#endif

// criticision section for multithreading
struct TBCriticalSection
{
   CRITICAL_SECTION cs;        // critical section
   int id;
};

TBCriticalSection tbRenderCS;
TBCriticalSection tbScriptCS;


// all command functions
int tbNumCommandSlots = 200;
int tbNumCommandHistory = 200;
int tbCommandCursor = 0;
char *tbCurCommandPtr = NULL;
int tbCurCommand = 0;
char **tbCommandHistory = NULL;
BOOL4 tbShowCommand = TRUE;
BOOL4 tbBeBlink = FALSE;
BOOL4 tbBeDisplayCommandHistory = FALSE;
float tbLogOpacity = 512.0f;
char tbLogMessage[MAX_STRING_CHARACTERS];

BOOL4 TbCommandParser(char *command, char *argument, BOOL4 bePasteCommand);
void TbCommandInput(WORD a);
void TbCommandFunKey(BYTE code, BOOL4 value);
void TbSetLogMessage(char *log);
BOOL4 TbCheckCommandAttributeB(char *, char *, BOOL4 *, int);
BOOL4 TbCheckCommandAttributeI(char *, char *, int *, int);
BOOL4 TbCheckCommandAttributeF(char *, char *, float *, int);
BOOL4 TbCheckCommandAttributeS(char *, char *, char *, int);
BOOL4 TbCheckReal(char *string);
BOOL4 TbCheckInt(char *string);
BOOL4 TbSelectPoseCmd(char *arg);
BOOL4 TbPlayTestPoseCmd(char *arg);
BOOL4 TbSwitchToolModeCmd(char *arg);
BOOL4 TbSetPathCmd(char *arg);
void TbStopPlay(UIid uiID, BOOL4 bePress);


// script command
struct TBScript
{
   BOOL4 beRun;
   int length;
   int maxLength;
   char *systemString;
};


// script variables
struct TBVariable
{
   BOOL4 beUsed;
   DWORD type;
   char name[32];
   char raw[256];
   union
   {
      float f;
      int i;
      BOOL4 b;
   } value;
};


struct TBLoopStack
{
   char index[64];
   int startV;
   int endV;
   int incV;
   int curV;
   int loopEntry;
};


int tbCurScript = FY_NONE;
TBScript tbScript[MAX_SCRIPTS];

int tbNumGlobal = 0;
int tbNumLocal = 0;
TBVariable tbGlobals[MAX_GLOBALS];
TBVariable tbLocals[MAX_LOCALS];

int tbCurLoopStackID = FY_NONE;
TBLoopStack tbLoopStack[MAX_LOOP_STACKS];

BOOL4 tbBeUseGlobals = TRUE;

void TbCopySystemString(char *string, int maxLen);
int TbGetChar();
int TbGetNewLine(char *str);
void TbTruncateAllSpaces(char * str);
void TbInitScriptSystem();
void TbDeinitScriptSystem();
void TbExecuteScript(int skip);
int TbRegisterVariable(char *name, char *raw, DWORD type, int arrayLen, BOOL4 beGlobal);
void TbUnregisterVariable(char *name, BOOL4 beGlobal);
char *TbExaminevariable(char *name, BOOL4 beGlobal);
char *TbSetVariableValue(char *cmd, char *argument);
BOOL4 TbLoadScript(char *arg);
BOOL4 TbSetIntegerCmd(char *arg, char *log, void *data);
BOOL4 TbSetBooleanCmd(char *arg, char *log, void *data);
BOOL4 TbSetFloatCmd(char *arg, char *log, void *data);
void TbUpdateCurrentFXUIs();


/*--------------------
  TbScene constructor
  C.Wang 1001, 2012
 ---------------------*/
TBScene::TBScene()
{
   iCurObject = FY_NONE;
   numObject = 0;
   maxObject = 0;
   objList = NULL;
   curMatID = FAILED_ID;

   numActor = 0;
   maxActor = 0;
   actorList = NULL;

   iCurFXS = FY_NONE;
   numFXS = 0;
   maxFXS = 0;
   fxsList = NULL;

   curFrame = 0.0f;
   sceneButtonID = FAILED_ID;
   camRefID = camID = mainLightID = auxLightID = FAILED_ID;
}


/*-------------------
  TbScene destructor
  C.Wang 1001, 2012
 --------------------*/
TBScene::~TBScene()
{
   int i;

   // clean object list
   if (objList != NULL) {
      for (i = 0; i < numObject; i++) {
         if (objList[i] != NULL) {
            FyFree(objList[i]);
            objList[i] = NULL;
         }
      }
      FyFree(objList);
      objList = NULL;
      numObject = 0;
      maxObject = 0;
   }

   // clean actor list
   if (actorList != NULL) {
      for (i = 0; i < numActor; i++) {
         if (actorList[i] != NULL) {
            FyFree(actorList[i]);
            actorList[i] = NULL;
         }
      }
      FyFree(actorList);
      actorList = NULL;
      numActor = 0;
      maxActor = 0;
   }

   // clean game FX list
   if (fxsList != NULL) {
      for (i = 0; i < numFXS; i++) {
         if (fxsList[i] != NULL) {
            FyFree(fxsList[i]);
            fxsList[i] = NULL;
         }
      }
      FyFree(fxsList);
      fxsList = NULL;
      numFXS = 0;
      maxFXS = 0;
   }
   iCurFXS = FY_NONE;

   // delete the scene
   FyDeleteScene(Object());
}


/*-------------------------------------
  clear the scene and reset the camera
  C.Wang 0115, 2014
 --------------------------------------*/
void TBScene::New()
{
   int i;
   if (objList != NULL) {
      for (i = 0; i < numObject; i++) {
         if (objList[i] != NULL) {
            if (objList[i]->UsedBy() == FAILED_ID) {
               this->DeleteObject(objList[i]->ID());
            }
            FyFree(objList[i]);
            objList[i] = NULL;
         }
      }
      FyFree(objList);
      objList = NULL;
      numObject = 0;
      maxObject = 0;
   }

   if (actorList != NULL) {
      for (i = 0; i < numActor; i++) {
         if (actorList[i] != NULL) {
            this->DeleteCharacter(actorList[i]->ID());
            FyFree(actorList[i]);
            actorList[i] = NULL;
         }
      }
      FyFree(actorList);
      actorList = NULL;
      numActor = 0;
      maxActor = 0;
   }

   if (fxsList != NULL) {
      for (i = 0; i < numFXS; i++) {
         if (fxsList[i] != NULL) {
            this->DeleteGameFXSystem(fxsList[i]->ID());
            FyFree(fxsList[i]);
            fxsList[i] = NULL;
         }
      }
      FyFree(fxsList);
      fxsList = NULL;
      numFXS = 0;
      maxFXS = 0;
   }

   iCurObject = FY_NONE;
   iCurActor = FY_NONE;
   iCurFXS = FY_NONE;
   curMatID = FAILED_ID;

   // reset the cameras & lights
   if (tbBeZUp) {
      tbCameraRef.Translate(0.0f, 0.0f, tbMainCamUp, REPLACE);
      tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
      tbCamera.Rotate(X_AXIS, 75.0f, GLOBAL);
   }
   else {
      tbCameraRef.Translate(0.0f, tbMainCamUp, 0.0f, REPLACE);
      tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
      tbCamera.Rotate(X_AXIS, -25.0f, GLOBAL);
   }

   // create the main light
   FnLight light;
   light.ID(mainLightID);
   light.Rotate(X_AXIS, 30.0f, LOCAL);
   light.Translate(100.0, -150.0f, 250.0f, LOCAL);
   light.SetColor(1.0f, 1.0f, 1.0f);
   light.SetIntensity(1.0f);
   light.SetRange(5000.0f);

   float fDir[3];
   fDir[0] = -10.0f; fDir[1] = 10.0f; fDir[2] = -100.0f;
   FyNormalizeVector3(fDir);
   light.SetDirection(fDir, NULL, TRUE);

   // create the auxilary light & object
   light.ID(auxLightID);
   light.Rotate(X_AXIS, -30.0f, LOCAL);

   // clean current material UI
   FnMaterial mat;
   TbPasteCurrentMaterialData(&mat);
}


/*------------------------------------------------------
  TbScene member function : reset the animation playing
  C.Wang 1206, 2012
 -------------------------------------------------------*/
void TBScene::ResetPlay(BOOL4 beBase)
{
   int i;

   // play character's motion
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         actorList[i]->Play(START, 0.0f, beBase, TRUE);

         // reset the position
         float pos[3];
         pos[0] = pos[1] = pos[2] = 0.0f;
         actorList[i]->SetPosition(pos);
      }
   }

   // play all game FXs
   for (i = 0; i < numFXS; i++) {
      if (fxsList[i] != NULL) {
         fxsList[i]->Reset();
      }
   }

   // play all model's motion
   for (i = 0; i < numObject; i++) {
      if (objList[i] != NULL && objList[i]->UsedBy() == FAILED_ID) {
         // play motion
         if (objList[i]->FrameNumber() > 0) {
            objList[i]->PlayFrame(0.0f);
         }

         // play vertex animation
         if (objList[i]->CheckVertexAnimation()) {
            objList[i]->NextVertexArray(0.0f, START);
         }

         // play texture animation
         if (objList[i]->CheckTextureAnimation()) {
            objList[i]->NextTexture(0, START);
         }

         // play texture UV animation
      }
   }

   curFrame = 0.0f;
}


/*-----------------------------------------------------------
  TbScene member function : play all animations and game FXs
  C.Wang 1206, 2012
 ------------------------------------------------------------*/
float TBScene::Play(float skipFrame, BOOL4 beBase, DWORD playMode)
{
   int i, f, r, u;

   // play all character's motion
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         if (!actorList[i]->Play(ONCE, skipFrame, beBase)) {
            if (playMode == LOOP) {
               actorList[i]->Play(START, 0.0f, beBase);

               // reset the position
               float pos[3];
               pos[0] = pos[1] = pos[2] = 0.0f;
               actorList[i]->SetPosition(pos);
            }
         }
         actorList[i]->QueryLastMoveResult(&f, &r, &u); 
      }
   }

   // play all game FXs
   for (i = 0; i < numFXS; i++) {
      if (fxsList[i] != NULL) {
         fxsList[i]->Play(skipFrame, playMode);
      }
   }

   // play all model's animation
   for (i = 0; i < numObject; i++) {
      if (objList[i] != NULL && objList[i]->UsedBy() == FAILED_ID) {
         // play motion
         if (objList[i]->FrameNumber() > 0) {
            float c = objList[i]->GetCurrentFrame();
            c += skipFrame;
            if (!objList[i]->PlayFrame(c)) {
               if (playMode == LOOP) {
                  objList[i]->PlayFrame(0.0f);
               }
            }
         }

         // play vertex animation
         if (objList[i]->CheckVertexAnimation()) {
            if (!objList[i]->NextVertexArray(skipFrame, ONCE)) {
               if (playMode == LOOP) {
                  objList[i]->NextVertexArray(0.0f, START);
               }
            }
         }

         // play texture animation
         if (objList[i]->CheckTextureAnimation()) {
            if (!objList[i]->NextTexture((int)skipFrame, ONCE)) {
               if (playMode == LOOP) {
                  objList[i]->NextTexture(0, START);
               }
            }
         }

         // play texture UV animation
      }
   }

   // update UIs for game FX

   // increment skip frames
   curFrame += skipFrame;

   return curFrame;
}


/*---------------------------------------------
  TbScene member function : pause all game FXs
  Eric Fei 0102, 2013
 ----------------------------------------------*/
float TBScene::PauseFXPlaying()
{
   // pause all game FXs
   for (int i = 0; i < numFXS; i++) {
      if (fxsList[i] != NULL) {
         fxsList[i]->Pause();
      }
   }

   return curFrame;
}


/*--------------------------------------------
  TbScene member function : get current frame
  C.Wang 1203, 2012
 ---------------------------------------------*/
float TBScene::GetCurrentFrame()
{
   return curFrame;
}


/*--------------------------------------------
  TbScene member function : set current frame
  C.Wang 1203, 2012
 ---------------------------------------------*/
void TBScene::SetCurrentFrame(float curFrame, BOOL4 beBase)
{
   int i, nFrame;
   float restFrame;

   // reset to 1st frame
   ResetPlay(beBase);

   nFrame = (int) curFrame;
   restFrame = curFrame - (float) nFrame;

   for (i = 0; i < nFrame; i++) {
      Play(1.0f, beBase, LOOP);
   }

   if (restFrame > 0.0f) {
      Play(restFrame, beBase, LOOP);
   }
}


/*-----------------------------------------------------------
  TbScene member function : get current object for the scene
  C.Wang 1001, 2012
 ------------------------------------------------------------*/
OBJECTid TBScene::GetCurrentObject()
{
   if (iCurObject < 0 || iCurObject >= numObject) return FAILED_ID;
   if (objList[iCurObject] == NULL) return FAILED_ID;
   return objList[iCurObject]->ID();
}


/*-----------------------------------------------
  TbScene member function : get current TBObject
  C.Wang 1001, 2012
 ------------------------------------------------*/
TBObject *TBScene::GetCurrentTBObject()
{
   if (iCurObject < 0 || iCurObject >= numObject) return NULL;
   return objList[iCurObject];
}


/*------------------------------------------------------------
  TbScene member function : find an object as the current one
  C.Wang 0809, 2012
 -------------------------------------------------------------*/
OBJECTid TBScene::FindObject(char *name)
{
   int i;

   for (i = 0; i < numObject; i++) {
      if (objList[i] != NULL) {
         if (FyCheckWord(objList[i]->GetName(), name)) {
            return objList[i]->ID();
         }
      }
   }
   return FAILED_ID;
}


/*---------------------------------------------
  TbScene member function : get an object's ID
  C.Wang 0110, 2014
 ----------------------------------------------*/
OBJECTid TBScene::GetObjectID(int iOne)
{
   if (iOne < 0 || iOne >= numObject) return FAILED_ID;
   if (objList[iOne] == NULL) return FAILED_ID;
   return objList[iOne]->ID();
}


/*--------------------------------------------------------------
  TbScene member function : get current character for the scene
  C.Wang 1001, 2012
 ---------------------------------------------------------------*/
CHARACTERid TBScene::GetCurrentCharacter()
{
   if (iCurActor < 0 || iCurActor >= numActor) return FAILED_ID;
   if (actorList[iCurActor] == NULL) return FAILED_ID;
   return actorList[iCurActor]->ID();
}


/*-----------------------------------------------
  TbScene member function : get current TBObject
  C.Wang 1001, 2012
 ------------------------------------------------*/
TBCharacter *TBScene::GetCurrentTBCharacter()
{
   if (iCurActor < 0 || iCurActor >= numActor) return NULL;
   return actorList[iCurActor];
}


/*---------------------------------------------
  TbScene member function : assign the cameras 
  C.Wang 0809, 2012
 ----------------------------------------------*/
void TBScene::AssignCamera(OBJECTid rID, OBJECTid cID)
{
   camRefID = rID;
   camID = cID;
}


/*------------------------------------------------
  TbScene member function : get number of cameras
  C.Wanf 1206, 2012
 -------------------------------------------------*/
int TBScene::GetCameraNumber()
{
   // get scene
   int numO = ObjectNumber();
   OBJECTid *oList = NULL;
   if (numO == 0) return 0;

   // get all objects
   oList = (OBJECTid *)FyAllocateBuffer(sizeof(OBJECTid)*numO);
   GetObjects(oList, numO);

   int i, count;
   FnObject obj;

   count = 0;
   for (i = 0; i < numO; i++) {
      obj.ID(oList[i]);
      if (obj.GetType() == CAMERA) {
         count++;
      }
   }

   // release buffer
   FyReleaseBuffer(oList);

   return count;
}


/*------------------------------------------
  TbScene member function : get all cameras
  C.Wanf 1207, 2012
 -------------------------------------------*/
void TBScene::GetCameras(OBJECTid *cList, int bufSize)
{
   // get scene
   int numO = ObjectNumber();
   OBJECTid *oList = NULL;
   if (numO == 0) return;

   // get all objects
   oList = (OBJECTid *)FyAllocateBuffer(sizeof(OBJECTid)*numO);
   GetObjects(oList, numO);

   int i, count;
   FnObject obj;

   count = 0;
   for (i = 0; i < numO; i++) {
      obj.ID(oList[i]);
      if (obj.GetType() == CAMERA && count < bufSize) {
         cList[count] = oList[i];
         count++;
      }
   }

   // release buffer
   FyReleaseBuffer(oList);
}


/*--------------------------------------------
  TbScene member function : assign the lights 
  C.Wang 0809, 2012
 ---------------------------------------------*/
void TBScene::AssignLights(OBJECTid mainID, OBJECTid auxID)
{
   mainLightID = mainID;
   auxLightID = auxID;
}


/*-----------------------------------------
  TbScene member function : get the lights 
  C.Wang 0809, 2012
 ------------------------------------------*/
void TBScene::GetLights(OBJECTid *mainID, OBJECTid *auxID)
{
   if (mainID != NULL) {
      *mainID = mainLightID;
   }
   if (auxID != NULL) {
      *auxID = auxLightID;
   }
}


/*-------------------------------------------
  TbScene member function : insert an object
  C.Wang 0917, 2014
 --------------------------------------------*/
void TBScene::InsertObject(OBJECTid oID, CHARACTERid aID, int type, int id, BOOL4 beGeo)
{
   int i;
   if (objList == NULL) {
      maxObject = 1024;
      objList = (TBObject **)FyMalloc(sizeof(TBObject *)*maxObject);
      for (i = 0; i < maxObject; i++) {
         objList[i] = NULL;
      }
      numObject = 0;
   }
   else {
      if (numObject + 1 >= maxObject) {
         maxObject += 512;
         objList = (TBObject **)FyRealloc(objList, sizeof(TBObject *)*maxObject);
         for (i = numObject; i < maxObject; i++) {
            objList[i] = NULL;
         }
      }
   }

   // scan the object list
   for (i = 0; i < numObject; i++) {
      if (objList[i] == NULL) {
         objList[i] = new TBObject();
         objList[i]->ID(oID);
         objList[i]->UsedBy(aID);
         if (aID != FAILED_ID) {
            objList[i]->Data(type, id);
         }
         else {
            objList[i]->Data(FY_NONE, FY_NONE);
         }
         return;
      }
   }
   objList[numObject] = new TBObject();
   objList[numObject]->ID(oID);
   objList[numObject]->UsedBy(aID);
   objList[numObject]->BeGeometry(beGeo);
   if (aID != FAILED_ID) {
      objList[numObject]->Data(type, id);
   }
   else {
      objList[numObject]->Data(FY_NONE, FY_NONE);
   }
   numObject++;
}


/*-------------------------------------------
  TbScene member function : remove an object
  C.Wang 0809, 2012
 --------------------------------------------*/
CHARACTERid TBScene::RemoveObject(OBJECTid oID, BOOL4 beD, BOOL4 beProcess)
{
   // scan the object list
   int i;
   CHARACTERid aID = FAILED_ID;
   for (i = 0; i < numObject; i++) {
      if (objList[i] != NULL) {
         if (objList[i]->ID() == oID) {
            aID = objList[i]->UsedBy();
            if (beProcess) {
               if (beD) {
                  DeleteObject(oID);
               }
               else {
                  FnObject obj(oID);
                  obj.DropToTrashCan(TRUE);
               }
            }
            delete objList[i];
            objList[i] = NULL;
            if (i == numObject - 1) numObject--;
            return aID;
         }
      }
   }
   return aID;
}


/*-------------------------------------------------------------
  TbScene member function : get number of effective characters
  C.Wang 0809, 2012
 --------------------------------------------------------------*/
int TBScene::GetEffectCharacterNumber()
{
   int i, iCount;
   iCount = 0;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         if (actorList[i]->ID() != FAILED_ID) {
            iCount++;
         }
      }
   }
   return iCount;
}


/*--------------------------------------------------------
  check if current object is one of the character's parts
  C.Wang 0109, 2012
 ---------------------------------------------------------*/
OBJECTid TBScene::CheckCurObjectInCurCharacter()
{
   int i, numO;
   CHARACTERid actorID = GetCurrentCharacter();
   FnCharacter actor(actorID);
   OBJECTid sID, oID;

   // check current object
   oID = GetCurrentObject();

   // check all skins
   numO = actor.GetSkinNumber();
   for (i = 0; i < numO; i++) {
      sID = actor.GetSkin(i);
      if (oID == sID) {
         return oID;
      }
   }

   // check all attachments
   numO = actor.GetAttachmentNumber();
   for (i = 0; i < numO; i++) {
      sID = actor.GetAttachment(i);
      if (oID == sID) {
         return oID;
      }
   }

   // check all bones
   numO = actor.GetBoneNumber();
   for (i = 0; i < numO; i++) {
      sID = actor.GetBoneObject(i);
      if (oID == sID) {
         return oID;
      }
   }
   return FAILED_ID;
}


/*-------------------------------------------------------
  TbScene member function : get the actir ID by his name
  C.Wang 0809, 2012
 --------------------------------------------------------*/
CHARACTERid TBScene::GetCharacterByName(char *name)
{
   int i;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         if (actorList[i]->ID() != FAILED_ID) {
            if (FyCheckWord(actorList[i]->GetName(), name)) {
               return actorList[i]->ID();
            }
         }
      }
   }

   return FAILED_ID;
}


/*-----------------------------------------------
  TbScene member function : get character's name
  C.Wang 0809, 2012
 ------------------------------------------------*/
char *TBScene::GetCharacterName(int id)
{
   if (id < 0 || id >= numActor) return NULL;
   if (actorList[id] == NULL) return NULL;
   if (actorList[id]->ID() == FAILED_ID) return NULL;

   return actorList[id]->GetName();
}


/*---------------------------------------------
  TbScene member function : insert a character
  C.Wang 0408, 2013
 ----------------------------------------------*/
void TBScene::InsertCharacter(CHARACTERid aID)
{
   int i;
   if (actorList == NULL) {
      maxActor = 256;
      actorList = (TBCharacter **)FyMalloc(sizeof(TBCharacter *)*maxActor);
      for (i = 0; i < maxActor; i++) {
         actorList[i] = NULL;
      }
      numActor = 0;
   }
   else {
      if (numActor + 1 >= maxActor) {
         maxActor += 128;
         actorList = (TBCharacter **)FyRealloc(actorList, sizeof(TBCharacter *)*maxActor);
         for (i = numActor; i < maxActor; i++) {
            actorList[i] = NULL;
         }
      }
   }

   // scan the character list
   TBCharacter *actor;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] == NULL) {
         actorList[i] = new TBCharacter();
         actorList[i]->ID(aID);
         return;
      }
   }
   actorList[numActor] = new TBCharacter();
   actorList[numActor]->ID(aID);
   actor = actorList[numActor];
   numActor++;

   // insert all character's objects into the object list
   OBJECTid objID;
   int numO = actor->GetBoneNumber();
   for (i = 0; i < numO; i++) {
      objID = actor->GetBoneObject(actor->FindBoneName(i));
      if (objID != FAILED_ID) {
         tbCurScene->InsertObject(objID, aID, 0, i, actor->IsGeometryBone(i));
      }
   }

   // insert all character's skins into the object list
   numO = actor->GetSkinNumber();
   for (i = 0; i < numO; i++) {
      objID = actor->GetSkin(i);
      if (objID != FAILED_ID) {
         tbCurScene->InsertObject(objID, aID, 1, i, TRUE);
      }
   }

   // insert all character's attachments into the object list
   numO = actor->GetAttachmentNumber();
   for (i = 0; i < numO; i++) {
      objID = (OBJECTid) actor->GetAttachment(i);
      if (objID != FAILED_ID) {
         tbCurScene->InsertObject(objID, aID, 2, actor->GetAttachmentParentBoneID(i), TRUE);
      }
   }
}


/*---------------------------------------------
  TbScene member function : remove a character
  C.Wang 0408, 2013
 ----------------------------------------------*/
void TBScene::RemoveCharacter(CHARACTERid aID, BOOL4 beD)
{
   // scan the actor list
   int i, j, nObj;
   OBJECTid oID;
   FnCharacter actor;

   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         if (actorList[i]->ID() == aID) {
            // check if current object is in current character
            OBJECTid curObj = tbCurScene->CheckCurObjectInCurCharacter();
            if (curObj != FAILED_ID) {
               SetCurrentObject(FAILED_ID);
            }

            // remove the objects in the character
            actor.ID(aID);
            nObj = actor.GetBoneNumber();
            for (j = 0; j < nObj; j++) {
               oID = actor.GetBoneObject(j);
               RemoveObject(oID, FALSE, FALSE);
            }

            nObj = actor.GetSkinNumber();
            for (j = 0; j < nObj; j++) {
               oID = actor.GetSkin(j);
               RemoveObject(oID, FALSE, FALSE);
            }

            nObj = actor.GetAttachmentNumber();
            for (j = 0; j < nObj; j++) {
               oID = actor.GetAttachment(j);
               RemoveObject(oID, FALSE, FALSE);
            }

            if (beD) {
               DeleteCharacter(aID);
            }
            else {
               FnCharacter actor(aID);
               actor.DropToTrashCan(TRUE);
            }
            delete actorList[i];
            actorList[i] = NULL;
            if (i == numActor - 1) numActor--;
            return;
         }
      }
   }
}


/*-----------------------------------------
  TbScene member function : get the object
  C.Wang 0809, 2012
 ------------------------------------------*/
TBObject *TBScene::GetTBObject(OBJECTid oID)
{
   int i;
   for (i = 0; i < numObject; i++) {
      if (objList[i] != NULL) {
         if (objList[i]->ID() == oID) {
            return objList[i];
         }
      }
   }
   return NULL;
}


/*--------------------------------------------------
  TbScene member function : make the current object
  C.Wang 0610, 2013
 ---------------------------------------------------*/
void TBScene::SetCurrentObject(OBJECTid oID)
{
   // make no current object
   TBObject *curObj = GetCurrentTBObject();
   if (curObj != NULL) {
      curObj->ShowBoundingBox(FALSE);
   }

   // find the ID of current object
   int i;
   iCurObject = FY_NONE;
   for (i = 0; i < numObject; i++) {
      if (objList[i] != NULL) {
         if (objList[i]->ID() == oID) {
            iCurObject = i;
         }
      }
   }
   curObj = GetCurrentTBObject();
   if (curObj != NULL) {
      // show new current object's bounding box
      curObj->ShowBoundingBox(TRUE);
   }

   // update UIs
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   FnUiCombo uiC;
   FnUiTypein uiTy;

   tbTris = tbVs = 0;
   if (curObj != NULL && curObj->ID() != FAILED_ID) {
      // enable the save button
      uiB.ID(tbSaveModelButtonID);
      uiB.Enable(TRUE);

      // enable the delete button
      uiB.ID(tbDeleteModelButtonID);
      uiB.Enable(TRUE);

      uiC.ID(tbFindModelComboID);
      uiC.PasteComboValue(curObj->GetName());

      // enable Transform Gizmo Typein
      FnUiPlate ui_TF;   // UIs for Transform
      for (i = 0; i < tbNumTransformUI; i++) {
         ui_TF.ID(tbTransformUIs[i]);
         ui_TF.Enable(TRUE);
      }

      // paste all Transform data to current Transform UIs
      TbUpdateTransformUIData(TRUE);

      // enable the material combo
      uiC.ID(tbSelectMaterialButtonID);
      uiC.Enable(TRUE);

      // paste materials to combo
      int numMat = curObj->GetMaterialNumber();
      MATERIALid mID[1024];
      curObj->GetMaterials(mID);

      FnMaterial mat;
      mat.ID(mID[0]);
      uiC.PasteComboValue(mat.GetName());
      curMatID = mID[0];

      if (curMatID == FAILED_ID) {
         uiC.Enable(FALSE);
      }
      else {
         uiC.Enable(TRUE);
         uiC.ClearMessages();
         for (i = 0; i < numMat; i++) {
            mat.ID(mID[i]);
            uiC.InsertMessage(i, mat.GetName());
         }
      }

      uiRB.ID(tbWireframeCurModelButtonID);
      uiRB.Enable(TRUE);
      if (curObj->GetRenderMode() == WIREFRAME) {
         uiRB.ChangeState(TRUE);
      }
      else {
         uiRB.ChangeState(FALSE);
      }

      uiRB.ID(tbShowCurModelButtonID);
      uiRB.Enable(TRUE);
      if (curObj->GetVisibility()) {
         uiRB.ChangeState(TRUE);
      }
      else {
         uiRB.ChangeState(FALSE);
      }

      uiTy.ID(tbRGCurModelID);
      uiTy.Enable(TRUE);
      char rgValue[64];
      sprintf(rgValue, "%d", curObj->GetRenderGroup());
      uiTy.PasteTypeinValue(rgValue);

      uiRB.ID(tbAlphaCurModelButtonID);
      uiRB.Enable(TRUE);
      if (curObj->GetAlphaFlag()) {
         uiRB.ChangeState(TRUE);
      }
      else {
         uiRB.ChangeState(FALSE);
      }

      // paste all material data to current material UIs
      FnUiPlate ui;
      for (i = 0; i < tbNumMatUI; i++) {
         ui.ID(tbMatUI[i]);
         ui.Enable(TRUE);
      }

      ui.ID(tbUpdateMaterialID);
      ui.Enable(FALSE);

      mat.ID(mID[0]);
      TbPasteCurrentMaterialData(&mat);

      FnTexture tex(mat.GetTexture(0, 0));
      char *value = tex.GetPathName();
      char path[256], name[256];
      if (value != NULL) {
         // get file name & texture number
         FyCheckFileFormat(value, path, name, NULL);
         strcpy(tbTmpTexturePath, path);
      }
      else {
         strcpy(tbTmpTexturePath, FyGetTexturePath());
      }

      SHADERid shaderID;
      shaderID = mat.GetShaderEffect();
      if (shaderID != FAILED_ID) {
         FnShader shader(shaderID);
         value = shader.GetPath();
         if (value != NULL) {
            strcpy(tbTmpShaderPath, value);
         }
         else {
            strcpy(tbTmpShaderPath, FyGetShaderPath());
         }
      }
      else {
         strcpy(tbTmpShaderPath, FyGetShaderPath());
      }

      // find triangles and vertices
      GEOMETRYid geoID;
      FnGeometry geo;
      FnTriangle tri;
      int nT, nV, numGeo = curObj->GetGeometryNumber();
      nT = nV = 0;
      for (i = 0; i < numGeo; i++) {
         geoID = curObj->GetGeometryID(i);
         geo.ID(geoID);
         if (geo.Type() == INDEXED_TRIANGLES) {
            tri.ID(geoID);
            nT += tri.GetTriangleNumber();
            nV += tri.GetVertexNumber(NULL);
         }
      }
      tbTris = nT;
      tbVs = nV;
   }
   else {
      // disable the save button
      uiB.ID(tbSaveModelButtonID);
      uiB.Enable(FALSE);

      // disable the delete button
      uiB.ID(tbDeleteModelButtonID);
      uiB.Enable(FALSE);

      uiC.ID(tbFindModelComboID);
      uiC.PasteComboValue("");

      // disable the material combo
      uiC.ID(tbSelectMaterialButtonID);
      uiC.Enable(FALSE);
      curMatID = FAILED_ID;

      TbPasteCurrentMaterialData(NULL);

      // clear the combo data
      uiC.PasteComboValue("");
      uiC.ClearMessages();

      // adjust UIs
      uiRB.ID(tbWireframeCurModelButtonID);
      uiRB.ChangeState(FALSE);
      uiRB.Enable(FALSE);

      uiRB.ID(tbShowCurModelButtonID);
      uiRB.ChangeState(TRUE);
      uiRB.Enable(FALSE);

      uiTy.ID(tbRGCurModelID);
      uiTy.Enable(TRUE);
      uiTy.PasteTypeinValue("1");

      uiRB.ID(tbAlphaCurModelButtonID);
      uiRB.ChangeState(FALSE);
      uiRB.Enable(FALSE);

      FnUiPlate ui;
      for (i = 0; i < tbNumMatUI; i++) {
         ui.ID(tbMatUI[i]);
         ui.Enable(FALSE);

         if (ui.GetType() == TYPEIN_UI) {
            uiTy.ID(tbMatUI[i]);
            uiTy.PasteTypeinValue("");
         }
         else if (ui.GetType() == COMBO_UI) {
            uiC.ID(tbMatUI[i]);
            uiC.ClearMessages();
            uiC.PasteComboValue("");
         }
      }

      // disable Transform Gizmo Typein
      FnUiPlate ui_TF;   // UIs for Transform
      for (i = 0; i < tbNumTransformUI; i++) {
         ui_TF.ID(tbTransformUIs[i]);
         ui_TF.Enable(FALSE);
      }

      strcpy(tbTmpShaderPath, FyGetShaderPath());
      strcpy(tbTmpTexturePath, FyGetTexturePath());
   }

   // check how many objects
   int numModel = 0;
   for (i = 0; i < numObject; i++) {
      if (objList[i] != NULL) {
         numModel++;
      }
   }

   // update some UIs
   if (numModel > 0) {
      uiB.ID(tbPreviousModelButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbNextModelButtonID);
      uiB.Enable(TRUE);
      uiC.ID(tbFindModelComboID);
      uiC.Enable(TRUE);
   }
   else {
      uiB.ID(tbPreviousModelButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbNextModelButtonID);
      uiB.Enable(FALSE);
      uiC.ID(tbFindModelComboID);
      uiC.Enable(FALSE);
   }
}


/*-----------------------------------------------------------------
  TbScene member function : make next object as the current object
  C.Wang 1001, 2012
 ------------------------------------------------------------------*/
BOOL4 TBScene::NextCurrentObject()
{
   TBObject *curObj = GetCurrentTBObject();
   if (curObj != NULL) {
      curObj->ShowBoundingBox(FALSE);
   }

   // find the ID of next current object
   int i, iOne;
   if (iCurObject < 0) iCurObject = 0;
   for (i = 1; i < numObject; i++) {
      iOne = i + iCurObject;
      if (iOne >= numObject) iOne -= numObject;
      if (iOne < 0) iOne += numObject;
      if (objList[iOne] != NULL) {
         iCurObject = iOne;
         break;
      }
   }
   if (iCurObject >= numObject) return FALSE;

   SetCurrentObject(objList[iCurObject]->ID());
   return TRUE;
}


/*---------------------------------------------------------------------
  TbScene member function : make previous object as the current object
  C.Wang 1001, 2012
 ----------------------------------------------------------------------*/
BOOL4 TBScene::PreviousCurrentObject()
{
   TBObject *curObj = GetCurrentTBObject();
   if (curObj != NULL) {
      curObj->ShowBoundingBox(FALSE);
   }

   // find the ID of previous current object
   int i, iOne;
   if (iCurObject < 0) iCurObject = 0;
   for (i = 1; i < numObject; i++) {
      iOne = iCurObject - i;
      if (iOne >= numObject) iOne -= numObject;
      if (iOne < 0) iOne += numObject;
      if (objList[iOne] != NULL) {
         iCurObject = iOne;
         break;
      }
   }
   if (iCurObject >= numObject) return FALSE;

   SetCurrentObject(objList[iCurObject]->ID());
   return TRUE;
}


/*-----------------------------------------------------
  TbScene member function : make the current character
  C.Wang 0803, 2015
 ------------------------------------------------------*/
void TBScene::SetCurrentCharacter(CHARACTERid aID)
{
   // backup current actor
   TBCharacter *curActor = GetCurrentTBCharacter();
   CHARACTERid oldA = FAILED_ID;
   OBJECTid baseID = FAILED_ID;
   FnObject base;

   if (curActor != NULL) {
      oldA = curActor->ID();
      baseID = curActor->GetBaseObject();
   }
   base.ID(baseID);
   base.Show(FALSE);

   // find the ID of current character
   int i;
   iCurActor = FY_NONE;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         if (actorList[i]->ID() == aID) {
            iCurActor = i;
         }
      }
   }
   curActor = GetCurrentTBCharacter();
   
   // update UIs
   FnUiButton uiB;
   FnUiCombo uiC;
   FnUiTypein uiTy;
   if (curActor != NULL && curActor->ID() != FAILED_ID) {
      // enable the save button
      uiB.ID(tbSaveCharacterButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbExportCharacterModelButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbLoadMotionButtonID);
      uiB.Enable(TRUE);

      uiTy.ID(tbSetLocalScaleID);
      uiTy.Enable(TRUE);

      char value[256];
      float scaleFactor = curActor->GetLocalScale();
      sprintf(value, "%6.3f", scaleFactor);
      uiTy.PasteTypeinValue(value);

      // enable the delete button
      uiB.ID(tbDeleteCharacterButtonID);
      uiB.Enable(TRUE);

      // paste the character to selection combo
      uiC.ID(tbSelectCharacterComboID);
      uiC.PasteComboValue(curActor->GetName());

      // show/hide the base node
      baseID = curActor->GetBaseObject();
      base.ID(baseID);
      base.Show(tbShowAux);
   }
   else {
      // disable the save button
      uiB.ID(tbSaveCharacterButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbExportCharacterModelButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbLoadMotionButtonID);
      uiB.Enable(FALSE);
      uiTy.ID(tbSetLocalScaleID);
      uiTy.Enable(FALSE);

      // disable the delete button
      uiB.ID(tbDeleteCharacterButtonID);
      uiB.Enable(FALSE);

      // paste empty to selection combos
      uiC.ID(tbSelectCharacterComboID);
      uiC.PasteComboValue("");

      // check current model is part of former current character
      TBObject *curObj = GetCurrentTBObject();
      if (curObj != NULL && curObj->UsedBy() == oldA) {
         SetCurrentObject(FAILED_ID);
      }
   }
}


/*---------------------------
  find scene center position
  C.Wang 1203, 2012
 ----------------------------*/
void TBScene::FindCenter(float *pos, float *radius)
{
   int i, count;
   float center[3], c[3], r, rMax;
   
   count = 0;
   rMax = 0.0f;
   center[0] = center[1] = center[2] = 0.0f;
   for (i = 0; i < numObject; i++) {
      if (objList[i] != NULL) {
         objList[i]->GetBoundingVolume(NULL, NULL, 0, c, &r);
         center[0] += c[0];
         center[1] += c[1];
         center[2] += c[2];
         if (rMax < r) {
            rMax = r;
         }
         count++;
      }
   }

   if (pos != NULL && count != 0) {
      pos[0] = center[0]/count;
      pos[1] = center[1]/count;
      pos[2] = center[2]/count;
   }

   if (radius != NULL) {
      *radius = rMax;
   }
}


/*--------------------------------------------------------
  TbScene member function : get number of game fx systems
  C.Wang 1002, 2012
 ---------------------------------------------------------*/
int TBScene::CheckEffectGameFXSystemNumber()
{
   int i, count = 0;
   for (i = 0; i < numFXS; i++) {
      if (fxsList[i] != NULL) {
         count++;
      }
   }
   return count;
}


/*-------------------------------------------------
  TbScene member function : get the game fx system
  C.Wang 1002, 2012
 --------------------------------------------------*/
TBGameFXSystem *TBScene::GetTBGameFXSystem(int iOne)
{
   int i, count = 0;
   for (i = 0; i < numFXS; i++) {
      if (fxsList[i] != NULL) {
         if (count == iOne) {
            return fxsList[i];
         }
         count++;
      }
   }
   return NULL;
}


/*---------------------------------------------------------
  TbScene member function : get the game fx system by name
  C.Wang 1004, 2012
 ----------------------------------------------------------*/
TBGameFXSystem *TBScene::GetTBGameFXSystem(char *name)
{
   int i;
   for (i = 0; i < numFXS; i++) {
      if (fxsList[i] != NULL) {
         if (FyCheckWord(fxsList[i]->GetName(), name)) {
            return fxsList[i];
         }
      }
   }
   return NULL;
}


/*------------------------------------------------------------
  TbScene member function : get current game FX for the scene
  C.Wang 1002, 2012
 -------------------------------------------------------------*/
TBGameFXSystem *TBScene::GetCurrentTBGameFXSystem()
{
   if (iCurFXS < 0 || iCurFXS >= numFXS) return NULL;
   if (fxsList[iCurFXS] == NULL) return NULL;
   return fxsList[iCurFXS];
}


/*------------------------------------------------------------
  TbScene member function : set current game FX for the scene
  C.Wang 1002, 2012
 -------------------------------------------------------------*/
void TBScene::SetCurrentGameFXSystem(GAMEFX_SYSTEMid gxID)
{
   TBGameFXSystem *curFX = NULL;

   // find the ID of current game FX system
   int i;
   iCurFXS = FY_NONE;

   for (i = 0; i < numFXS; i++) {
      if (fxsList[i] != NULL) {
         if (fxsList[i]->ID() == gxID) {
            iCurFXS = i;
         }
      }
   }
}


/*--------------------------------------------------
  TbScene member function : insert a game FX system
  C.Wang 1002, 2012
 ---------------------------------------------------*/
void TBScene::InsertGameFXSystem(GAMEFX_SYSTEMid gxID)
{
   int i;
   if (fxsList == NULL) {
      maxFXS = 256;
      fxsList = (TBGameFXSystem **)FyMalloc(sizeof(TBGameFXSystem *)*maxFXS);
      for (i = 0; i < maxFXS; i++) {
         fxsList[i] = NULL;
      }
      numFXS = 0;
   }
   else {
      if (numFXS + 1 >= maxFXS) {
         maxFXS += 128;
         fxsList = (TBGameFXSystem **)FyRealloc(fxsList, sizeof(TBGameFXSystem *)*maxFXS);
         for (i = numFXS; i < maxFXS; i++) {
            fxsList[i] = NULL;
         }
      }
   }

   // scan the game FX system list
   for (i = 0; i < numFXS; i++) {
      if (fxsList[i] == NULL) {
         fxsList[i] = new TBGameFXSystem();
         fxsList[i]->ID(gxID);
         return;
      }
   }
   fxsList[numFXS] = new TBGameFXSystem();
   fxsList[numFXS]->ID(gxID);
   numFXS++;
}


/*--------------------------------------------------
  TbScene member function : remove a game FX system
  C.Wang 1231, 2012
 ---------------------------------------------------*/
void TBScene::RemoveGameFXSystem(GAMEFX_SYSTEMid gxID, BOOL4 beD)
{
   // scan the game FX system list
   int i;
   for (i = 0; i < numFXS; i++) {
      if (fxsList[i] != NULL) {
         if (fxsList[i]->ID() == gxID) {
            if (beD) {
               DeleteGameFXSystem(gxID);
            }
            else {
               FnGameFXSystem gx(gxID);
               gx.DropToTrashCan(TRUE);
            }
            delete fxsList[i];
            fxsList[i] = NULL;
            if (i == numFXS - 1) numFXS--;
            if (iCurFXS == i) iCurFXS = FY_NONE;
            return;
         }
      }
   }
}


// quit command
class TbQuitCommand : public FcCommandBase
{
public:
   virtual void Init() { BeUndo(FALSE);
                         SetCommandName("Quit"); };                // initialize the command
   virtual void DeInit() {};                                       // deinitialize the command
   virtual BOOL4 Do(char *arg) { TbOpenQuit(tbExitButtonID, FALSE);
                                return TRUE; };                    // the "quit" command
   virtual void Undo() {};                                         // no undo
};


// new command
class TbNewCommand : public FcCommandBase
{
public:
   virtual void Init() { BeUndo(FALSE);
                         SetCommandName("New"); };                 // initialize the command
   virtual void DeInit() {};                                       // deinitialize the command
   virtual BOOL4 Do(char *arg) { TbOpenNew(tbNewButtonID, FALSE);
                                return TRUE; };                    // the "new" command
   virtual void Undo() {};                                         // no undo
};


// set Gizmo transform command
class TbSetTransformCommand : public FcCommandBase
{
protected:
   OBJECTid m_backupObjID;
   float    m_backupM12[12];
public:
   virtual void Init() { m_backupM12[0] = m_backupM12[1]  = m_backupM12[2]  = 0.0f;
                         m_backupM12[3] = m_backupM12[4]  = m_backupM12[5]  = 0.0f;
                         m_backupM12[6] = m_backupM12[7]  = m_backupM12[8]  = 0.0f;
                         m_backupM12[9] = m_backupM12[10] = m_backupM12[11] = 0.0f;
                         SetCommandName("SetTransform"); }; // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the command
   virtual void Undo();                                     // undo
};


      /*---------------------------
        set transform command - Do
        Eric Fei 1218, 2012
       ----------------------------*/
      BOOL4 TbSetTransformCommand::Do(char *arg)
      {
         // get current Object
         m_backupObjID = TbGetCurrentBaseObjID();
         if (m_backupObjID == FAILED_ID) return FALSE;
         FnObject curObj(m_backupObjID);

         // backup transform information
         curObj.GetMatrix(m_backupM12);

         // extract the position & rotate
         char *key, argument[256];
         float arguValue[6];
         BOOL4 bHasPos = FALSE;
         BOOL4 bHasRot = FALSE;
         int i, idx = 0;
         for (i = 0; i < 6; i++) arguValue[i] = 0.0f;

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-pos")) {
               idx = 0;
               key = strtok(NULL, " ");
               while ((key != NULL) && (idx < 3)) {
                  if (FyCheckWord(key, "-rot")) break;
                  arguValue[idx] = (float) atof(key);
                  key = strtok(NULL, " ");
                  idx ++;
                  bHasPos = TRUE;
               }
            }
            else if (FyCheckWord(key, "-rot")) {
               idx = 3;
               key = strtok(NULL, " ");
               while ((key != NULL) && (idx < 6)) {
                  if (FyCheckWord(key, "-pos")) break;
                  arguValue[idx] = (float) atof(key);
                  key = strtok(NULL, " ");
                  idx ++;
                  bHasRot = TRUE;
               }
            }
            else {
               key = strtok(NULL, " ");
            }
         }

         // get the transform from typein
         float newM12[12], curM12[12];
         curObj.GetMatrix(curM12);
         FyFormM12FromEulerAnglesXYZ(newM12, arguValue[3], arguValue[4], arguValue[5]);
         newM12[9]  = arguValue[0];
         newM12[10] = arguValue[1];
         newM12[11] = arguValue[2];
         if (!bHasPos) {  // recover current position
            newM12[9]  = curM12[9];
            newM12[10] = curM12[10];
            newM12[11] = curM12[11];
         }
         if (!bHasRot) {  // recover current rotation
            newM12[0] = curM12[0];
            newM12[1] = curM12[1];
            newM12[2] = curM12[2];
            newM12[3] = curM12[3];
            newM12[4] = curM12[4];
            newM12[5] = curM12[5];
            newM12[6] = curM12[6];
            newM12[7] = curM12[7];
            newM12[8] = curM12[8];
         }
         
         // update Typein UI
         TbPasteCurrentTransformData(newM12);

         // set new transform
         curObj.SetMatrix(newM12, REPLACE);

         return TRUE;
      }
      

      /*-----------------------------
        set transform command - UnDo
        Eric Fei 1218, 2012
       ------------------------------*/
      void TbSetTransformCommand::Undo()
      {
         if (m_backupObjID == FAILED_ID) return;

         // Change UIs for old Object
         tbCurScene->SetCurrentObject(m_backupObjID);

         // undo old transform
         FnObject curObj(m_backupObjID);
         curObj.SetMatrix(m_backupM12, REPLACE);
         
         // undo Typein
         TbPasteCurrentTransformData(m_backupM12);
      }


// set wireframe command
class TbSetWireframeCommand : public FcCommandBase
{
protected:
   OBJECTid objID;
   int lastState;

public:
   virtual void Init() { lastState = SOLID; 
                         objID = FAILED_ID;
                         SetCommandName("WireFrame"); };    // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the command
   virtual void Undo();                                     // undo
};


      /*---------------------------
        set wireframe command - Do
        C.Wang 0809, 2012
       ----------------------------*/
      BOOL4 TbSetWireframeCommand::Do(char *arg)
      {
         objID = tbCurScene->GetCurrentObject();

         FnObject model;
         model.ID(objID);
         lastState = model.GetRenderMode();

         if (arg != NULL) {
            char string[256];
            strcpy(string, arg);
            if (TbCheckCommandAttributeB(string, "-OFF", NULL, 0)) {
               model.SetRenderMode(SOLID);
            }
            else {
               model.SetRenderMode(WIREFRAME);
            }
         }
         else {
            model.SetRenderMode(WIREFRAME);
         }

         return TRUE;
      }


      /*-----------------------------
        set wireframe command - UnDo
        C.Wang 0809, 2012
       ------------------------------*/
      void TbSetWireframeCommand::Undo()
      {
         if (objID == FAILED_ID) return;

         FnObject model;
         model.ID(objID);
         model.SetRenderMode(lastState);
      }


// set render group command
class TbSetRenderGroupCommand : public FcCommandBase
{
protected:
   OBJECTid objID;
   int lastRG;

public:
   virtual void Init() { lastRG = FY_NONE; 
                         objID = FAILED_ID;
                         SetCommandName("SetRenderGroup"); };  // initialize the command
   virtual void DeInit() { };                                  // deinitialize the command
   virtual BOOL4 Do(char *arg);                                // the command
   virtual void Undo();                                        // undo
};


      /*------------------------------
        set render group command - Do
        C.Wang 0607, 2013
       -------------------------------*/
      BOOL4 TbSetRenderGroupCommand::Do(char *arg)
      {
         objID = tbCurScene->GetCurrentObject();

         FnObject model;
         model.ID(objID);
         lastRG = model.GetRenderGroup();

         int rg = 0;
         if (arg != NULL) {
            rg = atoi(arg);
            if (rg < 0) rg = 0;
            if (rg > 10000) rg = 0;
         }
         model.ChangeRenderGroup(rg);

         return TRUE;
      }


      /*--------------------------------
        set render group command - UnDo
        C.Wang 0607, 2013
       ---------------------------------*/
      void TbSetRenderGroupCommand::Undo()
      {
         if (objID == FAILED_ID) return;

         FnObject model;
         model.ID(objID);
         model.ChangeRenderGroup(lastRG);
      }


// set alpha command
class TbSetAlphaCommand : public FcCommandBase
{
protected:
   OBJECTid objID;
   BOOL4 lastState;

public:
   virtual void Init() { lastState = FALSE; 
                         objID = FAILED_ID;
                         SetCommandName("Alpha"); };        // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the command
   virtual void Undo();                                     // undo
};


      /*-----------------------
        set alpha command - Do
        C.Wang 1209, 2012
       ------------------------*/
      BOOL4 TbSetAlphaCommand::Do(char *arg)
      {
         objID = tbCurScene->GetCurrentObject();

         FnObject model;
         model.ID(objID);
         lastState = model.GetAlphaFlag();

         if (arg != NULL) {
            char string[256];
            strcpy(string, arg);
            if (TbCheckCommandAttributeB(string, "-OFF", NULL, 0)) {
               model.SetAlphaFlag(FALSE);
            }
            else {
               model.SetAlphaFlag(TRUE);
            }
         }
         else {
            model.SetAlphaFlag(TRUE);
         }

         return TRUE;
      }


      /*-------------------------
        set alpha command - UnDo
        C.Wang 1209, 2012
       --------------------------*/
      void TbSetAlphaCommand::Undo()
      {
         if (objID == FAILED_ID) return;

         FnObject model;
         model.ID(objID);
         model.SetAlphaFlag(lastState);
      }


// set show command
class TbSetShowCommand : public FcCommandBase
{
protected:
   OBJECTid objID;
   BOOL4 lastState;

public:
   virtual void Init() { lastState = FALSE; 
                         objID = FAILED_ID;
                         SetCommandName("Show"); };         // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the command
   virtual void Undo();                                     // undo
};


      /*-----------------------
        set show command - Do
        C.Wang 0607, 2013
       ------------------------*/
      BOOL4 TbSetShowCommand::Do(char *arg)
      {
         objID = tbCurScene->GetCurrentObject();

         FnObject model;
         model.ID(objID);
         lastState = model.GetVisibility();

         if (arg != NULL) {
            char string[256];
            strcpy(string, arg);
            if (TbCheckCommandAttributeB(string, "-OFF", NULL, 0)) {
               model.Show(FALSE);
            }
            else {
               model.Show(TRUE);
            }
         }
         else {
            model.Show(TRUE);
         }

         return TRUE;
      }


      /*-------------------------
        set show command - UnDo
        C.Wang 0607, 2013
       --------------------------*/
      void TbSetShowCommand::Undo()
      {
         if (objID == FAILED_ID) return;

         FnObject model;
         model.ID(objID);
         model.Show(lastState);
      }


// rename current material command
class TbRenameMaterialCommand : public FcCommandBase
{
protected:
   OBJECTid oID;
   MATERIALid matID;
   char name[256];

public:
   virtual void Init() { name[0] = '\0'; 
                         oID = FAILED_ID;
                         matID = FAILED_ID;
                         SetCommandName("RenameMaterial"); };    // initialize the command
   virtual void DeInit() { };                                    // deinitialize the command
   virtual BOOL4 Do(char *arg);                                  // the command
   virtual void Undo();                                          // undo
};


      /*-------------------------------------
        rename current material command - Do
        C.Wang 0809, 2012
       --------------------------------------*/
      BOOL4 TbRenameMaterialCommand::Do(char *arg)
      {
         oID = tbCurScene->GetCurrentObject();
         matID = tbCurScene->GetCurrentMaterial();

         FnMaterial mat;
         mat.ID(matID);
         strcpy(name, mat.GetName());
         mat.SetName(arg);

         // update UIs
         int i, numMat;
         MATERIALid mID[1024];
         FnUiCombo uiC;
         FnObject model;
         model.ID(oID);

         uiC.ID(tbSelectMaterialButtonID);
         uiC.PasteComboValue(mat.GetName());
         uiC.ClearMessages();
         numMat = model.GetMaterialNumber();
         model.GetMaterials(mID);
         for (i = 0; i < numMat; i++) {
            mat.ID(mID[i]);
            uiC.InsertMessage(i, mat.GetName());
         }

         return TRUE;
      }


      /*---------------------------------------
        rename current material command - UnDo
        C.Wang 0809, 2012
       ----------------------------------------*/
      void TbRenameMaterialCommand::Undo()
      {
         FnMaterial mat;
         mat.ID(matID);
         mat.SetName(name);

         // update UIs
         int i, numMat;
         MATERIALid mID[1024];
         FnUiCombo uiC;
         FnObject model;
         model.ID(oID);

         uiC.ID(tbSelectMaterialButtonID);
         uiC.PasteComboValue(mat.GetName());
         uiC.ClearMessages();
         numMat = model.GetMaterialNumber();
         model.GetMaterials(mID);
         for (i = 0; i < numMat; i++) {
            mat.ID(mID[i]);
            uiC.InsertMessage(i, mat.GetName());
         }
      }


// set current material command
class TbSetMaterialCommand : public FcCommandBase
{
protected:
   MATERIALid matID;

public:
   virtual void Init() { matID = FAILED_ID;
                         SetCommandName("SetMaterial"); };       // initialize the command
   virtual void DeInit() { };                                    // deinitialize the command
   virtual BOOL4 Do(char *arg);                                  // the command
   virtual void Undo();                                          // undo
};


      /*----------------------------------
        set current material command - Do
        C.Wang 0809, 2012
       -----------------------------------*/
      BOOL4 TbSetMaterialCommand::Do(char *arg)
      {
         // backup current material
         matID = tbCurScene->GetCurrentMaterial();

         // get new material
         OBJECTid oID = tbCurScene->GetCurrentObject();
         MATERIALid mID[1024];
         FnMaterial mat;
         FnUiCombo uiC;
         FnUiTypein uiTy;
         FnObject model;

         model.ID(oID);

         int i, numMat = model.GetMaterialNumber();
         model.GetMaterials(mID);
         for (i = 0; i < numMat; i++) {
            mat.ID(mID[i]);
            if (FyCheckWord(mat.GetName(), arg)) {
               break;
            }
         }
         if (i == numMat) {
            return FALSE;
         }

         // set current material
         tbCurScene->SetCurrentMaterial(mID[i]);

         // paste the material name to typein
         uiC.ID(tbSelectMaterialButtonID);
         uiC.PasteComboValue(mat.GetName());

         // paste all material data to current material UIs
         FnUiPlate ui;
         for (i = 0; i < tbNumMatUI; i++) {
            ui.ID(tbMatUI[i]);
            ui.Enable(TRUE);
         }
         ui.ID(tbUpdateMaterialID);
         ui.Enable(FALSE);

         TbPasteCurrentMaterialData(&mat);
         return TRUE;
      }


      /*------------------------------------
        set current material command - UnDo
        C.Wang 0809, 2012
       -------------------------------------*/
      void TbSetMaterialCommand::Undo()
      {
         tbCurScene->SetCurrentMaterial(matID);

         // paste the material name to typein
         FnMaterial mat;
         FnUiCombo uiC;
         mat.ID(matID);
         uiC.ID(tbSelectMaterialButtonID);
         uiC.PasteComboValue(mat.GetName());

         // paste all material data to current material UIs
         TbPasteCurrentMaterialData(&mat);
      }


// update current material command
class TbUpdateMaterialCommand : public FcCommandBase
{
protected:
   TBMaterial matData;

public:
   virtual void Init() { memset(&matData, 0, sizeof(TBMaterial));
                         SetCommandName("UpdateMaterial"); };    // initialize the command
   virtual void DeInit() { };                                    // deinitialize the command
   virtual BOOL4 Do(char *arg);                                  // the command
   virtual void Undo();                                          // undo
};


      /*-------------------------------------
        update current material command - Do
        C.Wang 0809, 2012
       --------------------------------------*/
      BOOL4 TbUpdateMaterialCommand::Do(char *arg)
      {
         // backup current material data
         memcpy(&matData, &tbMatBuffer, sizeof(TBMaterial));

         // extract all data from UIs and update to tbMatBuffer
         TbExtractMaterialUIData();

         // update current material
         FnMaterial mat;
         mat.ID(tbCurScene->GetCurrentMaterial());
         TbUpdateCurrentMaterial(&mat);

         // paste new data back to UI
         TbPasteCurrentMaterialData(&mat);

         return TRUE;
      }


      /*---------------------------------------
        update current material command - UnDo
        C.Wang 0809, 2012
       ----------------------------------------*/
      void TbUpdateMaterialCommand::Undo()
      {
         // copy the data from backup
         memcpy(&tbMatBuffer, &matData, sizeof(TBMaterial));

         // update current material
         FnMaterial mat;
         mat.ID(tbCurScene->GetCurrentMaterial());
         TbUpdateCurrentMaterial(&mat);

         // paste new data back to UI
         TbPasteCurrentMaterialData(&mat);

         // recover the UIs
         TbCancelMaterialEditing();
      }


// load model command
class TbLoadModelCommand : public FcCommandBase
{
protected:
   int numO;
   OBJECTid *backupObjID;
   OBJECTid lastObj;
   int numChar;
   CHARACTERid *backupChar;

public:
   virtual void Init() { numO = 0; 
                         backupObjID = NULL;
                         numChar = 0;
                         backupChar = NULL;
                         lastObj = FAILED_ID;
                         SetCommandName("Model"); };    // initialize the command
   virtual void DeInit() { if (backupObjID != NULL) {
                              FyFree(backupObjID); backupObjID = NULL;
                           };
                           if (backupChar != NULL) {
                              FyFree(backupChar); backupChar = NULL;
                           }
                         };                             // deinitialize the command
   virtual BOOL4 Do(char *arg);                         // the command
   virtual void Undo();                                 // undo
};


      /*-------------------------
        load models command - Do
        C.Wang 1207, 2012
       --------------------------*/
      BOOL4 TbLoadModelCommand::Do(char *arg)
      {
         // open the file browser for loading an object file
         int i;
         OPENFILENAME ofn;
         char openName[256], dir[256], log[256];
         BOOL4 beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(openName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            openName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Fly2 Model CW3 {*.cw3}\0*.cw3\0" \
                              "Fly2 Model in Binary CW4 {*.cw4}\0*.cw4\0" \
                              "Fly2 Model in Maya OBJ {*.obj}\0*.obj\0" \
                              "Fly2 Scene in ASCII CWN {*.cwn}\0*.cwn\0" \
                              "Fly2 Scene in Binary CW4 {*.cw4}\0*.cw4\0" \
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = openName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cw3";
            ofn.lpstrInitialDir = dir;

            // start to load the model file
            if (GetOpenFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            // check file format
            char path[256], name[256], ext[64];
            int format = FyCheckFileFormat(openName, path, name, ext);
            DWORD type;
            if (format == CW3) {
               type = 0;       // a CW3 model
            }
            else if (format == CWN) {
               type = 1;       // a scene
            }
            else if (format == WOBJ) {  // an OBJ model
               type = 4;
            }
            else {
               DWORD cw4Type = FyCheckCW4FileFormat(openName);
               if (cw4Type == CW4_MODEL) {
                  type = 0;
               }
               else if (cw4Type == CW4_SCENE) {
                  type = 2;
               }
               else {
                  type = 3;
               }
            }

            // set current rendering group
            //tbScene.SetCurrentRenderGroup(4);

            // create the object and load the file
            if (type == 0 || type == 4) {    // a single model file in CW3/CW4/OBJ
               FnObject model;
               OBJECTid newObj = FAILED_ID;
               //GEOMETRYid gID;
               newObj = tbScene.CreateObject();
               model.ID(newObj);
               if (model.Load(openName, TRUE)) {
                  // insert the new object into the object list
                  tbCurScene->InsertObject(newObj, FAILED_ID, FY_NONE, FY_NONE, TRUE);

                  // make the object as current one
                  lastObj = tbCurScene->GetCurrentObject();
                  tbCurScene->SetCurrentObject(newObj);

                  //model.GenerateTangentVectors(1);

                  // restore the extra system directory
                  FySetDataPath(".");

                  // echo the log
                  sprintf(log, "Load model %s OK", openName);
                  TbSetLogMessage(log);

                  // backup the model
                  numO = 1;
                  backupObjID = (OBJECTid *)FyMalloc(sizeof(OBJECTid)*numO);
                  backupObjID[0] = newObj;
                  beOK = TRUE;
               }
               else {
                  TbSetLogMessage("No models loaded");
                  return FALSE;
               }
            }
            else if (type == 1 || type == 2) {
               // load a scene
               int numObjInFile = tbScene.QueryObjectNumberInFile(openName);
               if (numObjInFile <= 0) {
                  // load nothing
                  TbSetLogMessage("No models loaded");
                  return FALSE;
               }
               else {
                  numO = numObjInFile;
                  backupObjID = (OBJECTid *)FyMalloc(sizeof(OBJECTid)*numO);
                  numChar = tbScene.QueryCharacterNumberInFile(openName);
                  if (numChar > 0) {
                     backupChar = (CHARACTERid *)FyMalloc(sizeof(CHARACTERid)*numChar);
                  }

                  // load the scene file
                  beOK = tbScene.Load(openName, backupObjID, numO, backupChar, numChar);

                  if (!beOK) {
                     FyFree(backupObjID);
                     backupObjID = NULL;
                     FyFree(backupChar);
                     backupChar = NULL;

                     TbSetLogMessage("No models/scenes loaded");
                     numO = 0;
                     return FALSE;
                  }
                  
                  // insert the objects to current scene
                  FnObject model;
                  for (i = 0; i < numO; i++) {
                     model.ID(backupObjID[i]);
                     model.SetRenderOption(SPECULAR, TRUE);

                     // insert the new object into the object list
                     tbCurScene->InsertObject(backupObjID[i], FAILED_ID, FY_NONE, FY_NONE, TRUE);                     
                  }

                  // insert the characters to current scene
                  int j;
                  FnCharacter actor;
                  for (j = 0; j < numChar; j++) {
                     FnObject model;
                     OBJECTid base = actor.GetBaseObject();
                     model.ID(base);
                     float color[3];
                     color[0] = 0.0f; color[1] = color[2] = 1.0f;
                     TbDrawBlueBox(&model, 5.0f, color);
                     model.Show(TRUE);

                     // insert the character to database & make it as the current character
                     tbCurScene->InsertCharacter(backupChar[j]);
                  }
                  if (numChar > 0) {
                     // update UIs
                     int numA = tbCurScene->GetEffectCharacterNumber();
                     int iCount;
                     FnUiCombo uiC;
                     uiC.ID(tbSelectCharacterComboID);
                     uiC.ClearMessages();
                     if (numA > 0) {
                        char *name;
                        uiC.Enable(TRUE);
                        iCount = 0;
                        numA = tbCurScene->GetCharacterNumber();
                        for (i = 0; i < numA; i++) {
                           name = tbCurScene->GetCharacterName(i);
                           if (name != NULL) {
                              uiC.InsertMessage(iCount, name);
                              iCount++;
                           }
                        }
                     }
                  }

                  // make the object as current one
                  lastObj = tbCurScene->GetCurrentObject();
                  tbCurScene->SetCurrentObject(backupObjID[0]);

                  // reset the animation of the scene
                  tbCurScene->ResetPlay(TRUE);
               }
            }
            else {
               // load nothing
               TbSetLogMessage("No models loaded");
               return FALSE;
            }
            
            // check play control panel

            // check current model
            OBJECTid curObj = tbCurScene->GetCurrentObject();
            FnUiRadioButton uiRB;
            FnObject model;
            model.ID(curObj);

            uiRB.ID(tbWireframeCurModelButtonID);
            if (curObj != FAILED_ID) {
               uiRB.Enable(TRUE);
               if (model.GetRenderMode() == WIREFRAME) {
                  uiRB.ChangeState(TRUE);
               }
               else {
                  uiRB.ChangeState(FALSE);
               }
            }
            else {
               uiRB.Enable(FALSE);
            }

            uiRB.ID(tbShowCurModelButtonID);
            if (curObj != FAILED_ID) {
               uiRB.Enable(TRUE);
               if (model.GetVisibility()) {
                  uiRB.ChangeState(TRUE);
               }
               else {
                  uiRB.ChangeState(FALSE);
               }
            }
            else {
               uiRB.Enable(FALSE);
            }

            FnUiTypein uiTy;
            uiTy.ID(tbRGCurModelID);
            if (curObj != FAILED_ID) {
               uiTy.Enable(TRUE);
               char rgValue[64];
               sprintf(rgValue, "%d", model.GetRenderGroup());
               uiTy.PasteTypeinValue(rgValue);
            }
            else {
               uiTy.Enable(FALSE);
            }

            uiRB.ID(tbAlphaCurModelButtonID);
            if (curObj != FAILED_ID) {
               uiRB.Enable(TRUE);
               if (model.GetAlphaFlag()) {
                  uiRB.ChangeState(TRUE);
               }
               else {
                  uiRB.ChangeState(FALSE);
               }
            }
            else {
               uiRB.Enable(FALSE);
            }

            // update working directory
            GetCurrentDirectory(256, tbWorkDir);
            uiTy.ID(tbWorkDirTypeinUI);
            uiTy.PasteTypeinValue(tbWorkDir);

            // check cameras
            FnUiCombo uiC(tbSelectCameraID);
            int numC = tbCurScene->GetCameraNumber();
            OBJECTid *cList;

            if (numC > 0) {
               FnObject obj;
               cList = (OBJECTid *)FyAllocateBuffer(sizeof(OBJECTid)*numC);
               tbCurScene->GetCameras(cList, numC);
               uiC.ClearMessages();
               for (i = 0; i < numC; i++) {
                  obj.ID(cList[i]);
                  uiC.InsertMessage(i, obj.GetName());
               }
               uiC.PasteComboValue("MainCam");
               tbRenderCamID = tbMainCamID;
               FnCamera camera;
               float aspect;
               camera.ID(tbMainCamID);
               aspect = camera.GetAspectRatio();
               tbWorkSpaceVPWidth = (int)(aspect*tbWorkSpaceVPHeight);
               tbWorkSpaceVP.SetSize(tbWorkSpaceVPWidth, tbWorkSpaceVPHeight);
               tbBeEditCam = TRUE;
               FyReleaseBuffer(cList);
            }

            // set standard cursor
            SetCursor(tbArrowCursor);
         }
         else {
            TbSetLogMessage("No models loaded");
            beOK = FALSE;
         }
         return beOK;
      }

      /*--------------------------
        load model command - UnDo
        C.Wang 0204, 2013
      ---------------------------*/
      void TbLoadModelCommand::Undo()
      {
         // delete the models
         if (backupObjID != NULL) {
            int i;
            for (i = 0; i < numO; i++) {
               tbCurScene->RemoveObject(backupObjID[i], TRUE);
               backupObjID[i] = FAILED_ID;
            }
            FyFree(backupObjID);
            backupObjID = NULL;
            numO = 0;

            // check cameras
            FnUiCombo uiC(tbSelectCameraID);
            int numC = tbCurScene->GetCameraNumber();
            OBJECTid *cList;

            if (numC > 0) {
               FnObject obj;
               cList = (OBJECTid *)FyAllocateBuffer(sizeof(OBJECTid)*numC);
               tbCurScene->GetCameras(cList, numC);
               uiC.ClearMessages();
               for (i = 0; i < numC; i++) {
                  obj.ID(cList[i]);
                  uiC.InsertMessage(i, obj.GetName());
               }
               uiC.PasteComboValue("MainCam");
               FnCamera camera;
               float aspect;
               camera.ID(tbMainCamID);
               aspect = camera.GetAspectRatio();
               tbWorkSpaceVPWidth = (int)(aspect*tbWorkSpaceVPHeight);
               tbWorkSpaceVP.SetSize(tbWorkSpaceVPWidth, tbWorkSpaceVPHeight);
               tbRenderCamID = tbMainCamID;
               tbBeEditCam = TRUE;
               FyReleaseBuffer(cList);
            }
         }

         // delete the characters
         if (backupChar != NULL) {
            int i;
            for (i = 0; i < numChar; i++) {
               tbCurScene->RemoveCharacter(backupChar[i], TRUE);
               backupChar[i] = FAILED_ID;
            }
            FyFree(backupChar);
            backupChar = NULL;
            numChar = 0;
         }
         tbCurScene->SetCurrentObject(lastObj);
      }


// save model command
class TbSaveModelCommand : public FcCommandBase
{
public:
   virtual void Init() { SetCommandName("SaveModel"); };    // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the command
   virtual void Undo() { };                                 // undo
};


      /*------------------------
        save model command - Do
        C.Wang 0809, 2012
       -------------------------*/
      BOOL4 TbSaveModelCommand::Do(char *arg)
      {
         // open the file browser for saving an object file
         OPENFILENAME ofn;
         char saveName[256], dir[256], log[256];
         BOOL4 beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(saveName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            saveName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Fly2 Model CW3 {*.cw3}\0*.cw3\0"\
                              "Fly2 Model in Binary CW4 {*.cw4}\0*.cw4\0"\
                              "Fly2 Model in Maya OBJ {*.obj}\0*.obj\0" \
                              "Fly2 Scene in Binary CW4 {*.cw4}\0*.cw4\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = saveName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cw3";
            ofn.lpstrInitialDir = dir;

            // get the model file name
            if (GetSaveFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            if (ofn.nFilterIndex == 4) {
               // save the scene in CW4
               tbCurScene->Save(saveName, TRUE, TRUE, tbExclusiveSaveList, tbNumExclusiveSave);
               strcpy(log, "The scene is saved.");
               TbSetLogMessage(log);
            }
            else {
               OBJECTid oID = tbCurScene->GetCurrentObject();
               if (oID != FAILED_ID) {
                  char path[256], name[256], ext[64];
                  FnObject model;
                  model.ID(oID);

                  // check file format
                  FyCheckFileFormat(saveName, path, name, ext);
                  model.Save(saveName);
                  sprintf(log, "The %s model is saved.", model.GetName());
                  TbSetLogMessage(log);
               }
               else {
                  strcpy(log, "Nothing to save");
                  TbSetLogMessage(log);
               }
            }
      
            // set standard cursor
            SetCursor(tbArrowCursor);

            // update working directory
            GetCurrentDirectory(256, tbWorkDir);
            FnUiTypein uiTy;
            uiTy.ID(tbWorkDirTypeinUI);
            uiTy.PasteTypeinValue(tbWorkDir);
         }
         else {
            strcpy(log, "Nothing to save");
            TbSetLogMessage(log);
         }

         return beOK;
      }


// current model command
class TbCurrentModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         SetCommandName("CurrentModel"); };    // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };         // deinitialize the command
   virtual BOOL4 Do(char *arg);                                // the next model command
   virtual void Undo();                                        // undo
};


      /*-------------------------
        next models command - Do
        C.Wang 0809, 2012
       --------------------------*/
      BOOL4 TbCurrentModelCommand::Do(char *arg)
      {
         backupObjID = tbCurScene->GetCurrentObject();
         OBJECTid oID = tbCurScene->FindObject(arg);
         if (oID != FAILED_ID) {
            char log[256];
            FnObject model;
            tbCurScene->SetCurrentObject(oID);
            model.ID(tbCurScene->GetCurrentObject());
            sprintf(log, "New current model : %s", model.GetName());
            TbSetLogMessage(log);
         }
         else {
            TbSetLogMessage("Failed to get new current model");
         }
         return TRUE;
      }

      /*--------------------------
        next model command - UnDo
        C.Wang 0613, 2010
      ---------------------------*/
      void TbCurrentModelCommand::Undo()
      {
         tbCurScene->SetCurrentObject(backupObjID);
         backupObjID = FAILED_ID;
      }


// next model command
class TbNextModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         SetCommandName("NextModel"); };    // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };      // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the next model command
   virtual void Undo();                                     // undo
};


      /*-------------------------
        next models command - Do
        C.Wang 0809, 2012
       --------------------------*/
      BOOL4 TbNextModelCommand::Do(char *arg)
      {
         backupObjID = tbCurScene->GetCurrentObject();
         BOOL4 beOK = tbCurScene->NextCurrentObject();
         if (beOK) {
            char log[256];
            FnObject model;
            model.ID(tbCurScene->GetCurrentObject());
            sprintf(log, "New current model : %s", model.GetName());
            TbSetLogMessage(log);
         }
         else {
            TbSetLogMessage("Failed to get next current model");
         }
         return beOK;
      }

      /*--------------------------
        next model command - UnDo
        C.Wang 0809, 2012
      ---------------------------*/
      void TbNextModelCommand::Undo()
      {
         tbCurScene->SetCurrentObject(backupObjID);
         backupObjID = FAILED_ID;
      }


// previous model command
class TbPreviousModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         SetCommandName("PreviousModel"); };    // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };          // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the previous model command
   virtual void Undo();                                         // undo
};


      /*-----------------------------
        previous models command - Do
        C.Wang 0809, 2012
       ------------------------------*/
      BOOL4 TbPreviousModelCommand::Do(char *arg)
      {
         backupObjID = tbCurScene->GetCurrentObject();
         BOOL4 beOK = tbCurScene->PreviousCurrentObject();
         if (beOK) {
            char log[256];
            FnObject model;
            model.ID(tbCurScene->GetCurrentObject());
            sprintf(log, "New current model : %s", model.GetName());
            TbSetLogMessage(log);
         }
         else {
            TbSetLogMessage("Failed to get previous current model");
         }
         return beOK;
      }

      /*------------------------------
        previous model command - UnDo
        C.Wang 0809, 2012
      --------------------------------*/
      void TbPreviousModelCommand::Undo()
      {
         tbCurScene->SetCurrentObject(backupObjID);
         backupObjID = FAILED_ID;
      }


// find model command
class TbFindModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         SetCommandName("FindModel"); };    // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };      // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the find model command
   virtual void Undo();                                     // undo
};


      /*-------------------------
        find models command - Do
        C.Wang 0610, 2013
       --------------------------*/
      BOOL4 TbFindModelCommand::Do(char *arg)
      {
         BOOL4 beOK;
         backupObjID = tbCurScene->GetCurrentObject();
         OBJECTid newObj = tbCurScene->FindObject(arg);
         if (newObj != FAILED_ID) {
            char log[256];
            FnObject model;
            tbCurScene->SetCurrentObject(newObj);
            model.ID(tbCurScene->GetCurrentObject());
            sprintf(log, "New current model : %s", model.GetName());
            TbSetLogMessage(log);
            beOK = TRUE;
         }
         else {
            TbSetLogMessage("Failed to get new current model");
            beOK = FALSE;
         }
         return beOK;
      }

      /*--------------------------
        find model command - UnDo
        C.Wang 0610, 2013
      ----------------------------*/
      void TbFindModelCommand::Undo()
      {
         tbCurScene->SetCurrentObject(backupObjID);
         backupObjID = FAILED_ID;
      }


// delete current model command
class TbDeleteModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;
   CHARACTERid aID;
   int type;
   int boneID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         aID = FAILED_ID;
                         boneID = FY_NONE;
                         type = FY_NONE;
                         SetCommandName("DeleteModel"); };      // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };          // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the next model command
   virtual void Undo();                                         // undo
};


      /*----------------------------------
        delete current model command - Do
        C.Wang 1204, 2012
       -----------------------------------*/
      BOOL4 TbDeleteModelCommand::Do(char *arg)
      {
         backupObjID = tbCurScene->GetCurrentObject();
         if (backupObjID == FAILED_ID) {
            type = FY_NONE;
            boneID = FY_NONE;
            return TRUE;
         }

         TBObject *tbObj = tbCurScene->GetTBObject(backupObjID);
         if (tbObj != NULL) {
            boneID = tbObj->BoneID();
            type = tbObj->Type();
         }
         else {
            type = FY_NONE;
            boneID = FY_NONE;
         }

         FnObject model;
         model.ID(backupObjID);
         aID = tbCurScene->RemoveObject(backupObjID, FALSE);
         tbCurScene->SetCurrentObject(FAILED_ID);

         if (aID == FAILED_ID) {
            type = FY_NONE;
            boneID = FY_NONE;
         }

         char log[256];
         sprintf(log, "Delete current model : %s", model.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*------------------------------------
        delete current model command - UnDo
        C.Wang 1204, 2012
       -------------------------------------*/
      void TbDeleteModelCommand::Undo()
      {
         if (backupObjID == FAILED_ID) return;

         FnObject model;
         model.ID(backupObjID);
         model.DropToTrashCan(FALSE);
         tbCurScene->InsertObject(backupObjID, aID, type, boneID, TRUE);
         tbCurScene->SetCurrentObject(backupObjID);

         if (aID != FAILED_ID) {
            FnCharacter actor;
            actor.ID(aID);
            if (type == 0) {
            }
            else if (type == 1) {
               actor.ApplySkin(backupObjID);
            }
            else if (type == 2) {
               actor.ApplyAttachmentByBoneID(backupObjID, boneID);
            }
         }

         backupObjID = FAILED_ID;
         aID = FAILED_ID;
         boneID = FY_NONE;
      }


// set paths command
class TbSetPathCommand : public FcCommandBase
{
protected:
   char work[256];
   char shader[256];
   char texture[256];
   char model[256];
   char actor[256];
   char audio[256];
   char extra[256];
   char scene[256];
   char gameFX[256];

public:
   virtual void Init() { work[0] = '\0'; shader[0] = '\0'; texture[0] = '\0'; model[0] = '\0'; actor[0] = '\0';
                         audio[256] = '\0'; extra[0] = '\0'; scene[0] = '\0'; gameFX[0] = '\0';
                         SetCommandName("SetPath"); };    // initialize the command
   virtual void DeInit() {};                              // deinitialize the command
   virtual BOOL4 Do(char *arg);                           // the load model command
   virtual void Undo();                                   // undo
};

      /*-----------------------
        set paths command - Do
        C.Wang 1024, 2012
       ------------------------*/
      BOOL4 TbSetPathCommand::Do(char *arg)
      {
         // backup the paths
         strcpy(work, tbWorkDir);
         strcpy(shader, FyGetShaderPath());
         strcpy(model, FyGetModelPath());
         strcpy(texture, FyGetTexturePath());
         strcpy(actor, FyGetCharacterPath());
         strcpy(scene, FyGetScenePath());
         strcpy(audio, FyGetAudioPath());
         strcpy(extra, FyGetDataPath());
         strcpy(gameFX, FyGetGameFXPath());

         // get new paths
         char path[256], fPath[256];
         BOOL4 beOKCmd = TbCheckCommandAttributeS(arg, "-Work", path, 256);

         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            SetCurrentDirectory(fPath);
            strcpy(tbWorkDir, fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-Texture", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetTexturePath(fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-Scene", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetScenePath(fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-Object", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetModelPath(fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-Shader", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetShaderPath(fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-SystemShader", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetSystemShaderPath(fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-GameFX", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetGameFXPath(fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-Character", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetCharacterPath(fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-Audio", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetAudioPath(fPath);
         }

         beOKCmd = TbCheckCommandAttributeS(arg, "-Extra", path, 256);
         if (beOKCmd) {
            TbCheckHomePath(path, fPath);
            FySetDataPath(fPath);
         }

         // update UIs (currently we only UIs for shader, object, texture and actor)
         FnUiTypein uiTy;
         uiTy.ID(tbWorkDirTypeinUI);
         uiTy.PasteTypeinValue(tbWorkDir);
         uiTy.ID(tbShaderDirTypeinUI);
         uiTy.PasteTypeinValue(FyGetShaderPath());
         uiTy.ID(tbModelDirTypeinUI);
         uiTy.PasteTypeinValue(FyGetModelPath());
         uiTy.ID(tbTextureDirTypeinUI);
         uiTy.PasteTypeinValue(FyGetTexturePath());
         uiTy.ID(tbCharacterDirTypeinUI);
         uiTy.PasteTypeinValue(FyGetCharacterPath());
         return TRUE;
      }


      /*-------------------------
        set paths command - UnDo
        C.Wang 1024, 2012
      --------------------------*/
      void TbSetPathCommand::Undo()
      {
         // recover the paths setting
         strcpy(tbWorkDir, work);
         FySetShaderPath(shader);
         FySetModelPath(model);
         FySetTexturePath(texture);
         FySetCharacterPath(actor);
         FySetAudioPath(audio);
         FySetScenePath(scene);
         FySetDataPath(extra);
         FySetGameFXPath(gameFX);

         // update UIs
         FnUiTypein uiTy;
         uiTy.ID(tbWorkDirTypeinUI);
         uiTy.PasteTypeinValue(tbWorkDir);
         uiTy.ID(tbShaderDirTypeinUI);
         uiTy.PasteTypeinValue(shader);
         uiTy.ID(tbModelDirTypeinUI);
         uiTy.PasteTypeinValue(model);
         uiTy.ID(tbTextureDirTypeinUI);
         uiTy.PasteTypeinValue(texture);
         uiTy.ID(tbCharacterDirTypeinUI);
         uiTy.PasteTypeinValue(actor);
      }


// load character command
class TbLoadCharacterCommand : public FcCommandBase
{
protected:
   CHARACTERid backupActorID;
   CHARACTERid lastActor;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         lastActor = FAILED_ID;
                         SetCommandName("character"); };    // initialize the command
   virtual void DeInit() {};                                // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the load model command
   virtual void Undo();                                     // undo
};


      /*----------------------------
        load character command - Do
        C.Wang 0311, 2015
       -----------------------------*/
      BOOL4 TbLoadCharacterCommand::Do(char *arg)
      {
         // open the file browser for loading a character file
         OPENFILENAME ofn;
         char openName[256], dir[256], log[256];
         FnCharacter actor;
         BOOL4 beOK = FALSE;

         // get current directory
         GetCurrentDirectory(256, dir);

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(openName, arg);
            beOK = TRUE;

            // set current working directory to the character folder
            char path[256];
            FyCheckFileFormat(openName, path, NULL, NULL);
            SetCurrentDirectory(path);
         }
         else {
            // prepare to open the file browser
            openName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Fly Engine Character CWA {*.cwa}\0*.cwa\0"\
                              "Fly Engine Character in CW4 {*.cw4}\0*.cw4\0"\
                              "Fly Engine Character in CWC {*.cwc}\0*.cwc\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = openName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cwa";
            ofn.lpstrInitialDir = dir;

            // start to load the character file
            if (GetOpenFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            CHARACTERid newAID;
            OBJECTid objID;
            FnCharacter actor;
            FnTriangle triObj;
            FnObject model;

            // set current rendering group
            //tbScene.SetCurrentRenderGroup(4);

            // load the file
            newAID = tbScene.LoadCharacter(openName);
            actor.ID(newAID);
            //actor.RunTimeUpdateSkinBoundingBox(TRUE);

            // for loaded character, we do 
            if (newAID != FAILED_ID) {
               int i;

               // insert the character to database & make it as the current character
               tbCurScene->InsertCharacter(newAID);

               // make the 1st skin as current object
               objID = actor.GetSkin(0);
               if (objID != FAILED_ID) {
                  // check the current object
                  tbCurScene->SetCurrentObject(objID);
               }
               
               // check current model
               OBJECTid curObj = tbCurScene->GetCurrentObject();
               FnUiRadioButton uiRB;
               uiRB.ID(tbWireframeCurModelButtonID);
               if (curObj != FAILED_ID) {
                  uiRB.Enable(TRUE);
               }
               else {
                  uiRB.Enable(FALSE);
               }

               FnObject model;
               OBJECTid base = actor.GetBaseObject();

               model.ID(base);
               float color[3];
               color[0] = 0.0f; color[1] = color[2] = 1.0f;
               TbDrawBlueBox(&model, 5.0f, color);
               model.Show(TRUE);

               // restore the extra system directory
               FySetDataPath(".");

               // echo the log
               sprintf(log, "Load character %s OK", openName);
               TbSetLogMessage(log);

               // set standard cursor
               SetCursor(tbArrowCursor);

               model.ID(curObj);
               if (model.GetRenderMode() == WIREFRAME) {
                  uiRB.ChangeState(TRUE);
               }
               else {
                  uiRB.ChangeState(FALSE);
               }

               uiRB.ID(tbAlphaCurModelButtonID);
               if (curObj != FAILED_ID) {
                  uiRB.Enable(TRUE);
               }
               else {
                  uiRB.Enable(FALSE);
               }
               if (model.GetAlphaFlag()) {
                  uiRB.ChangeState(TRUE);
               }
               else {
                  uiRB.ChangeState(FALSE);
               }

               uiRB.ID(tbShowCurModelButtonID);
               if (curObj != FAILED_ID) {
                  uiRB.Enable(TRUE);
               }
               else {
                  uiRB.Enable(FALSE);
               }
               if (model.GetVisibility()) {
                  uiRB.ChangeState(TRUE);
               }
               else {
                  uiRB.ChangeState(FALSE);
               }

               FnUiTypein uiTy;
               uiTy.ID(tbRGCurModelID);
               if (curObj != FAILED_ID) {
                  uiTy.Enable(TRUE);
                  char rgValue[64];
                  sprintf(rgValue, "%d", model.GetRenderGroup());
                  uiTy.PasteTypeinValue(rgValue);
               }
               else {
                  uiTy.Enable(FALSE);
               }

               float scaleFactor = actor.GetLocalScale();
               char value[256];
               uiTy.ID(tbSetLocalScaleID);
               sprintf(value, "%6.3", scaleFactor);
               uiTy.PasteTypeinValue(value);

               // backup the character
               lastActor = tbCurScene->GetCurrentCharacter();
               backupActorID = newAID;
               tbCurScene->SetCurrentCharacter(newAID);
               beOK = TRUE;

               // update UIs
               int numA = tbCurScene->GetEffectCharacterNumber();
               int iCount;
               FnUiCombo uiC;
               uiC.ID(tbSelectCharacterComboID);
               uiC.ClearMessages();
               if (numA > 0) {
                  char *name;
                  uiC.Enable(TRUE);
                  iCount = 0;
                  numA = tbCurScene->GetCharacterNumber();
                  for (i = 0; i < numA; i++) {
                     name = tbCurScene->GetCharacterName(i);
                     if (name != NULL) {
                        uiC.InsertMessage(iCount, name);
                        iCount++;
                     }
                  }

                  // paste current character
                  FnCharacter actor;
                  actor.ID(newAID);
                  uiC.PasteComboValue(actor.GetName());
               }
               else {
                  uiC.Enable(FALSE);
                  uiC.PasteComboValue("");
               }

               // find and paste the current body
               FnCharacter actor;
               actor.ID(tbCurScene->GetCurrentCharacter());
               TbUpdateCurrentBodyDataUI(&actor, TRUE);

               // check if the character has the facial blendshapes ?
               if (actor.GetBlendShapeNumber(0) > 0) {
                  FnUiTab uiT(tbBlendshapeTabID);
                  uiT.Enable(TRUE);
                  uiT.Open(TRUE);
               }
               else {
                  FnUiTab uiT(tbBlendshapeTabID);
                  uiT.Open(FALSE);
                  uiT.Enable(FALSE);
               }

               // update some UIs
               FnUiButton uiB;
               if (tbCurScene->GetCurrentCharacter() != FAILED_ID) {
                  uiB.ID(tbCalculateBBButtonID);
                  //uiB.Enable(TRUE);
                  uiB.ID(tbBakeCharacterVAID);
                  uiB.Enable(TRUE);
               }
               else {
                  uiB.ID(tbCalculateBBButtonID);
                  uiB.Enable(FALSE);
                  uiB.ID(tbBakeCharacterVAID);
                  uiB.Enable(FALSE);
               }
               tbCurrentFaceModelID = FAILED_ID;

               // update working directory
               GetCurrentDirectory(256, tbWorkDir);
               uiTy.ID(tbWorkDirTypeinUI);
               uiTy.PasteTypeinValue(tbWorkDir);
            }
            else {
               TbSetLogMessage("No character loaded");
               beOK = FALSE;
            }
         }
         else {
            TbSetLogMessage("No character loaded");
            beOK = FALSE;
         }

         // restore the current directory
         if (tbBeRestoreCurrentDir) {
            SetCurrentDirectory(dir);
         }

         return beOK;
      }

      /*------------------------------
        load character command - UnDo
        C.Wang 0809, 2012
      --------------------------------*/
      void TbLoadCharacterCommand::Undo()
      {
         // delete the character
         tbScene.DeleteCharacter(backupActorID);
         backupActorID = FAILED_ID;

         // recover current character
         tbCurScene->SetCurrentCharacter(lastActor);

         int numA = tbCurScene->GetEffectCharacterNumber();
         int i, iCount;
         FnUiCombo uiC;
         uiC.ID(tbSelectCharacterComboID);
         uiC.ClearMessages();
         if (numA > 0) {
            char *name;
            uiC.Enable(TRUE);
            iCount = 0;
            numA = tbCurScene->GetCharacterNumber();
            for (i = 0; i < numA; i++) {
               name = tbCurScene->GetCharacterName(i);
               if (name != NULL) {
                  uiC.InsertMessage(iCount, name);
                  iCount++;
               }
            }
         }
         else {
            uiC.Enable(FALSE);
            uiC.PasteComboValue("");
         }
         
         // find and paste the current body
         FnCharacter actor;
         actor.ID(tbCurScene->GetCurrentCharacter());
         TbUpdateCurrentBodyDataUI(&actor, TRUE);

         // update some UIs
         FnUiButton uiB;
         if (tbCurScene->GetCurrentCharacter() != FAILED_ID) {
            uiB.ID(tbCalculateBBButtonID);
            //uiB.Enable(TRUE);
            uiB.ID(tbBakeCharacterVAID);
            uiB.Enable(TRUE);
         }
         else {
            uiB.ID(tbCalculateBBButtonID);
            uiB.Enable(FALSE);
            uiB.ID(tbBakeCharacterVAID);
            uiB.Enable(FALSE);
         }
      }


// delete current character command
class TbDeleteCharacterCommand : public FcCommandBase
{
protected:
   CHARACTERid backupActorID;
   OBJECTid curObj;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         curObj = FAILED_ID;
                         SetCommandName("DeleteCharacter"); };      // initialize the command
   virtual void DeInit() { backupActorID = FAILED_ID; };        // deinitialize the command
   virtual BOOL4 Do(char *arg);                                     // the next model command
   virtual void Undo();                                             // undo
};


      /*--------------------------------------
        delete current character command - Do
        C.Wang 0408, 2013
       ---------------------------------------*/
      BOOL4 TbDeleteCharacterCommand::Do(char *arg)
      {
         backupActorID = tbCurScene->GetCurrentCharacter();

         FnCharacter actor;
         actor.ID(backupActorID);
         curObj = tbCurScene->CheckCurObjectInCurCharacter();

         // send the character to trash can
         tbCurScene->RemoveCharacter(backupActorID, FALSE);
         tbCurScene->SetCurrentCharacter(FAILED_ID);

         int numA = tbCurScene->GetEffectCharacterNumber();
         int i, iCount;
         FnUiCombo uiC;
         FnUiButton uiB;
         uiC.ID(tbSelectCharacterComboID);
         uiC.ClearMessages();
         if (numA > 0) {
            char *name;
            uiC.Enable(TRUE);
            iCount = 0;
            numA = tbCurScene->GetCharacterNumber();
            for (i = 0; i < numA; i++) {
               name = tbCurScene->GetCharacterName(i);
               if (name != NULL) {
                  uiC.InsertMessage(iCount, name);
                  iCount++;
               }
            }
         }
         else {
            uiC.Enable(FALSE);
         }
         uiC.PasteComboValue("");

         actor.ID(FAILED_ID);
         TbUpdateCurrentBodyDataUI(&actor, TRUE);

         // update some UIs
         if (tbCurScene->GetCurrentCharacter() != FAILED_ID) {
            uiB.ID(tbDeleteCharacterButtonID);
            uiB.Enable(TRUE);
         }
         else {
            uiB.ID(tbDeleteCharacterButtonID);
            uiB.Enable(FALSE);
         }

         if (curObj != FAILED_ID) {
            tbCurScene->SetCurrentObject(FAILED_ID);
         }

         tbCurScene->SetCurrentCharacter(FAILED_ID);

         char log[256];
         sprintf(log, "Delete current character : %s", actor.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*----------------------------------------
        delete current character command - UnDo
        C.Wang 1010, 2012
       -----------------------------------------*/
      void TbDeleteCharacterCommand::Undo()
      {
         FnCharacter actor;
         actor.ID(backupActorID);
         actor.DropToTrashCan(FALSE);
         tbCurScene->InsertCharacter(backupActorID);
         tbCurScene->SetCurrentCharacter(backupActorID);
         backupActorID = FAILED_ID;

         int numA = tbCurScene->GetEffectCharacterNumber();
         int i, iCount;
         FnUiCombo uiC;
         FnUiButton uiB;
         uiC.ID(tbSelectCharacterComboID);
         uiC.ClearMessages();
         if (numA > 0) {
            char *name;
            uiC.Enable(TRUE);
            iCount = 0;
            numA = tbCurScene->GetCharacterNumber();
            for (i = 0; i < numA; i++) {
               name = tbCurScene->GetCharacterName(i);
               if (name != NULL) {
                  uiC.InsertMessage(iCount, name);
                  iCount++;
               }
            }
            uiB.Enable(TRUE);
         }
         else {
            uiB.Enable(FALSE);
         }

         if (curObj != FAILED_ID) {
            tbCurScene->SetCurrentObject(curObj);
         }

         // update some UIs
         if (tbCurScene->GetCurrentCharacter() != FAILED_ID) {
            uiC.PasteComboValue(actor.GetName());   
            uiB.ID(tbDeleteCharacterButtonID);
            uiB.Enable(TRUE);
            uiB.ID(tbCalculateBBButtonID);
            //uiB.Enable(TRUE);
         }
         else {
            uiC.PasteComboValue("");
            uiB.ID(tbDeleteCharacterButtonID);
            uiB.Enable(TRUE);
            uiB.ID(tbCalculateBBButtonID);
            uiB.Enable(FALSE);
         }

         // find and paste the current body
         actor.ID(tbCurScene->GetCurrentCharacter());
         TbUpdateCurrentBodyDataUI(&actor, TRUE);
      }


// load character motion command
class TbLoadCharacterMotionCommand : public FcCommandBase
{
protected:
   char motionName[256];
   ACTIONid actionID;

public:
   virtual void Init() { motionName[0] = '\0';
                         actionID = FAILED_ID;
                         SetCommandName("Motion"); };       // initialize the command
   virtual void DeInit() {};                                // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the load model command
   virtual void Undo();                                     // undo
};


      /*-----------------------------------
        load character motion command - Do
        C.Wang 0121, 2014
       ------------------------------------*/
      BOOL4 TbLoadCharacterMotionCommand::Do(char *arg)
      {
         // open the file browser for loading a character file
         OPENFILENAME ofn;
         char openName[256], dir[256];
         FnCharacter actor;
         BOOL4 beOK = FALSE;

         // get current directory
         GetCurrentDirectory(256, dir);

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(openName, arg);
            beOK = TRUE;

            // set current working directory to the character folder
            char path[256];
            FyCheckFileFormat(openName, path, NULL, NULL);
            SetCurrentDirectory(path);
         }
         else {
            // prepare to open the file browser
            openName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Fly2 Character Motion CWM {*.cwm}\0*.cwm\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = openName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cwm";
            ofn.lpstrInitialDir = dir;

            // start to load the character file
            if (GetOpenFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         // check current character
         CHARACTERid actorID = tbCurScene->GetCurrentCharacter();
         if (actorID == FAILED_ID) beOK = FALSE;

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            // load the motion to current character
            FnCharacter actor(actorID);
            char mName[256];
            int startFrame, endFrame;
            actor.LoadMotion(openName, mName, &startFrame, &endFrame, NULL);
            actor.DefineBodyPose(actor.GetCurrentBodyName(), mName, startFrame, endFrame, endFrame, mName);
            actionID = actor.CreateAction(actor.GetCurrentBodyName(), mName, mName);

            strcpy(motionName, mName);

            // update UIs
            TbUpdateCurrentBodyDataUI(&actor, TRUE);

            SetCursor(tbArrowCursor);
            TbSetLogMessage("character motion loaded");
         }
         else {
            TbSetLogMessage("No character motion loaded");
            beOK = FALSE;
         }

         // restore the current directory
         if (tbBeRestoreCurrentDir) {
            SetCurrentDirectory(dir);
         }

         return beOK;
      }

      /*-------------------------------------
        load character motion command - UnDo
        C.Wang 0520, 2014
      ---------------------------------------*/
      void TbLoadCharacterMotionCommand::Undo()
      {
         // delete the character motion
         CHARACTERid actorID = tbCurScene->GetCurrentCharacter();
         if (actorID == FAILED_ID) return;

         FnCharacter actor(actorID);
         actor.DeleteAction(actionID); actionID = FAILED_ID;
         actor.DeleteBodyPose(actor.GetCurrentBodyName(), motionName);
         actor.RemoveMotion(motionName);
         motionName[0] = '\0';

         // update UIs
         TbUpdateCurrentBodyDataUI(&actor, TRUE);
      }


// set character scale command
class TbSetCharacterScaleCommand : public FcCommandBase
{
protected:
   float scale;

public:
   virtual void Init() { scale = 1.0f;
                         SetCommandName("CharacterScale"); }; // initialize the command
   virtual void DeInit() {};                                  // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the load model command
   virtual void Undo();                                       // undo
};


      /*---------------------------------
        set character scale command - Do
        C.Wang 0803, 2015
       ----------------------------------*/
      BOOL4 TbSetCharacterScaleCommand::Do(char *arg)
      {
         // delete the character motion
         CHARACTERid actorID = tbCurScene->GetCurrentCharacter();
         if (actorID == FAILED_ID) return FALSE;

         // check value
         if (!TbCheckReal(arg)) return FALSE;

         FnCharacter actor(actorID);
         scale = actor.GetLocalScale();

         float newScale = (float) atof(arg);
         actor.SetLocalScale(newScale);

         return TRUE;
      }

      /*-----------------------------------
        set character scale command - UnDo
        C.Wang 0803, 2015
      -------------------------------------*/
      void TbSetCharacterScaleCommand::Undo()
      {
         // delete the character motion
         CHARACTERid actorID = tbCurScene->GetCurrentCharacter();
         if (actorID == FAILED_ID) return;

         FnCharacter actor(actorID);
         actor.SetLocalScale(scale);
      }


// delete current action of current character command
class TbDeleteActionCommand : public FcCommandBase
{
protected:
   CHARACTERid backupActorID;
   char bodyName[256];
   int type;
   char actionName[256];
   char frontPoseName[256];
   char rearPoseName[256];
   float duration;
   float w0, w1;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         bodyName[0] = '\0';
                         type = SIMPLE_ACTION;
                         frontPoseName[0] = '\0';
                         rearPoseName[0] = '\0';
                         actionName[0] = '\0';
                         duration = 0.0f;
                         w0 = 1.0f;
                         w1 = 0.0f;
                         SetCommandName("DeleteAction"); };     // initialize the command
   virtual void DeInit() { backupActorID = FAILED_ID; };    // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the next model command
   virtual void Undo();                                         // undo
};


      /*-----------------------------------
        delete current action command - Do
        C.Wang 1119, 2012
       ------------------------------------*/
      BOOL4 TbDeleteActionCommand::Do(char *arg)
      {
         // backup data
         backupActorID = tbCurScene->GetCurrentCharacter();
         if (backupActorID == FAILED_ID) {
            TbSetLogMessage("No current character");
            return TRUE;
         }

         FnCharacter actor;
         actor.ID(backupActorID);
         char *name = actor.GetCurrentBodyName();
         if (name == NULL) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current body");
            return TRUE;
         }
         strcpy(bodyName, name);

         ACTIONid actionID = actor.GetCurrentAction(bodyName, 0);
         if (actionID == FAILED_ID) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current action");
            return TRUE;
         }

         FnAction action;
         action.ID(actionID);
         type = action.GetType();
         strcpy(frontPoseName, action.GetCurrentPose(0));
         if (action.GetCurrentPose(1) != NULL) {
            strcpy(rearPoseName, action.GetCurrentPose(1));
         }
         else {
            strcpy(rearPoseName, "");
         }
         duration = action.GetLength();
         strcpy(actionName, action.GetName());
         w0 = action.GetPoseWeight(0);
         w1 = action.GetPoseWeight(1);

         // delete the action
         actor.DeleteAction(actionID);

         // make the character no active action to play
         actor.SetCurrentAction(bodyName, 0, FAILED_ID);

         // update UIs
         int i, iCount, num;
         ACTIONid aTable[MAX_ACTIONS];
         FnUiCombo uiC;
         uiC.ID(tbSelectActionComboID);
         uiC.PasteComboValue("");
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, MAX_ACTIONS);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.ID(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         FnUiButton uiB;
         uiB.ID(tbDeleteActionButtonID);
         uiB.Enable(FALSE);
         uiB.ID(tbEditActionButtonID);
         uiB.Enable(FALSE);
         uiB.ID(tbAddActionButtonID);
         uiB.Enable(TRUE);

         char log[256];
         sprintf(log, "Delete current action : %s", actor.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*-------------------------------------
        delete current action command - UnDo
        C.Wang 1119, 2012
       --------------------------------------*/
      void TbDeleteActionCommand::Undo()
      {
         if (backupActorID == FAILED_ID) return;

         FnCharacter actor;
         actor.ID(backupActorID);
         ACTIONid actionID;
         if (type == CROSS_BLEND_ACTION) {
            actionID = actor.CreateCrossBlendAction(bodyName, actionName, frontPoseName, rearPoseName, duration);
         }
         else if (type == CONNECT_ACTION) {
            actionID = actor.CreateConnectAction(bodyName, actionName, frontPoseName, rearPoseName, duration);
         }
         else if (type == FULL_BLEND_ACTION) {
            actionID = actor.CreateFullBlendAction(bodyName, actionName, frontPoseName, rearPoseName, w0, w1);
         }
         else {
            actionID = actor.CreateAction(bodyName, actionName, frontPoseName);
         }

         // make the character no active action to play
         actor.SetCurrentAction(bodyName, 0, actionID);
         actor.Play(START, 0.0f, tbBeBase);

         // update UIs
         int i, iCount, num;
         ACTIONid aTable[MAX_ACTIONS];
         FnAction action;
         FnUiCombo uiC;
         uiC.ID(tbSelectActionComboID);
         uiC.PasteComboValue(actionName);
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, MAX_ACTIONS);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.ID(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         FnUiButton uiB;
         uiB.ID(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }


// edit current action of current character command
class TbEditActionCommand : public FcCommandBase
{
protected:
   CHARACTERid backupActorID;
   char bodyName[256];
   int type;
   ACTIONid actionID;
   char frontPoseName[256];
   char rearPoseName[256];
   float duration;
   float w0, w1;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         bodyName[0] = '\0';
                         type = SIMPLE_ACTION;
                         frontPoseName[0] = '\0';
                         rearPoseName[0] = '\0';
                         duration = 0.0f;
                         w0 = 1.0f;
                         w1 = 0.0f;
                         SetCommandName("EditAction"); };     // initialize the command
   virtual void DeInit() { backupActorID = FAILED_ID; };  // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};


      /*---------------------------------
        edit current action command - Do
        C.Wang 0810, 2012
       ----------------------------------*/
      BOOL4 TbEditActionCommand::Do(char *arg)
      {
         // backup data
         backupActorID = tbCurScene->GetCurrentCharacter();
         if (backupActorID == FAILED_ID) {
            TbSetLogMessage("No current character");
            return TRUE;
         }

         FnCharacter actor;
         actor.ID(backupActorID);
         char *name = actor.GetCurrentBodyName();
         if (name == NULL) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current body");
            return TRUE;
         }
         strcpy(bodyName, name);

         actionID = actor.GetCurrentAction(bodyName, 0);
         if (actionID == FAILED_ID) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current action");
            return TRUE;
         }

         FnAction action;
         action.ID(actionID);
         type = action.GetType();
         char *frontPose = action.GetCurrentPose(0);
         char *rearPose = action.GetCurrentPose(1);
         strcpy(frontPoseName, action.GetCurrentPose(0));
         if (rearPose != NULL) {
            strcpy(rearPoseName, action.GetCurrentPose(1));
         }
         else {
            rearPoseName[0] = '\0';
         }
         duration = action.GetLength();
         w0 = action.GetPoseWeight(0);
         w1 = action.GetPoseWeight(1);

         // parse the data from argument
         char poseName[256];
         BOOL4 beOK = TbCheckCommandAttributeS(arg, "-front", poseName, 256);
         if (beOK) {
            action.SetCurrentPose(0, poseName);
         }

         if (type != SIMPLE_ACTION) {
            beOK = TbCheckCommandAttributeS(arg, "-rear", poseName, 256);
            if (beOK) {
               action.SetCurrentPose(1, poseName);
            }

            beOK = TbCheckCommandAttributeF(arg, "-length", &duration, 1);
            if (beOK) {
               action.SetLength(duration);
            }
         }

         // make the character play the new action data
         actor.Play(START, 0.0f, tbBeBase);

         char log[256];
         sprintf(log, "Update current action : %s", action.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*-----------------------------------
        edit current action command - UnDo
        C.Wang 0810, 2012
       ------------------------------------*/
      void TbEditActionCommand::Undo()
      {
         if (backupActorID == FAILED_ID) return;

         FnCharacter actor;
         actor.ID(backupActorID);

         FnAction action;
         action.ID(actionID);
         action.SetCurrentPose(0, frontPoseName);
         if (type == CROSS_BLEND_ACTION) {
            action.SetCurrentPose(1, rearPoseName);
            action.SetLength(duration);
         }
         else if (type == CONNECT_ACTION) {
            action.SetCurrentPose(1, rearPoseName);
            action.SetLength(duration);
         }
         else if (type == FULL_BLEND_ACTION) {
            // to-be-done
            action.SetCurrentPose(1, rearPoseName);
         }

         // make the character play the new action data
         actor.Play(START, 0.0f, tbBeBase);
      }


// create a new action of current character command
class TbNewActionCommand : public FcCommandBase
{
protected:
   CHARACTERid backupActorID;
   char bodyName[256];
   ACTIONid actionID;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         bodyName[0] = '\0';
                         actionID = FAILED_ID;
                         SetCommandName("NewAction"); };      // initialize the command
   virtual void DeInit() { backupActorID = FAILED_ID; };  // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};


      /*---------------------------------
        create a new action command - Do
        C.Wang 0810, 2012
       ----------------------------------*/
      BOOL4 TbNewActionCommand::Do(char *arg)
      {
         // backup data
         backupActorID = tbCurScene->GetCurrentCharacter();
         if (backupActorID == FAILED_ID) {
            TbSetLogMessage("No current character");
            return TRUE;
         }

         FnCharacter actor;
         actor.ID(backupActorID);
         char *name = actor.GetCurrentBodyName();
         if (name == NULL) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current body");
            return TRUE;
         }
         strcpy(bodyName, name);

         actionID = actor.GetCurrentAction(bodyName, 0);

         BOOL4 beOK;
         DWORD type = SIMPLE_ACTION;
         float duration = 0.0f;
         int len;
         char frontPoseName[256], rearPoseName[256], aName[256];
         char poseName[256], typeName[256];

         frontPoseName[0] = '\0';
         rearPoseName[0] = '\0';
         duration = 0;

         // parse the data from argument
         beOK = TbCheckCommandAttributeS(arg, "-name", poseName, 256);
         if (beOK) {
            strcpy(aName, poseName);
            if (FyCheckWord(aName, "-")) {
               backupActorID = FAILED_ID;
               TbSetLogMessage("Illegal Action Name");
               return TRUE;
            }
         }

         beOK = TbCheckCommandAttributeS(arg, "-front", poseName, 256);
         if (beOK) {
            strcpy(frontPoseName, poseName);
            if (FyCheckWord(frontPoseName, "-")) {
               backupActorID = FAILED_ID;
               TbSetLogMessage("Illegal Action");
               return TRUE;
            }
         }

         beOK = TbCheckCommandAttributeS(arg, "-rear", poseName, 256);
         if (beOK) {
            strcpy(rearPoseName, poseName);
         }

         beOK = TbCheckCommandAttributeI(arg, "-length", &len, 1);
         if (beOK) {
            duration = (float) len;
         }

         beOK = TbCheckCommandAttributeS(arg, "-type", typeName, 256);
         if (beOK) {
            if (FyCheckWord(typeName, "Cross")) {
               type = CROSS_BLEND_ACTION;
            }
            else if (FyCheckWord(typeName, "Connection")) {
               type = CONNECT_ACTION;
            }
            else {
               type = SIMPLE_ACTION;
            }
         }

         // create a new action
         ACTIONid newActionID = FAILED_ID;
         if (type == CONNECT_ACTION) {
            newActionID = actor.CreateConnectAction(bodyName, aName, frontPoseName, rearPoseName, duration);
         }
         else if (type == CROSS_BLEND_ACTION) {
            newActionID = actor.CreateCrossBlendAction(bodyName, aName, frontPoseName, rearPoseName, duration);
         }
         else {
            newActionID = actor.CreateAction(bodyName, aName, frontPoseName);
         }

         // make the character play the new action data
         actor.SetCurrentAction(bodyName, 0, newActionID);
         actor.Play(START, 0.0f, tbBeBase);

         // update UIs
         int i, iCount, num;
         ACTIONid aTable[MAX_ACTIONS];
         FnAction action;
         FnUiCombo uiC;
         uiC.ID(tbSelectActionComboID);
         uiC.PasteComboValue(aName);
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, MAX_ACTIONS);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.ID(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         if (iCount > 0) {
            uiC.Enable(TRUE);
         }
         else {
            uiC.Enable(FALSE);
         }

         FnUiButton uiB;
         uiB.ID(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbAddActionButtonID);
         uiB.Enable(TRUE);
         
         char log[256];
         sprintf(log, "Create a new action : %s", actor.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*-----------------------------------
        create a new action command - UnDo
        C.Wang 0810, 2012
       ------------------------------------*/
      void TbNewActionCommand::Undo()
      {
         if (backupActorID == FAILED_ID) return;

         FnAction action;
         FnCharacter actor;
         actor.ID(backupActorID);

         // delete current action
         ACTIONid aID = actor.GetCurrentAction(bodyName, 0);
         actor.DeleteAction(aID);

         // recover last current action
         actor.SetCurrentAction(bodyName, 0, actionID);

         if (actionID != FAILED_ID) {
            // make the character play the new action data
            actor.Play(START, 0.0f, tbBeBase);
            action.ID(actionID);
         }

         // update UIs
         int i, iCount, num;
         ACTIONid aTable[MAX_ACTIONS];
         FnUiCombo uiC;
         uiC.ID(tbSelectActionComboID);
         if (actionID != FAILED_ID) {
            uiC.PasteComboValue(action.GetName());
         }
         else {
            uiC.PasteComboValue("-");
         }
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, MAX_ACTIONS);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.ID(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         if (iCount > 0) {
            uiC.Enable(TRUE);
         }
         else {
            uiC.Enable(FALSE);
         }

         FnUiButton uiB;
         uiB.ID(tbEditActionButtonID);
         if (actionID != FAILED_ID) {
            uiB.Enable(TRUE);
         }
         else {
            uiB.Enable(FALSE);
         }
         uiB.ID(tbDeleteActionButtonID);
         if (actionID != FAILED_ID) {
            uiB.Enable(TRUE);
         }
         else {
            uiB.Enable(FALSE);
         }
         uiB.ID(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }


// set current character command
class TbSetCharacterCommand : public FcCommandBase
{
protected:
   CHARACTERid aID;

public:
   virtual void Init() { aID = FAILED_ID;
                         SetCommandName("SetCharacter"); };   // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the command
   virtual void Undo();                                       // undo
};


      /*-----------------------------------
        set current character command - Do
        C.Wang 0810, 2012
       ------------------------------------*/
      BOOL4 TbSetCharacterCommand::Do(char *arg)
      {
         // backup current character
         aID = tbCurScene->GetCurrentCharacter();

         // get new character's name
         FnUiCombo uiC;
         uiC.ID(tbSelectCharacterComboID);
         uiC.PasteComboValue(arg);

         // find the character
         CHARACTERid newID = tbCurScene->GetCharacterByName(arg);

         // set current character
         tbCurScene->SetCurrentCharacter(newID);

         // find and paste the current body
         FnCharacter actor;
         actor.ID(tbCurScene->GetCurrentCharacter());
         TbUpdateCurrentBodyDataUI(&actor, TRUE);
         return TRUE;
      }


      /*-------------------------------------
        set current character command - UnDo
        C.Wang 0810, 2012
       --------------------------------------*/
      void TbSetCharacterCommand::Undo()
      {
         tbCurScene->SetCurrentCharacter(aID);

         // paste the character name to combo
         FnCharacter actor;
         actor.ID(tbCurScene->GetCurrentCharacter());
         TbUpdateCurrentBodyDataUI(&actor, TRUE);
      }


// save character command
class TbSaveCharacterCommand : public FcCommandBase
{
public:
   virtual void Init() { SetCommandName("SaveCharacter"); };    // initialize the command
   virtual void DeInit() { };                                   // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the command
   virtual void Undo() { };                                     // undo
};


      /*----------------------------
        save character command - Do
        C.Wang 0610, 2013
       -----------------------------*/
      BOOL4 TbSaveCharacterCommand::Do(char *arg)
      {
         // open the file browser for saving a character file
         OPENFILENAME ofn;
         char saveName[256], dir[256], log[256];
         BOOL4 beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(saveName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            saveName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Fly Engine Character in CWA {*.cwa}\0*.cwa\0"\
                              "Fly Engine Character in CW4 {*.cw4}\0*.cw4\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = saveName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cwa";
            ofn.lpstrInitialDir = dir;

            // get the actor file name
            if (GetSaveFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            CHARACTERid aID = tbCurScene->GetCurrentCharacter();
            if (aID != FAILED_ID) {
               FnCharacter actor;
               actor.ID(aID);

               // check file format
               BOOL4 beBinary = FALSE;
               char fName[256], ext[64];
               strcpy(fName, saveName);
               FyCheckFileFormat(fName, NULL, NULL, ext);
               if (FyCheckWord(ext, "cw4")) beBinary = TRUE;

               // save the character
               actor.Save(saveName, beBinary);

               sprintf(log, "The %s character is saved.", actor.GetName());
               TbSetLogMessage(log);

               // set standard cursor
               SetCursor(tbArrowCursor);
            }
            else {
               strcpy(log, "Nothing to save");
               TbSetLogMessage(log);
            }

            // update working directory
            GetCurrentDirectory(256, tbWorkDir);
            FnUiTypein uiTy;
            uiTy.ID(tbWorkDirTypeinUI);
            uiTy.PasteTypeinValue(tbWorkDir);
         }
         else {
            strcpy(log, "Nothing to save");
            TbSetLogMessage(log);
         }

         return beOK;
      }


// export character command
class TbExportCharacterCommand : public FcCommandBase
{
public:
   virtual void Init() { SetCommandName("ExportCharacter"); };  // initialize the command
   virtual void DeInit() { };                                   // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the command
   virtual void Undo() { };                                     // undo
};


      /*------------------------------
        export character command - Do
        C.Wang 0610, 2013
       -------------------------------*/
      BOOL4 TbExportCharacterCommand::Do(char *arg)
      {
         // open the file browser for saving a character file
         OPENFILENAME ofn;
         char saveName[256], dir[256], log[256];
         BOOL4 beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(saveName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            saveName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Maya OBJ File {*.obj}\0*.obj\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = saveName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "obj";
            ofn.lpstrInitialDir = dir;

            // get the actor file name
            if (GetSaveFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            CHARACTERid aID = tbCurScene->GetCurrentCharacter();
            if (aID != FAILED_ID) {
               FnCharacter actor;
               actor.ID(aID);

               // save the character
               actor.ExportMayaOBJ(saveName);

               sprintf(log, "The %s character is exported.", actor.GetName());
               TbSetLogMessage(log);

               // set standard cursor
               SetCursor(tbArrowCursor);
            }
            else {
               strcpy(log, "Nothing to export");
               TbSetLogMessage(log);
            }

            // update working directory
            GetCurrentDirectory(256, tbWorkDir);
            FnUiTypein uiTy;
            uiTy.ID(tbWorkDirTypeinUI);
            uiTy.PasteTypeinValue(tbWorkDir);
         }
         else {
            strcpy(log, "Nothing to export");
            TbSetLogMessage(log);
         }

         return beOK;
      }


// set current body command
class TbSetBodyCommand : public FcCommandBase
{
protected:
   BOOL4 beCurrentActor;
   char bodyName[256];

public:
   virtual void Init() { bodyName[0] = '\0';
                         beCurrentActor = TRUE;
                         SetCommandName("SetBody"); };        // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the command
   virtual void Undo();                                       // undo
};


      /*------------------------------
        set current body command - Do
        C.Wang 0810, 2012
       -------------------------------*/
      BOOL4 TbSetBodyCommand::Do(char *arg)
      {
         // check if there's no current character
         if (tbCurScene->GetCurrentCharacter() == FAILED_ID) {
            beCurrentActor = FALSE;
            return TRUE;
         }
         beCurrentActor = TRUE;

         // backup current body
         CHARACTERid aID = tbCurScene->GetCurrentCharacter();
         FnCharacter actor;
         actor.ID(aID);

         if (actor.GetCurrentBodyName() != NULL) {
            strcpy(bodyName, actor.GetCurrentBodyName());
         }
         else {
            beCurrentActor = FALSE;
            return TRUE;
         }

         actor.SetCurrentBody(arg);

         actor.ID(aID);
         TbUpdateCurrentBodyDataUI(&actor, TRUE);
         return TRUE;
      }


      /*--------------------------------
        set current body command - UnDo
        C.Wang 0810, 2012
       ---------------------------------*/
      void TbSetBodyCommand::Undo()
      {
         if (!beCurrentActor) return;

         FnCharacter actor;
         CHARACTERid aID = tbCurScene->GetCurrentCharacter();
         actor.ID(aID);
         TbUpdateCurrentBodyDataUI(&actor, TRUE);
      }


// set body pose data command
class TbSetBodyPoseCommand : public FcCommandBase
{
protected:
   BOOL4 beEditCurrentPose;
   char poseName[256];
   int startFrame, endFrame;
   char mName[256];

public:
   virtual void Init() { beEditCurrentPose = FALSE;
                         poseName[0] = '\0';
                         mName[0] = '\0';
                         startFrame = 0;
                         endFrame = 0;
                         SetCommandName("SetBodyPose"); };    // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the command
   virtual void Undo();                                       // undo
};


      /*--------------------------------
        set body pose data command - Do
        C.Wang 0430, 2014
       ---------------------------------*/
      BOOL4 TbSetBodyPoseCommand::Do(char *arg)
      {
         // check if we have a new pose or edit current pose
         FnCharacter actor;
         FnUiCombo uiC;
         char pName[256];
         uiC.ID(tbSelectPoseComboID);
         uiC.GetComboValue(pName);

         // check the pose name in combo
         actor.ID(tbCurScene->GetCurrentCharacter());
         beEditCurrentPose = actor.QueryBodyPoseDataWithName(actor.GetCurrentBodyName(), pName, NULL, NULL);
         strcpy(poseName, tbCurrentBodyPoseName);

         // backup the current start/end frames for new poses
         startFrame = tbTestPoseStart;
         endFrame = tbTestPoseEnd;
         strcpy(mName, tbTestMotionName);

         // we check two typeins for the start and end frames of the body pose
         int newStart, newEnd;
         char value[256], newMotion[256];
         FnUiTypein uiTy;
         uiTy.ID(tbPoseStartID);
         uiTy.GetTypeinValue(value);
         newStart = (int)atoi(value);
         uiTy.ID(tbPoseEndID);
         uiTy.GetTypeinValue(value);
         newEnd = (int)atoi(value);
         uiTy.ID(tbPoseMotionNameID);
         uiTy.GetTypeinValue(newMotion);

         // assign the new pose data
         actor.DefineBodyPose(actor.GetCurrentBodyName(), pName, newStart, newEnd, newEnd);
         strcpy(tbCurrentBodyPoseName, pName);

         if (!beEditCurrentPose) {
            // for new pose, update the combo
            TbUpdateCurrentBodyDataUI(&actor, FALSE);
         }

         // query the new results and paste them to typeins
         actor.QueryBodyPoseDataWithName(actor.GetCurrentBodyName(), pName, &newStart, &newEnd, newMotion);
         uiTy.ID(tbPoseStartID);
         sprintf(value, "%d", newStart);
         uiTy.PasteTypeinValue(value);
         uiTy.ID(tbPoseEndID);
         sprintf(value, "%d", newEnd);
         uiTy.PasteTypeinValue(value);

         tbTestPoseStart = newStart;
         tbTestPoseEnd = newEnd;
         tbTestPoseFrame = (float) tbTestPoseStart;
         strcpy(tbTestMotionName, newMotion);

         return TRUE;
      }


      /*----------------------------------
        set body pose data command - UnDo
        C.Wang 0927, 2012
       -----------------------------------*/
      void TbSetBodyPoseCommand::Undo()
      {
         FnCharacter actor;
         actor.ID(tbCurScene->GetCurrentCharacter());

         // remove the new defined pose
         if (!beEditCurrentPose) {
            actor.DeleteBodyPose(actor.GetCurrentBodyName(), tbCurrentBodyPoseName);
         }

         // recover the pose data
         actor.DefineBodyPose(actor.GetCurrentBodyName(), poseName, startFrame, endFrame, endFrame, mName);
         tbTestPoseStart = startFrame;
         tbTestPoseEnd = endFrame;
         tbTestPoseFrame = (float) tbTestPoseStart;
         strcpy(tbCurrentBodyPoseName, poseName);
         strcpy(tbTestMotionName, mName);

         // update UIs
         FnUiTypein uiTy;
         char value[256];
         uiTy.ID(tbPoseStartID);
         sprintf(value, "%d", startFrame);
         uiTy.PasteTypeinValue(value);
         uiTy.ID(tbPoseEndID);
         sprintf(value, "%d", endFrame);
         uiTy.PasteTypeinValue(value);
         uiTy.ID(tbPoseMotionNameID);
         uiTy.PasteTypeinValue(tbTestMotionName);

         if (!beEditCurrentPose) {
            // for new pose, update the combo
            TbUpdateCurrentBodyDataUI(&actor, FALSE);

            FnUiCombo uiC;
            uiC.ID(tbSelectPoseComboID);
            uiC.PasteComboValue(tbCurrentBodyPoseName);
         }
      }


// delete body pose command
class TbDeleteBodyPoseCommand : public FcCommandBase
{
protected:
   char poseName[256];
   int startFrame, endFrame;
   char mName[256];

public:
   virtual void Init() { poseName[0] = '\0';
                         startFrame = 0;
                         endFrame = 0;
                         mName[0] = '\0';
                         SetCommandName("DeleteBodyPose"); }; // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the command
   virtual void Undo();                                       // undo
};


      /*------------------------------
        delete body pose command - Do
        C.Wang 0928, 2012
       -------------------------------*/
      BOOL4 TbDeleteBodyPoseCommand::Do(char *arg)
      {
         FnCharacter actor;
         actor.ID(tbCurScene->GetCurrentCharacter());

         // backup the pose
         strcpy(poseName, tbCurrentBodyPoseName);
         startFrame = tbTestPoseStart;
         endFrame = tbTestPoseEnd;
         strcpy(mName, tbTestMotionName);

         // delete the pose
         actor.DeleteBodyPose(actor.GetCurrentBodyName(), poseName);

         // update the UI
         TbUpdateCurrentBodyDataUI(&actor, TRUE);

         return TRUE;
      }


      /*--------------------------------
        delete body pose command - UnDo
        C.Wang 0928, 2012
       ---------------------------------*/
      void TbDeleteBodyPoseCommand::Undo()
      {
         FnCharacter actor;
         actor.ID(tbCurScene->GetCurrentCharacter());

         // recover the pose
         if (strlen(poseName) > 0) {
            // assign the new pose data
            actor.DefineBodyPose(actor.GetCurrentBodyName(), poseName, startFrame, endFrame, endFrame, mName);
            strcpy(tbCurrentBodyPoseName, poseName);

            // for new pose, update the combo
            TbUpdateCurrentBodyDataUI(&actor, FALSE);

            char value[256];
            FnUiTypein uiTy;
            FnUiCombo uiC;
            uiC.ID(tbSelectPoseComboID);
            uiC.PasteComboValue(poseName);

            // query the new results and paste them to typeins
            uiTy.ID(tbPoseStartID);
            sprintf(value, "%d", startFrame);
            uiTy.PasteTypeinValue(value);
            uiTy.ID(tbPoseEndID);
            sprintf(value, "%d", endFrame);
            uiTy.PasteTypeinValue(value);
            uiTy.ID(tbPoseMotionNameID);
            uiTy.PasteTypeinValue(mName);

            tbTestPoseStart = startFrame;
            tbTestPoseEnd = endFrame;
            tbTestPoseFrame = (float) tbTestPoseStart;
            strcpy(tbTestMotionName, mName);
         }
      }


// select current action for current body command
class TbSelectActionCommand : public FcCommandBase
{
protected:
   BOOL4 beCurrentActor;
   char bodyName[256];
   ACTIONid actionID;

public:
   virtual void Init() { actionID = FAILED_ID;
                         beCurrentActor = TRUE;
                         bodyName[0] = '\0';
                         SetCommandName("SelectAction"); };   // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the command
   virtual void Undo();                                       // undo
};


      /*--------------------------------------------
        select action for current body command - Do
        C.Wang 0810, 2012
       ---------------------------------------------*/
      BOOL4 TbSelectActionCommand::Do(char *arg)
      {
         // check if there's no current character
         if (tbCurScene->GetCurrentCharacter() == FAILED_ID) {
            beCurrentActor = FALSE;
            return TRUE;
         }
         beCurrentActor = TRUE;

         // backup current action
         CHARACTERid aID = tbCurScene->GetCurrentCharacter();
         FnCharacter actor;
         actor.ID(aID);

         if (actor.GetCurrentBodyName() != NULL) {
            strcpy(bodyName, actor.GetCurrentBodyName());
         }
         else {
            beCurrentActor = FALSE;
            return TRUE;
         }
         actionID = actor.GetCurrentAction(bodyName, 0);

         // make current action
         ACTIONid animID = actor.GetBodyAction(bodyName, arg);
         actor.SetCurrentAction(bodyName, 0, animID);
         actor.Play(START, 0.0f, tbBeBase);

         FnUiCombo uiC;
         uiC.ID(tbSelectActionComboID);
         uiC.PasteComboValue(arg);

         FnUiButton uiB;
         uiB.ID(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbAddActionButtonID);
         uiB.Enable(TRUE);

         return TRUE;
      }


      /*----------------------------------------------
        select action for current body command - Undo
        C.Wang 0810, 2012
       -----------------------------------------------*/
      void TbSelectActionCommand::Undo()
      {
         if (!beCurrentActor) return;

         FnCharacter actor;
         CHARACTERid aID = tbCurScene->GetCurrentCharacter();
         actor.ID(aID);

         // recover current action
         actor.SetCurrentAction(actor.GetCurrentBodyName(), 0, actionID);
         actor.Play(START, 0.0f, tbBeBase);
         FnAction action;
         action.ID(actionID);
         actionID = FAILED_ID;

         // update UI
         FnUiCombo uiC;
         uiC.ID(tbSelectActionComboID);
         uiC.PasteComboValue(action.GetName());

         FnUiButton uiB;
         uiB.ID(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }


// rename current action command
class TbRenameActionCommand : public FcCommandBase
{
protected:
   CHARACTERid aID;
   char bodyName[256];
   ACTIONid actionID;
   char name[256];

public:
   virtual void Init() { name[0] = '\0'; 
                         aID = FAILED_ID;
                         actionID = FAILED_ID;
                         bodyName[0] = '\0';
                         SetCommandName("RenameAction"); };    // initialize the command
   virtual void DeInit() { };                                  // deinitialize the command
   virtual BOOL4 Do(char *arg);                                // the command
   virtual void Undo();                                        // undo
};


      /*-----------------------------------
        rename current action command - Do
        C.Wang 0810, 2012
       ------------------------------------*/
      BOOL4 TbRenameActionCommand::Do(char *arg)
      {
         // backup the data
         FnCharacter actor;
         aID = tbCurScene->GetCurrentCharacter();
         if (aID == FAILED_ID) {
            return TRUE;
         }

         actor.ID(aID);
         if (actor.GetCurrentBodyName() == NULL) {
            aID = FAILED_ID;
            return TRUE;
         }
         strcpy(bodyName, actor.GetCurrentBodyName());
         actionID = actor.GetCurrentAction(bodyName, 0);

         FnAction action;
         action.ID(actionID);
         strcpy(name, action.GetName());
         action.SetName(arg);

         // update the UI
         int i, iCount, num;
         ACTIONid aTable[1024];
         FnUiCombo uiC;
         uiC.ID(tbSelectActionComboID);
         uiC.PasteComboValue(arg);
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, 1024);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.ID(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }

         return TRUE;
      }


      /*-------------------------------------
        rename current action command - UnDo
        C.Wang 0810, 2012
       --------------------------------------*/
      void TbRenameActionCommand::Undo()
      {
         if (aID == FAILED_ID || actionID == FAILED_ID) return;
         FnAction action;
         action.ID(actionID);
         action.SetName(name);

         // update the UI
         FnCharacter actor;
         int i, iCount, num;
         ACTIONid aTable[MAX_ACTIONS];
         FnUiCombo uiC;

         actor.ID(aID);
         uiC.ID(tbSelectActionComboID);
         uiC.PasteComboValue(name);
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, MAX_ACTIONS);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.ID(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
      }


// set current face command for face tool
class TbSelectFaceCommand : public FcCommandBase
{
protected:
   OBJECTid faceID;
   float camRef[12], cam[12];

public:
   virtual void Init() { faceID = FAILED_ID;
                         SetCommandName("SelectFace"); };   // initialize the command
   virtual void DeInit() { faceID = FAILED_ID; };           // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the command
   virtual void Undo();                                     // undo
};


      /*------------------------------------
        set current face model command - Do
        C.Wang 0508, 2014
       -------------------------------------*/
      BOOL4 TbSelectFaceCommand::Do(char *arg)
      {
         // backup current face model
         faceID = tbCurrentFaceModelID;

         // paste new face's name
         FnUiCombo uiC;
         uiC.ID(tbFindFaceModelComboID);
         uiC.PasteComboValue(arg);

         // find the face model
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         OBJECTid newID = actor->GetSkinByName(arg);
         if (newID != FAILED_ID) {
            // set current face model
            tbCurrentFaceModelID = newID;
            strcpy(tbFaceModelName, arg);

            // point the camera to current face
            float pos[3];
            FnObject faceModel(tbCurrentFaceModelID);
            faceModel.GetBoundingVolume(NULL, NULL, FALSE, pos);

            tbCameraRef.GetMatrix(camRef);
            tbCamera.GetMatrix(cam);
            if (tbBeZUp) {
               tbCameraRef.Translate(pos[0], pos[1], pos[2], REPLACE);
               tbCamera.Translate(0.0f, 0.0f, tbFaceCamDist, REPLACE);
               tbCamera.Rotate(X_AXIS, 90.0f, GLOBAL);
            }
            else {
               tbCameraRef.Translate(pos[0], pos[1], pos[2], REPLACE);
               tbCamera.Translate(0.0f, 0.0f, tbFaceCamDist, REPLACE);
               tbCamera.Rotate(X_AXIS, 0.0f, GLOBAL);
            }

            // update UI
            TbInitFaceTool(FY_NONE);

            // form the texture object of current face
            TbFormTextureModel();

            return TRUE;
         }
         else {
            // update UI
            FnUiRadioButton uiRB(tbOrthoButtonID);
            uiRB.Enable(FALSE);
            uiRB.ChangeState(TRUE);

            // switch to 3D view
            TbShowOrthoView(tbOrthoButtonID, TRUE);

            faceID = FAILED_ID;
            return FALSE;
         }
      }


      /*--------------------------------------
        set current face model command - Undo
        C.Wang 0115, 2014
       ---------------------------------------*/
      void TbSelectFaceCommand::Undo()
      {
         FnObject faceModel(faceID);
         char name[256];

         if (faceID == FAILED_ID) {
            name[0] = '\0';
         }
         else {
            strcpy(name, faceModel.GetName());
         }

         // paste new face's name
         FnUiCombo uiC;
         uiC.ID(tbFindFaceModelComboID);
         uiC.PasteComboValue(name);
         strcpy(tbFaceModelName, name);
         tbCurrentFaceModelID = faceID;

         // recover the camera matrices
         tbCameraRef.SetMatrix(camRef, REPLACE);
         tbCamera.SetMatrix(cam, REPLACE);

         // initialize the face tool
         TbInitFaceTool(FY_NONE);

         // form the texture object of current face
         TbFormTextureModel();
      }


// set current face material
class TbSelectFaceMaterialCommand : public FcCommandBase
{
protected:
   int iOne;
   char matName[256];

public:
   virtual void Init() { iOne = FY_NONE; matName[0] = '\0';
                         SetCommandName("SelectFaceMaterial"); }; // initialize the command
   virtual void DeInit() { };                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                                   // the command
   virtual void Undo();                                           // undo
};


      /*---------------------------------------
        set current face material command - Do
        C.Wang 0117, 2014
       ----------------------------------------*/
      BOOL4 TbSelectFaceMaterialCommand::Do(char *arg)
      {
         // backup current face material
         iOne = tbMatSlotUsed;

         // paste new face material's name
         FnUiCombo uiC;
         uiC.ID(tbFindFaceMaterialComboID);
         uiC.PasteComboValue(arg);
         strcpy(matName, arg);

         // check the material name
         int i, newOne, numS = uiC.GetMessageNumber();
         char *string;
         newOne = FY_NONE;
         for (i = 0; i < numS; i++) {
            string = uiC.GetMessageString(i);
            if (FyCheckWord(string, arg)) {
               newOne = i;
               break;
            }
         }

         // update UI
         TbInitFaceTool(newOne);

         // form the texture object of current face
         TbFormTextureModel();

         if (newOne >= 0) {
            return TRUE;
         }
         else {
            return FALSE;
         }
      }


      /*-----------------------------------------
        set current face material command - Undo
        C.Wang 0117, 2014
       ------------------------------------------*/
      void TbSelectFaceMaterialCommand::Undo()
      {
         // paste new face material's name
         FnUiCombo uiC;
         uiC.ID(tbFindFaceMaterialComboID);
         uiC.PasteComboValue(matName);

         // initialize the face tool
         TbInitFaceTool(iOne);

         // form the texture object of current face
         TbFormTextureModel();
      }


// load face mask command
class TbLoadFaceMaskCommand : public FcCommandBase
{
protected:
   TBFaceMaskForQ3D *faceMask;
   OBJECTid faceMaskID;

public:
   virtual void Init() { faceMask = NULL;
                         faceMaskID = FAILED_ID;
                         SetCommandName("FaceMask"); }; // initialize the command
   virtual void DeInit() {
                         };                             // deinitialize the command
   virtual BOOL4 Do(char *arg);                         // the command
   virtual void Undo();                                 // undo
};


      /*----------------------------
        load face mask command - Do
        C.Wang 0120, 2014
       -----------------------------*/
      BOOL4 TbLoadFaceMaskCommand::Do(char *arg)
      {
         // open the file browser for loading a face mask file
         OPENFILENAME ofn;
         char openName[256], dir[256], log[256];
         BOOL4 beOK = FALSE;
         FnObject model;

         // get current directory
         GetCurrentDirectory(256, dir);

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(openName, arg);
            beOK = TRUE;
         }
         else {
            // prepare to open the file browser
            openName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Q3D Face Mask in TXT {*.txt}\0*.txt\0" \
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = openName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "txt";
            ofn.lpstrInitialDir = dir;

            // start to load the model file
            if (GetOpenFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);
               beOK = TRUE;
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            // load the Q3D face mask
            int nV, nT;
            FnObject faceModel(tbFaceOrthoID);
            GEOMETRYid gID = faceModel.GetGeometryID(0);
            FnTriangle triM(gID);
            nV = triM.GetVertexNumber(NULL);
            nT = triM.GetTriangleNumber();

            TBFaceMaskForQ3D *mask = TbLoadQ3DFaceMask(openName, nV, nT);
            if (mask != NULL) {
               // backup current face mask
               faceMask = tbCurrentFaceMask;
               faceMaskID = tbCurrentFaceMaskID;
               model.ID(faceMaskID);
               model.DropToTrashCan(TRUE);

               // assign current face mask
               tbCurrentFaceMask = mask;

               // draw the new face mask
               tbCurrentFaceMaskID = TbCreateFaceMask(tbFaceOrthoID, mask);

               // success to get the file name
               beOK = TRUE;
               sprintf(log, "%s loaded", openName);
            }
            else {
               beOK = FALSE;
               sprintf(log, "No face mask loaded");
            }

            // set standard cursor
            SetCursor(tbArrowCursor);
         }
         else {
            sprintf(log, "No face mask loaded");
         }

         // restore the current directory
         if (tbBeRestoreCurrentDir) {
            SetCurrentDirectory(dir);
         }

         TbSetLogMessage(log);
         return beOK;
      }

      /*------------------------------
        load face mask command - UnDo
        C.Wang 0120, 2014
      --------------------------------*/
      void TbLoadFaceMaskCommand::Undo()
      {
         if (tbCurrentFaceMask != NULL) {
            // delete the face mask object
            tbOrthoScene.DeleteObject(tbCurrentFaceMaskID);
            tbCurrentFaceMaskID = FAILED_ID;

            // erase current face mask
            FyFree(tbCurrentFaceMask->maskVert);
            FyFree(tbCurrentFaceMask->maskFace);
            FyFree(tbCurrentFaceMask->cornerVert);
            FyFree(tbCurrentFaceMask->blendFace);
            FyFree(tbCurrentFaceMask);
            tbCurrentFaceMask = NULL;
         }

         tbCurrentFaceMask = faceMask;

         FnObject model;
         model.ID(faceMaskID);
         model.DropToTrashCan(FALSE);
         tbCurrentFaceMaskID = faceMaskID;
      }


// adjust face mask command
class TbAdjustFaceMaskCommand : public FcCommandBase
{
protected:
   TBFaceMaskForQ3D *mBKFaceMask;   // Face Mask Structure Backup
   OBJECTid mBKFaceMaskID;          // Face Mask Object ID

public:
   virtual void Init() { mBKFaceMask = NULL;
                         mBKFaceMaskID = FAILED_ID;
                         SetCommandName("AdjustFaceMask"); };  // initialize the command
   virtual void DeInit(){};                                    // deinitialize the command
   virtual BOOL4 Do(char *arg);                                // the command
   virtual void Undo();                                        // undo
};

      /*------------------------------
        adjust face mask command - Do
        Eric Fei 0205, 2014
       -------------------------------*/
      BOOL4 TbAdjustFaceMaskCommand::Do(char *arg)
      {
         // extract argument
         int curStartX = 0;
         int curStartY = 0;
         int curEndX = 0;
         int curEndY = 0;
         int curMode = FT_MASK_NONE;
         int checkFlag = 0;
         char argument[256], *key, *key2;
         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-sX")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  curStartX = (int) atoi(key2);
                  checkFlag++;
               }
            }
            else if (FyCheckWord(key, "-sY")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  curStartY = (int) atoi(key2);
                  checkFlag++;
               }
            }
            else if (FyCheckWord(key, "-eX")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  curEndX = (int) atoi(key2);
                  checkFlag++;
               }
            }
            else if (FyCheckWord(key, "-eY")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  curEndY = (int) atoi(key2);
                  checkFlag++;
               }
            }
            else if (FyCheckWord(key, "-mode")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  curMode = (int) atoi(key2);
                  checkFlag++;
               }
            }
            key = strtok(NULL, " ");
         }

         // check argument result
         if (checkFlag != 5) return FALSE;

         // initialize
         BOOL4 beOK = FALSE;
         
         int nV, nT;
         FnObject faceModel(tbFaceOrthoID);
         GEOMETRYid gID = faceModel.GetGeometryID(0);
         FnTriangle triM(gID);
         nV = triM.GetVertexNumber(NULL);
         nT = triM.GetTriangleNumber();
         char log[256];

         // create new face mask
         TBFaceMaskForQ3D *mask = TbAdjustQ3DFaceMask(tbFaceOrthoID, curStartX, curStartY, curEndX, curEndY, curMode);
         if (mask != NULL) {
            // backup current face mask
            mBKFaceMask = tbCurrentFaceMask;
            mBKFaceMaskID = tbCurrentFaceMaskID;
            FnObject model(mBKFaceMaskID);
            model.DropToTrashCan(TRUE);

            // assign current face mask
            tbCurrentFaceMask = mask;

            // draw the new face mask
            tbCurrentFaceMaskID = TbCreateFaceMask(tbFaceOrthoID, mask);

            // success to get the file name
            beOK = TRUE;
            sprintf(log, "adjust face mask success");
         }
         else {
            beOK = FALSE;
            sprintf(log, "adjust face mask fail");
         }

         TbSetLogMessage(log);
         return beOK;
      }

      /*--------------------------------
        adjust face mask command - UnDo
        Eric Fei 0129, 2014
       ---------------------------------*/
      void TbAdjustFaceMaskCommand::Undo()
      {
         if (tbCurrentFaceMask != NULL) {
            // delete the face mask object
            tbOrthoScene.DeleteObject(tbCurrentFaceMaskID);
            tbCurrentFaceMaskID = FAILED_ID;

            // erase current face mask
            FyFree(tbCurrentFaceMask->maskVert);
            FyFree(tbCurrentFaceMask->maskFace);
            FyFree(tbCurrentFaceMask->cornerVert);
            FyFree(tbCurrentFaceMask->blendFace);
            tbCurrentFaceMask->maskVert = NULL;
            tbCurrentFaceMask->maskFace = NULL;
            tbCurrentFaceMask->cornerVert = NULL;
            tbCurrentFaceMask->blendFace = NULL;

            FyFree(tbCurrentFaceMask);
            tbCurrentFaceMask = NULL;
         }

         tbCurrentFaceMask = mBKFaceMask;

         FnObject model;
         model.ID(mBKFaceMaskID);
         model.DropToTrashCan(FALSE);
         tbCurrentFaceMaskID = mBKFaceMaskID;
      }


// save face mask command
class TbSaveFaceMaskCommand : public FcCommandBase
{
public:
   virtual void Init() { SetCommandName("SaveFaceMask"); }; // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL4 Do(char *arg);                             // the command
   virtual void Undo() { };                                 // undo
};


      /*----------------------------
        save face mask command - Do
        C.Wang 0120, 2014
       -----------------------------*/
      BOOL4 TbSaveFaceMaskCommand::Do(char *arg)
      {
         // open the file browser for saving an object file
         OPENFILENAME ofn;
         char saveName[256], dir[256], log[256];
         BOOL4 beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(saveName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            saveName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Q3D Face Mask in TXT {*.txt}\0*.txt\0" \
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = saveName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "txt";
            ofn.lpstrInitialDir = dir;

            // get the model file name
            if (GetSaveFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            // save the face mask file
            TbSaveQ3DFaceMask(saveName, tbCurrentFaceMask);
      
            // set standard cursor
            SetCursor(tbArrowCursor);
            sprintf(log, "%s saved", saveName);
            TbSetLogMessage(log);
         }
         else {
            strcpy(log, "Nothing to save");
            TbSetLogMessage(log);
         }

         return beOK;
      }


// create a new facial expression command
class TbNewFacialExpressionCommand : public FcCommandBase
{
protected:
   char name[256];

public:
   virtual void Init() { name[0] = '\0';
                         SetCommandName("NewFacialExpression"); }; // initialize the command
   virtual void DeInit() { };                                      // deinitialize the command
   virtual BOOL4 Do(char *arg);                                    // the command
   virtual void Undo();                                            // undo
};

      /*--------------------------------------------
        create a new facial expression command - Do
        C.Wang 0513, 2014
       ---------------------------------------------*/
      BOOL4 TbNewFacialExpressionCommand::Do(char *arg)
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) {
            char log[256];
            strcpy(log, "No current character to select the facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }

         // backup current facial expression
         char *oldName = actor->GetCurrentFacialExpression();
         if (oldName != NULL) {
            strcpy(name, oldName);
         }

         // get the new facial expression data
         char newName[256], argument[256], *key, *key2;
         int life = 100;

         strcpy(argument, arg);
         key = strtok(argument, " ");
         strcpy(newName, "NewFacialExpression");
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(newName, key2);
               }
            }
            else if (FyCheckWord(key, "-length")) {
               key2 = strtok(NULL, " ");
               life = atoi(key2);
            }
            key = strtok(NULL, " ");
         }

         actor->AddFacialExpression(newName);
         actor->SetFacialExpressionLength(newName, life);
         actor->MakeCurrentFacialExpression(newName);
         float weight[64];
         int i, numShapes = actor->GetBlendShapeNumber(0);

         for (i = 0; i < numShapes; i++) {
            weight[i] = 0.0f;
         }
         actor->AddFacialExpressionKey(newName, 0.0f, NULL, weight, numShapes);
         actor->AddFacialExpressionKey(newName, (float)life - 1.0f, NULL, weight, numShapes);

         // play the frame
         actor->PlayFacialExpression(START, 0.0f);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();
         return TRUE;
      }

      /*----------------------------------------------
        create a new facial expression command - Undo
        C.Wang 0513, 2014
       -----------------------------------------------*/
      void TbNewFacialExpressionCommand::Undo()
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) return;

         char *newName = actor->GetCurrentFacialExpression();
         if (strlen(name) > 0) {
            actor->MakeCurrentFacialExpression(name);
         }
         actor->RemoveFacialExpression(newName);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();
      }


// rename current facial expression command
class TbRenameCurrentFacialExpressionCommand : public FcCommandBase
{
protected:
   int iOne;
   char name[256];

public:
   virtual void Init() { name[0] = '\0'; iOne = FY_NONE;
                         SetCommandName("RenameCurrentFacialExpression"); }; // initialize the command
   virtual void DeInit() { };                                                // deinitialize the command
   virtual BOOL4 Do(char *arg);                                              // the command
   virtual void Undo();                                                      // undo
};

      /*----------------------------------------------
        rename current facial expression command - Do
        C.Wang 0513, 2014
       -----------------------------------------------*/
      BOOL4 TbRenameCurrentFacialExpressionCommand::Do(char *arg)
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) {
            char log[256];
            strcpy(log, "No current character to rename the facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }

         // get the new facial expression data
         char newName[256], argument[256], *key, *key2;
         int life = 100;

         newName[0] = '\0';
         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(newName, key2);
               }
            }
            key = strtok(NULL, " ");
         }
         if (strlen(newName) <= 0) {
            char log[256];
            strcpy(log, "No new name");
            TbSetLogMessage(log);
            return FALSE;
         }

         char *theName, *oldName = actor->GetCurrentFacialExpression();
         if (oldName == NULL) {
            char log[256];
            strcpy(log, "No current facial expression to be renamed");
            TbSetLogMessage(log);
            return FALSE;
         }
         strcpy(name, oldName);

         int i, numFE = actor->FacialExpressionNumber();
         for (i = 0; i < numFE; i++) {
            theName = actor->GetFacialExpressionName(i);
            if (FyCheckWord(theName, oldName)) {
               actor->SetFacialExpressionName(i, newName);
               iOne = i;
               break;
            }
         }

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();
         return TRUE;
      }

      /*------------------------------------------------
        select current facial expression command - Undo
        C.Wang 0513, 2014
       -------------------------------------------------*/
      void TbRenameCurrentFacialExpressionCommand::Undo()
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) return;
         if (strlen(name) <= 0 || iOne < 0) return;

         actor->SetFacialExpressionName(iOne, name);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();
      }


// set current facial expression animation length command
class TbSetFacialExpressionLengthCommand : public FcCommandBase
{
protected:
   int length;
   char name[256];

public:
   virtual void Init() { name[0] = '\0'; length = 0;
                         SetCommandName("FacialExpressionLength"); }; // initialize the command
   virtual void DeInit() { };                                         // deinitialize the command
   virtual BOOL4 Do(char *arg);                                       // the command
   virtual void Undo();                                               // undo
};

      /*------------------------------------------------------------
        set current facial expression animation length command - Do
        C.Wang 0513, 2014
       -------------------------------------------------------------*/
      BOOL4 TbSetFacialExpressionLengthCommand::Do(char *arg)
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) {
            char log[256];
            strcpy(log, "No current character to update the facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }

         // get the new facial expression data
         char *oldName = actor->GetCurrentFacialExpression();
         if (oldName == NULL) {
            char log[256];
            strcpy(log, "No current facial expression to be upadted");
            TbSetLogMessage(log);
            return FALSE;
         }
         strcpy(name, oldName);

         // get the new facial expression data
         char argument[256], *key, *key2;
         int life = 100;

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-length")) {
               key2 = strtok(NULL, " ");
               life = atoi(key2);
            }
            key = strtok(NULL, " ");
         }

         length = actor->GetFacialExpressionLength(oldName);
         actor->SetFacialExpressionLength(oldName, life);
         return TRUE;
      }

      /*--------------------------------------------------------------
        set current facial expression animation length command - Undo
        C.Wang 0513, 2014
       ---------------------------------------------------------------*/
      void TbSetFacialExpressionLengthCommand::Undo()
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) return;
         if (strlen(name) <= 0) return;

         actor->SetFacialExpressionLength(name, length);
      }


// make current facial expression command
class TbMakeCurrentFacialExpressionCommand : public FcCommandBase
{
protected:
   char name[256];

public:
   virtual void Init() { name[0] = '\0';
                         SetCommandName("MakeCurrentFacialExpression"); }; // initialize the command
   virtual void DeInit() { };                                              // deinitialize the command
   virtual BOOL4 Do(char *arg);                                            // the command
   virtual void Undo();                                                    // undo
};

      /*----------------------------------------------
        select current facial expression command - Do
        C.Wang 0513, 2014
       -----------------------------------------------*/
      BOOL4 TbMakeCurrentFacialExpressionCommand::Do(char *arg)
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) {
            char log[256];
            strcpy(log, "No current character to remove the facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }

         // get the new facial expression data
         char newName[256], argument[256], *key, *key2;
         int life = 100;

         newName[0] = '\0';
         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(newName, key2);
               }
            }
            key = strtok(NULL, " ");
         }
         if (strlen(newName) <= 0) {
            char log[256];
            strcpy(log, "No new facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }

         char *oldName = actor->GetCurrentFacialExpression();
         if (oldName != NULL) {
            strcpy(name, oldName);
         }

         actor->MakeCurrentFacialExpression(newName);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();

         TbStopPlayFacialExpression(tbStopFacialExpressionID, FALSE);
         return TRUE;
      }

      /*------------------------------------------------
        select current facial expression command - Undo
        C.Wang 0513, 2014
       -------------------------------------------------*/
      void TbMakeCurrentFacialExpressionCommand::Undo()
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) return;
         if (strlen(name) <= 0) return;

         actor->MakeCurrentFacialExpression(name);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();
         TbStopPlayFacialExpression(tbStopFacialExpressionID, FALSE);
      }


// remove a facial expression command
class TbRemoveFacialExpressionCommand : public FcCommandBase
{
protected:
   char name[256];
   int len;
   int numKey;
   float weight[1280];
   float posID[64];
   char *shapeName[1280];

public:
   virtual void Init() { name[0] = '\0'; len = 0; numKey = 0; memset(weight, 0, sizeof(float)*1280);
                         for (int i = 0; i < 1280; i++) { shapeName[i] = NULL; }
                         SetCommandName("RemoveFacialExpression"); }; // initialize the command
   virtual void DeInit() { for (int i = 0; i < 1280; i++) { if (shapeName[i] != NULL) FyFree(shapeName[i]); } };                                         // deinitialize the command
   virtual BOOL4 Do(char *arg);                                       // the command
   virtual void Undo();                                               // undo
};

      /*----------------------------------------
        remove a facial expression command - Do
        C.Wang 0513, 2014
       -----------------------------------------*/
      BOOL4 TbRemoveFacialExpressionCommand::Do(char *arg)
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) {
            char log[256];
            strcpy(log, "No current character to remove the facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }

         // backup current facial expression
         char *oldName = actor->GetCurrentFacialExpression();
         if (oldName == NULL) {
            char log[256];
            strcpy(log, "No current facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }
         strcpy(name, oldName);

         // get the facial expression data
         len = actor->GetFacialExpressionLength(name);
         numKey = actor->FacialExpressionKeyNumber(name);
         int i, j, numBS;
         float *w;

         numBS = actor->GetBlendShapeNumber(0);
         w = weight;
         for (i = 0; i < numKey; i++) {
            for (j = 0; j < numBS; j++) {
               shapeName[i*numBS + j] = (char *)FyMalloc(128);
            }
            posID[i] = actor->GetFacialExpressionKey(name, i, &(shapeName[i*numBS]), w, NULL);
            w += numBS;
         }

         actor->RemoveFacialExpression(oldName);
         actor->MakeCurrentFacialExpression(NULL);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();

         // play the frame 0
         TbStopPlayFacialExpression(tbStopFacialExpressionID, FALSE);
         return TRUE;
      }

      /*------------------------------------------
        remove a facial expression command - Undo
        C.Wang 0513, 2014
       -------------------------------------------*/
      void TbRemoveFacialExpressionCommand::Undo()
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) return;

         actor->AddFacialExpression(name);
         actor->SetFacialExpressionLength(name, len);

         int i;
         float *w = weight;
         int numBS = actor->GetBlendShapeNumber(0);
         for (i = 0; i < numKey; i++){
            actor->AddFacialExpressionKey(name, posID[i], &(shapeName[i*numBS]), w, NULL);
            w += numBS;
         }
         actor->MakeCurrentFacialExpression(name);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();

         // play the frame 0
         TbStopPlayFacialExpression(tbStopFacialExpressionID, FALSE);
      }


// add a facial expression key command
class TbAddFacialExpressionKeyCommand : public FcCommandBase
{
protected:
   char name[256];
   int posF;

public:
   virtual void Init() { name[0] = '\0'; posF = 0;
                         SetCommandName("AddFacialExpressionKey"); }; // initialize the command
   virtual void DeInit() { };                                         // deinitialize the command
   virtual BOOL4 Do(char *arg);                                       // the command
   virtual void Undo();                                               // undo
};

      /*---------------------------------------------
        add a new facial expression key command - Do
        C.Wang 0515, 2014
       ----------------------------------------------*/
      BOOL4 TbAddFacialExpressionKeyCommand::Do(char *arg)
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) {
            char log[256];
            strcpy(log, "No current character to add the facial expression key");
            TbSetLogMessage(log);
            return FALSE;
         }

         // backup current facial expression
         char *oldName = actor->GetCurrentFacialExpression();
         if (oldName == NULL) {
            char log[256];
            strcpy(log, "No current facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }
         strcpy(name, oldName);

         char value[256];
         FnUiCombo uiC(tbSelectFacialExpressionKeyID);
         uiC.GetComboValue(value);
         posF = atoi(value);

         int i, numBS = actor->GetBlendShapeNumber(0);
         float weight[64];

         FnUiTypein uiTy;
         for (i = 0; i < numBS; i++){
            uiTy.ID(tbBlendShapeID[i]);
            uiTy.Show(TRUE);
            uiTy.GetTypeinValue(value);
            weight[i] = (float)atof(value);
         }
         for (i = numBS; i < MAX_BLENDSHAPES; i++) {
            uiTy.ID(tbBlendShapeID[i]);
            uiTy.Show(FALSE);
         }

         // add the key
         actor->AddFacialExpressionKey(name, (float)posF, NULL, weight, numBS);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();

         // set the new key as current
         sprintf(value, "%d", posF);
         uiC.PasteComboValue(value);
         TbGotoFacialExpressionFrame(tbSelectFacialExpressionKeyID, value);

         return TRUE;
      }

      /*-------------------------------------------
        add a new facial expression command - Undo
        C.Wang 0515, 2014
       --------------------------------------------*/
      void TbAddFacialExpressionKeyCommand::Undo()
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) return;

         actor->RemoveFacialExpressionKey(name, (float)posF);

         FnUiCombo uiC(tbSelectFacialExpressionKeyID);
         uiC.PasteComboValue("");

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();

         // play the frame 0
         TbStopPlayFacialExpression(tbStopFacialExpressionID, FALSE);
      }

// update a facial expression key command
class TbUpdateFacialExpressionKeyCommand : public FcCommandBase
{
protected:
   char name[256];
   int posF;
   int numBS;
   float weight[64];

public:
   virtual void Init() { name[0] = '\0'; posF = 0; numBS = 0; memset(weight, 0, sizeof(float)*64);
                         SetCommandName("UpdateFacialExpressionKey"); }; // initialize the command
   virtual void DeInit() { };                                            // deinitialize the command
   virtual BOOL4 Do(char *arg);                                          // the command
   virtual void Undo();                                                  // undo
};

      /*--------------------------------------------
        update a facial expression key command - Do
        C.Wang 0515, 2014
       ---------------------------------------------*/
      BOOL4 TbUpdateFacialExpressionKeyCommand::Do(char *arg)
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) {
            char log[256];
            strcpy(log, "No current character to update the facial expression key");
            TbSetLogMessage(log);
            return FALSE;
         }

         // backup current facial expression
         char *oldName = actor->GetCurrentFacialExpression();
         if (oldName == NULL) {
            char log[256];
            strcpy(log, "No current facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }
         strcpy(name, oldName);

         char value[256];
         FnUiCombo uiC(tbSelectFacialExpressionKeyID);
         uiC.GetComboValue(value);
         posF = atoi(value);

         numBS = actor->GetBlendShapeNumber(0);
         int iOne = actor->CheckFacialExpressionKey(name, (float) posF);
         actor->GetFacialExpressionKey(name, iOne, NULL, weight);

         // update the key
         int i;
         float newWeight[64];
         FnUiTypein uiTy;
         for (i = 0; i < numBS; i++) {
            uiTy.ID(tbBlendShapeID[i]);
            uiTy.Show(TRUE);
            uiTy.GetTypeinValue(value);
            newWeight[i] = (float)atof(value);
         }
         for (i = numBS; i < MAX_BLENDSHAPES; i++) {
            uiTy.ID(tbBlendShapeID[i]);
            uiTy.Show(FALSE);
         }
         actor->ModifyFacialExpressionKey(name, (float)posF, NULL, newWeight, numBS);

         // play the frame 0
         actor->PlayFacialExpression(START, (float) posF);
         return TRUE;
      }

      /*------------------------------------------
        update a facial expression command - Undo
        C.Wang 0515, 2014
       -------------------------------------------*/
      void TbUpdateFacialExpressionKeyCommand::Undo()
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) return;

         // update the key
         actor->ModifyFacialExpressionKey(name, (float)posF, NULL, weight, numBS);
         int i;
         FnUiTypein uiTy;
         char value[256];

         for (i = 0; i < numBS; i++) {
            uiTy.ID(tbBlendShapeID[i]);
            uiTy.Show(TRUE);
            sprintf(value, "%f", weight[i]);
            uiTy.PasteTypeinValue(value);
         }
         for (i = numBS; i < MAX_BLENDSHAPES; i++) {
            uiTy.ID(tbBlendShapeID[i]);
            uiTy.Show(FALSE);
         }

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();

         // play the frame 0
         actor->PlayFacialExpression(START, (float) posF);
      }

// remove a facial expression key command
class TbRemoveFacialExpressionKeyCommand : public FcCommandBase
{
protected:
   char name[256];
   int posF;
   int numBS;
   float weight[64];

public:
   virtual void Init() { name[0] = '\0'; posF = 0; numBS = 0; memset(weight, 0, sizeof(float)*64);
                         SetCommandName("RemoveFacialExpressionKey"); }; // initialize the command
   virtual void DeInit() { };                                            // deinitialize the command
   virtual BOOL4 Do(char *arg);                                          // the command
   virtual void Undo();                                                  // undo
};

      /*--------------------------------------------
        remove a facial expression key command - Do
        C.Wang 0515, 2014
       ---------------------------------------------*/
      BOOL4 TbRemoveFacialExpressionKeyCommand::Do(char *arg)
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) {
            char log[256];
            strcpy(log, "No current character to remove the facial expression key");
            TbSetLogMessage(log);
            return FALSE;
         }

         // backup current facial expression
         char *oldName = actor->GetCurrentFacialExpression();
         if (oldName == NULL) {
            char log[256];
            strcpy(log, "No current facial expression");
            TbSetLogMessage(log);
            return FALSE;
         }
         strcpy(name, oldName);

         char value[256];
         FnUiCombo uiC(tbSelectFacialExpressionKeyID);
         uiC.GetComboValue(value);
         posF = atoi(value);
         uiC.PasteComboValue("");

         numBS = actor->GetBlendShapeNumber(0);
         int iOne = actor->CheckFacialExpressionKey(name, (float) posF);
         actor->GetFacialExpressionKey(name, iOne, NULL, weight);

         // remove the key
         actor->RemoveFacialExpressionKey(name, (float)posF);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();

         // play the frame 0
         actor->PlayFacialExpression(START, 0.0f);
         return TRUE;
      }

      /*------------------------------------------
        remove a facial expression command - Undo
        C.Wang 0515, 2014
       -------------------------------------------*/
      void TbRemoveFacialExpressionKeyCommand::Undo()
      {
         // check current facial expression
         TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
         if (actor == NULL) return;

         actor->AddFacialExpressionKey(name, (float)posF, NULL, weight, numBS);

         // update the UIs
         TbUpdateCurrentFacialExpressionUIs();

         // play the frame 0
         actor->PlayFacialExpression(START, (float) posF);
      }


// create a new game FX system command
class TbNewGameFXSystemCommand : public FcCommandBase
{
protected:
   GAMEFX_SYSTEMid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         SetCommandName("NewGameFXSystem"); }; // initialize the command
   virtual void DeInit() { };                                  // deinitialize the command
   virtual BOOL4 Do(char *arg);                                // the next model command
   virtual void Undo();                                        // undo
};

      /*-----------------------------------------
        create a new game FX system command - Do
        C.Wang 1004, 2012
       ------------------------------------------*/
      BOOL4 TbNewGameFXSystemCommand::Do(char *arg)
      {
         // check current game FX system
         TBGameFXSystem *curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX != NULL) {
            // backup current game FX system
            gxID = curFX->ID();
         }

         // create a new game FX system 
         GAMEFX_SYSTEMid newGXID = tbCurScene->CreateGameFXSystem();

         // insert the game FX system and set it as current one
         tbCurScene->InsertGameFXSystem(newGXID);
         tbCurScene->SetCurrentGameFXSystem(newGXID);

         curFX = tbCurScene->GetCurrentTBGameFXSystem();

         // setup the game FX system data
         char name[256], argument[256], *key, *key2;
         float life = 100.0f;

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(name, key2);
               }
            }
            key = strtok(NULL, " ");
         }

         // setup the name
         curFX->SetName(name);

         // reset the FX
         curFX->Reset();

         // update the UIs
         TbUpdateCurrentFXUIs();         
         return TRUE;
      }

      /*-------------------------------------------
        create a new game FX system command - UnDo
        C.Wang 1004, 2012
       --------------------------------------------*/
      void TbNewGameFXSystemCommand::Undo()
      {
         TBGameFXSystem *curFX = tbCurScene->GetCurrentTBGameFXSystem();
         tbCurScene->RemoveGameFXSystem(curFX->ID(), TRUE);

         // recover current game FX system
         if (gxID != FAILED_ID) {
            tbCurScene->SetCurrentGameFXSystem(gxID);
         }

         // update the UIs
         TbUpdateCurrentFXUIs();
      }


// select a game FX system command
class TbSelectGameFXSystemCommand : public FcCommandBase
{
protected:
   GAMEFX_SYSTEMid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         SetCommandName("SelectGameFXSystem"); }   // initialize the command
   virtual void DeInit() { };                                      // deinitialize the command
   virtual BOOL4 Do(char *arg);                                    // the next model command
   virtual void Undo();                                            // undo
};

      /*-------------------------------------
        select a game FX system command - Do
        C.Wang 1004, 2012
       --------------------------------------*/
      BOOL4 TbSelectGameFXSystemCommand::Do(char *arg)
      {
         TBGameFXSystem *curFX, *nextFX;
         nextFX = tbCurScene->GetTBGameFXSystem(arg);
         if (nextFX == NULL) return FALSE;

         // backup current game FX system
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX != NULL) {
            gxID = curFX->ID();
         }

         // set new current game FX system
         tbCurScene->SetCurrentGameFXSystem(nextFX->ID());

         // update the UIs
         TbUpdateCurrentFXUIs();         
         return TRUE;
      }

      /*---------------------------------------
        select a game FX system command - UnDo
        C.Wang 1004, 2012
       ----------------------------------------*/
      void TbSelectGameFXSystemCommand::Undo()
      {
         // recover current game FX system
         tbCurScene->SetCurrentGameFXSystem(gxID);

         // update the UIs
         TbUpdateCurrentFXUIs();
      }


// rename current game FX system command
class TbRenameGameFXSystemCommand : public FcCommandBase
{
protected:
   char lastCurrentFXName[256];

public:
   virtual void Init() { lastCurrentFXName[0] = '\0';
                         SetCommandName("RenameGameFXSystem"); }  // initialize the command
   virtual void DeInit() { };                                     // deinitialize the command
   virtual BOOL4 Do(char *arg);                                   // the next model command
   virtual void Undo();                                           // undo
};

      /*-------------------------------------
        rename a game FX system command - Do
        C.Wang 1004, 2012
       --------------------------------------*/
      BOOL4 TbRenameGameFXSystemCommand::Do(char *arg)
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         // backup current FX system's name
         strcpy(lastCurrentFXName, curFX->GetName());

         // rename !
         curFX->SetName(arg);

         // update the UIs
         TbUpdateCurrentFXUIs();         
         return TRUE;
      }

      /*---------------------------------------
        rename a game FX system command - UnDo
        C.Wang 1003, 2012
       ----------------------------------------*/
      void TbRenameGameFXSystemCommand::Undo()
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         // recover current FX system's name
         curFX->SetName(lastCurrentFXName);

         // update the UIs
         TbUpdateCurrentFXUIs();
      }


// save current game FX system command
class TbSaveGameFXSystemCommand : public FcCommandBase
{
protected:
   char lastCurrentFXName[256];

public:
   virtual void Init() { lastCurrentFXName[0] = '\0';
                         SetCommandName("SaveGameFXSystem"); }  // initialize the command
   virtual void DeInit() { };                                   // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the next model command
   virtual void Undo() {};                                      // undo
};

      /*-----------------------------------
        save a game FX system command - Do
        C.Wang 1226, 2012
       ------------------------------------*/
      BOOL4 TbSaveGameFXSystemCommand::Do(char *arg)
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         // open the file browser for saving an FX system
         OPENFILENAME ofn;
         char saveName[256], dir[256], log[256];
         BOOL4 beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(saveName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            saveName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "Fly Engine Game FX CWF {*.cwf}\0*.cwf\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = saveName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cwf";
            ofn.lpstrInitialDir = dir;

            // get the model file name
            if (GetSaveFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               FySetDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            // save the Game FX
            curFX->Save(saveName);

            strcpy(log, "The game FX system is saved.");
            TbSetLogMessage(log);
      
            // set standard cursor
            SetCursor(tbArrowCursor);

            // update working directory
            GetCurrentDirectory(256, tbWorkDir);
            FnUiTypein uiTy;
            uiTy.ID(tbWorkDirTypeinUI);
            uiTy.PasteTypeinValue(tbWorkDir);
         }
         else {
            strcpy(log, "Nothing to save");
            TbSetLogMessage(log);
         }

         // save it
         return TRUE;
      }


// load current game FX system command
class TbLoadGameFXSystemCommand : public FcCommandBase
{
protected:
   BOOL4 beDelete;
   GAMEFX_SYSTEMid gxSID;
   int numFX;
   GAMEFXid *backupFXID;

public:
   virtual void Init() { beDelete = FALSE;
                         gxSID = FAILED_ID;
                         numFX = 0;
                         backupFXID = NULL;
                         SetCommandName("LoadGameFXSystem"); }            // initialize the command
   virtual void DeInit() { if (backupFXID != NULL) FyFree(backupFXID); }; // deinitialize the command
   virtual BOOL4 Do(char *arg);                                           // the next model command
   virtual void Undo();                                                   // undo
};


      /*-----------------------------------
        load a game FX system command - Do
        C.Wang 1231, 2012
       ------------------------------------*/
      BOOL4 TbLoadGameFXSystemCommand::Do(char *arg)
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();

         // get current directory
         BOOL4 beOK;
         OPENFILENAME ofn;
         char dir[256], openName[256];
         GetCurrentDirectory(256, dir);

         // prepare to open the file browser
         openName[0] = '\0';
         memset(&ofn, 0, sizeof(OPENFILENAME));
         ofn.lStructSize = sizeof(OPENFILENAME);
         ofn.hwndOwner = NULL;
         ofn.lpstrFilter = "Fly2 Game FX CWF {*.cwf}\0*.cwf\0"\
                           "All files {*.*}\0*.*\0\0";
         ofn.lpstrFile = openName;
         ofn.nMaxFile = 256;
         ofn.Flags = OFN_FILEMUSTEXIST;
         ofn.lpstrDefExt = "cwf";
         ofn.lpstrInitialDir = dir;

         // start to load the model file
         if (GetOpenFileName(&ofn)) {
            // set the extra directory for data loading
            char name[256];
            GetCurrentDirectory(256, name);
            FySetDataPath(name);

            // success to get the file name
            beOK = TRUE;
         }

         if (beOK) {
            if (curFX == NULL) {
               beDelete = TRUE;

               // create a new game FX system
               gxSID = tbCurScene->CreateGameFXSystem();

               // insert the game FX system and set it as current one
               tbCurScene->InsertGameFXSystem(gxSID);
               tbCurScene->SetCurrentGameFXSystem(gxSID);

               // get current game FX system
               curFX = tbCurScene->GetCurrentTBGameFXSystem();
            }

            numFX = curFX->QueryFXNumberInFile(openName);
            if (numFX > 0) {
               backupFXID = (GAMEFXid *)FyMalloc(sizeof(GAMEFXid)*numFX);

               // load the game FX file
               curFX->Load(openName, FALSE, backupFXID, numFX);

               // add marker for FXs
               FnObject model;
               FnBaseGameFX base;
               OBJECTid oID;
               float color[3];
               color[0] = 0.0f; color[1] = color[2] = 1.0f;

               for (int i = 0; i < numFX; i++) {
                  base.ID(backupFXID[i]);
                  oID = base.GetBaseObject();
                  model.ID(oID);
                  if (base.GetFXType() == BASE_FX) {
                     TbDrawBlueBox(&model, 1.0f, color);
                  }
                  else if (base.GetFXType() == PARTICLES_FX) {
                     TbDrawBlueBox(&model, 1.0f, color);
                  }
                  else if (base.GetFXType() == FORCE_FX) {
                     TbDrawBlueBox(&model, 1.0f, color);
                  }
                  else if (base.GetFXType() == AUDIO_FX) {
                     TbDrawBlueBox(&model, 1.0f, color);
                  }
               }

               // reset the FX system
               curFX->Reset();

               // update UIs
               TbUpdateCurrentFXUIs();
            }
         }

         // restore the current directory
         if (tbBeRestoreCurrentDir) {
            SetCurrentDirectory(dir);
         }

         return beOK;
      }

      /*-------------------------------------
        load a game FX system command - Undo
        C.Wang 1231, 2012
       --------------------------------------*/
      void TbLoadGameFXSystemCommand::Undo()
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         if (numFX > 0) {
            int i;
            for (i = 0; i < numFX; i++) {
               curFX->DeleteFX(backupFXID[i], TRUE);
            }
            FyFree(backupFXID);
            backupFXID = NULL;
            numFX = 0;
         }

         if (beDelete) {
            tbCurScene->RemoveGameFXSystem(gxSID, TRUE);
         }

         // update UIs
         TbUpdateCurrentFXUIs();
      }


// delete current game FX system command
class TbDeleteGameFXSystemCommand : public FcCommandBase
{
protected:
   GAMEFX_SYSTEMid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         SetCommandName("DeleteGameFXSystem"); }   // initialize the command
   virtual void DeInit() {};                                       // deinitialize the command
   virtual BOOL4 Do(char *arg);                                    // the next model command
   virtual void Undo();                                            // undo
};


      /*-------------------------------------------
        delete current game FX system command - Do
        C.Wang 1231, 2012
       --------------------------------------------*/
      BOOL4 TbDeleteGameFXSystemCommand::Do(char *arg)
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         // backup current game FX system
         gxID = curFX->ID();

         // delete currenr game FX system
         tbCurScene->RemoveGameFXSystem(gxID, FALSE);

         // update UIs
         TbUpdateCurrentFXUIs();

         return TRUE;
      }

      /*---------------------------------------------
        delete current game FX system command - Undo
        C.Wang 1231, 2012
       ----------------------------------------------*/
      void TbDeleteGameFXSystemCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the game FX system
         FnGameFXSystem gx(gxID);
         gx.DropToTrashCan(FALSE);

         tbCurScene->InsertGameFXSystem(gxID);
         tbCurScene->SetCurrentGameFXSystem(gxID);

         // update UIs
         TbUpdateCurrentFXUIs();
      }


// create a new game FX command
class TbNewGameFXCommand : public FcCommandBase
{
protected:
   TBGameFXSystem *curFX;
   BOOL4 beNewOne;
   GAMEFXid gxID;
   char lastCurrentFX[256];

public:
   virtual void Init() { curFX = NULL;
                         beNewOne = TRUE;
                         gxID = FAILED_ID;
                         lastCurrentFX[0] = '\0';
                         SetCommandName("NewGameFX"); };      // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};

      /*----------------------------------
        create a new game FX command - Do
        C.Wang 0131, 2013
       -----------------------------------*/
      BOOL4 TbNewGameFXCommand::Do(char *arg)
      {
         // check current game FX system
         GAMEFXid gID;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) {
            // create a new game FX system
            GAMEFX_SYSTEMid gxID = tbCurScene->CreateGameFXSystem();

            // insert the game FX system and set it as current one
            tbCurScene->InsertGameFXSystem(gxID);
            tbCurScene->SetCurrentGameFXSystem(gxID);

            // get current game FX system
            curFX = tbCurScene->GetCurrentTBGameFXSystem();
            beNewOne = TRUE;
         }
         else {
            beNewOne = FALSE;

            // backup last current FX
            gID = curFX->GetCurrentGameFX();
            if (gID != FAILED_ID) {
               FnBaseGameFX gx(gID);
               strcpy(lastCurrentFX, gx.GetName());
            }
         }

         // extract game FX data
         DWORD type = BASE_FX;
         char argument[256], *key, *key2, name[256];
         int life = tbDefFXLife;
         int maxP = tbCurMaxParticles;
         float size[2], forceM = 32.0f;
         DWORD bbType = PLANE_BILLBOARD_CLOUD;
         DWORD bType = ADD_COLOR;
         DWORD forceType = GRAVITY;

         name[0] = '\0';
         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(name, key2);
               }
            }
            else if (FyCheckWord(key, "-Life")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  life = atoi(key2);
               }
            }
            else if (FyCheckWord(key, "-MaxP")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  maxP = atoi(key2);
               }
            }
            else if (FyCheckWord(key, "-Force")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  if (FyCheckWord(key2, "Gravity")) {
                     forceType = GRAVITY;
                  }
                  else if(FyCheckWord(key2, "Parallel")) {
                     forceType = PARALLEL;
                  }
                  else if(FyCheckWord(key2, "Sink")) {
                     forceType = SINK;
                  }
                  else if(FyCheckWord(key2, "Sphere")) {
                     forceType = SPHERE;
                  }
                  else if(FyCheckWord(key2, "Vortex")) {
                     forceType = VORTEX;
                  }
                  else if(FyCheckWord(key2, "Vortex2D")) {
                     forceType = VORTEX_2D;
                  }
                  else if(FyCheckWord(key2, "Viscosity")) {
                     forceType = VISCOSITY;
                  }
                  else if(FyCheckWord(key2, "UserDefined")) {
                     forceType = USER_DEFINED;
                  }
               }
            }
            else if (FyCheckWord(key, "-ForceMagnitude")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  forceM = (float) atof(key2);
               }
            }
            else if (FyCheckWord(key, "-type")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  if (FyCheckWord(key2, "Plate")) {
                     type = PLATE_FX;
                  }
                  else if (FyCheckWord(key2, "Particles")) {
                     type = PARTICLES_FX;
                  }
                  else if (FyCheckWord(key2, "Force")) {
                     type = FORCE_FX;
                  }
                  else if (FyCheckWord(key2, "Audio")) {
                     type = AUDIO_FX;
                  }
                  else if (FyCheckWord(key2, "Sword")) {
                     type = SWORD_FX;
                  }
                  else if (FyCheckWord(key2, "Geometry")) {
                     type = GEOMETRY_FX;
                  }
                  else if (FyCheckWord(key2, "Sword")) {
                     type = SWORD_FX;
                  }
               }
            }
            key = strtok(NULL, " ");
         }

         // create the new FX
         gxID = curFX->NewFX(type, TRUE);

         // set the FX name
         FnBaseGameFX baseFX(gxID);
         baseFX.SetName(name);
         baseFX.SetInitialLife((float)life);

         FnObject model;
         OBJECTid base = baseFX.GetBaseObject();
         model.ID(base);
         float color[3];
         color[0] = 0.0f; color[1] = color[2] = 1.0f;
         TbDrawBlueBox(&model, 1.0f, color);

         // initialize the FX
         if (type == PLATE_FX) {
            // add a z-up plate FX
            FnPlateGameFX plate(gxID);

            size[0] = size[1] = 40.0f;
            plate.CreatePlate(PLATE_ZUP, size);
         }
         else if (type == PARTICLES_FX) {
            // add a new particles FX
            FnParticlesGameFX particles(gxID);

            size[0] = size[1] = tbDefParticleSize;
            particles.CreateParticles(maxP, NULL, size, bbType, bType);
         }
         else if (type == FORCE_FX) {
            FnForceGameFX force(gxID);

            force.Initialize(forceType, forceM);
         }
         else if (type == AUDIO_FX) {
            // add a new audio FX
            FnAudioGameFX audio(gxID);

            audio.CreateAudio(NULL);
         }
         else if (type == GEOMETRY_FX) {
            FnGeometryGameFX geo(gxID);

            geo.Initialize();
         }
         else if (type == SWORD_FX) {
            // add a new sword FX
            FnSwordGameFX sword(gxID);

            sword.CreateSword();
         }

         // reset the FX
         curFX->Reset();

         // update the UIs
         TbUpdateCurrentFXUIs();         
         return TRUE;
      }

      /*------------------------------------
        create a new game FX command - UnDo
        C.Wang 1001, 2012
       -------------------------------------*/
      void TbNewGameFXCommand::Undo()
      {
         // check current FX system
         if (curFX == NULL) return;

         curFX->DeleteFX(gxID);
         gxID = FAILED_ID;

         // check if we should remove current game FX system
         if (beNewOne) {
            tbCurScene->RemoveGameFXSystem(curFX->ID(), TRUE);
         }
         else {
            // restore current FX
            gxID = curFX->GetGameFXByName(lastCurrentFX);
            curFX->MakeCurrentGameFX(gxID);
         }

         // update the UIs
         TbUpdateCurrentFXUIs();
      }


// select a game FX command
class TbSelectGameFXCommand : public FcCommandBase
{
protected:
   char lastCurrentFX[256];

public:
   virtual void Init() { lastCurrentFX[0] = '\0';
                         SetCommandName("SelectGameFX"); }    // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};

      /*------------------------------
        select a game FX command - Do
        C.Wang 1003, 2012
       -------------------------------*/
      BOOL4 TbSelectGameFXCommand::Do(char *arg)
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         // backup current FX's name
         GAMEFXid gxID = curFX->GetCurrentGameFX();

         FnBaseGameFX gx(gxID);
         if (gxID != FAILED_ID) {
            strcpy(lastCurrentFX, gx.GetName());
         }
         else {
            lastCurrentFX[0] = '\0';
         }

         // set new current FX
         gxID = curFX->GetGameFXByName(arg);
         curFX->MakeCurrentGameFX(gxID);

         // update the UIs
         TbUpdateCurrentFXUIs();         
         return TRUE;
      }

      /*--------------------------------
        select a game FX command - UnDo
        C.Wang 1003, 2012
       ---------------------------------*/
      void TbSelectGameFXCommand::Undo()
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         // recover current FX
         if (strlen(lastCurrentFX) > 0) {
            GAMEFXid gxID = curFX->GetGameFXByName(lastCurrentFX);
            curFX->MakeCurrentGameFX(gxID);
         }
         else {
            curFX->MakeCurrentGameFX(FAILED_ID);
         }

         // update the UIs
         TbUpdateCurrentFXUIs();
      }


// modify a game FX key value
class TbModifyGameFXKeyCommand : public FcCommandBase
{
protected:
   BOOL4 beAdd;
   int type;
   int iKey;
   float oldValue[4];

public:
   virtual void Init() { type = 0;
                         beAdd = TRUE;
                         iKey = 0;
                         oldValue[0] = oldValue[1] = oldValue[2] = oldValue[3] = 0.0f;
                         SetCommandName("ModifyFXKey"); }     // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};

      /*----------------------------------------
        modify a game FX key value command - Do
        C.Wang 1217, 2012
       -----------------------------------------*/
      BOOL4 TbModifyGameFXKeyCommand::Do(char *arg)
      {
         TBGameFXSystem *gx;
         GAMEFXid fxID;
         FnBaseGameFX baseFX;
         FnUiCombo uiC;
         FnUiTypein uiTy;
         char string[256];
         int i, numK, iV;
         float v[4];
         BOOL4 beOK = FALSE;

         // get current FX
         gx = tbCurScene->GetCurrentTBGameFXSystem();
         if (gx == NULL) return FALSE;
         fxID = gx->GetCurrentGameFX();
         if (fxID == FAILED_ID) return beOK;
         baseFX.ID(fxID);

         int life = (int) baseFX.GetLife();

         // check key
         type = atoi(arg);
         numK = 0;
         uiC.ID(tbGameFXKeyFrameID);
         uiC.GetComboValue(string);
         iKey = atoi(string);

         if (iKey < 0 || iKey >= life) {
            if (iKey < 0) iKey = 0;
            else iKey = life - 1;

            sprintf(string, "%d", iKey);
            uiC.PasteComboValue(string);
         }

         beAdd = TRUE;
         if (type == KEYFRAME_VISIBILITY) {
            numK = baseFX.GetVisibilityKeyNumber();
            for (i = 0; i < numK; i++) {
               baseFX.GetVisibilityKey(i, &iV, v);
               if (iV == iKey) {
                  beAdd = FALSE;
                  break;
               }
            }

            if (!beAdd) {
               oldValue[0] = v[0];
            }

            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.GetTypeinValue(string);
            v[0] = (float) atof(string);

            // set new keys
            baseFX.SetVisibilityKey(iKey, v[0]);

            // update the FX visibility
            baseFX.SetVisibility(v[0]);
            beOK = TRUE;
         }
         else if (type == KEYFRAME_POSITION) {
            numK = baseFX.GetPositionKeyNumber();
            for (i = 0; i < numK; i++) {
               baseFX.GetPositionKey(i, &iV, v);
               if (iV == iKey) {
                  beAdd = FALSE;
                  break;
               }
            }

            if (!beAdd) {
               oldValue[0] = v[0];
               oldValue[1] = v[1];
               oldValue[2] = v[2];
            }

            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.GetTypeinValue(string);
            v[0] = (float) atof(string);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.GetTypeinValue(string);
            v[1] = (float) atof(string);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.GetTypeinValue(string);
            v[2] = (float) atof(string);

            // set new keys
            baseFX.SetPositionKey(iKey, v);

            // update the FX position
            baseFX.SetPosition(v);
            beOK = TRUE;
         }
         else if (type == KEYFRAME_ROTATION) {
            numK = baseFX.GetRotationKeyNumber();
            for (i = 0; i < numK; i++) {
               baseFX.GetRotationKey(i, &iV, v);
               if (iV == iKey) {
                  beAdd = FALSE;
                  break;
               }
            }

            if (!beAdd) {
               oldValue[0] = v[0];
               oldValue[1] = v[1];
               oldValue[2] = v[2];
            }

            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.GetTypeinValue(string);
            v[0] = (float) atof(string);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.GetTypeinValue(string);
            v[1] = (float) atof(string);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.GetTypeinValue(string);
            v[2] = (float) atof(string);

            // update the FX orientation
            baseFX.SetRotation(v);

            // set new keys
            baseFX.SetRotationKey(iKey, v);
            beOK = TRUE;
         }
         else if (type == KEYFRAME_PLATE_SIZE) {
            if (baseFX.GetFXType() != PLATE_FX) return FALSE;
            FnPlateGameFX plateFX(fxID);

            numK = plateFX.GetPlateSizeKeyNumber();
            for (i = 0; i < numK; i++) {
               plateFX.GetPlateSizeKey(i, &iV, v);
               if (iV == iKey) {
                  beAdd = FALSE;
                  break;
               }
            }

            if (!beAdd) {
               oldValue[0] = v[0];
               oldValue[1] = v[1];
            }

            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.GetTypeinValue(string);
            v[0] = (float) atof(string);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.GetTypeinValue(string);
            v[1] = (float) atof(string);

            // update the FX size
            plateFX.SetPlateSize(v);

            // set new keys
            plateFX.SetPlateSizeKey(iKey, v);

            beOK = TRUE;
         }
         else if (type == KEYFRAME_PLATE_COLOR) {
            if (baseFX.GetFXType() != PLATE_FX) return FALSE;
            FnPlateGameFX plateFX(fxID);

            numK = plateFX.GetPlateColorKeyNumber();
            for (i = 0; i < numK; i++) {
               plateFX.GetPlateColorKey(i, &iV, v);
               if (iV == iKey) {
                  beAdd = FALSE;
                  break;
               }
            }

            if (!beAdd) {
               oldValue[0] = v[0];
               oldValue[1] = v[1];
               oldValue[2] = v[2];
               oldValue[3] = v[3];
            }

            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.GetTypeinValue(string);
            v[0] = (float) atof(string);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.GetTypeinValue(string);
            v[1] = (float) atof(string);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.GetTypeinValue(string);
            v[2] = (float) atof(string);
            uiTy.ID(tbGameFXKeyDataID[3]);
            uiTy.GetTypeinValue(string);
            v[3] = (float) atof(string);

            // update the FX plate color
            plateFX.SetPlateColor(v);

            // set new keys
            plateFX.SetPlateColorKey(iKey, v);

            beOK = TRUE;
         }


         // update UIs
         TbUpdatSelectKeysUI(&baseFX, iKey);

         return beOK;
      }

      /*------------------------------------------
        modify a game FX key value command - UnDo
        C.Wang 1217, 2012
       -------------------------------------------*/
      void TbModifyGameFXKeyCommand::Undo()
      {
         TBGameFXSystem *gx;
         GAMEFXid fxID;
         FnBaseGameFX baseFX;

         // get current FX
         gx = tbCurScene->GetCurrentTBGameFXSystem();
         if (gx == NULL) return;
         fxID = gx->GetCurrentGameFX();
         if (fxID == FAILED_ID) return;
         baseFX.ID(fxID);

         if (type == KEYFRAME_VISIBILITY) {
            if (beAdd) {
               baseFX.RemoveVisibilityKey(iKey);
            }
            else {
               baseFX.SetVisibilityKey(iKey, oldValue[0]);
            }
         }
         else if (type == KEYFRAME_POSITION) {
            if (beAdd) {
               baseFX.RemovePositionKey(iKey);
            }
            else {
               baseFX.SetPositionKey(iKey, oldValue);
            }
         }
         else if (type == KEYFRAME_ROTATION) {
            if (beAdd) {
               baseFX.RemoveRotationKey(iKey);
            }
            else {
               baseFX.SetRotationKey(iKey, oldValue);
            }
         }
         else if (type == KEYFRAME_PLATE_SIZE) {
            if (baseFX.GetFXType() != PLATE_FX) return;
            FnPlateGameFX plateFX(fxID);
            if (beAdd) {
               plateFX.RemovePlateSizeKey(iKey);
            }
            else {
               plateFX.SetPlateSizeKey(iKey, oldValue);
            }
         }
         else if (type == KEYFRAME_PLATE_COLOR) {
            if (baseFX.GetFXType() != PLATE_FX) return;
            FnPlateGameFX plateFX(fxID);
            if (beAdd) {
               plateFX.RemovePlateColorKey(iKey);
            }
            else {
               plateFX.SetPlateColorKey(iKey, oldValue);
            }
         }
      }


// remove a game FX key value
class TbRemoveGameFXKeyCommand : public FcCommandBase
{
protected:
   int type;
   int iKey;
   float oldValue[4];

public:
   virtual void Init() { type = 0;
                         iKey = 0;
                         oldValue[0] = oldValue[1] = oldValue[2] = oldValue[3] = 0.0f;
                         SetCommandName("RemoveFXKey"); }     // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};

      /*----------------------------------------
        remove a game FX key value command - Do
        C.Wang 1216, 2012
       -----------------------------------------*/
      BOOL4 TbRemoveGameFXKeyCommand::Do(char *arg)
      {
         TBGameFXSystem *gx;
         GAMEFXid fxID;
         FnBaseGameFX baseFX;
         FnUiCombo uiC;
         FnUiTypein uiTy;
         char string[256];
         int i, numK, iV;
         float v[4];
         BOOL4 beOK = FALSE;

         // get current FX
         gx = tbCurScene->GetCurrentTBGameFXSystem();
         if (gx == NULL) return FALSE;
         fxID = gx->GetCurrentGameFX();
         if (fxID == FAILED_ID) return beOK;
         baseFX.ID(fxID);

         // check key
         type = atoi(arg);
         numK = 0;
         uiC.ID(tbGameFXKeyFrameID);
         uiC.GetComboValue(string);
         iKey = atoi(string);

         if (type == KEYFRAME_VISIBILITY) {
            numK = baseFX.GetVisibilityKeyNumber();
            for (i = 0; i < numK; i++) {
               baseFX.GetVisibilityKey(i, &iV, v);
               if (iV == iKey) {
                  break;
               }
            }
            if (i >= numK) return FALSE;

            oldValue[0] = v[0];
            baseFX.RemoveVisibilityKey(iKey);
            beOK = TRUE;
         }
         else if (type == KEYFRAME_POSITION) {
            numK = baseFX.GetPositionKeyNumber();
            for (i = 0; i < numK; i++) {
               baseFX.GetPositionKey(i, &iV, v);
               if (iV == iKey) {
                  break;
               }
            }
            if (i >= numK) return FALSE;

            oldValue[0] = v[0];
            oldValue[1] = v[1];
            oldValue[2] = v[2];
            baseFX.RemovePositionKey(iKey);
            beOK = TRUE;
         }
         else if (type == KEYFRAME_ROTATION) {
            numK = baseFX.GetRotationKeyNumber();
            for (i = 0; i < numK; i++) {
               baseFX.GetRotationKey(i, &iV, v);
               if (iV == iKey) {
                  break;
               }
            }
            if (i >= numK) return FALSE;

            oldValue[0] = v[0];
            oldValue[1] = v[1];
            oldValue[2] = v[2];

            baseFX.RemoveRotationKey(iKey);

            beOK = TRUE;
         }
         else if (type == KEYFRAME_PLATE_SIZE) {
            if (baseFX.GetFXType() != PLATE_FX) return FALSE;
            FnPlateGameFX plateFX(fxID);

            numK = plateFX.GetPlateSizeKeyNumber();
            for (i = 0; i < numK; i++) {
               plateFX.GetPlateSizeKey(i, &iV, v);
               if (iV == iKey) {
                  break;
               }
            }
            if (i >= numK) return FALSE;

            oldValue[0] = v[0];
            oldValue[1] = v[1];

            plateFX.RemovePlateSizeKey(iKey);

            beOK = TRUE;
         }
         else if (type == KEYFRAME_PLATE_COLOR) {
            if (baseFX.GetFXType() != PLATE_FX) return FALSE;
            FnPlateGameFX plateFX(fxID);

            numK = plateFX.GetPlateColorKeyNumber();
            for (i = 0; i < numK; i++) {
               plateFX.GetPlateColorKey(i, &iV, v);
               if (iV == iKey) {
                  break;
               }
            }
            if (i >= numK) return FALSE;

            oldValue[0] = v[0];
            oldValue[1] = v[1];
            oldValue[2] = v[2];
            oldValue[3] = v[3];

            plateFX.RemovePlateColorKey(iKey);

            beOK = TRUE;
         }

         // update UIs
         TbUpdatSelectKeysUI(&baseFX, 0);

         return beOK;
      }


      /*------------------------------------------
        remove a game FX key value command - UnDo
        C.Wang 1217, 2012
       -------------------------------------------*/
      void TbRemoveGameFXKeyCommand::Undo()
      {
         TBGameFXSystem *gx;
         GAMEFXid fxID;
         FnBaseGameFX baseFX;

         // get current FX
         gx = tbCurScene->GetCurrentTBGameFXSystem();
         if (gx == NULL) return;
         fxID = gx->GetCurrentGameFX();
         if (fxID == FAILED_ID) return;
         baseFX.ID(fxID);

         if (type == KEYFRAME_VISIBILITY) {
            baseFX.SetVisibilityKey(iKey, oldValue[0]);
         }
         else if (type == KEYFRAME_POSITION) {
            baseFX.SetPositionKey(iKey, oldValue);
         }
         else if (type == KEYFRAME_ROTATION) {
            baseFX.SetRotationKey(iKey, oldValue);
         }
         else if (type == KEYFRAME_PLATE_SIZE) {
            if (baseFX.GetFXType() != PLATE_FX) return;
            FnPlateGameFX plateFX(fxID);

            plateFX.SetPlateSizeKey(iKey, oldValue);
         }
         else if (type == KEYFRAME_PLATE_COLOR) {
            if (baseFX.GetFXType() != PLATE_FX) return;
            FnPlateGameFX plateFX(fxID);

            plateFX.SetPlateColorKey(iKey, oldValue);
         }
      }


// delete a game FX command
class TbDeleteGameFXCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   int numPS;
   GAMEFXid particles[64];

public:
   virtual void Init() { gxID = FAILED_ID;
                         numPS = 0;
                         SetCommandName("DeleteGameFX"); };   // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};

      /*------------------------------
        delete a game FX command - Do
        C.Wang 0917, 2014
       -------------------------------*/
      BOOL4 TbDeleteGameFXCommand::Do(char *arg)
      {
         // backup current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         // check all particles using this forces
         int i;
         FnParticlesGameFX psFX;
         FnBaseGameFX baseFX(gxID);
         if (baseFX.GetFXType() == FORCE_FX) {
            GAMEFXid gFXID, forceID;
            FnBaseGameFX checkFX;
            int numForce, j, numFX = curFX->GameFXNumber();
            numPS = 0;
            for (i = 0; i < numFX; i++) {
               gFXID = curFX->GetGameFX(i);
               checkFX.ID(gFXID);
               if (checkFX.GetFXType() == PARTICLES_FX) {
                  psFX.ID(gFXID);
                  numForce = psFX.GetForceNumber();
                  for (j = 0; j < numForce; j++) {
                     forceID = psFX.GetForce(j);
                     if (forceID == gxID && numPS < 64) {
                        particles[numPS] = gFXID;
                        numPS++;
                     }
                  }
               }
            }
         }

         // remove the force from all particles using the force
         for (i = 0; i < numPS; i++) {
            psFX.ID(particles[i]);
            psFX.RemoveForce(gxID);
         }

         // move the FX to trash can
         curFX->DeleteFX(gxID, FALSE);

         // update the UIs
         TbUpdateCurrentFXUIs();         
         return TRUE;
      }


      /*--------------------------------
        delete a game FX command - UnDo
        C.Wang 0917, 2014
       ---------------------------------*/
      void TbDeleteGameFXCommand::Undo()
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         // restore the game FX back
         curFX->RestoreFX(gxID);

         // apply the force back to all particles
         int i;
         FnParticlesGameFX psFX;

         for (i = 0; i < numPS; i++) {
            psFX.ID(particles[i]);
            psFX.ApplyForce(gxID);
         }

         // make it as current editing FX
         curFX->MakeCurrentGameFX(gxID);

         // update the UIs
         TbUpdateCurrentFXUIs();
      }


// set parent object for current game FX command
class TbSetFXParentCommand : public FcCommandBase
{
protected:
   OBJECTid lastParent;
   char pName[256];

public:
   virtual void Init() { lastParent = FAILED_ID;
                         pName[0] = '\0';
                         SetCommandName("SetFXParent"); }    // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};

      /*--------------------------------------
        set parent for a game FX command - Do
        C.Wang 0117, 2013
       ---------------------------------------*/
      BOOL4 TbSetFXParentCommand::Do(char *arg)
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         // backup current FX's name
         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         lastParent = gx.GetParentObject();
         char *pNamePtr = gx.GetParentObjectName();
         if (pNamePtr != NULL) {
            strcpy(pName, pNamePtr);
         }
         else {
            pName[0] = '\0';
         }

         // set new parent object
         OBJECTid oID = tbCurScene->GetObjectByName(arg);
         gx.SetParentObject(oID, arg);

         // update UIs
         char name[256];
         strcpy(name, arg);
         FnUiCombo uiC(tbSetFXParentID);
         uiC.ClearMessages();
         uiC.InsertMessage(0, name);
         uiC.PasteComboValue(name);

         return TRUE;
      }

      /*----------------------------------------
        set parent for a game FX command - Undo
        C.Wang 0102, 2013
       -----------------------------------------*/
      void TbSetFXParentCommand::Undo()
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         // recover current FX's name
         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return;

         FnBaseGameFX gx(gxID);
         gx.SetParentObject(lastParent, pName);

         // update UIs
         FnObject model(lastParent);
         FnUiCombo uiC(tbSetFXParentID);
         uiC.PasteComboValue(model.GetName());
         uiC.ClearMessages();
         uiC.InsertMessage(0, model.GetName());
      }


// rename current game FX command
class TbRenameGameFXCommand : public FcCommandBase
{
protected:
   char lastCurrentFXName[256];

public:
   virtual void Init() { lastCurrentFXName[0] = '\0';
                         SetCommandName("RenameGameFX"); }    // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                               // the next model command
   virtual void Undo();                                       // undo
};

      /*------------------------------
        rename a game FX command - Do
        C.Wang 1003, 2012
       -------------------------------*/
      BOOL4 TbRenameGameFXCommand::Do(char *arg)
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         // backup current FX's name
         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         strcpy(lastCurrentFXName, gx.GetName());

         // rename !
         gx.SetName(arg);

         // update the UIs
         TbUpdateCurrentFXUIs();         
         return TRUE;
      }

      /*--------------------------------
        rename a game FX command - UnDo
        C.Wang 1003, 2012
       ---------------------------------*/
      void TbRenameGameFXCommand::Undo()
      {
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         // recover current FX's name
         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return;

         FnBaseGameFX gx(gxID);
         gx.SetName(lastCurrentFXName);

         // update the UIs
         TbUpdateCurrentFXUIs();
      }


// change plate type command
class TbChangePlateTypeCommand : public FcCommandBase
{
protected:
   DWORD lastType;

public:
   virtual void Init() { lastType = PLATE_ZUP;
                         SetCommandName("ChangePlateType"); }  // initialize the command
   virtual void DeInit() { };                                  // deinitialize the command
   virtual BOOL4 Do(char *arg);                                // the next model command
   virtual void Undo();                                        // undo
};

      /*-------------------------------
        change plate type command - Do
        C.Wang 0115, 2013
       --------------------------------*/
      BOOL4 TbChangePlateTypeCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return FALSE;

         DWORD newType;
         float size[2], color[4];
         char texName[256], alphaName[256];
         int numTex;

         FnPlateGameFX plate(gxID);
         lastType = plate.GetPlateType();
         plate.GetTextureData(texName, &numTex);
         plate.GetAlphamapData(alphaName);

         if (FyCheckWord(arg, "Plate Z-up")) {
            newType = PLATE_ZUP;
         }
         else if (FyCheckWord(arg, "Plate Y-up")) {
            newType = PLATE_YUP;
         }
         else {
            newType = BILLBOARD;
         }

         plate.GetPlateSize(size);
         plate.GetPlateColor(color);
         plate.CreatePlate(newType, size, color, texName, numTex, FY_NONE, alphaName);

         return TRUE;
      }

      /*---------------------------------
        change plate type command - UnDo
        C.Wang 1005, 2012
       ----------------------------------*/
      void TbChangePlateTypeCommand::Undo()
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return;

         char texName[256];
         int numTex;
         float size[2], color[4];
         FnPlateGameFX plate(gxID);

         plate.GetTextureData(texName, &numTex);
         plate.GetPlateSize(size);
         plate.GetPlateColor(color);
         plate.CreatePlate(lastType, size, color, texName, numTex, 10);

         // update UIs
         FnUiCombo uiC(tbPlateFXTypeID);
         if (lastType == PLATE_ZUP) {
            uiC.PasteComboValue("Plate Z-up");
         }
         else if (lastType == PLATE_YUP) {
            uiC.PasteComboValue("Plate Y-up");
         }
         else {
            uiC.PasteComboValue("Billboard");
         }
      }


// change plate blending type command
class TbChangePlateBlendTypeCommand : public FcCommandBase
{
protected:
   DWORD lastType;

public:
   virtual void Init() { lastType = ADD_COLOR;
                         SetCommandName("ChangePlateBlendType"); }  // initialize the command
   virtual void DeInit() { };                                       // deinitialize the command
   virtual BOOL4 Do(char *arg);                                     // the next model command
   virtual void Undo();                                             // undo
};

      /*----------------------------------------
        change plate blending type command - Do
        C.Wang 1008, 2012
       -----------------------------------------*/
      BOOL4 TbChangePlateBlendTypeCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return FALSE;

         DWORD newType;

         FnPlateGameFX plate(gxID);
         lastType = plate.GetBlendingType();

         if (FyCheckWord(arg, "Alpha")) {
            newType = ALPHA_BLENDING;
         }
         else if (FyCheckWord(arg, "Subtract Color")) {
            newType = SUBTRACT_COLOR;
         }
         else {
            newType = ADD_COLOR;
         }

         plate.SetBlendingType(newType);

         return TRUE;
      }

      /*------------------------------------------
        change plate blending type command - UnDo
        C.Wang 1008, 2012
       -------------------------------------------*/
      void TbChangePlateBlendTypeCommand::Undo()
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return;

         FnPlateGameFX plate(gxID);
         plate.SetBlendingType(lastType);

         // update UIs
         FnUiCombo uiC(tbPlateFXBlendTypeID);
         if (lastType == ALPHA_BLENDING) {
            uiC.PasteComboValue("Alpha");
         }
         else if (lastType == SUBTRACT_COLOR) {
            uiC.PasteComboValue("Subtract Color");
         }
         else {
            uiC.PasteComboValue("Add Color");
         }
      }


// load plate textures command
class TbLoadPlateTextureCommand : public FcCommandBase
{
protected:
   char texturePath[256];
   int numTex;
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         texturePath[0] = '\0';
                         numTex = 0;
                         SetCommandName("LoadPlateTexture"); }  // initialize the command
   virtual void DeInit() { };                                   // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the next model command
   virtual void Undo();                                         // undo
};

      /*---------------------------------
        load plate textures command - Do
        C.Wang 0114, 2013
       ----------------------------------*/
      BOOL4 TbLoadPlateTextureCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return FALSE;

         FnPlateGameFX plate(gxID);

         // backup texture information
         plate.GetTextureData(texturePath, &numTex);

         // get texture path
         char *texPath = FyGetTexturePath();
   
         // load new textures
         int num;
         char *key, *key2, argument[256], texName[256], alphaName[256];

         strcpy(argument, arg);
         key = strtok(argument, " ");
         num = 0;
         texName[0] = '\0';
         alphaName[0] = '\0';
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(texName, key2);
               }
            }
            else if (FyCheckWord, "-number") {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  num = atoi(key2);
                  if (num < 0 || num >= 10000) num = 0;
               }
            }
            key = strtok(NULL, " ");
         }
         if (strlen(texName) == 0) return FALSE;

         // load the texture
         BOOL4 beOK = FALSE;
         if (num == 0) {
            // case using full texture name
            beOK = plate.LoadTextures(texName, 0);
         }
         else {
            // extract texture head
            int len = (int) strlen(texName);
            char *checkName;
            if (len > 4) {
               checkName = texName + len - 4;
               if (TbCheckInt(checkName)) {
                  checkName = texName;
                  checkName[len - 4] = '\0';
               }
               else {
                  checkName = texName;
               }
            }
            else {
               checkName = texName;
            }

            // case using texture animation
            beOK = plate.LoadTextures(checkName, num);
         }

         if (beOK) {
            int n;
            char tName[256], value[256];
            plate.GetTextureData(tName, &n);

            // update UIs
            FnUiTypein uiTy;
            uiTy.ID(tbPlateFXTextureNameID);
            FyCheckFileFormat(tName, NULL, value, NULL);
            uiTy.PasteTypeinValue(value);

            sprintf(value, "%d", n);
            uiTy.ID(tbPlateFXTextureNumberID);
            uiTy.PasteTypeinValue(value);
         }

         return beOK;
      }

      /*-----------------------------------
        load plate textures command - Undo
        C.Wang 0114, 2013
       ------------------------------------*/
      void TbLoadPlateTextureCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnPlateGameFX plate(gxID);
         plate.LoadTextures(texturePath, numTex);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbPlateFXTextureNameID);
         FyCheckFileFormat(texturePath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);

         sprintf(value, "%d", numTex);
         uiTy.ID(tbPlateFXTextureNumberID);
         uiTy.PasteTypeinValue(value);
      }


// load plate alphamap command
class TbLoadPlateAlphamapCommand : public FcCommandBase
{
protected:
   char alphaPath[256];
   int numTex;
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         alphaPath[0] = '\0';
                         numTex = 0;
                         SetCommandName("LoadPlateAlphamap"); } // initialize the command
   virtual void DeInit() { };                                   // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the next model command
   virtual void Undo();                                         // undo
};

      /*---------------------------------
        load plate alphamap command - Do
        C.Wang 0114, 2013
       ----------------------------------*/
      BOOL4 TbLoadPlateAlphamapCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return FALSE;

         FnPlateGameFX plate(gxID);

         // backup texture information
         plate.GetAlphamapData(alphaPath);

         // get texture path
         char *texPath = FyGetTexturePath();
   
         // load new textures
         int num;
         char *key, *key2, argument[256], texName[256], alphaName[256];

         strcpy(argument, arg);
         key = strtok(argument, " ");
         num = 0;
         texName[0] = '\0';
         alphaName[0] = '\0';
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(texName, key2);
               }
            }
            key = strtok(NULL, " ");
         }
         if (strlen(texName) == 0) return FALSE;

         // load the texture
         BOOL4 beOK = FALSE;
         plate.GetTextureData(NULL, &num);

         if (num == 0) {
            // case using full texture name
            beOK = plate.LoadAlphamaps(texName);
         }
         else {
            // extract texture head
            int len = (int) strlen(texName);
            char *checkName;
            if (len > 4) {
               checkName = texName + len - 4;
               if (TbCheckInt(checkName)) {
                  checkName = texName;
                  checkName[len - 4] = '\0';
               }
               else {
                  checkName = texName;
               }
            }
            else {
               checkName = texName;
            }

            // case using alphamap animation
            beOK = plate.LoadAlphamaps(checkName);
         }

         if (beOK) {
            char tName[256], value[256];
            plate.GetAlphamapData(tName);

            // update UIs
            FnUiTypein uiTy;
            uiTy.ID(tbPlateFXAlphamapNameID);
            FyCheckFileFormat(tName, NULL, value, NULL);
            uiTy.PasteTypeinValue(value);
         }

         return beOK;
      }

      /*-----------------------------------
        load plate alphamap command - Undo
        C.Wang 0114, 2013
       ------------------------------------*/
      void TbLoadPlateAlphamapCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnPlateGameFX plate(gxID);
         plate.LoadAlphamaps(alphaPath);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbPlateFXAlphamapNameID);
         FyCheckFileFormat(alphaPath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);
      }


// remove/delete plate textures command
class TbRemovePlateTextureCommand : public FcCommandBase
{
protected:
   char alphaPath[256];
   char texturePath[256];
   int numTex;
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         alphaPath[0] = '\0';
                         texturePath[0] = '\0';
                         numTex = 0;
                         SetCommandName("RemovePlateTexture"); }  // initialize the command
   virtual void DeInit() { };                                     // deinitialize the command
   virtual BOOL4 Do(char *arg);                                   // the next model command
   virtual void Undo();                                           // undo
};

      /*-----------------------------------
        remove plate textures command - Do
        C.Wang 0114, 2013
       ------------------------------------*/
      BOOL4 TbRemovePlateTextureCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return FALSE;

         FnPlateGameFX plate(gxID);

         // backup texture information
         plate.GetTextureData(texturePath, &numTex);
         plate.GetAlphamapData(alphaPath);

         // remove the texture
         plate.LoadTextures("", 0);
         plate.LoadAlphamaps("");

         // update UIs
         FnUiTypein uiTy;
         uiTy.ID(tbPlateFXTextureNameID);
         uiTy.PasteTypeinValue("");

         uiTy.ID(tbPlateFXTextureNumberID);
         uiTy.PasteTypeinValue("0");

         uiTy.ID(tbPlateFXAlphamapNameID);
         uiTy.PasteTypeinValue("");
         return TRUE;
      }

      /*-------------------------------------
        remove plate textures command - Undo
        C.Wang 0114, 2013
       --------------------------------------*/
      void TbRemovePlateTextureCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnPlateGameFX plate(gxID);
         plate.LoadTextures(texturePath, numTex);
         plate.LoadAlphamaps(alphaPath);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbPlateFXTextureNameID);
         FyCheckFileFormat(texturePath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);

         sprintf(value, "%d", numTex);
         uiTy.ID(tbPlateFXTextureNumberID);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbPlateFXAlphamapNameID);
         FyCheckFileFormat(alphaPath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);
      }


// load audio file command
class TbGameFXAudioLoadFileCommand : public FcCommandBase
{
protected:
   char audioPath[256];
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         audioPath[0] = '\0';
                         SetCommandName("LoadGameFXAudio"); }  // initialize the command
   virtual void DeInit() { };                                  // deinitialize the command
   virtual BOOL4 Do(char *arg);                                // the next model command
   virtual void Undo();                                        // undo
};

      /*---------------------------------
        load audio command - Do
        Eric Fei 1228, 2012
       ----------------------------------*/
      BOOL4 TbGameFXAudioLoadFileCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != AUDIO_FX) return FALSE;

         if (arg == NULL) return FALSE;

         FnAudioGameFX audio(gxID);

         // backup audio information
         audio.GetAudioName(audioPath);
   
         // get new audio name
         char *key, *key2, argument[256], audName[256];
         audName[0] = '\0';

         strcpy(argument, arg);
         key = strtok(argument, " ");
         if (FyCheckWord(key, "-name")) {
            key2 = strtok(NULL, " ");
            if (key2 != NULL)
               strcpy(audName, key2);
         }
         if (strlen(audName) == 0) return FALSE;

         // load the audio
         char fullPath[256];
         GetCurrentDirectory(256, fullPath);
         strcat(fullPath, "\\");
         strcat(fullPath, audName);
         BOOL4 beOK = audio.LoadAudio(fullPath);
         if (beOK) {
            char value[256], ext[32];
            audio.GetAudioName(fullPath);
            FyCheckFileFormat(fullPath, NULL, value, ext);

            // update UIs
            FnUiTypein uiTy;
            uiTy.ID(tbAudioFXNameID);
            if (strlen(value) != 0) {
               strcat(value, ".");
            }
            strcat(value, ext);
            uiTy.PasteTypeinValue(value);
         }

         return beOK;
      }

      /*-----------------------------------
        load audio command - UnDo
        Eric Fei 0102, 2013
       ------------------------------------*/
      void TbGameFXAudioLoadFileCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the audio
         FnAudioGameFX audio(gxID);
         audio.LoadAudio(audioPath);

         // update UIs
         char value[256], ext[32];
         FyCheckFileFormat(audioPath, NULL, value, ext);

         FnUiTypein uiTy;
         uiTy.ID(tbAudioFXNameID);
         if (strlen(value) != 0) {
            strcat(value, ".");
         }
         strcat(value, ext);
         uiTy.PasteTypeinValue(value);
      }

      
// change start frame of audio command
class TbGameFXAudioChangeStartCommand : public FcCommandBase
{
protected:
   float backupStartFrame;
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         backupStartFrame = 0.0f;
                         SetCommandName("ChangeGameFXAudioStartTime"); }   // initialize the command
   virtual void DeInit() { };                                              // deinitialize the command
   virtual BOOL4 Do(char *arg);                                            // the next model command
   virtual void Undo();                                                    // undo
};

   
      /*---------------------------------------
        change start frame of audio command - Do
        Eric Fei 1228, 2012
       ----------------------------------------*/
      BOOL4 TbGameFXAudioChangeStartCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != AUDIO_FX) return FALSE;

         if (arg == NULL) return FALSE;

         FnAudioGameFX audio(gxID);

         // backup audio information
         audio.GetAudioStartFrame(&backupStartFrame);
   
         // get new start frame
         char *key, *key2, argument[256], aStartFrame[256];
         aStartFrame[0] = '\0';

         strcpy(argument, arg);
         key = strtok(argument, " ");
         if (FyCheckWord(key, "-s")) {
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               strcpy(aStartFrame, key2);
            }
            else {
               return FALSE;
            }
         }
         else {
            return FALSE;
         }

         // set the new start frame
         float fStartFrame = float(atof(aStartFrame));
         audio.SetAudioStartFrame(fStartFrame);

         // update UIs
         char value[256];
         sprintf(value, "%5.2f", fStartFrame);
         FnUiTypein uiTy;
         uiTy.ID(tbAudioFXStartID);
         uiTy.PasteTypeinValue(value);

         return TRUE;
      }

      /*------------------------------------------
        change start frame of audio command - UnDo
        Eric Fei 0102, 2013
       -------------------------------------------*/
      void TbGameFXAudioChangeStartCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the backup start frame
         FnAudioGameFX audio(gxID);
         audio.SetAudioStartFrame(backupStartFrame);

         // update UIs
         char value[256];
         sprintf(value, "%5.2f", backupStartFrame);

         FnUiTypein uiTy;
         uiTy.ID(tbAudioFXStartID);
         uiTy.PasteTypeinValue(value);
      }


class TbGameFXSwordChangeCharacterCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   CHARACTERid backupCharID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         backupCharID = FAILED_ID;
                         SetCommandName("ChangeGameFXSwordCharacter"); }   // initialize the command
   virtual void DeInit() { };                                              // deinitialize the command
   virtual BOOL4 Do(char *arg);                                            // the next model command
   virtual void Undo();                                                    // undo
};

      /*---------------------------------------
        change character of sword command - Do
        Eric Fei 0109, 2013
       ----------------------------------------*/
      BOOL4 TbGameFXSwordChangeCharacterCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != SWORD_FX) return FALSE;

         if (arg == NULL) return FALSE;

         FnSwordGameFX sword(gxID);

         // backup current character
         char lastCharacterName[256];
         sword.GetCharacterName(lastCharacterName);
         backupCharID = tbCurScene->GetCharacterByName(lastCharacterName);

         // get new character's name
         char *key, *key2, argument[256], newCharacterName[256];
         newCharacterName[0] = '\0';

         strcpy(argument, arg);
         key = strtok(argument, " ");
         if (FyCheckWord(key, "-char")) {
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               strcpy(newCharacterName, key2);
            }
            else {
               strcpy(newCharacterName, "-");
            }
         }
         else {
            return FALSE;
         }

         // set current character
         sword.SetCharacterName(newCharacterName);

         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);

         return TRUE;
      }

      /*------------------------------------------
        change character of sword command - UnDo
        Eric Fei 0109, 2013
       -------------------------------------------*/
      void TbGameFXSwordChangeCharacterCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the backup character
         FnSwordGameFX sword(gxID);
         FnCharacter character(backupCharID);

         if (character.ID() != FAILED_ID)
            sword.SetCharacterName(character.GetName());
         
         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);
      }


class TbGameFXSwordChangeDummyCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   OBJECTid backupDmy1ID;
   OBJECTid backupDmy2ID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         backupDmy1ID = FAILED_ID;
                         backupDmy2ID = FAILED_ID;
                         SetCommandName("ChangeGameFXSwordDummy"); }       // initialize the command
   virtual void DeInit() { };                                              // deinitialize the command
   virtual BOOL4 Do(char *arg);                                            // the next model command
   virtual void Undo();                                                    // undo
};

      /*-------------------------------------
        change dummy 1 of sword command - Do
        Eric Fei 0109, 2013
       --------------------------------------*/
      BOOL4 TbGameFXSwordChangeDummyCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != SWORD_FX) return FALSE;

         if (arg == NULL) return FALSE;

         FnSwordGameFX sword(gxID);

         // backup current dummy1/dummy2
         char lastDummy1Name[256], lastDummy2Name[256];
         sword.GetDummyObject1Name(lastDummy1Name);
         sword.GetDummyObject2Name(lastDummy2Name);
         backupDmy1ID = tbCurScene->GetObjectByName(lastDummy1Name);
         backupDmy2ID = tbCurScene->GetObjectByName(lastDummy2Name);

         // get new dummy's name
         char *key, argument[256], newDummy1Name[256], newDummy2Name[256];
         newDummy1Name[0] = '\0';
         newDummy2Name[0] = '\0';

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-dmy1")) {
               key = strtok(NULL, " ");
               if (key != NULL) {
                  if (FyCheckWord(key, "-dmy2")) {
                     break;
                  }
                  else {
                     strcpy(newDummy1Name, key);
                  }
               }
               else {
                  strcpy(newDummy1Name, "-");
               }
            }
            else if (FyCheckWord(key, "-dmy2")) {
               key = strtok(NULL, " ");
               if (key != NULL) {
                  if (FyCheckWord(key, "-dmy1")) {
                     break;
                  }
                  else {
                     strcpy(newDummy2Name, key);
                  }
               }
               else {
                  strcpy(newDummy2Name, "-");
               }
            }
            key = strtok(NULL, " ");
         }

         // set current character
         if (strlen(newDummy1Name) != 0) {
            OBJECTid oID = tbCurScene->GetObjectByName(newDummy1Name);
            sword.SetDummyObject1(oID);
         }

         if (strlen(newDummy2Name) != 0) {
            OBJECTid oID = tbCurScene->GetObjectByName(newDummy2Name);
            sword.SetDummyObject2(oID);
         }

         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);

         return TRUE;
      }
      
      /*---------------------------------------
        change dummy 2 of sword command - UnDo
        Eric Fei 0109, 2013
       ----------------------------------------*/
      void TbGameFXSwordChangeDummyCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the backup character
         FnSwordGameFX sword(gxID);

         sword.SetDummyObject1(backupDmy1ID);
         sword.SetDummyObject2(backupDmy2ID);

         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);
      }


class TbGameFXSwordChangeParamCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   BOOL4 backupState;
   int backupBladeMaxNum;
   int backupBladeDivide;
   int backupBladeSegment;
   DWORD backupBlendType;
   float backupColor[4];
   float backupStart;
   float backupEnd;

public:
   virtual void Init() { gxID = FAILED_ID;
                         backupState = TRUE;
                         backupBladeMaxNum = 0;
                         backupBladeDivide = 0;
                         backupBlendType = 0;
                         backupColor[0] = 0.0f;
                         backupColor[1] = 0.0f;
                         backupColor[2] = 0.0f;
                         backupColor[3] = 0.0f;
                         backupStart = 0.0f;
                         backupEnd = 0.0f;
                         SetCommandName("ChangeGameFXSwordParam"); }       // initialize the command
   virtual void DeInit() { };                                              // deinitialize the command
   virtual BOOL4 Do(char *arg);                                            // the next model command
   virtual void Undo();                                                    // undo
};

      /*---------------------------------------
        change parameters of sword command - Do
        Eric Fei 0119, 2013
       ----------------------------------------*/
      BOOL4 TbGameFXSwordChangeParamCommand::Do(char *arg)
      {
         if (arg == NULL) return FALSE;

         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != SWORD_FX) return FALSE;
         FnSwordGameFX sword(gxID);

         // backup current dummy1/dummy2
         backupState       = sword.GetState();
         backupBladeMaxNum = sword.GetBladeMaxNum();
         backupBladeDivide = sword.GetDivideNum();
         backupBlendType   = sword.GetBlendingType();
         sword.GetColor(backupColor);

         // get new dummy's name
         char *key, argument[256], newDummy1Name[256], newDummy2Name[256];
         newDummy1Name[0] = '\0';
         newDummy2Name[0] = '\0';

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while(key != NULL) {
            if (FyCheckWord(key, "-enable")) {              // Enable Sword FX
               sword.SetState(TRUE);
            }
            else if (FyCheckWord(key, "-disable")) {         // Disable Sword FX
               sword.SetState(FALSE);
            }
            else if (FyCheckWord(key, "-tlen")) {           // Change Tail Length
               key = strtok(NULL, " ");
               if (key != NULL) {
                  if (FyCheckWord(key, "-", TRUE, 1)) {
                     continue;
                  }
                  else {
                     sword.SetBladeMaxNum(atoi(key));
                  }
               }
               else {
                  return FALSE;
               }
            }

            else if (FyCheckWord(key, "-tdiv")) {     // Change Tail Division
               key = strtok(NULL, " ");
               if (key != NULL) {
                  if (FyCheckWord(key, "-", TRUE, 1)) {
                     continue;
                  }
                  else {
                     sword.SetDivideNum(atoi(key));
                  }
               }
               else {
                  return FALSE;
               }
            }

            else if (FyCheckWord(key, "-tseg")) {     // Change Tail Segment
               key = strtok(NULL, " ");
               if (key != NULL) {
                  if (FyCheckWord(key, "-", TRUE, 1)) {
                     continue;
                  }
                  else {
                     sword.SetSegmentNum(atoi(key));
                  }
               }
               else {
                  return FALSE;
               }
            }
            else if (FyCheckWord(key, "-blend")) {    // Change Blend Type
               key = strtok(NULL, " ");

               DWORD newType;
               if (FyCheckWord(key, "Alpha")) newType = ALPHA_BLENDING;
               else if (FyCheckWord(key, "Subtract")) newType = SUBTRACT_COLOR;
               else newType = ADD_COLOR;

               sword.SetBlendingType(newType);
            }

            else if (FyCheckWord(key, "-color")) {    // Change Color
               float color[4] = { backupColor[0], backupColor[1], backupColor[2], backupColor[3] };
               int   idx = 0;
               key = strtok(NULL, " ");
               while (key != NULL) {
                  if (FyCheckWord(key, "-", TRUE, 1)) {
                     break;
                  }
                  else {
                     color[idx] = float(atof(key));
                     idx++;
                  }
                  key = strtok(NULL, " ");
               }
               sword.SetColor(color);
               continue;
            }
            key = strtok(NULL, " ");
         }

         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);

         return TRUE;
      }
      
      /*------------------------------------------
        change parameters of sword command - UnDo
        Eric Fei 0109, 2013
       -------------------------------------------*/
      void TbGameFXSwordChangeParamCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the backup character
         FnSwordGameFX sword(gxID);
         
         sword.SetState(backupState);
         sword.SetBladeMaxNum(backupBladeMaxNum);
         sword.SetDivideNum(backupBladeDivide);
         sword.SetSegmentNum(backupBladeSegment);
         sword.SetBlendingType(backupBlendType);
         sword.SetColor(backupColor);
         
         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);
      }
    

class TbGameFXSwordChangeTextureCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   char     backupTexName[256];
   int      backupTexNum;

public:
   virtual void Init() { gxID = FAILED_ID;
                         backupTexName[0] = '\0';
                         backupTexNum = 0;
                         SetCommandName("ChangeGameFXSwordTexture"); }     // initialize the command
   virtual void DeInit() { };                                              // deinitialize the command
   virtual BOOL4 Do(char *arg);                                            // the next model command
   virtual void Undo();                                                    // undo
};

      /*---------------------------------------
        change parameters of sword command - Do
        Eric Fei 0115, 2013
       ----------------------------------------*/
      BOOL4 TbGameFXSwordChangeTextureCommand::Do(char *arg)
      {
         if (arg == NULL) return FALSE;

         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != SWORD_FX) return FALSE;
         FnSwordGameFX sword(gxID);

         // backup current texutre information
         sword.GetTextureInfo(backupTexName, &backupTexNum);

         // get new dummy's name
         char *key, *key2, argument[256];
         argument[0] = '\0';

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while(key != NULL) {
            if (FyCheckWord(key, "-tex")) {
               // Add/Change Texture
               key  = strtok(NULL, " ");
               key2 = strtok(NULL, " ");
               if (key == NULL || key2 == NULL) return FALSE;

               sword.LoadTextures(key, atoi(key2));
            }
            else if (FyCheckWord(key, "-rmtex")) {
               // Remove Texture
               sword.LoadTextures("", 0);
            }
            key = strtok(NULL, " ");
         }

         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);

         return TRUE;
      }
      
      /*------------------------------------------
        change parameters of sword command - UnDo
        Eric Fei 0109, 2013
       -------------------------------------------*/
      void TbGameFXSwordChangeTextureCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the backup character
         FnSwordGameFX sword(gxID);
         sword.LoadTextures(backupTexName, backupTexNum);
         
         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);
      }
      

class TbGameFXSwordChangeAlphamapCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   char backupAlpahmapName[256];

public:
   virtual void Init() { gxID = FAILED_ID;
                         backupAlpahmapName[0] = '\0';
                         SetCommandName("ChangeGameFXSwordTexture"); }     // initialize the command
   virtual void DeInit() { };                                              // deinitialize the command
   virtual BOOL4 Do(char *arg);                                            // the next model command
   virtual void Undo();                                                    // undo
};

      /*---------------------------------------
        change parameters of sword command - Do
        Eric Fei 0115, 2013
       ----------------------------------------*/
      BOOL4 TbGameFXSwordChangeAlphamapCommand::Do(char *arg)
      {
         if (arg == NULL) return FALSE;

         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != SWORD_FX) return FALSE;

         FnSwordGameFX sword(gxID);

         // backup current texutre information
         sword.GetAlphamapInfo(backupAlpahmapName);

         // get new dummy's name
         char *key, argument[256];
         argument[0] = '\0';

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while(key != NULL) {
            if (FyCheckWord(key, "-alpha")) {
               // Add/Change Alpha Map
               key = strtok(NULL, " ");
               if (key == NULL) return FALSE;

               sword.LoadAlphamaps(key);
            }
            else if (FyCheckWord(key, "-rmalpha")) {
               // Remove Alpha Map
               sword.LoadAlphamaps("");
            }
            key = strtok(NULL, " ");
         }

         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);

         return TRUE;
      }
      
      /*------------------------------------------
        change parameters of sword command - UnDo
        Eric Fei 0109, 2013
       -------------------------------------------*/
      void TbGameFXSwordChangeAlphamapCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the backup character
         FnSwordGameFX sword(gxID);
         sword.LoadAlphamaps(backupAlpahmapName);
         
         // update Sword FX UIs
         TbUpdateSwordFXUIs(gxID);
      }


// modify base FX visibility command
class TbModifyBaseFXVisibilityCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldV;

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldV = 1.0f;
                         SetCommandName("ModifyBaseFXVisibility"); }  // initialize the command
   virtual void DeInit() { };                                         // deinitialize the command
   virtual BOOL4 Do(char *arg);                                       // the next model command
   virtual void Undo();                                               // undo
};

      /*---------------------------------------
        modify base FX visibility command - Do
        C.Wang 1220, 2012
       ----------------------------------------*/
      BOOL4 TbModifyBaseFXVisibilityCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);

         // backup visibility information
         oldV = gx.GetVisibility();

         // extract the new visibility
         char *key1, argument[256];
         float newV;

         strcpy(argument, arg);
         key1 = strtok(argument, " ");
         newV = 0.0f;
         if (key1 != NULL) {
            newV = (float) atof(key1);
         }

         // set new visibility
         gx.SetVisibility(newV, TRUE);
         return TRUE;
      }


      /*-----------------------------------------
        modify base FX visibility command - UnDo
        C.Wang 1220, 2012
       ------------------------------------------*/
      void TbModifyBaseFXVisibilityCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the size
         FnBaseGameFX gx(gxID);
         gx.SetVisibility(oldV);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbBaseFXShowID);
         sprintf(value, "%f", oldV);
         uiTy.PasteTypeinValue(value);
      }


// modify base FX life command
class TbModifyBaseFXLifeCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldL;
   BOOL4 beA;

public:
   virtual void Init() { gxID = FAILED_ID;
                         beA = FALSE;
                         oldL = 500.0f;
                         SetCommandName("ModifyBaseFXLife"); }  // initialize the command
   virtual void DeInit() { };                                   // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the next model command
   virtual void Undo();                                         // undo
};

      /*---------------------------------
        modify base FX life command - Do
        C.Wang 1231, 2012
       ----------------------------------*/
      BOOL4 TbModifyBaseFXLifeCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);

         // backup life information
         oldL = gx.GetLife();

         // extract the new life
         char *key1, *key2, argument[256];
         float newL;
         BOOL4 beAuto = FALSE;

         strcpy(argument, arg);
         key1 = strtok(argument, " ");
         newL = 500.0f;
         if (key1 != NULL) {
            newL = (float) atof(key1);
         }
         if (newL < 0) newL = 1.0f;

         key2 = strtok(NULL, " ");
         if (key2 != NULL) {
            if (FyCheckWord(key2, "Auto")) {
               beAuto = TRUE;
            }
         }
         beA = beAuto;

         // set new life
         gx.AdjustLife(newL, beAuto);
         return TRUE;
      }


      /*-----------------------------------
        modify base FX life command - UnDo
        C.Wang 1231, 2012
       ------------------------------------*/
      void TbModifyBaseFXLifeCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the size
         FnBaseGameFX gx(gxID);
         gx.AdjustLife(oldL, beA);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbBaseLifeID);
         sprintf(value, "%d", (int) oldL);
         uiTy.PasteTypeinValue(value);
      }


// modify base FX rotation command
class TbModifyBaseFXRotationCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldRotation[3];

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldRotation[0] = oldRotation[1] = oldRotation[2] = 0.0f;
                         SetCommandName("ModifyBaseFXRotation"); }  // initialize the command
   virtual void DeInit() { };                                       // deinitialize the command
   virtual BOOL4 Do(char *arg);                                     // the next model command
   virtual void Undo();                                             // undo
};

      /*-------------------------------------
        modify base FX rotation command - Do
        C.Wang 1220, 2012
       --------------------------------------*/
      BOOL4 TbModifyBaseFXRotationCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);

         // backup rotation information
         gx.GetRotation(oldRotation);

         // extract the new rotation
         char *key1, *key2, *key3, argument[256];
         float newRotation[3];

         strcpy(argument, arg);
         key1 = strtok(argument, " ");
         newRotation[0] = newRotation[1] = newRotation[2] = 0.0f;
         if (key1 != NULL) {
            newRotation[0] = (float) atof(key1);
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               newRotation[1] = (float) atof(key2);
               key3 = strtok(NULL, " ");
               if (key3 != NULL) {
                  newRotation[2] = (float) atof(key3);
               }
            }
         }

         // set new rotation
         gx.SetRotation(newRotation);
         return TRUE;
      }


      /*---------------------------------------
        modify base FX rotation command - UnDo
        C.Wang 1220, 2012
       ----------------------------------------*/
      void TbModifyBaseFXRotationCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the size
         FnBaseGameFX gx(gxID);
         gx.SetPosition(oldRotation);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbBaseFXFXRotateID[0]);
         sprintf(value, "%f", oldRotation[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbBaseFXFXRotateID[1]);
         sprintf(value, "%f", oldRotation[1]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbBaseFXFXRotateID[2]);
         sprintf(value, "%f", oldRotation[2]);
         uiTy.PasteTypeinValue(value);
      }


// modify base FX position command
class TbModifyBaseFXPositionCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldPosition[3];

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldPosition[0] = oldPosition[1] = oldPosition[2] = 40.0f;
                         SetCommandName("ModifyBaseFXPosition"); }  // initialize the command
   virtual void DeInit() { };                                       // deinitialize the command
   virtual BOOL4 Do(char *arg);                                     // the next model command
   virtual void Undo();                                             // undo
};

      /*-------------------------------------
        modify base FX position command - Do
        C.Wang 1212, 2012
       --------------------------------------*/
      BOOL4 TbModifyBaseFXPositionCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);

         // backup position information
         gx.GetPosition(oldPosition);

         // extract the new position
         char *key1, *key2, *key3, argument[256];
         float newPosition[3];

         strcpy(argument, arg);
         key1 = strtok(argument, " ");
         newPosition[0] = newPosition[1] = newPosition[2] = 0.0f;
         if (key1 != NULL) {
            newPosition[0] = (float) atof(key1);
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               newPosition[1] = (float) atof(key2);
               key3 = strtok(NULL, " ");
               if (key3 != NULL) {
                  newPosition[2] = (float) atof(key3);
               }
            }
         }

         // set new position
         gx.SetPosition(newPosition);
         return TRUE;
      }

      /*---------------------------------------
        modify base FX position command - UnDo
        C.Wang 1212, 2012
       ----------------------------------------*/
      void TbModifyBaseFXPositionCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the size
         FnBaseGameFX gx(gxID);
         gx.SetPosition(oldPosition);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbBaseFXFXPosID[0]);
         sprintf(value, "%f", oldPosition[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbBaseFXFXPosID[1]);
         sprintf(value, "%f", oldPosition[1]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbBaseFXFXPosID[2]);
         sprintf(value, "%f", oldPosition[2]);
         uiTy.PasteTypeinValue(value);
      }


// modify plate size command
class TbModifyPlateSizeCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldSize[2];

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldSize[0] = oldSize[1] = 40.0f;
                         SetCommandName("ModifyPlateSize"); }  // initialize the command
   virtual void DeInit() { };                                  // deinitialize the command
   virtual BOOL4 Do(char *arg);                                // the next model command
   virtual void Undo();                                        // undo
};

      /*-------------------------------
        modify plate size command - Do
        C.Wang 1008, 2012
       --------------------------------*/
      BOOL4 TbModifyPlateSizeCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return FALSE;

         FnPlateGameFX plate(gxID);

         // backup plate size information
         plate.GetPlateSize(oldSize);

         // extract the new size
         char *key, *key2, argument[256];
         float newSize[2];

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while (key != NULL) {
            if (FyCheckWord(key, "-width")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  newSize[0] = (float) atof(key2);
               }
            }
            else if (FyCheckWord, "-height") {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  newSize[1] = (float) atof(key2);
               }
            }
            key = strtok(NULL, " ");
         }

         // set new size
         plate.SetPlateSize(newSize);
         return TRUE;
      }

      /*---------------------------------
        modify plate size command - UnDo
        C.Wang 1008, 2012
       ----------------------------------*/
      void TbModifyPlateSizeCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the size
         FnPlateGameFX plate(gxID);
         plate.SetPlateSize(oldSize);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbPlateFXSizeID[0]);
         sprintf(value, "%f", oldSize[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbPlateFXSizeID[1]);
         sprintf(value, "%f", oldSize[1]);
         uiTy.PasteTypeinValue(value);
      }


// modify plate color command
class TbModifyPlateColorCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldColor[4];

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldColor[0] = oldColor[1] = oldColor[2] = oldColor[3] = 1.0f;
                         SetCommandName("ModifyPlateColor"); }  // initialize the command
   virtual void DeInit() { };                                   // deinitialize the command
   virtual BOOL4 Do(char *arg);                                 // the next model command
   virtual void Undo();                                         // undo
};

      /*--------------------------------
        modify plate color command - Do
        C.Wang 1008, 2012
       ---------------------------------*/
      BOOL4 TbModifyPlateColorCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PLATE_FX) return FALSE;

         FnPlateGameFX plate(gxID);

         // backup plate color information
         plate.GetPlateColor(oldColor);

         // extract the new size
         char *key, *key2, *key3, *key4, argument[256];
         float newColor[4];

         newColor[0] = newColor[1] = newColor[2] = newColor[3] = 1.0f;
         strcpy(argument, arg);
         key = strtok(argument, " ");
         if (key != NULL) {
            newColor[0] = (float) atof(key);
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               newColor[1] = (float) atof(key2);
               key3 = strtok(NULL, " ");
               if (key3 != NULL) {
                  newColor[2] = (float) atof(key3);
                  key4 = strtok(NULL, " ");
                  if (key4 != NULL) {
                     newColor[3] = (float) atof(key4);
                  }
               }
            }
         }

         // set new color
         plate.SetPlateColor(newColor);
         return TRUE;
      }

      /*----------------------------------
        modify plate color command - UnDo
        C.Wang 1008, 2012
       -----------------------------------*/
      void TbModifyPlateColorCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the size
         FnPlateGameFX plate(gxID);
         plate.SetPlateColor(oldColor);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbPlateFXColorID[0]);
         sprintf(value, "%5.4f", oldColor[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbPlateFXColorID[1]);
         sprintf(value, "%5.4f", oldColor[1]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbPlateFXColorID[2]);
         sprintf(value, "%5.4f", oldColor[2]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbPlateFXColorID[3]);
         sprintf(value, "%5.4f", oldColor[3]);
         uiTy.PasteTypeinValue(value);
      }


// modify particle life command
class TbModifyParticleLifeCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldLife[2];

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldLife[0] = oldLife[1] = 0.0f;
                         SetCommandName("ModifyParticleLife"); }  // initialize the command
   virtual void DeInit() { };                                     // deinitialize the command
   virtual BOOL4 Do(char *arg);                                   // the next model command
   virtual void Undo();                                           // undo
};

      /*----------------------------------
        modify particle life command - Do
        C.Wang 1224, 2012
       -----------------------------------*/
      BOOL4 TbModifyParticleLifeCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         FnParticlesGameFX par(gxID);

         // backup life information
         par.GetParticleLife(oldLife);

         // extract the new life data
         char *key1, *key2, argument[256];
         float newLife[2];

         strcpy(argument, arg);
         key1 = strtok(argument, " ");
         newLife[0] = newLife[1] = 0.0f;
         if (key1 != NULL) {
            newLife[0] = (float) atof(key1);
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               newLife[1] = (float) atof(key2);
            }
         }

         // set new life
         par.SetParticleLife(newLife);
         return TRUE;
      }

      /*------------------------------------
        modify particle life command - Undo
        C.Wang 1224, 2012
       -------------------------------------*/
      void TbModifyParticleLifeCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the life
         FnParticlesGameFX par(gxID);

         // set new life data
         par.SetParticleLife(oldLife);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbParticleLifeID[0]);
         sprintf(value, "%7.1f", oldLife[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleLifeID[1]);
         sprintf(value, "%7.1f", oldLife[1]);
         uiTy.PasteTypeinValue(value);
      }


// modify particle size command
class TbModifyParticleSizeCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldSize[2];

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldSize[0] = oldSize[1] = 40.0f;
                         SetCommandName("ModifyParticleSize"); }  // initialize the command
   virtual void DeInit() { };                                     // deinitialize the command
   virtual BOOL4 Do(char *arg);                                   // the next model command
   virtual void Undo();                                           // undo
};

      /*----------------------------------
        modify particle size command - Do
        C.Wang 1224, 2012
       -----------------------------------*/
      BOOL4 TbModifyParticleSizeCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         FnParticlesGameFX par(gxID);

         // backup size information
         par.GetParticleSize(oldSize);

         // extract the new size data
         char *key1, *key2, argument[256];
         float newSize[2];

         strcpy(argument, arg);
         key1 = strtok(argument, " ");
         newSize[0] = newSize[1] = 40.0f;
         if (key1 != NULL) {
            newSize[0] = (float) atof(key1);
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               newSize[1] = (float) atof(key2);
            }
         }

         // set new size
         par.SetParticleSize(newSize);
         return TRUE;
      }

      /*------------------------------------
        modify particle size command - Undo
        C.Wang 1224, 2012
       -------------------------------------*/
      void TbModifyParticleSizeCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the size
         FnParticlesGameFX par(gxID);

         // set new size data
         par.SetParticleSize(oldSize);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbParticleSizeID[0]);
         sprintf(value, "%7.1f", oldSize[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleSizeID[1]);
         sprintf(value, "%7.1f", oldSize[1]);
         uiTy.PasteTypeinValue(value);
      }


// modify particle velocity command
class TbModifyParticleVelocityCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   float oldV[6];

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldV[0] = oldV[1] = oldV[2] = oldV[3] = oldV[4] = oldV[5] = 0.0f;
                         SetCommandName("ModifyParticleVelocity"); }  // initialize the command
   virtual void DeInit() { };                                         // deinitialize the command
   virtual BOOL4 Do(char *arg);                                       // the next model command
   virtual void Undo();                                               // undo
};

      /*--------------------------------------
        modify particle velocity command - Do
        C.Wang 1226, 2012
       ---------------------------------------*/
      BOOL4 TbModifyParticleVelocityCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         FnParticlesGameFX par(gxID);

         // backup velocity information
         par.GetParticleVelocity(&oldV[0], &oldV[3]);

         // extract the new velocity data
         char *key0, *key1, *key2, *key3, *key4, *key5, argument[256];
         float newV[6];

         strcpy(argument, arg);
         key0 = strtok(argument, " ");
         newV[0] = newV[1] = newV[2] = 0.0f;
         newV[3] = newV[4] = newV[5] = 10.0f;
         if (key0 != NULL) {
            newV[0] = (float) atof(key0);
            key1 = strtok(NULL, " ");
            if (key1 != NULL) {
               newV[1] = (float) atof(key1);
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  newV[2] = (float) atof(key2);
                  key3 = strtok(NULL, " ");
                  if (key3 != NULL) {
                     newV[3] = (float) atof(key3);
                     key4 = strtok(NULL, " ");
                     if (key4 != NULL) {
                        newV[4] = (float) atof(key4);
                        key5 = strtok(NULL, " ");
                        if (key5 != NULL) {
                           newV[5] = (float) atof(key5);
                        }
                     }
                  }
               }
            }
         }

         // set new velocity
         par.SetParticleVelocity(&newV[0], &newV[3]);
         return TRUE;
      }

      /*----------------------------------------
        modify particle velocity command - Undo
        C.Wang 1226, 2012
       -----------------------------------------*/
      void TbModifyParticleVelocityCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the life
         FnParticlesGameFX par(gxID);

         // recover the velocity data
         par.SetParticleVelocity(&oldV[0], &oldV[3]);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbParticleVelocityID[0]);
         sprintf(value, "%8.2f", oldV[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleVelocityID[1]);
         sprintf(value, "%8.2f", oldV[1]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleVelocityID[2]);
         sprintf(value, "%8.2f", oldV[2]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleVelocityVarID[0]);
         sprintf(value, "%8.2f", oldV[3]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleVelocityVarID[1]);
         sprintf(value, "%8.2f", oldV[4]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleVelocityVarID[2]);
         sprintf(value, "%8.2f", oldV[5]);
         uiTy.PasteTypeinValue(value);
      }


// modify particle texture animation command
class TbModifyParticleTexAnimCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   int texAnimNum;
   int texAnim[2];

public:
   virtual void Init() { gxID = FAILED_ID;
                         texAnimNum = texAnim[0] = texAnim[1] = 1;
                         SetCommandName("ModifyParticleTextureAnimation"); }  // initialize the command
   virtual void DeInit() { };                                                 // deinitialize the command
   virtual BOOL4 Do(char *arg);                                               // the next model command
   virtual void Undo();                                                       // undo
};

      /*-----------------------------------------------
        modify particle texture animation command - Do
        C.Wang 0109, 2013
       ------------------------------------------------*/
      BOOL4 TbModifyParticleTexAnimCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         FnParticlesGameFX par(gxID);

         // backup texture animation information
         par.GetTextureAnimation(&texAnimNum, texAnim);

         // extract the new life data
         char *key1, *key2, *key3, argument[256];
         int texAnimNumNew;
         int texAnimNew[2];

         texAnimNumNew = 1;
         texAnimNew[0] = texAnimNew[1] = 1;

         strcpy(argument, arg);
         key1 = strtok(argument, " ");
         if (key1 != NULL) {
            texAnimNumNew = (int) atoi(key1);
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               texAnimNew[0] = (int) atoi(key2);
               key3 = strtok(NULL, " ");
               if (key3 != NULL) {
                  texAnimNew[1] = (int) atoi(key3);
               }
            }
         }

         // set new texture animation data
         par.SetupTextureAnimation(texAnimNumNew, texAnimNew);

         return TRUE;
      }

      /*-------------------------------------------------
        modify particle texture animation command - Undo
        C.Wang 1224, 2012
       --------------------------------------------------*/
      void TbModifyParticleTexAnimCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the life
         FnParticlesGameFX par(gxID);

         // set new life data
         par.SetupTextureAnimation(texAnimNum, texAnim);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbParticleTextureAnimID[0]);
         sprintf(value, "%d", texAnimNum);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleTextureAnimID[1]);
         sprintf(value, "%d", texAnim[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleTextureAnimID[2]);
         sprintf(value, "%d", texAnim[1]);
         uiTy.PasteTypeinValue(value);
      }


// modify particle born command
class TbModifyParticleBornCommand : public FcCommandBase
{
protected:
   GAMEFXid gxID;
   int oldBorn;

public:
   virtual void Init() { gxID = FAILED_ID;
                         oldBorn = 0;
                         SetCommandName("ModifyParticleBorn"); }  // initialize the command
   virtual void DeInit() { };                                     // deinitialize the command
   virtual BOOL4 Do(char *arg);                                   // the next model command
   virtual void Undo();                                           // undo
};


      /*----------------------------------
        modify particle born command - Do
        C.Wang 1224, 2012
       -----------------------------------*/
      BOOL4 TbModifyParticleBornCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         FnParticlesGameFX par(gxID);

         // backup born information
         oldBorn = par.GetParticleBornPerFrame();

         // extract the new born data
         char *key1, argument[256];
         int newBorn;

         strcpy(argument, arg);
         key1 = strtok(argument, " ");
         newBorn = 1;
         if (key1 != NULL) {
            newBorn = atoi(key1);
         }

         // set new born number
         par.SetParticleBornPerFrame(newBorn);
         return TRUE;
      }

      /*------------------------------------
        modify particle born command - Undo
        C.Wang 1224, 2012
       -------------------------------------*/
      void TbModifyParticleBornCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the born number
         FnParticlesGameFX par(gxID);

         // set new born number
         par.SetParticleBornPerFrame(oldBorn);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbParticleBornID[0]);
         sprintf(value, "%d", oldBorn);
         uiTy.PasteTypeinValue(value);
      }


// load particle textures command
class TbLoadParticleTextureCommand : public FcCommandBase
{
protected:
   char texturePath[256];
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         texturePath[0] = '\0';
                         SetCommandName("LoadParticleTexture"); }  // initialize the command
   virtual void DeInit() { };                                      // deinitialize the command
   virtual BOOL4 Do(char *arg);                                    // the next model command
   virtual void Undo();                                            // undo
};

      /*---------------------------------
        load plate textures command - Do
        C.Wang 1225, 2012
       ----------------------------------*/
      BOOL4 TbLoadParticleTextureCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         FnParticlesGameFX pPX(gxID);

         // backup texture information
         pPX.GetTextureData(texturePath);

         // get texture path
         char *texPath = FyGetTexturePath();
   
         // load new textures
         char *key, *key2, argument[256], texName[256];

         strcpy(argument, arg);
         key = strtok(argument, " ");
         texName[0] = '\0';
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(texName, key2);
               }
            }
            key = strtok(NULL, " ");
         }
         if (strlen(texName) == 0) return FALSE;

         // load the texture
         BOOL4 beOK = pPX.LoadTextures(texName);
         if (beOK) {
            char tName[256], value[256];
            pPX.GetTextureData(tName);

            // update UIs
            FnUiTypein uiTy;
            uiTy.ID(tbParticleTextureNameID);
            FyCheckFileFormat(tName, NULL, value, NULL);
            uiTy.PasteTypeinValue(value);
         }

         return beOK;
      }

      /*--------------------------------------
        load particle textures command - UnDo
        C.Wang 1225, 2012
       ---------------------------------------*/
      void TbLoadParticleTextureCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnParticlesGameFX pPX(gxID);
         pPX.LoadTextures(texturePath);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbParticleTextureNameID);
         FyCheckFileFormat(texturePath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);
      }


// load particle alphamap command
class TbLoadParticleAlphamapCommand : public FcCommandBase
{
protected:
   char alphamapPath[256];
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         alphamapPath[0] = '\0';
                         SetCommandName("LoadParticleAlphamap"); }  // initialize the command
   virtual void DeInit() { };                                       // deinitialize the command
   virtual BOOL4 Do(char *arg);                                     // the next model command
   virtual void Undo();                                             // undo
};

      /*---------------------------------
        load plate alphamap command - Do
        C.Wang 0115, 2013
       ----------------------------------*/
      BOOL4 TbLoadParticleAlphamapCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         FnParticlesGameFX pPX(gxID);

         // backup texture information
         pPX.GetAlphamapData(alphamapPath);

         // get texture path
         char *texPath = FyGetTexturePath();
   
         // load new textures
         char *key, *key2, argument[256], texName[256];

         strcpy(argument, arg);
         key = strtok(argument, " ");
         texName[0] = '\0';
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(texName, key2);
               }
            }
            key = strtok(NULL, " ");
         }
         if (strlen(texName) == 0) return FALSE;

         // load the texture
         BOOL4 beOK = pPX.LoadAlphamaps(texName);
         if (beOK) {
            char tName[256], value[256];
            pPX.GetAlphamapData(tName);

            // update UIs
            FnUiTypein uiTy;
            uiTy.ID(tbParticleAlphamapNameID);
            FyCheckFileFormat(tName, NULL, value, NULL);
            uiTy.PasteTypeinValue(value);
         }

         return beOK;
      }

      /*--------------------------------------
        load particle alphamap command - UnDo
        C.Wang 0115, 2013
       ---------------------------------------*/
      void TbLoadParticleAlphamapCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnParticlesGameFX pPX(gxID);
         pPX.LoadAlphamaps(alphamapPath);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbParticleAlphamapNameID);
         FyCheckFileFormat(alphamapPath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);
      }


// remove/delete paticle textures command
class TbRemoveParticleTextureCommand : public FcCommandBase
{
protected:
   char texturePath[256];
   char alphamapPath[256];
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         texturePath[0] = '\0';
                         alphamapPath[0] = '\0';
                         SetCommandName("RemoveTextureTexture"); }  // initialize the command
   virtual void DeInit() { };                                       // deinitialize the command
   virtual BOOL4 Do(char *arg);                                     // the next model command
   virtual void Undo();                                             // undo
};


      /*--------------------------------------
        remove particle textures command - Do
        C.Wang 0115, 2013
       ---------------------------------------*/
      BOOL4 TbRemoveParticleTextureCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         FnParticlesGameFX pPX(gxID);

         // backup texture information
         pPX.GetTextureData(texturePath);
         pPX.GetTextureData(alphamapPath);

         // remove the textures
         pPX.LoadTextures("");
         pPX.LoadAlphamaps("");

         // update UIs
         FnUiTypein uiTy;
         uiTy.ID(tbParticleTextureNameID);
         uiTy.PasteTypeinValue("");
         uiTy.ID(tbParticleAlphamapNameID);
         uiTy.PasteTypeinValue("");

         return TRUE;
      }

      /*----------------------------------------
        remove particle textures command - UnDo
        C.Wang 1225, 2012
       -----------------------------------------*/
      void TbRemoveParticleTextureCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnParticlesGameFX pFX(gxID);
         pFX.LoadTextures(texturePath);
         pFX.LoadAlphamaps(alphamapPath);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbParticleTextureNameID);
         FyCheckFileFormat(texturePath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleAlphamapNameID);
         FyCheckFileFormat(alphamapPath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);
      }


// change particle blending type command
class TbChangeParticleBlendTypeCommand : public FcCommandBase
{
protected:
   DWORD lastType;

public:
   virtual void Init() { lastType = ADD_COLOR;
                         SetCommandName("ChangeParticleBlendType"); }  // initialize the command
   virtual void DeInit() { };                                          // deinitialize the command
   virtual BOOL4 Do(char *arg);                                        // the next model command
   virtual void Undo();                                                // undo
};

      /*-------------------------------------------
        change particle blending type command - Do
        C.Wang 1224, 2012
       --------------------------------------------*/
      BOOL4 TbChangeParticleBlendTypeCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return FALSE;

         DWORD newType;

         FnParticlesGameFX par(gxID);
         lastType = par.GetBlendingType();

         if (FyCheckWord(arg, "Alpha")) {
            newType = ALPHA_BLENDING;
         }
         else if (FyCheckWord(arg, "Subtract Color")) {
            newType = SUBTRACT_COLOR;
         }
         else {
            newType = ADD_COLOR;
         }

         par.SetBlendingType(newType);

         return TRUE;
      }

      /*---------------------------------------------
        change particle blending type command - UnDo
        C.Wang 1224, 2012
       ----------------------------------------------*/
      void TbChangeParticleBlendTypeCommand::Undo()
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != PARTICLES_FX) return;

         FnParticlesGameFX par(gxID);
         par.SetBlendingType(lastType);

         // update UIs
         FnUiCombo uiC(tbParticleBlendTypeID);
         if (lastType == ALPHA_BLENDING) {
            uiC.PasteComboValue("Alpha");
         }
         else if (lastType == SUBTRACT_COLOR) {
            uiC.PasteComboValue("Subtract Color");
         }
         else {
            uiC.PasteComboValue("Add Color");
         }
      }


// change particle system using the force command
class TbChangeParticleUsedCommand : public FcCommandBase
{
protected:
   GAMEFXid lastPS;

public:
   virtual void Init() { lastPS = FAILED_ID;
                         SetCommandName("ChangeForceParticle"); }  // initialize the command
   virtual void DeInit() { };                                      // deinitialize the command
   virtual BOOL4 Do(char *arg);                                    // the next model command
   virtual void Undo();                                            // undo
};

      /*----------------------------------------------------
        change particle system using the force command - Do
        C.Wang 0101, 2013
       -----------------------------------------------------*/
      BOOL4 TbChangeParticleUsedCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != FORCE_FX) return FALSE;

         GAMEFXid newPS;

         // backup last particle system used
         FnParticlesGameFX pFX;
         FnForceGameFX fFX(gxID);
         lastPS = fFX.UsedBy();

         if (FyCheckWord(arg, "-")) {
            newPS = FAILED_ID;

            // remove the force from last particle FX
            pFX.ID(lastPS);
            pFX.RemoveForce(gxID);
         }
         else {
            newPS = curFX->GetGameFXByName(arg);
            if (newPS == lastPS) return FALSE;

            // remove the force from last particle FX
            pFX.ID(lastPS);
            pFX.RemoveForce(gxID);

            // apply the force to the new particle system
            pFX.ID(newPS);
            pFX.ApplyForce(gxID);
         }

         return TRUE;
      }

      /*------------------------------------------------------
        change particle system using the force command - Undo
        C.Wang 0101, 2013
       -------------------------------------------------------*/
      void TbChangeParticleUsedCommand::Undo()
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != FORCE_FX) return;

         FnParticlesGameFX pFX;
         FnForceGameFX fFX(gxID);

         GAMEFXid newPS;

         // backup last particle system used
         newPS = fFX.UsedBy();
         pFX.ID(newPS);
         pFX.RemoveForce(gxID);

         FnUiCombo uiC(tbForceUsedByID);
         if (lastPS == FAILED_ID) {
            uiC.PasteComboValue("-");
         }
         else {
            pFX.ID(lastPS);
            pFX.ApplyForce(gxID);

            FnBaseGameFX base(lastPS);
            uiC.PasteComboValue(base.GetName());
         }
      }


// change the force parameter command
class TbModifyForceParamCommand : public FcCommandBase
{
protected:
   float lastMagnitude;       // Backup of Force Magnitude
   float lastMinDistance;     // Backup of Minimum Distance for Vortex/Vortex2D
   DWORD lastType;            // Backup of Force Type

public:
   virtual void Init() { lastMagnitude = 32.0f;
                         lastMinDistance = 1.0f;
                         lastType = GRAVITY;
                         SetCommandName("ModifyForceMagnitude"); }  // initialize the command
   virtual void DeInit() { };                                       // deinitialize the command
   virtual BOOL4 Do(char *arg);                                     // the next model command
   virtual void Undo();                                             // undo
};

      /*----------------------------------------
        change the force magnitude command - Do
        C.Wang 0101, 2013
        Eric Fei 0128, 2013
       -----------------------------------------*/
      BOOL4 TbModifyForceParamCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != FORCE_FX) return FALSE;

         // backup last force parameter
         FnForceGameFX fFX(gxID);
         lastMagnitude = fFX.GetForceMagnitude();
         lastMinDistance = fFX.GetForceMinDistance();
         lastType = fFX.GetForceType();

         // check the new value
         char *key, *key2, argument[256];
         DWORD type;

         strcpy(argument, arg);
         key = strtok(argument, " ");
         while(key != NULL) {
            if (FyCheckWord(key, "-type")) {                    // Chagne Force FX Type
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  if(FyCheckWord(key2, "Gravity")) type = GRAVITY;
                  else if(FyCheckWord(key2, "Parallel")) type = PARALLEL;
                  else if(FyCheckWord(key2, "SINK")) type = SINK;
                  else if(FyCheckWord(key2, "Sphere")) type = SPHERE;
                  else if(FyCheckWord(key2, "Vortex")) type = VORTEX;
                  else if(FyCheckWord(key2, "Vortex2D")) type = VORTEX_2D;
                  else if(FyCheckWord(key2, "Viscosity")) type = VISCOSITY;
                  else type = USER_DEFINED;

                  fFX.SetForceType(type);
               }
            }
            else if (FyCheckWord(key, "-mag")) {               // Change Force FX Magnitude
               key2 = strtok(NULL, " ");
               if (key2 != NULL) fFX.SetForceMagnitude(float(atof(key2)));
            }
            else if (FyCheckWord(key, "-dist")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) fFX.SetForceMinDistance(float(atof(key2)));
            }
            key = strtok(NULL, " ");
         }

         // update UI
         TbUpdateCurrentFXUIs();

         return TRUE;
      }

      /*------------------------------------------
        change the force parameter command - Undo
        C.Wang 0101, 2013
        Eric Fei 0128, 2013
       -------------------------------------------*/
      void TbModifyForceParamCommand::Undo()
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != FORCE_FX) return;

         // recover setting
         FnForceGameFX fFX(gxID);
         fFX.SetForceMagnitude(lastMagnitude);
         fFX.SetForceMinDistance(lastMinDistance);
         fFX.SetForceType(lastType);

         // update UI
         TbUpdateCurrentFXUIs();
      }


// load geometry FX's model command
class TbLoadGeometryFXModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupID;
   GAMEFXid gxID;
   char fullPath[256];

public:
   virtual void Init() { backupID = FAILED_ID;
                         gxID = FAILED_ID;
                         fullPath[0] = '0';
                         SetCommandName("LoadGeometryFXModel"); }  // initialize the command
   virtual void DeInit() { };                                      // deinitialize the command
   virtual BOOL4 Do(char *arg);                                    // the next model command
   virtual void Undo();                                            // undo
};

      /*------------------------------------
        load geometry FX model command - Do
        C.Wang 0114, 2013
       -------------------------------------*/
      BOOL4 TbLoadGeometryFXModelCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != GEOMETRY_FX) return FALSE;

         FnGeometryGameFX gFX(gxID);

         // backup model information
         backupID = gFX.GetModelObject();
         strcpy(fullPath, gFX.GetModelFullName());

         // create the new geometry object
         OBJECTid newID = gFX.Initialize(FALSE);
         FnObject newO(newID);

         // load the file
         char dir[256], openName[256];
         OPENFILENAME ofn;
         BOOL4 beOK = FALSE;
         GEOMETRYid geoID;
         MATERIALid mID;

         // get current directory
         GetCurrentDirectory(256, dir);

         // prepare to open the file browser
         openName[0] = '\0';
         memset(&ofn, 0, sizeof(OPENFILENAME));
         ofn.lStructSize = sizeof(OPENFILENAME);
         ofn.hwndOwner = NULL;
         ofn.lpstrFilter = "Fly2 Model CW3 {*.cw3}\0*.cw3\0"\
                           "Fly2 Model in Maya OBJ {*.obj}\0*.obj\0" \
                           "Fly2 Model in Binary CW4 {*.cw4}\0*.cw4\0"\
                           "All files {*.*}\0*.*\0\0";
         ofn.lpstrFile = openName;
         ofn.nMaxFile = 256;
         ofn.Flags = OFN_FILEMUSTEXIST;
         ofn.lpstrDefExt = "cw3";
         ofn.lpstrInitialDir = dir;

         // start to load the model file
         if (GetOpenFileName(&ofn)) {
            // set the extra directory for data loading
            char name[256];
            GetCurrentDirectory(256, name);
            FySetDataPath(name);

            // load the model
            newO.Load(openName);

            // success to get the file name
            beOK = TRUE;
         }

         // restore the current directory
         if (tbBeRestoreCurrentDir) {
            SetCurrentDirectory(dir);
         }

         if (!beOK) {
            gFX.SetModelObject(backupID, NULL, TRUE);
         }
         else {
            // paste the new name and path
            char *getPath = gFX.GetModelFullName();
            strcpy(getPath, openName);

            char name[256];
            FyCheckFileFormat(openName, NULL, name, NULL);

            // paste new object name
            FnUiTypein uiTy(tbGeometryNameFXID);
            uiTy.PasteTypeinValue(name);

            // update the texture information
            char string[256];
            geoID = newO.GetGeometryID(0);
            FnGeometry tri(geoID);

            mID = tri.GetMaterial();
            FnMaterial mat(mID);
            int numTex = mat.GetTextureSlotNumber();
            sprintf(string, "%d", numTex);
            
            TEXTUREid texID = mat.GetTexture(0, 0);
            FnTexture tex(texID);

#ifdef FLY2_DX9
            mat.AddShaderEffect("Fly2GameFXShader", "GeometryTex2");
#else
            mat.AddShaderEffect("GeometryTex2");
#endif

            if (texID != FAILED_ID) {
               char *texPath = tex.GetPathName();
               if (texPath != NULL) {
                  char fileName[256], name[256];
                  strcpy(fileName, texPath);
                  FyCheckFileFormat(fileName, NULL, name, NULL);

                  uiTy.ID(tbGeometryFXTextureNumberID);
                  uiTy.PasteTypeinValue(string);

                  uiTy.ID(tbGeometryFXTextureNameID);
                  uiTy.PasteTypeinValue(name);
               }
               else {
                  uiTy.ID(tbGeometryFXTextureNumberID);
                  uiTy.PasteTypeinValue("");
                  uiTy.ID(tbGeometryFXTextureNameID);
                  uiTy.PasteTypeinValue("-");
               }
            }
            else {
               uiTy.ID(tbGeometryFXTextureNumberID);
               uiTy.PasteTypeinValue("");
               uiTy.ID(tbGeometryFXTextureNameID);
               uiTy.PasteTypeinValue("-");
            }

            texID = mat.GetTexture(0, 1);
            tex.ID(texID);
            if (texID != FAILED_ID) {
               char *texPath = tex.GetPathName();
               if (texPath != NULL) {
                  char fileName[256], name[256];
                  strcpy(fileName, texPath);
                  FyCheckFileFormat(fileName, NULL, name, NULL);

                  uiTy.ID(tbGeometryFXAlphamapNameID);
                  uiTy.PasteTypeinValue(name);
               }
               else {
                  uiTy.ID(tbGeometryFXAlphamapNameID);
                  uiTy.PasteTypeinValue("-");
               }
            }
            else {
               uiTy.ID(tbGeometryFXAlphamapNameID);
               uiTy.PasteTypeinValue("-");
            }
         }
   
         return beOK;
      }

      /*--------------------------------------
        load geometry FX model command - Undo
        C.Wang 0113, 2013
       ---------------------------------------*/
      void TbLoadGeometryFXModelCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         FnObject backObj(backupID);

         // recover the model
         FnGeometryGameFX gFX(gxID);
         gFX.SetModelObject(backupID, fullPath, TRUE);

         // update UIs
         FnUiTypein uiTy(tbGeometryNameFXID);
         uiTy.PasteTypeinValue(backObj.GetName());
      }


// change geometry blending type command
class TbChangeGeometryBlendTypeCommand : public FcCommandBase
{
protected:
   DWORD lastType;

public:
   virtual void Init() { lastType = ADD_COLOR;
                         SetCommandName("ChangeGeometryBlendType"); } // initialize the command
   virtual void DeInit() { };                                         // deinitialize the command
   virtual BOOL4 Do(char *arg);                                       // the next model command
   virtual void Undo();                                               // undo
};

      /*-------------------------------------------
        change geometry blending type command - Do
        C.Wang 0116, 2013
       --------------------------------------------*/
      BOOL4 TbChangeGeometryBlendTypeCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != GEOMETRY_FX) return FALSE;

         DWORD newType;

         FnGeometryGameFX gFX(gxID);
         lastType = gFX.GetBlendingType();

         if (FyCheckWord(arg, "Alpha")) {
            newType = ALPHA_BLENDING;
         }
         else if (FyCheckWord(arg, "Subtract Color")) {
            newType = SUBTRACT_COLOR;
         }
         else {
            newType = ADD_COLOR;
         }

         gFX.SetBlendingType(newType);

         return TRUE;
      }

      /*---------------------------------------------
        change geometry blending type command - UnDo
        C.Wang 0116, 2013
       ----------------------------------------------*/
      void TbChangeGeometryBlendTypeCommand::Undo()
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return;

         GAMEFXid gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != GEOMETRY_FX) return;

         FnGeometryGameFX gFX(gxID);
         gFX.SetBlendingType(lastType);

         // update UIs
         FnUiCombo uiC(tbGeometryFXBlendTypeID);
         if (lastType == ALPHA_BLENDING) {
            uiC.PasteComboValue("Alpha");
         }
         else if (lastType == SUBTRACT_COLOR) {
            uiC.PasteComboValue("Subtract Color");
         }
         else {
            uiC.PasteComboValue("Add Color");
         }
      }


// load geometry textures command
class TbLoadGeometryTextureCommand : public FcCommandBase
{
protected:
   char texturePath[256];
   int numTex;
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         texturePath[0] = '\0';
                         numTex = 0;
                         SetCommandName("LoadGeometryTexture"); } // initialize the command
   virtual void DeInit() { };                                     // deinitialize the command
   virtual BOOL4 Do(char *arg);                                   // the next model command
   virtual void Undo();                                           // undo
};

      /*------------------------------------
        load geometry textures command - Do
        C.Wang 0116, 2013
       -------------------------------------*/
      BOOL4 TbLoadGeometryTextureCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != GEOMETRY_FX) return FALSE;

         FnGeometryGameFX gFX(gxID);

         // backup texture information
         gFX.GetTextureData(texturePath, &numTex);

         // get texture path
         char *texPath = FyGetTexturePath();
   
         // load new textures
         int num;
         char *key, *key2, argument[256], texName[256], alphaName[256];

         strcpy(argument, arg);
         key = strtok(argument, " ");
         num = 0;
         texName[0] = '\0';
         alphaName[0] = '\0';
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(texName, key2);
               }
            }
            else if (FyCheckWord, "-number") {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  num = atoi(key2);
                  if (num < 0 || num >= 10000) num = 0;
               }
            }
            key = strtok(NULL, " ");
         }
         if (strlen(texName) == 0) return FALSE;

         // load the texture
         BOOL4 beOK = FALSE;
         if (num == 0) {
            // case using full texture name
            beOK = gFX.LoadTextures(texName, 0);
         }
         else {
            // extract texture head
            int len = (int) strlen(texName);
            char *checkName;
            if (len > 4) {
               checkName = texName + len - 4;
               if (TbCheckInt(checkName)) {
                  checkName = texName;
                  checkName[len - 4] = '\0';
               }
               else {
                  checkName = texName;
               }
            }
            else {
               checkName = texName;
            }

            // case using texture animation
            beOK = gFX.LoadTextures(checkName, num);
         }

         if (beOK) {
            int n;
            char tName[256], value[256];
            gFX.GetTextureData(tName, &n);

            // update UIs
            FnUiTypein uiTy;
            uiTy.ID(tbGeometryFXTextureNameID);
            FyCheckFileFormat(tName, NULL, value, NULL);
            uiTy.PasteTypeinValue(value);

            sprintf(value, "%d", n);
            uiTy.ID(tbGeometryFXTextureNumberID);
            uiTy.PasteTypeinValue(value);
         }

         return beOK;
      }

      /*--------------------------------------
        load geometry textures command - Undo
        C.Wang 0116, 2013
       ---------------------------------------*/
      void TbLoadGeometryTextureCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnGeometryGameFX gFX(gxID);
         gFX.LoadTextures(texturePath, numTex);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbGeometryFXTextureNameID);
         FyCheckFileFormat(texturePath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);

         sprintf(value, "%d", numTex);
         uiTy.ID(tbGeometryFXTextureNumberID);
         uiTy.PasteTypeinValue(value);
      }


// load geometry alphamap command
class TbLoadGeometryAlphamapCommand : public FcCommandBase
{
protected:
   char alphaPath[256];
   int numTex;
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         alphaPath[0] = '\0';
                         numTex = 0;
                         SetCommandName("LoadGeometryAlphamap"); } // initialize the command
   virtual void DeInit() { };                                      // deinitialize the command
   virtual BOOL4 Do(char *arg);                                    // the next model command
   virtual void Undo();                                            // undo
};

      /*------------------------------------
        load geometry alphamap command - Do
        C.Wang 0114, 2013
       -------------------------------------*/
      BOOL4 TbLoadGeometryAlphamapCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != GEOMETRY_FX) return FALSE;

         FnGeometryGameFX gFX(gxID);

         // backup texture information
         gFX.GetAlphamapData(alphaPath);

         // get texture path
         char *texPath = FyGetTexturePath();
   
         // load new textures
         int num;
         char *key, *key2, argument[256], texName[256], alphaName[256];

         strcpy(argument, arg);
         key = strtok(argument, " ");
         num = 0;
         texName[0] = '\0';
         alphaName[0] = '\0';
         while (key != NULL) {
            if (FyCheckWord(key, "-name")) {
               key2 = strtok(NULL, " ");
               if (key2 != NULL) {
                  strcpy(texName, key2);
               }
            }
            key = strtok(NULL, " ");
         }
         if (strlen(texName) == 0) return FALSE;

         // load the texture
         BOOL4 beOK = FALSE;
         gFX.GetTextureData(NULL, &num);

         if (num == 0) {
            // case using full texture name
            beOK = gFX.LoadAlphamaps(texName);
         }
         else {
            // extract texture head
            int len = (int) strlen(texName);
            char *checkName;
            if (len > 4) {
               checkName = texName + len - 4;
               if (TbCheckInt(checkName)) {
                  checkName = texName;
                  checkName[len - 4] = '\0';
               }
               else {
                  checkName = texName;
               }
            }
            else {
               checkName = texName;
            }

            // case using alphamap animation
            beOK = gFX.LoadAlphamaps(checkName);
         }

         if (beOK) {
            char tName[256], value[256];
            gFX.GetAlphamapData(tName);

            // update UIs
            FnUiTypein uiTy;
            uiTy.ID(tbGeometryFXAlphamapNameID);
            FyCheckFileFormat(tName, NULL, value, NULL);
            uiTy.PasteTypeinValue(value);
         }

         return beOK;
      }

      /*--------------------------------------
        load geometry alphamap command - Undo
        C.Wang 0116, 2013
       ---------------------------------------*/
      void TbLoadGeometryAlphamapCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnGeometryGameFX gFX(gxID);
         gFX.LoadAlphamaps(alphaPath);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbGeometryFXAlphamapNameID);
         FyCheckFileFormat(alphaPath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);
      }


// remove/delete geometry textures command
class TbRemoveGeometryTextureCommand : public FcCommandBase
{
protected:
   char alphaPath[256];
   char texturePath[256];
   int numTex;
   GAMEFXid gxID;

public:
   virtual void Init() { gxID = FAILED_ID;
                         alphaPath[0] = '\0';
                         texturePath[0] = '\0';
                         numTex = 0;
                         SetCommandName("RemoveGeometryTexture"); } // initialize the command
   virtual void DeInit() { };                                       // deinitialize the command
   virtual BOOL4 Do(char *arg);                                     // the next model command
   virtual void Undo();                                             // undo
};

      /*--------------------------------------
        remove geometry textures command - Do
        C.Wang 0116, 2013
       ---------------------------------------*/
      BOOL4 TbRemoveGeometryTextureCommand::Do(char *arg)
      {
         // get current FX
         TBGameFXSystem *curFX;
         curFX = tbCurScene->GetCurrentTBGameFXSystem();
         if (curFX == NULL) return FALSE;

         gxID = curFX->GetCurrentGameFX();
         if (gxID == FAILED_ID) return FALSE;

         FnBaseGameFX gx(gxID);
         if (gx.GetFXType() != GEOMETRY_FX) return FALSE;

         FnGeometryGameFX gFX(gxID);

         // backup texture information
         gFX.GetTextureData(texturePath, &numTex);
         gFX.GetAlphamapData(alphaPath);

         // remove the texture
         gFX.LoadTextures("", 0);
         gFX.LoadAlphamaps("");

         // update UIs
         FnUiTypein uiTy;
         uiTy.ID(tbGeometryFXTextureNameID);
         uiTy.PasteTypeinValue("");

         uiTy.ID(tbGeometryFXTextureNumberID);
         uiTy.PasteTypeinValue("0");

         uiTy.ID(tbGeometryFXAlphamapNameID);
         uiTy.PasteTypeinValue("");
         return TRUE;
      }

      /*----------------------------------------
        remove geometry textures command - Undo
        C.Wang 0116, 2013
       -----------------------------------------*/
      void TbRemoveGeometryTextureCommand::Undo()
      {
         if (gxID == FAILED_ID) return;

         // recover the textures
         FnGeometryGameFX gFX(gxID);
         gFX.LoadTextures(texturePath, numTex);
         gFX.LoadAlphamaps(alphaPath);

         // update UIs
         char value[256];
         FnUiTypein uiTy;
         uiTy.ID(tbGeometryFXTextureNameID);
         FyCheckFileFormat(texturePath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);

         sprintf(value, "%d", numTex);
         uiTy.ID(tbGeometryFXTextureNumberID);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbGeometryFXAlphamapNameID);
         FyCheckFileFormat(alphaPath, NULL, value, NULL);
         uiTy.PasteTypeinValue(value);
      }


/*--------------------
  update select keys 
  C.Wang 1217, 2012
 --------------------*/
void TbUpdatSelectKeysUI(FnBaseGameFX *baseFX, int iFrame)
{
   FnUiCombo uiC;
   FnUiTypein uiTy;
   float data[4];
   char string[256];
   int i, iKey, numKeys;

   // update UIs
   if (tbKeyframeType == KEYFRAME_VISIBILITY) {
      // show/update/initialize UIs (visibility, frame)
      uiC.ID(tbGameFXKeyFrameID);

      // paste all keys
      numKeys = baseFX->GetVisibilityKeyNumber();
      uiC.ClearMessages();
      for (i = 0; i < numKeys; i++) {
         baseFX->GetVisibilityKey(i, &iKey, data);
         sprintf(string, "%d", iKey);
         uiC.InsertMessage(i, string);

         if (iKey == iFrame) {
            uiC.PasteComboValue(string);

            // paste all data to typeins
            sprintf(string, "%f", data[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
         }
      }
   }
   else if (tbKeyframeType == KEYFRAME_POSITION) {
      // show/update/initialize UIs (position, frame)
      uiC.ID(tbGameFXKeyFrameID);

      // paste all keys
      numKeys = baseFX->GetPositionKeyNumber();
      uiC.ClearMessages();
      for (i = 0; i < numKeys; i++) {
         baseFX->GetPositionKey(i, &iKey, data);
         sprintf(string, "%d", iKey);
         uiC.InsertMessage(i, string);

         if (iKey == iFrame) {
            uiC.PasteComboValue(string);

            // paste all data to typeins
            sprintf(string, "%f", data[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", data[1]);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", data[2]);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.PasteTypeinValue(string);
         }
      }
   }
   else if (tbKeyframeType == KEYFRAME_ROTATION) {
      // show/update/initialize UIs (rotation, frame)
      uiC.ID(tbGameFXKeyFrameID);

      // paste all keys
      numKeys = baseFX->GetRotationKeyNumber();
      uiC.ClearMessages();
      for (i = 0; i < numKeys; i++) {
         baseFX->GetRotationKey(i, &iKey, data);
         sprintf(string, "%d", iKey);
         uiC.InsertMessage(i, string);

         if (iKey == iFrame) {
            uiC.PasteComboValue(string);

            // paste all data to typeins
            sprintf(string, "%f", data[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", data[1]);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", data[2]);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.PasteTypeinValue(string);
         }
      }
   }
   else if (tbKeyframeType == KEYFRAME_PLATE_SIZE) {
      if (baseFX->GetFXType() != PLATE_FX) return;
      FnPlateGameFX plateFX(baseFX->ID());

      // show/update/initialize UIs (plate size, frame)
      uiC.ID(tbGameFXKeyFrameID);

      // paste all keys
      numKeys = plateFX.GetPlateSizeKeyNumber();
      uiC.ClearMessages();
      for (i = 0; i < numKeys; i++) {
         plateFX.GetPlateSizeKey(i, &iKey, data);
         sprintf(string, "%d", iKey);
         uiC.InsertMessage(i, string);

         if (iKey == iFrame) {
            uiC.PasteComboValue(string);

            // paste all data to typeins
            sprintf(string, "%f", data[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", data[1]);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.PasteTypeinValue(string);
         }
      }
   }
   else if (tbKeyframeType == KEYFRAME_PLATE_COLOR) {
      if (baseFX->GetFXType() != PLATE_FX) return;
      FnPlateGameFX plateFX(baseFX->ID());

      // show/update/initialize UIs (plate color, frame)
      uiC.ID(tbGameFXKeyFrameID);

      // paste all keys
      numKeys = plateFX.GetPlateColorKeyNumber();
      uiC.ClearMessages();
      for (i = 0; i < numKeys; i++) {
         plateFX.GetPlateColorKey(i, &iKey, data);
         sprintf(string, "%d", iKey);
         uiC.InsertMessage(i, string);

         if (iKey == iFrame) {
            uiC.PasteComboValue(string);

            // paste all data to typeins
            sprintf(string, "%f", data[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", data[1]);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", data[2]);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", data[3]);
            uiTy.ID(tbGameFXKeyDataID[3]);
            uiTy.PasteTypeinValue(string);
         }
      }
   }
}


/*---------------------------------------
  paste the current material data to UIs
  C.Wang 0408, 2013
 ----------------------------------------*/
void TbPasteCurrentMaterialData(FnMaterial *mat)
{
   FnUiTypein uiTy;
   FnUiCombo uiC;
   FnUiButton uiB;
   char string[256];
   BOOL4 beOK = TRUE;

   if (mat == NULL) {
      beOK = FALSE;
   }
   else {
      if (mat->ID() == FAILED_ID) {
         beOK = FALSE;
      }
   }

   if (!beOK) {
      uiTy.ID(tbAmbientRID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbAmbientGID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbAmbientBID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbDiffuseRID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbDiffuseGID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbDiffuseBID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbSpecularRID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbSpecularGID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbSpecularBID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbEmissiveRID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbEmissiveGID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbEmissiveBID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbShininessID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiC.ID(tbSelectMaterialButtonID);
      uiC.PasteComboValue("-");
      uiC.Enable(FALSE);
      uiTy.ID(tbShaderFileID);
      uiTy.PasteTypeinValue("-");
      uiTy.Enable(FALSE);
#ifdef FLY2_DX9
      uiC.ID(tbEffectID);
      uiC.Enable(FALSE);
#endif
      uiTy.ID(tbTexture0ID);
      uiTy.PasteTypeinValue("-");
      uiTy.Enable(FALSE);
      uiTy.ID(tbTextureNumID);
      uiTy.PasteTypeinValue("0");
      uiTy.Enable(FALSE);
      uiTy.ID(tbTexture1ID);
      uiTy.PasteTypeinValue("-");
      uiTy.Enable(FALSE);
      uiTy.ID(tbTexture2ID);
      uiTy.PasteTypeinValue("-");
      uiTy.Enable(FALSE);
      uiTy.ID(tbTexture3ID);
      uiTy.PasteTypeinValue("-");
      uiTy.Enable(FALSE);
      uiTy.ID(tbTexture4ID);
      uiTy.PasteTypeinValue("-");
      uiTy.Enable(FALSE);
      uiTy.ID(tbTexture5ID);
      uiTy.PasteTypeinValue("-");
      uiTy.Enable(FALSE);
      uiB.ID(tbShaderLoaderID);
      uiB.Enable(FALSE);
      uiB.ID(tbShaderRemoveID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture0AddID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture0RemoveID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture1AddID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture1RemoveID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture2AddID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture2RemoveID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture3AddID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture3RemoveID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture4AddID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture4RemoveID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture5AddID);
      uiB.Enable(FALSE);
      uiB.ID(tbTexture5RemoveID);
      uiB.Enable(FALSE);

      for (int j = 0; j < MAX_MATERIAL_DATA; j++) {
         uiTy.ID(tbMatDataUI[j*3]);
         uiTy.PasteTypeinValue("");
         uiTy.Enable(FALSE);
         uiTy.ID(tbMatDataUI[j*3 + 1]);
         uiTy.PasteTypeinValue("");
         uiTy.Enable(FALSE);
         uiTy.ID(tbMatDataUI[j*3 + 2]);
         uiTy.PasteTypeinValue("");
         uiTy.Enable(FALSE);
      }

      return;
   }
   else {
      uiTy.ID(tbAmbientRID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbAmbientGID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbAmbientBID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbDiffuseRID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbDiffuseGID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbDiffuseBID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbSpecularRID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbSpecularGID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbSpecularBID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbEmissiveRID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbEmissiveGID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbEmissiveBID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbShininessID);
      uiTy.Enable(TRUE);
      uiC.ID(tbSelectMaterialButtonID);
      uiC.Enable(TRUE);
      uiTy.ID(tbShaderFileID);
      uiTy.Enable(TRUE);
#ifdef FLY2_DX9
      uiC.ID(tbEffectID);
      uiC.Enable(TRUE);
#endif
      uiTy.ID(tbTexture0ID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbTextureNumID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbTexture1ID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbTexture2ID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbTexture3ID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbTexture4ID);
      uiTy.Enable(TRUE);
      uiTy.ID(tbTexture5ID);
      uiTy.Enable(TRUE);
      uiB.ID(tbShaderLoaderID);
      uiB.Enable(TRUE);
      uiB.ID(tbShaderRemoveID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture0AddID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture0RemoveID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture1AddID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture1RemoveID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture2AddID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture2RemoveID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture3AddID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture3RemoveID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture4AddID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture4RemoveID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture5AddID);
      uiB.Enable(TRUE);
      uiB.ID(tbTexture5RemoveID);
      uiB.Enable(TRUE);
   }

   float *data = mat->GetAmbient();
   if (data != NULL) {
      tbMatBuffer.ambient[0] = data[0];
      tbMatBuffer.ambient[1] = data[1];
      tbMatBuffer.ambient[2] = data[2];
   }
   else {
      tbMatBuffer.ambient[0] = tbMatBuffer.ambient[1] = tbMatBuffer.ambient[2] = 0.3f;
   }
   uiTy.ID(tbAmbientRID);
   sprintf(string, "%5.4f", tbMatBuffer.ambient[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbAmbientGID);
   sprintf(string, "%5.4f", tbMatBuffer.ambient[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbAmbientBID);
   sprintf(string, "%5.4f", tbMatBuffer.ambient[2]);
   uiTy.PasteTypeinValue(string);

   data = mat->GetDiffuse();
   if (data != NULL) {
      tbMatBuffer.diffuse[0] = data[0];
      tbMatBuffer.diffuse[1] = data[1];
      tbMatBuffer.diffuse[2] = data[2];
   }
   else {
      tbMatBuffer.diffuse[0] = tbMatBuffer.diffuse[1] = tbMatBuffer.diffuse[2] = 0.7f;
   }
   uiTy.ID(tbDiffuseRID);
   sprintf(string, "%5.4f", tbMatBuffer.diffuse[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbDiffuseGID);
   sprintf(string, "%5.4f", tbMatBuffer.diffuse[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbDiffuseBID);
   sprintf(string, "%5.4f", tbMatBuffer.diffuse[2]);
   uiTy.PasteTypeinValue(string);

   data = mat->GetSpecular();
   if (data != NULL) {
      tbMatBuffer.specular[0] = data[0];
      tbMatBuffer.specular[1] = data[1];
      tbMatBuffer.specular[2] = data[2];
   }
   else {
      tbMatBuffer.specular[0] = tbMatBuffer.specular[1] = tbMatBuffer.specular[2] = 0.2f;
   }
   uiTy.ID(tbSpecularRID);
   sprintf(string, "%5.4f", tbMatBuffer.specular[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbSpecularGID);
   sprintf(string, "%5.4f", tbMatBuffer.specular[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbSpecularBID);
   sprintf(string, "%5.4f", tbMatBuffer.specular[2]);
   uiTy.PasteTypeinValue(string);

   data = mat->GetEmissive();
   if (data != NULL) {
      tbMatBuffer.emissive[0] = data[0];
      tbMatBuffer.emissive[1] = data[1];
      tbMatBuffer.emissive[2] = data[2];
   }
   else {
      tbMatBuffer.emissive[0] = tbMatBuffer.emissive[1] = tbMatBuffer.emissive[2] = 0.0f;
   }
   uiTy.ID(tbEmissiveRID);
   sprintf(string, "%5.4f", tbMatBuffer.emissive[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbEmissiveGID);
   sprintf(string, "%5.4f", tbMatBuffer.emissive[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbEmissiveBID);
   sprintf(string, "%5.4f", tbMatBuffer.emissive[2]);
   uiTy.PasteTypeinValue(string);

   float d = mat->GetShininess();
   tbMatBuffer.power = d;
   uiTy.ID(tbShininessID);
   sprintf(string, "%5.4f", d);
   uiTy.PasteTypeinValue(string);

   SHADERid shaderID = mat->GetShaderEffect();
   FnShader shader(shaderID);

   char *sf = shader.GetSourceName();
   char *ef = shader.GetShaderEffectName();
   uiTy.ID(tbShaderFileID);
#ifdef FLY2_DX9
   uiC.ID(tbEffectID);
#endif
   if (sf != NULL && strlen(sf) > 0) {
      uiTy.PasteTypeinValue(sf);
#ifdef FLY2_DX9
      uiC.PasteComboValue(ef);

      char name[256];
      uiC.ClearMessages();
      int i, numShader = FuCheckShaderNumberInFile(sf);
      if (numShader > 0) {
         for (i = 0; i < numShader; i++) {
            FuGetShaderNameInFile(sf, i, name);
            uiC.InsertMessage(i, name);
         }
      }
#endif
   }
   else {
      uiTy.PasteTypeinValue("-");
#ifdef FLY2_DX9
      uiC.PasteComboValue("-");
#endif
   }

   if (sf != NULL) {
      strcpy(tbMatBuffer.shaderFile, sf);
   }
   else {
      strcpy(tbMatBuffer.shaderFile, "-");
   }

   if (ef != NULL) {
      strcpy(tbMatBuffer.effectName, ef);
   }
   else {
      strcpy(tbMatBuffer.effectName, "-");
   }

   char *value = shader.GetPath();
   if (value != NULL) {
      strcpy(tbMatBuffer.shaderPath, value);
   }
   else {
      strcpy(tbMatBuffer.shaderPath, ".");
   }

   // update shader data
   int numPass, i, j, k, id, numData, count, len;
   char *cName, tName[256];
   float vv;

#ifdef FLY2_DX9
   numPass = shader.GetPassNumber();
#else
   numPass = 1;
#endif

   count = 0;
   for (j = 0; j < numPass && count < MAX_MATERIAL_DATA; j++) {
      numData = shader.GetDataNumber(j);
      for (i = 0; i < numData; i++) {
         cName = shader.GetDataVariableName(j, i, &id, &len);
         if (cName != NULL) {
            strcpy(tName, cName);
         }
         else {
            sprintf(tName, "Data %d", i);
         }
         uiTy.ID(tbMatDataUI[count*3]);
         uiTy.ReCaption(tName);

         // find the data value and paste to the typein
         tbMatDataID[count] = id;
         tbMatDataLen[count] = len;
         
         for (k = 0; k < len; k++) {
            uiTy.ID(tbMatDataUI[count*3 + k]);
            vv = mat->GetData(id + k);
            sprintf(tName, "%f", vv);
            uiTy.PasteTypeinValue(tName);
            uiTy.Enable(TRUE);
         }
         for (k = len; k < 3; k++) {
            uiTy.ID(tbMatDataUI[count*3 + k]);
            uiTy.Enable(FALSE);
         }
         count++;
      }
   }

   tbNumShowMatDataUI = count;
   for (j = tbNumShowMatDataUI; j < MAX_MATERIAL_DATA; j++) {
      uiTy.ID(tbMatDataUI[j*3]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbMatDataUI[j*3 + 1]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbMatDataUI[j*3 + 2]);
      uiTy.Enable(FALSE);
   }

   // paste texture number
   tbMatBuffer.numMatTexture = mat->GetTextureSlotNumber();
   uiTy.ID(tbTextureNumID);
   if (tbMatBuffer.numMatTexture == 1) {
      tbMatBuffer.numMatTexture = 0;
   }
   sprintf(string, "%d", tbMatBuffer.numMatTexture);
   uiTy.PasteTypeinValue(string);

   // paste texture 0
   TbPasteCurrentTexture(mat, 0);

   // paste texture 1
   TbPasteCurrentTexture(mat, 1);

   // paste texture 2
   TbPasteCurrentTexture(mat, 2);

   // paste texture 3
   TbPasteCurrentTexture(mat, 3);

   // paste texture 4
   TbPasteCurrentTexture(mat, 4);

   // paste texture 5
   TbPasteCurrentTexture(mat, 5);
}


/*-------------------------------------------------------------
  paste current layer n material texture with material buffer
  Eric Fei 0124, 2013
 --------------------------------------------------------------*/
void TbPasteCurrentTexture(FnMaterial *mat, int layer)
{
   // [in/out] mat:   Material Function
   // [in]     layer: Layer number
   
   FnUiTypein uiTy;
   char *textureName;
   if (layer == 0) {
      uiTy.ID(tbTexture0ID);
      textureName = tbMatBuffer.texture0;
   }
   else if (layer == 1) {
      uiTy.ID(tbTexture1ID);
      textureName = tbMatBuffer.texture1;
   }
   else if (layer == 2) {
      uiTy.ID(tbTexture2ID);
      textureName = tbMatBuffer.texture2;
   }
   else if (layer == 3) {
      uiTy.ID(tbTexture3ID);
      textureName = tbMatBuffer.texture3;
   }
   else if (layer == 4) {
      uiTy.ID(tbTexture4ID);
      textureName = tbMatBuffer.texture4;
   }
   else if (layer == 5) {
      uiTy.ID(tbTexture5ID);
      textureName = tbMatBuffer.texture5;
   }
   
   FnTexture tex(mat->GetTexture(0, layer));
   char *value = tex.GetPathName();
   char path[256], name[256];
   if (value != NULL) {
      // get file name & texture number
      FyCheckFileFormat(value, path, name, NULL);
      TbAdjustTextureFileNameFromMat(name, tbMatBuffer.numMatTexture);
      strcpy(tbMatBuffer.texturePath, path);
      strcpy(textureName, name);
      // paste file name
      uiTy.PasteTypeinValue(textureName);
   }
   else {
      //strcpy(tbMatBuffer.texturePath, ".");
      strcpy(textureName, "-");
      uiTy.PasteTypeinValue(textureName);
   }
}


/*----------------------
  update current FX UIs
  C.Wang 1231, 2012
 -----------------------*/
void TbSetFXParentTypeinUI(UIid uiID, char *value)
{
   int i, len;
   char string[256];
   FnUiCombo uiC(uiID);
   char *name, nameCheck[256];

   int count;
   OBJECTid oID;
   FnObject model;

   int numO = tbCurScene->GetObjectNumber();

   strcpy(string, value);
   FyTruncateStringSpace(string);
   len = (int)strlen(string);

   // popup the message box
   uiC.ID(tbSetFXParentID);
   uiC.PopupMessageBox(TRUE);

   if (len == 0) {
      uiC.ClearMessages();

      // insert all models
      count = 0;
      for (i = 0; i < numO; i++) {
         oID = tbCurScene->GetObjectID(i);
         if (oID != FAILED_ID) {
            model.ID(oID);
            name = model.GetName();
            uiC.InsertMessage(count, name);
            count++;
         }
      }
   }
   else {
      uiC.ClearMessages();

      // insert all models
      count = 0;
      for (i = 0; i < numO; i++) {
         oID = tbCurScene->GetObjectID(i);
         if (oID != FAILED_ID) {
            model.ID(oID);
            name = model.GetName();
            strcpy(nameCheck, name);
            if (FyCheckWord(nameCheck, string, TRUE, len)) {
               uiC.InsertMessage(count, name);
               count++;
            }
         }
      }
   }
}


/*----------------------
  update current FX UIs
  C.Wang 0131, 2013
 -----------------------*/
void TbUpdateCurrentFXUIs()
{
   int i, numFXS;
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   FnUiCombo uiC;
   FnUiTab uiT;
   FnUiTypein uiTy;
   FnUiPlate uiP;
   TBGameFXSystem *fxs;
   BOOL4 beCurrentFX;
   char caption[256];
   GAMEFXid gxID;
   FnBaseGameFX gx;

   // check current FX system
   TBGameFXSystem *curFX = tbCurScene->GetCurrentTBGameFXSystem();
   beCurrentFX = FALSE;
   if (curFX == NULL) {
      uiB.ID(tbLoadFXSystemButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbSaveFXSystemButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbNewFXSystemButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbDeleteFXSystemButtonID);
      uiB.Enable(FALSE);

      // check if there are no game FX system
      numFXS = tbCurScene->CheckEffectGameFXSystemNumber();
      uiC.ID(tbSelectFXSystemComboID);
      if (numFXS > 0) {
         uiC.Enable(TRUE);
         uiC.ClearMessages();
         for (i = 0; i < numFXS; i++) {
            fxs = tbCurScene->GetTBGameFXSystem(i);
            uiC.InsertMessage(i, fxs->GetName());
         }
         uiC.PasteComboValue("");
      }
      else {
         uiC.ClearMessages();
         uiC.PasteComboValue("");
         uiC.Enable(FALSE);
      }

      uiB.ID(tbNewFXButtonID);
      uiB.Enable(FALSE);

      uiC.ID(tbSelectFXComboID);
      uiC.Enable(FALSE);
      uiC.ClearMessages();
      uiC.PasteComboValue("");
   }
   else {
      uiB.ID(tbLoadFXSystemButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbSaveFXSystemButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbNewFXSystemButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbDeleteFXSystemButtonID);
      uiB.Enable(TRUE);
      uiC.ID(tbSelectFXSystemComboID);
      uiC.Enable(TRUE);

      // paste all game FX systems to the selection combo
      uiC.ClearMessages();
      numFXS = tbCurScene->CheckEffectGameFXSystemNumber();
      for (i = 0; i < numFXS; i++) {
         fxs = tbCurScene->GetTBGameFXSystem(i);
         uiC.InsertMessage(i, fxs->GetName());
      }
      uiC.PasteComboValue(curFX->GetName());

      // check current FX
      uiB.ID(tbNewFXButtonID);
      uiB.Enable(TRUE);

      uiC.ID(tbSelectFXComboID);
      uiC.ClearMessages();

      int numFX = curFX->GameFXNumber();
      if (numFX > 0) {
         uiC.Enable(TRUE);
         for (i = 0; i < numFX; i++) {
            gxID = curFX->GetGameFX(i);
            gx.ID(gxID);
            uiC.InsertMessage(i, gx.GetName());
         }

         // paste the name of current editing game FX
         gxID = curFX->GetCurrentGameFX();
         if (gxID != FAILED_ID) {
            gx.ID(gxID);
            uiC.PasteComboValue(gx.GetName());
            beCurrentFX = TRUE;
         }
         else {
            uiC.PasteComboValue("");
         }
      }
      else {
         uiC.Enable(FALSE);
         uiC.PasteComboValue("");
      }
   }

   // update current FX system's caption
   if (curFX == NULL) {
      uiT.ID(tbFXSystemCurrentTabID);
      uiT.SetCaption("FX System : None", 0.2f, 0.2f, 0.2f);
   }
   else {
      sprintf(caption, "FX System : %s", curFX->GetName());
      uiT.ID(tbFXSystemCurrentTabID);
      uiT.SetCaption(caption, 0.0f, 0.0f, 0.0f);
   }

   uiTy.ID(tbBaseFXFXPosID[0]);
   uiTy.Enable(beCurrentFX);
   uiTy.ID(tbBaseFXFXPosID[1]);
   uiTy.Enable(beCurrentFX);
   uiTy.ID(tbBaseFXFXPosID[2]);
   uiTy.Enable(beCurrentFX);

   uiB.ID(tbBaseFXFXPosEditKeyID);
   uiB.Enable(beCurrentFX);

   uiTy.ID(tbBaseFXFXRotateID[0]);
   uiTy.Enable(beCurrentFX);
   uiTy.ID(tbBaseFXFXRotateID[1]);
   uiTy.Enable(beCurrentFX);
   uiTy.ID(tbBaseFXFXRotateID[2]);
   uiTy.Enable(beCurrentFX);

   uiB.ID(tbBaseFXFXRotateEditKeyID);
   uiB.Enable(beCurrentFX);

   uiTy.ID(tbBaseFXShowID);
   uiTy.Enable(beCurrentFX);

   uiB.ID(tbBaseFXShowEditKeyID);
   uiB.Enable(beCurrentFX);

   uiTy.ID(tbBaseLifeID);
   uiTy.Enable(beCurrentFX);

   uiC.ID(tbSetFXParentID);
   uiC.Enable(beCurrentFX);

   uiRB.ID(tbBaseLifeAutoAdjustID);
   //uiRB.Enable(beCurrentFX);

   if (curFX != NULL) {
      GAMEFXid baseID;
      FnBaseGameFX base;
      OBJECTid oID;
      FnObject model;
      int i, numFX = curFX->GameFXNumber();

      gxID = curFX->GetCurrentGameFX();
      for (i = 0; i < numFX; i++) {
         baseID = curFX->GetGameFX(i);
         base.ID(baseID);
         if (base.GetFXType() != PLATE_FX) {
            oID = base.GetBaseObject();
            model.ID(oID);
            if (gxID == baseID) {
               model.Show(TRUE);
            }
            else {
               model.Show(FALSE);
            }
         }
      }
   }

   // update current FX UIs
   if (beCurrentFX) {
      // get current FX
      gxID = curFX->GetCurrentGameFX();
      gx.ID(gxID);

      uiB.ID(tbDeleteFXButtonID);
      uiB.Enable(TRUE);

      // paste position, orientation, and visibility data
      float data[4];
      char string[256];

      gx.GetPosition(data);
      uiTy.ID(tbBaseFXFXPosID[0]);
      sprintf(string, "%f", data[0]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXPosID[1]);
      sprintf(string, "%f", data[1]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXPosID[2]);
      sprintf(string, "%f", data[2]);
      uiTy.PasteTypeinValue(string);

      gx.GetRotation(data);
      uiTy.ID(tbBaseFXFXRotateID[0]);
      sprintf(string, "%f", data[0]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXRotateID[1]);
      sprintf(string, "%f", data[1]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXRotateID[2]);
      sprintf(string, "%f", data[2]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXRotateID[3]);
      sprintf(string, "%f", data[3]);
      uiTy.PasteTypeinValue(string);

      data[0] = gx.GetVisibility();
      uiTy.ID(tbBaseFXShowID);
      sprintf(string, "%3.1f", data[0]);
      uiTy.PasteTypeinValue(string);

      data[0] = gx.GetLife();
      uiTy.ID(tbBaseLifeID);
      sprintf(string, "%d", (int) data[0]);
      uiTy.PasteTypeinValue(string);

      // check parent's name
      uiC.ID(tbSetFXParentID);
      uiC.ClearMessages();

      OBJECTid pID = gx.GetParentObject();
      FnObject p(pID);
      char *name = p.GetName();
      if (name != NULL && strlen(name) > 0) {
         uiC.PasteComboValue(name);
         uiC.InsertMessage(0, name);
      }
      else {
         name = gx.GetParentObjectName();
         if (name != NULL && strlen(name) > 0) {
            uiC.PasteComboValue(name);
            uiC.InsertMessage(0, name);
         }
         else {
            uiC.PasteComboValue("");
         
            // insert all models
            int count;
            OBJECTid oID;
            FnObject model;
            int numO = tbCurScene->GetObjectNumber();
            count = 0;
            for (i = 0; i < numO; i++) {
               oID = tbCurScene->GetObjectID(i);
               if (oID != FAILED_ID) {
                  model.ID(oID);
                  name = model.GetName();
                  uiC.InsertMessage(count, name);
                  count++;
               }
            }
         }
      }

      DWORD fxType = gx.GetFXType();
      if (fxType == PLATE_FX) {
         uiT.ID(tbEditPlateFXTabID);
         sprintf(caption, "Edit Plate FX : %s", gx.GetName());
         uiT.SetCaption(caption, 0.0f, 0.0f, 0.0f);
         uiT.Show(TRUE);

         // check plate type
         FnPlateGameFX plate(gxID);
         DWORD plateType = plate.GetPlateType();
         uiC.ID(tbPlateFXTypeID);
         if (plateType == PLATE_ZUP) {
            uiC.PasteComboValue("Plate Z-up");
         }
         else if (plateType == PLATE_YUP) {
            uiC.PasteComboValue("Plate Y-up");
         }
         else {
            uiC.PasteComboValue("Billboard");
         }
         uiC.Enable(TRUE);

         char texName[256], value[256], name[256];
         int numTex;
         plate.GetTextureData(texName, &numTex);
         uiTy.ID(tbPlateFXTextureNameID);
         uiTy.Enable(TRUE);
         FyCheckFileFormat(texName, NULL, name, NULL);
         uiTy.PasteTypeinValue(name);

         sprintf(value, "%d", numTex);
         uiTy.ID(tbPlateFXTextureNumberID);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         plate.GetAlphamapData(texName);
         uiTy.ID(tbPlateFXAlphamapNameID);
         uiTy.Enable(TRUE);
         FyCheckFileFormat(texName, NULL, name, NULL);
         uiTy.PasteTypeinValue(name);

         uiB.ID(tbPlateFXTextureAddID);
         uiB.Enable(TRUE);
         uiB.ID(tbPlateFXTextureRemoveID);
         uiB.Enable(TRUE);

         uiB.ID(tbPlateFXAlphamapAddID);
         uiB.Enable(TRUE);

         DWORD bType;
         bType = plate.GetBlendingType();
         if (bType == ALPHA_BLENDING) {
            strcpy(value, "Alpha");
         }
         else if (bType == SUBTRACT_COLOR) {
            strcpy(value, "Subtract Color");
         }
         else {
            strcpy(value, "Add Color");
         }
         uiC.ID(tbPlateFXBlendTypeID);
         uiC.PasteComboValue(value);
         uiC.Enable(TRUE);

         float size[2], color[4];

         plate.GetPlateSize(size);
         uiTy.ID(tbPlateFXSizeID[0]);
         sprintf(value, "%f", size[0]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);
         uiTy.ID(tbPlateFXSizeID[1]);
         sprintf(value, "%f", size[1]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);
         uiB.ID(tbPlateFXSizeEditKeyID);
         uiB.Enable(TRUE);

         plate.GetPlateColor(color);
         uiTy.ID(tbPlateFXColorID[0]);
         sprintf(value, "%5.4f", color[0]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);
         uiTy.ID(tbPlateFXColorID[1]);
         sprintf(value, "%5.4f", color[1]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);
         uiTy.ID(tbPlateFXColorID[2]);
         sprintf(value, "%5.4f", color[2]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);
         uiTy.ID(tbPlateFXColorID[3]);
         sprintf(value, "%5.4f", color[3]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);
         uiB.ID(tbPlateFXColorEditKeyID);
         uiB.Enable(TRUE);

         // make plate edit is the next tab Ui of current FX system editing panel
         uiT.ID(tbFXSystemCurrentTabID);
         uiT.NextTab(tbEditPlateFXTabID);

         uiT.ID(tbEditParticleFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditForceFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditAudioFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditSwordFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditGeometryFXTabID);
         uiT.Show(FALSE);
      }
      else if (fxType == GEOMETRY_FX) {
         uiT.ID(tbEditGeometryFXTabID);
         sprintf(caption, "Edit Geometry FX : %s", gx.GetName());
         uiT.SetCaption(caption, 0.0f, 0.0f, 0.0f);
         uiT.Show(TRUE);

         int numT;
         FnGeometryGameFX gFX(gxID);
         OBJECTid oID = gFX.GetModelObject();
         char fullPath[256], name[128], value[256], texName[256];

         strcpy(fullPath, gFX.GetModelFullName());
         FyCheckFileFormat(fullPath, NULL, name, NULL);

         // make geometry edit is the next tab Ui of current FX system editing panel
         uiT.ID(tbFXSystemCurrentTabID);
         uiT.NextTab(tbEditGeometryFXTabID);

         uiB.ID(tbLoadGeometryFXID);
         uiB.Enable(TRUE);

         uiTy.ID(tbGeometryNameFXID);
         uiTy.PasteTypeinValue(name);
         uiTy.Enable(FALSE);

         DWORD bType;
         uiC.ID(tbGeometryFXBlendTypeID);
         bType = gFX.GetBlendingType();
         if (bType == ALPHA_BLENDING) {
            strcpy(value, "Alpha");
         }
         else if (bType == SUBTRACT_COLOR) {
            strcpy(value, "Subtract Color");
         }
         else {
            strcpy(value, "Add Color");
         }
         uiC.PasteComboValue(value);
         uiC.Enable(TRUE);

         uiTy.ID(tbGeometryFXTextureNameID);
         gFX.GetTextureData(texName, &numT);
         uiTy.PasteTypeinValue(texName);
         uiTy.Enable(TRUE);

         uiTy.ID(tbGeometryFXTextureNumberID);
         sprintf(value, "%d", numT);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbGeometryFXAlphamapNameID);
         gFX.GetAlphamapData(texName);
         uiTy.PasteTypeinValue(texName);
         uiTy.Enable(TRUE);

         uiB.ID(tbGeometryFXTextureAddID);
         uiB.Enable(TRUE);

         uiB.ID(tbGeometryFXAlphamapAddID);
         uiB.Enable(TRUE);


         uiB.ID(tbGeometryFXTextureRemoveID);
         uiB.Enable(TRUE);

         uiT.ID(tbEditPlateFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditParticleFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditForceFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditAudioFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditSwordFXTabID);
         uiT.Show(FALSE);
      }
      else if (fxType == PARTICLES_FX) {
         // show particle FX UI tab
         uiT.ID(tbEditParticleFXTabID);
         sprintf(caption, "Edit Particles FX : %s", gx.GetName());
         uiT.SetCaption(caption, 0.0f, 0.0f, 0.0f);
         uiT.Show(TRUE);

         float life[2], size[2], vel[3], velVar[3];
         int numP;
         DWORD bType;
         char texName[256], alphaName[256], value[256], name[256];
         int texAnimNum, texAnim[2];

         FnParticlesGameFX par(gxID);
         par.GetParticleLife(life);
         par.GetParticleSize(size);
         numP = par.GetParticleBornPerFrame();
         bType = par.GetBlendingType();
         par.GetTextureData(texName);
         par.GetAlphamapData(alphaName);
         par.GetParticleVelocity(vel, velVar);
         par.GetTextureAnimation(&texAnimNum, texAnim);

         uiTy.ID(tbParticleLifeID[0]);
         sprintf(value, "%7.1f", life[0]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleLifeID[1]);
         sprintf(value, "%7.1f", life[1]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleBornID[0]);
         sprintf(value, "%d", numP);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiP.ID(tbParticleBornID[1]);
         uiP.Enable(TRUE);

         uiTy.ID(tbParticleSizeID[0]);
         sprintf(value, "%7.1f", size[0]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleSizeID[1]);
         sprintf(value, "%7.1f", size[1]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         if (bType == ALPHA_BLENDING) {
            strcpy(value, "Alpha");
         }
         else if (bType == SUBTRACT_COLOR) {
            strcpy(value, "Subtract Color");
         }
         else {
            strcpy(value, "Add Color");
         }
         uiC.ID(tbParticleBlendTypeID);
         uiC.PasteComboValue(value);
         uiC.Enable(TRUE);

         uiTy.ID(tbParticleVelocityID[0]);
         sprintf(value, "%8.2f", vel[0]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleVelocityID[1]);
         sprintf(value, "%8.2f", vel[1]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleVelocityID[2]);
         sprintf(value, "%8.2f", vel[2]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleVelocityVarID[0]);
         sprintf(value, "%8.2f", velVar[0]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleVelocityVarID[1]);
         sprintf(value, "%8.2f", velVar[1]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleVelocityVarID[2]);
         sprintf(value, "%8.2f", velVar[2]);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         uiTy.ID(tbParticleTextureNameID);
         uiTy.Enable(TRUE);
         FyCheckFileFormat(texName, NULL, name, NULL);
         uiTy.PasteTypeinValue(name);

         uiTy.ID(tbParticleAlphamapNameID);
         uiTy.Enable(TRUE);
         FyCheckFileFormat(alphaName, NULL, name, NULL);
         uiTy.PasteTypeinValue(name);

         uiB.ID(tbParticleTextureAddID);
         uiB.Enable(TRUE);
         uiB.ID(tbParticleAlphamapAddID);
         uiB.Enable(TRUE);
         uiB.ID(tbParticleTextureRemoveID);
         uiB.Enable(TRUE);

         uiTy.ID(tbParticleTextureAnimID[0]);
         uiTy.Enable(TRUE);
         sprintf(value, "%d", texAnimNum);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleTextureAnimID[1]);
         uiTy.Enable(TRUE);
         sprintf(value, "%d", texAnim[0]);
         uiTy.PasteTypeinValue(value);

         uiTy.ID(tbParticleTextureAnimID[2]);
         uiTy.Enable(TRUE);
         sprintf(value, "%d", texAnim[1]);
         uiTy.PasteTypeinValue(value);

         // make particle edit is the next tab Ui of current FX system editing panel
         uiT.ID(tbFXSystemCurrentTabID);
         uiT.NextTab(tbEditParticleFXTabID);

         uiT.ID(tbEditPlateFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditForceFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditAudioFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditSwordFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditGeometryFXTabID);
         uiT.Show(FALSE);
      }
      else if (fxType == AUDIO_FX) {
         // show audio FX UI tab
         uiT.ID(tbEditAudioFXTabID);
         sprintf(caption, "Edit Audio FX : %s", gx.GetName());
         uiT.SetCaption(caption, 0.0f, 0.0f, 0.0f);
         uiT.Show(TRUE);
         
         // get Audio FX Information to UI & Turn On UI
         FnAudioGameFX audio(gxID);
         char aAudioName[256];
         float fAudioStartFrame;
         char aAudioStartFrame[32];

         uiTy.ID(tbAudioFXNameID);
         audio.GetAudioName(aAudioName);
         FyCheckFileFormat(aAudioName, NULL, aAudioName, NULL);
         uiTy.PasteTypeinValue(aAudioName);
         uiTy.Enable(TRUE);

         uiB.ID(tbAudioFXLoadButtonID);
         uiB.Enable(TRUE);

         uiTy.ID(tbAudioFXStartID);
         audio.GetAudioStartFrame(&fAudioStartFrame);
         sprintf(aAudioStartFrame, "%5.2f", fAudioStartFrame);
         uiTy.PasteTypeinValue(aAudioStartFrame);
         uiTy.Enable(TRUE);
         
         // make autio edit is the next tab Ui of current FX system editing panel
         uiT.ID(tbFXSystemCurrentTabID);
         uiT.NextTab(tbEditAudioFXTabID);
         
         uiT.ID(tbEditPlateFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditParticleFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditForceFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditSwordFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditGeometryFXTabID);
         uiT.Show(FALSE);
      }
      else if (fxType == FORCE_FX) {
         // show force FX UI tab
         uiT.ID(tbEditForceFXTabID);
         sprintf(caption, "Edit Force FX : %s", gx.GetName());
         uiT.SetCaption(caption, 0.0f, 0.0f, 0.0f);
         uiT.Show(TRUE);

         // update UIs
         FnForceGameFX fFX(gxID);
         DWORD fType = fFX.GetForceType();
         if (fType == GRAVITY) {
            uiC.ID(tbForceFXTypeID);
            uiC.PasteComboValue("Gravity");
         }
         else if (fType == PARALLEL) {
            uiC.ID(tbForceFXTypeID);
            uiC.PasteComboValue("Parallel");
         }
         else if (fType == SINK) {
            uiC.ID(tbForceFXTypeID);
            uiC.PasteComboValue("Sink");
         }
         else if (fType == SPHERE) {
            uiC.ID(tbForceFXTypeID);
            uiC.PasteComboValue("Sphere");
         }
         else if (fType == VORTEX) {
            uiC.ID(tbForceFXTypeID);
            uiC.PasteComboValue("Vortex");
         }
         else if (fType == VORTEX_2D) {
            uiC.ID(tbForceFXTypeID);
            uiC.PasteComboValue("Vortex 2D");
         }
         else if (fType == VISCOSITY) {
            uiC.ID(tbForceFXTypeID);
            uiC.PasteComboValue("Viscosity");
         }
         else if (fType == USER_DEFINED) {
            uiC.ID(tbForceFXTypeID);
            uiC.PasteComboValue("User Defined");
         }
         uiC.Enable(TRUE);

         char value[256];

         // update magnitude
         float fM = fFX.GetForceMagnitude();
         uiTy.ID(tbForceMagnitudeID);
         sprintf(value, "%8.2f", fM);
         uiTy.PasteTypeinValue(value);
         uiTy.Enable(TRUE);

         // update minimum distance
         float fD = fFX.GetForceMinDistance();
         uiTy.ID(tbForceMinDistID);
         sprintf(value, "%8.2f", fD);
         uiTy.PasteTypeinValue(value);
         if(fType == VORTEX || fType == VORTEX_2D)
            uiTy.Show(TRUE);
         else
            uiTy.Show(FALSE);

         // check all particle systems
         uiC.ID(tbForceUsedByID);
         uiC.ClearMessages();

         int i, numFX = curFX->GameFXNumber();
         GAMEFXid baseID;
         FnBaseGameFX base;

         int iLine = 0;
         uiC.InsertMessage(iLine, "-");
         iLine++;

         for (i = 0; i < numFX; i++) {
            baseID = curFX->GetGameFX(i);
            base.ID(baseID);
            if (base.GetFXType() == PARTICLES_FX) {
               uiC.InsertMessage(iLine, base.GetName());
               iLine++;
            }
         }

         GAMEFXid pID = fFX.UsedBy();
         if (pID == FAILED_ID) {
            uiC.PasteComboValue("-");
         }
         else {
            base.ID(pID);
            uiC.PasteComboValue(base.GetName());
         }
         uiC.Enable(TRUE);

         // make force edit is the next tab Ui of current FX system editing panel
         uiT.ID(tbFXSystemCurrentTabID);
         uiT.NextTab(tbEditForceFXTabID);

         uiT.ID(tbEditPlateFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditParticleFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditAudioFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditSwordFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditGeometryFXTabID);
         uiT.Show(FALSE);
      }
      else if (fxType == SWORD_FX) {
         // show sword FX UI tab
         uiT.ID(tbEditSwordFXTabID);
         sprintf(caption, "Edit Sword FX : %s", gx.GetName());
         uiT.SetCaption(caption, 0.0f, 0.0f, 0.0f);
         uiT.Show(TRUE);
         
         // get Sword FX Information to UI & Turn On UI
         TbUpdateSwordFXUIs();
         
         // make sword edit is the next tab Ui of current FX system editing panel
         uiT.ID(tbFXSystemCurrentTabID);
         uiT.NextTab(tbEditSwordFXTabID);
         
         uiT.ID(tbEditPlateFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditParticleFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditForceFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditAudioFXTabID);
         uiT.Show(FALSE);
         uiT.ID(tbEditGeometryFXTabID);
         uiT.Show(FALSE);
      }
      else {
         // for the FX types that are not available now!
         uiT.ID(tbEditPlateFXTabID);
         uiT.SetCaption("Edit Plate FX : None", 0.2f, 0.2f, 0.2f);
         uiT.Show(FALSE);
         uiT.ID(tbEditParticleFXTabID);
         uiT.SetCaption("Edit Particle FX : None", 0.2f, 0.2f, 0.2f);
         uiT.Show(FALSE);
         uiT.ID(tbEditForceFXTabID);
         uiT.SetCaption("Edit Force FX : None", 0.2f, 0.2f, 0.2f);
         uiT.Show(FALSE);
         uiT.ID(tbEditAudioFXTabID);
         uiT.SetCaption("Edit Audio FX : None", 0.2f, 0.2f, 0.2f);
         uiT.Show(FALSE);
         uiT.ID(tbEditSwordFXTabID);
         uiT.SetCaption("Edit Sword FX : None", 0.2f, 0.2f, 0.2f);
         uiT.Show(FALSE);
         uiT.ID(tbEditGeometryFXTabID);
         uiT.SetCaption("Edit geometry FX : None", 0.2f, 0.2f, 0.2f);
         uiT.Show(FALSE);
      }

      TbEnableGameFXKeyUI(!tbBePlay);
   }
   else {
      uiB.ID(tbDeleteFXButtonID);
      uiB.Enable(FALSE);

      uiT.ID(tbEditPlateFXTabID);
      uiT.SetCaption("Edit Plate FX : None", 0.2f, 0.2f, 0.2f);
      uiT.Show(FALSE);
      uiT.ID(tbEditParticleFXTabID);
      uiT.SetCaption("Edit Particle FX : None", 0.2f, 0.2f, 0.2f);
      uiT.Show(FALSE);
      uiT.ID(tbEditForceFXTabID);
      uiT.SetCaption("Edit Force FX : None", 0.2f, 0.2f, 0.2f);
      uiT.Show(FALSE);
      uiT.ID(tbEditAudioFXTabID);
      uiT.SetCaption("Edit Audio FX : None", 0.2f, 0.2f, 0.2f);
      uiT.Show(FALSE);
      uiT.ID(tbEditSwordFXTabID);
      uiT.SetCaption("Edit Sword FX : None", 0.2f, 0.2f, 0.2f);
      uiT.Show(FALSE);
      uiT.ID(tbEditGeometryFXTabID);
      uiT.SetCaption("Edit Geometry FX : None", 0.2f, 0.2f, 0.2f);
      uiT.Show(FALSE);
   }
}


/*-------------------------
  update current body data
  C.Wang 0430, 2014
 --------------------------*/
void TbUpdateCurrentBodyDataUI(FnCharacter *actor, BOOL4 beUpdateUI)
{
   FnUiCombo uiC, uiC2;
   FnUiTypein uiTy;
   FnUiRadioButton uiRB;
   FnUiButton uiB;
   int i, iCount;

   // find and paste the current body
   uiC.ID(tbSelectBodyComboID);
   uiC.ClearMessages();
   if (actor->ID() != FAILED_ID) {
      char *name = actor->GetCurrentBodyName();
      if (name == NULL) {
         name = actor->GetRootBodyName();
         actor->SetCurrentBody(name);
      }
      uiC.PasteComboValue(name);
      uiC.Enable(TRUE);

      // update action data for current body
      uiC2.ID(tbSelectActionComboID);
      uiC2.ClearMessages();
      int numA = actor->GetBodyTotalActionNumber(name);
      if (numA > 0) {
         int numAction;
         ACTIONid actionID[MAX_ACTIONS], curActionID;
         FnAction action;
         numAction = actor->GetBodyAllActions(name, actionID, MAX_ACTIONS);
         iCount = 0;
         for (i = 0; i < numAction; i++) {
            action.ID(actionID[i]);
            if (action.GetName()) {
               uiC2.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         uiC2.Enable(TRUE);

         // check if there are current action ?
         curActionID = actor->GetCurrentAction(name, 0);
         if (curActionID == FAILED_ID) {
            action.ID(actionID[0]);
            uiC2.PasteComboValue(action.GetName());
            actor->SetCurrentAction(name, 0, actionID[0]);
            actor->Play(START, 0.0f, tbBeBase);
         }
         else {
            action.ID(curActionID);
            uiC2.PasteComboValue(action.GetName());
            actor->SetCurrentAction(name, 0, curActionID);
            actor->Play(START, 0.0f, tbBeBase);
         }
         uiB.ID(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.ID(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }
      else {
         uiC2.Enable(FALSE);
         uiC2.PasteComboValue("");
         uiB.ID(tbEditActionButtonID);
         uiB.Enable(FALSE);
         uiB.ID(tbDeleteActionButtonID);
         uiB.Enable(FALSE);
         uiB.ID(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }

      // loop to find all bodies
      BOOL4 beOK;
      int numB = actor->GetBodyNumber();
      int startFrame, endFrame;
      char poseName[256], string[256], mName[256];
      iCount = 0;
      for (i = 0; i < numB; i++) {
         if (actor->GetBodyName(i) != NULL) {
            uiC.InsertMessage(iCount, actor->GetBodyName(i));
            iCount++;
         }
      }

      // update pose UIs
      numB = actor->GetBodyPoseNumber(name);
      uiC.ID(tbSelectPoseComboID);
      uiC.ClearMessages();

      uiB.ID(tbDeletePoseButtonID);
      uiB.Enable(FALSE);

      if (numB > 0) {
         iCount = 0;
         for (i = 0; i < numB; i++) {
            beOK = actor->QueryBodyPose(name, i, poseName, &startFrame, &endFrame, mName);
            if (beOK) {
               uiC.InsertMessage(iCount, poseName);
               if (iCount == 0 && beUpdateUI) {
                  uiC.PasteComboValue(poseName);
                  strcpy(tbCurrentBodyPoseName, poseName);
                  uiC.Enable(TRUE);

                  uiTy.ID(tbPoseStartID);
                  sprintf(string, "%d", startFrame);
                  uiTy.PasteTypeinValue(string);
                  uiTy.Enable(TRUE);
                  tbTestPoseStart = startFrame;
                  tbTestPoseFrame = (float) startFrame;

                  uiTy.ID(tbPoseEndID);
                  sprintf(string, "%d", endFrame);
                  uiTy.PasteTypeinValue(string);
                  uiTy.Enable(TRUE);
                  tbTestPoseEnd = endFrame;

                  uiTy.ID(tbPoseMotionNameID);
                  uiTy.PasteTypeinValue(mName);
                  strcpy(tbTestMotionName, mName);
               }
               uiB.ID(tbDeletePoseButtonID);
               uiB.Enable(TRUE);
               iCount++;
            }
         }
         tbBePlayPose = FALSE;
         uiRB.ID(tbPlayPoseButtonID);
         uiRB.ChangeState(tbBePlayPose);
         uiRB.Enable(TRUE);
         uiB.ID(tbMakeBaseMoveButtonID);
         uiB.Enable(TRUE);
      }
      else {
         uiC.ID(tbSelectPoseComboID);
         uiC.ClearMessages();
         uiC.PasteComboValue("");
         tbCurrentBodyPoseName[0] = '\0';
         uiC.Enable(FALSE);
         uiTy.ID(tbPoseStartID);
         uiTy.PasteTypeinValue("");
         uiTy.Enable(FALSE);
         uiTy.ID(tbPoseEndID);
         uiTy.PasteTypeinValue("");
         uiTy.Enable(FALSE);
         uiTy.ID(tbPoseMotionNameID);
         uiTy.PasteTypeinValue("");
         uiRB.ID(tbPlayPoseButtonID);
         tbBePlayPose = FALSE;
         uiRB.ChangeState(tbBePlayPose);
         uiRB.Enable(FALSE);
         uiB.ID(tbMakeBaseMoveButtonID);
         uiB.Enable(FALSE);
         tbTestPoseStart = tbTestPoseEnd = FY_NONE;
         tbTestPoseFrame = (float) tbTestPoseStart;
         tbTestMotionName[0] = '\0';
      }
   }
   else {
      uiC.Enable(FALSE);
      uiC.PasteComboValue("");

      // update pose UIs
      uiC.ID(tbSelectPoseComboID);
      uiC.ClearMessages();
      uiC.PasteComboValue("");
      tbCurrentBodyPoseName[0] = '\0';
      uiC.Enable(FALSE);
      uiTy.ID(tbPoseStartID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbPoseEndID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbPoseMotionNameID);
      uiTy.PasteTypeinValue("");
      uiRB.ID(tbPlayPoseButtonID);
      tbBePlayPose = FALSE;
      uiRB.ChangeState(tbBePlayPose);
      uiRB.Enable(FALSE);
      uiB.ID(tbDeletePoseButtonID);
      uiB.Enable(FALSE);
      tbTestPoseStart = tbTestPoseEnd = FY_NONE;
      tbTestPoseFrame = (float) tbTestPoseStart;
      uiC2.ID(tbSelectActionComboID);
      uiC2.ClearMessages();
      uiC2.PasteComboValue("");
      uiC2.Enable(FALSE);
      uiB.ID(tbDeleteActionButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbEditActionButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbAddActionButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbMakeBaseMoveButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbDeletePoseButtonID);
      uiB.Enable(FALSE);
   }
}


/*-----------------------------------------
  extract the UIs' data to material buffer
  C.Wang 1103, 2012
 ------------------------------------------*/
void TbExtractMaterialUIData()
{
   FnUiTypein uiTy;
   FnUiCombo uiC;
   FnUiButton uiB;
   char value[256];

   uiTy.ID(tbAmbientRID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.ambient[0] = (float) atof(value);
   if (tbMatBuffer.ambient[0] < 0.0f) tbMatBuffer.ambient[0] = 0.0f;

   uiTy.ID(tbAmbientGID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.ambient[1] = (float) atof(value);
   if (tbMatBuffer.ambient[1] < 0.0f) tbMatBuffer.ambient[1] = 0.0f;

   uiTy.ID(tbAmbientBID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.ambient[2] = (float) atof(value);
   if (tbMatBuffer.ambient[2] < 0.0f) tbMatBuffer.ambient[2] = 0.0f;

   uiTy.ID(tbDiffuseRID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.diffuse[0] = (float) atof(value);
   if (tbMatBuffer.diffuse[0] < 0.0f) tbMatBuffer.diffuse[0] = 0.0f;

   uiTy.ID(tbDiffuseGID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.diffuse[1] = (float) atof(value);
   if (tbMatBuffer.diffuse[1] < 0.0f) tbMatBuffer.diffuse[1] = 0.0f;

   uiTy.ID(tbDiffuseBID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.diffuse[2] = (float) atof(value);
   if (tbMatBuffer.diffuse[2] < 0.0f) tbMatBuffer.diffuse[2] = 0.0f;

   uiTy.ID(tbSpecularRID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.specular[0] = (float) atof(value);
   if (tbMatBuffer.specular[0] < 0.0f) tbMatBuffer.specular[0] = 0.0f;

   uiTy.ID(tbSpecularGID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.specular[1] = (float) atof(value);
   if (tbMatBuffer.specular[1] < 0.0f) tbMatBuffer.specular[1] = 0.0f;

   uiTy.ID(tbSpecularBID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.specular[2] = (float) atof(value);
   if (tbMatBuffer.specular[2] < 0.0f) tbMatBuffer.specular[2] = 0.0f;

   uiTy.ID(tbEmissiveRID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.emissive[0] = (float) atof(value);
   if (tbMatBuffer.emissive[0] < 0.0f) tbMatBuffer.emissive[0] = 0.0f;

   uiTy.ID(tbEmissiveGID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.emissive[1] = (float) atof(value);
   if (tbMatBuffer.emissive[1] < 0.0f) tbMatBuffer.emissive[1] = 0.0f;

   uiTy.ID(tbEmissiveBID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.emissive[2] = (float) atof(value);
   if (tbMatBuffer.emissive[2] < 0.0f) tbMatBuffer.emissive[2] = 0.0f;

   uiTy.ID(tbShininessID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.power = (float) atof(value);
   if (tbMatBuffer.power < 0.0f) tbMatBuffer.power = 0.0f;

   uiTy.ID(tbShaderFileID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.shaderFile, value);

#ifdef FLY2_DX9
   uiC.ID(tbEffectID);
   uiC.GetComboValue(value);
   strcpy(tbMatBuffer.effectName, value);
#endif

   strcpy(tbMatBuffer.shaderPath, tbTmpShaderPath);
   strcpy(tbMatBuffer.texturePath, tbTmpTexturePath);

   uiTy.ID(tbTexture0ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture0, value);

   uiTy.ID(tbTextureNumID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.numMatTexture = atoi(value);
   uiTy.ID(tbTexture1ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture1, value);

   uiTy.ID(tbTexture2ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture2, value);

   uiTy.ID(tbTexture3ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture3, value);

   uiTy.ID(tbTexture4ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture4, value);

   uiTy.ID(tbTexture5ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture5, value);

   uiB.ID(tbUpdateMaterialID);
   uiB.Enable(FALSE);

   // update shader data
   int i;
   for (i = 0; i < tbNumShowMatDataUI; i++) {
      uiTy.ID(tbMatDataUI[i*3]);
      uiTy.GetTypeinValue(value);
      tbMatBuffer.data[i*3] = (float) atof(value);

      uiTy.ID(tbMatDataUI[i*3 + 1]);
      uiTy.GetTypeinValue(value);
      tbMatBuffer.data[i*3 + 1] = (float) atof(value);

      uiTy.ID(tbMatDataUI[i*3 + 2]);
      uiTy.GetTypeinValue(value);
      tbMatBuffer.data[i*3 + 2] = (float) atof(value);

      tbMatBuffer.dataID[i] = tbMatDataID[i];
      tbMatBuffer.dataLen[i] = tbMatDataLen[i];
   }
   tbMatBuffer.numMatData = tbNumShowMatDataUI;
}


/*---------------------------------------------
  update current material with material buffer
  C.Wang 1103, 2012
 ----------------------------------------------*/
void TbUpdateCurrentMaterial(FnMaterial *mat)
{
   // update ambient
   mat->SetAmbient(tbMatBuffer.ambient);

   // update diffuse
   mat->SetDiffuse(tbMatBuffer.diffuse);

   // update specular
   mat->SetSpecular(tbMatBuffer.specular);

   // update emissive
   mat->SetEmissive(tbMatBuffer.emissive);

   // update shininess
   mat->SetShininess(tbMatBuffer.power);

   // update shader
   if (FyCheckWord(tbMatBuffer.shaderFile, "-")) {
      mat->ReplaceShaderEffect(FAILED_ID);
   }
   else {
      // add the new shader
      char fileName[256];
      sprintf(fileName, "%s\\%s", tbMatBuffer.shaderPath, tbMatBuffer.shaderFile);

      mat->AddShaderEffect(fileName, tbMatBuffer.effectName);
   }

   // update textures
   int i, j;
   int numMatTex = mat->GetTextureSlotNumber();

   // reset textures
   if (tbMatBuffer.numMatTexture < numMatTex) {
      //mat->InitializeTextures(tbMatBuffer.numMatTexture, 6);
      mat->InitializeTextures(numMatTex, 6);
   }
   else {
      for (i = 0; i < 6; i++) {
         for (j = 0; j < numMatTex; j++) {
            mat->RemoveTexture(j, i);
         }
      }
   }

   // add texture 0
   if (!FyCheckWord(tbMatBuffer.texture0, "-")) {
      TbUpdateCurrentTexture(mat, 0);
   }

   // add texutre 1
   if (!FyCheckWord(tbMatBuffer.texture1, "-")) {
      TbUpdateCurrentTexture(mat, 1);
   }

   // add texture 2
   if (!FyCheckWord(tbMatBuffer.texture2, "-")) {
      TbUpdateCurrentTexture(mat, 2);
   }

   // add texture 3
   if (!FyCheckWord(tbMatBuffer.texture3, "-")) {
      TbUpdateCurrentTexture(mat, 3);
   }

   // add texture 4
   if (!FyCheckWord(tbMatBuffer.texture4, "-")) {
      TbUpdateCurrentTexture(mat, 4);
   }

   // add texture 5
   if (!FyCheckWord(tbMatBuffer.texture5, "-")) {
      TbUpdateCurrentTexture(mat, 5);
   }

   for (i = 0; i < tbNumShowMatDataUI; i++) {
      for (j = 0; j < tbMatBuffer.dataLen[i]; j++) {
         mat->SetData(tbMatBuffer.dataID[i] + j, tbMatBuffer.data[i*3 + j]);
      }
   }
}
/*-------------------------------------------------------------
  update current layer n material texture with material buffer
  Eric Fei 0124, 2013
 --------------------------------------------------------------*/
void TbUpdateCurrentTexture(FnMaterial *mat, int layer)
{
   int i;
   char fileName[256];

   if (layer == 0) {
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture0);
   }
   else if (layer == 1) {
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture1);
   }
   else if (layer == 2) {
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture2);
   }
   else if (layer == 3) {
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture3);
   }
   else if (layer == 4) {
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture4);
   }
   else if (layer == 5) {
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture5);
   }

   TEXTUREid texID;
   int len = (int) strlen(tbMatBuffer.texturePath) + 1;    // get path length + "\"
   char *pFileStart = fileName + len;                // get file name start address
   char *pFileTail  = NULL;                          // get file name end address
   if (TbAdjustTextureFileNameFromUI(pFileStart, &pFileTail, tbMatBuffer.numMatTexture)) {
      for (i = 0; i < tbMatBuffer.numMatTexture; i++) {
         pFileTail[0] = '\0';
         sprintf(pFileTail, "%04d", i);
         texID = mat->LoadTexture(i, layer, fileName);
      }
   }
   else {
      mat->LoadTexture(0, layer, fileName);
   }
}


/*-----------------------------------------
  cancel the current material data editing
  C.Wang 0813, 2012
 ------------------------------------------*/
void TbCancelMaterialEditing()
{
   FnUiTypein uiTy;
   FnUiCombo uiC;
   FnUiButton uiB;
   char string[256];

   float *data = tbMatBuffer.ambient;
   uiTy.ID(tbAmbientRID);
   sprintf(string, "%5.4f", data[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbAmbientGID);
   sprintf(string, "%5.4f", data[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbAmbientBID);
   sprintf(string, "%5.4f", data[2]);
   uiTy.PasteTypeinValue(string);

   data = tbMatBuffer.diffuse;
   uiTy.ID(tbDiffuseRID);
   sprintf(string, "%5.4f", data[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbDiffuseGID);
   sprintf(string, "%5.4f", data[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbDiffuseBID);
   sprintf(string, "%5.4f", data[2]);
   uiTy.PasteTypeinValue(string);

   data = tbMatBuffer.specular;
   uiTy.ID(tbSpecularRID);
   sprintf(string, "%5.4f", data[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbSpecularGID);
   sprintf(string, "%5.4f", data[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbSpecularBID);
   sprintf(string, "%5.4f", data[2]);
   uiTy.PasteTypeinValue(string);

   data = tbMatBuffer.emissive;
   uiTy.ID(tbEmissiveRID);
   sprintf(string, "%5.4f", data[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbEmissiveGID);
   sprintf(string, "%5.4f", data[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbEmissiveBID);
   sprintf(string, "%5.4f", data[2]);
   uiTy.PasteTypeinValue(string);

   uiTy.ID(tbShininessID);
   sprintf(string, "%5.4f", tbMatBuffer.power);
   uiTy.PasteTypeinValue(string);

   uiTy.ID(tbShaderFileID);
   uiTy.PasteTypeinValue(tbMatBuffer.shaderFile);

#ifdef FLY2_DX9
   uiC.ID(tbEffectID);
   uiC.PasteComboValue(tbMatBuffer.effectName);
#endif

   uiTy.ID(tbTexture0ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture0);
   uiTy.ID(tbTextureNumID);
   sprintf(string, "%d", tbMatBuffer.numMatTexture);
   uiTy.PasteTypeinValue(string);
   uiTy.ID(tbTexture1ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture1);
   uiTy.ID(tbTexture2ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture2);
   uiTy.ID(tbTexture3ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture3);
   uiTy.ID(tbTexture4ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture4);
   uiTy.ID(tbTexture5ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture5);
   
   uiB.ID(tbUpdateMaterialID);
   uiB.Enable(FALSE);
}


/*------------------------------------------
  clear shader data at material editing UIs
  C.Wang 0813, 2012
 -------------------------------------------*/
void TbClearShaderData(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      FnUiTypein uiTy;
      FnUiCombo uiC;
      FnUiButton uiB;

      uiTy.ID(tbShaderFileID);
      uiTy.PasteTypeinValue("-");

#ifdef FLY2_DX9
      uiC.ID(tbEffectID);
      uiC.PasteComboValue("-");
      uiC.ClearMessages();
#endif

      uiB.ID(tbUpdateMaterialID);
      uiB.Enable(TRUE);
   }
}


/*-------------------------------------------
  clear texture data at material editing UIs
  C.Wang 0813, 2012
 --------------------------------------------*/
void TbClearTextureData(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      FnUiTypein uiTy;
      FnUiButton uiB;

      if (uiID == tbTexture0RemoveID) {
         uiTy.ID(tbTexture0ID);
      }
      else if (uiID == tbTexture1RemoveID) {
         uiTy.ID(tbTexture1ID);
      }
      else if (uiID == tbTexture2RemoveID) {
         uiTy.ID(tbTexture2ID);
      }
      else if (uiID == tbTexture3RemoveID) {
         uiTy.ID(tbTexture3ID);
      }
      else if (uiID == tbTexture4RemoveID) {
         uiTy.ID(tbTexture4ID);
      }
      else if (uiID == tbTexture5RemoveID) {
         uiTy.ID(tbTexture5ID);
      }
      uiTy.PasteTypeinValue("-");

      uiB.ID(tbUpdateMaterialID);
      uiB.Enable(TRUE);
   }
}


/*------------------------------------------
  add shader data at material editing UIs
  C.Wang 0813, 2012
 -------------------------------------------*/
void TbAddShaderData(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // open the file browser to load the shader
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL4 beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "Fly CWS Shader Description File {*.cws}\0*.cws\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "cws";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         // paste the name to UIs
         FnUiTypein uiTy;
         FnUiButton uiB;
         uiTy.ID(tbShaderFileID);
         uiTy.PasteTypeinValue(file);

         strcpy(tbTmpShaderPath, path);

#ifdef FLY2_DX9
         FnUiCombo uiC;
         char name[256];

         uiC.ID(tbEffectID);
         int i, numShader = FuCheckShaderNumberInFile(fileName);
         uiC.ClearMessages();
         if (numShader > 0) {
            for (i = 0; i < numShader; i++) {
               FuGetShaderNameInFile(fileName, i, name);
               if (i == 0) {
                  uiC.PasteComboValue(name);
               }
               uiC.InsertMessage(i, name);
            }
         }
         else {
            uiC.PasteComboValue("-");
         }
#endif

         uiB.ID(tbUpdateMaterialID);
         uiB.Enable(TRUE);
      }

      // restore the current directory
      //if (tbBeRestoreCurrentDir) {
      SetCurrentDirectory(dir);
      //}
      
      // update working directory
      GetCurrentDirectory(256, tbWorkDir);
      FnUiTypein uiTy;
      uiTy.ID(tbWorkDirTypeinUI);
      uiTy.PasteTypeinValue(tbWorkDir);
   }
}


/*-----------------------------------------
  add texture data at material editing UIs
  C.Wang 0813, 2012
 ------------------------------------------*/
void TbAddTextureData(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // open the file browser to load the shader
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL4 beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "JPEG texture File {*.jpg}\0*.jpg\0"\
                        "DDS texture File {*.dds}\0*.dds\0"\
                        "PNG texture File {*.png}\0*.png\0"\
                        "MS Bitmap texture File {*.bmp}\0*.bmp\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "jpg";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         // paste the name to UIs
         FnUiTypein uiTy;
         FnUiButton uiB;

         if (uiID == tbTexture0AddID) {
            uiTy.ID(tbTexture0ID);
         }
         else if (uiID == tbTexture1AddID) {
            uiTy.ID(tbTexture1ID);
         }
         else if (uiID == tbTexture2AddID) {
            uiTy.ID(tbTexture2ID);
         }
         else if (uiID == tbTexture3AddID) {
            uiTy.ID(tbTexture3ID);
         }
         else if (uiID == tbTexture4AddID) {
            uiTy.ID(tbTexture4ID);
         }
         else if (uiID == tbTexture5AddID) {
            uiTy.ID(tbTexture5ID);
         }
         uiTy.PasteTypeinValue(file);

         strcpy(tbTmpTexturePath, path);

         uiB.ID(tbUpdateMaterialID);
         uiB.Enable(TRUE);
      }

      // restore the current directory
      //if (tbBeRestoreCurrentDir) {
      SetCurrentDirectory(dir);
      //}
      
      // update working directory
      GetCurrentDirectory(256, tbWorkDir);
      FnUiTypein uiTy;
      uiTy.ID(tbWorkDirTypeinUI);
      uiTy.PasteTypeinValue(tbWorkDir);
   }
}


/*-------------------------------------
  update current facial expression UIs
  C.Wang 0515, 2014
 --------------------------------------*/
void TbUpdateCurrentFacialExpressionUIs()
{
   TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
   if (actor == NULL) return;

   int i, len, posID;
   FnUiButton uiB;
   FnUiTypein uiTy;
   FnUiCombo uiC;
   char string[256], *name;

   uiC.ID(tbSelectFacialExpressionID);
   uiC.ClearMessages();

   int numFE = actor->FacialExpressionNumber();
   for (i = 0; i < numFE; i++) {
      name = actor->GetFacialExpressionName(i);
      uiC.InsertMessage(i, name);
   }

   if (numFE > 0) {
      uiB.ID(tbSaveFacialExpressionID);
      uiB.Enable(TRUE);
   }
   else {
      uiB.ID(tbSaveFacialExpressionID);
      uiB.Enable(FALSE);
   }

   int numBS = actor->GetBlendShapeNumber(0);   // we don't need the base shape
   char *feName = actor->GetCurrentFacialExpression();

   if (feName == NULL) {
      uiC.ID(tbSelectFacialExpressionID);
      uiC.PasteComboValue(" ");
      uiB.ID(tbAddFacialExpressionID);
      uiB.Enable(TRUE);
      uiB.ID(tbRemoveFacialExpressionID);
      uiB.Enable(FALSE);
      uiTy.ID(tbFaceFrameCountID);
      uiTy.PasteTypeinValue("0");
      uiTy.Enable(FALSE);
      uiB.ID(tbStopFacialExpressionID);
      uiB.Enable(FALSE);
      uiB.ID(tbPlayFacialExpressionID);
      uiB.Enable(FALSE);
      uiC.ID(tbSelectFacialExpressionKeyID);
      uiC.PasteComboValue(" ");
      uiC.Enable(FALSE);
      uiB.ID(tbAddFacialExpressionKeyID);
      uiB.Enable(FALSE);
      uiB.ID(tbRemoveFacialExpressionKeyID);
      uiB.Enable(FALSE);

      for (i = 0; i < numBS; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.PasteTypeinValue("0");
         uiTy.Enable(FALSE);
         uiTy.Show(TRUE);
      }
      for (i = numBS; i < MAX_BLENDSHAPES; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.Show(FALSE);
      }
      return;
   }
   
   uiC.ID(tbSelectFacialExpressionID);
   uiC.PasteComboValue(feName);

   uiB.ID(tbAddFacialExpressionID);
   uiB.Enable(TRUE);

   uiB.ID(tbRemoveFacialExpressionID);
   uiB.Enable(TRUE);

   uiTy.ID(tbFaceFrameCountID);
   len = actor->GetFacialExpressionLength(feName);
   sprintf(string, "%d", len);
   uiTy.PasteTypeinValue(string);
   uiTy.Enable(TRUE);

   uiB.ID(tbStopFacialExpressionID);
   uiB.Enable(TRUE);
   uiB.ID(tbPlayFacialExpressionID);
   uiB.Enable(TRUE);

   int numKey = actor->FacialExpressionKeyNumber(feName);
   uiC.ID(tbSelectFacialExpressionKeyID);
   uiC.ClearMessages();
   uiC.Enable(TRUE);
   for (i = 0; i < numKey; i++) {
      posID = (int) actor->GetFacialExpressionKey(feName, i, NULL, NULL, NULL);
      sprintf(string, "%d", posID);
      uiC.InsertMessage(i, string);
   }

   uiB.ID(tbAddFacialExpressionKeyID);
   uiB.Enable(TRUE);
   int curKey = actor->CurrentFacialExpressionKey();
   if (curKey < 0) {
      uiC.ID(tbSelectFacialExpressionKeyID);
      uiC.PasteComboValue(" ");
      for (i = 0; i < numBS; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.PasteTypeinValue("0");
         uiTy.Show(TRUE);
         uiTy.Enable(FALSE);
      }
      for (i = numBS; i < MAX_BLENDSHAPES; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.Show(FALSE);
      }
      uiB.ID(tbRemoveFacialExpressionKeyID);
      uiB.Enable(FALSE);
   }
   else {
      float weight[30];
      uiC.ID(tbSelectFacialExpressionKeyID);
      posID = (int) actor->GetFacialExpressionKey(feName, curKey, NULL, weight, NULL);
      sprintf(string, "%d", posID);
      uiC.PasteComboValue(string);
      for (i = 0; i < numBS; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         sprintf(string, "%f", weight[i]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);
         uiTy.Enable(TRUE);
      }
      for (i = numBS; i < MAX_BLENDSHAPES; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.Show(FALSE);
      }
      uiB.ID(tbRemoveFacialExpressionKeyID);
      uiB.Enable(TRUE);
   }
}


/*------------------
  quit the program
  C.Wang 0120, 2014
 -------------------*/
void TbQuitTool(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // remove face mask data
      if (tbCurrentFaceMask != NULL) {
         FyFree(tbCurrentFaceMask->blendFace);
         FyFree(tbCurrentFaceMask->cornerVert);
         FyFree(tbCurrentFaceMask->maskFace);
         FyFree(tbCurrentFaceMask->maskVert);
         FyFree(tbCurrentFaceMask);
         tbCurrentFaceMask = NULL;
      }

      tbScene.DeleteObject(tbGizmoMID);
      tbScene.DeleteObject(tbGizmoSID);

      // remove the characters/objects
      int i;
      for (i = 0; i < tbNumScene; i++) {
         if (tbSceneList[i] != NULL) {
            delete tbSceneList[i];
            tbSceneList[i] = NULL;
         }
      }

      FyFree(tbSceneList);
      tbNumScene = 0;
      tbMaxScene = 0;
      tbMainSceneID = FAILED_ID;

      // de-initialize the job system
      FjUseJobSystem(0);

      // empty the command history
      for (i = 0; i < tbNumCommandHistory; i++) {
         FyFree(tbCommandHistory[i]);
         tbCommandHistory[i] = NULL;
      }
      FyFree(tbCommandHistory);
      tbCommandHistory = NULL;

      // empty the command queue
      FcSetupCommandQueue(0);

      // de-initialize the script system
      TbDeinitScriptSystem();

      // delete all UIs
      FyEndUISystem(tbMenuBarID);
      FyEndUISystem(tbMenuID);
      FyEndUISystem(tbSideMenuID);
      FyEndUISystem(tbWarnDialogueID);
      FyEndUISystem(tbAddActionDialogueID);
      FyEndUISystem(tbAddGameFXDialogueID);
      FyEndUISystem(tbAddGameFXSystemDialogueID);
      FyEndUISystem(tbGameFXKeyframeDialogueID);
      FyEndUISystem(tbAddGameFXSystemDialogueID);
      FyEndUISystem(tbAddFacialExpressionDialogueID);

      // end the game world
      FyQuitFlyWin32();
   }
}


/*------------------
  new the system
  C.Wang 0813, 2012
 -------------------*/
void TbNewTool(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      tbWarnUI.PopUp(FALSE);

      // clear command list
      FcClearCommand();

      // clear/new current scene
      tbCurScene->New();

      // remove the dumper scene
      //FyDeleteScene(tbDumperSceneID);
      //tbDumperSceneID = FyCreateScene(1);
      //tbDumperScene.ID(tbDumperSceneID);

      // reset the UIs
      tbCurScene->SetCurrentObject(FAILED_ID);
      tbCurScene->SetCurrentCharacter(FAILED_ID);

      FnUiCombo uiC;
      FnUiTypein uiTy;
      FnUiRadioButton uiRB;
      FnUiButton uiB;
      uiC.ID(tbSelectCharacterComboID);
      uiC.ClearMessages();
      uiC.PasteComboValue("");
      uiC.Enable(FALSE);

      uiB.ID(tbBakeCharacterVAID);
      uiB.Enable(FALSE);

      // clear current body
      uiC.ID(tbSelectBodyComboID);
      uiC.ClearMessages();
      uiC.Enable(FALSE);
      uiC.PasteComboValue("");

      // clear current action
      uiC.ID(tbSelectActionComboID);
      uiC.ClearMessages();
      uiC.Enable(FALSE);
      uiC.PasteComboValue("");
      uiB.ID(tbEditActionButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbDeleteActionButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbAddActionButtonID);
      uiB.Enable(FALSE);

      // clear bounding box
      uiB.ID(tbCalculateBBButtonID);
      uiB.Enable(FALSE);

      // update pose UIs
      uiC.ID(tbSelectBodyComboID);
      uiC.ClearMessages();
      uiC.PasteComboValue("");
      uiC.Enable(FALSE);
      uiTy.ID(tbPoseStartID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbPoseEndID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.ID(tbPoseMotionNameID);
      uiTy.PasteTypeinValue("");
      uiRB.ID(tbPlayPoseButtonID);
      uiRB.ChangeState(FALSE);
      uiRB.Enable(FALSE);
      uiB.ID(tbMakeBaseMoveButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbDeletePoseButtonID);
      uiB.Enable(FALSE);
      tbBePlayPose = FALSE;

      TbStopPlay(tbStopID, TRUE);

      tbPlaySpeed = 1.0f;
      uiTy.ID(tbPlaySpeedID);
      uiTy.PasteTypeinValue("1.000");

      uiTy.ID(tbCurFrameID);
      char msg[256];
      tbCurFrame = 0.0f;
      sprintf(msg, "%8.1f", tbCurFrame);
      uiTy.PasteTypeinValue(msg);

      uiC.ID(tbSelectCameraID);
      uiC.ClearMessages();
      uiC.InsertMessage(0, "MainCam");
      uiC.PasteComboValue("MainCam");
      FnCamera camera;
      float aspect;
      camera.ID(tbMainCamID);
      aspect = camera.GetAspectRatio();
      tbWorkSpaceVPWidth = (int)(aspect*tbWorkSpaceVPHeight);
      tbWorkSpaceVP.SetSize(tbWorkSpaceVPWidth, tbWorkSpaceVPHeight);
      tbRenderCamID = tbMainCamID;
      tbBeEditCam = TRUE;

      // initialize face tool
      tbFaceModelName[0] = '\0';
      tbCurrentFaceModelID = FAILED_ID;

      // delete working objects in texture view
      tbOrthoScene.DeleteObject(tbFaceOrthoID);
      tbFaceOrthoID = FAILED_ID;
      tbOrthoScene.DeleteObject(tbFaceOrthoEdgeID);
      tbFaceOrthoEdgeID = FAILED_ID;
      tbOrthoScene.DeleteObject(tbCurrentFaceMaskID);
      tbCurrentFaceMaskID = FAILED_ID;

      if (tbCurrentFaceMask != NULL) {
         FyFree(tbCurrentFaceMask->blendFace);
         FyFree(tbCurrentFaceMask->cornerVert);
         FyFree(tbCurrentFaceMask->maskFace);
         FyFree(tbCurrentFaceMask->maskVert);
         FyFree(tbCurrentFaceMask);
         tbCurrentFaceMask = NULL;
      }

      uiRB.ID(tbOrthoButtonID);
      uiRB.ChangeState(TRUE);
      uiRB.Enable(FALSE);

      // switch to 3D view
      TbShowOrthoView(tbOrthoButtonID, TRUE);

      tbTool = SYSTEM_TOOL;
   }
}


/*--------------------------------
  make the system to model viewer
  C.Wang 0115, 2014
 ---------------------------------*/
void TbModelUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.ID(tbModelSystemButtonID);
      button.ChangeState(TRUE);

      // hide the others
      button.ID(tbEnvironmentSystemButtonID);
      button.Show(FALSE);
      button.ID(tbScriptSystemButtonID);
      button.Show(FALSE);
      button.ID(tbPostProcessingSystemButtonID);
      button.Show(FALSE);
      button.ID(tbFaceToolButtonID);
      button.Show(FALSE);

      button.ID(tbOrthoButtonID);
      button.ChangeState(TRUE);
      button.Enable(FALSE);

      // switch to 3D view
      TbShowOrthoView(tbOrthoButtonID, TRUE);

      TbSwitchMenuBarRadios(tbModelSystemButtonID, TRUE);
      tbTool = MODEL_TOOL;
   }
}


/*---------------------------------
  make the system to script loader
  C.Wang 0115, 2014
 ----------------------------------*/
void TbScriptUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.ID(tbScriptSystemButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.ID(tbEnvironmentSystemButtonID);
      button.Show(FALSE);
      button.ID(tbPostProcessingSystemButtonID);
      button.Show(FALSE);
      button.ID(tbFaceToolButtonID);
      button.Show(FALSE);

      button.ID(tbOrthoButtonID);
      button.ChangeState(TRUE);
      button.Enable(FALSE);

      // switch to 3D view
      TbShowOrthoView(tbOrthoButtonID, TRUE);

      TbSwitchMenuBarRadios(tbScriptSystemButtonID, TRUE);
      tbTool = SYSTEM_TOOL;
   }
}


/*------------------------------------------------
  make the system to setup post-processing effect
  C.Wang 0115, 2014
 -------------------------------------------------*/
void TbPostProcessingUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.ID(tbPostProcessingSystemButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.ID(tbEnvironmentSystemButtonID);
      button.Show(FALSE);

      button.ID(tbScriptSystemButtonID);
      button.Show(FALSE);

      button.ID(tbFaceToolButtonID);
      button.Show(FALSE);

      button.ID(tbOrthoButtonID);
      button.ChangeState(TRUE);
      button.Enable(FALSE);

      // switch to 3D view
      TbShowOrthoView(tbOrthoButtonID, TRUE);

      TbSwitchMenuBarRadios(tbPostProcessingSystemButtonID, TRUE);
      tbTool = SYSTEM_TOOL;
   }
}


/*--------------------
  initialie face tool
  C.Wang 0512, 2014
 ---------------------*/
void TbInitFaceTool(int iOne)
{
   // check if there're "Face" skin models
   FnUiTypein uiTy;
   FnUiCombo uiC;
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   int i;
   BOOL4 beBS = FALSE;

   TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();

   if (FyCheckObjectValidity(tbCurrentFaceModelID)) {
      // enable UIs
      uiB.ID(tbLoadFaceMaskButtonID);
      uiB.Enable(TRUE);
      uiB.ID(tbSaveFaceMaskButtonID);
      uiB.Enable(TRUE);
      uiC.ID(tbFindFaceMaterialComboID);
      uiC.Enable(TRUE);
      uiC.ClearMessages();

      // paste the materials to combo
      FnObject model(tbCurrentFaceModelID);
      int numMat = model.GetMaterialNumber();
      MATERIALid matID[256];
      FnMaterial mat;

      model.GetMaterials(matID, numMat);
      for (i = 0; i < numMat; i++) {
         mat.ID(matID[i]);
         uiC.InsertMessage(i, mat.GetName());
      }

      if (iOne < 0 || iOne >= numMat) {
         mat.ID(matID[0]);
         tbMatSlotUsed = 0;
      }
      else {
         mat.ID(matID[iOne]);
         tbMatSlotUsed = iOne;
      }
      uiC.PasteComboValue(mat.GetName());

      // enable 2D/3D view switch and switch to 3D view
      uiRB.ID(tbOrthoButtonID);
      uiRB.Enable(TRUE);
      //uiRB.ChangeState(TRUE);
      //TbShowOrthoView(tbOrthoButtonID, TRUE);

      // check if there're blendshapes for the head
      if (actor->GetBlendShapeNumber(0) > 0) {
         OBJECTid baseID = actor->GetBaseShape(0);
         if (baseID == tbCurrentFaceModelID) {
            FnUiTab uiT(tbBlendshapeTabID);
            uiT.Enable(TRUE);
            uiT.Open(TRUE);

            // flag indicated that there're blendshapes
            beBS = TRUE;
         }
         else {
            FnUiTab uiT(tbBlendshapeTabID);
            uiT.Open(FALSE);
            uiT.Enable(FALSE);
         }
      }
      else {
         FnUiTab uiT(tbBlendshapeTabID);
         uiT.Open(FALSE);
         uiT.Enable(FALSE);
      }
   }
   else {
      if (actor == NULL) {
         uiC.ID(tbFindFaceModelComboID);
         uiC.Enable(FALSE);
      }
      else {
         uiC.ID(tbFindFaceModelComboID);
         uiC.Enable(TRUE);
      }

      if (actor != NULL) {
         OBJECTid skinID;
         FnObject skin;
         int count, numSkin = actor->GetSkinNumber();

         uiC.ClearMessages();
         for (count = 0, i = 0; i < numSkin; i++) {
            skinID = actor->GetSkin(i);
            skin.ID(skinID);
            if (skin.GetName() != NULL && skin.GetVisibility()) {
               // check current face model name
               uiC.InsertMessage(count, skin.GetName());
               count++;
            }
         }
      }

      tbCurrentFaceModelID = FAILED_ID;
      tbFaceModelName[0] = '\0';   // no current skin model
      uiC.PasteComboValue(tbFaceModelName);

      // disable UIs
      uiB.ID(tbLoadFaceMaskButtonID);
      uiB.Enable(FALSE);
      uiB.ID(tbSaveFaceMaskButtonID);
      uiB.Enable(FALSE);
      uiC.ID(tbFindFaceMaterialComboID);
      uiC.Enable(FALSE);
      uiC.ClearMessages();
      uiC.PasteComboValue("");
      tbMatSlotUsed = FY_NONE;

      uiRB.ID(tbOrthoButtonID);
      uiRB.Enable(FALSE);
      uiRB.ChangeState(TRUE);
      TbShowOrthoView(tbOrthoButtonID, TRUE);

      FnUiTab uiT(tbBlendshapeTabID);
      uiT.Open(FALSE);
      uiT.Enable(FALSE);
   }

   if (beBS) {
      char *name;
      int numBS = actor->GetBlendShapeNumber(0);   // we don't need the base shape
      for (i = 0; i < numBS; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.Show(TRUE);

         name = actor->GetBlendShapeName(0, i);
         uiTy.SetCaption(LEFT_CENTERED, 2, 5, name, 0.0f, 0.0f, 0.0f);
      }
      for (i = numBS; i < MAX_BLENDSHAPES; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.Show(FALSE);
      }

      TbUpdateCurrentFacialExpressionUIs();
   }
   else {
      for (i = 0; i < MAX_BLENDSHAPES; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.Show(FALSE);
      }

      TbUpdateCurrentFacialExpressionUIs();
   }
}


/*-----------------------------
  make the system to face tool
  C.Wang 0115, 2014
 ------------------------------*/
void TbFaceToolUICommand(UIid uiID, BOOL4 bePress)
{
   BOOL4 beOK = FALSE;
   if (uiID == tbFaceToolButtonID) {
      if (bePress) {
         beOK = TRUE;
      }
      else {
         TbSwitchMenuBarRadios(uiID, bePress);
         return;
      }
   }
   else if (uiID == tbFaceButtonID) {
      if (!bePress) {
         beOK = TRUE;
      }
   }

   if (beOK) {
      FnUiRadioButton button;
      button.ID(tbFaceToolButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.ID(tbEnvironmentSystemButtonID);
      button.Show(FALSE);

      button.ID(tbScriptSystemButtonID);
      button.Show(FALSE);

      button.ID(tbPostProcessingSystemButtonID);
      button.Show(FALSE);

      TbSwitchMenuBarRadios(tbFaceToolButtonID, TRUE);
      tbTool = FACE_TOOL;

      TbInitFaceTool(FY_NONE);
   }
}


/*---------------------------------------------------------
  create a Q3D face mask (which was defined by Ricky Choa)
  C.Wang 0120, 2014
 ----------------------------------------------------------*/
OBJECTid TbCreateFaceMask(OBJECTid fID, TBFaceMaskForQ3D *mask)
{
   OBJECTid oID;
   FnObject face(fID);
   GEOMETRYid gID = face.GetGeometryID(0);
   FnTriangle triM(gID);
   if (gID == FAILED_ID) return FAILED_ID;
   FnScene scene;
   FnObject maskModel;

   int i, vLen, nV, nT, *index, len;
   float *v = triM.LockVertexArray(&vLen, &nV);

   // get all vertices
   len = sizeof(float)*vLen*nV;
   float *newV = (float *)FyAllocateBuffer(len);
   memcpy(newV, v, len);
   triM.UnlockVertexArray();

   // get all triangle indexes
   int tLen, iOne;
   BOOL4 beR;
   index = triM.LockIndexArray(&tLen, &nT, FALSE, &beR);

   len = sizeof(int)*3*nT;
   int *triNoSort = (int *)FyAllocateBuffer(len);
   int *triSorted = (int *)FyAllocateBuffer(len);
   BOOL4 *selected = (BOOL4 *)FyAllocateBuffer(sizeof(BOOL4)*nT);

   if (beR) {
      memcpy(triNoSort, index, len);
   }
   else {
      for (i = 0; i < nT; i++) {
         triNoSort[i*3] = index[i*3 + 2];
         triNoSort[i*3 + 1] = index[i*3 + 1];
         triNoSort[i*3 + 2] = index[i*3];
      }
   }
   triM.UnlockIndexArray();

   // create the face mask model
   scene.ID(face.GetScene());
   oID = scene.CreateObject(OBJECT);
   maskModel.ID(oID);

   // sort the index according to the mask data
   memset(selected, 0, sizeof(BOOL4)*nT);
   for (i = 0; i < mask->numMaskFace; i++) {
      iOne = mask->maskFace[i];
      selected[iOne] = TRUE;
   }

   int iHead, iTail;
   iHead = 0;
   iTail = nT*3 - 3;
   for (i = 0; i < nT; i++) {
      if (selected[i]) {
         memcpy(&triSorted[iHead], &triNoSort[i*3], sizeof(int)*3);
         iHead += 3;
      }
      else {
         memcpy(&triSorted[iTail], &triNoSort[i*3], sizeof(int)*3);
         iTail -= 3;
      }
   }

   float color[3];
   int texLen = 2;
   color[0] = 1.0f; color[1] = color[2] = 0.0f;
   MATERIALid mID = FyCreateMaterial(color, color, color, 1.0f, color);
   maskModel.SetOpacity(0.2f);

   // mask region
   maskModel.Triangles(XYZ_NORM, mID, 1, nV, 1, &texLen, newV, nT, triSorted, 0, TRUE, NULL, mask->numMaskFace);

   // sort the index according to the blending zone data
   memset(selected, 0, sizeof(BOOL4)*nT);
   for (i = 0; i < mask->numBlendFace; i++) {
      iOne = mask->blendFace[i];
      selected[iOne] = TRUE;
   }

   iHead = 0;
   iTail = nT*3 - 3;
   for (i = 0; i < nT; i++) {
      if (selected[i]) {
         memcpy(&triSorted[iHead], &triNoSort[i*3], sizeof(int)*3);
         iHead += 3;
      }
      else {
         memcpy(&triSorted[iTail], &triNoSort[i*3], sizeof(int)*3);
         iTail -= 3;
      }
   }

   // blending region
   color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f;
   mID = FyCreateMaterial(color, color, color, 1.0f, color);
   maskModel.Triangles(XYZ_NORM, mID, 1, nV, 1, &texLen, newV, nT, triSorted, 0, TRUE, NULL, mask->numBlendFace);

   // release the buffers
   FyReleaseBuffer(selected);
   FyReleaseBuffer(triSorted);
   FyReleaseBuffer(triNoSort);
   FyReleaseBuffer(newV);

   return oID;
}


/*---------------------------------------------------------------
  adjust Q3D face mask structure (which was defined by Eric Fei)
  Eric Fei 0205, 2014
 ----------------------------------------------------------------*/
TBFaceMaskForQ3D *TbAdjustQ3DFaceMask(OBJECTid fID, int startX, int startY, int endX, int endY, int mode)
{
   // get current face vertices/triangle number
   int i, j;
   int nV, nT;
   FnObject faceModel(fID);
   GEOMETRYid gID = faceModel.GetGeometryID(0);
   FnTriangle triM(gID);
   nV = triM.GetVertexNumber(NULL);
   nT = triM.GetTriangleNumber();

   // copy/create FaceMask from tbCurrentFaceMask
   TBFaceMaskForQ3D *mask = (TBFaceMaskForQ3D *) FyMalloc(sizeof(TBFaceMaskForQ3D));

   if (tbCurrentFaceMask != NULL) {
      mask->numMaskFaceVert = tbCurrentFaceMask->numMaskFaceVert;
      mask->maskVert = (int *)FyMalloc(sizeof(int)*(nV));
      memset(mask->maskVert, -1, sizeof(int)*(nV));

      mask->numMaskFace = tbCurrentFaceMask->numMaskFace;
      mask->maskFace = (int *)FyMalloc(sizeof(int)*(nT));
      memcpy(mask->maskFace, tbCurrentFaceMask->maskFace, sizeof(int)*(tbCurrentFaceMask->numMaskFace));

      mask->numCornerVert = tbCurrentFaceMask->numCornerVert;
      mask->cornerVert = (int *)FyMalloc(sizeof(int)*(nV));
      memset(mask->cornerVert, -1, sizeof(int)*(nV));

      mask->numBlendFace = tbCurrentFaceMask->numBlendFace;
      mask->blendFace = (int *)FyMalloc(sizeof(int)*(nT));
      memset(mask->blendFace, -1, sizeof(int)*(nT));
   }
   else {
      mask->numMaskFaceVert = 0;
      mask->maskVert = (int *)FyMalloc(sizeof(int)*(nV));
      memset(mask->maskVert, -1, sizeof(int)*(nV));

      mask->numMaskFace = 0;
      mask->maskFace = (int *)FyMalloc(sizeof(int)*(nT));
      memset(mask->maskFace, -1, sizeof(int)*(nT));

      mask->numCornerVert = 0;
      mask->cornerVert = (int *)FyMalloc(sizeof(int)*(nV));
      memset(mask->cornerVert, -1, sizeof(int)*(nV));

      mask->numBlendFace = 0;
      mask->blendFace = (int *)FyMalloc(sizeof(int)*(nT));
      memset(mask->blendFace, -1, sizeof(int)*(nT));
   }

   // get all vertices
   int vLen, len;
   float *v, *vCP;
   v = triM.LockVertexArray(&vLen, &nV);
   len = sizeof(float)*vLen*nV;
   vCP = (float *)FyAllocateBuffer(len);
   memcpy(vCP, v, len);
   triM.UnlockVertexArray();
   
   // get all triangle indexes
   BOOL4 beR;
   int tLen, *idx, *idxCP;
   idx = triM.LockIndexArray(&tLen, &nT, FALSE, &beR);
   len = sizeof(int)*tLen*nT;
   idxCP = (int *)FyAllocateBuffer(len);
   if (beR) {
      memcpy(idxCP, idx, len);
   }
   else {
      for (i = 0; i < nT; i++) {
         idxCP[i*3] = idx[i*3 + 2];
         idxCP[i*3 + 1] = idx[i*3 + 1];
         idxCP[i*3 + 2] = idx[i*3];
      }
   }
   triM.UnlockIndexArray();

   // check single click
   float vDist[2] = {(float)(endX - startX), (float)(endY - startY)};
   BOOL4 bClick = FALSE;

   if ((vDist[0]*vDist[0]+vDist[1]*vDist[1]) < 3.0f) bClick = TRUE;

   // add/sub to face mask
   if (bClick) {
      // only detect end point
      float xyz[3];
      int idxFace;
      GEOMETRYid geoID = tbWorkSpaceVP.HitPosition(tbFaceOrthoID, tbOrthoCamID, endX, endY, xyz, &idxFace);
      FnTriangle tri(geoID);
      if (geoID != FAILED_ID) {
         TbAddSubFaceMaskTri(mask, idxFace, mode);
      }
   }
   else {
      // remove viewport offset
      startX -= tbWorkSpaceVPOX;
      startY -= tbWorkSpaceVPOY;
      endX -= tbWorkSpaceVPOX;
      endY -= tbWorkSpaceVPOY;

      // get bound XY
      int minX, maxX, minY, maxY;
      if (startX < endX) {
         minX = startX;
         maxX = endX;
      }
      else {
         minX = endX;
         maxX = startX;
      }

      if (startY < endY) {
         minY = startY;
         maxY = endY;
      }
      else {
         minY = endY;
         maxY = startY;
      }

#ifdef _DEBUG
      char debugBuf[256];
      sprintf(debugBuf, "startX = %d,  startY = %d,  endX = %d,  endY = %d\n", startX, startY, endX, endY);
      OutputDebugString(debugBuf);
      sprintf(debugBuf, "minX = %d,  maxX = %d,  minY = %d,  maxY = %d\n", minX, maxX, minY, maxY);
      OutputDebugString(debugBuf);
#endif

      // check square area
      float pt2D[3];
      float pt3D[3];
      int *idxTriInRng = (int *) FyAllocateBuffer(sizeof(int)*nT);  // selected triangle
      int count = 0;                                               // selected triangle number
      for (i = 0; i < nT; i++) {
         // Check 3 Points of Current Triangle
         for (j = 0; j < 3; j++) {
            pt3D[0] = vCP[idxCP[i*tLen+j]*vLen];
            pt3D[1] = vCP[idxCP[i*tLen+j]*vLen+1];
            pt3D[2] = vCP[idxCP[i*tLen+j]*vLen+2];
            tbWorkSpaceVP.ComputeScreenPosition(tbOrthoCamID, pt2D, pt3D, PHYSICAL_SCREEN, FALSE);    // get screen position

            // check square area
            if (pt2D[0] > minX && pt2D[0] < maxX && pt2D[1] > minY && pt2D[1] < maxY) {
               idxTriInRng[count] = i;
               count++;
               break;
            }
         }
      }
      
      // add/sub to face mask
      for (i = 0; i < count; i++) {
         TbAddSubFaceMaskTri(mask, idxTriInRng[i], mode);
      }

      // release resource
      FyReleaseBuffer(idxTriInRng);
   }

   // sort face mask
   TbHeapsortSingle(mask->numMaskFace, mask->maskFace);

   // get all face mask vertices
   int *rVerts = (int *) FyAllocateBuffer(sizeof(int)*(nT)*3);    // red region vertices
   int countRVerts = 0;                                         // red region vertices number
   for (i = 0, countRVerts = 0; i <mask->numMaskFace; i++) {
      rVerts[countRVerts++] = idxCP[mask->maskFace[i]*3+0];
      rVerts[countRVerts++] = idxCP[mask->maskFace[i]*3+1];
      rVerts[countRVerts++] = idxCP[mask->maskFace[i]*3+2];
   }

   // create triangle of red region
   OBJECTid regionOID = FAILED_ID;     // object with red resion only
   if (countRVerts >= 3) {
      int texLen = 2;
      regionOID = tbOrthoScene.CreateObject();
      FnObject obj(regionOID);
      obj.Triangles(XYZ_NORM, FAILED_ID, 1, nV, 1, &texLen, vCP, countRVerts/3, rVerts);
   }
   
   // update face mask vertices
   int countCopy = countRVerts;                                       // red region vertices number copy
   int *rVertsCopy = (int *) FyAllocateBuffer(sizeof(int)*countCopy);   // red region vertices copy
   memcpy(rVertsCopy, rVerts, sizeof(int)*countCopy);
   TbHeapsortSingle(countCopy, rVertsCopy);
   TbRemoveRepeatData(&countCopy, rVertsCopy);
   if (countCopy <= nV) {
      memcpy(mask->maskVert, rVertsCopy, sizeof(int)*countCopy);
      mask->numMaskFaceVert = countCopy;
   }
   FyReleaseBuffer(rVertsCopy);
   rVertsCopy = NULL;

   // get boundary vertices
   if (regionOID != FAILED_ID) {
      // Has Red Region
      FnObject triBObj(regionOID);
      FnTriangle triB(triBObj.GetGeometryID(0));
      int numEdges = triB.GetEdgeNumber();                                 // Edges of Red Region
      int countBFace = 0, countBVert = 0;                                  // Boundaray Faces/Vertices Number
      int *bdFaces = (int *) FyAllocateBuffer(sizeof(int)*(numEdges)*2);   // Boundary Faces
      int *bdVerts = (int *) FyAllocateBuffer(sizeof(int)*(numEdges)*2);   // Boundary Vertices
      for (i = 0; i < numEdges; i++) {
         int f0, f1, v0, v1;
         f0 = f1 = v0 = f1 = -1;
         
         // check each edge
         triB.GetEdgeFaces(i, &f0, &f1);
         if ( (f0 < 0 && f1 >= 0) || (f0 >= 0 && f1 < 0) ) { // check bound triangle
            // get boundary vertices
            triB.GetEdgeVertices(i, &v0, &v1);
            if (v0 >= 0) bdVerts[countBVert++] = v0;
            if (v1 >= 0) bdVerts[countBVert++] = v1;
         }
      }

      // update Mask Vertices
      TbHeapsortSingle(countBVert, bdVerts);
      TbRemoveRepeatData(&countBVert, bdVerts);
      mask->numCornerVert = countBVert;
      memcpy(mask->cornerVert, bdVerts, sizeof(int)*(countBVert));

      // update Mask Triangle
      for (i = 0; i < countBVert; i++) {
         for (j = 0; j < countRVerts/3; j++) {
            if ((rVerts[j*3+0] == bdVerts[i]) || (rVerts[j*3+1] == bdVerts[i]) || 
                (rVerts[j*3+2] == bdVerts[i])) bdFaces[countBFace++] = j;
         }
      }

      TbHeapsortSingle(countBFace, bdFaces);
      TbRemoveRepeatData(&countBFace, bdFaces);
      mask->numBlendFace = countBFace;
      for (i = 0; i < countBFace; i++) {
         mask->blendFace[i] = mask->maskFace[bdFaces[i]];
      }
      FyReleaseBuffer(bdFaces);
      FyReleaseBuffer(bdVerts);
   }
   else {
      // no Red Region
      mask->numBlendFace = 0;
      mask->numCornerVert = 0;
   }
   
   // release the buffers
   FyReleaseBuffer(vCP);
   FyReleaseBuffer(idxCP);
   FyReleaseBuffer(rVerts);

   // release regionOID
   tbOrthoScene.DeleteObject(regionOID);

   return mask;
}


/*---------------------------------------------------------------
  add/sub mask triangle indexs (which was defined by Eric Fei)
  Eric Fei 0202, 2014
 ----------------------------------------------------------------*/
void TbAddSubFaceMaskTri(TBFaceMaskForQ3D *mask, int idxTri, int mode)
{
   int i;

   switch(mode)
   {
   case FT_MASK_ADD:
      {
         // check idxFace existed
         for (i = 0; i<mask->numMaskFace; i++) {
            if (mask->maskFace[i] == idxTri) break;
         }

         // add idxFace
         if (i == mask->numMaskFace) {
            mask->maskFace[i] = idxTri;
            mask->numMaskFace++;
         }
      }
      break;
   case FT_MASK_SUB:
      {
         // find idxFace
         for (i = 0; i < mask->numMaskFace; i++) {
            if (mask->maskFace[i] == idxTri) {
               // move the last
               if (i + 1 < mask->numMaskFace) {
                  memcpy(&mask->maskFace[i], &mask->maskFace[i+1], sizeof(int)*(mask->numMaskFace-(i+1)));
               }

               // countdown
               mask->numMaskFace--;
            }
         }
      }
      break;
   }
}


/*----------------------------------------------------------
  left mouse control for adjusting face mask (just pressed)
  Eric Fei 0205, 2014
 -----------------------------------------------------------*/
void TbAdjustFaceMaskLMPress(int x, int y)
{
   tbFTStartX = x;
   tbFTStartY = y;

   // get selected square area start point
   if (tbSquareAreaID) {
      FnObject square(tbSquareAreaID);
      GEOMETRYid geoID;
      int vLen, nV, idxFace;
      float *v, xyz[3];

      square.GetGeometries(&geoID, 1);
      v = square.LockVertexBuffer(geoID, &nV, &vLen);
      if (tbWorkSpaceVP.HitPosition(tbFaceOrthoID, tbOrthoCamID, x, y, xyz, &idxFace)) {
         v[0] = xyz[0];
         v[1] = xyz[1];
         v[6] = xyz[0];
         v[7] = xyz[1] + 0.1f;
         v[12] = xyz[0] + 0.1f;
         v[13] = xyz[1] + 0.1f;
         v[18] = xyz[0] + 0.1f;
         v[19] = xyz[1];
         tbBeFTPressed = TRUE;
      }
      square.UnlockVertexBuffer(geoID);
   }
}


/*-------------------------------------------------------------
  left mouse control for adjusting face mask (during dragging)
  Eric Fei 0205, 2014
 --------------------------------------------------------------*/
void TbAdjustFaceMaskLMDrag(int x, int y)
{
   tbFTEndX = x;
   tbFTEndY = y;

   // adjust selected square area
   if (tbBeFTPressed && tbSquareAreaID) {
      FnObject square(tbSquareAreaID);
      GEOMETRYid geoID;
      int vLen, nV, idxFace;
      float *v, xyz[3];

      square.GetGeometries(&geoID, 1);
      v = square.LockVertexBuffer(geoID, &nV, &vLen);
      tbWorkSpaceVP.HitPosition(tbFaceOrthoID, tbOrthoCamID, x, y, xyz, &idxFace);
      if (idxFace >= 0) {
         v[7] = xyz[1];     // v1.y
         v[12] = xyz[0];    // v2.x
         v[13] = xyz[1];    // v2.y
         v[18] = xyz[0];    // v3.x
      }
      square.UnlockVertexBuffer(geoID);
      square.Show(TRUE);
   }
}


/*-----------------------------------------------------
  left mouse control for adjusting face mask (release)
  Eric Fei 0205, 2014
 ------------------------------------------------------*/
void TbAdjustFaceMaskLMRelease(int x, int y)
{
   tbFTEndX = x;
   tbFTEndY = y;

   // check face tool left mouse press flag
   if (!tbBeFTPressed) return;
   tbBeFTPressed = FALSE;

   // hide selected square area
   if (tbSquareAreaID) {
      FnObject square(tbSquareAreaID);
      square.Show(FALSE);
   }

   // Excute Adjust Face Mask
   TbAdjustFaceMaskCreateCommand();
}


/*-------------------------------------------
  Create AdjustFaceMask Command
  Eric Fei 0205, 2014
 --------------------------------------------*/
void TbAdjustFaceMaskCreateCommand()
{
   // extract the data from current variable
   char argument[256], value[256];
   argument[0] = '\0';

   strcat(argument, "-sX ");
   sprintf(value, "%d", tbFTStartX);
   strcat(argument, value);
   strcat(argument, " -sY ");
   sprintf(value, "%d", tbFTStartY);
   strcat(argument, value);
   
   strcat(argument, " -eX ");
   sprintf(value, "%d", tbFTEndX);
   strcat(argument, value);
   strcat(argument, " -eY ");
   sprintf(value, "%d", tbFTEndY);
   strcat(argument, value);
   
   strcat(argument, " -mode ");
   sprintf(value, "%d", tbMaskOpMode);
   strcat(argument, value);

   // excute command
   TbCommandParser("AdjustFaceMask", argument, FALSE);
}


/*---------------------------
  Heap Sort for Single Array
  Eric Fei 0205, 2014
 ----------------------------*/
void TbHeapsortSingle(int num, int * ary)
{
   if (num <= 1) return;

   int l, j, ir, i, index1, index2;
   int tmp;

   l = (num >> 1) + 1;
   ir = num;

   for (;;) {
      if (l > 1) {
         --l;
         index1 = l - 1;
         tmp = ary[index1];
      }
      else {
         index1 = 0;
         index2 = ir - 1;
         tmp = ary[index2];
         ary[index2] = ary[index1];
         if (--ir == 1) {
            ary[index1] = tmp;
            return;
         }
      }

      i = l;
      j = l << 1;
      while (j <= ir) {
         index1 = j;
         index2 = j - 1;
         if (j < ir && ary[index2] < ary[index1]) ++j;

         index1 = i - 1;
         index2 = j - 1;
         if (tmp < ary[index2]) {
            ary[index1] = ary[index2];
            j += (i = j);
         }
         else {
            j = ir + 1;
         }
      }
      index1 = i - 1;
      ary[index1] = tmp;
   }
}


/*-------------------------------------------
  Remove Repeat Data in Single Array
  Eric Fei 0205, 2014
 --------------------------------------------*/
void TbRemoveRepeatData(int * num, int * ary)
{
   for (int i = 1; i < *num; i++) {
      if (ary[i-1] == ary[i]) {
         memcpy(&ary[i], &ary[i+1], sizeof(int)*(*num-1-i));
         (*num)--;
         i--;
      }
   }
}


/*----------------------------------------------------------
  save Q3D face mask file (which was defined by Ricky Choa)
  C.Wang 0120, 2014
 -----------------------------------------------------------*/
 void TbSaveQ3DFaceMask(char *fileName, TBFaceMaskForQ3D *mask)
{
   if (mask == NULL) return;

   // open the file
   FILE *fp = fopen(fileName, "wt");
   if (fp == NULL) return;

   int i, *index;
   fprintf(fp, "NumVertices: %d\n", mask->numMaskFaceVert);
   index = mask->maskVert;
   for (i = 0; i < mask->numMaskFaceVert; i++) {
      fprintf(fp, "%d\n", index[i]);
   }

   fprintf(fp, "\nNumTriangles: %d\n", mask->numMaskFace);
   index = mask->maskFace;
   for (i = 0; i < mask->numMaskFace; i++) {
      fprintf(fp, "%d\n", index[i]);
   }

   fprintf(fp, "\nNumCornerVertices: %d\n", mask->numCornerVert);
   index = mask->cornerVert;
   for (i = 0; i < mask->numCornerVert; i++) {
      fprintf(fp, "%d\n", index[i]);
   }

   fprintf(fp, "\nNumSmoothTriangles: %d\n", mask->numBlendFace);
   index = mask->blendFace;
   for (i = 0; i < mask->numBlendFace; i++) {
      fprintf(fp, "%d\n", index[i]);
   }

   // close the file
   fclose(fp);
}


/*----------------------------------------------------------
  load Q3D face mask file (which was defined by Ricky Choa)
  C.Wang 0120, 2014
 -----------------------------------------------------------*/
TBFaceMaskForQ3D *TbLoadQ3DFaceMask(char *fileName, int nV, int nT)
{
   // open the file
   FILE *fp = fopen(fileName, "rt");
   if (fp == NULL) return NULL;

   TBFaceMaskForQ3D *mask = (TBFaceMaskForQ3D *) FyMalloc(sizeof(TBFaceMaskForQ3D));
   mask->maskVert = NULL;
   mask->numMaskFaceVert = 0;
   mask->maskFace = NULL;
   mask->numMaskFace = 0;
   mask->cornerVert = NULL;
   mask->numCornerVert = 0;
   mask->blendFace = NULL;
   mask->numBlendFace = 0;

   int i, len, num, *index;
   char s[256], s2[256], *key, *key2;

   while ((len = FyReadNewLine(fp, s)) >= 0) {
      FyTruncateStringSpace(s);
      len = (int) strlen(s);
      if (s[0] != '#' && strlen(s) > 0) {
         key = strtok(s, " ");
         if (FyCheckWord(key, "NumVertices:", TRUE)) {
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               num = mask->numMaskFaceVert = atoi(key2);
               index = mask->maskVert = (int *)FyMalloc(sizeof(int)*nV);
               for (i = 0; i < num; i++) {
                  // get a new line
                  while ((len = FyReadNewLine(fp, s2)) >= 0) {
                     FyTruncateStringSpace(s2);
                     len = (int) strlen(s2);
                     if (s2[0] != '#' && strlen(s2) > 0) {
                        break;
                     }
                  }

                  index[0] = atoi(s2);
                  index++;
               }
            }
         }
         else if (FyCheckWord(key, "NumTriangles:", TRUE)) {
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               num = mask->numMaskFace = atoi(key2);
               index = mask->maskFace = (int *)FyMalloc(sizeof(int)*nT);
               for (i = 0; i < num; i++) {
                  // get a new line
                  while ((len = FyReadNewLine(fp, s2)) >= 0) {
                     FyTruncateStringSpace(s2);
                     len = (int) strlen(s2);
                     if (s2[0] != '#' && strlen(s2) > 0) {
                        break;
                     }
                  }

                  index[0] = atoi(s2);
                  index++;
               }
            }
         }
         else if (FyCheckWord(key, "NumCornerVertices:", TRUE)) {
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               num = mask->numCornerVert = atoi(key2);
               index = mask->cornerVert = (int *)FyMalloc(sizeof(int)*nV);
               for (i = 0; i < num; i++) {
                  // get a new line
                  while ((len = FyReadNewLine(fp, s2)) >= 0) {
                     FyTruncateStringSpace(s2);
                     len = (int) strlen(s2);
                     if (s2[0] != '#' && strlen(s2) > 0) {
                        break;
                     }
                  }

                  index[0] = atoi(s2);
                  index++;
               }
            }
         }
         else if (FyCheckWord(key, "NumSmoothTriangles:", TRUE)) {
            key2 = strtok(NULL, " ");
            if (key2 != NULL) {
               num = mask->numBlendFace = atoi(key2);
               index = mask->blendFace = (int *)FyMalloc(sizeof(int)*nT);
               for (i = 0; i < num; i++) {
                  // get a new line
                  while ((len = FyReadNewLine(fp, s2)) >= 0) {
                     FyTruncateStringSpace(s2);
                     len = (int) strlen(s2);
                     if (s2[0] != '#' && strlen(s2) > 0) {
                        break;
                     }
                  }

                  index[0] = atoi(s2);
                  index++;
               }
            }
         }
      }
   }

   // close the file
   fclose(fp);

   return mask;
}


/*---------------------------------------------------------
  select the face as the current editing one for face tool
  C.Wang 0115, 2014
 ----------------------------------------------------------*/
void TbSelectFaceModelComboUICommand(UIid uiID, char *value)
{
   // make the current face
   TbCommandParser("SelectFace", value, FALSE);
}


/*-------------------------------------
  select the face material for editing
  C.Wang 0117, 2014
 --------------------------------------*/
void TbSelectFaceMaterialComboUICommand(UIid uiID, char *value)
{
   // make the current face material
   TbCommandParser("SelectFaceMaterial", value, FALSE);
}



/*-------------------------------------
  open the load face mask file browser
  C.Wang 0120, 2014
 --------------------------------------*/
void TbLoadFaceMaskUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("FaceMask", NULL, FALSE);
   }
}


/*-------------------------------------
  open the save face mask file browser
  C.Wang 0120, 2014
 --------------------------------------*/
void TbSaveFaceMaskUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("SaveFaceMask", NULL, FALSE);
   }
}


/*----------------------------------
  open the load script file browser
  C.Wang 0813, 2012
 -----------------------------------*/
void TbLoadScriptUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("Load", NULL, FALSE);
   }
}


/*---------------------------------
  open the load model file browser
  C.Wang 0813, 2012
 ----------------------------------*/
void TbLoadModelUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("Model", NULL, FALSE);
   }
}


/*---------------------------------
  open the save model file browser
  C.Wang 0813, 2012
 ----------------------------------*/
void TbSaveModelUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("SaveModel", NULL, FALSE);
   }
}


/*------------------------------
  find current model (by combo)
  C.Wang 0610, 2013
 -------------------------------*/
void TbFindModelComboUICommand(UIid uiID, char *value)
{
   // execute command to find current model
   TbCommandParser("FindModel", value, FALSE);
}


/*-------------------------------
  find current model (by typein)
  C.Wang 0917, 2014
 --------------------------------*/
void TbFindModelTypeinUI(UIid uiID, char *value)
{
   int i, len;
   char string[256];
   FnUiCombo uiC(uiID);
   char *name, nameCheck[256];

   int count;
   OBJECTid oID;
   TBObject *tbO;

   int numO = tbCurScene->GetObjectNumber();

   strcpy(string, value);
   FyTruncateStringSpace(string);
   len = (int) strlen(string);

   // popup the message box
   uiC.ID(tbFindModelComboID);
   uiC.PopupMessageBox(TRUE);

   if (len == 0) {
      uiC.ClearMessages();

      // insert all models
      count = 0;
      for (i = 0; i < numO; i++) {
         oID = tbCurScene->GetObjectID(i);
         if (oID != FAILED_ID) {
            tbO = tbCurScene->GetTBObject(oID);
            if (tbO->IsGeometry()) {
               name = tbO->GetName();
               uiC.InsertMessage(count, name);
               count++;
            }
         }
      }
   }
   else {
      uiC.ClearMessages();

      // insert all models if the naming is matching
      count = 0;
      for (i = 0; i < numO; i++) {
         oID = tbCurScene->GetObjectID(i);
         if (oID != FAILED_ID) {
            tbO = tbCurScene->GetTBObject(oID);
            if (tbO->IsGeometry()) {
               name = tbO->GetName();
               strcpy(nameCheck, name);
               if (FyCheckWord(nameCheck, string, TRUE, len)) {
                  uiC.InsertMessage(count, name);
                  count++;
               }
            }
         }
      }
   }
}


/*-------------------------------------
  edit current action for current body
  C.Wang 0813, 2012
 --------------------------------------*/
void TbEditActionUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      tbAddActionUI.PopUp(FALSE);

      // extract the data from UIs
      FnUiCombo uiC;
      FnUiTypein uiTy;
      char argument[256], value[256];
      argument[0] = '\0';

      strcat(argument, "-front ");
      uiC.ID(tbAddActionFrontPoseID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -rear ");
      uiC.ID(tbAddActionEndPoseID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -length ");
      uiTy.ID(tbAddActionDurationID);
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      // execute command to modify current action
      TbCommandParser("EditAction", argument, FALSE);
   }
}


/*-------------------------------------
  create a new action for current body
  C.Wang 0813, 2012
 --------------------------------------*/
void TbNewActionUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      tbAddActionUI.PopUp(FALSE);

      // extract the data from UIs
      FnUiCombo uiC;
      FnUiTypein uiTy;
      char argument[256], value[256];
      argument[0] = '\0';

      strcat(argument, "-name ");
      uiTy.ID(tbAddActionNameID);
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      strcat(argument, " -type ");
      uiC.ID(tbAddActionTypeID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -front ");
      uiC.ID(tbAddActionFrontPoseID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -rear ");
      uiC.ID(tbAddActionEndPoseID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -length ");
      uiTy.ID(tbAddActionDurationID);
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      // execute command to create a new action
      TbCommandParser("NewAction", argument, FALSE);
   }
}


/*-----------------------------------------------
  create a new game FX in current game FX system
  C.Wang 0129, 2013
 ------------------------------------------------*/
void TbNewFXUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      tbAddGameFXUI.PopUp(FALSE);

      // extract the data from UIs
      FnUiCombo uiC;
      FnUiTypein uiTy;
      char argument[256], value[256];
      argument[0] = '\0';

      uiC.ID(tbAddFXTypeID);
      uiTy.ID(tbAddFXNameID);

      strcat(argument, "-name ");
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      strcat(argument, " -type ");
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -life ");
      uiTy.ID(tbFXLifeID);
      uiTy.GetTypeinValue(value);
      if (!TbCheckInt(value)) {
         sprintf(value, "%d", tbDefFXLife);
      }
      strcat(argument, value);

      strcat(argument, " -maxP ");
      uiTy.ID(tbMaxParticleNumID);
      uiTy.GetTypeinValue(value);
      if (!TbCheckInt(value)) {
         sprintf(value, "%d", tbCurMaxParticles);
      }
      else {
         int num = atoi(value);
         if (num > 0 && num < MAXIMUM_PARTICLES) {
            tbCurMaxParticles = num;
         }
         else {
            sprintf(value, "%d", tbCurMaxParticles);
            uiTy.PasteTypeinValue(value);
         }
      }
      sprintf(value, "%d", tbCurMaxParticles);
      strcat(argument, value);

      strcat(argument, " -Force ");
      uiC.ID(tbPickForceTypeID);
      uiC.GetComboValue(value);
      TbTruncateAllSpaces(value);
      strcat(argument, value);
      strcat(argument, " ");

      strcat(argument, " -ForceMagnitude ");
      uiTy.ID(tbPickForceMagnitudeID);
      uiTy.GetTypeinValue(value);
      strcat(argument, value);
      strcat(argument, " ");

      // execute command to create a new game FX
      TbCommandParser("NewGameFX", argument, FALSE);
   }
}


/*-------------------------------------------
  delete a game FX in current game FX system
  C.Wang 1008, 2012
 --------------------------------------------*/
void TbDeleteFXUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      // execute command to delete a game FX
      TbCommandParser("DeleteGameFX", NULL, FALSE);
   }
}


/*----------------------------
  load current game FX system
  C.Wang 1228, 2012
 -----------------------------*/
void TbLoadFXSystemUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      // execute command to load a game FX system
      TbCommandParser("LoadGameFXSystem", NULL, FALSE);
   }
}


/*------------------------------
  delete current game FX system
  C.Wang 1231, 2012
 -------------------------------*/
void TbDeleteFXSystemUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      // execute command to delete current game FX system
      TbCommandParser("DeleteGameFXSystem", NULL, FALSE);
   }
}


/*----------------------------
  save current game FX system
  C.Wang 1226, 2012
 -----------------------------*/
void TbSaveFXSystemUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      // execute command to save a game FX system
      TbCommandParser("SaveGameFXSystem", NULL, FALSE);
   }
}


/*----------------------------
  create a new game FX System
  C.Wang 1027, 2012
 -----------------------------*/
void TbNewFXSystemUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      tbAddGameFXSystemUI.PopUp(FALSE);

      // extract the data from UIs
      FnUiTypein uiTy;
      char argument[256], value[256];
      argument[0] = '\0';

      uiTy.ID(tbAddFXSystemNameID);
      strcat(argument, "-name ");
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      // execute command to create a new game FX system
      TbCommandParser("NewGameFXSystem", argument, FALSE);
   }
}


/*-------------------------------------------
  select a game FX in current game FX system
  C.Wang 1003, 2012
 --------------------------------------------*/
void TbMakeCurrentFXUICommand(UIid uiID, char *value)
{
   // execute command to select current FX
   TbCommandParser("SelectGameFX", value, FALSE);
}


/*------------------------
  select a game FX system
  C.Wang 1004, 2012
 -------------------------*/
void TbMakeCurrentFXSystemUICommand(UIid uiID, char *value)
{
   // execute command to select current FX system
   TbCommandParser("SelectGameFXSystem", value, FALSE);
}


/*---------------------
  select plate FX type
  C.Wang 1005, 2012
 ----------------------*/
void TbChangePlateTypeUICommand(UIid uiID, char *value)
{
   // execute command to select current FX system
   TbCommandParser("ChangePlateType", value, FALSE);
}


/*---------------------------
  set FX's parent (by combo)
  C.Wang 0102, 2013
 ----------------------------*/
void TbSetFXParentComboUICommand(UIid uiID, char *value)
{
   // execute command to set parent for current FX
   TbCommandParser("SetFXParent", value, FALSE);
}


/*--------------------------------------
  select plate FX texture blending type
  C.Wang 1008, 2012
 ---------------------------------------*/
void TbChangePlateBlendTypeUICommand(UIid uiID, char *value)
{
   // execute command to change texture blending type
   TbCommandParser("ChangePlateBlendType", value, FALSE);
}


/*-----------------------------------------
  select particle FX texture blending type
  C.Wang 1224, 2012
 ------------------------------------------*/
void TbChangeParticleBlendTypeUICommand(UIid uiID, char *value)
{
   // execute command to change texture blending type
   TbCommandParser("ChangeParticleBlendType", value, FALSE);
}


/*-----------------------
  Load plate FX alphamap
  C.Wang 0114, 2013
 ------------------------*/
void TbLoadPlateFXAlphamapUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   uiTy.ID(tbPlateFXAlphamapNameID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to load alphamap on a plate FX
   TbCommandParser("LoadPlateAlphamap", argument, FALSE);
}


/*--------------------------
  Load geometry FX alphamap
  C.Wang 0116, 2013
 ---------------------------*/
void TbLoadGeometryFXAlphamapUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   uiTy.ID(tbGeometryFXAlphamapNameID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to load alphamap on a plate FX
   TbCommandParser("LoadGeometryAlphamap", argument, FALSE);
}


/*-----------------------
  Load plate FX textures
  C.Wang 0115, 2013
 ------------------------*/
void TbLoadPlateFXTextureUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   uiTy.ID(tbPlateFXTextureNameID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   strcat(argument, " -number ");
   uiTy.ID(tbPlateFXTextureNumberID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to load textures on a plate FX
   TbCommandParser("LoadPlateTexture", argument, FALSE);

   if (uiID == tbPlateFXTextureNumberID) {
      TbLoadPlateFXAlphamapUICommand(tbPlateFXAlphamapNameID, NULL);
   }
}


/*-----------------------------
  load a model for geometry FX
  C.Wang 0113, 2013
 ------------------------------*/
void TbGameFXGeometryLoadBtnCallback(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // execute command to load a model for geometry FX
      TbCommandParser("LoadGeometryFXModel", NULL, FALSE);
   }
}


/*-----------------------------------------
  select geometry FX texture blending type
  C.Wang 0116, 2012
 ------------------------------------------*/
void TbChangeGeometryBlendTypeUICommand(UIid uiID, char *value)
{
   // execute command to change texture blending type
   TbCommandParser("ChangeGeometryBlendType", value, FALSE);
}


/*--------------------------
  Load geometry FX textures
  C.Wang 0116, 2013
 ---------------------------*/
void TbLoadGeometryFXTextureUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   uiTy.ID(tbGeometryFXTextureNameID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   strcat(argument, " -number ");
   uiTy.ID(tbGeometryFXTextureNumberID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to load textures on a geometry FX
   TbCommandParser("LoadGeometryTexture", argument, FALSE);

   if (uiID == tbGeometryFXTextureNumberID) {
      TbLoadGeometryFXAlphamapUICommand(tbGeometryFXAlphamapNameID, NULL);
   }
}


/*-------------------------
  Load particle FX texture
  C.Wang 1225, 2012
 --------------------------*/
void TbLoadParticleTextureUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   uiTy.ID(tbParticleTextureNameID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to load textures on a particle FX
   TbCommandParser("LoadParticleTexture", argument, FALSE);
}


/*-------------------------
  Load alphamap FX texture
  C.Wang 0115, 2015
 --------------------------*/
void TbLoadParticleAlphamapUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   uiTy.ID(tbParticleAlphamapNameID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to load alphamaps on a particle FX
   TbCommandParser("LoadParticleAlphamap", argument, FALSE);
}


/*--------------------------------
  modify base FX position command
  C.Wang 1212, 2012
 ---------------------------------*/
void TbModifyBasePositionUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbBaseFXFXPosID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbBaseFXFXPosID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbBaseFXFXPosID[2]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change the FX position
   TbCommandParser("ModifyBaseFXPosition", argument, FALSE);
}


/*-----------------------------
  modify particle life command
  C.Wang 1224, 2012
 ------------------------------*/
void TbModifyParticleLifeUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbParticleLifeID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleLifeID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change the particle life
   TbCommandParser("ModifyParticleLife", argument, FALSE);
}


/*------------------------------------------
  modify particle texture animation command
  C.Wang 0109, 2013
 -------------------------------------------*/
void TbModifyParticleAnimUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbParticleTextureAnimID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleTextureAnimID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleTextureAnimID[2]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   // execute command to change the particle texture animation
   TbCommandParser("ModifyParticleTextureAnimation", argument, FALSE);
}


/*-----------------------------
  modify particle size command
  C.Wang 1224, 2012
 ------------------------------*/
void TbModifyParticleSizeUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbParticleSizeID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleSizeID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change the particle size
   TbCommandParser("ModifyParticleSize", argument, FALSE);
}


/*---------------------------------
  modify particle velocity command
  C.Wang 1226, 2012
 ----------------------------------*/
void TbModifyParticleVelocityUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbParticleVelocityID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleVelocityID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleVelocityID[2]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleVelocityVarID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleVelocityVarID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbParticleVelocityVarID[2]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change the particle velocity data
   TbCommandParser("ModifyParticleVelocity", argument, FALSE);
}


/*----------------------------------------------
  modify particle born number per frame command
  C.Wang 1224, 2012
 -----------------------------------------------*/
void TbModifyParticleBornUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbParticleBornID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   // execute command to change the particle born
   TbCommandParser("ModifyParticleBorn", argument, FALSE);
}


/*---------------------------------
  callback for Key Frame Cancel
  Eric Fei 0108, 2013
 ----------------------------------*/
void TbGameFXKeyFrameCancelCallback(UIid uiID, BOOL4 bePress)
{
   FyDefineHotKey(FY_SHIFT, NULL);
   FyStandardPopupQuit(uiID, bePress);
}


/*---------------------------------
  open the load audio file browser
  Eric Fei 0102, 2013
 ----------------------------------*/
void TbGameFXAudioLoadBtnCallback(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // declare variables to open a file browser
      BOOL4 beOK = FALSE;
      char dir[128], fileName[256];
      OPENFILENAME ofn;

      // prepare variables to open the file browser
      GetCurrentDirectory(128, dir);
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize     = sizeof(OPENFILENAME);
      ofn.hwndOwner       = NULL;
      ofn.lpstrFilter     = "Fly WAV audio File {*.wav}\0*.wav\0"\
                            "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile       = fileName;
      ofn.nMaxFile        = 256;
      ofn.Flags           = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt     = "wav";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);
         FySetExtraWorkPath(path);

         // paste the data to audio UIs
         FnUiTypein uiTy;
         uiTy.ID(tbAudioFXNameID);
         if (strlen(file) != 0) {
            strcat(file, ".");
         }
         strcat(file, ext);
         uiTy.PasteTypeinValue(file);

         // generate load audio command
         TbGameFXAudioLoadToCommand(FAILED_ID, "");
      }
   }
}


/*-------------------------
  Load Audio of Audio FX
  Eric Fei 1228, 2012
 --------------------------*/
void TbGameFXAudioLoadToCommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   uiTy.ID(tbAudioFXNameID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to load audio on a audio FX
   TbCommandParser("LoadGameFXAudio", argument, FALSE);
}


/*-------------------------------
  Change Start Frame of Audio FX
  Eric Fei 0102, 2013
 --------------------------------*/
void TbGameFXAudioChangeStartCallback(UIid uiID, char *value)
{
   // generate change start frame command
   TbGameFXAudioChangeStartToCommand(FAILED_ID, "");
}


/*-------------------------------
  Change Start Frame of Audio FX
  Eric Fei 0102, 2013
 --------------------------------*/
void TbGameFXAudioChangeStartToCommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-s ");
   uiTy.ID(tbAudioFXStartID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change start frame on a audio FX
   TbCommandParser("ChangeGameFXAudioStartTime", argument, FALSE);
}


/*-----------------------------
  Select Character of Sword FX
  Eric Fei 0109, 2013
 ------------------------------*/
void TbGameFXSwordSelectCharComboCallback(UIid uiID, char *value)
{
   // generate change character command
   TbGameFXSwordSelectCharToCommand(FAILED_ID, "");
}


/*-----------------------------
  Select Character of Sword FX
  Eric Fei 0109, 2013
 ------------------------------*/
void TbGameFXSwordSelectCharToCommand(UIid uiID, char *value)
{
   // create the argument
   FnUiCombo uiC;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-char ");
   uiC.ID(tbSwordFXCharacterComboID);
   uiC.GetComboValue(v);
   strcat(argument, v);

   // execute command to change character on a sword FX
   TbCommandParser("ChangeGameFXSwordCharacter", argument, FALSE);
}


/*---------------------------
  Select Dummy 1 of Sword FX
  Eric Fei 0109, 2013
 ----------------------------*/
void TbGameFXSwordSelectDummy1ComboCallback(UIid uiID, char *value)
{
   // generate change dummy 1 command
   TbGameFXSwordSelectDummy1ToCommand(FAILED_ID, "");
}


/*---------------------------
  Select Dummy 1 of Sword FX
  Eric Fei 0109, 2013
 ----------------------------*/
void TbGameFXSwordSelectDummy1ToCommand(UIid uiID, char *value)
{
   // create the argument
   FnUiCombo uiC;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-dmy1 ");
   uiC.ID(tbSwordFXDummy1ComboID);
   uiC.GetComboValue(v);
   strcat(argument, v);

   // execute command to change character on a sword FX
   TbCommandParser("ChangeGameFXSwordDummy", argument, FALSE);
}


/*-----------------------------------
  search object & update Dummy 1 UIs
  Eric Fei 0117, 2013
 ------------------------------------*/
void TbGameFXSwordSelectDummy1SearchCallback(UIid uiID, char *value)
{
   // popup the message box
   FnUiCombo uiC(tbSwordFXDummy1ComboID);
   uiC.PopupMessageBox(TRUE);

   TbGameFXSwordSelectDummySearchAlg(tbSwordFXDummy1ComboID, value);
}


/*---------------------------
  Select Dummy 2 of Sword FX
  Eric Fei 0109, 2013
 ----------------------------*/
void TbGameFXSwordSelectDummy2ComboCallback(UIid uiID, char *value)
{
   // generate change dummy 2 command
   TbGameFXSwordSelectDummy2ToCommand(FAILED_ID, "");
}


/*---------------------------
  Select Dummy 2 of Sword FX
  Eric Fei 0109, 2013
 ----------------------------*/
void TbGameFXSwordSelectDummy2ToCommand(UIid uiID, char *value)
{
   // create the argument
   FnUiCombo uiC;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-dmy2 ");
   uiC.ID(tbSwordFXDummy2ComboID);
   uiC.GetComboValue(v);
   strcat(argument, v);

   // execute command to change character on a sword FX
   TbCommandParser("ChangeGameFXSwordDummy", argument, FALSE);
}


/*-----------------------------------
  search object & update Dummy 2 UIs
  Eric Fei 0117, 2013
 ------------------------------------*/
void TbGameFXSwordSelectDummy2SearchCallback(UIid uiID, char *value)
{
   // popup the message box
   FnUiCombo uiC(tbSwordFXDummy2ComboID);
   uiC.PopupMessageBox(TRUE);

   TbGameFXSwordSelectDummySearchAlg(tbSwordFXDummy2ComboID, value);
}


/*----------------------------
  Dummy Name Search Algorithm
  Eric Fei 0117, 2013
 -----------------------------*/
void TbGameFXSwordSelectDummySearchAlg(UIid uiID, char *value)
{
   int i, len;
   char string[256];
   char *name, nameChar[256];
   FnUiCombo uiC(uiID);
   FnUiCombo uiCC(tbSwordFXCharacterComboID);   // Character Combo UI
   uiCC.GetComboValue(nameChar);

   int count;
   CHARACTERid charID;
   FnObject model;

   int numO = tbCurScene->GetObjectNumber();

   strcpy(string, value);
   FyTruncateStringSpace(string);
   len = (int) strlen(string);

   if (len == 0) {
      uiC.ClearMessages();
      
      // check Character exist
      BOOL4 bHasChar = TRUE;
      if (strlen(nameChar) == 0 ||                                            // Character Name = ""
          FyCheckWord(nameChar, "-") ||                                       // Character Name = "-"
          (charID = tbCurScene->GetCharacterByName(nameChar)) == FAILED_ID) { // Character No Loading
         bHasChar = FALSE;
      }

      // insert name
      if (bHasChar) {
         // insert all match attachments of character
         FnCharacter actor(charID);
         numO  = actor.GetAttachmentNumber();
         count = 0;
         for (i = 0; i < numO; i++) {
            model.ID(actor.GetAttachment(i));
            if (model.ID() != FAILED_ID) {
               uiC.InsertMessage(count, model.GetName());
               count++;
            }
         }
      }
      else {
         // insert all models
         count = 0;
         for (i = 0; i < numO; i++) {
            model.ID(tbCurScene->GetObjectID(i));
            if (model.ID() != FAILED_ID) {
               uiC.InsertMessage(count, model.GetName());
               count++;
            }
         }
      }
   }
   else {
      uiC.ClearMessages();

      // check Character exist
      BOOL4 bHasChar = TRUE;
      if (strlen(nameChar) == 0 ||                                            // Character Name = ""
          FyCheckWord(nameChar, "-") ||                                       // Character Name = "-"
          (charID = tbCurScene->GetCharacterByName(nameChar)) == FAILED_ID) { // Character No Loading
         bHasChar = FALSE;
      }

      // insert name
      if(bHasChar)
      {
         // insert all match attachments of character
         FnCharacter actor(charID);
         numO  = actor.GetAttachmentNumber();
         count = 0;
         for (i = 0; i < numO; i++) {
            model.ID(actor.GetAttachment(i));
            if (model.ID() != FAILED_ID) {
               name = model.GetName();
               if (FyCheckWord(name, string, TRUE, len)) {
                  uiC.InsertMessage(count, name);
                  count++;
               }
            }
         }
      }
      else {
         // insert all match models
         count = 0;
         for (i = 0; i < numO; i++) {
            model.ID(tbCurScene->GetObjectID(i));
            if (model.ID() != FAILED_ID) {
               name = model.GetName();
               if (FyCheckWord(name, string, TRUE, len)) {
                  uiC.InsertMessage(count, name);
                  count++;
               }
            }
         }
      }
   }
}


/*----------------------------------------------------------
  (Radio) Button UI Callback for Setting Param. of Sword FX
  Eric Fei 0119, 2013
 -----------------------------------------------------------*/
void TbGameFXSwordChangeParamButtonCallback(UIid uiID, BOOL4 bePress)
{
   TbGameFXSwordChangeParamTypeinToCommand(uiID, "");
}


/*-------------------------------------------
  UI Callback for Setting Param. of Sword FX
  Eric Fei 0114, 2013
 --------------------------------------------*/
void TbGameFXSwordChangeParamTypeinCallback(UIid uiID, char *value)
{
   // generate change dummy 2 command
   TbGameFXSwordChangeParamTypeinToCommand(uiID, "");
}


/*-------------------------------------------
  UI Callback for Setting Param. of Sword FX
  Eric Fei 0122, 2013
 --------------------------------------------*/
void TbGameFXSwordChangeParamTypeinToCommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein      uiTy;
   FnUiRadioButton uiRB;
   FnUiCombo       uiC;
   char argument[256], v[256];
   argument[0] = '\0';

   uiRB.ID(tbSwordFXEnableRButtonID);
   if (uiRB.GetState()) {
      strcat(argument, " -enable ");
   }
   else {
      strcat(argument, " -disable ");
   }

   strcat(argument, " -tlen ");
   uiTy.ID(tbSwordFXTailLengthTypeinID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   strcat(argument, " -tdiv ");
   uiTy.ID(tbSwordFXTailDivisionTypeinID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   strcat(argument, " -tseg ");
   uiTy.ID(tbSwordFXTailSegmentTypeinID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   strcat(argument, " -blend ");
   uiC.ID(tbSwordFXBlendModeComboID);
   uiC.GetComboValue(v);
   strcat(argument, v);
   
   strcat(argument, " -color ");
   uiTy.ID(tbSwordFXColorTypeinID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");
   uiTy.ID(tbSwordFXColorTypeinID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");
   uiTy.ID(tbSwordFXColorTypeinID[2]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");
   uiTy.ID(tbSwordFXColorTypeinID[3]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change character on a sword FX
   TbCommandParser("ChangeGameFXSwordParam", argument, FALSE);
}


/*--------------------
  set texture file
  Eric Fei 0117, 2013
 ---------------------*/
void TbGameFXSwordTextureTypeinCallback(UIid uiID, char *value)
{
   // generate load texture command
   TbGameFXSwordChangeTextureToCommand(FAILED_ID, "");
}


/*--------------------
  set alpha map file
  Eric Fei 0117, 2013
 ---------------------*/
void TbGameFXSwordAlphamapTypeinCallback(UIid uiID, char *value)
{
   FnUiTypein uiTy;
   uiTy.ID(tbSwordFXAlphaMapNameTypeinID);
   uiTy.PasteTypeinValue(value);

   // generate load texture command
   TbGameFXSwordChangeAlphamapToCommand(FAILED_ID, "");
}


/*---------------------------------------------
  open the load texture/alpha map file browser
  Eric Fei 0117, 2013
 ----------------------------------------------*/
void TbGameFXSwordAddBtnCallback(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // declare variables to open a file browser
      BOOL4 beOK = FALSE;
      char dir[128], fileName[256];
      OPENFILENAME ofn;

      // prepare variables to open the file browser
      GetCurrentDirectory(128, dir);
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize     = sizeof(OPENFILENAME);
      ofn.hwndOwner       = NULL;
      ofn.lpstrFilter     = "JPEG texture File {*.jpg}\0*.jpg\0"\
                            "DDS texture File {*.dds}\0*.dds\0"\
                            "PNG texture File {*.png}\0*.png\0"\
                            "MS Bitmap texture File {*.bmp}\0*.bmp\0"\
                            "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile       = fileName;
      ofn.nMaxFile        = 256;
      ofn.Flags           = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt     = "jpg";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);
         FySetExtraWorkPath(path);

         // paste the data to sword texture UIs
         FnUiTypein uiTy;
         if (uiID == tbSwordFXTextureAddButtonID) {
            // update UI content
            uiTy.ID(tbSwordFXTextureNameTypeinID);
            uiTy.PasteTypeinValue(file);

            // generate load texture command
            TbGameFXSwordChangeTextureToCommand(FAILED_ID, "");
         }
         else if (uiID == tbSwordFXAlphaMapAddButtonID) {
            // update UI content
            uiTy.ID(tbSwordFXAlphaMapNameTypeinID);
            uiTy.PasteTypeinValue(file);

            // generate load texture command
            TbGameFXSwordChangeAlphamapToCommand(FAILED_ID, "");
         }
      }
   }
}


/*--------------------------------------
  remove texture/alpha map file browser
  Eric Fei 0118, 2013
 ---------------------------------------*/
void TbGameFXSwordRemoveBtnCallback(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // remove the data frome sword texture/alpha map UIs
      FnUiTypein uiTy;

      // remove texture
      uiTy.ID(tbSwordFXTextureNameTypeinID);
      uiTy.PasteTypeinValue("");
      TbGameFXSwordChangeTextureToCommand(FAILED_ID, "");

      // remove alphamap
      uiTy.ID(tbSwordFXAlphaMapNameTypeinID);
      uiTy.PasteTypeinValue("");
      TbGameFXSwordChangeAlphamapToCommand(FAILED_ID, "");
   }
}


/*---------------------------
  Change Texture of Sword FX
  Eric Fei 0117, 2013
 ----------------------------*/
void TbGameFXSwordChangeTextureToCommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256], v2[256];
   argument[0] = '\0';

   uiTy.ID(tbSwordFXTextureNameTypeinID);
   uiTy.GetTypeinValue(v);
   uiTy.ID(tbSwordFXTextureNumTypeinID);
   uiTy.GetTypeinValue(v2);

   if (strlen(v) == 0) {
      strcat(argument, "-rmtex ");
   }
   else {
      strcat(argument, "-tex ");
      strcat(argument, v);
      strcat(argument, " ");
      strcat(argument, v2);
   }

   // execute command to load audio on a audio FX
   TbCommandParser("ChangeGameFXSwordTexture", argument, FALSE);
}


/*-----------------------------
  Change Alpha Map of Sword FX
  Eric Fei 0117, 2013
 ------------------------------*/
void TbGameFXSwordChangeAlphamapToCommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';
   
   uiTy.ID(tbSwordFXAlphaMapNameTypeinID);
   uiTy.GetTypeinValue(v);

   if (strlen(v) == 0) {
      strcat(argument, "-rmalpha ");
   }
   else {
      strcat(argument, "-alpha ");
      strcat(argument, v);
   }

   // execute command to load audio on a audio FX
   TbCommandParser("ChangeGameFXSwordAlphamap", argument, FALSE);
}


/*---------------------------
  Update all UIs of Sword FX
  Eric Fei 0113, 2013
 ----------------------------*/
void TbUpdateSwordFXUIs(GAMEFXid gxID)
{
   // get Sword FX
   FnBaseGameFX  gx(gxID);
   FnSwordGameFX sword(gxID);

   if ((gxID == FAILED_ID) || (gx.GetFXType() != SWORD_FX)) {
      // get current FX
      TBGameFXSystem *curFX;
      curFX = tbCurScene->GetCurrentTBGameFXSystem();
      if (curFX == NULL) return;

      gxID = curFX->GetCurrentGameFX();
      if (gxID == FAILED_ID) return;

      gx.ID(gxID);
      if (gx.GetFXType() != SWORD_FX) return;

      sword.ID(gxID);
   }

   // update UIs
   FnUiCombo uiC1, uiC2;
   FnUiTypein uiTy;
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   char value[256];
   value[0] = '\0';
   uiRB.ID(tbSwordFXEnableRButtonID);
   uiRB.Enable(TRUE);
   if (sword.GetState()) {
      uiRB.ReCaption("Enable");
      uiRB.ChangeState(TRUE);
   }
   else {
      uiRB.ReCaption("Disable");
      uiRB.ChangeState(FALSE);
   }

   //--- update character name UI
   uiC1.ID(tbSwordFXCharacterComboID);
   uiC1.ClearMessages();
   int numA = tbCurScene->GetEffectCharacterNumber();
   if (numA > 0) {
      char *name;
      int   iCount = 0;
      numA = tbCurScene->GetCharacterNumber();
      for (int i = 0; i < numA; i++) {
         name = tbCurScene->GetCharacterName(i);
         if (name != NULL) {
            uiC1.InsertMessage(iCount, name);
            iCount++;
         }
      }
   }
   
   uiC1.Enable(TRUE);
   sword.GetCharacterName(value);
   if (strlen(value) == 0) {
      uiC1.PasteComboValue("-");
   }
   else {
      uiC1.PasteComboValue(value);
   }

   //--- update dummy UIs
   uiC1.ID(tbSwordFXDummy1ComboID);
   uiC2.ID(tbSwordFXDummy2ComboID);
   CHARACTERid charID(tbCurScene->GetCharacterByName(value));
   if (charID == FAILED_ID) {
      // check parent's name
      uiC1.ClearMessages();
      uiC2.ClearMessages();
   
      // insert all models
      OBJECTid oID;
      FnObject model;
      char * name;
      int numO = tbCurScene->GetObjectNumber();
      int count = 0;
      for (int i = 0; i < numO; i++) {
         oID = tbCurScene->GetObjectID(i);
         if (oID != FAILED_ID) {
            model.ID(oID);
            name = model.GetName();
            uiC1.InsertMessage(count, name);
            uiC2.InsertMessage(count, name);
            count++;
         }
      }
      
      // paste dummy names
      char dummy1[256], dummy2[256];
      dummy1[0] = dummy2[0] = '\0';

      sword.GetDummyObject1Name(dummy1);
      if (strlen(dummy1) == 0) {
         uiC1.PasteComboValue("-");
      }
      else {
         uiC1.PasteComboValue(dummy1);
      }

      sword.GetDummyObject2Name(dummy2);
      if (strlen(dummy2) == 0) {
         uiC2.PasteComboValue("-");
      }
      else {
         uiC2.PasteComboValue(dummy2);
      }
   }
   else {
      FnCharacter actor(charID);
      FnObject obj;
      int numO = actor.GetAttachmentNumber();
      int iCount = 0;
      
      uiC1.ClearMessages();
      uiC2.ClearMessages();
      for (int i = 0; i < numO; i++) {
         obj.ID(actor.GetAttachment(i));
         if (obj.ID() != FAILED_ID) {
            uiC1.InsertMessage(iCount, obj.GetName());
            uiC2.InsertMessage(iCount, obj.GetName());
            iCount++;
         }
      }

      // paste dummy names
      char dummy1[256], dummy2[256];
      dummy1[0] = dummy2[0] = '\0';

      sword.GetDummyObject1Name(dummy1);
      if (strlen(dummy1) == 0) {
         uiC1.PasteComboValue("-");
      }
      else {
         uiC1.PasteComboValue(dummy1);
      }

      sword.GetDummyObject2Name(dummy2);
      if (strlen(dummy2) == 0) {
         uiC2.PasteComboValue("-");
      }
      else {
         uiC2.PasteComboValue(dummy2);
      }
   }
   uiC1.Enable(TRUE);
   uiC2.Enable(TRUE);

   //--- update Tail Length UI
   int tailLength = sword.GetBladeMaxNum();
   uiTy.ID(tbSwordFXTailLengthTypeinID);
   sprintf(value, "%d", tailLength);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);

   //--- update Tail Density UI
   int tailDivision = sword.GetDivideNum();
   uiTy.ID(tbSwordFXTailDivisionTypeinID);
   sprintf(value, "%d", tailDivision);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);

   //--- update Tail Segment UI
   int tailSegment = sword.GetSegmentNum();
   uiTy.ID(tbSwordFXTailSegmentTypeinID);
   sprintf(value, "%d", tailSegment);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);

   //--- update Texture UI
   int num;
   sword.GetTextureInfo(value, &num);
   uiTy.ID(tbSwordFXTextureNameTypeinID);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);
   uiTy.ID(tbSwordFXTextureNumTypeinID);
   sprintf(value, "%d", num);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);
   uiB.ID(tbSwordFXTextureAddButtonID);
   uiB.Enable(TRUE);
   uiB.ID(tbSwordFXTextureRemoveButtonID);
   uiB.Enable(TRUE);

   //--- update Alpha Map UI
   uiTy.ID(tbSwordFXAlphaMapNameTypeinID);
   sword.GetAlphamapInfo(value);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);
   uiB.ID(tbSwordFXAlphaMapAddButtonID);
   uiB.Enable(TRUE);
   uiB.ID(tbSwordFXAlphaMapRemoveButtonID);
   uiB.Enable(TRUE);

   uiC1.ID(tbSwordFXBlendModeComboID);
   TbGetBlendString(value, sword.GetBlendingType());
   uiC1.PasteComboValue(value);
   uiC1.Enable(TRUE);

   //--- update Color UI
   float fColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
   sword.GetColor(fColor);
   uiTy.ID(tbSwordFXColorTypeinID[0]);
   sprintf(value, "%5.2f", fColor[0]);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);
   uiTy.ID(tbSwordFXColorTypeinID[1]);
   sprintf(value, "%5.2f", fColor[1]);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);
   uiTy.ID(tbSwordFXColorTypeinID[2]);
   sprintf(value, "%5.2f", fColor[2]);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);
   uiTy.ID(tbSwordFXColorTypeinID[3]);
   sprintf(value, "%5.2f", fColor[3]);
   uiTy.PasteTypeinValue(value);
   uiTy.Enable(TRUE);
}


/*------------------------------
  get blend type ID from string
  Eric Fei 0118, 2013
 -------------------------------*/
DWORD TbGetBlendNum(char * value)
{
   // [In] value: blend type string
   // RETURN:     blend type ID

   if (FyCheckWord(value, "Alpha")) {
      return ALPHA_BLENDING;
   }
   else if (FyCheckWord(value, "Subtract Color") || FyCheckWord(value, "Subtract")) {
      return SUBTRACT_COLOR;
   }
   else if (FyCheckWord(value, "Add Color") || FyCheckWord(value, "Add")) {
      return ADD_COLOR;
   }
   else {
      return 0;
   }
}


/*------------------------------
  get blend string from type ID
  Eric Fei 0118, 2013
 -------------------------------*/
void TbGetBlendString(char * value, DWORD num)
{
   // [Out] value: blend type string
   // [In]  num:   blend type ID

   if (num == ALPHA_BLENDING) {
      strcpy(value, "Alpha");
   }
   else if (num == SUBTRACT_COLOR) {
      strcpy(value, "Subtract Color");
   }
   else if (num == ADD_COLOR) {
      strcpy(value, "Add Color");
   }
   else {
      strcpy(value, "");
   }
}


/*-----------------------------------------------------
  adjust texture file name according to Texture number
  Eric Fei 0123, 2013
 ------------------------------------------------------*/
BOOL4 TbAdjustTextureFileNameFromUI(char * fileName, char ** fileTail, int nTex)
{
   // [in/out] fileName: file name start address (exclude path)
   // [out]    fileTail: file name tail address
   // [in]     nTex:     texture number
   // RETURN:  TRUE  => need to add number
   //          FALSE => no need to add number

   if (fileName == NULL) return FALSE;           // no file name
   if (nTex < 1) return FALSE;                   // only one picture & don't use number (ex: box)

   int len = (int) strlen(fileName);
   if (len >= 256)  return FALSE;                // no '\0'

   char *pCheckName = fileName + len - 4;
   if (len > 4 && TbCheckInt(pCheckName)) {      // tail has number (ex: FANS0000)
      // legal number
      pCheckName[0] = '\0';
      if (fileTail != NULL) *fileTail = pCheckName;
      return TRUE;
   }
   else {                                             // tail no number (ex: FANS)
      if (fileTail != NULL) *fileTail = fileName + len;
      return TRUE;
   }
}


/*--------------------------------------------------
  adjust texture file name according texture number
  Eric Fei 0124, 2013
 ---------------------------------------------------*/
void TbAdjustTextureFileNameFromMat(char * fileName, int nTex)
{
   // [in/out] fileName: file name start address (exclude path)
   // [out]    nTex:     texture number (use to adjust)

   if (fileName == NULL) return;               // no file name

   int len = (int) strlen(fileName);
   char *pCheckName = fileName + len - 4;
   if (len > 4 && TbCheckInt(pCheckName)) {    // tail has number (ex: FANS0000, FANS0001,...)
      if (nTex > 1) pCheckName[0] = '\0';
   }
}


/*--------------------------------
  modify base FX rotation command
  C.Wang 1220, 2012
 ---------------------------------*/
void TbModifyBaseRotationUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbBaseFXFXRotateID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbBaseFXFXRotateID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbBaseFXFXRotateID[2]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change the FX rotation
   TbCommandParser("ModifyBaseFXRotation", argument, FALSE);
}


/*----------------------------------
  modify base FX visibility command
  C.Wang 1220, 2012
 -----------------------------------*/
void TbModifyBaseVisibilityUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbBaseFXShowID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change the FX visibility
   TbCommandParser("ModifyBaseFXVisibility", argument, FALSE);
}


/*----------------------------
  modify base FX life command
  C.Wang 1220, 2012
 -----------------------------*/
void TbModifyBaseLifeUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   FnUiRadioButton uiRB;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbBaseLifeID);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");
   uiRB.ID(tbBaseLifeAutoAdjustID);
   if (uiRB.GetState()) {
      strcat(argument, "Auto");
   }

   // execute command to change the FX life
   TbCommandParser("ModifyBaseFXLife", argument, FALSE);
}


/*--------------------------
  modify plate size command
  C.Wang 1008, 2012
 ---------------------------*/
void TbModifyPlateSizeUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   strcat(argument, "-width ");
   uiTy.ID(tbPlateFXSizeID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   strcat(argument, " -height ");
   uiTy.ID(tbPlateFXSizeID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change the plate size
   TbCommandParser("ModifyPlateSize", argument, FALSE);
}


/*---------------------------
  modify plate color command
  C.Wang 1008, 2012
 ----------------------------*/
void TbModifyPlateColorUICommand(UIid uiID, char *value)
{
   // create the argument
   FnUiTypein uiTy;
   char argument[256], v[256];
   argument[0] = '\0';

   uiTy.ID(tbPlateFXColorID[0]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbPlateFXColorID[1]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbPlateFXColorID[2]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);
   strcat(argument, " ");

   uiTy.ID(tbPlateFXColorID[3]);
   uiTy.GetTypeinValue(v);
   strcat(argument, v);

   // execute command to change the plate size
   TbCommandParser("ModifyPlateColor", argument, FALSE);
}


/*--------------------------------
  remove/delete plate FX textures
  C.Wang 1008, 2012
 ---------------------------------*/
void TbRemovePlateFXTextureUICommand(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // execute command to remove and delete textures on a plate FX
      TbCommandParser("RemovePlateTexture", NULL, FALSE);
   }
}


/*-----------------------------------
  remove/delete geometry FX textures
  C.Wang 0116, 2013
 ------------------------------------*/
void TbRemoveGeometryFXTextureUICommand(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // execute command to remove and delete textures on a geometry FX
      TbCommandParser("RemoveGeometryTexture", NULL, FALSE);
   }
}


/*--------------------------------
  remove/delete particle textures
  C.Wang 1225, 2012
 ---------------------------------*/
void TbRemoveParticleTextureUICommand(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // execute command to remove and delete textures on a particle FX
      TbCommandParser("RemoveParticleTexture", NULL, FALSE);
   }
}


/*----------------------
  add plate FX textures
  C.Wang 0114, 2013
 -----------------------*/
void TbAddPlateFXTexture(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // open a file browser
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL4 beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "JPEG texture File {*.jpg}\0*.jpg\0"\
                        "DDS texture File {*.dds}\0*.dds\0"\
                        "PNG texture File {*.png}\0*.png\0"\
                        "MS Bitmap texture File {*.bmp}\0*.bmp\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "jpg";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         FySetExtraWorkPath(path);

         // paste the data to plate texture UIs
         FnUiTypein uiTy;
         uiTy.ID(tbPlateFXTextureNumberID);
         uiTy.PasteTypeinValue("0");

         uiTy.ID(tbPlateFXTextureNameID);
         uiTy.PasteTypeinValue(file);

         // execute load texture command
         TbLoadPlateFXTextureUICommand(tbPlateFXTextureNameID, "");
      }
   }
}


/*----------------------
  add plate FX alphamap
  C.Wang 0114, 2013
 -----------------------*/
void TbAddPlateFXAlphamap(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // open a file browser
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL4 beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "JPEG texture File {*.jpg}\0*.jpg\0"\
                        "DDS texture File {*.dds}\0*.dds\0"\
                        "PNG texture File {*.png}\0*.png\0"\
                        "MS Bitmap texture File {*.bmp}\0*.bmp\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "jpg";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         FySetExtraWorkPath(path);

         // paste the data to plate texture UIs
         FnUiTypein uiTy;
         uiTy.ID(tbPlateFXAlphamapNameID);
         uiTy.PasteTypeinValue(file);

         // execute load texture command
         TbLoadPlateFXAlphamapUICommand(tbPlateFXAlphamapNameID, "");
      }
   }
}


/*-------------------------
  add particle FX textures
  C.Wang 1225, 2012
 --------------------------*/
void TbAddParticleTexture(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // open a file browser
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL4 beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "Fly JPEG texture File {*.jpg}\0*.jpg\0"\
                        "Fly DDS texture File {*.dds}\0*.dds\0"\
                        "Fly PNG texture File {*.png}\0*.png\0"\
                        "Fly bitmap texture File {*.bmp}\0*.bmp\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "jpg";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         FySetExtraWorkPath(path);

         // paste the data to plate texture UIs
         FnUiTypein uiTy;
         uiTy.ID(tbParticleTextureNameID);
         uiTy.PasteTypeinValue(file);

         // execute load texture command
         TbLoadParticleTextureUICommand(tbParticleTextureNameID, "");
      }
   }
}


/*--------------------------
  add particle FX alphamaps
  C.Wang 0115, 2013
 ---------------------------*/
void TbAddParticleAlphamap(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // open a file browser
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL4 beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "Fly JPEG texture File {*.jpg}\0*.jpg\0"\
                        "Fly DDS texture File {*.dds}\0*.dds\0"\
                        "Fly PNG texture File {*.png}\0*.png\0"\
                        "Fly bitmap texture File {*.bmp}\0*.bmp\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "jpg";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         FySetExtraWorkPath(path);

         // paste the data to plate texture UIs
         FnUiTypein uiTy;
         uiTy.ID(tbParticleAlphamapNameID);
         uiTy.PasteTypeinValue(file);

         // execute load texture command
         TbLoadParticleAlphamapUICommand(tbParticleAlphamapNameID, "");
      }
   }
}


/*-------------------------
  add geometry FX textures
  C.Wang 0116, 2013
 --------------------------*/
void TbAddGeometryFXTexture(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // open a file browser
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL4 beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "JPEG texture File {*.jpg}\0*.jpg\0"\
                        "DDS texture File {*.dds}\0*.dds\0"\
                        "Fly PNG texture File {*.png}\0*.png\0"\
                        "MS Bitmap texture File {*.bmp}\0*.bmp\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "jpg";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         FySetExtraWorkPath(path);

         // paste the data to geometry texture UIs
         FnUiTypein uiTy;
         uiTy.ID(tbGeometryFXTextureNumberID);
         uiTy.PasteTypeinValue("0");

         uiTy.ID(tbGeometryFXTextureNameID);
         uiTy.PasteTypeinValue(file);

         // execute load texture command
         TbLoadGeometryFXTextureUICommand(tbGeometryFXTextureNameID, "");
      }
   }
}


/*-------------------------
  add geometry FX alphamap
  C.Wang 0116, 2013
 --------------------------*/
void TbAddGeometryFXAlphamap(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // open a file browser
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL4 beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "JPEG texture File {*.jpg}\0*.jpg\0"\
                        "DDS texture File {*.dds}\0*.dds\0"\
                        "Fly PNG texture File {*.png}\0*.png\0"\
                        "MS Bitmap texture File {*.bmp}\0*.bmp\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "jpg";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         FySetExtraWorkPath(path);

         // paste the data to geometry texture UIs
         FnUiTypein uiTy;
         uiTy.ID(tbGeometryFXAlphamapNameID);
         uiTy.PasteTypeinValue(file);

         // execute load texture command
         TbLoadGeometryFXAlphamapUICommand(tbGeometryFXAlphamapNameID, "");
      }
   }
}


/*------------------------------
  rename/select current game FX
  C.Wang 1003, 2012
 -------------------------------*/
void TbRenameCurrentFXUICommand(UIid uiID, char *value)
{
   // check if the value is valid or not
   TBGameFXSystem *curFX = tbCurScene->GetCurrentTBGameFXSystem();
   if (curFX == NULL) return;
   
   GAMEFXid gxID2, gxID = curFX->GetCurrentGameFX();
   if (gxID == FAILED_ID) {
      // check if the user wants to select a FX for editing
      gxID2 = curFX->GetGameFXByName(value);
      if (gxID2 != FAILED_ID) {
         TbCommandParser("SelectGameFX", value, FALSE);
         return;
      }
   }
   else {
      // check if the user wants to select a FX for editing or rename current FX
      gxID2 = curFX->GetGameFXByName(value);
      if (gxID2 != FAILED_ID) {
         // execute command to select current FX
         TbCommandParser("SelectGameFX", value, FALSE);
      }
      else {
         // execute command to rename current FX
         TbCommandParser("RenameGameFX", value, FALSE);
      }
      return;
   }

   // update the UIs
   TbUpdateCurrentFXUIs();
}


/*-------------------------------------
  rename/select current game FX system
  C.Wang 1003, 2012
 --------------------------------------*/
void TbRenameCurrentFXSystemUICommand(UIid uiID, char *value)
{
   // check if the value is valid or not
   TBGameFXSystem *fxs, *curFX = tbCurScene->GetCurrentTBGameFXSystem();
   
   if (curFX == NULL) {
      // check if the user wants to select a FX system for editing
      fxs = tbCurScene->GetTBGameFXSystem(value);
      if (fxs != NULL) {
         TbCommandParser("SelectGameFXSystem", value, FALSE);
         return;
      }
   }
   else {
      // check if the user wants to select a FX system for editing or rename current FX system
      fxs = tbCurScene->GetTBGameFXSystem(value);
      if (fxs != FAILED_ID) {
         // execute command to select current FX system
         TbCommandParser("SelectGameFXSystem", value, FALSE);
      }
      else {
         // execute command to rename current FX system
         TbCommandParser("RenameGameFXSystem", value, FALSE);
      }
      return;
   }

   // update the UIs
   TbUpdateCurrentFXUIs();
}


/*-------------------------------------------------------
  change the particle system that uses the force command
  C.Wang 0101, 2013
 --------------------------------------------------------*/
void TbChangeForceParticleUICommand(UIid uiID, char *value)
{
   // execute command to change the particle system that uses this force FX
   TbCommandParser("ChangeForceParticle", value, FALSE);
}


/*-----------------------------------
  modify the force parameter command
  Eric Fei 0129, 2013
 ------------------------------------*/
void TbModifyForceParamUICommand(UIid uiID, char *value)
{
   // execute command to change the particle system that uses this force FX
   char argument[256], type[256];
   argument[0] = '\0';

   if (uiID == tbForceFXTypeID) {
      strcpy(argument, "-type ");
      memcpy(type, value, 256); 
      TbTruncateAllSpaces(type);
      strcat(argument, type);
   }
   else if (uiID == tbForceMagnitudeID) {
      strcpy(argument, "-mag ");
      strcat(argument, value);
   }
   else if (uiID == tbForceMinDistID) {
      strcpy(argument, "-dist ");
      strcat(argument, value);
   }
   else {
      return;
   }

   TbCommandParser("ModifyForceParam", argument, FALSE);
}


/*---------------------
  delete current model
  C.Wang 0813, 2012
 ----------------------*/
void TbDeleteModelUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("DeleteModel", NULL, FALSE);
   }
}


/*------------------------------------
  make previous object as current one
  C.Wang 0813, 2012
 -------------------------------------*/
void TbPreviousModelUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("PreviousModel", NULL, FALSE);
   }
}


/*--------------------------------
  make next object as current one
  C.Wang 0813, 2012
 ---------------------------------*/
void TbNextModelUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("NextModel", NULL, FALSE);
   }
}


/*------------------------
  update Gizmo transform
  Eric Fei 1218, 2012
 -------------------------*/
void TbUpdateTransformCommand(UIid uiID, char *value)
{
   // get string on typein
   char arg[256];
   TbGenTransformArgu(arg, sizeof(arg));
   TbCommandParser("SetTransform", arg, FALSE);
}


/*------------------------
  rename current material
  C.Wang 0813, 2012
 -------------------------*/
void TbRenameMaterialUICommand(UIid uiID, char *value)
{
   // get string on typein
   char arg[256];
   sprintf(arg, "-Material %s", value);
   TbCommandParser("Rename", arg, FALSE);
}


/*---------------------
  set current material
  C.Wang 0813, 2012
 ----------------------*/
void TbMakeCurrentMaterialUICommand(UIid uiID, char *value)
{
   // get string on typein
   TbCommandParser("SetMaterial", value, FALSE);
}


/*------------------------
  update current material
  C.Wang 0813, 2012
 -------------------------*/
void TbUpdateMaterialUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      TbCommandParser("UpdateMaterial", NULL, FALSE);
   }
}



/*-----------------------------------------
  set the rendering mode for current model
  C.Wang 0813, 2012
 ------------------------------------------*/
void TbWireframeModelUICommand(UIid uiID, BOOL4 bePress)
{
   FnUiRadioButton uiRB;
   uiRB.ID(uiID);
   if (bePress) {
      TbCommandParser("Wireframe", NULL, FALSE);
   }
   else {
      char arg[256];
      sprintf(arg, "-OFF");
      TbCommandParser("Wireframe", arg, FALSE);
   }
}


/*----------------------------------------------
  set the alpha blending flag for current model
  C.Wang 1209, 2012
 -----------------------------------------------*/
void TbAlphaModelUICommand(UIid uiID, BOOL4 bePress)
{
   FnUiRadioButton uiRB;
   uiRB.ID(uiID);
   if (bePress) {
      TbCommandParser("Alpha", NULL, FALSE);
   }
   else {
      char arg[256];
      sprintf(arg, "-OFF");
      TbCommandParser("Alpha", arg, FALSE);
   }
}


/*---------------------------------------------
  modify the rendering group for current model
  C.Wang 0607, 2013
 ----------------------------------------------*/
void TbModifyRenderGroupUICommand(UIid uiID, char *value)
{
   // get string on typein
   TbCommandParser("SetRenderGroup", value, FALSE);
}


/*-----------------------------------------
  set the show/hide flag for current model
  C.Wang 0607, 2013
 ------------------------------------------*/
void TbShowModelUICommand(UIid uiID, BOOL4 bePress)
{
   FnUiRadioButton uiRB;
   uiRB.ID(uiID);
   if (bePress) {
      TbCommandParser("Show", NULL, FALSE);
   }
   else {
      char arg[256];
      sprintf(arg, "-OFF");
      TbCommandParser("Show", arg, FALSE);
   }
}


/*------------------------------------
  make the system to character viewer
  C.Wang 0115, 2014
 -------------------------------------*/
void TbCharacterUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.ID(tbCharacterSystemButtonID);
      button.ChangeState(TRUE);

      // hide the others
      button.ID(tbEnvironmentSystemButtonID);
      button.Show(FALSE);
      button.ID(tbPostProcessingSystemButtonID);
      button.Show(FALSE);
      button.ID(tbFaceToolButtonID);
      button.Show(FALSE);
      button.ID(tbScriptSystemButtonID);
      button.Show(FALSE);

      button.ID(tbOrthoButtonID);
      button.ChangeState(TRUE);
      button.Enable(FALSE);

      // switch to 3D view
      TbShowOrthoView(tbOrthoButtonID, TRUE);

      TbSwitchMenuBarRadios(tbCharacterSystemButtonID, TRUE);
      tbTool = CHARACTER_TOOL;
   }
}


/*------------------------------------
  make the system to FX viewer
  C.Wang 0115, 2013
 -------------------------------------*/
void TbFXUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.ID(tbFXSystemButtonID);
      button.ChangeState(TRUE);

      // hide the others
      button.ID(tbEnvironmentSystemButtonID);
      button.Show(FALSE);
      button.ID(tbPostProcessingSystemButtonID);
      button.Show(FALSE);
      button.ID(tbFaceToolButtonID);
      button.Show(FALSE);
      button.ID(tbScriptSystemButtonID);
      button.Show(FALSE);

      button.ID(tbOrthoButtonID);
      button.ChangeState(TRUE);
      button.Enable(FALSE);

      // switch to 3D view
      TbShowOrthoView(tbOrthoButtonID, TRUE);

      TbSwitchMenuBarRadios(tbFXSystemButtonID, TRUE);
      tbTool = FX_TOOL;
   }
}


/*-----------------------
  select the action type
  C.Wang 0813, 2012
 ------------------------*/
void TbSelectActionType(UIid uiID, char *value)
{
   FnUiCombo uiC;
   FnUiTypein uiTy;

   uiC.ID(uiID);
   uiC.PasteComboValue(value);

   // according to the selected action type, update the UIs
   if (FyCheckWord(value, "Cross Fade Blending")) {
      uiC.ID(tbAddActionFrontPoseID);
      uiC.Enable(TRUE);
      uiC.ID(tbAddActionEndPoseID);
      uiC.Enable(TRUE);
      uiTy.ID(tbAddActionDurationID);
      uiTy.Enable(TRUE);
   }
   else if (FyCheckWord(value, "Connection")) {
      uiC.ID(tbAddActionFrontPoseID);
      uiC.Enable(TRUE);
      uiC.ID(tbAddActionEndPoseID);
      uiC.Enable(TRUE);
      uiTy.ID(tbAddActionDurationID);
      uiTy.Enable(TRUE);
   }
   else {
      uiC.ID(tbAddActionFrontPoseID);
      uiC.Enable(TRUE);
      uiC.ID(tbAddActionEndPoseID);
      uiC.Enable(FALSE);
      uiTy.ID(tbAddActionDurationID);
      uiTy.Enable(FALSE);
   }
}


/*------------------
  check home path
  C.Wang 0821, 2012
 -------------------*/
void TbCheckHomePath(char *path, char *fPath)
{
   char str[256], *key;

   strcpy(str, path);

   key = strtok(str, "\\");
   fPath[0] = '\0';
   while (key != NULL) {
      if (FyCheckWord(key, "$Home")) {
         key = tbHomeDir;
      }

      // sprintf key to new path
      strcat(fPath, key);

      key = strtok(NULL, "\\");
      if (key != NULL) {
         strcat(fPath, "\\");
      }
   }
}


/*-----------------------
  select the action pose
  C.Wang 0813, 2012
 ------------------------*/
void TbSelectActionPose(UIid uiID, char *value)
{
   FnUiCombo uiC;
   FnUiTypein uiTy;

   uiC.ID(uiID);
   uiC.PasteComboValue(value);
}


/*----------------------
  set current character
  C.Wang 0813, 2012
 -----------------------*/
void TbMakeCurrentCharacterUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SetCharacter", value, FALSE);
}


/*------------------
  set current body
  C.Wang 0813, 2012
 -------------------*/
void TbMakeCurrentBodyUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SetBody", value, FALSE);
}


/*-----------------------------
  set current body's pose data
  C.Wang 0926, 2012
 ------------------------------*/
void TbSetBodyPoseDataUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SetBodyPose", value, FALSE);
}


/*---------------------------
  delete current body's pose
  C.Wang 0926, 2012
 ----------------------------*/
void TbDeleteBodyPoseUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // send command
      TbCommandParser("DeleteBodyPose", NULL, FALSE);
   }
}


/*---------------------------------------
  select current action for current body
  C.Wang 0813, 2012
 ----------------------------------------*/
void TbSelectCurrentBodyActionUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SelectAction", value, FALSE);
}


/*----------------------
  rename current action
  C.Wang 0813, 2012
 -----------------------*/
void TbRenameActionUICommand(UIid uiID, char *value)
{
   // send command
   char arg[256];
   sprintf(arg, "-Action %s", value);
   TbCommandParser("Rename", arg, FALSE);
}


/*----------------------
  delete current action
  C.Wang 0813, 2012
 -----------------------*/
void TbDeleteActionUICommand(UIid uiID, BOOL4 beP)
{
   if (!beP) {
      // send command
      TbCommandParser("DeleteAction", NULL, FALSE);
   }
}


/*---------------------------
  select pose for previewing
  C.Wang 0813, 2012
 ----------------------------*/
void TbSelectPoseUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SelectPose", value, FALSE);
}


/*------------------
  play test pose
  C.Wang 0813, 2012
 -------------------*/
void TbPlayTestPoseUICommand(UIid uiID, BOOL4 beP)
{
   // send command
   if (beP) {
      TbCommandParser("PlayPose", NULL, FALSE);
   }
   else {
      TbCommandParser("PlayPose", "-off", FALSE);
   }
}


/*-----------------------
  make base move command
  C.Wang 0813, 2012
 ------------------------*/
void TbMakeBaseMoveUICommand(UIid uiID, BOOL4 beP)
{
   if (beP) return;

   // extract pose data
   char string[256], poseName[256];
   FnCharacter actor;
   FnUiCombo uiC;
   int iStart, iEnd;

   uiC.ID(tbSelectPoseComboID);
   uiC.GetComboValue(poseName);

   actor.ID(tbCurScene->GetCurrentCharacter());
   actor.QueryBodyPoseDataWithName(actor.GetCurrentBodyName(), poseName, &iStart, &iEnd);

   sprintf(string, "-Frame %d %d", iStart, iEnd);
   TbCommandParser("MakeBaseMove", string, FALSE);
}


/*---------------------------------------------
  make the system to working environment setup
  C.Wang 0115, 2014
 ----------------------------------------------*/
void TbEnvironmentUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.ID(tbEnvironmentSystemButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.ID(tbScriptSystemButtonID);
      button.Show(FALSE);
      button.ID(tbPostProcessingSystemButtonID);
      button.Show(FALSE);
      button.ID(tbFaceToolButtonID);
      button.Show(FALSE);

      TbSwitchMenuBarRadios(tbEnvironmentSystemButtonID, TRUE);
      tbTool = SYSTEM_TOOL;
   }
}


/*-------------------------------------
  open the load character file browser
  C.Wang 0813, 2012
 --------------------------------------*/
void TbLoadCharacterUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("Character", NULL, FALSE);
   }
}


/*----------------------------------------------
  open the load character's motion file browser
  C.Wang 0813, 2012
 -----------------------------------------------*/
void TbLoadCharacterMotionUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("Motion", NULL, FALSE);
   }
}


/*----------------------------------------------
  open the load character's motion file browser
  C.Wang 0813, 2012
 -----------------------------------------------*/
void TbSetCharacterScaleUICommand(UIid uiID, char *value)
{
   // check value validity
   if (!TbCheckReal(value)) {
      FnUiTypein uiTy;
      char string[256];
      CHARACTERid actorID = tbCurScene->GetCurrentCharacter();
      if (actorID != FAILED_ID) {
         FnCharacter actor(actorID);
         sprintf(string, "%6.3f", actor.GetLocalScale());
      }
      else {
         strcpy(string, "1.0");
      }
      
      uiTy.PasteTypeinValue(string);
   }
   else {
      TbCommandParser("CharacterScale", value, FALSE);
   }
}


/*-------------------------
  delete current character
  C.Wang 0813, 2012
 --------------------------*/
void TbDeleteCharacterUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("DeleteCharacter", NULL, FALSE);
   }
}


/*-------------------------------------
  open the save character file browser
  C.Wang 0813, 2012
 --------------------------------------*/
void TbSaveCharacterUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("SaveCharacter", NULL, FALSE);
   }
}


/*---------------------------------------
  export the character to one model file
  C.Wang 0110, 2014
 ----------------------------------------*/
void TbExportCharacterModelUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      TbCommandParser("ExportCharacter", NULL, FALSE);
   }
}


/*------------------------------------------------------------
  bake all the character's model to a vertex animation object
  C.Wang 0312, 2015
 -------------------------------------------------------------*/
void TbCharacterVAUICommand(UIid uiID, BOOL4 bePress)
{
   if (bePress) return;

   CHARACTERid actorID = tbCurScene->GetCurrentCharacter();
   if (actorID == FAILED_ID) return;
   FnCharacter actor(actorID);

   // bake the models with vertex animation
   OBJECTid oID = FAILED_ID;
   oID = actor.BakeCharacterToVertexAnimation(0, oID, VA_FROM_ACTION, 0, -1, TRUE, TRUE);

   // insert the object to scene's object list
   tbCurScene->InsertObject(oID, FAILED_ID, FY_NONE, FY_NONE, TRUE);
}


/*------------------------------
  initialize a critical section
  C.Wang 0813, 2012
 -------------------------------*/
void TbInitializeCriticalSection(TBCriticalSection *cs)
{
   InitializeCriticalSection(&cs->cs);
   cs->id = FY_NONE;
}


/*-------------------------
  enter a critical section
  C.Wang 0813, 2012
 --------------------------*/
BOOL4 TbEnterCriticalSection(TBCriticalSection *cs, int owner)
{
   if (cs == NULL) return FALSE;

   // these are the magic setting for tbRenderCS
   if (cs->id == 0 && owner == 0) return FALSE;
   if (cs->id == 1 && owner == 1) return FALSE;

   // make the critical section locked
   EnterCriticalSection(&cs->cs);
   cs->id = owner;
   return TRUE;
}


/*-------------------------
  leave a critical section
  C.Wang 0813, 2012
 --------------------------*/
void TbLeaveCriticalSection(TBCriticalSection *cs)
{
   LeaveCriticalSection(&cs->cs);
   cs->id = FY_NONE;
}


/*--------------------------
  delete a critical section
  C.Wang 0813, 2012
 ---------------------------*/
void TbDeleteCriticalSection(TBCriticalSection *cs)
{
   DeleteCriticalSection(&cs->cs);
}


/*-----------------------------------
  create the object in texture space
  C.Wang 0116, 2014
 ------------------------------------*/
void TbFormTextureModel()
{
   FnObject textureObj;
   FnObject wireframeObj;
   FnObject faceModel(tbCurrentFaceModelID);

   if (tbFaceOrthoID == FAILED_ID) {
      tbFaceOrthoID = tbOrthoScene.CreateObject();
      textureObj.ID(tbFaceOrthoID);

      tbFaceOrthoEdgeID = tbOrthoScene.CreateObject();
      wireframeObj.ID(tbFaceOrthoEdgeID);
   }
   else {
      textureObj.ID(tbFaceOrthoID);
      wireframeObj.ID(tbFaceOrthoEdgeID);
   }
               
   // clean the geometry
   textureObj.RemoveAllGeometries();
   wireframeObj.RemoveAllGeometries();

   if (tbCurrentFaceModelID == FAILED_ID) return;

   // get all face model's materials
   FnMaterial mat;
   char matName[256];
   GEOMETRYid geoID;
   FnTriangle geo;
   MATERIALid mID;
   int i, numGeo = faceModel.GetGeometryNumber();

   // get material name
   FnUiCombo uiC(tbFindFaceMaterialComboID);
   uiC.GetComboValue(matName);

   // form the texture object
   geoID = FAILED_ID;
   for (i = 0; i < numGeo; i++) {
      geoID = faceModel.GetGeometryID(i);
      geo.ID(geoID);
      mID = geo.GetMaterial();
      mat.ID(mID);
      if (FyCheckWord(mat.GetName(), matName)) break;
   }
   if (geoID == FAILED_ID) return;

   DWORD vType = geo.GetVertexType();
   int vLen, nV, tLen, nT, texLen = 2;
   BOOL4 beR;
   float *ov, *oldV = geo.LockVertexArray(&vLen, &nV, 0);
   float *nv, *newV = (float *)FyAllocateBuffer(sizeof(float)*8*nV);
   int *ot, *oldT = geo.LockIndexArray(&tLen, &nT, FALSE, &beR);
   int *nt, *newT = (int *)FyAllocateBuffer(sizeof(int)*3*nT*2);

   ov = oldV;
   nv = newV;
   if (vType == XYZ) {
      for (i = 0; i < nV; i++) {
         nv[0] = ov[3];
         nv[1] = 1.0f - ov[4];
         nv[2] = 0.0f;
         nv[3] = 0.0f;
         nv[4] = 0.0f;
         nv[5] = 1.0f;
         nv[6] = ov[3];
         nv[7] = 1.0f - ov[4];
         ov += vLen;
         nv += 8;
      }
   }
   else if (vType == XYZ_NORM) {
      for (i = 0; i < nV; i++) {
         nv[0] = ov[6];
         nv[1] = 1.0f - ov[7];
         nv[2] = 0.0f;
         nv[3] = 0.0f;
         nv[4] = 0.0f;
         nv[5] = 1.0f;
         nv[6] = ov[6];
         nv[7] = 1.0f - ov[7];
         ov += vLen;
         nv += 8;
      }
   }
   else if (vType == XYZ_RGB) {
      for (i = 0; i < nV; i++) {
         nv[0] = ov[4];
         nv[1] = 1.0f - ov[5];
         nv[2] = 0.0f;
         nv[3] = 0.0f;
         nv[4] = 0.0f;
         nv[5] = 1.0f;
         nv[6] = ov[4];
         nv[7] = 1.0f - ov[5];
         ov += vLen;
         nv += 8;
      }
   }

   ot = oldT;
   nt = newT;
   if (!beR) {
      for (i = 0; i < nT; i++) {
         nt[0] = ot[2];
         nt[1] = ot[1];
         nt[2] = ot[0];
         nt += 3;
         ot += 3;
      }
   }
   else {
      memcpy(newT, oldT, sizeof(int)*3*nT);
   }

   geo.UnlockVertexArray();
   geo.UnlockIndexArray();

   // form the new triangles
   textureObj.Triangles(XYZ_NORM, mID, 1, nV, 1, &texLen, newV, nT, newT);

   // form the wireframe mask
   MATERIALid newMatID = FAILED_ID;
   float color[3];
   color[0] = 0.0f; color[1] = 0.3f; color[2] = 0.0f;
   newMatID = FyCreateMaterial(color, color, color, 10.0f, color);
   nv = newV;
   for (i = 0; i < nV; i++) {
      nv[2] = 1.0f;
      nv += 8;
   }
   wireframeObj.Triangles(XYZ_NORM, newMatID, 1, nV, 1, &texLen, newV, nT, newT);
   wireframeObj.SetRenderMode(WIREFRAME);

   // release the buffer
   FyReleaseBuffer(newT);
   FyReleaseBuffer(newV);
}


/*--------------------------------------------
  get current object ID according to Tool Tab
  C.Wang 0408, 2013
 ---------------------------------------------*/
OBJECTid TbGetCurrentBaseObjID()
{
   if (tbTool == FX_TOOL) {
      TBGameFXSystem * pCurGX = tbCurScene->GetCurrentTBGameFXSystem();
      if (pCurGX == NULL) return FAILED_ID;

      FnBaseGameFX curBaseFX(pCurGX->GetCurrentGameFX());
      if (curBaseFX.ID() == FAILED_ID) return FAILED_ID;

      return curBaseFX.GetBaseObject();
   }
   else {
      return tbCurScene->GetCurrentObject();
   }
}


/*----------------------------------------
  twiddle on/off the translate gizmo view
  Eric Fei 1217, 2012
 -----------------------------------------*/
void TbSwitchGizmo(UIid uiID, BOOL4 bePress)
{
   // update Gizmo Base Position
   FnObject curObj(TbGetCurrentBaseObjID());
   if (curObj.ID() == FAILED_ID)  return;
   
   FnObject gizmoObj;
   float    pos[3];
   curObj.GetPosition(pos);

   gizmoObj.ID(tbGizmoTranslate.baseID);
   gizmoObj.SetPosition(pos);
   gizmoObj.ID(tbGizmoRotate.baseID);
   gizmoObj.SetPosition(pos);

   // switch Gizmo
   FnUiRadioButton radio;

   if (uiID == tbModelTFTranslateButtonID) {   // switch to Translate Gizmo
      // open Translate Gizmo
      radio.ID(tbModelTFTranslateButtonID);
      radio.ChangeState(bePress);
      tbGizmoTranslate.isShow = bePress;
      if (FyCheckHotKeyStatus(FY_SHIFT)) {
         for (int i = 0; i < tbGizmoTranslate.numGizmoObj; i++) {
            gizmoObj.ID(tbGizmoTranslate.gizmoIDs[i]);
            gizmoObj.Show(tbGizmoTranslate.isShow);
         }
      }

      // Close Rotate Gizmo
      radio.ID(tbModelTFRotateButtonID);
      radio.ChangeState(FALSE);
      tbGizmoRotate.isShow    = FALSE;
      if (FyCheckHotKeyStatus(FY_SHIFT)) {
         for (int i = 0; i < tbGizmoRotate.numGizmoObj; i++) {
            gizmoObj.ID(tbGizmoRotate.gizmoIDs[i]);
            gizmoObj.Show(FALSE);
         }
      }
   }
   else if (uiID == tbModelTFRotateButtonID) { // Switch to Rotate Gizmo
      // Close Translate Gizmo
      radio.ID(tbModelTFTranslateButtonID);
      radio.ChangeState(FALSE);
      tbGizmoTranslate.isShow = FALSE;
      if (FyCheckHotKeyStatus(FY_SHIFT)) {
         for (int i = 0; i < tbGizmoTranslate.numGizmoObj; i++) {
            gizmoObj.ID(tbGizmoTranslate.gizmoIDs[i]);
            gizmoObj.Show(FALSE);
         }
      }

      // Open Rotate Gizmo
      radio.ID(tbModelTFRotateButtonID);
      radio.ChangeState(bePress);
      tbGizmoRotate.isShow = bePress;
      if (FyCheckHotKeyStatus(FY_SHIFT)) {
         for (int i = 0; i < tbGizmoRotate.numGizmoObj; i++) {
            gizmoObj.ID(tbGizmoRotate.gizmoIDs[i]);
            gizmoObj.Show(tbGizmoRotate.isShow);
         }
      }
   }
}


/*------------------------------------
  hotkey callback for Show/Hide Gizmo
  Eric Fei 0107, 2013
 -------------------------------------*/
void TbShowGizmo(BYTE code, BOOL4 value)
{
   BOOL4 beShow;      // Show/Hide

   // Update Gizmo Object Position
   FnObject curObj(TbGetCurrentBaseObjID());
   if (curObj.ID() == FAILED_ID) return;
   
   FnObject gizmoObj;
   float pos[3];
   curObj.GetPosition(pos);

   gizmoObj.ID(tbGizmoTranslate.baseID);
   gizmoObj.SetPosition(pos);
   gizmoObj.ID(tbGizmoRotate.baseID);
   gizmoObj.SetPosition(pos);

   // Show/Hide Translate Gizmo
   beShow = tbGizmoTranslate.isShow && value;
   for (int i = 0; i < tbGizmoTranslate.numGizmoObj; i++) {
      gizmoObj.ID(tbGizmoTranslate.gizmoIDs[i]);
      gizmoObj.Show(beShow);
   }

   // Show/Hide Rotate Gizmo
   beShow = tbGizmoRotate.isShow && value;
   for (int i = 0; i < tbGizmoRotate.numGizmoObj; i++) {
      gizmoObj.ID(tbGizmoRotate.gizmoIDs[i]);
      gizmoObj.Show(beShow);
   }
}


/*------------------------------------------------------
  hotkey callback for ESC key to cancel Gizmo operation
  Eric Fei 1218, 2012
 -------------------------------------------------------*/
void TbCancelGizmoProcess(BYTE code, BOOL4 value)
{
   if (tbGizmoTranslate.beDrag || tbGizmoRotate.beDrag) {
      tbGizmoTranslate.iCurGizmo = -1;
      tbGizmoRotate.iCurGizmo = -1;
      tbGizmoTranslate.beDrag = FALSE;
      tbGizmoRotate.beDrag = FALSE;

      // recover original Position/Rotate of Current Object
      FnObject curObj(tbCurScene->GetCurrentObject());
      curObj.SetMatrix(tbGizmoBackupM12, REPLACE);

      // recover original Position of Translate Gizmo
      float pos[3];
      FnObject gizmoObj(tbGizmoTranslate.baseID);
      curObj.GetPosition(pos);
      gizmoObj.SetPosition(pos);

      // recover Transform Typein UI
      TbPasteCurrentTransformData(tbGizmoBackupM12);
   }
}


/*------------------------------------
  Generate SetTransform Argument
  Eric Fei 1218, 2012
 -------------------------------------*/
void TbGenTransformArgu(char * argu, int len)
{
   FnUiTypein uiTy;
   char v[256];
   argu[0] = '\0';

   if (tbTool == MODEL_TOOL) {
      strcat(argu, "-pos ");

      uiTy.ID(tbModelTFTranslateXID);
      uiTy.GetTypeinValue(v);
      strcat(argu, v);
      strcat(argu, " ");

      uiTy.ID(tbModelTFTranslateYID);
      uiTy.GetTypeinValue(v);
      strcat(argu, v);
      strcat(argu, " ");

      uiTy.ID(tbModelTFTranslateZID);
      uiTy.GetTypeinValue(v);
      strcat(argu, v);
      strcat(argu, " ");
      
      strcat(argu, "-rot ");

      uiTy.ID(tbModelTFRotateXID);
      uiTy.GetTypeinValue(v);
      strcat(argu, v);
      strcat(argu, " ");

      uiTy.ID(tbModelTFRotateYID);
      uiTy.GetTypeinValue(v);
      strcat(argu, v);
      strcat(argu, " ");

      uiTy.ID(tbModelTFRotateZID);
      uiTy.GetTypeinValue(v);
      strcat(argu, v);
      strcat(argu, " ");
   }
   else if (tbTool == FX_TOOL) {
      if (tbGizmoTranslate.isShow) {
         strcat(argu, "-pos ");

         uiTy.ID(tbGameFXKeyDataID[0]);
         uiTy.GetTypeinValue(v);
         strcat(argu, v);
         strcat(argu, " ");

         uiTy.ID(tbGameFXKeyDataID[1]);
         uiTy.GetTypeinValue(v);
         strcat(argu, v);
         strcat(argu, " ");

         uiTy.ID(tbGameFXKeyDataID[2]);
         uiTy.GetTypeinValue(v);
         strcat(argu, v);
         strcat(argu, " ");
      }
      else if (tbGizmoRotate.isShow) {
         strcat(argu, "-rot ");

         uiTy.ID(tbGameFXKeyDataID[0]);
         uiTy.GetTypeinValue(v);
         strcat(argu, v);
         strcat(argu, " ");

         uiTy.ID(tbGameFXKeyDataID[1]);
         uiTy.GetTypeinValue(v);
         strcat(argu, v);
         strcat(argu, " ");

         uiTy.ID(tbGameFXKeyDataID[2]);
         uiTy.GetTypeinValue(v);
         strcat(argu, v);
         strcat(argu, " ");
      }
   }
}


/*------------------------------------------------
  paste the position/rotation value to Typein UIs
  Eric Fei 0108, 2013
 -------------------------------------------------*/
void TbPasteCurrentTypeinValue(float * pos, float * rot)
{
   FnUiTypein  uiTy;          // for Typein UI
   char typein[32];           // for String Value of Typein UI

   if (tbTool == FX_TOOL) {
      if (tbGizmoTranslate.isShow && pos != NULL) {
         uiTy.ID(tbGameFXKeyDataID[0]);
         sprintf(typein, "%5.2f", pos[0]);
         uiTy.PasteTypeinValue(typein);

         uiTy.ID(tbGameFXKeyDataID[1]);
         sprintf(typein, "%5.2f", pos[1]);
         uiTy.PasteTypeinValue(typein);

         uiTy.ID(tbGameFXKeyDataID[2]);
         sprintf(typein, "%5.2f", pos[2]);
         uiTy.PasteTypeinValue(typein);
      }
      else if (tbGizmoRotate.isShow && rot != NULL) {
         uiTy.ID(tbGameFXKeyDataID[0]);
         sprintf(typein, "%5.2f", rot[0]);
         uiTy.PasteTypeinValue(typein);

         uiTy.ID(tbGameFXKeyDataID[1]);
         sprintf(typein, "%5.2f", rot[1]);
         uiTy.PasteTypeinValue(typein);

         uiTy.ID(tbGameFXKeyDataID[2]);
         sprintf(typein, "%5.2f", rot[2]);
         uiTy.PasteTypeinValue(typein);
      }
   }
   else {
      if (pos != NULL) {
         uiTy.ID(tbModelTFTranslateXID);
         sprintf(typein, "%5.2f", pos[0]);
         uiTy.PasteTypeinValue(typein);
         uiTy.ID(tbModelTFTranslateYID);
         sprintf(typein, "%5.2f", pos[1]);
         uiTy.PasteTypeinValue(typein);
         uiTy.ID(tbModelTFTranslateZID);
         sprintf(typein, "%5.2f", pos[2]);
         uiTy.PasteTypeinValue(typein);
      }

      if (rot != NULL) {
         uiTy.ID(tbModelTFRotateXID);
         sprintf(typein, "%5.2f", rot[0]);
         uiTy.PasteTypeinValue(typein);
         uiTy.ID(tbModelTFRotateYID);
         sprintf(typein, "%5.2f", rot[1]);
         uiTy.PasteTypeinValue(typein);
         uiTy.ID(tbModelTFRotateZID);
         sprintf(typein, "%5.2f", rot[2]);
         uiTy.PasteTypeinValue(typein);
      }
   }
}


/*---------------------------------------
  paste the material data to Typein UIs
  Eric Fei 1219, 2012
 ----------------------------------------*/
void TbPasteCurrentTransformData(float * m12)
{
   float angle[3];

   FyFormEulerAnglesXYZFromM12(&angle[0], &angle[1], &angle[2], m12);
   TbPasteCurrentTypeinValue(&m12[9], angle);
}


/*---------------------------------------
  update the transform data to Typein UIs
  Eric Fei 1230, 2012
 ----------------------------------------*/
void TbUpdateTransformUIData(BOOL4 beUpdate)
{
   if (beUpdate) {
      FnObject curObj(TbGetCurrentBaseObjID());
      float m12[12];
      curObj.GetMatrix(m12);
      TbPasteCurrentTransformData(m12);
   }
}


/*----------------------------------
  twiddle on/off the auxiliary view
  C.Wang 0813, 2012
 -----------------------------------*/
void TbShowAuxView(UIid uiID, BOOL4 bePress)
{
   FnUiRadioButton radio;
   radio.ID(tbAuxButtonID);
   tbShowAux = radio.GetState();

   // show/hide bounding box
   FyBeRenderBoundingBox(tbShowAux);

   // show/hide current character's base
   CHARACTERid aID = tbCurScene->GetCurrentCharacter();
   if (aID != FAILED_ID) {
      FnCharacter actor;
      actor.ID(aID);

      FnObject base;
      base.ID(actor.GetBaseObject());
      base.Show(tbShowAux);
   }
}


/*-----------------------------------
  twiddle on/off the orthogonal view
  C.Wang 0813, 2012
 ------------------------------------*/
void TbShowOrthoView(UIid uiID, BOOL4 bePress)
{
   FnUiRadioButton radio;
   FnUiRadioButton radioAux;

   radio.ID(tbOrthoButtonID);
   radioAux.ID(tbAuxButtonID);
   if (radio.GetState()) {
      tbRenderCamera = PERSPECTIVE;
      radioAux.Enable(TRUE);
   }
   else {
      tbRenderCamera = ORTHOGONAL;
      radioAux.Enable(FALSE);
   }
}


/*------------------------------------------------------
  command to setup the working and data searching paths
  C.Wang 1024, 2012
 -------------------------------------------------------*/
BOOL4 TbSetPathCmd(char *argument)
{
   BOOL4 beOK = FALSE;
   char path[256];

   // check argument
   BOOL4 beOKCmd = TbCheckCommandAttributeS(argument, "-Work", path, 256);
   if (beOKCmd) {
      SetCurrentDirectory(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Texture", path, 256);
   if (beOKCmd) {
      FySetTexturePath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Scene", path, 256);
   if (beOKCmd) {
      FySetScenePath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Object", path, 256);
   if (beOKCmd) {
      FySetModelPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Shader", path, 256);
   if (beOKCmd) {
      FySetShaderPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-GameFX", path, 256);
   if (beOKCmd) {
      FySetGameFXPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Character", path, 256);
   if (beOKCmd) {
      FySetCharacterPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Audio", path, 256);
   if (beOKCmd) {
      FySetAudioPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Extra", path, 256);
   if (beOKCmd) {
      FySetDataPath(path);
      beOK = TRUE;
   }

   if (beOK) {
      TbSetLogMessage("Set paths OK");
   }
   else {
      TbSetLogMessage("Nothing set!");
   }
   return beOK;
}


/*-------------------------------------------
  command to change the tool mode to
  Model Viewer, Character Viwer, ...
  C.Wang 0115, 2014
 --------------------------------------------*/
BOOL4 TbSwitchToolModeCmd(char *argument)
{
   // check argument
   BOOL4 beOK = FALSE;
   BOOL4 beOKCmd = TbCheckCommandAttributeS(argument, "-Model", NULL, 256);
   if (beOKCmd) {
      TbModelUICommand(tbModelButtonID, FALSE);
      TbSetLogMessage("Switch to Model Viewer");
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Script", NULL, 256);
   if (beOKCmd) {
      TbScriptUICommand(tbScriptButtonID, FALSE);
      TbSetLogMessage("Switch to Script Loader");
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Character", NULL, 256);
   if (beOKCmd) {
      TbCharacterUICommand(tbCharacterButtonID, FALSE);
      TbSetLogMessage("Switch to Character Viewer");
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-FX", NULL, 256);
   if (beOKCmd) {
      //TbFXUICommand(tbFXButtonID, FALSE);
      TbSetLogMessage("Switch to FX Viewer");
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Environment", NULL, 256);
   if (beOKCmd) {
      TbEnvironmentUICommand(tbEnvironmentButtonID, FALSE);
      TbSetLogMessage("Switch to Setup Working Environment");
      beOK = TRUE;
   }
   beOKCmd = TbCheckCommandAttributeS(argument, "-Post", NULL, 256);
   if (beOKCmd) {
      TbPostProcessingUICommand(tbPostProcessingButtonID, FALSE);
      TbSetLogMessage("Switch to Setup Post Processing");
      beOK = TRUE;
   }
   beOKCmd = TbCheckCommandAttributeS(argument, "-Face", NULL, 256);
   if (beOKCmd) {
      TbFaceToolUICommand(tbFaceButtonID, FALSE);
      TbSetLogMessage("Switch to Face Tool");
      beOK = TRUE;
   }

   return beOK;
}


/*---------------------------------------
  play speed command - set playing speed
  C.Wang 0813, 2012
 ----------------------------------------*/
BOOL4 TbSetPlaySpeedCmd(char *arg)
{
   // find the speed value
   char string[256], log[256], *key;
   float speed = 1.0f;

   strcpy(string, arg);
   key = strtok(string, " ");
   if (key != NULL) {
      speed = (float) atof(key);
   }
   tbPlaySpeed = FYABS(speed);

   // paste the play seed data to UI
   FnUiTypein uiTy;
   char value[256];
   uiTy.ID(tbPlaySpeedID);
   sprintf(value, "%5.3f", tbPlaySpeed);
   uiTy.PasteTypeinValue(value);

   sprintf(log, "Set playing speed = %6.3f", tbPlaySpeed);
   TbSetLogMessage(log);
   return TRUE;
}


/*---------------------
  smooth current model
  C.Wang 0813, 2012
 ----------------------*/
BOOL4 TbSmoothModelCmd(char *arg)
{
   OBJECTid oID = tbCurScene->GetCurrentObject();
   if (oID == FAILED_ID) {
      TbSetLogMessage("No Current Object");
      return TRUE;
   }

   FnObject model;
   //FnTriangle tri;

   model.ID(oID);
   int i, numG = model.GetGeometryNumber();
   for (i = 0; i < numG; i++) {
      //tri.ID(oID, i);
      //tri.AddNormalData();
   }

   TbSetLogMessage("Smooth Current Object");
   return TRUE;
}


/*--------------------------------------
  command to select pose for previewing
  C.Wang 0430, 2014
 ---------------------------------------*/
BOOL4 TbSelectPoseCmd(char *argument)
{
   // extract pose data
   FnCharacter actor;
   char string[256];
   BOOL4 beEditPose = FALSE;
   actor.ID(tbCurScene->GetCurrentCharacter());
   beEditPose = actor.QueryBodyPoseDataWithName(actor.GetCurrentBodyName(), argument, &tbTestPoseStart, &tbTestPoseEnd, tbTestMotionName);

   FnUiButton uiB;
   uiB.ID(tbDeletePoseButtonID);
   if (beEditPose) {
      // edit the pose
      tbTestPoseFrame = (float) tbTestPoseStart;
      FnUiTypein uiTy;
      uiTy.ID(tbPoseStartID);
      sprintf(string, "%d", tbTestPoseStart);
      uiTy.PasteTypeinValue(string);
      uiTy.Enable(TRUE);
   
      uiTy.ID(tbPoseEndID);
      sprintf(string, "%d", tbTestPoseEnd);
      uiTy.PasteTypeinValue(string);
      uiTy.Enable(TRUE);

      uiTy.ID(tbPoseMotionNameID);
      uiTy.PasteTypeinValue(tbTestMotionName);

      uiB.Enable(TRUE);
      strcpy(tbCurrentBodyPoseName, argument);
   }
   else {
      uiB.Enable(FALSE);
      strcpy(tbCurrentBodyPoseName, "");
   }

   return TRUE;
}


/*--------------------------------------
  command to select pose for previewing
  C.Wang 0813, 2012
 ---------------------------------------*/
BOOL4 TbPlayTestPoseCmd(char *argument)
{
   // check argument
   if (argument == NULL) {
      tbBePlayPose = TRUE;
   }
   else {
      if (FyCheckWord(argument, "-off")) {
         tbBePlayPose = FALSE;
      }
      else {
         tbBePlayPose = TRUE;
      }
   }

   return TRUE;
}


/*---------------------------------------------
  command to twiddle on/off the auxiliary view
  C.Wang 0813, 2012
 ----------------------------------------------*/
BOOL4 TbShowAuxViewCmd(char *argument)
{
   // check argument
   BOOL4 beOK = TbCheckCommandAttributeS(argument, "-OFF", NULL, 256);
   FnUiRadioButton radio;
   radio.ID(tbAuxButtonID);
   if (beOK) {
      radio.ChangeState(FALSE);
      TbShowAuxView(tbAuxButtonID, FALSE);
   }
   else {
      radio.ChangeState(TRUE);
      TbShowAuxView(tbAuxButtonID, TRUE);
   }

   return TRUE;
}


/*----------------------------------------------
  command to twiddle on/off the orthogonal view
  C.Wang 0813, 2012
 -----------------------------------------------*/
BOOL4 TbShowOrthoViewCmd(char *argument)
{
   // check argument
   BOOL4 beOK = TbCheckCommandAttributeS(argument, "-OFF", NULL, 256);
   FnUiRadioButton radio;
   radio.ID(tbOrthoButtonID);
   if (beOK) {
      radio.ChangeState(TRUE);
      TbShowOrthoView(tbOrthoButtonID, TRUE);
   }
   else {
      radio.ChangeState(FALSE);
      TbShowOrthoView(tbOrthoButtonID, FALSE);
   }

   return TRUE;
}


/*------------------
  make base to move
  C.Wang 0814, 2012
 -------------------*/
BOOL4 TbMakeBaseMoveCmd(char *argument)
{
   FnCharacter actor;
   actor.ID(tbCurScene->GetCurrentCharacter());
   if (actor.ID() == FAILED_ID) {
      return FALSE;
   }

   int iStart, iEnd, frame[2];
   iStart = 0;
   iEnd = 30;
   BOOL4 beOK = TbCheckCommandAttributeI(argument, "-Frame", frame, 2);
   if (beOK) {
      iStart = frame[0];
      iEnd = frame[1];
   }

   // check if the bone's parent is the base
   char *rootName = NULL;
   OBJECTid oID = actor.GetRootBone(0);
   FnObject rootObject(oID);
   rootName = rootObject.GetName();
   actor.MakeBaseMove(rootName, iStart, iEnd);
   actor.PlayFrame(tbTestPoseFrame, tbBeBase, TRUE, tbPlaySpeed);

   char log[256];
   sprintf(log, "Make the base of %s to move from %d to %d", actor.GetName(), iStart, iEnd);
   TbSetLogMessage(log);
   return TRUE;
}


/*---------------------------------------
  make the material update flag to dirty
  C.Wang 0814, 2012
 ----------------------------------------*/
void TbMakeUpdateDirtyTy(UIid uiID, char *data)
{
   FnUiButton uiB;
   uiB.ID(tbUpdateMaterialID);
   uiB.Enable(TRUE);
}


/*---------------------------------------
  make the material update flag to dirty
  C.Wang 0814, 2012
 ----------------------------------------*/
void TbMakeUpdateDirtyC(UIid uiID, char *data)
{
   FnUiButton uiB;
   uiB.ID(tbUpdateMaterialID);
   uiB.Enable(TRUE);
}


/*---------------------------------------------
  open the load facial expression file browser
  C.Wang 0515, 2015
 ----------------------------------------------*/
void TbLoadFacialExpressionUI(UIid uiID, BOOL4 bePress)
{
   if (bePress) return;

   TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
   if (actor == NULL) return;

   // open the file browser for loading an object file
   OPENFILENAME ofn;
   char openName[256], dir[256];
   BOOL4 beOK = FALSE;

   // get current directory
   GetCurrentDirectory(256, dir);

   // prepare to open the file browser
   openName[0] = '\0';
   memset(&ofn, 0, sizeof(OPENFILENAME));
   ofn.lStructSize = sizeof(OPENFILENAME);
   ofn.hwndOwner = NULL;
   ofn.lpstrFilter = "Fly2 Facial Expression CWF {*.cwf}\0*.cwf\0" \
                     "All files {*.*}\0*.*\0\0";
   ofn.lpstrFile = openName;
   ofn.nMaxFile = 256;
   ofn.Flags = OFN_FILEMUSTEXIST;
   ofn.lpstrDefExt = "cwf";
   ofn.lpstrInitialDir = dir;

   // start to load the model file
   if (GetOpenFileName(&ofn)) {
      // set the extra directory for data loading
      char name[256];
      GetCurrentDirectory(256, name);
      FySetDataPath(name);

      // success to get the file name
      beOK = TRUE;
   }

   // restore the current directory
   if (tbBeRestoreCurrentDir) {
      SetCurrentDirectory(dir);
   }

   if (beOK) {
      // set waiting cursor
      SetCursor(tbWaitCursor);

      BOOL4 beLoadOK = actor->LoadFacialExpression(openName);
      if (beLoadOK){
         // update UIs
         TbUpdateCurrentFacialExpressionUIs();
         TbStopPlayFacialExpression(tbStopFacialExpressionID, FALSE);

         // clear all commands since this step can not undo
         FcClearCommand();
      }

      // set standard cursor
      SetCursor(tbArrowCursor);
   }
}


/*---------------------------------
  open the save model file browser
  C.Wang 0813, 2012
 ----------------------------------*/
void TbSaveFacialExpressionUI(UIid uiID, BOOL4 bePress)
{
   if (bePress) return;

   TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
   if (actor == NULL) return;

   // open the file browser for saving an object file
   OPENFILENAME ofn;
   char saveName[256], dir[256];
   BOOL4 beOK = FALSE;

   // get current directory
   GetCurrentDirectory(256, dir);

   // prepare to open the file browser
   saveName[0] = '\0';
   memset(&ofn, 0, sizeof(OPENFILENAME));
   ofn.lStructSize = sizeof(OPENFILENAME);
   ofn.hwndOwner = NULL;
   ofn.lpstrFilter = "Fly2 Facial Expression CWF {*.cwf}\0*.cwf\0"\
                     "All files {*.*}\0*.*\0\0";
   ofn.lpstrFile = saveName;
   ofn.nMaxFile = 256;
   ofn.Flags = OFN_FILEMUSTEXIST;
   ofn.lpstrDefExt = "cwf";
   ofn.lpstrInitialDir = dir;

   // get the model file name
   if (GetSaveFileName(&ofn)) {
      // set the extra directory for data loading
      char name[256];
      GetCurrentDirectory(256, name);
      FySetDataPath(name);

      // success to get the file name
      beOK = TRUE;
   }

   // restore the current directory
   if (tbBeRestoreCurrentDir) {
      SetCurrentDirectory(dir);
   }

   if (beOK) {
      // set waiting cursor
      SetCursor(tbWaitCursor);

      // save the facial expression
      actor->SaveFacialExpression(saveName);
      
      // set standard cursor
      SetCursor(tbArrowCursor);
   }
}


/*--------------------------------------------
  open the "add a facial expression" dialogue
  C.Wang 0513, 2014
 ---------------------------------------------*/
void TbAddFaceExpression(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // popup the dialogue
      tbAddFacialExpressionUI.PopUp(TRUE);

      // initialize the dialogue
      FnUiTypein uiTy;
      char tValue[256];
      int id;

      uiTy.ID(tbAddFacialExpressionNameID);
      id = FyRandomInteger(0, 9999, 10000);
      sprintf(tValue, "NewFacialExp_%04d", id);
      uiTy.PasteTypeinValue(tValue);

      uiTy.ID(tbAddFacialExpressionLengthID);
      uiTy.PasteTypeinValue("100");
      tbInputInteger = 100;
   }
}


/*---------------------------------
  select current facial expression
  C.Wang 0513, 2014
 ----------------------------------*/
void TbSelectFaceExpressionComboUICommand(UIid uiID, char *value)
{
   // extract the data from UIs
   FnUiCombo uiC;
   char argument[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   strcat(argument, value);

   // execute command to make current facial expression
   TbCommandParser("MakeCurrentFacialExpression", argument, FALSE);
}


/*---------------------------------
  rename current facial expression
  C.Wang 0513, 2014
 ----------------------------------*/
void TbRenameFacialExpressionUICommand(UIid uiID, char *value)
{
   // extract the data from UIs
   FnUiCombo uiC;
   char argument[256];
   argument[0] = '\0';

   strcat(argument, "-name ");
   strcat(argument, value);

   // execute command to rename current facial expression
   TbCommandParser("RenameCurrentFacialExpression", argument, FALSE);
}


/*-------------------------------
  create a new facial expression
  C.Wang 0513, 2014
 --------------------------------*/
void TbNewFacialExpressionUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      tbAddFacialExpressionUI.PopUp(FALSE);

      // extract the data from UIs
      FnUiTypein uiTy;
      char argument[256], value[256];
      argument[0] = '\0';

      uiTy.ID(tbAddFacialExpressionNameID);
      strcat(argument, "-name ");
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      uiTy.ID(tbAddFacialExpressionLengthID);
      strcat(argument, " -length ");
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      // execute command to create a new facial expression
      TbCommandParser("NewFacialExpression", argument, FALSE);
   }
}


/*---------------------------------
  remove current facial expression
  C.Wang 0513, 2014
 ----------------------------------*/
void TbRemoveFacialExpressionUICommand(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {   
      // extract the data from UIs
      char argument[256];
      argument[0] = '\0';

      TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
      if (actor == NULL) return;

      char *name = actor->GetCurrentFacialExpression();
      if (name == NULL) return;

      strcat(argument, "-name ");
      strcat(argument, name);

      // execute command to remove current facial expression
      TbCommandParser("RemoveFacialExpression", argument, FALSE);
   }
}


/*---------------------------------------
  set current facial expression's length
  C.Wang 0513, 2014
 ----------------------------------------*/
void TbSetFaceExpressionLengthUICommand(UIid uiID, char *value)
{
   // extract the data from UIs
   char argument[256];
   argument[0] = '\0';

   TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
   if (actor == NULL) return;

   char *name = actor->GetCurrentFacialExpression();
   if (name == NULL) return;

   strcat(argument, "-length ");
   strcat(argument, value);

   // execute command to set current facial expression animation length
   TbCommandParser("FacialExpressionlength", argument, FALSE);
}


/*---------------------------------------
  remove a keyframe of facial expression
  C.Wang 0515, 2014
 ----------------------------------------*/
void TbSetupBlendShapeValueCommand(UIid uiID, char *value)
{
   // execute command to remove a facial expression key
   TbCommandParser("UpdateFacialExpressionKey", NULL, FALSE);
}


/*---------------------------------------
  remove a keyframe of facial expression
  C.Wang 0515, 2014
 ----------------------------------------*/
void TbRemoveFaceExpressionKeyCommand(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // execute command to remove a facial expression key
      TbCommandParser("RemoveFacialExpressionKey", NULL, FALSE);
   }
}


/*----------------------------------------
  add a new keyframe of facial expression
  C.Wang 0515, 2014
 -----------------------------------------*/
void TbAddFaceExpressionKeyCommand(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      // execute command to add a new facial expression key
      TbCommandParser("AddFacialExpressionKey", NULL, FALSE);
   }
}


/*-------------------------------
  stop to play facial expression
  C.Wang 0515, 2014
 --------------------------------*/
void TbStopPlayFacialExpression(UIid uiID, BOOL4 bePressed)
{
   if (!bePressed) {
      tbBePlayFacialExpression = FALSE;

      FnUiRadioButton uiRB;
      uiRB.ID(tbPlayFacialExpressionID);
      uiRB.ChangeState(FALSE);

      TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
      if (actor == NULL) return;

      BOOL4 beOK = actor->PlayFacialExpression(START, 0.0f);
      if (!beOK) {
         // show original base shape
      }
   }
}


/*---------------------------------
  play/pause the facial expression
  C.Wang 0515, 2014
 ----------------------------------*/
void TbMakePlayFacialExpression(UIid uiID, BOOL4 bePressed)
{
   tbBePlayFacialExpression = bePressed;
}


/*--------------------------------------
  go to some frame of facial expression
  C.Wang 0515, 2014
 ---------------------------------------*/
void TbGotoFacialExpressionFrame(UIid uiID, char *value)
{
   TBCharacter *actor = tbCurScene->GetCurrentTBCharacter();
   if (actor == NULL) return;

   // get current facial expression length
   FnUiCombo uiC(uiID);
   char string[256];
   char *name = actor->GetCurrentFacialExpression();
   if (name == NULL) return;
   int length = actor->GetFacialExpressionLength(name);

   // check value
   if (!TbCheckInt(value)) {
      if (tbCurFacialExpressionFrame >= 0) {
         sprintf(string, "%d", tbCurFacialExpressionFrame);
      }
      else {
         string[0] = '\0';
      }
      uiC.PasteComboValue(string);
      return;
   }
   
   int i, iOne, numBS, curFrame = atoi(value);
   if (curFrame < 0) curFrame = 0;
   if (curFrame >= length) curFrame = length;

   // check the selection if keyframe or not
   float weight[64];
   FnUiTypein uiTy;
   FnUiButton uiB;
   numBS = actor->GetBlendShapeNumber(0);
   if ((iOne = actor->CheckFacialExpressionKey(name, (float)curFrame)) >= 0) {
      // enable/paste all blendshapes' weights
      actor->GetFacialExpressionKey(name, iOne, NULL, weight);
      for (i = 0; i < numBS; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         sprintf(string, "%f", weight[i]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);
         uiTy.Enable(TRUE);
      }
      for (i = numBS; i < MAX_BLENDSHAPES; i++) {
         uiTy.ID(tbBlendShapeID[i]);
         uiTy.Show(FALSE);
      }

      // disable "add new keyframe"
      uiB.ID(tbAddFacialExpressionKeyID);
      uiB.Enable(FALSE);

      // enable "remove keyframe"
      uiB.ID(tbRemoveFacialExpressionKeyID);
      uiB.Enable(TRUE);

      // play the frame
      actor->PlayFacialExpression(START, (float) curFrame);

      return;
   }

   // enable "add new keyframe"
   uiB.ID(tbAddFacialExpressionKeyID);
   uiB.Enable(TRUE);

   // disable "remove keyframe"
   uiB.ID(tbRemoveFacialExpressionKeyID);
   uiB.Enable(FALSE);

   sprintf(string, "%d", curFrame);
   uiC.PasteComboValue(string);

   // enable the blendshape typeins
   for (i = 0; i < numBS; i++) {
      uiTy.ID(tbBlendShapeID[i]);
      uiTy.Show(TRUE);
      uiTy.Enable(FALSE);
   }
   for (i = numBS; i < MAX_BLENDSHAPES; i++) {
      uiTy.ID(tbBlendShapeID[i]);
      uiTy.Show(FALSE);
   }

   // play the frame
   actor->PlayFacialExpression(START, (float) curFrame);
}


/*--------------------------------------
  go to some frame of facial expression
  C.Wang 0515, 2014
 ---------------------------------------*/
void TbSelectFacialExpressionKey(UIid uiID, char *value)
{
   TbGotoFacialExpressionFrame(uiID, value);
}


/*----------------------------------------
  open game FX keyframe editing dialogure
  Eric Fei 0108, 2013
 -----------------------------------------*/
void TbOpenKeyframeEditor(UIid uiID, BOOL4 bePress)
{
   int i, iKey, numKeys;
   FnUiPlate uiP;
   FnUiTypein uiTy;
   FnUiCombo uiC;
   FnUiRadioButton uiRB;
   char string[256];
   TBGameFXSystem *gx;
   GAMEFXid fxID;
   FnBaseGameFX baseFX;

   if (!bePress) {
      // popup the UIs
      tbGameFXKeyframeUI.PopUp(TRUE);

      // get current FX
      gx = tbCurScene->GetCurrentTBGameFXSystem();
      if (gx == NULL) return;
      fxID = gx->GetCurrentGameFX();
      if (fxID == FAILED_ID) return;
      baseFX.ID(fxID);

      // pause the animation
      uiRB.ID(tbPlayID);
      uiRB.ChangeState(FALSE);
      TbMakePlay(tbPlayID, FALSE);

      // update UIs
      if (uiID == tbBaseFXShowEditKeyID) {
         float data[4];

         // set editing parameters
         tbKeyframeType = KEYFRAME_VISIBILITY;
         uiP.ID(tbGameFXKeyframeBackgroundID);

         // change caption
         uiP.SetCaption(LEFT_TOP, 6, 7, "Edit Show/hide Keys", 0.0f, 0.0f, 0.0f);

         // show/update/initialize UIs (visibility, frame)
         uiC.ID(tbGameFXKeyFrameID);
         sprintf(string, "%d", (int) tbCurFrame);
         uiC.PasteComboValue(string);

         // paste all keys
         numKeys = baseFX.GetVisibilityKeyNumber();
         uiC.ClearMessages();
         for (i = 0; i < numKeys; i++) {
            baseFX.GetVisibilityKey(i, &iKey, NULL);
            sprintf(string, "%d", iKey);
            uiC.InsertMessage(i, string);
         }

         // get FX's current visibility
         data[0] = baseFX.GetVisibility();

         uiTy.ID(tbGameFXKeyDataID[0]);
         uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Show", 0.0f, 0.0f, 0.0f);
         sprintf(string, "%f", data[0]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[1]);
         uiTy.Show(FALSE);

         uiTy.ID(tbGameFXKeyDataID[2]);
         uiTy.Show(FALSE);

         uiTy.ID(tbGameFXKeyDataID[3]);
         uiTy.Show(FALSE);
      }
      else if (uiID == tbBaseFXFXPosEditKeyID) {
         float data[4];

         // set editing parameters
         tbKeyframeType = KEYFRAME_POSITION;
         uiP.ID(tbGameFXKeyframeBackgroundID);

         // change caption
         uiP.SetCaption(LEFT_TOP, 6, 7, "Edit Position Keys", 0.0f, 0.0f, 0.0f);

         // show/update/initialize UIs (position, frame)
         uiC.ID(tbGameFXKeyFrameID);
         sprintf(string, "%d", (int) tbCurFrame);
         uiC.PasteComboValue(string);

         // paste all keys
         numKeys = baseFX.GetPositionKeyNumber();
         uiC.ClearMessages();
         for (i = 0; i < numKeys; i++) {
            baseFX.GetPositionKey(i, &iKey, NULL);
            sprintf(string, "%d", iKey);
            uiC.InsertMessage(i, string);
         }

         // get FX's current position
         baseFX.GetPosition(data);

         uiTy.ID(tbGameFXKeyDataID[0]);
         uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Position", 0.0f, 0.0f, 0.0f);
         sprintf(string, "%f", data[0]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[1]);
         sprintf(string, "%f", data[1]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[2]);
         sprintf(string, "%f", data[2]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[3]);
         uiTy.Show(FALSE);

         // show Translate Gizmo
         TbSwitchGizmo(tbModelTFTranslateButtonID, TRUE);
         FyDefineHotKey(FY_SHIFT, TbShowGizmo, TRUE);
      }
      else if (uiID == tbBaseFXFXRotateEditKeyID) {
         float data[4];

         // set editing parameters
         tbKeyframeType = KEYFRAME_ROTATION;
         uiP.ID(tbGameFXKeyframeBackgroundID);

         // change caption
         uiP.SetCaption(LEFT_TOP, 6, 7, "Edit Rotation Keys", 0.0f, 0.0f, 0.0f);

         // show/update/initialize UIs (rotation, frame)
         uiC.ID(tbGameFXKeyFrameID);
         sprintf(string, "%d", (int) tbCurFrame);
         uiC.PasteComboValue(string);

         // paste all keys
         numKeys = baseFX.GetRotationKeyNumber();
         uiC.ClearMessages();
         for (i = 0; i < numKeys; i++) {
            baseFX.GetRotationKey(i, &iKey, NULL);
            sprintf(string, "%d", iKey);
            uiC.InsertMessage(i, string);
         }

         // get FX's current rotation
         baseFX.GetRotation(data);

         uiTy.ID(tbGameFXKeyDataID[0]);
         uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Rotation", 0.0f, 0.0f, 0.0f);
         sprintf(string, "%f", data[0]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[1]);
         sprintf(string, "%f", data[1]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[2]);
         sprintf(string, "%f", data[2]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[3]);
         uiTy.Show(FALSE);

         // Show Rotate Gizmo
         TbSwitchGizmo(tbModelTFRotateButtonID, TRUE);
         FyDefineHotKey(FY_SHIFT, TbShowGizmo, TRUE);
      }
      else if (uiID == tbPlateFXSizeEditKeyID) {
         if (baseFX.GetFXType() != PLATE_FX) return;
         FnPlateGameFX plateFX(fxID);
         float data[4];

         // set editing parameters
         tbKeyframeType = KEYFRAME_PLATE_SIZE;
         uiP.ID(tbGameFXKeyframeBackgroundID);

         // change caption
         uiP.SetCaption(LEFT_TOP, 6, 7, "Edit Plate Size Keys", 0.0f, 0.0f, 0.0f);

         // show/update/initialize UIs (plate size, frame)
         uiC.ID(tbGameFXKeyFrameID);
         sprintf(string, "%d", (int) tbCurFrame);
         uiC.PasteComboValue(string);

         // paste all keys
         numKeys = plateFX.GetPlateSizeKeyNumber();
         uiC.ClearMessages();
         for (i = 0; i < numKeys; i++) {
            plateFX.GetPlateSizeKey(i, &iKey, NULL);
            sprintf(string, "%d", iKey);
            uiC.InsertMessage(i, string);
         }

         // get FX's current plate size
         plateFX.GetPlateSize(data);

         uiTy.ID(tbGameFXKeyDataID[0]);
         uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Size", 0.0f, 0.0f, 0.0f);
         sprintf(string, "%f", data[0]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[1]);
         sprintf(string, "%f", data[1]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[2]);
         uiTy.Show(FALSE);

         uiTy.ID(tbGameFXKeyDataID[3]);
         uiTy.Show(FALSE);
      }
      else if (uiID == tbPlateFXColorEditKeyID) {
         if (baseFX.GetFXType() != PLATE_FX) return;
         FnPlateGameFX plateFX(fxID);
         float data[4];

         // set editing parameters
         tbKeyframeType = KEYFRAME_PLATE_COLOR;
         uiP.ID(tbGameFXKeyframeBackgroundID);

         // change caption
         uiP.SetCaption(LEFT_TOP, 6, 7, "Edit Plate Background Color Keys", 0.0f, 0.0f, 0.0f);

         // show/update/initialize UIs (plate color, frame)
         uiC.ID(tbGameFXKeyFrameID);
         sprintf(string, "%d", (int) tbCurFrame);
         uiC.PasteComboValue(string);

         // paste all keys
         numKeys = plateFX.GetPlateColorKeyNumber();
         uiC.ClearMessages();
         for (i = 0; i < numKeys; i++) {
            plateFX.GetPlateColorKey(i, &iKey, NULL);
            sprintf(string, "%d", iKey);
            uiC.InsertMessage(i, string);
         }

         // get FX's current plate color
         plateFX.GetPlateColor(data);

         uiTy.ID(tbGameFXKeyDataID[0]);
         uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Color", 0.0f, 0.0f, 0.0f);
         sprintf(string, "%f", data[0]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[1]);
         sprintf(string, "%f", data[1]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[2]);
         sprintf(string, "%f", data[2]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);

         uiTy.ID(tbGameFXKeyDataID[3]);
         sprintf(string, "%f", data[3]);
         uiTy.PasteTypeinValue(string);
         uiTy.Show(TRUE);
      }
   }
}


/*-------------------------
  modify key value command
  C.Wang 1212, 2012
 --------------------------*/
void TbModifyFXKeyCommand(UIid uiID, BOOL4 beP)
{
   if (!beP) {
      char string[256];
      sprintf(string, "%d", tbKeyframeType);

      // execute the command
      TbCommandParser("ModifyFXKey", string, FALSE);
   }
}


/*-------------------------
  remove key value command
  C.Wang 1212, 2012
 --------------------------*/
void TbRemoveFXKeyCommand(UIid uiID, BOOL4 beP)
{
   if (!beP) {
      char string[256];
      sprintf(string, "%d", tbKeyframeType);

      // execute the command
      TbCommandParser("RemoveFXKey", string, FALSE);
   }
}


/*-------------------------------
  check key range for keyframing
  C.Wang 0117, 2013
 --------------------------------*/
void TbCheckKeys(UIid uiID, char *value)
{
   TBGameFXSystem *gx;
   GAMEFXid fxID;
   FnBaseGameFX baseFX;

   // get current FX
   gx = tbCurScene->GetCurrentTBGameFXSystem();
   if (gx == NULL) return;
   fxID = gx->GetCurrentGameFX();
   if (fxID == FAILED_ID) return;
   baseFX.ID(fxID);

   int iFrame = atoi(value);
   float life = baseFX.GetLife();

   if (life <= (float) iFrame) {
      iFrame = (int)life - 1;
   }
   else if (iFrame < 0) {
      iFrame = 0;
   }

   FnUiTypein uiTy;
   FnUiCombo uiC;
   uiC.ID(tbGameFXKeyFrameID);
   char string[256];
   float v[4];

   sprintf(string, "%d", iFrame);
   uiC.PasteComboValue(string);

   gx->PlayFrame((float) iFrame);

   if (tbKeyframeType == KEYFRAME_VISIBILITY) {
      v[0] = baseFX.GetVisibility();
      sprintf(string, "%f", v[0]);
      uiTy.ID(tbGameFXKeyDataID[0]);
      uiTy.PasteTypeinValue(string);
   }
   else if (tbKeyframeType == KEYFRAME_POSITION) {
      baseFX.GetPosition(v);
      sprintf(string, "%f", v[0]);
      uiTy.ID(tbGameFXKeyDataID[0]);
      uiTy.PasteTypeinValue(string);
      sprintf(string, "%f", v[1]);
      uiTy.ID(tbGameFXKeyDataID[1]);
      uiTy.PasteTypeinValue(string);
      sprintf(string, "%f", v[2]);
      uiTy.ID(tbGameFXKeyDataID[2]);
      uiTy.PasteTypeinValue(string);
   }
   else if (tbKeyframeType == KEYFRAME_ROTATION) {
      baseFX.GetRotation(v);
      sprintf(string, "%f", v[0]);
      uiTy.ID(tbGameFXKeyDataID[0]);
      uiTy.PasteTypeinValue(string);
      sprintf(string, "%f", v[1]);
      uiTy.ID(tbGameFXKeyDataID[1]);
      uiTy.PasteTypeinValue(string);
      sprintf(string, "%f", v[2]);
      uiTy.ID(tbGameFXKeyDataID[2]);
      uiTy.PasteTypeinValue(string);
   }
   else if (tbKeyframeType == KEYFRAME_PLATE_SIZE) {
      if (baseFX.GetFXType() != PLATE_FX) return;
      FnPlateGameFX plateFX(fxID);
      plateFX.GetPlateSize(v);
      sprintf(string, "%f", v[0]);
      uiTy.ID(tbGameFXKeyDataID[0]);
      uiTy.PasteTypeinValue(string);
      sprintf(string, "%f", v[1]);
      uiTy.ID(tbGameFXKeyDataID[1]);
      uiTy.PasteTypeinValue(string);
   }
   else if (tbKeyframeType == KEYFRAME_PLATE_COLOR) {
      if (baseFX.GetFXType() != PLATE_FX) return;
      FnPlateGameFX plateFX(fxID);
      plateFX.GetPlateColor(v);
      sprintf(string, "%f", v[0]);
      uiTy.ID(tbGameFXKeyDataID[0]);
      uiTy.PasteTypeinValue(string);
      sprintf(string, "%f", v[1]);
      uiTy.ID(tbGameFXKeyDataID[1]);
      uiTy.PasteTypeinValue(string);
      sprintf(string, "%f", v[2]);
      uiTy.ID(tbGameFXKeyDataID[2]);
      uiTy.PasteTypeinValue(string);
      sprintf(string, "%f", v[3]);
      uiTy.ID(tbGameFXKeyDataID[3]);
      uiTy.PasteTypeinValue(string);
   }
}


/*--------------------------
  select current key values
  C.Wang 1217, 2012
 ---------------------------*/
void TbSelectKeys(UIid uiID, char *value)
{
   int i, iKey, iSelect, numKeys;
   char string[256];
   float v[4];
   TBGameFXSystem *gx;
   GAMEFXid fxID;
   FnBaseGameFX baseFX;
   FnUiTypein uiTy;

   // get current FX
   gx = tbCurScene->GetCurrentTBGameFXSystem();
   if (gx == NULL) return;
   fxID = gx->GetCurrentGameFX();
   if (fxID == FAILED_ID) return;
   baseFX.ID(fxID);

   iSelect = atoi(value);
   if (tbKeyframeType == KEYFRAME_VISIBILITY) {
      numKeys = baseFX.GetVisibilityKeyNumber();
      for (i = 0; i < numKeys; i++) {
         baseFX.GetVisibilityKey(i, &iKey, v);
         if (iSelect == iKey) {
            sprintf(string, "%f", v[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            goto FINISH;
         }
      }
   }
   else if (tbKeyframeType == KEYFRAME_POSITION) {
      numKeys = baseFX.GetPositionKeyNumber();
      for (i = 0; i < numKeys; i++) {
         baseFX.GetPositionKey(i, &iKey, v);
         if (iSelect == iKey) {
            sprintf(string, "%f", v[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", v[1]);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", v[2]);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.PasteTypeinValue(string);
            goto FINISH;
         }
      }
   }
   else if (tbKeyframeType == KEYFRAME_ROTATION) {
      numKeys = baseFX.GetRotationKeyNumber();
      for (i = 0; i < numKeys; i++) {
         baseFX.GetRotationKey(i, &iKey, v);
         if (iSelect == iKey) {
            sprintf(string, "%f", v[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", v[1]);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", v[2]);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.PasteTypeinValue(string);
            goto FINISH;
         }
      }
   }
   else if (tbKeyframeType == KEYFRAME_PLATE_SIZE) {
      if (baseFX.GetFXType() != PLATE_FX) return;
      FnPlateGameFX plateFX(fxID);

      numKeys = plateFX.GetPlateSizeKeyNumber();
      for (i = 0; i < numKeys; i++) {
         plateFX.GetPlateSizeKey(i, &iKey, v);
         if (iSelect == iKey) {
            sprintf(string, "%f", v[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", v[1]);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.PasteTypeinValue(string);
            goto FINISH;
         }
      }
   }
   else if (tbKeyframeType == KEYFRAME_PLATE_COLOR) {
      if (baseFX.GetFXType() != PLATE_FX) return;
      FnPlateGameFX plateFX(fxID);

      numKeys = plateFX.GetPlateColorKeyNumber();
      for (i = 0; i < numKeys; i++) {
         plateFX.GetPlateColorKey(i, &iKey, v);
         if (iSelect == iKey) {
            sprintf(string, "%f", v[0]);
            uiTy.ID(tbGameFXKeyDataID[0]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", v[1]);
            uiTy.ID(tbGameFXKeyDataID[1]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", v[2]);
            uiTy.ID(tbGameFXKeyDataID[2]);
            uiTy.PasteTypeinValue(string);
            sprintf(string, "%f", v[3]);
            uiTy.ID(tbGameFXKeyDataID[3]);
            uiTy.PasteTypeinValue(string);
            goto FINISH;
         }
      }
   }

FINISH:
   gx->PlayFrame((float) iSelect);
}


/*-----------------------
  open the quit dialogue
  C.Wang 0814, 2012
 ------------------------*/
void TbOpenQuit(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // set warning message
      FnUiPlate msg;
      msg.ID(tbWarnMessageID);
      msg.SetUIMessage(0, LEFT_TOP, 18, 40, "Quit ?", 0.5f, 0.0f, 0.0f);

      // bind the callback to the OK button
      FnUiButton okButton;
      okButton.ID(tbWarnOKID);
      okButton.BindCallback(TbQuitTool);

      // popup the dialogue
      tbWarnUI.PopUp(TRUE);
   }
}


/*------------------------
  open the "new" dialogue
  C.Wang 0814, 2012
 -------------------------*/
void TbOpenNew(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // set warning message
      FnUiPlate msg;
      msg.ID(tbWarnMessageID);
      msg.SetUIMessage(0, LEFT_TOP, 18, 40, "Do You Want to Reset the System ?", 0.5f, 0.0f, 0.0f);

      // bind the callback to the OK button
      FnUiButton okButton;
      okButton.ID(tbWarnOKID);
      okButton.BindCallback(TbNewTool);

      // popup the dialogue
      tbWarnUI.PopUp(TRUE);
   }
}


/*-------------------------------
  open the "add action" dialogue
  C.Wang 0814, 2012
 --------------------------------*/
void TbOpenAddAction(UIid uiID, BOOL4 bePress)
{
   // get current action data
   CHARACTERid aID = tbCurScene->GetCurrentCharacter();
   if (aID == FAILED_ID) return;

   FnCharacter actor;
   actor.ID(aID);

   char *bodyName = actor.GetCurrentBodyName();
   ACTIONid actionID = actor.GetCurrentAction(bodyName, 0);
   if (uiID == tbEditActionButtonID && actionID == FAILED_ID) return;

   FnAction action;
   action.ID(actionID);

   FnUiPlate uiP;
   FnUiCombo uiCType;
   FnUiCombo uiCFront, uiCEnd;
   FnUiTypein uiTyName, uiTyLen;
   uiP.ID(tbAddActionBackgroundID);
   uiCType.ID(tbAddActionTypeID);
   uiTyName.ID(tbAddActionNameID);
   uiCFront.ID(tbAddActionFrontPoseID);
   uiCEnd.ID(tbAddActionEndPoseID);
   uiTyLen.ID(tbAddActionDurationID);

   BOOL4 beOK;
   char poseName[256], string[256];
   int i, iCount, numB = actor.GetBodyPoseNumber(bodyName);
   uiCFront.ClearMessages();
   uiCEnd.ClearMessages();
   if (numB > 0) {
      iCount = 0;
      for (i = 0; i < numB; i++) {
         beOK = actor.QueryBodyPose(bodyName, i, poseName, NULL, NULL);
         if (beOK) {
            uiCFront.InsertMessage(iCount, poseName);
            uiCEnd.InsertMessage(iCount, poseName);
            iCount++;
         }
      }
   }

   if (!bePress) {
      if (uiID == tbAddActionButtonID) {
         uiP.SetCaption(LEFT_TOP, 6, 7, "Add New Action", 0.0f, 0.0f, 0.0f);
         uiCType.PasteComboValue("Simple Action");

         char newName[256];
         sprintf(newName, "NewAction%d", FyRandomInteger(0, 1000, 1001));
         uiTyName.PasteTypeinValue(newName);
         uiCFront.PasteComboValue("-");
         uiCEnd.PasteComboValue("-");
         uiTyLen.PasteTypeinValue("-");

         uiCType.Enable(TRUE);
         uiTyName.Enable(TRUE);
         uiCEnd.Enable(FALSE);
         uiTyLen.Enable(FALSE);
      }
      else {
         uiP.SetCaption(LEFT_TOP, 6, 7, "Edit Action", 0.0f, 0.0f, 0.0f);
         uiTyName.PasteTypeinValue(action.GetName());
         uiTyName.Enable(FALSE);
         
         DWORD type = action.GetType();
         if (type == CROSS_BLEND_ACTION) {
            uiCType.PasteComboValue("Cross Fade Blending");
            uiCFront.PasteComboValue(action.GetCurrentPose(0));
            uiCEnd.PasteComboValue(action.GetCurrentPose(1));
            sprintf(string, "%6.2f", action.GetLength());
            uiTyLen.PasteTypeinValue(string);

            uiCEnd.Enable(TRUE);
            uiTyLen.Enable(TRUE);
         }
         else if (type == CONNECT_ACTION) {
            uiCType.PasteComboValue("Connection");
            uiCFront.PasteComboValue(action.GetCurrentPose(0));
            uiCEnd.PasteComboValue(action.GetCurrentPose(1));
            sprintf(string, "%6.2f", action.GetLength());
            uiTyLen.PasteTypeinValue(string);

            uiCEnd.Enable(TRUE);
            uiTyLen.Enable(TRUE);
         }
         else {
            uiCType.PasteComboValue("Simple Action");
            uiCFront.PasteComboValue(action.GetCurrentPose(0));
            uiCEnd.PasteComboValue("-");
            uiTyLen.PasteTypeinValue("-");

            uiCEnd.Enable(FALSE);
            uiTyLen.Enable(FALSE);
         }
         uiCType.Enable(FALSE);
      }

      // bind the callback to the OK button
      FnUiButton okButton;
      okButton.ID(tbAddActionOKID);
      if (uiID == tbAddActionButtonID) {
         okButton.BindCallback(TbNewActionUICommand);
      }
      else {
         okButton.BindCallback(TbEditActionUICommand);
      }

      // popup the dialogue
      tbAddActionUI.PopUp(TRUE);
   }
}


/*--------------------------------
  open the "add game FX" dialogue
  C.Wang 0101, 2013
 ---------------------------------*/
void TbOpenAddGameFX(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // popup the dialogue
      tbAddGameFXUI.PopUp(TRUE);

      // initialize the dialogue
      FnUiTypein uiTy;
      FnUiCombo uiC;
      char value[256], tValue[256];
      int id;

      uiC.ID(tbAddFXTypeID);
      uiC.GetComboValue(value);

      id = FyRandomInteger(0, 9999, 10000);
      if (FyCheckWord(value, "Plate")) {
         sprintf(tValue, "PlateFX_%04d", id);

         // show/hide the associated UIs
         uiTy.ID(tbMaxParticleNumID);
         uiTy.Show(FALSE);
         uiC.ID(tbPickForceTypeID);
         uiC.Show(FALSE);
         uiTy.ID(tbPickForceMagnitudeID);
         uiTy.Show(FALSE);
      }
      else if (FyCheckWord(value, "Particles")) {
         sprintf(tValue, "ParticleFX_%04d", id);

         // show/hide the associated UIs
         uiTy.ID(tbMaxParticleNumID);
         uiTy.Show(TRUE);
         uiC.ID(tbPickForceTypeID);
         uiC.Show(FALSE);
         uiTy.ID(tbPickForceMagnitudeID);
         uiTy.Show(FALSE);
      }
      else if (FyCheckWord(value, "Geometry")) {
         sprintf(tValue, "GeometryFX_%04d", id);

         // show/hide the associated UIs
         uiTy.ID(tbMaxParticleNumID);
         uiTy.Show(FALSE);
         uiC.ID(tbPickForceTypeID);
         uiC.Show(FALSE);
         uiTy.ID(tbPickForceMagnitudeID);
         uiTy.Show(FALSE);
      }
      else if (FyCheckWord(value, "Force")) {
         sprintf(tValue, "ForceFX_%04d", id);

         // show/hide the associated UIs
         uiTy.Show(FALSE);
         uiC.ID(tbPickForceTypeID);
         uiC.Show(TRUE);
         uiTy.ID(tbPickForceMagnitudeID);
         uiTy.Show(TRUE);
      }
      else if (FyCheckWord(value, "Audio")) {
         sprintf(tValue, "AudioFX_%04d", id);

         // show/hide the associated UIs
         uiTy.ID(tbMaxParticleNumID);
         uiTy.Show(FALSE);
         uiC.ID(tbPickForceTypeID);
         uiC.Show(FALSE);
         uiTy.ID(tbPickForceMagnitudeID);
         uiTy.Show(FALSE);
      }
      else if (FyCheckWord(value, "Sword")) {
         sprintf(tValue, "SwordFX_%04d", id);

         // show/hide the associated UIs
         uiTy.ID(tbMaxParticleNumID);
         uiTy.Show(FALSE);
         uiC.ID(tbPickForceTypeID);
         uiC.Show(FALSE);
         uiTy.ID(tbPickForceMagnitudeID);
         uiTy.Show(FALSE);
      }

      uiTy.ID(tbAddFXNameID);
      uiTy.PasteTypeinValue(tValue);
      tbBe1stSetName = TRUE;
   }
}


/*---------------------------------------
  open the "add game FX System" dialogue
  C.Wang 1004, 2012
 ----------------------------------------*/
void TbOpenAddGameFXSystem(UIid uiID, BOOL4 bePress)
{
   if (!bePress) {
      // popup the dialogue
      tbAddGameFXSystemUI.PopUp(TRUE);

      // initialize the dialogue
      FnUiTypein uiTy;
      char tValue[256];
      int id;

      uiTy.ID(tbAddFXSystemNameID);
      id = FyRandomInteger(0, 9999, 10000);
      sprintf(tValue, "NewFXSystem_%04d", id);
      uiTy.PasteTypeinValue(tValue);
   }
}


/*------------------------
  select new Game FX type
  C.Wang 0917, 2014
 -------------------------*/
void TbSelectFXType(UIid uiID, char *value)
{
   int id;
   char tValue[256];
   FnUiTypein uiTy;
   FnUiCombo uiC;
   
   id = FyRandomInteger(0, 9999, 10000);
   if (FyCheckWord(value, "Plate")) {
      sprintf(tValue, "PlateFX_%04d", id);
      uiTy.ID(tbMaxParticleNumID);
      uiTy.Show(FALSE);
      uiC.ID(tbPickForceTypeID);
      uiC.Show(FALSE);
      uiTy.ID(tbPickForceMagnitudeID);
      uiTy.Show(FALSE);
   }
   else if (FyCheckWord(value, "Particles")) {
      sprintf(tValue, "ParticleFX_%04d", id);
      uiTy.ID(tbMaxParticleNumID);
      uiTy.Show(TRUE);
      uiC.ID(tbPickForceTypeID);
      uiC.Show(FALSE);
      uiTy.ID(tbPickForceMagnitudeID);
      uiTy.Show(FALSE);
   }
   else if (FyCheckWord(value, "Force")) {
      sprintf(tValue, "ForceFX_%04d", id);
      uiTy.ID(tbMaxParticleNumID);
      uiTy.Show(FALSE);
      uiC.ID(tbPickForceTypeID);
      uiC.Show(TRUE);
      uiTy.ID(tbPickForceMagnitudeID);
      uiTy.Show(TRUE);
   }
   else if (FyCheckWord(value, "Audio")) {
      sprintf(tValue, "AudioFX_%04d", id);
      uiTy.ID(tbMaxParticleNumID);
      uiTy.Show(FALSE);
      uiC.ID(tbPickForceTypeID);
      uiC.Show(FALSE);
      uiTy.ID(tbPickForceMagnitudeID);
      uiTy.Show(FALSE);
   }
   else if (FyCheckWord(value, "Sword")) {
      sprintf(tValue, "SwordFX_%04d", id);
      uiTy.ID(tbMaxParticleNumID);
      uiTy.Show(FALSE);
      uiC.ID(tbPickForceTypeID);
      uiC.Show(FALSE);
      uiTy.ID(tbPickForceMagnitudeID);
      uiTy.Show(FALSE);
   }
   else if (FyCheckWord(value, "Geometry")) {
      sprintf(tValue, "GeometryFX_%04d", id);
      uiTy.ID(tbMaxParticleNumID);
      uiTy.Show(FALSE);
      uiC.ID(tbPickForceTypeID);
      uiC.Show(FALSE);
      uiTy.ID(tbPickForceMagnitudeID);
      uiTy.Show(FALSE);
   }

   if (tbBe1stSetName) {
      uiTy.ID(tbAddFXNameID);
      uiTy.PasteTypeinValue(tValue);
   }
}


/*------------------------------------
  update Game FX UI for current frame
  C.Wang 1211, 2012
 -------------------------------------*/
void TbEnableGameFXKeyUI(BOOL4 beEnable)
{
   // check current game FX
   TBGameFXSystem *curFX;
   curFX = tbCurScene->GetCurrentTBGameFXSystem();
   if (curFX == NULL) return;

   GAMEFXid gxID = curFX->GetCurrentGameFX();
   if (gxID == FAILED_ID) return;

   int i;
   FnUiPlate p;

   for (i = 0; i <tbNumGameKeyUI; i++) {
      p.ID(tbGameKeyUI[i]);
      p.Enable(beEnable);
   }

   // paste current frame data to UIs
   float data[4];
   FnBaseGameFX baseFX;
   FnPlateGameFX plateFX;
   FnUiTypein uiTy;
   FnUiButton uiB;
   FnUiCombo  uiC;
   char string[256];

   if (beEnable) {
      baseFX.ID(gxID);

      // paste current position
      baseFX.GetPosition(data);
      uiTy.ID(tbBaseFXFXPosID[0]);
      sprintf(string, "%f", data[0]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXPosID[1]);
      sprintf(string, "%f", data[1]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXPosID[2]);
      sprintf(string, "%f", data[2]);
      uiTy.PasteTypeinValue(string);

      // paste current orientation
      baseFX.GetRotation(data);
      uiTy.ID(tbBaseFXFXRotateID[0]);
      sprintf(string, "%f", data[0]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXRotateID[1]);
      sprintf(string, "%f", data[1]);
      uiTy.PasteTypeinValue(string);
      uiTy.ID(tbBaseFXFXRotateID[2]);
      sprintf(string, "%f", data[2]);
      uiTy.PasteTypeinValue(string);

      // paste current show data
      data[0] = baseFX.GetVisibility();
      uiTy.ID(tbBaseFXShowID);
      sprintf(string, "%f3.1", data[0]);
      uiTy.PasteTypeinValue(string);

      // for a plate FX
      if (baseFX.GetFXType() == PLATE_FX) {
         // paste current size
         plateFX.ID(gxID);
         plateFX.GetPlateSize(data);
         uiTy.ID(tbPlateFXSizeID[0]);
         sprintf(string, "%f", data[0]);
         uiTy.PasteTypeinValue(string);
         uiTy.ID(tbPlateFXSizeID[1]);
         sprintf(string, "%f", data[1]);
         uiTy.PasteTypeinValue(string);

         // paste current background color
         plateFX.GetPlateColor(data);
         uiTy.ID(tbPlateFXColorID[0]);
         sprintf(string, "%f", data[0]);
         uiTy.PasteTypeinValue(string);
         uiTy.ID(tbPlateFXColorID[1]);
         sprintf(string, "%f", data[1]);
         uiTy.PasteTypeinValue(string);
         uiTy.ID(tbPlateFXColorID[2]);
         sprintf(string, "%f", data[2]);
         uiTy.PasteTypeinValue(string);
         uiTy.ID(tbPlateFXColorID[3]);
         sprintf(string, "%f", data[3]);
         uiTy.PasteTypeinValue(string);
      }
   }
   DWORD type = baseFX.GetFXType();
   if(type == AUDIO_FX) {              // for a audio fx
      uiTy.ID(tbBaseFXFXPosID[0]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbBaseFXFXPosID[1]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbBaseFXFXPosID[2]);
      uiTy.Enable(FALSE);
      uiB.ID(tbBaseFXFXPosEditKeyID);
      uiB.Enable(FALSE);

      uiTy.ID(tbBaseFXFXRotateID[0]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbBaseFXFXRotateID[1]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbBaseFXFXRotateID[2]);
      uiTy.Enable(FALSE);
      uiB.ID(tbBaseFXFXRotateEditKeyID);
      uiB.Enable(FALSE);

      uiTy.ID(tbBaseFXShowID);
      uiTy.Enable(FALSE);
      uiB.ID(tbBaseFXShowEditKeyID);
      uiB.Enable(FALSE);

      uiTy.ID(tbBaseLifeID);
      uiTy.Enable(FALSE);

      uiC.ID(tbSetFXParentID);
      uiC.Enable(FALSE);
   }
   else if(type == SWORD_FX) {         // for a sword fx
      uiTy.ID(tbBaseFXFXPosID[0]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbBaseFXFXPosID[1]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbBaseFXFXPosID[2]);
      uiTy.Enable(FALSE);
      uiB.ID(tbBaseFXFXPosEditKeyID);
      uiB.Enable(FALSE);

      uiTy.ID(tbBaseFXFXRotateID[0]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbBaseFXFXRotateID[1]);
      uiTy.Enable(FALSE);
      uiTy.ID(tbBaseFXFXRotateID[2]);
      uiTy.Enable(FALSE);
      uiB.ID(tbBaseFXFXRotateEditKeyID);
      uiB.Enable(FALSE);

      uiTy.ID(tbBaseFXShowID);
      uiTy.Enable(FALSE);
      uiB.ID(tbBaseFXShowEditKeyID);
      uiB.Enable(FALSE);

      uiTy.ID(tbBaseLifeID);
      uiTy.Enable(FALSE);

      uiC.ID(tbSetFXParentID);
      uiC.Enable(FALSE);
   }
}

/*------------------------------------
  enable/disable Mateiral UI of Model
  Eric Fei 0124, 2013
 -------------------------------------*/
void TbEnableModelMaterialUI(BOOL4 beEnable)
{
   // Enable/Disable Material UI
   FnUiPlate ui;
   for (int i = 0; i < tbNumMatUI; i++) {
      ui.ID(tbMatUI[i]);
      ui.Enable(beEnable);
   }

   // Enable/Disiable Material Update UI
   FnUiButton uiB;
   uiB.ID(tbUpdateMaterialID);
   if (beEnable) {
      uiB.Enable(tbModelUpdateState);
   }
   else {
      tbModelUpdateState = uiB.IsAble();
      uiB.Enable(FALSE);
   }
}


/*---------------------------------
  check game FX name in the typein
  C.Wang 1001, 2012
 ----------------------------------*/
void TbCheckTypeinGameFXName(UIid uiID, char *value)
{
   tbBe1stSetName = FALSE;
}


/*---------------------------------
  check game FX life in the typein
  C.Wang 1027, 2012
 ----------------------------------*/
void TbCheckLifeDataInput(UIid uiID, char *value)
{
   if (!TbCheckInt(value)) {
      FnUiTypein uiTy(uiID);
      char value[256];
      sprintf(value, "%d", tbDefFXLife);
      uiTy.PasteTypeinValue(value);
   }
}


/*-------------------------------------------------------------
  check the value of maximum number of particles in the typein
  C.Wang 1223, 2012
 --------------------------------------------------------------*/
void TbCheckMaxParticlesInput(UIid uiID, char *value)
{
   FnUiTypein uiTy(uiID);

   if (!TbCheckInt(value)) {
      sprintf(value, "%d", tbCurMaxParticles);
      uiTy.PasteTypeinValue(value);
   }
   else {
      int num = atoi(value);
      if (num < 0 || num > MAXIMUM_PARTICLES) {
         sprintf(value, "%d", tbCurMaxParticles);
         uiTy.PasteTypeinValue(value);
      }
   }
}


/*------------------
  set current frame
  C.Wang 1203, 2012
 -------------------*/
void TbGotoCurrentFrame(UIid uiID, char *value)
{
   // update current frame
   float v;

   if (TbCheckReal(value)) {
      v = (float)atof(value);
      tbCurFrame = v;

      FnUiTypein uiTy(uiID);
      char msg[256];
      sprintf(msg, "%8.1f", tbCurFrame);
      uiTy.PasteTypeinValue(msg);

      // update the scene to current frame
      tbCurScene->SetCurrentFrame(tbCurFrame, tbBeBase);
   }
}


/*------------------
  select play mode
  C.Wang 1217, 2012
 -------------------*/
void TbSelectPlayMode(UIid uiID, char *value)
{
   if (FyCheckWord(value, "ONCE")) {
      tbPlayMode = ONCE;
   }
   else {
      tbPlayMode = LOOP;
   }
}


/*----------------------------------
  switch between play or pause mode
  C.Wang 0103, 2013
 -----------------------------------*/
void TbMakePlay(UIid uiID, BOOL4 bePress)
{
   tbBePlay = bePress;
   tbBePause = !bePress;
   if (tbBePlay) {
      FnUiRadioButton uiRB;
      uiRB.ID(tbPlayPoseButtonID);
      uiRB.ChangeState(FALSE);
      tbBePlayPose = FALSE;
      uiRB.Enable(FALSE);

      if (tbCurFrame <= 0.0f) {
         tbCurScene->ResetPlay(tbBeBase);
      }

      // keep playing
      //tbCurScene->BePlay(TRUE);
   }
   else {
      FnUiRadioButton uiRB;
      uiRB.ID(tbPlayPoseButtonID);
      uiRB.Enable(TRUE);

      // pause the playing
      //tbCurScene->BePlay(FALSE);
   }

   // Update M12 to Transform UIs
   TbUpdateTransformUIData(!tbBePlay);

   // update Game FX UI according to the tbBePlay value
   TbEnableGameFXKeyUI(!tbBePlay);

   // disable Material UI of Model
   if (tbBePlay) {
      TbEnableModelMaterialUI(FALSE);
   }
}


/*------------------
  stop the playing
  C.Wang 0814, 2012
 -------------------*/
void TbStopPlay(UIid uiID, BOOL4 bePress)
{
   // force to stop the playing
   tbBePlay = FALSE;
   tbBePause = FALSE;
   tbCurFrame = 0.0f;

   FnUiTypein uiTy(tbCurFrameID);
   char msg[256];
   sprintf(msg, "%8.1f", tbCurFrame);
   uiTy.PasteTypeinValue(msg);

   // update the associated UIs
   FnUiRadioButton uiRB;
   uiRB.ID(tbPlayID);
   uiRB.ChangeState(FALSE);

   // reset all characters' pose & game FXs
   tbCurScene->ResetPlay(tbBeBase);

   uiRB.ID(tbPlayPoseButtonID);
   uiRB.Enable(TRUE);
   
   // Update M12 to Transform UIs
   TbUpdateTransformUIData(!tbBePlay);

   // update Game FX UI according to the tbBePlay value
   TbEnableGameFXKeyUI(!tbBePlay);
   
   // enable Material UI of Model
   if (bePress) {
      TbEnableModelMaterialUI(TRUE);
   }

   // stop the facial expression too
}


/*------------------------
  enter working directory
  C.Wang 0814, 2012
 -------------------------*/
void TbSetDir(UIid uiID, char *value)
{
   // prepare the command
   char argument[256];

   if (uiID == tbWorkDirTypeinUI) {
      sprintf(argument, "-work %s", value);
   }
   else if (uiID == tbShaderDirTypeinUI) {
      sprintf(argument, "-shader %s", value);
   }
   else if (uiID == tbModelDirTypeinUI) {
      sprintf(argument, "-object %s", value);
   }
   else if (uiID == tbCharacterDirTypeinUI) {
      sprintf(argument, "-character %s", value);
   }
   else if (uiID == tbTextureDirTypeinUI) {
      sprintf(argument, "-texture %s", value);
   }

   // execute the command
   TbCommandParser("SetPath", argument, FALSE);
}


/*------------------
  set play speed
  C.Wang 0814, 2012
 -------------------*/
void TbPlaySpeedUICommand(UIid uiID, char *value)
{
   // execute the command
   TbCommandParser("Speed", value, FALSE);
}


/*-------------------------------------
  recalculate the UI position and size
  C.Wang 0814, 2012
 --------------------------------------*/
void TbViewportLayout(int width, int height)
{
   int w, h;
   FyQueryWindowFrameSize(&w, &h);

   // top menu bar
   tbMenuBarOX = 0;
   tbMenuBarOY = 0;
   tbMenuBarWidth = width;

   // main menu
   //tbMenuOX = tbSideMenuWidth;
   tbMenuOX = 0;
   tbMenuOY = height - tbMenuHeight;
   tbMenuWidth = width - tbSideMenuWidth;

   // work space template
   //tbBottomVPOX = tbSideMenuWidth;
   tbBottomVPOX = 0;
   tbBottomVPOY = tbMenuBarHeight;
   tbBottomVPWidth = width - tbSideMenuWidth;
   tbBottomVPHeight = height - tbMenuBarHeight - tbMenuHeight;

   // work space
   tbWorkSpaceVPWidth = tbBottomVPWidth - 30;
   tbWorkSpaceVPHeight = tbBottomVPHeight - 20;
   tbWorkSpaceVPOX = (tbBottomVPWidth - tbWorkSpaceVPWidth)/2;
   tbWorkSpaceVPOY = tbMenuBarHeight + (tbBottomVPHeight - tbWorkSpaceVPHeight)/2;

   // side menu
   //tbSideMenuOX = 0;
   tbSideMenuOX = width - tbSideMenuWidth;
   tbSideMenuOY = tbMenuBarHeight;
   tbSideMenuHeight = height - tbMenuBarHeight;
}


/*--------------------------
  twidde ON/OFF SSAO Effect
  C.Wang, 0610, 2013
 -----------------------------------*/
void TbTwiddleSSAOEffect(UIid uiID, BOOL4 bePressed)
{
   tbBeSSAO = bePressed;
}


/*-------------------------
  twidde ON/OFF HDR Effect
  C.Wang, 0611, 2013
 ----------------------------------*/
void TbTwiddleHDREffect(UIid uiID, BOOL4 bePressed)
{
   tbBeHDR = bePressed;
}


/*------------------------------------------
  set sample range in pixel for SSAO effect
  C.Wang, 0610, 2013
 -------------------------------------------*/
void TbSetupSSAOSampleRange(UIid uiID, char *value)
{
   if (TbCheckReal(value)) {
      tbSSAOSampleRange = (float) atof(value);
      if (tbSSAOSampleRange < 0.0f) tbSSAOSampleRange = 18.0f;
      if (tbSSAOSampleRange > 200.0f) tbSSAOSampleRange = 18.0f;

      tbWorkSpaceVP.SetupSSAOEffect(tbSSAOSampleRange, tbSSAODistance);
   }
}


/*-----------------------------------
  set distance in 3D for SSAO effect
  C.Wang, 0610, 2013
 ------------------------------------*/
void TbSetupSSAODistance(UIid uiID, char *value)
{
   if (TbCheckReal(value)) {
      tbSSAODistance = (float) atof(value);
      if (tbSSAODistance < 0.0f) tbSSAODistance = 6.0f;
      if (tbSSAODistance > 10000.0f) tbSSAODistance = 6.0f;

      tbWorkSpaceVP.SetupSSAOEffect(tbSSAOSampleRange, tbSSAODistance);
   }
}


/*----------------------------------
  manage the menu bar radio buttons
  C.Wang, 0116, 2014
 -----------------------------------*/
void TbSwitchMenuBarRadios(UIid uiID, BOOL4 bePressed)
{
   FnUiPlate plate;

   if (bePressed) {
      if (uiID == tbSystemButtonID) {
         // hide all not used UIs
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(TRUE);
         tbTool = SYSTEM_TOOL;

         // Disable Show Gizmo function
         FyDefineHotKey(FY_SHIFT, NULL);
      }
	   else if (uiID == tbScriptSystemButtonID) {
         // hide all not used UIs
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(TRUE);
         tbTool = SYSTEM_TOOL;

         // Disable Show Gizmo function
         FyDefineHotKey(FY_SHIFT, NULL);
	   }
	   else if (uiID == tbModelSystemButtonID) {
         // hide all not used UIs
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(TRUE);
         tbTool = MODEL_TOOL;

         // Enable Show Gizmo function
         FyDefineHotKey(FY_SHIFT, TbShowGizmo, TRUE);
	   }
      else if (uiID == tbCharacterSystemButtonID) {
         // hide all not used UIs
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(TRUE);
         tbTool = CHARACTER_TOOL;

         // Disable Show Gizmo function
         FyDefineHotKey(FY_SHIFT, NULL);
      }
      else if (uiID == tbFXSystemButtonID) {
         // hide all not used UIs
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(TRUE);
         tbTool = FX_TOOL;

         // Disable Show Gizmo function
         FyDefineHotKey(FY_SHIFT, NULL);

         // update the UIs for current FX systems/FXs
         TbUpdateCurrentFXUIs();
      }
      else if (uiID == tbEnvironmentSystemButtonID) {
         // hide all not used UIs
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(TRUE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(TRUE);
         tbTool = SYSTEM_TOOL;

         // Disable Show Gizmo function
         FyDefineHotKey(FY_SHIFT, NULL);
      }
      else if (uiID == tbPostProcessingSystemButtonID) {
         // hide all not used UIs
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(TRUE);
         tbTool = SYSTEM_TOOL;

         // Disable Show Gizmo function
         FyDefineHotKey(FY_SHIFT, NULL);
      }
      else if (uiID == tbFaceToolButtonID) {
         // hide all not used UIs
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(TRUE);
         tbTool = FACE_TOOL;

         // Disable Show Gizmo function
         FyDefineHotKey(FY_SHIFT, NULL);
      }

      plate.ID(tbSideMenuPlateID);
      plate.Show(FALSE);

      FnUiRadioButton button;
      button.ID(tbOrthoButtonID);
      if (tbTool == FACE_TOOL) {
         TbInitFaceTool(FY_NONE);
      }
      else {
         button.Enable(FALSE);
      }

      // switch to 3D view
      button.ChangeState(TRUE);
      TbShowOrthoView(tbOrthoButtonID, TRUE);
   }
   else {
      // hide all associated UIs
      if (uiID == tbSystemButtonID) {
         plate.ID(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbScriptSystemButtonID) {
         plate.ID(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbModelSystemButtonID) {
         plate.ID(tbSideMenuModelPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbCharacterSystemButtonID) {
         plate.ID(tbSideMenuCharacterPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbFXSystemButtonID) {
         plate.ID(tbSideMenuFXPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbEnvironmentSystemButtonID) {
         plate.ID(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.ID(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
      }
      else if (uiID == tbPostProcessingSystemButtonID) {
         plate.ID(tbSideMenuPostProcessingPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbFaceToolButtonID) {
         plate.ID(tbSideMenuFaceToolPlateID);
         plate.Show(FALSE);
      }

      // show empty plate
      plate.ID(tbSideMenuPlateID);
      plate.Show(TRUE);
      tbTool = SYSTEM_TOOL;

      FnUiRadioButton button;
      button.ID(tbOrthoButtonID);
      button.ChangeState(TRUE);
      button.Enable(FALSE);

      // switch to 3D view
      TbShowOrthoView(tbOrthoButtonID, TRUE);

      // Disable Show Gizmo function
      FyDefineHotKey(FY_SHIFT, NULL);
   }
}


/*-------------------
  hit current object
  C.Wang 1210, 2012
 --------------------*/
void TbHitCurrentObject(int x, int y)
{
   int numO, numH;
   OBJECTid *objList, *bufObj, hitObj;

   // get all objects from current scene
   OBJECTid oID;
   float xyz[3], xyzM[3], minDist, dist, pos[3];
   int i, num = tbCurScene->GetObjectNumber();
   if (num == 0) return;

   objList = (OBJECTid *)FyAllocateBuffer(sizeof(OBJECTid)*num);
   bufObj = (OBJECTid *)FyAllocateBuffer(sizeof(OBJECTid)*num);
   for (i = 0, numO = 0; i < num; i++) {
      objList[i] = FAILED_ID;
      bufObj[i] = FAILED_ID;
      oID = tbCurScene->GetObjectID(i);
      if (oID != FAILED_ID) {
         objList[numO] = oID;
         numO++;
      }
   }

   // hit test
   numH = 0;
   tbWorkSpaceVP.HitObject(objList, numO, tbRenderCamID, x, y, bufObj, &numH, TRUE);

   hitObj = FAILED_ID;
   minDist = 10000000.0f;
   tbCamera.GetPosition(pos, TRUE);
   for (i = 0; i < numH; i++) {
      if (tbWorkSpaceVP.HitPosition(bufObj[i], tbRenderCamID, x, y, xyz)) {
         // find the distance to camera
         dist = FyDistance(xyz, pos);
         if (dist < minDist) {
            hitObj = bufObj[i];
            minDist = dist;
            xyzM[0] = xyz[0];
            xyzM[1] = xyz[1];
            xyzM[2] = xyz[2];
         }
      }
   }

   // check if we hit something and make it as current object
   if (hitObj != FAILED_ID) {
      FnObject model;
      tbCurScene->SetCurrentObject(hitObj);
   }

   // release the buffers
   FyReleaseBuffer(objList);
   FyReleaseBuffer(bufObj);
}


/*--------------------
  hit gizmo object
  Eric Fei 1219, 2012
 ---------------------*/
void TbHitGizmo(int x, int y, BOOL4 beChoose)
{
   // x: Mouse X in Screen Coord.
   // y: Moure Y in Screen Coord.
   // beChoose: true  => sotre Current Gizmo Line       (in Mouse Press)
   //           false => don't store Current Gizmo Line (at Mouse Move)

   BOOL4 behit = FALSE;                      // Hit to end
   float cpOX = float(x - tbWorkSpaceVPOX);  // Local X of Click-Point
   float cpOY = float(y - tbWorkSpaceVPOY);  // Local Y of Click-Point
   float pixelDis = 3.0f;                    // Show Choose Distance
   float slop[3];                            // beHit Line Slop
   
   // store Backup M12
   if (beChoose) {
      FnObject curObj(TbGetCurrentBaseObjID());
      curObj.GetMatrix(tbGizmoBackupM12);
   }

   if (tbGizmoTranslate.isShow) {
      // Hit Translate Gizmo (For XYZ-axis currently)
      float      dis;                                       // distance between Click Point & Line
      FnMaterial gizmoMat;                                  // Access material of Gizmo

      // Initialization
      tbGizmoTranslate.iCurGizmo  = -1;
      tbGizmoTranslate.curSlop[0] = 0.0f;
      tbGizmoTranslate.curSlop[1] = 0.0f;
      gizmoMat.ID(tbGizmoTranslate.gizmoMIDs[X_AXIS*2]);
      gizmoMat.SetEmissive(tbGizmoTranslate.colorX);
      gizmoMat.ID(tbGizmoTranslate.gizmoMIDs[Y_AXIS*2]);
      gizmoMat.SetEmissive(tbGizmoTranslate.colorY);
      gizmoMat.ID(tbGizmoTranslate.gizmoMIDs[Z_AXIS*2]);
      gizmoMat.SetEmissive(tbGizmoTranslate.colorZ);
      
      // hit X-axis
      TbHitGizmoEle(cpOX, cpOY, &tbGizmoTranslate, X_AXIS, &dis, slop);
      if (dis < pixelDis) {
         // Choosed
         gizmoMat.ID(tbGizmoTranslate.gizmoMIDs[X_AXIS*2]);
         gizmoMat.SetEmissive(tbGizmoTranslate.colorChoose);
         behit = TRUE;
         if (beChoose) {
            tbGizmoTranslate.iCurGizmo = X_AXIS;
            tbGizmoTranslate.curSlop[0] = slop[0];
            tbGizmoTranslate.curSlop[1] = slop[1];
         }
      }

      // hit Y-axis
      if (!behit) {
         TbHitGizmoEle(cpOX, cpOY, &tbGizmoTranslate, Y_AXIS, &dis, slop);
         if (dis < pixelDis) {
            // Choosed
            gizmoMat.ID(tbGizmoTranslate.gizmoMIDs[Y_AXIS*2]);
            gizmoMat.SetEmissive(tbGizmoTranslate.colorChoose);
            behit = TRUE;
            if (beChoose) {
               tbGizmoTranslate.iCurGizmo = Y_AXIS;
               tbGizmoTranslate.curSlop[0] = slop[0];
               tbGizmoTranslate.curSlop[1] = slop[1];
            }
         }
      }

      // hit Z-axis
      if (!behit) {
         TbHitGizmoEle(cpOX, cpOY, &tbGizmoTranslate, Z_AXIS, &dis, slop);
         if (dis < pixelDis) {
            // Choosed
            gizmoMat.ID(tbGizmoTranslate.gizmoMIDs[Z_AXIS*2]);
            gizmoMat.SetEmissive(tbGizmoTranslate.colorChoose);
            behit = TRUE;
            if (beChoose) {
               tbGizmoTranslate.iCurGizmo = Z_AXIS;
               tbGizmoTranslate.curSlop[0] = slop[0];
               tbGizmoTranslate.curSlop[1] = slop[1];
            }
         }
      }
   }
   else if(tbGizmoRotate.isShow) {
      // Hit Rotate Gizmo
      float      dis;                                       // distance between Click Point & Line
      FnMaterial gizmoMat;                                  // Access material of Gizmo

      // Initialization
      tbGizmoRotate.iCurGizmo  = -1;
      tbGizmoRotate.curSlop[0] = 0.0f;
      tbGizmoRotate.curSlop[1] = 0.0f;
      gizmoMat.ID(tbGizmoRotate.gizmoMIDs[X_AXIS*2]);
      gizmoMat.SetEmissive(tbGizmoRotate.colorX);
      gizmoMat.ID(tbGizmoRotate.gizmoMIDs[Y_AXIS*2]);
      gizmoMat.SetEmissive(tbGizmoRotate.colorY);
      gizmoMat.ID(tbGizmoRotate.gizmoMIDs[Z_AXIS*2]);
      gizmoMat.SetEmissive(tbGizmoRotate.colorZ);
      
      // hit X-cricle
      TbHitGizmoEle(cpOX, cpOY, &tbGizmoRotate, X_AXIS, &dis, slop);
      if (dis < pixelDis) {
         // Choosed
         gizmoMat.ID(tbGizmoRotate.gizmoMIDs[X_AXIS*2]);
         gizmoMat.SetEmissive(tbGizmoRotate.colorChoose);
         behit = TRUE;
         if (beChoose) {
            tbGizmoRotate.iCurGizmo = X_AXIS;
            tbGizmoRotate.curSlop[0] = slop[0];
            tbGizmoRotate.curSlop[1] = slop[1];
         }
      }
      
      // hit Y-cricle
      TbHitGizmoEle(cpOX, cpOY, &tbGizmoRotate, Y_AXIS, &dis, slop);
      if (!behit && dis < pixelDis) {
         // Choosed
         gizmoMat.ID(tbGizmoRotate.gizmoMIDs[Y_AXIS*2]);
         gizmoMat.SetEmissive(tbGizmoRotate.colorChoose);
         behit = TRUE;
         if (beChoose) {
            tbGizmoRotate.iCurGizmo = Y_AXIS;
            tbGizmoRotate.curSlop[0] = slop[0];
            tbGizmoRotate.curSlop[1] = slop[1];
         }
      }
      
      // hit Z-cricle
      TbHitGizmoEle(cpOX, cpOY, &tbGizmoRotate, Z_AXIS, &dis, slop);
      if (!behit && dis < pixelDis) {
         // Choosed
         gizmoMat.ID(tbGizmoRotate.gizmoMIDs[Z_AXIS*2]);
         gizmoMat.SetEmissive(tbGizmoRotate.colorChoose);
         behit = TRUE;
         if (beChoose) {
            tbGizmoRotate.iCurGizmo = Z_AXIS;
            tbGizmoRotate.curSlop[0] = slop[0];
            tbGizmoRotate.curSlop[1] = slop[1];
         }
      }
   }
}


/*-------------------------
  hit one element of Gizmo
  Eric Fei 1222, 2012
 --------------------------*/
void TbHitGizmoEle(float x, float y, TBGizmo *gizmo, int idxEle, float *dis, float *slop)
{
   // [in] x:      X of Click Point
   // [in] y:      Y of Click Point
   // [in] gizmo:  Gizmo Object
   // [in] idxEle: Gizmo Element Type (X_AXIS, Y_AXIS, Z_AXIS, XY_PLANE, YZ_PLANE, ZX_PLANE)
   // [out] dis:   pixel distance between Click Point & Gizmo Element
   //              >=0 is Hit Successed
   //              <0  is Hit Failed
   // [out] slop:  dx, dy of line in Screen Coord.

   // Initialization
   *dis = 100.0f;
   
   float basePos[3];                         // Current Gizmo Base Object Position
   FnObject curObj(gizmo->baseID);           // Current Gizmo Base Object
   curObj.GetPosition(basePos);
   
   float line2D[6];                          // 2 positions of line in 2D
   float line3D[6];                          // 2 positions of line in 3D
   float minDis = 100.0f;                    // used to compare distance

   if (gizmo->typeGizmoObjV[idxEle] == LINE_SEGMENTS) {
      for (int i = gizmo->idxGizmoObjV[idxEle], j = 0; j < gizmo->numGizmoObjV[idxEle] - 1; i += 2*3, j++) {
         // get line in 3D
         line3D[0] = basePos[0] + gizmo->gizmoObjVex[i];
         line3D[1] = basePos[1] + gizmo->gizmoObjVex[i + 1];
         line3D[2] = basePos[2] + gizmo->gizmoObjVex[i + 2];
         line3D[3] = basePos[0] + gizmo->gizmoObjVex[i + 3];
         line3D[4] = basePos[1] + gizmo->gizmoObjVex[i + 4];
         line3D[5] = basePos[2] + gizmo->gizmoObjVex[i + 5];
         
         // line-end position from 3D to 2D
         tbWorkSpaceVP.ComputeScreenPosition(tbRenderCamID, line2D, line3D, PHYSICAL_SCREEN, FALSE);
         tbWorkSpaceVP.ComputeScreenPosition(tbRenderCamID, &line2D[3], &line3D[3], PHYSICAL_SCREEN, FALSE);
         line2D[2] = line2D[5] = 0.0f;

         // hit line
         TbHitLine(x, y, line2D, dis);

         // choose the smallest distance
         if (minDis > *dis) {
            minDis = *dis;

            // store slop of this line
            slop[0] = line2D[3] - line2D[0];
            slop[1] = line2D[4] - line2D[1];
            slop[2] = 0.0f;
            FyNormalizeVector3(slop);
         }
      }
      *dis   = minDis;
   }
   else if (gizmo->typeGizmoObjV[idxEle] == CLOSE_POLYLINE) {
      int i, j;
      for (i = gizmo->idxGizmoObjV[idxEle], j = 0; j < gizmo->numGizmoObjV[idxEle] - 1; i += 1*3, j++) {
         // get line in 3D
         line3D[0] = basePos[0] + gizmo->gizmoObjVex[i];
         line3D[1] = basePos[1] + gizmo->gizmoObjVex[i + 1];
         line3D[2] = basePos[2] + gizmo->gizmoObjVex[i + 2];
         line3D[3] = basePos[0] + gizmo->gizmoObjVex[i + 3];
         line3D[4] = basePos[1] + gizmo->gizmoObjVex[i + 4];
         line3D[5] = basePos[2] + gizmo->gizmoObjVex[i + 5];
         
         // line-end position from 3D to 2D
         tbWorkSpaceVP.ComputeScreenPosition(tbRenderCamID, line2D, line3D, PHYSICAL_SCREEN, FALSE);
         tbWorkSpaceVP.ComputeScreenPosition(tbRenderCamID, &line2D[3], &line3D[3], PHYSICAL_SCREEN, FALSE);
         line2D[2] = line2D[5] = 0.0f;

         // hit line
         TbHitLine(x, y, line2D, dis);

         // choose the smallest distance
         if (*dis >= 0 && minDis > *dis) {
            minDis = *dis;

            // store slop of this line
            slop[0] = line2D[3] - line2D[0];
            slop[1] = line2D[4] - line2D[1];
            slop[2] = 0.0f;
            FyNormalizeVector3(slop);
         }
      }

      // for the Last Line :
      // get line in 3D
      line3D[0] = basePos[0] + gizmo->gizmoObjVex[i];
      line3D[1] = basePos[1] + gizmo->gizmoObjVex[i + 1];
      line3D[2] = basePos[2] + gizmo->gizmoObjVex[i + 2];
      line3D[3] = basePos[0] + gizmo->gizmoObjVex[gizmo->idxGizmoObjV[idxEle]];
      line3D[4] = basePos[1] + gizmo->gizmoObjVex[gizmo->idxGizmoObjV[idxEle] + 1];
      line3D[5] = basePos[2] + gizmo->gizmoObjVex[gizmo->idxGizmoObjV[idxEle] + 2];
         
      // line-end position from 3D to 2D
      tbWorkSpaceVP.ComputeScreenPosition(tbRenderCamID, line2D, line3D, PHYSICAL_SCREEN, FALSE);
      tbWorkSpaceVP.ComputeScreenPosition(tbRenderCamID, &line2D[3], &line3D[3], PHYSICAL_SCREEN, FALSE);
      line2D[2] = line2D[5] = 0.0f;

      // hit line
      TbHitLine(x, y, line2D, dis);

      // choose the smallest distance
      if (*dis >= 0 && minDis > *dis) {
         minDis = *dis;

         // store slop of this line
         slop[0] = line2D[3] - line2D[0];
         slop[1] = line2D[4] - line2D[1];
         slop[2] = 0.0f;
         FyNormalizeVector3(slop);
      }
      *dis = minDis;
   }
}


/*--------------------
  hit 3D line
  Eric Fei 1221, 2012
 ---------------------*/
void TbHitLine(float x, float y, float *line2D, float *dis)
{
   // [in] x:      X of Click Point
   // [in] y:      Y of Click Point
   // [in] line2D[]: line2D[0~2] is 1st point of Line in 2D
   //                line2D[3~5] is 2nd point of Line in 2D
   // [out] dis:   pixel distance between Click Point & line
   
   float lineVec[3];                      // vector of line
   float cpRay[6];                        // 2 positions of cpVec[] vector
   float cpVec[3];                        // vector from Click Point, and perpendicular to line
   float upVec[3];                        // up vector
   float tLine;                           // Intersection point parameter of line vector
   float intersectionPos[3];              // Intersection point in 2D

   // get lineVec[] vector
   lineVec[0] = line2D[3] - line2D[0];
   lineVec[1] = line2D[4] - line2D[1];
   lineVec[2] = 0.0f;

   // get upVec[] vector
   upVec[0] = 0.0f;
   upVec[1] = 0.0f;
   upVec[2] = -1.0f;

   // get vector which is perpendicular to line
   FyCross(cpVec, lineVec, upVec);

   // get cpRay[] Ray
   cpRay[0] = x;
   cpRay[1] = y;
   cpRay[2] = 0.0f;
   cpRay[3] = x + cpVec[0];
   cpRay[4] = y + cpVec[1];
   cpRay[5] = 0.0f;

   // calculate intersection point
   tLine = ((cpRay[0] - line2D[0])*(cpRay[4] - cpRay[1]) + (cpRay[3] - cpRay[0])*(line2D[1] - cpRay[1]))/
           ((line2D[3] - line2D[0])*(cpRay[4] - cpRay[1]) - (cpRay[3] - cpRay[0])*(line2D[4] - line2D[1]));
   if (tLine < -0.1f || tLine > 1.0f) return;                            // out-side of line
   intersectionPos[0] = line2D[0] + (line2D[3] - line2D[0])*tLine;
   intersectionPos[1] = line2D[1] + (line2D[4] - line2D[1])*tLine;
   intersectionPos[2] = 0.0f;

   // get distance
   *dis = FyDistance(cpRay, intersectionPos);
}


/*--------------------
  Drag Gizmo by Mouse
  Eric Fei 1222, 2012
 ---------------------*/
void TbDragGizmo(int x, int y)
{
   // [in] x:      X of Drag Point
   // [in] y:      Y of Drag Point

   // get current object
   FnObject curObj(TbGetCurrentBaseObjID());
   if (curObj.ID() == FAILED_ID) return;

   if (tbGizmoTranslate.isShow) {
      // get camera distance from current object
      float curObjPos[3], curCameraPos[3], disCamera;
      curObj.GetPosition(curObjPos);
      tbCamera.GetPosition(curCameraPos);
      disCamera = FyDistance(curObjPos, curCameraPos)*0.0005f;

      // move current object
      switch(tbGizmoTranslate.iCurGizmo)
      {
      case X_AXIS:
         {
            tbGizmoTranslate.beDrag = TRUE;
            curObj.Translate((x-tbOldX)*tbGizmoTranslate.curSlop[0]*disCamera +
                             (y-tbOldY)*tbGizmoTranslate.curSlop[1]*disCamera,
                             0.0f, 0.0f, GLOBAL);
         }
         break;

      case Y_AXIS:
         {
            tbGizmoTranslate.beDrag = TRUE;
            curObj.Translate(0.0f,
                             (x-tbOldX)*tbGizmoTranslate.curSlop[0]*disCamera +
                             (y-tbOldY)*tbGizmoTranslate.curSlop[1]*disCamera,
                             0.0f, GLOBAL);
         }
         break;

      case Z_AXIS:
         {
            tbGizmoTranslate.beDrag = TRUE;
            curObj.Translate(0.0f, 0.0f,
                             (x-tbOldX)*tbGizmoTranslate.curSlop[0]*disCamera +
                             (y-tbOldY)*tbGizmoTranslate.curSlop[1]*disCamera,
                             GLOBAL);
         }
         break;

      default:
         tbGizmoTranslate.beDrag = FALSE;
         return;
      }
   }
   else if(tbGizmoRotate.isShow) {
      // transform World Origin/Current position
      float worldOPos[3], curObjPos[3];
      worldOPos[0] = worldOPos[1] = worldOPos[2] = 0.0f;
      curObj.GetPosition(curObjPos);
      
      // rotate current object
      switch(tbGizmoRotate.iCurGizmo)
      {
      case X_AXIS:
         {
            tbGizmoRotate.beDrag = TRUE;
            curObj.SetPosition(worldOPos);
            curObj.Rotate(X_AXIS,
                          (tbOldX-x)*tbGizmoRotate.curSlop[0] +
                          (tbOldY-y)*tbGizmoRotate.curSlop[1],
                          GLOBAL);
            curObj.SetPosition(curObjPos);
         }
         break;

      case Y_AXIS:
         {
            tbGizmoRotate.beDrag = TRUE;
            curObj.SetPosition(worldOPos);
            curObj.Rotate(Y_AXIS,
                          (tbOldX-x)*tbGizmoRotate.curSlop[0] +
                          (tbOldY-y)*tbGizmoRotate.curSlop[1],
                          GLOBAL);
            curObj.SetPosition(curObjPos);
         }
         break;

      case Z_AXIS:
         {
            tbGizmoRotate.beDrag = TRUE;
            curObj.SetPosition(worldOPos);
            curObj.Rotate(Z_AXIS,
                          (tbOldX-x)*tbGizmoRotate.curSlop[0] +
                          (tbOldY-y)*tbGizmoRotate.curSlop[1],
                          GLOBAL);
            curObj.SetPosition(curObjPos);
         }
         break;

      default:
         tbGizmoRotate.beDrag = FALSE;
         return;
      }
   }

   // update Typein UIs
   float oriValue[6];   // Original  Position-XYZ/Rotate-XYZ
   float newValue[6];   // New       Position-XYZ/Rotate-XYZ
   float difValue[6];   // Different Position-XYZ/Rotate-XYZ
   float newM12[12];    // M12 of Current Object

   //--- get original value
   memcpy(oriValue, &tbGizmoBackupM12[9], sizeof(float)*3);
   FyFormEulerAnglesXYZFromM12(&oriValue[3], &oriValue[4], &oriValue[5], tbGizmoBackupM12);

   //--- get new value
   curObj.GetMatrix(newM12);
   memcpy(newValue, &newM12[9], sizeof(float)*3);
   FyFormEulerAnglesXYZFromM12(&newValue[3], &newValue[4], &newValue[5], newM12);

   //--- get different value
   for (int i = 0; i < 6; i++) {
      difValue[i] = newValue[i] - oriValue[i];
   }

   //--- update Typein value
   TbPasteCurrentTypeinValue(difValue, &difValue[3]);
}


/*-----------------------
  Release Gizmo by Mouse
  Eric Fei 1222, 2012
 ------------------------*/
void TbReleaseGizmo()
{
   if (tbGizmoTranslate.beDrag || tbGizmoRotate.beDrag) {
      // update Transform Typein UI from Current Model
      TbUpdateTransformUIData(TRUE);

      // Excute Set Transform Command
      FnObject curObj(TbGetCurrentBaseObjID());
      curObj.SetMatrix(tbGizmoBackupM12, REPLACE);
      TbUpdateTransformCommand(FAILED_ID, NULL);
      
      // set flag
      tbGizmoTranslate.beDrag = FALSE;
      tbGizmoRotate.beDrag    = FALSE;
   }
}


/*----------------------------
  select camera for rendering
  C.Wang 1209, 2012
 -----------------------------*/
void TbSelectCamera(UIid uiID, char *value)
{
   FnCamera camera;
   float aspect;

   if (FyCheckWord(value, "MainCam")) {
      tbRenderCamID = tbMainCamID;
      tbBeEditCam = TRUE;

      camera.ID(tbMainCamID);
      aspect = camera.GetAspectRatio();
      tbWorkSpaceVPWidth = (int)(aspect*tbWorkSpaceVPHeight);
      tbWorkSpaceVP.SetSize(tbWorkSpaceVPWidth, tbWorkSpaceVPHeight);
   }
   else {
      // get the camera
      FnObject obj;
      char name[256];
      FnUiCombo uiC(tbSelectCameraID);
      int i, numC = tbCurScene->GetCameraNumber();
      OBJECTid *cList = (OBJECTid *)FyAllocateBuffer(sizeof(OBJECTid)*numC);
      tbCurScene->GetCameras(cList, numC);
      uiC.GetComboValue(name);
      for (i = 0; i < numC; i++) {
         obj.ID(cList[i]);
         if (FyCheckWord(obj.GetName(), name)) {
            tbRenderCamID = cList[i];
            camera.ID(tbRenderCamID);
            aspect = camera.GetAspectRatio();
            int h = tbWorkSpaceVPHeight;
            int w = (int)(aspect*tbWorkSpaceVPHeight);
            if (w > tbWorkSpaceVPWidth) {
               w = tbWorkSpaceVPWidth;
               h = (int) (w/aspect);
            }
            tbWorkSpaceVP.SetSize(w, h);
            tbBeEditCam = FALSE;

            // play camera's animation
            camera.PlayFrame(tbCurFrame);
            camera.PlayFOV(tbCurFrame);
            break;
         }
      }
   }
}


/*---------------------------------
  check typein input is an integer
  C.Wang 0513, 2014
 ----------------------------------*/
void TbCheckIntegerInput(UIid uiID, char *value)
{
   if (TbCheckInt(value)) {
      tbInputInteger = atoi(value);
   }
   else {
      FnUiTypein uiTy(uiID);
      char string[256];

      sprintf(string, "%d", tbInputInteger);
      uiTy.PasteTypeinValue(string);
   }
}


/*--------------------------------------------------------
  callback function for double-clicking left mouse button
  C.Wang 0814, 2012
 ---------------------------------------------------------*/
void TbLeftMouseDoubleClick(int x, int y)
{
   // check if the mouse pressing a UI
   if (FyCheckAllLeftMouseDoubleClick(tbAllUISysID, tbNumUISys, x, y)) return;
}


/*-------------------------------------------------
  callback function for pressing left mouse button
  C.Wang 1219, 2012
 --------------------------------------------------*/
void TbLeftMousePress(int x, int y)
{
   // check if the mouse pressing a UI
   if (FyCheckAllLeftMousePress(tbAllUISysID, tbNumUISys, x, y)) {
      tbBeUIPressed = TRUE;
      return;
   }

   // perform hit test current object
   if (FyCheckHotKeyStatus(FY_CTRL)) {
      if (tbTool == FACE_TOOL) {
         TbAdjustFaceMaskLMPress(x, y);
      }
      TbHitCurrentObject(x, y);
      tbBeHitTest = TRUE;
      return;
   }

   // perform hit gizmo object
   if (FyCheckHotKeyStatus(FY_SHIFT)) {
      if (tbTool == FACE_TOOL){
         TbAdjustFaceMaskLMPress(x, y);
      }
      TbHitGizmo(x, y, TRUE);
      tbOldX = x;
      tbOldY = y;
      return;
   }

   if (!tbBeEditCam) return;

   // check if the mouse inside the workspace
   int xx, yy;
   xx = x - tbBottomVPOX;
   yy = y - tbBottomVPOY;
   if (xx > 0 && xx < tbBottomVPWidth && yy > 0 && yy < tbBottomVPHeight) {
      FyBindInputFunction(TbCommandInput);
      FyDefineHotKey(FY_LEFT, TbCommandFunKey, TRUE);
      FyDefineHotKey(FY_UP, TbCommandFunKey);
      FyDefineHotKey(FY_RIGHT, TbCommandFunKey, TRUE);
      FyDefineHotKey(FY_DOWN, TbCommandFunKey);
   }
   tbBeUIPressed = FALSE;

   // check for camera
   tbOldX = x;
   tbOldY = y;
   TbRenderItOnce();
}


/*-------------------------------------------------
  callback function for dragging left mouse button
  C.Wang 0814, 2012
 --------------------------------------------------*/
void TbLeftMouseDrag(int x, int y)
{
   if (tbTool == FACE_TOOL && tbBeFTPressed) {
      TbAdjustFaceMaskLMDrag(x, y);
   }
   else if (FyCheckHotKeyStatus(FY_SHIFT)) {
      TbDragGizmo(x, y);

      // update x and y position
      tbOldX = x; tbOldY = y;
   }
   else if (!tbBeUIPressed && !tbBeHitTest) {     // check for camera
      if (!tbBeEditCam) return;
      if (tbRenderCamera == PERSPECTIVE) {
         if (tbBeZUp) {
            tbCameraRef.Rotate(Z_AXIS, (float) (tbOldX - x), LOCAL);
         }
         else {
            tbCameraRef.Rotate(Y_AXIS, (float) (tbOldX - x), LOCAL);
         }
    
         if (tbBeZUp) {
            tbCamera.Rotate(X_AXIS, (float) (tbOldY - y), GLOBAL);    
         }
         else {
            tbCamera.Rotate(X_AXIS, (float) (tbOldY - y), GLOBAL);    
         }
      }
      else {
      }

      // update x and y position
      tbOldX = x; tbOldY = y;
      TbRenderItOnce();
   }
   else {
      FyCheckAllLeftMouseDrag(tbAllUISysID, tbNumUISys, x, y);
   }
}


/*--------------------------------------------------
  callback function for releasing left mouse button
  C.Wang 0814, 2012
 ---------------------------------------------------*/
void TbLeftMouseRelease(int x, int y)
{
   TbReleaseGizmo();

   // check if the mouse release in Face Tool
   if (tbBeFTPressed) {
      TbAdjustFaceMaskLMRelease(x, y);
   }

   // check if the mouse releasing a UI
   if (tbBeUIPressed) {
      FyCheckAllLeftMouseRelease(tbAllUISysID, tbNumUISys, x, y);
      tbBeUIPressed = FALSE;
   }

   if (!tbBeEditCam) return;

   if (tbBeHitTest) {
      tbBeHitTest = FALSE;
   }

   tbRenderedFrame = 0;
}


/*--------------------------------------------------
  callback function for pressing right mouse button
  C.Wang 0814, 2012
 ---------------------------------------------------*/
void TbRightMousePress(int x, int y)
{
   if (!tbBeEditCam) return;
   
   // check for camera
   tbOldX = x;
   tbOldY = y;

   // check if the shift key is pressing
   if (FyCheckHotKeyStatus(FY_SHIFT)) {
      if (tbRenderCamera == PERSPECTIVE) {
      }
      else {
      }
   }
}


/*--------------------------------------------------
  callback function for dragging right mouse button
  C.Wang 0814, 2012
 ---------------------------------------------------*/
void TbRightMouseDrag(int x, int y)
{
   if (!tbBeEditCam) return;

   // check for camera
   // if FY_CTRL key is down, pan->pivot
   if (FyCheckHotKeyStatus(FY_ALT)) {
      if (tbRenderCamera == PERSPECTIVE) {
         if (x != tbOldX) {
            tbCameraRef.Translate((float) (tbOldX - x)*tbMoveSpeed, 0.0f, 0.0f, LOCAL);
         }
        
         if (y != tbOldY) {
            tbCamera.Translate(0.0f, (float) (y - tbOldY)*tbMoveSpeed, 0.0f, LOCAL);
         }
      }
      else {
         if (tbRenderCamera == ORTHOGONAL) {
            if (x != tbOldX) {
               tbOrthoCamera.Translate((float) (tbOldX - x)*tbMoveSpeed*0.01f, 0.0f, 0.0f, LOCAL);
            }
        
            if (y != tbOldY) {
               tbOrthoCamera.Translate(0.0f, (float) (y - tbOldY)*tbMoveSpeed*0.01f, 0.0f, LOCAL);
            }
         }
      }
      TbRenderItOnce();
   }
   else {
      if (FyCheckHotKeyStatus(FY_SHIFT)) {
         if (tbRenderCamera == PERSPECTIVE) {
         }
         else {
         }
      }
      else {
         if (tbRenderCamera == ORTHOGONAL) {
            if (x != tbOldX) {
               tbOrthoCamera.Translate((float) (tbOldX - x)*tbMoveSpeed*0.01f, 0.0f, 0.0f, LOCAL);
            }
        
            if (y != tbOldY) {
               tbOrthoCamera.Translate(0.0f, (float) (y - tbOldY)*tbMoveSpeed*0.01f, 0.0f, LOCAL);
            }
         }
      }
      TbRenderItOnce();
   }

   // update x and y position
   tbOldX = x;
   tbOldY = y;
}


/*---------------------------------------------------
  callback function for releasing right mouse button
  C.Wang 0814, 2012
 ----------------------------------------------------*/
void TbRightMouseRelease(int x, int y)
{
   if (!tbBeEditCam) return;

   if (FyCheckHotKeyStatus(FY_SHIFT)) {
      if (tbRenderCamera == PERSPECTIVE) {
      }
      else {
      }
   }

   tbRenderedFrame = 0;
}


/*---------------------------------------------------
  callback function for pressing middle mouse button
  C.Wang 0814, 2012
 ----------------------------------------------------*/
void TbMiddleMousePress(int x, int y)
{
   if (!tbBeEditCam) return;

   // check for camera
   tbOldX = x;
   tbOldY = y;
}


/*---------------------------------------------------
  callback function for dragging middle mouse button
  C.Wang 0814, 2012
 ----------------------------------------------------*/
void TbMiddleMouseDrag(int x, int y)
{
   if (!tbBeEditCam) return;

   // check for camera
   if (FyCheckHotKeyStatus(FY_ALT)) {
      if (tbRenderCamera == PERSPECTIVE) {
         if (x != tbOldX) {
            tbCamera.Translate(0.0f, 0.0f, (float)(tbOldX - x)*tbZoomSpeed, LOCAL);
         }

         if (y != tbOldY) {
            tbCamera.Translate(0.0f, 0.0f, (float)(tbOldY - y)*tbZoomSpeed, LOCAL);
         }
      }
      else {
         if (x != tbOldX) {
            if (tbOldX - x > 0) {
               tbOrthScaleFactor *= tbOrthScaleRate;
            }
            else {
               tbOrthScaleFactor /= tbOrthScaleRate;
            }
            tbOrthoCamera.SetScaleFactor(tbOrthScaleFactor);
         }
      }
      TbRenderItOnce();
   }
   else {
      // if pressing SHIFT key, moving current object
      if (FyCheckHotKeyStatus(FY_SHIFT)) {
      }
      else {
         if (tbRenderCamera == ORTHOGONAL) {
            if (x != tbOldX) {
               if (tbOldX - x > 0) {
                  tbOrthScaleFactor *= tbOrthScaleRate;
               }
               else {
                  tbOrthScaleFactor /= tbOrthScaleRate;
               }
               tbOrthoCamera.SetScaleFactor(tbOrthScaleFactor);
            }
         }
      }
      TbRenderItOnce();
   }

   tbOldX = x;
   tbOldY = y;
}


/*----------------------------------------------------
  callback function for releasing middle mouse button
  C.Wang 0814, 2012
 -----------------------------------------------------*/
void TbMiddleMouseRelease(int x, int y)
{
   tbRenderedFrame = 0;

   if (!tbBeEditCam) return;
}


/*----------------------------------------------------
  callback function for mouse move
  Eric Fei 1219, 2012
 -----------------------------------------------------*/
void TbMouseMove(int x, int y)
{
   if (FyCheckHotKeyStatus(FY_SHIFT)) {
      TbHitGizmo(x, y);
   }
}


/*-------------------
  create menu bar UI
  C.Wang 0814, 2012
 --------------------*/
void TbCreateMenuBarUI()
{
   FnUiPlate uiP;
   FnUiButton uiB;
   FnUiRadioButton uiRB;

   // create the menu bar Ui system
   tbMenuBarID = FyBeginUISystem(tbMenuBarOX, tbMenuBarOY, tbMenuBarWidth, tbMenuBarHeight);
   tbMenuBarUI.ID(tbMenuBarID);
   tbMenuBarUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbMenuBarID;

   // create the plate
   tbMenuBarPlateID = tbMenuBarUI.CreatePlate("BarPlate", 0, 0, tbMenuBarWidth - 1, tbMenuBarHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbMenuBarPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);

   // create the "System" function
   tbSystemButtonID = tbMenuBarUI.CreateRadioButton("SystemRadioButton", 5, 5, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "System", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);

   // create the "script loader" function
   tbScriptSystemButtonID = tbMenuBarUI.CreateRadioButton("ScriptRadioButton", 5 + BUTTON_WIDTH + LONG_BUTTON_WIDTH*5 + 18, 5,
                                                           LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbScriptSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Script", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);
   uiRB.Show(FALSE);

   // create the "Model Tool" function
   tbModelSystemButtonID = tbMenuBarUI.CreateRadioButton("ModelRadioButton", 5 + BUTTON_WIDTH + 3, 5,
                                                          LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbModelSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Scene/Models", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);

   // create the "Character Tool" function
   tbCharacterSystemButtonID = tbMenuBarUI.CreateRadioButton("CharacterRadioButton", 5 + BUTTON_WIDTH + LONG_BUTTON_WIDTH + 6, 5,
                                                          LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbCharacterSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Characters", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);

   // create the "Game FX Tool" function
   tbFXSystemButtonID = tbMenuBarUI.CreateRadioButton("FXRadioButton", 5 + BUTTON_WIDTH + LONG_BUTTON_WIDTH*2 + 9, 5,
                                                       LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbFXSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Game FXs", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);

   // create the "Build Scene Tool" function
   tbBuildSceneButtonID = tbMenuBarUI.CreateRadioButton("BuildSceneRadioButton", 5 + BUTTON_WIDTH + LONG_BUTTON_WIDTH*3 + 12, 5,
                                                       LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbBuildSceneButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Build Scene", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.Enable(FALSE);

   // create the "Game Player Tool" function
   tbGamePlayerButtonID = tbMenuBarUI.CreateRadioButton("GamePlayerRadioButton", 5 + BUTTON_WIDTH + LONG_BUTTON_WIDTH*4 + 15, 5,
                                                       LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbGamePlayerButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Game Player", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.Enable(FALSE);

   // create the "Working Environment" function
   tbEnvironmentSystemButtonID = tbMenuBarUI.CreateRadioButton("EnvironmentRadioButton", 5 + BUTTON_WIDTH + LONG_BUTTON_WIDTH*5 + 18, 5,
                                                          LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbEnvironmentSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Environment", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);
   uiRB.Show(FALSE);

   // create the "Post Processing" function
   tbPostProcessingSystemButtonID = tbMenuBarUI.CreateRadioButton("PostProcessingRadioButton", 5 + BUTTON_WIDTH + LONG_BUTTON_WIDTH*5 + 18, 5,
                                                          LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbPostProcessingSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Post Processing", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);
   uiRB.Show(FALSE);

   // create the "Face Tool" function
   tbFaceToolButtonID = tbMenuBarUI.CreateRadioButton("FaceToolRadioButton", 5 + BUTTON_WIDTH + LONG_BUTTON_WIDTH*5 + 18, 5,
                                                       LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbFaceToolButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Face Tool", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbFaceToolUICommand);
   uiRB.Show(FALSE);

   // create the "Ortho View" function
   tbOrthoButtonID = tbMenuBarUI.CreateRadioButton("OrthoButton", tbMenuBarWidth - BUTTON_WIDTH*3 - 5 - 6, 5, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbOrthoButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "3D View", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   if (tbRenderCamera == PERSPECTIVE) {
      uiRB.ChangeState(TRUE);
   }
   else {
      uiRB.ChangeState(FALSE);
   }
   uiRB.BindCallback(TbShowOrthoView);
   uiRB.Enable(FALSE);

   // create the "Aux View" function
   tbAuxButtonID = tbMenuBarUI.CreateRadioButton("AuxButton", tbMenuBarWidth - BUTTON_WIDTH*2 - 5 - 3, 5, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbAuxButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Aux View", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.ChangeState(tbShowAux);
   uiRB.BindCallback(TbShowAuxView);

   // create the "Exit" function
   tbExitButtonID = tbMenuBarUI.CreateButton("ExitButton", tbMenuBarWidth - BUTTON_WIDTH - 5, 5, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbExitButtonID);
   uiB.SetParent(tbMenuBarPlateID);
   uiB.SetCaption(CENTERED, 0, 0, "Exit", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(TbOpenQuit);
}


/*--------------------
  create main menu UI
  C.Wang 0814, 2012
 ---------------------*/
void TbCreateMenuUI()
{
   FnUiPlate uiP;
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   FnUiTypein uiTy;
   FnUiCombo uiC;
   int ox, oy;
   char value[256];

   // create the menu Ui system
   tbMenuID = FyBeginUISystem(tbMenuOX, tbMenuOY, tbMenuWidth, tbMenuHeight);
   tbMenuUI.ID(tbMenuID);
   tbMenuUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbMenuID;

   // create the plates
   tbControlPanelPlateUI = tbMenuUI.CreatePlate("Control Panel", 0, 0, tbMenuWidth - 1, tbMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbControlPanelPlateUI);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);

   // create the play/stop buttons
   ox = tbMenuWidth - tbSideMenuWidth + 50;
   oy = 10;
   tbStopID = tbMenuUI.CreateButton("StopButton", ox, oy, SHORT_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbStopID);
   uiB.SetCaption(CENTERED, 0, 0, "¡½", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(3);
   uiB.SetParent(tbControlPanelPlateUI);
   uiB.UseChineseFontSet(TRUE);
   uiB.BindCallback(TbStopPlay);

   ox += SHORT_BUTTON_WIDTH + 3;
   tbPlayID = tbMenuUI.CreateRadioButton("PlayButton", ox, oy, SHORT_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbPlayID);
   uiRB.SetCaption(CENTERED, 0, 0, ">", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(3);
   uiRB.SetParent(tbControlPanelPlateUI);
   uiRB.BindCallback(TbMakePlay);

   oy -= 5;
   ox += SHORT_BUTTON_WIDTH + 3;
   tbPlaySpeedID = tbMenuUI.CreateTypein("PlaySpeedTypein", ox, oy, 120, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlaySpeedID);
   uiTy.SetParent(tbControlPanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 50, 2, 50);
   uiTy.SetCaption(LEFT_CENTERED, 12, 2, "Speed", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   sprintf(value, "%5.3f", tbPlaySpeed);
   uiTy.PasteTypeinValue(value);
   uiTy.BindCallbacks(TbPlaySpeedUICommand);
   uiTy.SetLayer(3);

   ox += 125;
   tbDebugTypeinID = tbMenuUI.CreateTypein("DebugTypein", ox, oy, 35, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbDebugTypeinID);
   uiTy.SetParent(tbControlPanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 2, 2, 30);
   uiTy.SetCaption(LEFT_CENTERED, 0, 2, "", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.SetLayer(3);

   ox = tbMenuWidth - tbSideMenuWidth + 50 + (SHORT_BUTTON_WIDTH + 3)*2 + 10;
   oy = 10 + 30;
   tbSelectPlayModeID = tbMenuUI.CreateCombo("SelectPlayModeCombo", ox, oy, 140, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                             35, 70, 2, FALSE);
   uiC.ID(tbSelectPlayModeID);
   uiC.SetLayer(2);
   uiC.SetParent(tbControlPanelPlateUI);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Mode", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Loop");
   uiC.InsertMessage(1, "Once");
   uiC.PasteComboValue("Loop");
   uiC.BindCallbacks(NULL, TbSelectPlayMode);
   uiC.Enable(TRUE);
   tbPlayMode = LOOP;

   ox -= (180 + 10);
   oy -= 5;
   tbCurFrameID = tbMenuUI.CreateTypein("CurrentFrameTypein", ox, oy, 188, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbCurFrameID);
   uiTy.SetParent(tbControlPanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 90, 2, 88);
   uiTy.SetCaption(LEFT_CENTERED, 2, 2, "Current Frame", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGotoCurrentFrame);
   uiTy.SetLayer(3);

   char msg[256];
   sprintf(msg, "%8.1f", tbCurFrame);
   uiTy.PasteTypeinValue(msg);

   // create the camera selection cambo
   ox = tbMenuWidth - tbSideMenuWidth + 50 - 200;
   oy = 10;
   tbSelectCameraID = tbMenuUI.CreateCombo("SelectCameraCombo", ox, oy, 200, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                            85, 100, 4);
   uiC.ID(tbSelectCameraID);
   uiC.SetLayer(2);
   uiC.SetParent(tbControlPanelPlateUI);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Select Camera", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "MainCam");
   uiC.PasteComboValue("MainCam");
   uiC.BindCallbacks(NULL, TbSelectCamera);
   uiC.Enable(TRUE);

   // create the UIs for seting up the directories
   int objectPanelWidth = 410;
   ox = 5;
   oy = 2;
   tbEnvironmentPlanelPlateUI = tbMenuUI.CreatePlate("Directory Panel", ox, oy, objectPanelWidth*2, tbMenuHeight - 5, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbEnvironmentPlanelPlateUI);
   uiP.SetParent(tbControlPanelPlateUI);
   uiP.SetLayer(1);
   uiP.Show(FALSE);

   tbWorkDirTypeinUI = tbMenuUI.CreateTypein("WorkEnvTypein", ox, oy, objectPanelWidth - 50, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbWorkDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Working >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbSetDir);

   // get working directory and paste it on UI
   char dir[256];
   GetCurrentDirectory(256, dir);
   uiTy.PasteTypeinValue(dir);
   strcpy(tbWorkDir, dir);

   oy += 25;
   char *dirS = FyGetModelPath();
   tbModelDirTypeinUI = tbMenuUI.CreateTypein("WorkModelEnvTypein", ox, oy, objectPanelWidth - 50, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbModelDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Model >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue(dirS);
   uiTy.BindCallbacks(TbSetDir);

   oy += 25;
   dirS = FyGetTexturePath();
   tbTextureDirTypeinUI = tbMenuUI.CreateTypein("WorkTextureEnvTypein", ox, oy, objectPanelWidth - 50, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbTextureDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Texture >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue(dirS);
   uiTy.BindCallbacks(TbSetDir);

   oy += 25;
   dirS = FyGetShaderPath();
   tbShaderDirTypeinUI = tbMenuUI.CreateTypein("WorkShaderEnvTypein", ox, oy, objectPanelWidth - 50, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbShaderDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Shader >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue(dirS);
   uiTy.BindCallbacks(TbSetDir);

   ox += objectPanelWidth - 10;
   dirS = FyGetCharacterPath();
   tbCharacterDirTypeinUI = tbMenuUI.CreateTypein("WorkCharacterEnvTypein", ox, oy, objectPanelWidth - 50, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbCharacterDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Character >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue(dirS);
   uiTy.BindCallbacks(TbSetDir);
}


/*--------------------
  create side menu UI
  C.Wang 1103, 2012
 ---------------------*/
void TbCreateSideMenuUI()
{
   int i;
   int ox, oy;
   FnUiPlate uiP;
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   FnUiTab uiT;
   FnUiCombo uiC;
   FnUiTypein uiTy;

   // create the menu UI system
   tbSideMenuID = FyBeginUISystem(tbSideMenuOX, tbSideMenuOY, tbSideMenuWidth, tbSideMenuHeight);
   tbSideMenuUI.ID(tbSideMenuID);
   tbSideMenuUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbSideMenuID;

   // create the UIs
   tbSideMenuPlateID = tbSideMenuUI.CreatePlate("SideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(TRUE);

   // for system menu
   tbSideMenuSystemPlateID = tbSideMenuUI.CreatePlate("SystemSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuSystemPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 5;
   oy = 5;
   tbNewButtonID = tbSideMenuUI.CreateButton("NewButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbNewButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "New", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbOpenNew);

   oy += 23;
   tbModelButtonID = tbSideMenuUI.CreateButton("ModelButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Scene/Models", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbModelUICommand);

   oy += 23;
   tbCharacterButtonID = tbSideMenuUI.CreateButton("CharacterButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbCharacterButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Characters", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbCharacterUICommand);

   oy += 23;
   tbFXButtonID = tbSideMenuUI.CreateButton("FXButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbFXButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Game FXs", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbFXUICommand);

   oy += 23;
   tbScriptButtonID = tbSideMenuUI.CreateButton("ScriptButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbScriptButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Script", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbScriptUICommand);

   oy += 23;
   tbPostProcessingButtonID = tbSideMenuUI.CreateButton("PostProcessingButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbPostProcessingButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Post Processing", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbPostProcessingUICommand);

   oy += 23;
   tbEnvironmentButtonID = tbSideMenuUI.CreateButton("EnvironmentButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbEnvironmentButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Environment", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbEnvironmentUICommand);

   oy += 23;
   tbFaceButtonID = tbSideMenuUI.CreateButton("FaceToolButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbFaceButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Face Tool", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbFaceToolUICommand);

   // for script menu
   tbSideMenuScriptPlateID = tbSideMenuUI.CreatePlate("ScriptSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuScriptPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 5;
   oy = 5;
   tbLoadScriptButtonID = tbSideMenuUI.CreateButton("LoadScriptButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbLoadScriptButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuScriptPlateID);
   uiB.BindCallback(TbLoadScriptUICommand);

   // for post-processing menu
   tbSideMenuPostProcessingPlateID = tbSideMenuUI.CreatePlate("PostProcessingSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuPostProcessingPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 2;
   oy = 2;
   tbSSAOTabID = tbSideMenuUI.CreateTab("SSAOTab", ox, oy, tbSideMenuWidth - 5, 75, 1, 0.45f, 0.45f, 0.45f, 1.0f, FALSE, TRUE);
   uiT.ID(tbSSAOTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuPostProcessingPlateID);
   uiT.SetCaption("Screen Space Ambient Occlusion", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbSSAORadioID = tbSideMenuUI.CreateRadioButton("SSAORadio", ox, oy, BUTTON_WIDTH + 7, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f, FY_NONE, CHECK_BOX);
   uiRB.ID(tbSSAORadioID);
   uiRB.SetCaption(LEFT_CENTERED, 25, 0, "On / Off", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbSSAOTabID);
   uiRB.BindCallback(TbTwiddleSSAOEffect);

   ox += BUTTON_WIDTH + 7 + 10;
   oy -= 2;
   tbSampleRangeTypeinID = tbSideMenuUI.CreateTypein("SampleRange", ox, oy, 160, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSampleRangeTypeinID);
   uiTy.SetParent(tbSSAOTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 90, 5, 60);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Sample Range", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   char value[64];
   sprintf(value, "%8.1f", tbSSAOSampleRange);
   uiTy.PasteTypeinValue(value);
   uiTy.BindCallbacks(TbSetupSSAOSampleRange, NULL);

   oy += 25;
   tbSSAODistanceTypeinID = tbSideMenuUI.CreateTypein("SSAODistance", ox, oy, 160, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSSAODistanceTypeinID);
   uiTy.SetParent(tbSSAOTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 90, 5, 60);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Distance", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   sprintf(value, "%8.1f", tbSSAODistance);
   uiTy.PasteTypeinValue(value);
   uiTy.BindCallbacks(TbSetupSSAODistance, NULL);

   ox = 2;
   oy = 76 + 2;
   tbHDRTabID = tbSideMenuUI.CreateTab("HDRTab", ox, oy, tbSideMenuWidth - 5, 100, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbHDRTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuPostProcessingPlateID);
   uiT.SetCaption("HDR", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbHDRRadioID = tbSideMenuUI.CreateRadioButton("HDRRadio", ox, oy, BUTTON_WIDTH + 7, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f, FY_NONE, CHECK_BOX);
   uiRB.ID(tbHDRRadioID);
   uiRB.SetCaption(LEFT_CENTERED, 25, 0, "On / Off", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbHDRTabID);
   uiRB.BindCallback(TbTwiddleHDREffect);

   ox = 2;
   oy = 76 + 101 + 2;
   tbDOFTabID = tbSideMenuUI.CreateTab("DOFTab", ox, oy, tbSideMenuWidth - 5, 100, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbDOFTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuPostProcessingPlateID);
   uiT.SetCaption("Depth Of Field", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   uiT.ID(tbSSAOTabID);
   uiT.NextTab(tbHDRTabID);
   uiT.ID(tbHDRTabID);
   uiT.NextTab(tbDOFTabID);

   // for face tool
   tbSideMenuFaceToolPlateID = tbSideMenuUI.CreatePlate("FaceToolSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuFaceToolPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   int faceToolHeight = 60;
//#ifdef Q3D
   faceToolHeight += 55;
//#endif

   ox = 2;
   oy = 2;
   tbSelectFaceTabID = tbSideMenuUI.CreateTab("SelectFaceTab", ox, oy, tbSideMenuWidth - 5, faceToolHeight, 1, 0.45f, 0.45f, 0.45f, 1.0f, FALSE, TRUE);
   uiT.ID(tbSelectFaceTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFaceToolPlateID);
   uiT.SetCaption("Select Face Model", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbFindFaceModelComboID = tbSideMenuUI.CreateCombo("FaceMaskModel", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                               80, tbSideMenuWidth - ox - ox/3 - 90);
   uiC.ID(tbFindFaceModelComboID);
   uiC.SetParent(tbSelectFaceTabID);
   uiC.SetLayer(10);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Select Face >", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(NULL, TbSelectFaceModelComboUICommand, NULL);
   uiC.Enable(FALSE);

//#ifdef Q3D
   oy += 25;
   tbFindFaceMaterialComboID = tbSideMenuUI.CreateCombo("FindFaceModelMaterial", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                               80, tbSideMenuWidth - ox - ox/3 - 90);
   uiC.ID(tbFindFaceMaterialComboID);
   uiC.SetParent(tbSelectFaceTabID);
   uiC.SetLayer(10);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "     Material >", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(NULL, TbSelectFaceMaterialComboUICommand, NULL);
   uiC.Enable(FALSE);

   oy += 28;
   tbLoadFaceMaskButtonID = tbSideMenuUI.CreateButton("LoadFaceMaskButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbLoadFaceMaskButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbSelectFaceTabID);
   uiB.BindCallback(TbLoadFaceMaskUICommand);

   ox += BUTTON_WIDTH + 5;
   tbSaveFaceMaskButtonID = tbSideMenuUI.CreateButton("SaveFaceMaskButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbSaveFaceMaskButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Save", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbSelectFaceTabID);
   uiB.BindCallback(TbSaveFaceMaskUICommand);
//#endif

   ox = 2;
   oy = faceToolHeight + 2;
   tbBlendshapeTabID = tbSideMenuUI.CreateTab("BlendshapesTab", ox, oy, tbSideMenuWidth - 5, 680, 1, 0.45f, 0.45f, 0.45f, 1.0f, FALSE, TRUE);
   uiT.ID(tbBlendshapeTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFaceToolPlateID);
   uiT.SetCaption("Facial Expression", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);
   uiT.Enable(FALSE);

   ox = 15;
   oy = 25;
   tbLoadFacialExpressionID = tbSideMenuUI.CreateButton("LoadFacialExpressionButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbLoadFacialExpressionID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbBlendshapeTabID);
   uiB.BindCallback(TbLoadFacialExpressionUI);

   ox += BUTTON_WIDTH + 5;
   tbSaveFacialExpressionID = tbSideMenuUI.CreateButton("SaveFacialExpressionButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbSaveFacialExpressionID);
   uiB.SetCaption(CENTERED, 0, 0, "Save", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbBlendshapeTabID);
   uiB.BindCallback(TbSaveFacialExpressionUI);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 28;
   tbSelectFacialExpressionID = tbSideMenuUI.CreateCombo("SelectFacialExp", ox, oy, tbSideMenuWidth - ox - ox/3 - 45, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                         50, tbSideMenuWidth - ox - ox/3 - 105);
   uiC.ID(tbSelectFacialExpressionID);
   uiC.SetParent(tbBlendshapeTabID);
   uiC.SetLayer(10);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Select >", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(TbRenameFacialExpressionUICommand, TbSelectFaceExpressionComboUICommand, NULL);

   ox += tbSideMenuWidth - ox - ox/3 - 44;
   oy += 1;
   tbAddFacialExpressionID = tbSideMenuUI.CreateButton("AddNewFacialExpressionButton", ox, oy, 17, 17, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddFacialExpressionID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbBlendshapeTabID);
   uiB.BindCallback(TbAddFaceExpression);

   ox += 20;
   tbRemoveFacialExpressionID = tbSideMenuUI.CreateButton("RemoveFacialExpressionButton", ox, oy, 17, 17, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbRemoveFacialExpressionID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbBlendshapeTabID);
   uiB.BindCallback(TbRemoveFacialExpressionUICommand);

   ox = 15;
   oy += 26;
   tbFaceFrameCountID = tbSideMenuUI.CreateTypein("FacialExpressionLength", ox, oy, 140, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbFaceFrameCountID);
   uiTy.SetParent(tbBlendshapeTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 75);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Length :", 0.0f, 0.0f, 0.0f);
   uiTy.PasteTypeinValue("100.0");
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbSetFaceExpressionLengthUICommand, NULL);

   ox += 145;
   oy += 2;
   tbStopFacialExpressionID = tbSideMenuUI.CreateButton("StopPlayFacialExpressionButton", ox, oy, SHORT_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbStopFacialExpressionID);
   uiB.SetCaption(CENTERED, 0, 0, "¡½", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(3);
   uiB.SetParent(tbBlendshapeTabID);
   uiB.UseChineseFontSet(TRUE);
   uiB.BindCallback(TbStopPlayFacialExpression);

   ox += SHORT_BUTTON_WIDTH + 3;
   tbPlayFacialExpressionID = tbSideMenuUI.CreateRadioButton("PlayFacialExpressionButton", ox, oy, SHORT_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.ID(tbPlayFacialExpressionID);
   uiRB.SetCaption(CENTERED, 0, 0, ">", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(3);
   uiRB.SetParent(tbBlendshapeTabID);
   uiRB.BindCallback(TbMakePlayFacialExpression);

   ox = 15;
   oy += 28;
   tbSelectFacialExpressionKeyID = tbSideMenuUI.CreateCombo("SelectFacialKey", ox, oy, 200, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f, 50, 165);
   uiC.ID(tbSelectFacialExpressionKeyID);
   uiC.SetParent(tbBlendshapeTabID);
   uiC.SetLayer(10);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Key ID >", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(TbGotoFacialExpressionFrame, TbSelectFacialExpressionKey, NULL);

   ox += 203;
   oy += 1;
   tbAddFacialExpressionKeyID = tbSideMenuUI.CreateButton("AddNewFacialExpressionKeyButton", ox, oy, 17, 17, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddFacialExpressionKeyID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbBlendshapeTabID);
   uiB.BindCallback(TbAddFaceExpressionKeyCommand);

   ox += 20;
   tbRemoveFacialExpressionKeyID = tbSideMenuUI.CreateButton("RemoveFacialExpressionKeyButton", ox, oy, 17, 17, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbRemoveFacialExpressionKeyID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbBlendshapeTabID);
   uiB.BindCallback(TbRemoveFaceExpressionKeyCommand);

   int ny = oy + 26;
   for (i = 0; i < MAX_BLENDSHAPES; i++) {
      if (i == 20) {
         ox = 150;
         oy = ny;
      }
      else if (i > 20) {
         ox = 150;
         oy += 26;
      }
      else {
         ox = 15;
         oy += 26;
      }
      tbBlendShapeID[i] = tbSideMenuUI.CreateTypein("BlendShape", ox, oy, 135, 25, 256, 0.45f, 0.45f, 0.45f);
      uiTy.ID(tbBlendShapeID[i]);
      uiTy.SetParent(tbBlendshapeTabID);
      uiTy.SetLayer(2);
      uiTy.SetTypein(LEFT_CENTERED, 70, 5, 55);
      uiTy.SetCaption(LEFT_CENTERED, 2, 5, "BS :", 0.0f, 0.0f, 0.0f);
      uiTy.PasteTypeinValue("0.0");
      uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
      uiTy.Show(FALSE);
      uiTy.BindCallbacks(TbSetupBlendShapeValueCommand, NULL);
   }

   uiT.ID(tbSelectFaceTabID);
   uiT.NextTab(tbBlendshapeTabID);


   // for model menu
   tbSideMenuModelPlateID = tbSideMenuUI.CreatePlate("ModelSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuModelPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 2;
   oy = 2;
   tbModelFileTabID = tbSideMenuUI.CreateTab("ModelFileTab", ox, oy, tbSideMenuWidth - 5, 106, 1, 0.45f, 0.45f, 0.45f, 1.0f, FALSE, TRUE);
   uiT.ID(tbModelFileTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Current Model", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbLoadModelButtonID = tbSideMenuUI.CreateButton("LoadModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbLoadModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbLoadModelUICommand);

   ox += BUTTON_WIDTH + 5;
   tbPreviousModelButtonID = tbSideMenuUI.CreateButton("PreviousModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbPreviousModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Previous", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbPreviousModelUICommand);
   uiB.Enable(FALSE);

   ox += BUTTON_WIDTH + 5;
   tbNextModelButtonID = tbSideMenuUI.CreateButton("NextModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbNextModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Next", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbNextModelUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbSaveModelButtonID = tbSideMenuUI.CreateButton("SaveModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbSaveModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Save", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbSaveModelUICommand);
   uiB.Enable(FALSE);

   ox += (BUTTON_WIDTH + 5);
   tbDeleteModelButtonID = tbSideMenuUI.CreateButton("DeleteModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbDeleteModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Delete", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbDeleteModelUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbFindModelComboID = tbSideMenuUI.CreateCombo("FindCurrentModel", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                               80, tbSideMenuWidth - ox - ox/3 - 90);
   uiC.ID(tbFindModelComboID);
   uiC.SetParent(tbModelFileTabID);
   uiC.SetLayer(10);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Find Model", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(TbFindModelComboUICommand, TbFindModelComboUICommand, TbFindModelTypeinUI);
   uiC.Enable(FALSE);

   // define Gizmo Uis
   ox = 2;
   oy = 101 + 2;   // 103: Models Tab Height(2 + 100 + 1),  2: Bound
   tbModelTransformTabID = tbSideMenuUI.CreateTab("TransformTab", ox, oy, tbSideMenuWidth - 5, tbTransformHeight, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbModelTransformTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Transform", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);
   
   ox = 15;
   oy = 25;       // 25: Transform Tab Text Height
   tbModelTFTranslateButtonID = tbSideMenuUI.CreateRadioButton("TFPositionButton", ox, oy+3, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbTransformUIs[0] = tbModelTFTranslateButtonID;
   uiRB.ID(tbModelTFTranslateButtonID);
   uiRB.SetCaption(CENTERED, 0, 0, "P", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbModelTransformTabID);
   uiRB.ChangeState(TRUE);
   uiRB.BindCallback(TbSwitchGizmo);
   uiRB.Enable(FALSE);

   ox += 16 + 5;  // 16: Button Width,  5: Bound
   tbModelTFTranslateXID = tbSideMenuUI.CreateTypein("TFPositionXTypein", ox, oy, 60+50+4, 25, 256, 0.45f, 0.45f, 0.45f);    // 60: Word Width,  50: Tyepin Width,  4: Right Bound,  25: UI Height
   tbTransformUIs[1] = tbModelTFTranslateXID;
   uiTy.ID(tbModelTFTranslateXID);
   uiTy.SetParent(tbModelTransformTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 60, 0, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 0, "Position", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(TbUpdateTransformCommand);
   uiTy.Enable(FALSE);
   
   ox += 60 + 50 + 4;
   tbModelTFTranslateYID = tbSideMenuUI.CreateTypein("TFPositionYTypein", ox, oy, 1+50+4, 25, 256, 0.45f, 0.45f, 0.45f);  // 1: Left Bound,  50: Tyepin Width,  4: Right Bound,  25: UI Height
   tbTransformUIs[2] = tbModelTFTranslateYID;
   uiTy.ID(tbModelTFTranslateYID);
   uiTy.SetParent(tbModelTransformTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 0, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(TbUpdateTransformCommand);
   uiTy.Enable(FALSE);
   
   ox += 1 + 50 + 4;
   tbModelTFTranslateZID = tbSideMenuUI.CreateTypein("TFPositionZTypein", ox, oy, 1+50+4, 25, 256, 0.45f, 0.45f, 0.45f);  // 1: Left Bound,  50: Tyepin Width,   4: Right Bound,   25: UI Height
   tbTransformUIs[3] = tbModelTFTranslateZID;
   uiTy.ID(tbModelTFTranslateZID);
   uiTy.SetParent(tbModelTransformTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 0, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(TbUpdateTransformCommand);
   uiTy.Enable(FALSE);
   
   ox = 15;
   oy += tbOneTypeinHeight;
   tbModelTFRotateButtonID = tbSideMenuUI.CreateRadioButton("TFRotateButton", ox, oy+3, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbTransformUIs[4] = tbModelTFRotateButtonID;
   uiRB.ID(tbModelTFRotateButtonID);
   uiRB.SetCaption(CENTERED, 0, 0, "R", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbModelTransformTabID);
   uiRB.ChangeState(FALSE);
   uiRB.BindCallback(TbSwitchGizmo);
   uiRB.Enable(FALSE);
   
   ox += 16 + 5;  // 16: Button Width,  5: Bound
   tbModelTFRotateXID = tbSideMenuUI.CreateTypein("TFRotateXTypein", ox, oy, 60+50+4, 25, 256, 0.45f, 0.45f, 0.45f);    // 60: Word Width,  50: Tyepin Width,  4: Right Bound,  25: UI Height
   tbTransformUIs[5] = tbModelTFRotateXID;
   uiTy.ID(tbModelTFRotateXID);
   uiTy.SetParent(tbModelTransformTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 60, 0, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 0, "Rotate", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(TbUpdateTransformCommand);
   uiTy.Enable(FALSE);
   
   ox += 60 + 50 + 4;
   tbModelTFRotateYID = tbSideMenuUI.CreateTypein("TFRotateYTypein", ox, oy, 1+50+4, 25, 256, 0.45f, 0.45f, 0.45f);  // 1: Left Bound,  50: Tyepin Width,  4: Right Bound,  25: UI Height
   tbTransformUIs[6] = tbModelTFRotateYID;
   uiTy.ID(tbModelTFRotateYID);
   uiTy.SetParent(tbModelTransformTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 0, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(TbUpdateTransformCommand);
   uiTy.Enable(FALSE);
   
   ox += 1 + 50 + 4;
   tbModelTFRotateZID = tbSideMenuUI.CreateTypein("TFRotateZTypein", ox, oy, 1+50+4, 25, 256, 0.45f, 0.45f, 0.45f);  // 1: Left Bound,  50: Tyepin Width,   4: Right Bound,   25: UI Height
   tbTransformUIs[7] = tbModelTFRotateZID;
   uiTy.ID(tbModelTFRotateZID);
   uiTy.SetParent(tbModelTransformTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 0, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(TbUpdateTransformCommand);
   uiTy.Enable(FALSE);

   tbMatHeight = 228;
   tbRenderHeight = 80;

#ifdef FLY2_DX9
   tbMatHeight += 22;
#endif

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + 1;
   tbRenderCurrentTabID = tbSideMenuUI.CreateTab("CurrentRenderTab", ox, oy, tbSideMenuWidth - 5, tbRenderHeight, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbRenderCurrentTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Rendering", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbWireframeCurModelButtonID = tbSideMenuUI.CreateRadioButton("WireframeModelButton", ox, oy, BUTTON_WIDTH + 7, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f, FY_NONE, CHECK_BOX);
   uiRB.ID(tbWireframeCurModelButtonID);
   uiRB.SetCaption(LEFT_CENTERED, 25, 0, "Wireframe", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbRenderCurrentTabID);
   uiRB.BindCallback(TbWireframeModelUICommand);
   uiRB.Enable(FALSE);

   ox += BUTTON_WIDTH + 7 + 5;
   tbAlphaCurModelButtonID = tbSideMenuUI.CreateRadioButton("AlphaModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f, FY_NONE, CHECK_BOX);
   uiRB.ID(tbAlphaCurModelButtonID);
   uiRB.SetCaption(LEFT_CENTERED, 25, 0, "Alpha", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbRenderCurrentTabID);
   uiRB.BindCallback(TbAlphaModelUICommand);
   uiRB.Enable(FALSE);

   ox += BUTTON_WIDTH + 5;
   tbShowCurModelButtonID = tbSideMenuUI.CreateRadioButton("ShowModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f, FY_NONE, CHECK_BOX);
   uiRB.ID(tbShowCurModelButtonID);
   uiRB.SetCaption(LEFT_CENTERED, 25, 0, "Show", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbRenderCurrentTabID);
   uiRB.BindCallback(TbShowModelUICommand);
   uiRB.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbRGCurModelID = tbSideMenuUI.CreateTypein("RenderGroup", ox, oy, 20 + 55 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbRGCurModelID);
   uiTy.SetParent(tbRenderCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 45, 5, 35);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Group", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("1");
   uiTy.BindCallbacks(TbModifyRenderGroupUICommand);
   uiTy.Enable(FALSE);

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + tbRenderHeight + 1;
   tbMaterialCurrentTabID = tbSideMenuUI.CreateTab("CurrentMaterialTab", ox, oy, tbSideMenuWidth - 5, tbMatHeight, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbMaterialCurrentTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Materials", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + tbMatHeight + tbRenderHeight + 1;
   tbTexture0TabID = tbSideMenuUI.CreateTab("Texture0Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbTexture0TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 0 - Color", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + tbMatHeight + tbRenderHeight + 1 + 55;
   tbTexture1TabID = tbSideMenuUI.CreateTab("Texture1Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbTexture1TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 1 - Normal", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + tbMatHeight + tbRenderHeight + 1 + 55 + 55;
   tbTexture2TabID = tbSideMenuUI.CreateTab("Texture2Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbTexture2TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 2 - Specular", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + tbMatHeight + tbRenderHeight + 1 + 55 + 55 + 55;
   tbTexture3TabID = tbSideMenuUI.CreateTab("Texture3Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbTexture3TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 3 - AO / Lightmap", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + tbMatHeight + tbRenderHeight + 1 + 55 + 55 + 55 + 55;
   tbTexture4TabID = tbSideMenuUI.CreateTab("Texture4Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbTexture4TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 4 - Glossiness", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + tbMatHeight + tbRenderHeight + 1 + 55 + 55 + 55 + 55 + 55;
   tbTexture5TabID = tbSideMenuUI.CreateTab("Texture5Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbTexture5TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 5 - Reflection", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 101 + tbTransformHeight + tbMatHeight + tbRenderHeight + 1 + 55 + 55 + 55 + 55 + 55 + 55;
   tbMatDataTabID = tbSideMenuUI.CreateTab("MaterialDataTab", ox, oy, tbSideMenuWidth - 5, tbOneTypeinHeight*12, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbMatDataTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Material Data", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 15;
   oy = 25;
   tbUpdateMaterialID = tbSideMenuUI.CreateButton("UpdateMaterialButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[29] = tbUpdateMaterialID;
   uiB.ID(tbUpdateMaterialID);
   uiB.SetCaption(CENTERED, 0, 0, "Update", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbMaterialCurrentTabID);
   uiB.BindCallback(TbUpdateMaterialUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 30;
   tbSelectMaterialButtonID = tbSideMenuUI.CreateCombo("SelectMaterialCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                       50, tbSideMenuWidth - ox - ox/3 - 60);
   uiC.ID(tbSelectMaterialButtonID);
   uiC.SetLayer(2);
   uiC.SetParent(tbMaterialCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Material", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbRenameMaterialUICommand, TbMakeCurrentMaterialUICommand);
   uiC.Enable(FALSE);

   oy += 22;
   tbShaderFileID = tbSideMenuUI.CreateTypein("ShaderFileTypein", ox, oy, tbSideMenuWidth - ox - ox/3 - 45, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[0] = tbShaderFileID;
   uiTy.ID(tbShaderFileID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Shader", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45;
   oy += 4;
   tbShaderLoaderID = tbSideMenuUI.CreateButton("AddShaderButton", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[1] = tbShaderLoaderID;
   uiB.ID(tbShaderLoaderID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbMaterialCurrentTabID);
   uiB.BindCallback(TbAddShaderData);
   uiB.Enable(FALSE);

   ox += 20;
   tbShaderRemoveID = tbSideMenuUI.CreateButton("RemoveShaderButton", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[2] = tbShaderRemoveID;
   uiB.ID(tbShaderRemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbMaterialCurrentTabID);
   uiB.BindCallback(TbClearShaderData);
   uiB.Enable(FALSE);

#ifdef FLY2_DX9
   ox = 15;
   oy += 22;
   tbEffectID = tbSideMenuUI.CreateCombo("SelectEffectCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                          50, tbSideMenuWidth - ox - ox/3 - 60);
   tbMatUI[3] = tbEffectID;
   uiC.ID(tbEffectID);
   uiC.SetLayer(2);
   uiC.SetParent(tbMaterialCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Effect", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbMakeUpdateDirtyC, TbMakeUpdateDirtyC);
   uiC.Enable(FALSE);
#else
   tbMatUI[3] = FAILED_ID;
#endif

   ox = 15;
   oy += 22;
   tbAmbientRID = tbSideMenuUI.CreateTypein("RedAmbientTypein", ox, oy, 55 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[4] = tbAmbientRID;
   uiTy.ID(tbAmbientRID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Ambient", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14;
   tbAmbientGID = tbSideMenuUI.CreateTypein("GreenAmbientTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[5] = tbAmbientGID;
   uiTy.ID(tbAmbientGID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55;
   tbAmbientBID = tbSideMenuUI.CreateTypein("BlueAmbientTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[6] = tbAmbientBID;
   uiTy.ID(tbAmbientBID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += tbOneTypeinHeight;
   tbDiffuseRID = tbSideMenuUI.CreateTypein("RedDiffuseTypein", ox, oy, 55 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[7] = tbDiffuseRID;
   uiTy.ID(tbDiffuseRID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Diffuse", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14;
   tbDiffuseGID = tbSideMenuUI.CreateTypein("GreenDiffuseTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[8] = tbDiffuseGID;
   uiTy.ID(tbDiffuseGID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55;
   tbDiffuseBID = tbSideMenuUI.CreateTypein("BlueDiffuseTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[9] = tbDiffuseBID;
   uiTy.ID(tbDiffuseBID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += tbOneTypeinHeight;
   tbSpecularRID = tbSideMenuUI.CreateTypein("RedSpecularTypein", ox, oy, 55 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[10] = tbSpecularRID;
   uiTy.ID(tbSpecularRID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Specular", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14;
   tbSpecularGID = tbSideMenuUI.CreateTypein("GreenSpecularTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[11] = tbSpecularGID;
   uiTy.ID(tbSpecularGID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55;
   tbSpecularBID = tbSideMenuUI.CreateTypein("BlueSpecularTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[12] = tbSpecularBID;
   uiTy.ID(tbSpecularBID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += tbOneTypeinHeight;
   tbShininessID = tbSideMenuUI.CreateTypein("ShininessTypein", ox, oy, 55 + 90 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[13] = tbShininessID;
   uiTy.ID(tbShininessID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 100);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Power", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += tbOneTypeinHeight;
   tbEmissiveRID = tbSideMenuUI.CreateTypein("RedEmissiveTypein", ox, oy, 55 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[14] = tbEmissiveRID;
   uiTy.ID(tbEmissiveRID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Emissive", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14;
   tbEmissiveGID = tbSideMenuUI.CreateTypein("GreenEmissiveTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[15] = tbEmissiveGID;
   uiTy.ID(tbEmissiveGID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55;
   tbEmissiveBID = tbSideMenuUI.CreateTypein("BlueEmissiveTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[16] = tbEmissiveBID;
   uiTy.ID(tbEmissiveBID);
   uiTy.SetParent(tbMaterialCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   // generate some typeins for shader data
   oy = tbOneTypeinHeight + 2;
   for (i = 0; i < MAX_MATERIAL_DATA; i++) {
      char tName[256];
      ox = 15;
      sprintf(tName, "MaterialData%d", i*3);
      tbMatDataUI[i*3] = tbSideMenuUI.CreateTypein(tName, ox, oy, 75 + 45 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
      uiTy.ID(tbMatDataUI[i*3]);
      uiTy.SetParent(tbMatDataTabID);
      uiTy.SetLayer(2);
      uiTy.SetTypein(LEFT_CENTERED, 75, 5, 55);
      uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Data", 0.0f, 0.0f, 0.0f);
      uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
      uiTy.PasteTypeinValue("");
      uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
      uiTy.Enable(FALSE);

      ox += 75 + 45 + 14;
      sprintf(tName, "MaterialData%d", i*3 + 1);
      tbMatDataUI[i*3 + 1] = tbSideMenuUI.CreateTypein(tName, ox, oy, 60, 25, 256, 0.45f, 0.45f, 0.45f);
      uiTy.ID(tbMatDataUI[i*3 + 1]);
      uiTy.SetParent(tbMatDataTabID);
      uiTy.SetLayer(2);
      uiTy.SetTypein(LEFT_CENTERED, 1, 5, 58);
      uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
      uiTy.PasteTypeinValue("");
      uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
      uiTy.Enable(FALSE);

      ox += 65;
      sprintf(tName, "MaterialData%d", i*3 + 1);
      tbMatDataUI[i*3 + 2] = tbSideMenuUI.CreateTypein(tName, ox, oy, 60, 25, 256, 0.45f, 0.45f, 0.45f);
      uiTy.ID(tbMatDataUI[i*3 + 2]);
      uiTy.SetParent(tbMatDataTabID);
      uiTy.SetLayer(2);
      uiTy.SetTypein(LEFT_CENTERED, 1, 5, 58);
      uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
      uiTy.PasteTypeinValue("");
      uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
      uiTy.Enable(FALSE);

      oy += tbOneTypeinHeight;
   }
   tbNumShowMatDataUI = 0;

   ox = tbMatTexutreTypeinOffset;
   oy = tbOneTypeinHeight + 2;
   tbTexture0ID = tbSideMenuUI.CreateTypein("Texture0Typein", ox, oy, tbMatTexture0TypeinWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[17] = tbTexture0ID;
   uiTy.ID(tbTexture0ID);
   uiTy.SetParent(tbTexture0TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 0, 5, tbMatTexture0TypeinWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbMatTexture0TypeinWidth + tbMatTextureUIBound;
   tbTextureNumID = tbSideMenuUI.CreateTypein("TextureNumTypein", ox, oy, tbMatTextureNumWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[18] = tbTextureNumID;
   uiTy.ID(tbTextureNumID);
   uiTy.SetParent(tbTexture0TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 0, 5, tbMatTextureNumWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbMatTextureNumWidth + tbMatTextureUIBound;
   oy += 4;
   tbTexture0AddID = tbSideMenuUI.CreateButton("AddTexture0Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[19] = tbTexture0AddID;
   uiB.ID(tbTexture0AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture0TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture0RemoveID = tbSideMenuUI.CreateButton("RemoveTexture0Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[20] = tbTexture0RemoveID;
   uiB.ID(tbTexture0RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture0TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   ox = tbMatTexutreTypeinOffset;
   oy = tbOneTypeinHeight + 2;
   tbTexture1ID = tbSideMenuUI.CreateTypein("Texture1Typein", ox, oy, tbMatTextureTypeinWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[21] = tbTexture1ID;
   uiTy.ID(tbTexture1ID);
   uiTy.SetParent(tbTexture1TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 0, 5, tbMatTextureTypeinWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbMatTextureTypeinWidth + tbMatTextureUIBound;
   oy += 4;
   tbTexture1AddID = tbSideMenuUI.CreateButton("AddTexture1Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[22] = tbTexture1AddID;
   uiB.ID(tbTexture1AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture1TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture1RemoveID = tbSideMenuUI.CreateButton("RemoveTexture1Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[23] = tbTexture1RemoveID;
   uiB.ID(tbTexture1RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture1TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   ox = tbMatTexutreTypeinOffset;
   oy = tbOneTypeinHeight + 2;
   tbTexture2ID = tbSideMenuUI.CreateTypein("Texture2Typein", ox, oy, tbMatTextureTypeinWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[24] = tbTexture2ID;
   uiTy.ID(tbTexture2ID);
   uiTy.SetParent(tbTexture2TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 0, 5, tbMatTextureTypeinWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbMatTextureTypeinWidth + tbMatTextureUIBound;
   oy += 4;
   tbTexture2AddID = tbSideMenuUI.CreateButton("AddTexture2Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[25] = tbTexture2AddID;
   uiB.ID(tbTexture2AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture2TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture2RemoveID = tbSideMenuUI.CreateButton("RemoveTexture2Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[26] = tbTexture2RemoveID;
   uiB.ID(tbTexture2RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture2TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   ox = tbMatTexutreTypeinOffset;
   oy = tbOneTypeinHeight + 2;
   tbTexture3ID = tbSideMenuUI.CreateTypein("Texture3Typein", ox, oy, tbMatTextureTypeinWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[27] = tbTexture3ID;
   uiTy.ID(tbTexture3ID);
   uiTy.SetParent(tbTexture3TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 0, 5, tbMatTextureTypeinWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbMatTextureTypeinWidth + tbMatTextureUIBound;
   oy += 4;
   tbTexture3AddID = tbSideMenuUI.CreateButton("AddTexture3Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[28] = tbTexture3AddID;
   uiB.ID(tbTexture3AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture3TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture3RemoveID = tbSideMenuUI.CreateButton("RemoveTexture3Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[29] = tbTexture3RemoveID;
   uiB.ID(tbTexture3RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture3TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   ox = tbMatTexutreTypeinOffset;
   oy = tbOneTypeinHeight + 2;
   tbTexture4ID = tbSideMenuUI.CreateTypein("Texture4Typein", ox, oy, tbMatTextureTypeinWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[30] = tbTexture4ID;
   uiTy.ID(tbTexture4ID);
   uiTy.SetParent(tbTexture4TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 0, 5, tbMatTextureTypeinWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbMatTextureTypeinWidth + tbMatTextureUIBound;
   oy += 4;
   tbTexture4AddID = tbSideMenuUI.CreateButton("AddTexture4Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[31] = tbTexture4AddID;
   uiB.ID(tbTexture4AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture4TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture4RemoveID = tbSideMenuUI.CreateButton("RemoveTexture4Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[32] = tbTexture4RemoveID;
   uiB.ID(tbTexture4RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture4TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   ox = tbMatTexutreTypeinOffset;
   oy = tbOneTypeinHeight + 2;
   tbTexture5ID = tbSideMenuUI.CreateTypein("Texture5Typein", ox, oy, tbMatTextureTypeinWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[33] = tbTexture5ID;
   uiTy.ID(tbTexture5ID);
   uiTy.SetParent(tbTexture5TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 0, 5, tbMatTextureTypeinWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbMatTextureTypeinWidth + tbMatTextureUIBound;
   oy += 4;
   tbTexture5AddID = tbSideMenuUI.CreateButton("AddTexture5Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[34] = tbTexture5AddID;
   uiB.ID(tbTexture5AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture5TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture5RemoveID = tbSideMenuUI.CreateButton("RemoveTexture5Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[35] = tbTexture5RemoveID;
   uiB.ID(tbTexture5RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture5TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   // setup the connection between tab UIs
   uiT.ID(tbModelFileTabID);
   uiT.NextTab(tbModelTransformTabID);
   uiT.ID(tbModelTransformTabID);
   uiT.NextTab(tbRenderCurrentTabID);
   uiT.ID(tbRenderCurrentTabID);
   uiT.NextTab(tbMaterialCurrentTabID);
   uiT.ID(tbMaterialCurrentTabID);
   uiT.NextTab(tbTexture0TabID);
   uiT.ID(tbTexture0TabID);
   uiT.NextTab(tbTexture1TabID);
   uiT.ID(tbTexture1TabID);
   uiT.NextTab(tbTexture2TabID);
   uiT.ID(tbTexture2TabID);
   uiT.NextTab(tbTexture3TabID);
   uiT.ID(tbTexture3TabID);
   uiT.NextTab(tbTexture4TabID);
   uiT.ID(tbTexture4TabID);
   uiT.NextTab(tbTexture5TabID);
   uiT.ID(tbTexture5TabID);
   uiT.NextTab(tbMatDataTabID);

   // for character menu
   tbSideMenuCharacterPlateID = tbSideMenuUI.CreatePlate("CharacterSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuCharacterPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 2;
   oy = 2;
   tbCharacterFileTabID = tbSideMenuUI.CreateTab("CharacterFileTab", ox, oy, tbSideMenuWidth - 5, 138, 1, 0.45f, 0.45f, 0.45f, 1.0f, FALSE, TRUE);
   uiT.ID(tbCharacterFileTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuCharacterPlateID);
   uiT.SetCaption("Characters", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbLoadCharacterButtonID = tbSideMenuUI.CreateButton("LoadCharacterButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbLoadCharacterButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbCharacterFileTabID);
   uiB.BindCallback(TbLoadCharacterUICommand);

   ox = 15;
   oy += 27;
   tbSaveCharacterButtonID = tbSideMenuUI.CreateButton("SaveCharacterButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbSaveCharacterButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Save", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbCharacterFileTabID);
   uiB.BindCallback(TbSaveCharacterUICommand);
   uiB.Enable(FALSE);

   ox += (BUTTON_WIDTH + 5);
   tbExportCharacterModelButtonID = tbSideMenuUI.CreateButton("ExportCharacterButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbExportCharacterModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Export", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbCharacterFileTabID);
   uiB.BindCallback(TbExportCharacterModelUICommand);
   uiB.Enable(FALSE);

   ox += (BUTTON_WIDTH + 5);
   tbDeleteCharacterButtonID = tbSideMenuUI.CreateButton("DeleteCharacterButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbDeleteCharacterButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Delete", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbCharacterFileTabID);
   uiB.BindCallback(TbDeleteCharacterUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbSelectCharacterComboID = tbSideMenuUI.CreateCombo("SelectCharacterCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                    60, tbSideMenuWidth - ox - ox/3 - 70);
   uiC.ID(tbSelectCharacterComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbCharacterFileTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Character", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbMakeCurrentCharacterUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbBakeCharacterVAID = tbSideMenuUI.CreateButton("BakeCharacterVertexAnimation", ox, oy, 150, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbBakeCharacterVAID);
   uiB.SetCaption(CENTERED, 0, 0, "Bake VA From Action", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbCharacterFileTabID);
   uiB.BindCallback(TbCharacterVAUICommand);
   uiB.Enable(FALSE);
   
   ox = 2;
   oy = 2 + 76;
   tbCharacterCurrentTabID = tbSideMenuUI.CreateTab("CurrentCharacterTab", ox, oy, tbSideMenuWidth - 5, 165, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbCharacterCurrentTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuCharacterPlateID);
   uiT.SetCaption("Current Character", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbSelectBodyComboID = tbSideMenuUI.CreateCombo("SelectBodyCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                   60, tbSideMenuWidth - ox - ox/3 - 70);
   uiC.ID(tbSelectBodyComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbCharacterCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "   Body", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbMakeCurrentBodyUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbSelectActionComboID = tbSideMenuUI.CreateCombo("SelectActionCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                   60, tbSideMenuWidth - ox - ox/3 - 70);
   uiC.ID(tbSelectActionComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbCharacterCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "   Action", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbRenameActionUICommand, TbSelectCurrentBodyActionUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 28;
   tbEditActionButtonID = tbSideMenuUI.CreateButton("EditActionButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbEditActionButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Edit Action", 0.0f, 1.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbCharacterCurrentTabID);
   uiB.BindCallback(TbOpenAddAction);
   uiB.Enable(FALSE);

   ox += 5 + BUTTON_WIDTH;
   tbAddActionButtonID = tbSideMenuUI.CreateButton("AddActionButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddActionButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "+ Action", 0.0f, 0.0f, 1.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbCharacterCurrentTabID);
   uiB.BindCallback(TbOpenAddAction);
   uiB.Enable(FALSE);

   ox += 5 + BUTTON_WIDTH;
   tbDeleteActionButtonID = tbSideMenuUI.CreateButton("DeleteActionButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbDeleteActionButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "- Action", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbCharacterCurrentTabID);
   uiB.BindCallback(TbDeleteActionUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 28;
   tbLoadMotionButtonID = tbSideMenuUI.CreateButton("LoadCharacterMotionButton", ox, oy, BUTTON_WIDTH*3/2, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbLoadMotionButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load Motion", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbCharacterCurrentTabID);
   uiB.Enable(FALSE);
   uiB.BindCallback(TbLoadCharacterMotionUICommand);

   ox = 15;
   oy += 23;
   tbSetLocalScaleID = tbSideMenuUI.CreateTypein("LocalScaleTypein", ox, oy, 60 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSetLocalScaleID);
   uiTy.SetParent(tbCharacterCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Scale :", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbSetCharacterScaleUICommand, NULL);
   uiTy.PasteTypeinValue("1.0");
   uiTy.Enable(FALSE);

   //ox = 15;
   //oy += 28;
   //tbCalculateBBButtonID = tbSideMenuUI.CreateButton("CalculateBBButton", ox, oy, BUTTON_WIDTH*3/2, 20, 1, 0.45f, 0.45f, 0.45f);
   //uiB.ID(tbCalculateBBButtonID);
   //uiB.SetCaption(CENTERED, 0, 0, "Calculate BBox", 0.0f, 0.0f, 0.0f);
   //uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   //uiB.SetLayer(2);
   //uiB.SetParent(tbCharacterCurrentTabID);
   //uiB.BindCallback(???);
   //uiB.Enable(FALSE);

   ox = 2;
   oy = 2 + 76 + 76;
   tbPoseExploreTabID = tbSideMenuUI.CreateTab("PoseViewerTab", ox, oy, tbSideMenuWidth - 5, 103, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbPoseExploreTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuCharacterPlateID);
   uiT.SetCaption("Pose Viewer", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 15;
   oy = 25;
   tbSelectPoseComboID = tbSideMenuUI.CreateCombo("SelectPoseCombo", ox, oy, tbSideMenuWidth - 2*ox - BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                  50, tbSideMenuWidth - ox - ox/3 - 60, 10);
   uiC.ID(tbSelectPoseComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbPoseExploreTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "   Poses", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbSelectPoseUICommand, TbSelectPoseUICommand);
   uiC.Enable(FALSE);

   ox = tbSideMenuWidth - BUTTON_WIDTH - ox;
   tbDeletePoseButtonID = tbSideMenuUI.CreateButton("DeletePoseButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbDeletePoseButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Delete Pose", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbPoseExploreTabID);
   uiB.BindCallback(TbDeleteBodyPoseUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbPoseStartID = tbSideMenuUI.CreateTypein("StartPoseTypein", ox, oy, 60 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPoseStartID);
   uiTy.SetParent(tbPoseExploreTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "   Frame", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbSetBodyPoseDataUICommand, NULL);
   uiTy.PasteTypeinValue("");

   ox += 55 + 40 + 14;
   tbPoseEndID = tbSideMenuUI.CreateTypein("EndPoseTypein", ox, oy, 55 + 10, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPoseEndID);
   uiTy.SetParent(tbPoseExploreTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 10, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, ":", 0.0f, 0.0f, 0.0f);
   uiTy.BindCallbacks(TbSetBodyPoseDataUICommand, NULL);
   uiTy.PasteTypeinValue("");

   ox = tbSideMenuWidth - BUTTON_WIDTH - 15;
   oy += 2;
   tbPlayPoseButtonID = tbSideMenuUI.CreateRadioButton("PlayPoseButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbPlayPoseButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Play Pose", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbPoseExploreTabID);
   uiB.BindCallback(TbPlayTestPoseUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbPoseMotionNameID = tbSideMenuUI.CreateTypein("PoseMotionName", ox, oy, 170, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPoseMotionNameID);
   uiTy.SetParent(tbPoseExploreTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, 93);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "   Motion : ", 0.0f, 0.0f, 0.0f);
   uiTy.PasteTypeinValue("");
   uiTy.Enable(FALSE);

   ox = tbSideMenuWidth - BUTTON_WIDTH - 15;
   oy += 2;
   tbMakeBaseMoveButtonID = tbSideMenuUI.CreateButton("MakeBaseMoveButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbMakeBaseMoveButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Root -> Base", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbPoseExploreTabID);
   uiB.BindCallback(TbMakeBaseMoveUICommand);
   uiB.Enable(FALSE);

   uiT.ID(tbCharacterFileTabID);
   uiT.NextTab(tbCharacterCurrentTabID);
   uiT.ID(tbCharacterCurrentTabID);
   uiT.NextTab(tbPoseExploreTabID);

   // for FX menu
   tbSideMenuFXPlateID = tbSideMenuUI.CreatePlate("FXSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuFXPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 2;
   oy = 2;
   tbGameFXFileTabID = tbSideMenuUI.CreateTab("GameFXFileTab", ox, oy, tbSideMenuWidth - 5, 80 + 27, 1, 0.45f, 0.45f, 0.45f, 1.0f, FALSE, TRUE);
   uiT.ID(tbGameFXFileTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFXPlateID);
   uiT.SetCaption("Game FX Systems", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbNewFXSystemButtonID = tbSideMenuUI.CreateButton("NewGameFXSystemButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbNewFXSystemButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Create", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbGameFXFileTabID);
   uiB.BindCallback(TbOpenAddGameFXSystem);

   ox = 15;
   oy += 27;
   tbLoadFXSystemButtonID = tbSideMenuUI.CreateButton("LoadGameFXSystemButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbLoadFXSystemButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbGameFXFileTabID);
   uiB.BindCallback(TbLoadFXSystemUICommand);

   ox += BUTTON_WIDTH + 5;
   tbSaveFXSystemButtonID = tbSideMenuUI.CreateButton("SaveGameFXSystemButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbSaveFXSystemButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Save", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbGameFXFileTabID);
   uiB.BindCallback(TbSaveFXSystemUICommand);
   uiB.Enable(FALSE);

   ox += BUTTON_WIDTH + 5;
   tbDeleteFXSystemButtonID = tbSideMenuUI.CreateButton("DeleteGameFXSystemButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbDeleteFXSystemButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Delete", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbGameFXFileTabID);
   uiB.BindCallback(TbDeleteFXSystemUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbSelectFXSystemComboID = tbSideMenuUI.CreateCombo("SelectFXSystemCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                      60, tbSideMenuWidth - ox - ox/3 - 70);
   uiC.ID(tbSelectFXSystemComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbGameFXFileTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "FX System", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbRenameCurrentFXSystemUICommand, TbMakeCurrentFXSystemUICommand);
   uiC.Enable(FALSE);

   ox = 2;
   oy = 2 + 76 + 27;
   tbFXSystemCurrentTabID = tbSideMenuUI.CreateTab("CurrentGameFXSystemTab", ox, oy, tbSideMenuWidth - 5, 220, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbFXSystemCurrentTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFXPlateID);
   uiT.SetCaption("FX System : None", 0.2f, 0.2f, 0.2f);

   ox = 15;
   oy = 25;
   tbNewFXButtonID = tbSideMenuUI.CreateButton("NewGameFXButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbNewFXButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "New FX", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbFXSystemCurrentTabID);
   uiB.BindCallback(TbOpenAddGameFX);

   ox += BUTTON_WIDTH + 5;
   tbDeleteFXButtonID = tbSideMenuUI.CreateButton("DeleteGameFXButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbDeleteFXButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Delete", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbFXSystemCurrentTabID);
   uiB.BindCallback(TbDeleteFXUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbSelectFXComboID = tbSideMenuUI.CreateCombo("SelectFXCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                 30, tbSideMenuWidth - ox - ox/3 - 40);
   uiC.ID(tbSelectFXComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbFXSystemCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "FX", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbRenameCurrentFXUICommand, TbMakeCurrentFXUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbBaseFXFXPosID[0] = tbSideMenuUI.CreateTypein("BaseFXPosXTypein", ox, oy, 50 + 55 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbBaseFXFXPosID[0]);
   uiTy.SetParent(tbFXSystemCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 55);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Position", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyBasePositionUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXFXPosID[0];
   tbNumGameKeyUI++;

   ox += 63 + 40 + 14;
   tbBaseFXFXPosID[1] = tbSideMenuUI.CreateTypein("BaseFXPosYTypein", ox, oy, 58, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbBaseFXFXPosID[1]);
   uiTy.SetParent(tbFXSystemCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyBasePositionUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXFXPosID[1];
   tbNumGameKeyUI++;

   ox += 63;
   tbBaseFXFXPosID[2] = tbSideMenuUI.CreateTypein("BaseFXPosZTypein", ox, oy, 58, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbBaseFXFXPosID[2]);
   uiTy.SetParent(tbFXSystemCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyBasePositionUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXFXPosID[2];
   tbNumGameKeyUI++;

   ox += 63;
   oy += 4;
   tbBaseFXFXPosEditKeyID = tbSideMenuUI.CreateButton("EditBaseFXPositionKey", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbBaseFXFXPosEditKeyID);
   uiB.SetCaption(CENTERED, 0, 0, "k", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbFXSystemCurrentTabID);
   uiB.BindCallback(TbOpenKeyframeEditor);
   uiB.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXFXPosEditKeyID;
   tbNumGameKeyUI++;

   ox = 15;
   oy += 25;
   tbBaseFXFXRotateID[0] = tbSideMenuUI.CreateTypein("BaseFXRotXTypein", ox, oy, 50 + 55 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbBaseFXFXRotateID[0]);
   uiTy.SetParent(tbFXSystemCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 55);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Rotation", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyBaseRotationUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXFXRotateID[0];
   tbNumGameKeyUI++;

   ox += 63 + 40 + 14;
   tbBaseFXFXRotateID[1] = tbSideMenuUI.CreateTypein("BaseFXRotYTypein", ox, oy, 58, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbBaseFXFXRotateID[1]);
   uiTy.SetParent(tbFXSystemCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyBaseRotationUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXFXRotateID[1];
   tbNumGameKeyUI++;

   ox += 63;
   tbBaseFXFXRotateID[2] = tbSideMenuUI.CreateTypein("BaseFXRotZTypein", ox, oy, 58, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbBaseFXFXRotateID[2]);
   uiTy.SetParent(tbFXSystemCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyBaseRotationUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXFXRotateID[2];
   tbNumGameKeyUI++;

   ox += 63;
   oy += 4;
   tbBaseFXFXRotateEditKeyID = tbSideMenuUI.CreateButton("EditBaseFXRotationKey", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbBaseFXFXRotateEditKeyID);
   uiB.SetCaption(CENTERED, 0, 0, "k", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbFXSystemCurrentTabID);
   uiB.BindCallback(TbOpenKeyframeEditor);
   uiB.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXFXRotateEditKeyID;
   tbNumGameKeyUI++;

   ox = 15;
   oy += 25;
   tbBaseFXShowID = tbSideMenuUI.CreateTypein("BaseFXShowTypein", ox, oy, 50 + 55 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbBaseFXShowID);
   uiTy.SetParent(tbFXSystemCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 55);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Show", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("1.0");
   uiTy.BindCallbacks(TbModifyBaseVisibilityUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXShowID;
   tbNumGameKeyUI++;

   ox += 63 + 40 + 14;
   oy += 4;
   tbBaseFXShowEditKeyID = tbSideMenuUI.CreateButton("EditBaseFXShowKey", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbBaseFXShowEditKeyID);
   uiB.SetCaption(CENTERED, 0, 0, "k", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbFXSystemCurrentTabID);
   uiB.BindCallback(TbOpenKeyframeEditor);
   uiB.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbBaseFXShowEditKeyID;
   tbNumGameKeyUI++;

   ox = 15;
   oy += 25;
   tbBaseLifeID = tbSideMenuUI.CreateTypein("BaseFXLifeTypein", ox, oy, 50 + 55 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbBaseLifeID);
   uiTy.SetParent(tbFXSystemCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 55);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Life", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyBaseLifeUICommand);
   uiTy.Enable(FALSE);

   ox += 50 + 55 + 10;
   oy += 3;
   tbBaseLifeAutoAdjustID = tbSideMenuUI.CreateRadioButton("AutoLifeScaleButton", ox, oy, 120, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f, FY_NONE, CHECK_BOX);
   uiRB.ID(tbBaseLifeAutoAdjustID);
   uiRB.SetCaption(LEFT_CENTERED, 25, 0, "Auto Life Scale", 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbFXSystemCurrentTabID);
   uiRB.ChangeState(TRUE);
   uiRB.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbSetFXParentID = tbSideMenuUI.CreateCombo("SetParentForFXCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                               80, tbSideMenuWidth - ox - ox/3 - 90, 10, TRUE, 1024);
   uiC.ID(tbSetFXParentID);
   uiC.SetParent(tbFXSystemCurrentTabID);
   uiC.SetLayer(10);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Set Parent", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(TbSetFXParentComboUICommand, TbSetFXParentComboUICommand, TbSetFXParentTypeinUI);
   uiC.Enable(FALSE);

   // for a Plate FX UIs
   ox = 2;
   oy = 2 + 76 + 27 + 145 + 20 + 25;
   tbEditPlateFXTabID = tbSideMenuUI.CreateTab("EditPlateFXTab", ox, oy, tbSideMenuWidth - 5, 200, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbEditPlateFXTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFXPlateID);
   uiT.SetCaption("Current FX", 0.2f, 0.2f, 0.2f);

   ox = 15;
   oy = 27;
   tbPlateFXTypeID = tbSideMenuUI.CreateCombo("SelectPlateFXTypeCombo", ox, oy, tbSideMenuWidth/2 + 50, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                               50, tbSideMenuWidth/2, 3, FALSE);
   uiC.ID(tbPlateFXTypeID);
   uiC.SetLayer(2);
   uiC.SetParent(tbEditPlateFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Type", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Plate Z-up");
   uiC.InsertMessage(1, "Billboard");
   uiC.InsertMessage(2, "Plate Y-up");
   uiC.PasteComboValue("Plate Z-up");
   uiC.BindCallbacks(NULL, TbChangePlateTypeUICommand);
   uiC.Enable(FALSE);

   oy += 25;
   tbPlateFXTextureNameID = tbSideMenuUI.CreateTypein("TextureOnPlate", ox, oy, tbSideMenuWidth - ox - ox/3 - 45 - 50, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXTextureNameID);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40 - 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Textures", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbLoadPlateFXTextureUICommand);
   uiTy.PasteTypeinValue("-");
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45 - 50 + 3;
   tbPlateFXTextureNumberID = tbSideMenuUI.CreateTypein("TextureNumberOnPlate", ox, oy, 42, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXTextureNumberID);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 2, 5, 38);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0");
   uiTy.BindCallbacks(TbLoadPlateFXTextureUICommand);
   uiTy.Enable(FALSE);

   ox += 42 + 5;
   oy += 4;
   tbPlateFXTextureAddID = tbSideMenuUI.CreateButton("AddTextureOnPlate", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbPlateFXTextureAddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditPlateFXTabID);
   uiB.BindCallback(TbAddPlateFXTexture);
   uiB.Enable(FALSE);

   ox += 20;
   tbPlateFXTextureRemoveID = tbSideMenuUI.CreateButton("RemoveTextureOnPlate", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbPlateFXTextureRemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditPlateFXTabID);
   uiB.BindCallback(TbRemovePlateFXTextureUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbPlateFXAlphamapNameID = tbSideMenuUI.CreateTypein("AlphamapOnPlate", ox, oy, tbSideMenuWidth - ox - ox/3 - 45 - 50, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXAlphamapNameID);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40 - 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Alphamap", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbLoadPlateFXAlphamapUICommand);
   uiTy.PasteTypeinValue("-");
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45 - 50 + 3 + 42 + 5;
   oy += 4;
   tbPlateFXAlphamapAddID = tbSideMenuUI.CreateButton("AddAlphaOnPlate", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbPlateFXAlphamapAddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditPlateFXTabID);
   uiB.BindCallback(TbAddPlateFXAlphamap);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbPlateFXBlendTypeID = tbSideMenuUI.CreateCombo("PlateFXBlendTypeCombo", ox, oy, tbSideMenuWidth/2 + 50, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                    50, 130, 3, FALSE);
   uiC.ID(tbPlateFXBlendTypeID);
   uiC.SetLayer(2);
   uiC.SetParent(tbEditPlateFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Blending", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Alpha");
   uiC.InsertMessage(1, "Add Color");
   uiC.InsertMessage(2, "Subtract Color");
   uiC.PasteComboValue("Add Color");
   uiC.BindCallbacks(NULL, TbChangePlateBlendTypeUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbPlateFXSizeID[0] = tbSideMenuUI.CreateTypein("PlateFXWidthTypein", ox, oy, 55 + 40 + 14 + 40, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXSizeID[0]);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 48 + 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Size", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(TbModifyPlateSizeUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbPlateFXSizeID[0];
   tbNumGameKeyUI++;

   ox += 55 + 40 + 14 + 40;
   tbPlateFXSizeID[1] = tbSideMenuUI.CreateTypein("PlateFXHeightTypein", ox, oy, 50 + 40, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXSizeID[1]);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 48 + 40);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(TbModifyPlateSizeUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbPlateFXSizeID[1];
   tbNumGameKeyUI++;

   ox += 50 + 40 + 7;
   oy += 4;
   tbPlateFXSizeEditKeyID = tbSideMenuUI.CreateButton("EditPlateSizeKey", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbPlateFXSizeEditKeyID);
   uiB.SetCaption(CENTERED, 0, 0, "k", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditPlateFXTabID);
   uiB.BindCallback(TbOpenKeyframeEditor);
   uiB.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbPlateFXSizeEditKeyID;
   tbNumGameKeyUI++;

   ox = 15;
   oy += 27;
   tbPlateFXColorID[0] = tbSideMenuUI.CreateTypein("PlateFXColorRedTypein", ox, oy, 50 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXColorID[0]);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Color", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("1.0");
   uiTy.BindCallbacks(TbModifyPlateColorUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbPlateFXColorID[0];
   tbNumGameKeyUI++;

   ox += 48 + 40 + 14;
   tbPlateFXColorID[1] = tbSideMenuUI.CreateTypein("PlateFXColorGreenTypein", ox, oy, 43, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXColorID[1]);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 40);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("1.0");
   uiTy.BindCallbacks(TbModifyPlateColorUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbPlateFXColorID[1];
   tbNumGameKeyUI++;

   ox += 48;
   tbPlateFXColorID[2] = tbSideMenuUI.CreateTypein("PlateFXColorBlueTypein", ox, oy, 43, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXColorID[2]);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 40);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("1.0");
   uiTy.BindCallbacks(TbModifyPlateColorUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbPlateFXColorID[2];
   tbNumGameKeyUI++;

   ox += 48;
   tbPlateFXColorID[3] = tbSideMenuUI.CreateTypein("PlateFXColorAlphaTypein", ox, oy, 43, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPlateFXColorID[3]);
   uiTy.SetParent(tbEditPlateFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 40);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("1.0");
   uiTy.BindCallbacks(TbModifyPlateColorUICommand);
   uiTy.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbPlateFXColorID[3];
   tbNumGameKeyUI++;

   ox += 43 + 5;
   oy += 4;
   tbPlateFXColorEditKeyID = tbSideMenuUI.CreateButton("EditPlateColorKey", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbPlateFXColorEditKeyID);
   uiB.SetCaption(CENTERED, 0, 0, "k", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditPlateFXTabID);
   uiB.BindCallback(TbOpenKeyframeEditor);
   uiB.Enable(FALSE);
   tbGameKeyUI[tbNumGameKeyUI] = tbPlateFXColorEditKeyID;
   tbNumGameKeyUI++;

   // for a Particle FX UIs
   ox = 2;
   oy = 2 + 76 + 27 + 145 + 20 + 25;
   tbEditParticleFXTabID = tbSideMenuUI.CreateTab("EditParticleFXTab", ox, oy, tbSideMenuWidth - 5, 280, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbEditParticleFXTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFXPlateID);
   uiT.SetCaption("Current FX", 0.2f, 0.2f, 0.2f);

   ox = 15;
   oy = 29;
   tbParticleLifeID[0] = tbSideMenuUI.CreateTypein("ParticleLife", ox, oy, 100, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleLifeID[0]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Life", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbModifyParticleLifeUICommand);
   uiTy.PasteTypeinValue("60.0");
   uiTy.Enable(FALSE);

   ox += 105;
   tbParticleLifeID[1] = tbSideMenuUI.CreateTypein("ParticleLifeVar", ox, oy, 70, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleLifeID[1]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 25, 5, 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "+/-", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbModifyParticleLifeUICommand);
   uiTy.PasteTypeinValue("10.0");
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbParticleBornID[0] = tbSideMenuUI.CreateTypein("ParticleBorn", ox, oy, 100, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleBornID[0]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Born", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbModifyParticleBornUICommand);
   uiTy.PasteTypeinValue("2");
   uiTy.Enable(FALSE);

   ox += 105;
   tbParticleBornID[1] = tbSideMenuUI.CreatePlate("ParticleLifeVar", ox, oy, 70, 25, 256, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbParticleBornID[1]);
   uiP.SetParent(tbEditParticleFXTabID);
   uiP.SetLayer(2);
   uiP.SetCaption(LEFT_CENTERED, 2, 5, "/ Frame", 0.0f, 0.0f, 0.0f);
   uiP.Enable(FALSE);

   ox = 15;
   oy += 29;
   tbParticleBlendTypeID = tbSideMenuUI.CreateCombo("ParticleBlendTypeCombo", ox, oy, tbSideMenuWidth/2 + 50, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                    50, 130, 3, FALSE);
   uiC.ID(tbParticleBlendTypeID);
   uiC.SetLayer(2);
   uiC.SetParent(tbEditParticleFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Blending", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Alpha");
   uiC.InsertMessage(1, "Add Color");
   uiC.InsertMessage(2, "Subtract Color");
   uiC.PasteComboValue("Add Color");
   uiC.BindCallbacks(NULL, TbChangeParticleBlendTypeUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbParticleSizeID[0] = tbSideMenuUI.CreateTypein("ParticleWidthTypein", ox, oy, 55 + 40 + 14 + 40, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleSizeID[0]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 48 + 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Size", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("3.0");
   uiTy.BindCallbacks(TbModifyParticleSizeUICommand);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14 + 40;
   tbParticleSizeID[1] = tbSideMenuUI.CreateTypein("ParticleHeightTypein", ox, oy, 50 + 40, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleSizeID[1]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 48 + 40);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("3.0");
   uiTy.BindCallbacks(TbModifyParticleSizeUICommand);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbParticleVelocityID[0] = tbSideMenuUI.CreateTypein("ParticleVelXTypein", ox, oy, 50 + 55 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleVelocityID[0]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 55);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Velocity", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyParticleVelocityUICommand);
   uiTy.Enable(FALSE);

   ox += 63 + 40 + 14;
   tbParticleVelocityID[1] = tbSideMenuUI.CreateTypein("ParticleVelYTypein", ox, oy, 58, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleVelocityID[1]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyParticleVelocityUICommand);
   uiTy.Enable(FALSE);

   ox += 63;
   tbParticleVelocityID[2] = tbSideMenuUI.CreateTypein("ParticleVelZTypein", ox, oy, 58, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleVelocityID[2]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyParticleVelocityUICommand);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 27;
   tbParticleVelocityVarID[0] = tbSideMenuUI.CreateTypein("ParticleVelVarXTypein", ox, oy, 50 + 55 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleVelocityVarID[0]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2); 
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 55);
   uiTy.SetCaption(LEFT_CENTERED, 25, 5, "+/-", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("3.0");
   uiTy.BindCallbacks(TbModifyParticleVelocityUICommand);
   uiTy.Enable(FALSE);

   ox += 63 + 40 + 14;
   tbParticleVelocityVarID[1] = tbSideMenuUI.CreateTypein("ParticleVelVarYTypein", ox, oy, 58, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleVelocityVarID[1]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("3.0");
   uiTy.BindCallbacks(TbModifyParticleVelocityUICommand);
   uiTy.Enable(FALSE);

   ox += 63;
   tbParticleVelocityVarID[2] = tbSideMenuUI.CreateTypein("ParticleVelVarZTypein", ox, oy, 58, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleVelocityVarID[2]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("3.0");
   uiTy.BindCallbacks(TbModifyParticleVelocityUICommand);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbParticleTextureNameID = tbSideMenuUI.CreateTypein("TextureOnParticle", ox, oy, tbSideMenuWidth - ox - ox/3 - 45 - 50, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleTextureNameID);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40 - 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Textures", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbLoadParticleTextureUICommand);
   uiTy.PasteTypeinValue("-");
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45 - 50 + 3;
   oy += 4;
   tbParticleTextureAddID = tbSideMenuUI.CreateButton("AddTextureOnParticle", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbParticleTextureAddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditParticleFXTabID);
   uiB.BindCallback(TbAddParticleTexture);
   uiB.Enable(FALSE);

   ox += 20;
   tbParticleTextureRemoveID = tbSideMenuUI.CreateButton("RemoveTextureOnParticle", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbParticleTextureRemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditParticleFXTabID);
   uiB.BindCallback(TbRemoveParticleTextureUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbParticleAlphamapNameID = tbSideMenuUI.CreateTypein("AlphamapOnParticle", ox, oy, tbSideMenuWidth - ox - ox/3 - 45 - 50, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleAlphamapNameID);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40 - 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Alphamap", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbLoadParticleAlphamapUICommand);
   uiTy.PasteTypeinValue("-");
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45 - 50 + 3;
   oy += 4;
   tbParticleAlphamapAddID = tbSideMenuUI.CreateButton("AddAlphamapOnParticle", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbParticleAlphamapAddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditParticleFXTabID);
   uiB.BindCallback(TbAddParticleAlphamap);
   uiB.Enable(FALSE);


   ox = 15;
   oy += 25;
   tbParticleTextureAnimID[0] = tbSideMenuUI.CreateTypein("TextureAnimation", ox, oy, 110, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleTextureAnimID[0]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 55);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "TexAnim", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbModifyParticleAnimUICommand);
   uiTy.PasteTypeinValue("1");
   uiTy.Enable(FALSE);

   ox += 115;
   tbParticleTextureAnimID[1] = tbSideMenuUI.CreateTypein("TextureAnimationU", ox, oy, 65, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleTextureAnimID[1]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 5, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbModifyParticleAnimUICommand);
   uiTy.PasteTypeinValue("1");
   uiTy.Enable(FALSE);

   ox += 70;
   tbParticleTextureAnimID[2] = tbSideMenuUI.CreateTypein("TextureAnimationV", ox, oy, 65, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbParticleTextureAnimID[2]);
   uiTy.SetParent(tbEditParticleFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 5, 5, 55);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbModifyParticleAnimUICommand);
   uiTy.PasteTypeinValue("1");
   uiTy.Enable(FALSE);

   // for a force FX UIs
   ox = 2;
   oy = 2 + 76 + 27 + 145 + 20 + 25;
   tbEditForceFXTabID = tbSideMenuUI.CreateTab("EditForceFXTab", ox, oy, tbSideMenuWidth - 5, 200, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbEditForceFXTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFXPlateID);
   uiT.SetCaption("Current FX", 0.2f, 0.2f, 0.2f);

   ox = 15;
   oy = 27;
   tbForceFXTypeID = tbSideMenuUI.CreateCombo("SelectForceTypeCombo", ox, oy, tbForceTypeWidth, 20, 10, 0.45f, 0.45f, 0.45f, 1.0f,
                                               tbForceTypeTextWidth, tbForceTypeWidth-tbForceTypeTextWidth-14, 9, FALSE);
   uiC.ID(tbForceFXTypeID);
   uiC.SetLayer(9);
   uiC.SetParent(tbEditForceFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Type", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Gravity");
   uiC.InsertMessage(1, "Parallel");
   uiC.InsertMessage(2, "Sink");
   uiC.InsertMessage(3, "Sphere");
   uiC.InsertMessage(4, "Vortex");
   uiC.InsertMessage(5, "Vortex 2D");
   uiC.InsertMessage(6, "Viscosity");
   uiC.InsertMessage(7, "User Defined");
   uiC.PasteComboValue("Gravity");
   uiC.BindCallbacks(NULL, TbModifyForceParamUICommand);
   uiC.Enable(FALSE);

   oy += 25;
   tbForceUsedByID = tbSideMenuUI.CreateCombo("ForceUsedByCombo", ox, oy, tbForceUsedByWidth, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                               tbForceUsedByTextWidth, tbForceUsedByWidth-tbForceUsedByTextWidth-14);
   uiC.ID(tbForceUsedByID);
   uiC.SetParent(tbEditForceFXTabID);
   uiC.SetLayer(10);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Used By", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "-");
   uiC.PasteComboValue("-");
   uiC.BindCallbacks(NULL, TbChangeForceParticleUICommand);
   uiC.Enable(FALSE);

   oy += 25;
   tbForceMagnitudeID = tbSideMenuUI.CreateTypein("ForceMagnitude", ox, oy, 150, 25, 3, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbForceMagnitudeID);
   uiTy.SetParent(tbEditForceFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 80, 5, 67);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Magnitude", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyForceParamUICommand);
   uiTy.Enable(FALSE);
   
   oy += 25;
   tbForceMinDistID = tbSideMenuUI.CreateTypein("ForceMinDist", ox, oy, tbForceMinDistWidth, 25, 3, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbForceMinDistID);
   uiTy.SetParent(tbEditForceFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbForceMinDistTextWidth, 5, tbForceMinDistWidth-tbForceMinDistTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Min Distance", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbModifyForceParamUICommand);
   uiTy.Enable(TRUE);

   // for a Geometry FX UIs
   ox = 2;
   oy = 2 + 76 + 27 + 145 + 20 + 25;
   tbEditGeometryFXTabID = tbSideMenuUI.CreateTab("EditGeometryFXTab", ox, oy, tbSideMenuWidth - 5, 140, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbEditGeometryFXTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFXPlateID);
   uiT.SetCaption("Current Geometry FX", 0.2f, 0.2f, 0.2f);

   ox = 15;
   oy = 27;
   tbLoadGeometryFXID = tbSideMenuUI.CreateButton("LoadGeometryFXButton", ox, oy, 70, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbLoadGeometryFXID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditGeometryFXTabID);
   uiB.BindCallback(TbGameFXGeometryLoadBtnCallback);
   uiB.Enable(FALSE);

   ox += 75;
   oy -= 2;
   tbGeometryNameFXID = tbSideMenuUI.CreateTypein("GeometryName", ox, oy, tbSideMenuWidth - 70 - 25, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbGeometryNameFXID);
   uiTy.SetParent(tbEditGeometryFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 5, 5, tbSideMenuWidth - 70 - 20 - 15);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("-");
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 29;
   tbGeometryFXBlendTypeID = tbSideMenuUI.CreateCombo("GeometryFXBlendTypeCombo", ox, oy, tbSideMenuWidth/2 + 50, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                       50, 130, 3, FALSE);
   uiC.ID(tbGeometryFXBlendTypeID);
   uiC.SetLayer(2);
   uiC.SetParent(tbEditGeometryFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Blending", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Alpha");
   uiC.InsertMessage(1, "Add Color");
   uiC.InsertMessage(2, "Subtract Color");
   uiC.PasteComboValue("Add Color");
   uiC.BindCallbacks(NULL, TbChangeGeometryBlendTypeUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbGeometryFXTextureNameID = tbSideMenuUI.CreateTypein("TextureOnGeometry", ox, oy, tbSideMenuWidth - ox - ox/3 - 45 - 50, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbGeometryFXTextureNameID);
   uiTy.SetParent(tbEditGeometryFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40 - 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Textures", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbLoadGeometryFXTextureUICommand);
   uiTy.PasteTypeinValue("-");
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45 - 50 + 3;
   tbGeometryFXTextureNumberID = tbSideMenuUI.CreateTypein("TextureNumberOnGeometry", ox, oy, 42, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbGeometryFXTextureNumberID);
   uiTy.SetParent(tbEditGeometryFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 2, 5, 38);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0");
   uiTy.BindCallbacks(TbLoadGeometryFXTextureUICommand);
   uiTy.Enable(FALSE);

   ox += 42 + 5;
   oy += 4;
   tbGeometryFXTextureAddID = tbSideMenuUI.CreateButton("AddTextureOnGeometry", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbGeometryFXTextureAddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditGeometryFXTabID);
   uiB.BindCallback(TbAddGeometryFXTexture);
   uiB.Enable(FALSE);

   ox += 20;
   tbGeometryFXTextureRemoveID = tbSideMenuUI.CreateButton("RemoveTextureOnGeometry", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbGeometryFXTextureRemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditGeometryFXTabID);
   uiB.BindCallback(TbRemoveGeometryFXTextureUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 25;
   tbGeometryFXAlphamapNameID = tbSideMenuUI.CreateTypein("AlphamapOnGeometry", ox, oy, tbSideMenuWidth - ox - ox/3 - 45 - 50, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbGeometryFXAlphamapNameID);
   uiTy.SetParent(tbEditGeometryFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40 - 45);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Alphamap", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbLoadGeometryFXAlphamapUICommand);
   uiTy.PasteTypeinValue("-");
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45 - 50 + 3;
   ox += 42 + 5;
   oy += 4;
   tbGeometryFXAlphamapAddID = tbSideMenuUI.CreateButton("AddAlphamapOnGeometry", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbGeometryFXAlphamapAddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditGeometryFXTabID);
   uiB.BindCallback(TbAddGeometryFXAlphamap);
   uiB.Enable(FALSE);


   // for an Audio FX UIs
   ox = 2;
   oy = 2 + 76 + 27 + 145 + 20 + 25;
   tbEditAudioFXTabID = tbSideMenuUI.CreateTab("EditAudioFXTab", ox, oy, tbSideMenuWidth - 5, tbAudioTabHeight, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbEditAudioFXTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFXPlateID);
   uiT.SetCaption("Current Audio FX", 0.2f, 0.2f, 0.2f);
   
   ox = 15;
   oy = 25;
   tbAudioFXNameID = tbSideMenuUI.CreateTypein("AudioFXName", ox, oy, tbAudioNameWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbAudioFXNameID);
   uiTy.SetParent(tbEditAudioFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbAudioTextWidth, 5, tbAudioNameWidth-tbAudioTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Audio", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGameFXAudioLoadToCommand);
   uiTy.PasteTypeinValue("");
   uiTy.Enable(FALSE);
   
   ox += tbAudioNameWidth + 5;
   tbAudioFXLoadButtonID = tbSideMenuUI.CreateButton("LoadAudioFXButton", ox, oy+2, tbAudioButtonWidth, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAudioFXLoadButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditAudioFXTabID);
   uiB.BindCallback(TbGameFXAudioLoadBtnCallback);
   uiB.Enable(FALSE);
   
   ox = 15;
   oy += 25 + tbAudioUIBound;
   tbAudioFXStartID = tbSideMenuUI.CreateTypein("AudioStartFrame", ox, oy, tbAudioStartWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbAudioFXStartID);
   uiTy.SetParent(tbEditAudioFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbAudioStartTextWidth, 5, tbAudioStartWidth-tbAudioStartTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Start Frame", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGameFXAudioChangeStartCallback);
   uiTy.PasteTypeinValue("0.00");
   uiTy.Enable(FALSE);

   // for a Sword FX UIs
   ox = 2;
   oy = 2 + 76 + 27 + 145 + 20 + 25;
   tbEditSwordFXTabID = tbSideMenuUI.CreateTab("EditSwordFXTab", ox, oy, tbSideMenuWidth - 5, tbSwordTabHeight, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.ID(tbEditSwordFXTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuFXPlateID);
   uiT.SetCaption("Current Sword FX", 0.2f, 0.2f, 0.2f);

   ox = 10;
   oy = 27;
   tbSwordFXEnableRButtonID = tbSideMenuUI.CreateRadioButton("SwordFXEnableButton", ox, oy, tbSwordEnableWidth, 25, 1, 0.45f, 0.45f, 0.45f, 1.0f, FY_NONE, CHECK_BOX);
   uiRB.ID(tbSwordFXEnableRButtonID);
   uiRB.SetCaption(LEFT_CENTERED, 25, 0, "Enable", 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbEditSwordFXTabID);
   uiRB.ChangeState(TRUE);
   uiRB.BindCallback(TbGameFXSwordChangeParamButtonCallback);
   uiRB.Enable(FALSE);
   ox = 15;
   oy += 25 + tbSwordUIBound;
   tbSwordFXCharacterComboID = tbSideMenuUI.CreateCombo("SelectCharacterCombo", ox, oy, tbSwordCharNameWidth, 20, 10, 0.45f, 0.45f, 0.45f, 1.0f,
                                                        tbSwordCharNameTextWidth, tbSwordCharNameWidth-tbSwordCharNameTextWidth-14, 5);
   uiC.ID(tbSwordFXCharacterComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbEditSwordFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Character", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(TbGameFXSwordSelectCharComboCallback, TbGameFXSwordSelectCharComboCallback);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 20 + tbSwordUIBound;
   tbSwordFXDummy1ComboID = tbSideMenuUI.CreateCombo("SelectDommy1Combo", ox, oy, tbSwordDummy1Width, 20, 10, 0.45f, 0.45f, 0.45f, 1.0f,
                                                     tbSwordDummy1TextWidth, tbSwordDummy1Width-tbSwordDummy1TextWidth-14, 5);
   uiC.ID(tbSwordFXDummy1ComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbEditSwordFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Dummy 1", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(TbGameFXSwordSelectDummy1ComboCallback, TbGameFXSwordSelectDummy1ComboCallback, TbGameFXSwordSelectDummy1SearchCallback);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 20 + tbSwordUIBound;
   tbSwordFXDummy2ComboID = tbSideMenuUI.CreateCombo("SelectDommy2Combo", ox, oy, tbSwordDummy2Width, 20, 10, 0.45f, 0.45f, 0.45f, 1.0f,
                                                     tbSwordDummy2TextWidth, tbSwordDummy2Width-tbSwordDummy2TextWidth-14, 5);
   uiC.ID(tbSwordFXDummy2ComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbEditSwordFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Dummy 2", 0.0f, 0.0f, 0.0f);
   uiC.PasteComboValue("");
   uiC.BindCallbacks(TbGameFXSwordSelectDummy2ComboCallback, TbGameFXSwordSelectDummy2ComboCallback, TbGameFXSwordSelectDummy2SearchCallback);
   uiC.Enable(FALSE);
   
   ox = 15;
   oy += 25 + tbSwordUIBound;
   tbSwordFXTailLengthTypeinID = tbSideMenuUI.CreateTypein("SwordTailLength", ox, oy, tbSwordTailLengthWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXTailLengthTypeinID);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbSwordTailLengthTextWidth, 5, tbSwordTailLengthWidth-tbSwordTailLengthTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Tail         Length", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGameFXSwordChangeParamTypeinCallback);
   uiTy.PasteTypeinValue("6");
   uiTy.Enable(FALSE);
   
   ox = tbSwordTailDivisionOffset;
   oy += 25 + tbSwordUIBound;
   tbSwordFXTailDivisionTypeinID = tbSideMenuUI.CreateTypein("SwordTailDivision", ox, oy, tbSwordTailDivisionWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXTailDivisionTypeinID);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbSwordTailDivisionTextWidth, 5, tbSwordTailDivisionWidth-tbSwordTailDivisionTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Division", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGameFXSwordChangeParamTypeinCallback);
   uiTy.PasteTypeinValue("3");
   uiTy.Enable(FALSE);

   ox += tbSwordTailDivisionWidth + (tbSwordUIBound<<1);
   tbSwordFXTailSegmentTypeinID = tbSideMenuUI.CreateTypein("SwordTailSegment", ox, oy, tbSwordTailSegmentWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXTailSegmentTypeinID);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbSwordTailSegmentTextWidth, 5, tbSwordTailSegmentWidth-tbSwordTailSegmentTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Segment", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGameFXSwordChangeParamTypeinCallback);
   uiTy.PasteTypeinValue("4");
   uiTy.Enable(FALSE);
   
   ox = 15;
   oy += 25 + tbSwordUIBound;
   tbSwordFXTextureNameTypeinID = tbSideMenuUI.CreateTypein("SwordTextureName", ox, oy, tbSwordTextureNameWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXTextureNameTypeinID);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbSwordTextureTextWidth, 5, tbSwordTextureNameWidth-tbSwordTextureTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Texture", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGameFXSwordTextureTypeinCallback);
   uiTy.PasteTypeinValue("");
   uiTy.Enable(FALSE);
   
   ox += tbSwordTextureNameWidth + tbSwordUIBound;
   tbSwordFXTextureNumTypeinID = tbSideMenuUI.CreateTypein("SwordTextureNumber", ox, oy, tbSwordTextureNumWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXTextureNumTypeinID);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 0, 5, tbSwordTextureNumWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGameFXSwordTextureTypeinCallback);
   uiTy.PasteTypeinValue("0");
   uiTy.Enable(FALSE);

   ox += tbSwordTextureNumWidth + (tbSwordUIBound<<1);
   tbSwordFXTextureAddButtonID = tbSideMenuUI.CreateButton("AddSwordFXTextureButton", ox, oy+4, tbSwordTextureButtonWidth, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbSwordFXTextureAddButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditSwordFXTabID);
   uiB.BindCallback(TbGameFXSwordAddBtnCallback);
   uiB.Enable(FALSE);

   ox += tbSwordTextureButtonWidth + tbSwordUIBound;
   tbSwordFXTextureRemoveButtonID = tbSideMenuUI.CreateButton("RemoveSwordFXTextureButton", ox, oy+4, tbSwordTextureButtonWidth, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbSwordFXTextureRemoveButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditSwordFXTabID);
   uiB.BindCallback(TbGameFXSwordRemoveBtnCallback);
   uiB.Enable(FALSE);
   
   ox = 15;
   oy += 25;
   tbSwordFXAlphaMapNameTypeinID = tbSideMenuUI.CreateTypein("SwordFXAlphaMapName", ox, oy, tbSwordAlphaMapWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXAlphaMapNameTypeinID);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbSwordAlphaMapTextWidth, 5, tbSwordAlphaMapWidth-tbSwordAlphaMapTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Alpha Map", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbGameFXSwordAlphamapTypeinCallback);
   uiTy.PasteTypeinValue("");
   uiTy.Enable(FALSE);

   ox += tbSwordAlphaMapWidth + tbSwordAlphaMapBtnOffset;
   tbSwordFXAlphaMapAddButtonID = tbSideMenuUI.CreateButton("AddSwordFXAlphaMapButton", ox, oy+4, tbSwordTextureButtonWidth, 16, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbSwordFXAlphaMapAddButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbEditSwordFXTabID);
   uiB.BindCallback(TbGameFXSwordAddBtnCallback);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 25 + tbSwordUIBound -1;
   tbSwordFXBlendModeComboID = tbSideMenuUI.CreateCombo("SwordFXBlendTypeCombo", ox, oy, tbSwordBlendModeWidth, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                        tbSwordBlendModeTextWidth,
                                                        tbSwordBlendModeWidth - tbSwordBlendModeTextWidth -14, 3, FALSE);
   uiC.ID(tbSwordFXBlendModeComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbEditSwordFXTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Blending", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Alpha");
   uiC.InsertMessage(1, "Add Color");
   uiC.InsertMessage(2, "Subtract Color");
   uiC.PasteComboValue("Alpha");
   uiC.BindCallbacks(NULL, TbGameFXSwordChangeParamTypeinCallback);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 20 + tbSwordUIBound;
   tbSwordFXColorTypeinID[0] = tbSideMenuUI.CreateTypein("SwordColorRed", ox, oy, tbSwordColorRWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXColorTypeinID[0]);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, tbSwordColorTextWidth, 5, tbSwordColorRWidth-tbSwordColorTextWidth);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Color", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbGameFXSwordChangeParamTypeinCallback);
   uiTy.Enable(FALSE);

   ox += tbSwordColorRWidth + tbSwordUIBound -2;
   tbSwordFXColorTypeinID[1] = tbSideMenuUI.CreateTypein("SwordColorGreen", ox, oy, tbSwordColorGWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXColorTypeinID[1]);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 2, 5, tbSwordColorGWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbGameFXSwordChangeParamTypeinCallback);
   uiTy.Enable(FALSE);

   ox += tbSwordColorGWidth + tbSwordUIBound;
   tbSwordFXColorTypeinID[2] = tbSideMenuUI.CreateTypein("SwordColorBlue", ox, oy, tbSwordColorBWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXColorTypeinID[2]);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 2, 5, tbSwordColorBWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbGameFXSwordChangeParamTypeinCallback);
   uiTy.Enable(FALSE);

   ox += tbSwordColorBWidth + tbSwordUIBound;
   tbSwordFXColorTypeinID[3] = tbSideMenuUI.CreateTypein("SwordColorAlpha", ox, oy, tbSwordColorAWidth, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbSwordFXColorTypeinID[3]);
   uiTy.SetParent(tbEditSwordFXTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 2, 5, tbSwordColorAWidth);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("0.0");
   uiTy.BindCallbacks(TbGameFXSwordChangeParamTypeinCallback);
   uiTy.Enable(FALSE);

   // hide all FX editing panels
   uiT.ID(tbEditPlateFXTabID);
   uiT.Show(FALSE);
   uiT.ID(tbEditParticleFXTabID);
   uiT.Show(FALSE);
   uiT.ID(tbEditForceFXTabID);
   uiT.Show(FALSE);

   // connect all FX TAB UIs
   uiT.ID(tbGameFXFileTabID);
   uiT.NextTab(tbFXSystemCurrentTabID);

   // for working environment
   tbSideMenuEnvironmentPlateID = tbSideMenuUI.CreatePlate("EnvironmentSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbSideMenuEnvironmentPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);
}


/*--------------------
  create dialogue UIs
  C.Wang 1222, 2012
 ---------------------*/
void TbCreateDialogues()
{
   FnUiPlate uiP;
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   FnUiCombo uiC;
   FnUiTypein uiTy;

   // position the dialogue
   tbWarnWidth = 250;
   tbWarnHeight = 120;
   tbWarnOX = tbBottomVPOX + (tbBottomVPWidth - tbWarnWidth)/2;
   tbWarnOY = tbBottomVPOY + (tbBottomVPHeight - tbWarnHeight)/2;

   // create the menu Ui system
   tbWarnDialogueID = FyBeginUISystem(tbWarnOX, tbWarnOY, tbWarnWidth, tbWarnHeight);
   tbWarnUI.ID(tbWarnDialogueID);
   tbWarnUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbWarnDialogueID;

   // create the plates
   tbWarnMessageID = tbWarnUI.CreatePlate("Warning", 0, 0, tbWarnWidth - 1, tbWarnHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbWarnMessageID);
   uiP.SetCaption(LEFT_TOP, 6, 7, "Warning ....", 0.0f, 0.0f, 0.0f);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   int vi[10];
   vi[0] = 9; vi[1] = 30;
   vi[2] = tbWarnWidth - 13; vi[3] = 30;
   vi[4] = tbWarnWidth - 13; vi[5] = tbWarnHeight - 40;
   vi[6] = 9; vi[7] = tbWarnHeight - 40;
   vi[8] = 9; vi[9] = 30;
   uiP.Lines(vi, 5, 0.0f, 0.0f, 0.0f);
   uiP.CreateUIMessageSlots(3);

   // create buttons
   tbWarnOKID = tbWarnUI.CreateButton("OK", tbWarnWidth - BUTTON_WIDTH*2 - 20, tbWarnHeight - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbWarnOKID);
   uiB.SetParent(tbWarnMessageID);
   uiB.SetLayer(1);
   uiB.SetCaption(CENTERED, 0, 0, "OK", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);

   tbWarnCancelID = tbWarnUI.CreateButton("Cancel", tbWarnWidth - BUTTON_WIDTH - 10, tbWarnHeight - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbWarnCancelID);
   uiB.SetParent(tbWarnMessageID);
   uiB.SetLayer(1);
   uiB.SetCaption(CENTERED, 0, 0, "Cancel", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(FyStandardPopupQuit);

   // make it a popup UI
   tbWarnUI.BePopUp(TRUE);

   // create the add-action dialogue
   tbAddActionW = 400;
   tbAddActionH = 250;
   tbAddActionOX = tbBottomVPOX + (tbBottomVPWidth - tbAddActionW)/2;
   tbAddActionOY = tbBottomVPOY + (tbBottomVPHeight - tbAddActionH)/2;
   tbAddActionDialogueID = FyBeginUISystem(tbAddActionOX, tbAddActionOY, tbAddActionW, tbAddActionH);
   tbAddActionUI.ID(tbAddActionDialogueID);
   tbAddActionUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbAddActionDialogueID;

   // create the UIs for adding new action
   tbAddActionBackgroundID = tbAddActionUI.CreatePlate("AddNewAction", 0, 0, tbAddActionW - 1, tbAddActionH - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbAddActionBackgroundID);
   uiP.SetCaption(LEFT_TOP, 6, 7, "Add New Action", 0.0f, 0.0f, 0.0f);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   vi[0] = 9; vi[1] = 30;
   vi[2] = tbAddActionW - 13; vi[3] = 30;
   vi[4] = tbAddActionW - 13; vi[5] = tbAddActionH - 40;
   vi[6] = 9; vi[7] = tbAddActionH - 40;
   vi[8] = 9; vi[9] = 30;
   uiP.Lines(vi, 5, 0.0f, 0.0f, 0.0f);

   tbAddActionOKID = tbAddActionUI.CreateButton("AActionOK", tbAddActionW - BUTTON_WIDTH*2 - 20, tbAddActionH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddActionOKID);
   uiB.SetParent(tbAddActionBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "OK", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);

   tbAddActionCancelID = tbAddActionUI.CreateButton("AActionCancel", tbAddActionW - BUTTON_WIDTH - 10, tbAddActionH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddActionCancelID);
   uiB.SetParent(tbAddActionBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "Cancel", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(FyStandardPopupQuit);

   int ox, oy;

   ox = 15;
   oy = 40;
   tbAddActionNameID = tbAddActionUI.CreateTypein("AActionName", ox, oy, tbAddActionW - 30, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbAddActionNameID);
   uiTy.SetParent(tbAddActionBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbAddActionW - 100);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Name", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);

   ox = 15;
   oy += 26;
   tbAddActionTypeID = tbAddActionUI.CreateCombo("AActionType", ox, oy, tbAddActionW - 35, 20, 2, 0.45f, 0.45f, 0.45f, 1.0f,
                                                 50, tbAddActionW - ox - ox/3 - 90, 3, FALSE);
   uiC.ID(tbAddActionTypeID);
   uiC.SetLayer(3);
   uiC.SetParent(tbAddActionBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "  Type", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Simple Action");
   uiC.InsertMessage(1, "Cross Fade Blending");
   uiC.InsertMessage(2, "Connection");
   uiC.PasteComboValue("Simple Action");
   uiC.BindCallbacks(NULL, TbSelectActionType);

   ox = 15;
   oy += 24;
   tbAddActionFrontPoseID = tbAddActionUI.CreateCombo("AActionFrontPose", ox, oy, (tbAddActionW - 40)/2, 20, 2, 0.45f, 0.45f, 0.45f, 1.0f,
                                                      50, (tbAddActionW - 80)/2, 6);
   uiC.ID(tbAddActionFrontPoseID);
   uiC.SetLayer(3);
   uiC.SetParent(tbAddActionBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 0, 5, "Front P", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbSelectActionPose);

   ox += (tbAddActionW - 30)/2;
   tbAddActionEndPoseID = tbAddActionUI.CreateCombo("AActionRearPose", ox, oy, (tbAddActionW - 40)/2, 20, 2, 0.45f, 0.45f, 0.45f, 1.0f,
                                                    50, (tbAddActionW - 80)/2, 6);
   uiC.ID(tbAddActionEndPoseID);
   uiC.SetLayer(3);
   uiC.SetParent(tbAddActionBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 5, 5, "Rear P", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbSelectActionPose);

   ox = 15;
   oy += 23;
   tbAddActionDurationID = tbAddActionUI.CreateTypein("AActionDuration", ox, oy, tbAddActionW/2, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbAddActionDurationID);
   uiTy.SetParent(tbAddActionBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbAddActionW/2 - 55);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Length", 0.0f, 0.0f, 0.0f);
   uiTy.SetLayer(2);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);

   // make it a popup UI
   tbAddActionUI.BePopUp(TRUE);

   // create the add-facial-expression dialogue
   tbAddFacialExpressionW = 400;
   tbAddFacialExpressionH = 250;
   tbAddFacialExpressionOX = tbBottomVPOX + (tbBottomVPWidth - tbAddFacialExpressionW)/2;
   tbAddFacialExpressionOY = tbBottomVPOY + (tbBottomVPHeight - tbAddFacialExpressionH)/2;
   tbAddFacialExpressionDialogueID = FyBeginUISystem(tbAddFacialExpressionOX, tbAddFacialExpressionOY, tbAddFacialExpressionW, tbAddFacialExpressionH);
   tbAddFacialExpressionUI.ID(tbAddFacialExpressionDialogueID);
   tbAddFacialExpressionUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbAddFacialExpressionDialogueID;

   // create UIs for adding a new facial expression
   tbAddFacialExpressionBackgroundID = tbAddFacialExpressionUI.CreatePlate("AddFacialExpression", 0, 0, tbAddFacialExpressionW - 1, 
                                                                           tbAddFacialExpressionH - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbAddFacialExpressionBackgroundID);
   uiP.SetCaption(LEFT_TOP, 6, 7, "Add New Facial Expression", 0.0f, 0.0f, 0.0f);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   vi[0] = 9; vi[1] = 30;
   vi[2] = tbAddFacialExpressionW - 13; vi[3] = 30;
   vi[4] = tbAddFacialExpressionW - 13; vi[5] = tbAddFacialExpressionH - 40;
   vi[6] = 9; vi[7] = tbAddFacialExpressionH - 40;
   vi[8] = 9; vi[9] = 30;
   uiP.Lines(vi, 5, 0.0f, 0.0f, 0.0f);

   tbAddFacialExpressionOKID = tbAddFacialExpressionUI.CreateButton("AddFacialExpressionOK", tbAddFacialExpressionW - BUTTON_WIDTH*2 - 20, 
                                                                     tbAddFacialExpressionH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddFacialExpressionOKID);
   uiB.SetParent(tbAddFacialExpressionBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "OK", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(TbNewFacialExpressionUICommand);

   tbAddFacialExpressionCancelID = tbAddFacialExpressionUI.CreateButton("AddFacialExpressionCancel", tbAddFacialExpressionW - BUTTON_WIDTH - 10,
                                                                         tbAddFacialExpressionH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddFacialExpressionCancelID);
   uiB.SetParent(tbAddFacialExpressionBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "Cancel", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(FyStandardPopupQuit);

   ox = 15;
   oy = 40;
   tbAddFacialExpressionNameID = tbAddFacialExpressionUI.CreateTypein("AddFacialExpressionName", ox, oy, tbAddFacialExpressionW - 30, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbAddFacialExpressionNameID);
   uiTy.SetParent(tbAddFacialExpressionBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbAddFacialExpressionW - 100);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Name", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);

   oy += 30;
   tbAddFacialExpressionLengthID = tbAddFacialExpressionUI.CreateTypein("AddFacialExpressionLength", ox, oy, tbAddFacialExpressionW - 30, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbAddFacialExpressionLengthID);
   uiTy.SetParent(tbAddFacialExpressionBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbAddFacialExpressionW - 100);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Length", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(NULL, TbCheckIntegerInput);

   // make it a popup UI
   tbAddFacialExpressionUI.BePopUp(TRUE);

   // create the add-gameFX dialogue
   tbAddGameFXW = 400;
   tbAddGameFXH = 250;
   tbAddGameFXOX = tbBottomVPOX + (tbBottomVPWidth - tbAddGameFXW)/2;
   tbAddGameFXOY = tbBottomVPOY + (tbBottomVPHeight - tbAddGameFXH)/2;
   tbAddGameFXDialogueID = FyBeginUISystem(tbAddGameFXOX, tbAddGameFXOY, tbAddGameFXW, tbAddGameFXH);
   tbAddGameFXUI.ID(tbAddGameFXDialogueID);
   tbAddGameFXUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbAddGameFXDialogueID;

   // create UIs for adding a new game FX
   tbAddFXBackgroundID = tbAddGameFXUI.CreatePlate("AddNewGameFX", 0, 0, tbAddGameFXW - 1, tbAddGameFXH - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbAddFXBackgroundID);
   uiP.SetCaption(LEFT_TOP, 6, 7, "Add New Game FX", 0.0f, 0.0f, 0.0f);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   vi[0] = 9; vi[1] = 30;
   vi[2] = tbAddGameFXW - 13; vi[3] = 30;
   vi[4] = tbAddGameFXW - 13; vi[5] = tbAddGameFXH - 40;
   vi[6] = 9; vi[7] = tbAddGameFXH - 40;
   vi[8] = 9; vi[9] = 30;
   uiP.Lines(vi, 5, 0.0f, 0.0f, 0.0f);

   tbAddFXOKID = tbAddGameFXUI.CreateButton("AddGameFXOK", tbAddGameFXW - BUTTON_WIDTH*2 - 20, tbAddGameFXH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddFXOKID);
   uiB.SetParent(tbAddFXBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "OK", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(TbNewFXUICommand);

   tbAddFXCancelID = tbAddGameFXUI.CreateButton("AddGameFXCancel", tbAddGameFXW - BUTTON_WIDTH - 10, tbAddGameFXH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddFXCancelID);
   uiB.SetParent(tbAddFXBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "Cancel", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(FyStandardPopupQuit);

   ox = 15;
   oy = 40;
   tbAddFXNameID = tbAddGameFXUI.CreateTypein("AddFXName", ox, oy, tbAddGameFXW - 30, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbAddFXNameID);
   uiTy.SetParent(tbAddFXBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbAddGameFXW - 100);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Name", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(NULL, TbCheckTypeinGameFXName);

   ox = 15;
   oy += 26;
   tbAddFXTypeID = tbAddGameFXUI.CreateCombo("AddFXType", ox, oy, tbAddGameFXW/2, 20, 256, 0.45f, 0.45f, 0.45f, 1.0f,
                                             50, tbAddGameFXW/2 - ox, 6, FALSE);
   uiC.ID(tbAddFXTypeID);
   uiC.SetParent(tbAddFXBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "  Type", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Plate");
   uiC.InsertMessage(1, "Geometry");
   uiC.InsertMessage(2, "Particles");
   uiC.InsertMessage(3, "Force");
   uiC.InsertMessage(4, "Audio");
   uiC.InsertMessage(5, "Sword");
   uiC.PasteComboValue("Plate");
   uiC.BindCallbacks(NULL, TbSelectFXType);

   ox += tbAddGameFXW/2 + 5;
   tbFXLifeID = tbAddGameFXUI.CreateTypein("SetFXLife", ox, oy, tbAddGameFXW/2 - 40, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbFXLifeID);
   uiTy.SetParent(tbAddFXBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 30, 5, tbAddGameFXW/2 - 80);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Life", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   char value[256];
   sprintf(value, "%d", tbDefFXLife);
   uiTy.PasteTypeinValue(value);
   uiTy.BindCallbacks(TbCheckLifeDataInput, NULL);

   ox = 15;
   oy += 26;
   tbMaxParticleNumID = tbAddGameFXUI.CreateTypein("SetMaxParticles", ox, oy, 210, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbMaxParticleNumID);
   uiTy.SetParent(tbAddFXBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 130, 5, 200 - 130);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Maximum Particles", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   sprintf(value, "%d", tbCurMaxParticles);
   uiTy.PasteTypeinValue(value);
   uiTy.BindCallbacks(TbCheckMaxParticlesInput, NULL);

   tbPickForceTypeID = tbAddGameFXUI.CreateCombo("PickupForceType", ox, oy, tbAddGameFXW/2, 20, 256, 0.45f, 0.45f, 0.45f, 1.0f,
                                                 50, tbAddGameFXW/2 - ox, 6, TRUE);
   uiC.ID(tbPickForceTypeID);
   uiC.SetParent(tbAddFXBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "  Force", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Gravity");
   uiC.InsertMessage(1, "Parallel");
   uiC.InsertMessage(2, "Sink");
   uiC.InsertMessage(3, "Sphere");
   uiC.InsertMessage(4, "Vortex");
   uiC.InsertMessage(5, "Vortex 2D");
   uiC.InsertMessage(6, "Viscosity");
   uiC.InsertMessage(7, "User Defined");
   uiC.PasteComboValue("Gravity");
   //uiC.BindCallbacks(NULL, TbPickupForceType);

   ox = 15 + tbAddGameFXW/2;
   tbPickForceMagnitudeID = tbAddGameFXUI.CreateTypein("SetForceMagnitude", ox, oy, tbAddGameFXW/2 - 40, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbPickForceMagnitudeID);
   uiTy.SetParent(tbAddFXBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 65, 5, tbAddGameFXW/2 - 110);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Magnitude", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   sprintf(value, "%10.3f", tbCurMaxForcemagnitude);
   uiTy.PasteTypeinValue(value);
   //uiTy.BindCallbacks(TbCheckMaxParticlesInput, NULL);

   // make it a popup UI
   tbAddGameFXUI.BePopUp(TRUE);

   // create the add-gameFX-System dialogue
   tbAddGameFXSystemW = 400;
   tbAddGameFXSystemH = 250;
   tbAddGameFXSystemOX = tbBottomVPOX + (tbBottomVPWidth - tbAddGameFXSystemW)/2;
   tbAddGameFXSystemOY = tbBottomVPOY + (tbBottomVPHeight - tbAddGameFXSystemH)/2;
   tbAddGameFXSystemDialogueID = FyBeginUISystem(tbAddGameFXSystemOX, tbAddGameFXSystemOY, tbAddGameFXSystemW, tbAddGameFXSystemH);
   tbAddGameFXSystemUI.ID(tbAddGameFXSystemDialogueID);
   tbAddGameFXSystemUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbAddGameFXSystemDialogueID;

   // create UIs for adding a new game FX system
   tbAddFXSystemBackgroundID = tbAddGameFXSystemUI.CreatePlate("AddNewGameFXSystem", 0, 0, tbAddGameFXSystemW - 1, tbAddGameFXSystemH - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbAddFXSystemBackgroundID);
   uiP.SetCaption(LEFT_TOP, 6, 7, "Add New Game FX System", 0.0f, 0.0f, 0.0f);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   vi[0] = 9; vi[1] = 30;
   vi[2] = tbAddGameFXSystemW - 13; vi[3] = 30;
   vi[4] = tbAddGameFXSystemW - 13; vi[5] = tbAddGameFXSystemH - 40;
   vi[6] = 9; vi[7] = tbAddGameFXSystemH - 40;
   vi[8] = 9; vi[9] = 30;
   uiP.Lines(vi, 5, 0.0f, 0.0f, 0.0f);

   tbAddFXSystemOKID = tbAddGameFXSystemUI.CreateButton("AddGameFXSystemOK", tbAddGameFXSystemW - BUTTON_WIDTH*2 - 20, tbAddGameFXSystemH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddFXSystemOKID);
   uiB.SetParent(tbAddFXSystemBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "OK", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(TbNewFXSystemUICommand);

   tbAddFXSystemCancelID = tbAddGameFXSystemUI.CreateButton("AddGameFXSystemCancel", tbAddGameFXSystemW - BUTTON_WIDTH - 10, tbAddGameFXSystemH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbAddFXSystemCancelID);
   uiB.SetParent(tbAddFXSystemBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "Cancel", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(FyStandardPopupQuit);

   ox = 15;
   oy = 40;
   tbAddFXSystemNameID = tbAddGameFXSystemUI.CreateTypein("AddFXSystemName", ox, oy, tbAddGameFXSystemW - 30, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbAddFXSystemNameID);
   uiTy.SetParent(tbAddFXSystemBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbAddGameFXSystemW - 100);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Name", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);

   // make it a popup UI
   tbAddGameFXSystemUI.BePopUp(TRUE);

   // create the keyframe editor dialogue
   tbGameFXKeyframeW = 400;
   tbGameFXKeyframeH = 250;
   tbGameFXKeyframeOX = tbBottomVPOX + tbBottomVPWidth - tbGameFXKeyframeW;
   tbGameFXKeyframeOY = tbBottomVPOY;
   tbGameFXKeyframeDialogueID = FyBeginUISystem(tbGameFXKeyframeOX, tbGameFXKeyframeOY, tbGameFXKeyframeW, tbGameFXKeyframeH);
   tbGameFXKeyframeUI.ID(tbGameFXKeyframeDialogueID);
   tbGameFXKeyframeUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbGameFXKeyframeDialogueID;

   // create UIs for editing keyframe game FX system
   tbGameFXKeyframeBackgroundID = tbGameFXKeyframeUI.CreatePlate("EditGameFXKeyframe", 0, 0, tbGameFXKeyframeW - 1, tbGameFXKeyframeH - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.ID(tbGameFXKeyframeBackgroundID);
   uiP.SetCaption(LEFT_TOP, 6, 7, "Edit Game FX Keyframes", 0.0f, 0.0f, 0.0f);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   vi[0] = 9; vi[1] = 30;
   vi[2] = tbGameFXKeyframeW - 13; vi[3] = 30;
   vi[4] = tbGameFXKeyframeW - 13; vi[5] = tbGameFXKeyframeH - 40;
   vi[6] = 9; vi[7] = tbGameFXKeyframeH - 40;
   vi[8] = 9; vi[9] = 30;
   uiP.Lines(vi, 5, 0.0f, 0.0f, 0.0f);

   tbGameFXKeyframeModifyID = tbGameFXKeyframeUI.CreateButton("GameFXKeyframeOK", tbGameFXKeyframeW - 3*BUTTON_WIDTH - 30, tbGameFXKeyframeH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbGameFXKeyframeModifyID);
   uiB.SetParent(tbGameFXKeyframeBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "Modify", 0.0f, 0.0f, 1.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(TbModifyFXKeyCommand);

   tbGameFXKeyframeRemoveID = tbGameFXKeyframeUI.CreateButton("GameFXKeyframeRemove", tbGameFXKeyframeW - 2*BUTTON_WIDTH - 20, tbGameFXKeyframeH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbGameFXKeyframeRemoveID);
   uiB.SetParent(tbGameFXKeyframeBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "Remove", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(TbRemoveFXKeyCommand);

   tbGameFXKeyframeCancelID = tbGameFXKeyframeUI.CreateButton("GameFXKeyframeCancel", tbGameFXKeyframeW - BUTTON_WIDTH - 10, tbGameFXKeyframeH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.ID(tbGameFXKeyframeCancelID);
   uiB.SetParent(tbGameFXKeyframeBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "Close", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(TbGameFXKeyFrameCancelCallback);

   ox = 15;
   oy = 40;
   tbGameFXKeyFrameID = tbGameFXKeyframeUI.CreateCombo("KeyframeID", ox, oy, 150, 25, 5, 0.45f, 0.45f, 0.45f, 1.0f, 50, 100, 6, TRUE);
   uiC.ID(tbGameFXKeyFrameID);
   uiC.SetParent(tbGameFXKeyframeBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "  Frame", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbCheckKeys, TbSelectKeys);

   oy += 30;
   tbGameFXKeyDataID[0] = tbGameFXKeyframeUI.CreateTypein("KeyframeData0ID", ox, oy, 142, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbGameFXKeyDataID[0]);
   uiTy.SetParent(tbGameFXKeyframeBackgroundID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, 70);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Position", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);

   ox += 143;
   tbGameFXKeyDataID[1] = tbGameFXKeyframeUI.CreateTypein("KeyframeData1ID", ox, oy, 73, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbGameFXKeyDataID[1]);
   uiTy.SetParent(tbGameFXKeyframeBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 2, 5, 70);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.SetLayer(2);

   ox += 74;
   tbGameFXKeyDataID[2] = tbGameFXKeyframeUI.CreateTypein("KeyframeData2ID", ox, oy, 73, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbGameFXKeyDataID[2]);
   uiTy.SetParent(tbGameFXKeyframeBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 2, 5, 70);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.SetLayer(2);

   ox += 74;
   tbGameFXKeyDataID[3] = tbGameFXKeyframeUI.CreateTypein("KeyframeData3ID", ox, oy, 73, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.ID(tbGameFXKeyDataID[3]);
   uiTy.SetParent(tbGameFXKeyframeBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 2, 5, 70);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.SetLayer(2);

   // make it a popup UI
   tbGameFXKeyframeUI.BePopUp(TRUE);
}


/*------------------------
  quit function for win32
  C.Wang 0814, 2012
 -------------------------*/
void TbQuitFun()
{
   TbOpenQuit(tbExitButtonID, FALSE);
}


/*----------------------------------------------
  the timer callback for testbed used as a tool
  C.Wang 1206, 2012
 -----------------------------------------------*/
void TbToolMode(int skip)
{
   // check current character
   FnCharacter actor;
   actor.ID(tbCurScene->GetCurrentCharacter());

   // play the all animation
   if (tbBePlay) {
      // play all animation of the scene
      tbCurFrame = tbCurScene->Play(skip*tbPlaySpeed, tbBeBase, tbPlayMode);
   }

   if (tbBePause) {
      tbCurFrame = tbCurScene->PauseFXPlaying();
   }

   // play test pose
   if (tbBePlayPose) {
      if (tbTestPoseStart >= 0 && tbTestPoseEnd >= 0) {
         // play the frame
         actor.PlayFrame(tbTestPoseFrame, tbBeBase, TRUE, skip*tbPlaySpeed, tbTestMotionName);
         tbTestPoseFrame += skip*tbPlaySpeed;
         if (tbTestPoseFrame > (float) tbTestPoseEnd) {
            tbTestPoseFrame = (float) tbTestPoseStart;

            // reset the character's position
            float pos[3];
            pos[0] = pos[1] = pos[2] = 0.0f;
            actor.SetPosition(pos);
         }
      }
   }
   else {
      if (FYABS(tbTestPoseFrame - (float)tbTestPoseStart) > 0.01f) {
         tbTestPoseFrame = (float) tbTestPoseStart;
         actor.PlayFrame(tbTestPoseFrame, tbBeBase, TRUE, skip*tbPlaySpeed);
      }
   }

   if (tbBePlay) {
      char msg[256];
      FnUiTypein uiTy(tbCurFrameID);
      sprintf(msg, "%8.1f", tbCurFrame);
      uiTy.PasteTypeinValue(msg);
   }

   // control the log message's opacity
   if (tbLogOpacity >= 0) {
      tbLogOpacity -= 1;
   }

   // play the facial expression
   if (tbBePlayFacialExpression){
      actor.PlayFacialExpression(LOOP, (float)skip);
   }
}


/*------------------------------------------------------
  the timer callback for testbed used as a game control
  C.Wang 0814, 2012
 -------------------------------------------------------*/
void TbPlayMode(int skip)
{
}


/*------------------------------------------------
  this is a timer callback for main loop in 30fps
  C.Wang 0814, 2012
 -------------------------------------------------*/
void TbMainLoop(int skip)
{
   // do the batch jobs
   FjDoJobs(skip);

   // execute the script
   TbExecuteScript(skip);

   // check the control mode
   if (tbControlMode == TOOL_MODE) {
      TbToolMode(skip);
   }
   else if (tbControlMode == PLAY_MODE) {
      TbPlayMode(skip);
   }
}


/*--------------------------------------
  display the command & command history
  C.Wang 0814, 2012
 ---------------------------------------*/
void TbDisplayCommands()
{
   int i, ox, oy, oxCmd;
   char msg[MAX_STRING_CHARACTERS];

   // begin to display console messages
   if (!tbBeSmallShuFa) {
      tbSmallEFont.Begin(tbWorkSpaceBTID);
   }

   // display console prompt & messages
   ox = tbBottomVPOX + 2;
   oy = tbBottomVPOY + tbBottomVPHeight - 20;

   // the prompt
   ox = tbSmallEFont.Write(" >", ox, oy, 0, 255, 0);
   oxCmd = ox;

   // the entering command
   strcpy(msg, tbCurCommandPtr);

   if (tbBeBlink) {
      // cursor
      msg[tbCommandCursor] = '_';
      msg[tbCommandCursor+1] = '\0';
   } 
   ox = tbSmallEFont.Write(msg, ox, oy, 0, 255, 0);

   // show log
   oy -= 15;
   if (tbLogOpacity >= 0.0f) {
      ox = 5;
      int o = (int) tbLogOpacity;
      if (o > 255) o = 255;
      ox = tbSmallEFont.Write(tbLogMessage, ox, oy, 0, 255, 200, o);
      tbLogOpacity -= 0.5f;
   }

   // the command history
   if (tbBeDisplayCommandHistory) {
      oy -= 5;
      for (i = tbCurCommand - 1; i >= 0; i--) {
         ox = oxCmd;
         oy -= 15;
         ox = tbSmallEFont.Write(tbCommandHistory[i], ox, oy, 255, 255, 0, 200);
      }
   }

   // end to display
   if (!tbBeSmallShuFa) {
      tbSmallEFont.End();
   }
}


/*---------------------------------------
  this is a timer callback for rendering
  C.Wang 0821, 2012
 ----------------------------------------*/
void TbRenderIt(int skip)
{
   char string[256];
   float elapsTime;

   // render the viewports
   tbWSBottomVP.Render3D(0, TRUE, TRUE);

   if (tbRenderCamera == ORTHOGONAL) {
      // render the work space
      FyTimerReset(1);
      tbWorkSpaceVP.Render3D(tbOrthoCamID, TRUE, TRUE);
      elapsTime = FyTimerCheckTime(1);
   }
   else {
      float M16[16], M12[12];

      if (tbShowAux && tbRenderCamID == tbMainCamID) {
         // synchronize the aux camera with the main one
         tbCamera.GetMatrix(M16, TRUE);
         M12[0] = M16[0];  M12[1] = M16[1];   M12[2] = M16[2];
         M12[3] = M16[4];  M12[4] = M16[5];   M12[5] = M16[6];
         M12[6] = M16[8];  M12[7] = M16[9];   M12[8] = M16[10];
         M12[9] = M16[12]; M12[10] = M16[13]; M12[11] = M16[14];
         tbAuxCamera.SetMatrix(M12, REPLACE);
         tbAux2Camera.SetMatrix(M12, REPLACE);

         // render the auxiliary scene
         tbWorkSpaceVP.Render3D(tbAuxCamID, TRUE, TRUE);

         // render the work space
         FyTimerReset(1);
         if (tbBeSSAO) {
            tbWorkSpaceVP.RenderSSAOEffect(tbRenderCamID, TRUE, TRUE);
         }
         else if (tbBeHDR) {
            if (tbHDRType == BLOOM_EFFECT) {
               tbWorkSpaceVP.RenderBloomEffect(tbRenderCamID, TRUE, TRUE, tbHDRLoop);
            }
            else {
               tbWorkSpaceVP.RenderHDREffect(tbRenderCamID, TRUE, TRUE, tbHDRLoop);
            }
         }
         else {
            tbWorkSpaceVP.Render3D(tbRenderCamID, FALSE, FALSE);
         }
         elapsTime = FyTimerCheckTime(1);

         // render the auxiliary scene 2
         tbWorkSpaceVP.Render3D(tbAux2CamID, FALSE, FALSE);
      }
      else {
         // render the work space
         FyTimerReset(1);
         if (tbBeSSAO) {
            tbWorkSpaceVP.RenderSSAOEffect(tbRenderCamID, TRUE, TRUE);
         }
         else if (tbBeHDR) {
            if (tbHDRType == BLOOM_EFFECT) {
               tbWorkSpaceVP.RenderBloomEffect(tbRenderCamID, TRUE, TRUE, tbHDRLoop);
            }
            else {
               tbWorkSpaceVP.RenderHDREffect(tbRenderCamID, TRUE, TRUE, tbHDRLoop);
            }
         }
         else {
            tbWorkSpaceVP.Render3D(tbRenderCamID, TRUE, TRUE);
         }
         elapsTime = FyTimerCheckTime(1);
      }

      if (tbShowAux && tbRenderCamID == tbMainCamID) {
         // synchronize the aux camera with the main one
         FnCamera camera(tbMiniCamID);
         camera.Translate(0.0f, 0.0f, tbMiniCamDist, REPLACE);
         M12[9] = M12[10] = M12[11] = 0.0f;
         camera.SetMatrix(M12, GLOBAL);

         // render the minimport
         tbMiniPost.Render3D(tbMiniCamID, FALSE, TRUE);
      }
   }

   // show workspace caption
   if (tbShowWorkSpaceCaption) {
      if (!tbBeSmallShuFa) {
         tbSmallEFont.Begin(tbWorkSpaceBTID);
         tbBeSmallShuFa = TRUE;
      }

      FnCharacter actor;
      CHARACTERid curA = tbCurScene->GetCurrentCharacter();
      int iOne = 0;
      int ox = tbBottomVPOX;
      int oy = tbBottomVPOY;
      if (tbRenderCamera == ORTHOGONAL) {
         strcpy(string, "Texture View ");
      }
      else {
         strcpy(string, "3D View - ");
         if (tbBeZUp) {
            strcat(string, " Zup ");
         }
         else {
            strcat(string, " Yup ");
         }
      }

      // check total rendering group
      int numRG = tbCurScene->GetRenderGroupNumber();
      char rgString[64];
      sprintf(rgString, "(RG = %d)", numRG);
      strcat(string, rgString);

      if (tbBePlay) {
         strcat(string, " - Playing");
         ox = tbSmallEFont.Write(string, ox, oy, 0, 255, 0);

         if (curA != FAILED_ID) {
            actor.ID(curA);
            ACTIONid actionID = actor.GetCurrentAction(0, NULL);
            FnAction action;
            if (actionID != FAILED_ID) {
               action.ID(actionID);
               sprintf(string, " %s", action.GetName());
               ox = tbSmallEFont.Write(string, ox, oy, 128, 255, 0);
            }
         }
      }
      else {
         ox = tbSmallEFont.Write(string, ox, oy, 0, 255, 0);
      }

      // calculate the FPS data
      if (tbRenderedFrame == 0) FyTimerReset(0);
      if (tbRenderedFrame/10*10 == tbRenderedFrame) tbFPS = tbRenderedFrame/FyTimerCheckTime(0);
      tbRenderedFrame += skip;
      if (tbRenderedFrame >= 1000) tbRenderedFrame = 0;

      // display FPS on screen
      sprintf(string, " FPS %6.2f", tbFPS);
      ox = tbSmallEFont.Write(string, ox, oy, 255, 128, 0, 255);

      if (tbShowFPSFrame) {
         sprintf(string, " (%6.2f)", 1.0f/elapsTime);
         ox = tbSmallEFont.Write(string, ox, oy, 255, 128, 0, 255);
      }

      if (tbShowDeb) {
         //tbSmallEFont.Write(tbDebugInfo, ox, oy, 255, 0, 0);
      }
      iOne++;

      // show current character's name
      if (tbTool == FACE_TOOL) {
         // check if the face model is alive!
         if (FyCheckObjectValidity(tbCurrentFaceModelID)) {
            FnObject faceModel(tbCurrentFaceModelID);
            sprintf(string, "Face Model : %s", faceModel.GetName());
         }
         else {
            strcpy(string, "No Face Model");
         }
         ox = tbBottomVPOX;
         oy = tbBottomVPOY + 15*iOne;
         ox = tbSmallEFont.Write(string, ox, oy, 128, 255, 0);
         iOne++;

         // check if the shift/ctrl key down
         if (FyCheckHotKeyStatus(FY_CTRL)) {
            sprintf(string, "Mask Operation Mode: + (%d, %d) --> (%d, %d)", tbFTStartX, tbFTStartY, tbFTEndX, tbFTEndY);
            tbMaskOpMode = FT_MASK_ADD;
         }
         else if (FyCheckHotKeyStatus(FY_SHIFT)) {
            sprintf(string, "Mask Operation Mode: - (%d, %d) --> (%d, %d)", tbFTStartX, tbFTStartY, tbFTEndX, tbFTEndY);
            tbMaskOpMode = FT_MASK_SUB;
         }
         else {
            sprintf(string, "Mask Operation Mode: none (%d, %d) --> (%d, %d)", tbFTStartX, tbFTStartY, tbFTEndX, tbFTEndY);
            tbMaskOpMode = FT_MASK_NONE;
         }
         ox = tbBottomVPOX;
         oy = tbBottomVPOY + 15*iOne;
         ox = tbSmallEFont.Write(string, ox, oy, 128, 255, 0);
         iOne++;
      }
      else {
         if (curA != FAILED_ID) {
            actor.ID(curA);
            sprintf(string, "Character : %s", actor.GetName());
            ox = tbBottomVPOX;
            oy = tbBottomVPOY + 15*iOne;
            ox = tbSmallEFont.Write(string, ox, oy, 128, 255, 0);
            iOne++;
         }

         // show current object's name
         OBJECTid curObj = tbCurScene->GetCurrentObject();
         if (curObj != FAILED_ID) {
            FnObject model;
            char string2[256];

            model.ID(curObj);
            sprintf(string, "Model : %s", model.GetName());
            sprintf(string2, "    Triangles: %d  Vertices: %d", tbTris, tbVs);
            ox = tbBottomVPOX;
            oy += 15;
            ox = tbSmallEFont.Write(string, ox, oy, 0, 255, 0);
            ox = tbSmallEFont.Write(string2, ox, oy, 255, 255, 0);
            iOne++;
         }
      }

      if (tbShowDeb) {
         /* show debug info
         ox = tbBottomVPOX;
         oy = tbBottomVPOY + 15*iOne + 20;
         tbSmallEFont.Write(tbDebugInfo, ox, oy, 255, 0, 0);

         int id = atoi(tbDebugInfo);
         OBJECTid objID = tbCurScene->GetCurrentObject();
         FnTriangle triObj;
         triObj.ID(objID, 0);

         int kkk, inc[3];
         float xyz[16], p2D[3];
         triObj.GetTopology(id, inc);

         for (kkk = 0; kkk < 3; kkk++) {
            triObj.GetVertex(inc[kkk], xyz);
            tbWorkSpaceVP.ComputeScreenPosition(tbRenderCamID, p2D, xyz, PHYSICAL_SCREEN, FALSE);
            if (p2D[0] > 0 && p2D[0] < tbWorkSpaceVPWidth && p2D[1] > 0 && p2D[1] < tbWorkSpaceVPHeight) {
               char sss[256];
               sprintf(sss, "%d", inc[kkk]);
               ox = tbSmallEFont.Write(sss, tbWorkSpaceVPOX + (int)p2D[0], tbWorkSpaceVPOY + (int)p2D[1], 0, 255, 0);
            }
         }*/

         //ox = tbBottomVPOX;
         //oy = tbBottomVPOY + 15*iOne + 20;
         //sprintf(tbDebugInfo, "Pose Frame : %d", (int)tbTestPoseFrame);
         //tbSmallEFont.Write(tbDebugInfo, ox, oy, 255, 0, 0);
      }

      if (tbBeSmallShuFa) {
         tbSmallEFont.End();
         tbBeSmallShuFa = FALSE;
      }
   }

   // render the UI
   FyRenderAllUISystem(tbAllUISysID, tbNumUISys);

   // show console
   if (FyGetInputFunction() == TbCommandInput) {
      TbDisplayCommands();
   }

   if (tbBeSmallShuFa) {
      tbSmallEFont.End();
      tbBeSmallShuFa = FALSE;
   }

   // swap buffers
   FySwapBuffers();
}


/*-------------------------------------------
  render everything when the screen is dirty
  C.Wang 0821, 2012
 --------------------------------------------*/
void TbRenderItOnce()
{
   TbRenderIt(1);
}


/*--------------------------------
  blinking control timer callback
  fps = 1
  C.Wang 0814, 2012
 ---------------------------------*/
void TbBlinkControl(int skip)
{
   tbBeBlink = !tbBeBlink;
}


/*-------------------------
  batch rendering callback
  C.Wang 0814, 2012
 --------------------------*/
void TbRenderAll()
{
   TbRenderIt(1);
}


/*---------------------------
  create the axis/test plane
  C.Wang 0821, 2012
 ----------------------------*/
void TbCreateAuxObjects()
{
   FnObject object;
   MATERIALid mIDR, mIDG, mIDB, mID;
   float v[16];
   float color[3];

   // create the axis object
   tbAxisID = tbAuxScene.CreateObject();
   object.ID(tbAxisID);
   object.ChangeRenderGroup(1);
   object.Show(FALSE);

   v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f;
   v[3] = 10.0f; v[4] = 0.0f; v[5] = 0.0f;
   color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f;
   mIDR = FyCreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mIDR, v, 2, TRUE);

   v[3] = 0.0f; v[4] = 10.0f; v[5] = 0.0f;
   color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f;
   mIDG = FyCreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mIDG, v, 2, TRUE);

   v[3] = 0.0f; v[4] = 0.0f; v[5] = 10.0f;
   color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f;
   mIDB = FyCreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mIDB, v, 2, TRUE);

   // create the test plane
   tbTestPlaneID = tbAuxScene.CreateObject();
   object.ID(tbTestPlaneID);
   object.SetOpacity(0.4f);
   object.ChangeRenderGroup(0);

   color[0] = 0.7f; color[1] = 0.7f; color[2] = 0.7f;
   mID = FyCreateMaterial(color, color, color, 20.0f, color);

   color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f;
   mIDB = FyCreateMaterial(color, color, color, 20.0f, color);

   int i, j, div = 10;
   float x, dx;
   dx = tbTestPlaneSize/div;
   x = -tbTestPlaneSize;
   for (i = -div; i <= div; i++) {
      v[0] = -tbTestPlaneSize; v[1] = x; v[2] = 0.0f;
      v[3] = tbTestPlaneSize; v[4] = x; v[5] = 0.0f;
      if (i == 0) {
         object.Lines(LINE_SEGMENTS, mIDB, v, 2, TRUE);
      }
      else {
         object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);
      }
      x += dx;
   }

   x = -tbTestPlaneSize;
   for (i = -div; i <= div; i++) {
      v[1] = -tbTestPlaneSize; v[0] = x; v[2] = 0.0f;
      v[4] = tbTestPlaneSize; v[3] = x; v[5] = 0.0f;
      if (i == 0) {
         object.Lines(LINE_SEGMENTS, mIDB, v, 2, TRUE);
      }
      else {
         object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);
      }
      x += dx;
   }
   object.SetRenderOption(Z_BUFFER_WRITE, FALSE);

   if (!tbBeZUp) {
      object.Rotate(X_AXIS, 90.0f, GLOBAL);
   }

   // create two gizmo tools
   FnUiRadioButton uiRB;
   OBJECTid   objID;
   float colorR[3] = {0.9f, 0.1f, 0.1f};  // Red
   float colorG[3] = {0.1f, 0.7f, 0.1f};  // Green
   float colorB[3] = {0.1f, 0.1f, 0.9f};  // Blue
   float colorY[3] = {1.0f, 1.0f, 0.0f};  // Yellow

   //--- Translate Gizmo
   uiRB.ID(tbModelTFTranslateButtonID);
   tbGizmoTranslate.isShow      = uiRB.GetState();
   tbGizmoTranslate.numGizmoObj = 6;
   tbGizmoTranslate.iCurGizmo   = -1;
   memcpy(tbGizmoTranslate.colorX,      colorR, sizeof(colorR));
   memcpy(tbGizmoTranslate.colorY,      colorG, sizeof(colorG));
   memcpy(tbGizmoTranslate.colorZ,      colorB, sizeof(colorB));
   memcpy(tbGizmoTranslate.colorChoose, colorY, sizeof(colorY));
   tbGizmoTranslate.radiusBig   = tbGizmoRadiusTBig;
   tbGizmoTranslate.radiusSmall = tbGizmoRadiusTSmall;

   objID = tbAux2Scene.CreateObject();                // Translate Gizmo Base Object
   tbGizmoTranslate.baseID = objID;
   object.ID(objID);
   object.Show(FALSE);
   object.SetRenderOption(Z_BUFFER, FALSE);
   
   objID = tbAux2Scene.CreateObject();                // Translate Gizmo X-axis Object
   object.ID(objID);
   tbGizmoTranslate.gizmoIDs[X_AXIS]    = objID;
   tbGizmoTranslate.gizmoMIDs[X_AXIS*2] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorX);
   v[0] = tbGizmoRadiusTSmall/2;
   v[1] = 0.0f;
   v[2] = 0.0f;
   v[3] = tbGizmoRadiusTBig;
   v[4] = 0.0f;
   v[5] = 0.0f;
   object.Lines(LINE_SEGMENTS, tbGizmoTranslate.gizmoMIDs[X_AXIS*2], v, 2, TRUE);
   tbGizmoTranslate.idxGizmoObjV[X_AXIS]  = 0;
   tbGizmoTranslate.numGizmoObjV[X_AXIS]  = 2;
   tbGizmoTranslate.typeGizmoObjV[X_AXIS] = LINE_SEGMENTS;
   for (i = tbGizmoTranslate.idxGizmoObjV[X_AXIS], j = 0; j < 6; i++, j++) {
      tbGizmoTranslate.gizmoObjVex[i] = v[j];
   }
   object.SetParent(tbGizmoTranslate.baseID);
   object.Show(FALSE);
   object.SetRenderOption(Z_BUFFER, FALSE);
   
   objID = tbAux2Scene.CreateObject();                 // Translate Gizmo Y-axis Object
   object.ID(objID);
   tbGizmoTranslate.gizmoIDs[Y_AXIS]    = objID;
   tbGizmoTranslate.gizmoMIDs[Y_AXIS*2] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorY);
   v[0] = 0.0f;
   v[1] = tbGizmoRadiusTSmall/2;
   v[2] = 0.0f;
   v[3] = 0.0f;
   v[4] = tbGizmoRadiusTBig;
   v[5] = 0.0f;
   object.Lines(LINE_SEGMENTS,tbGizmoTranslate.gizmoMIDs[Y_AXIS*2], v, 2, TRUE);
   tbGizmoTranslate.idxGizmoObjV[Y_AXIS]  = tbGizmoTranslate.idxGizmoObjV[Y_AXIS-1] + tbGizmoTranslate.numGizmoObjV[Y_AXIS-1]*3;
   tbGizmoTranslate.numGizmoObjV[Y_AXIS]  = 2;
   tbGizmoTranslate.typeGizmoObjV[Y_AXIS] = LINE_SEGMENTS;
   for (i = tbGizmoTranslate.idxGizmoObjV[Y_AXIS], j = 0; j < 6; i++, j++) {
      tbGizmoTranslate.gizmoObjVex[i] = v[j];
   }

   object.SetParent(tbGizmoTranslate.baseID);
   object.Show(FALSE);
   object.SetRenderOption(Z_BUFFER, FALSE);
   
   objID = tbAux2Scene.CreateObject();                 // Translate Gizmo Z-axis Object
   object.ID(objID);
   tbGizmoTranslate.gizmoIDs[Z_AXIS]    = objID;
   tbGizmoTranslate.gizmoMIDs[Z_AXIS*2] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorZ);
   v[0] = 0.0f;
   v[1] = 0.0f;
   v[2] = tbGizmoRadiusTSmall/2;
   v[3] = 0.0f;
   v[4] = 0.0f;
   v[5] = tbGizmoRadiusTBig;
   object.Lines(LINE_SEGMENTS, tbGizmoTranslate.gizmoMIDs[Z_AXIS*2], v, 2, TRUE);
   tbGizmoTranslate.idxGizmoObjV[Z_AXIS]  = tbGizmoTranslate.idxGizmoObjV[Z_AXIS-1] + tbGizmoTranslate.numGizmoObjV[Z_AXIS-1]*3;
   tbGizmoTranslate.numGizmoObjV[Z_AXIS]  = 2;
   tbGizmoTranslate.typeGizmoObjV[Z_AXIS] = LINE_SEGMENTS;
   for (i = tbGizmoTranslate.idxGizmoObjV[Z_AXIS], j = 0; j < 6; i++, j++) {
      tbGizmoTranslate.gizmoObjVex[i] = v[j];
   }

   object.SetParent(tbGizmoTranslate.baseID);
   object.Show(FALSE);
   object.SetRenderOption(Z_BUFFER, FALSE);

   objID = tbAux2Scene.CreateObject();                 // Translate Gizmo XY-plane Object
   object.ID(objID);

   tbGizmoTranslate.gizmoIDs[XY_PLANE]      = objID;
   tbGizmoTranslate.gizmoMIDs[XY_PLANE*2]   = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorX);
   tbGizmoTranslate.gizmoMIDs[XY_PLANE*2+1] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorY);
   v[0] = tbGizmoRadiusTSmall; v[1] = 0.0f;                v[2] = 0.0f;
   v[3] = tbGizmoRadiusTSmall; v[4] = tbGizmoRadiusTSmall; v[5] = 0.0f;
   v[6] = 00.0f;               v[7] = tbGizmoRadiusTSmall; v[8] = 0.0f;
   object.Lines(OPEN_POLYLINE, tbGizmoTranslate.gizmoMIDs[XY_PLANE*2],   v,     2, TRUE);
   object.Lines(OPEN_POLYLINE, tbGizmoTranslate.gizmoMIDs[XY_PLANE*2+1], &v[3], 2, TRUE);
   tbGizmoTranslate.idxGizmoObjV[XY_PLANE]  = tbGizmoTranslate.idxGizmoObjV[XY_PLANE-1] + tbGizmoTranslate.numGizmoObjV[XY_PLANE-1]*3;
   tbGizmoTranslate.numGizmoObjV[XY_PLANE]  = 3;
   tbGizmoTranslate.typeGizmoObjV[XY_PLANE] = OPEN_POLYLINE;
   for (i = tbGizmoTranslate.idxGizmoObjV[XY_PLANE], j = 0; j < 9; i++, j++) {
      tbGizmoTranslate.gizmoObjVex[i] = v[j];
   }
   object.SetParent(tbGizmoTranslate.baseID);
   object.Show(FALSE);
   object.SetRenderOption(Z_BUFFER, FALSE);


   // create Gizmo objects
   objID = tbAux2Scene.CreateObject();      // Translate Gizmo YZ-plane Object
   object.ID(objID);
   tbGizmoTranslate.gizmoIDs[YZ_PLANE] = objID;
   tbGizmoTranslate.gizmoMIDs[YZ_PLANE*2]   = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorY);
   tbGizmoTranslate.gizmoMIDs[YZ_PLANE*2+1] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorZ);
   v[0] = 0.0f; v[1] = tbGizmoRadiusTSmall; v[2] = 0.0f;
   v[3] = 0.0f; v[4] = tbGizmoRadiusTSmall; v[5] = tbGizmoRadiusTSmall;
   v[6] = 0.0f; v[7] = 0.0f;                v[8] = tbGizmoRadiusTSmall;
   object.Lines(OPEN_POLYLINE, tbGizmoTranslate.gizmoMIDs[YZ_PLANE*2],   v,     2, TRUE);
   object.Lines(OPEN_POLYLINE, tbGizmoTranslate.gizmoMIDs[YZ_PLANE*2+1], &v[3], 2, TRUE);
   tbGizmoTranslate.idxGizmoObjV[YZ_PLANE]  = tbGizmoTranslate.idxGizmoObjV[YZ_PLANE-1] + tbGizmoTranslate.numGizmoObjV[YZ_PLANE-1]*3;
   tbGizmoTranslate.numGizmoObjV[YZ_PLANE]  = 3;
   tbGizmoTranslate.typeGizmoObjV[YZ_PLANE] = OPEN_POLYLINE;
   for (i = tbGizmoTranslate.idxGizmoObjV[YZ_PLANE], j = 0; j < 9; i++, j++) {
      tbGizmoTranslate.gizmoObjVex[i] = v[j];
   }

   object.SetParent(tbGizmoTranslate.baseID);
   object.Show(FALSE);
   object.SetRenderOption(Z_BUFFER, FALSE); 
   
   objID = tbAux2Scene.CreateObject();      // Translate Gizmo XZ-plane Object
   object.ID(objID);
   tbGizmoTranslate.gizmoIDs[ZX_PLANE]      = objID;
   tbGizmoTranslate.gizmoMIDs[ZX_PLANE*2]   = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorZ);
   tbGizmoTranslate.gizmoMIDs[ZX_PLANE*2+1] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoTranslate.colorX);
   v[0] = 0.0f;                v[1] = 0.0f; v[2] = tbGizmoRadiusTSmall;
   v[3] = tbGizmoRadiusTSmall; v[4] = 0.0f; v[5] = tbGizmoRadiusTSmall;
   v[6] = tbGizmoRadiusTSmall; v[7] = 0.0f; v[8] = 0.0f;
   object.Lines(OPEN_POLYLINE, tbGizmoTranslate.gizmoMIDs[ZX_PLANE*2],   v,     2, TRUE);
   object.Lines(OPEN_POLYLINE, tbGizmoTranslate.gizmoMIDs[ZX_PLANE*2+1], &v[3], 2, TRUE);
   tbGizmoTranslate.idxGizmoObjV[ZX_PLANE]  = tbGizmoTranslate.idxGizmoObjV[ZX_PLANE-1] + tbGizmoTranslate.numGizmoObjV[ZX_PLANE-1]*3;
   tbGizmoTranslate.numGizmoObjV[ZX_PLANE]  = 3;
   tbGizmoTranslate.typeGizmoObjV[ZX_PLANE] = OPEN_POLYLINE;
   for (i = tbGizmoTranslate.idxGizmoObjV[ZX_PLANE], j = 0; j < 9; i++, j++) {
      tbGizmoTranslate.gizmoObjVex[i] = v[j];
   }

   object.SetParent(tbGizmoTranslate.baseID);
   object.Show(FALSE);
   object.SetRenderOption(Z_BUFFER, FALSE);

   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoTranslate.baseID;
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoTranslate.gizmoIDs[X_AXIS];
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoTranslate.gizmoIDs[Y_AXIS];
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoTranslate.gizmoIDs[Z_AXIS];
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoTranslate.gizmoIDs[XY_PLANE];
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoTranslate.gizmoIDs[YZ_PLANE];
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoTranslate.gizmoIDs[ZX_PLANE];

   //--- Rotate Gizmo
   uiRB.ID(tbModelTFRotateButtonID);
   tbGizmoRotate.isShow      = uiRB.GetState();
   tbGizmoRotate.numGizmoObj = 3;
   tbGizmoRotate.iCurGizmo   = -1;
   memcpy(tbGizmoRotate.colorX,      colorR, sizeof(colorR));
   memcpy(tbGizmoRotate.colorY,      colorG, sizeof(colorG));
   memcpy(tbGizmoRotate.colorZ,      colorB, sizeof(colorB));
   memcpy(tbGizmoRotate.colorChoose, colorY, sizeof(colorY));
   tbGizmoRotate.radiusBig   = tbGizmoRadiusR;

   float r;
   float pi = 3.14159f;
   int   segment = 20;
   float vc[60];

   objID = tbAux2Scene.CreateObject();                 // Rotate Gizmo Base Object
   tbGizmoRotate.baseID = objID;
   object.ID(objID);
   object.Show(FALSE);

   objID = tbAux2Scene.CreateObject();                 // Rotate Gizmo X-axis Object
   object.ID(objID);
   tbGizmoRotate.gizmoIDs[X_AXIS] = objID;
   tbGizmoRotate.gizmoMIDs[X_AXIS*2] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoRotate.colorX);
   r = 0.0f;
   for (i = 0; i < segment; i++) {
      vc[i*3]   = 0.0f;
      vc[i*3+1] = tbGizmoRadiusR*sin(r);
      vc[i*3+2] = tbGizmoRadiusR*cos(r);
      r += pi*2/segment;
   }
   object.Lines(CLOSE_POLYLINE, tbGizmoRotate.gizmoMIDs[X_AXIS*2], vc, segment, TRUE);
   tbGizmoRotate.idxGizmoObjV[X_AXIS]  = 0;
   tbGizmoRotate.numGizmoObjV[X_AXIS]  = segment;
   tbGizmoRotate.typeGizmoObjV[X_AXIS] = CLOSE_POLYLINE;
   for (i = tbGizmoRotate.idxGizmoObjV[X_AXIS], j = 0; j < segment*3; i++, j++) {
      tbGizmoRotate.gizmoObjVex[i] = vc[j];
   }

   object.SetParent(tbGizmoRotate.baseID);
   object.Show(FALSE);
   
   objID = tbAux2Scene.CreateObject();                 // Rotate Gizmo Y-axis Object
   object.ID(objID);
   tbGizmoRotate.gizmoIDs[Y_AXIS]    = objID;
   tbGizmoRotate.gizmoMIDs[Y_AXIS*2] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoRotate.colorY);
   r = 0.0f;
   for (i = 0; i < segment; i++) {
      vc[i*3]   = -tbGizmoRadiusR*sin(r);
      vc[i*3+1] = 0.0f;
      vc[i*3+2] = tbGizmoRadiusR*cos(r);
      r += pi*2/segment;
   }
   object.Lines(CLOSE_POLYLINE, tbGizmoRotate.gizmoMIDs[Y_AXIS*2], vc, segment, TRUE);
   tbGizmoRotate.idxGizmoObjV[Y_AXIS]  = tbGizmoRotate.idxGizmoObjV[Y_AXIS-1] + tbGizmoRotate.numGizmoObjV[Y_AXIS-1]*3;
   tbGizmoRotate.numGizmoObjV[Y_AXIS]  = segment;
   tbGizmoRotate.typeGizmoObjV[Y_AXIS] = CLOSE_POLYLINE;
   for (i = tbGizmoRotate.idxGizmoObjV[Y_AXIS], j = 0; j < segment*3; i++, j++) {
      tbGizmoRotate.gizmoObjVex[i] = vc[j];
   }

   object.SetParent(tbGizmoRotate.baseID);
   object.Show(FALSE);
   
   objID = tbAux2Scene.CreateObject();                 // Rotate Gizmo Z-axis Object
   object.ID(objID);
   tbGizmoRotate.gizmoIDs[Z_AXIS] = objID;
   tbGizmoRotate.gizmoMIDs[Z_AXIS*2] = FyCreateMaterial(NULL, NULL, NULL, 0.0f, tbGizmoRotate.colorZ);
   r = 0.0f;
   for (i = 0; i < segment; i++) {
      vc[i*3]   = tbGizmoRadiusR*sin(r);
      vc[i*3+1] = tbGizmoRadiusR*cos(r);
      vc[i*3+2] = 0.0f;
      r += pi*2/segment;
   }
   object.Lines(CLOSE_POLYLINE, tbGizmoRotate.gizmoMIDs[Z_AXIS*2], vc, segment, TRUE);
   tbGizmoRotate.idxGizmoObjV[Z_AXIS]  = tbGizmoRotate.idxGizmoObjV[Z_AXIS-1] + tbGizmoRotate.numGizmoObjV[Z_AXIS-1]*3;
   tbGizmoRotate.numGizmoObjV[Z_AXIS]  = segment;
   tbGizmoRotate.typeGizmoObjV[Z_AXIS] = CLOSE_POLYLINE;
   for (i = tbGizmoRotate.idxGizmoObjV[Z_AXIS], j = 0; j < segment*3; i++, j++) {
      tbGizmoRotate.gizmoObjVex[i] = vc[j];
   }

   object.SetParent(tbGizmoRotate.baseID);
   object.Show(FALSE);
   
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoRotate.baseID;
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoRotate.gizmoIDs[X_AXIS];
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoRotate.gizmoIDs[Y_AXIS];
   tbExclusiveSaveList[tbNumExclusiveSave++] = tbGizmoRotate.gizmoIDs[Z_AXIS];

   // create the grid plane for texture space
   tbOrthoPlaneID = tbOrthoScene.CreateObject();
   object.ID(tbOrthoPlaneID);

   color[0] = 0.2f; color[1] = 0.2f; color[2] = 0.2f;
   mID = FyCreateMaterial(color, color, color, 20.0f, color);

   div = 2;
   dx = 2.0f/div;
   x = -2.0f;
   for (i = -div; i <= div; i++) {
      v[0] = -2.0f; v[1] = x; v[2] = 0.0f;
      v[3] = 2.0f; v[4] = x; v[5] = 0.0f;
      object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);
      x += dx;
   }

   x = -2.0f;
   for (i = -div; i <= div; i++) {
      v[1] = -2.0f; v[0] = x; v[2] = 0.0f;
      v[4] = 2.0f; v[3] = x; v[5] = 0.0f;
      object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);
      x += dx;
   }

   // create selected square area
   MATERIALid matID;
   FnMaterial mat;
   FnObject square;
   float vSq[24] = {0.0f};
   int idxSq[6] = {0,1,2,0,2,3};
   vSq[0] = 0.0f; vSq[1] = 0.0f; vSq[2] = 0.0f; vSq[3] = 0.0f; vSq[4] = 0.0f; vSq[5] = 1.0f;
   vSq[6] = 0.0f; vSq[7] = 0.5f; vSq[8] = 0.0f; vSq[9] = 0.0f; vSq[10] = 0.0f; vSq[11] = 1.0f;
   vSq[12] = 0.5f; vSq[13] = 0.5f; vSq[14] = 0.0f; vSq[15] = 0.0f; vSq[16] = 0.0f; vSq[17] = 1.0f;
   vSq[18] = 0.5f; vSq[19] = 0.0f; vSq[20] = 0.0f; vSq[21] = 0.0f; vSq[22] = 0.0f; vSq[23] = 1.0f;

   color[0] = color[1] = color[2] = 1.0f;
   matID = FyCreateMaterial(color, color, color, 1.0f, color);

   tbSquareAreaID = tbOrthoScene.CreateObject();
   square.ID(tbSquareAreaID);
   square.Triangles(XYZ_NORM, matID, 1, 4, 0, NULL, vSq, 2, idxSq);
   square.SetOpacity(0.2f);
   square.SetRenderOption(CULLING_MODE, CULL_NONE);
   square.SetRenderOption(VIEWING_CHECK, FALSE);
   square.Show(FALSE);
}


/*-------------------------
  draw a blue box in lines
  C.Wang 1224, 2012
 --------------------------*/
void TbDrawBlueBox(FnObject *object, float s, float *color)
{
   // clean the model
   object->RemoveAllGeometries();

   // draw a box
   MATERIALid mID = FyCreateMaterial(color, color, color, 1.0f, color);
   float v[24];
   v[0] = -s; v[1] = -s; v[2] = 0.0f;
   v[3] =  s; v[4] = -s; v[5] = 0.0f;
   v[6] =  s; v[7] =  s; v[8] = 0.0f;
   v[9] = -s; v[10] = s; v[11] = 0.0f;
   object->Lines(CLOSE_POLYLINE, mID, v, 4, TRUE);
   v[0] = -s; v[1] = -s; v[2] = s;
   v[3] =  s; v[4] = -s; v[5] = s;
   v[6] =  s; v[7] =  s; v[8] = s;
   v[9] = -s; v[10] = s; v[11] = s;
   object->Lines(CLOSE_POLYLINE, mID, v, 4, TRUE);
   v[0] = -s; v[1] = -s; v[2] = 0.0f;
   v[3] = -s; v[4] = -s; v[5] = s;
   v[6] =  s; v[7] =  s; v[8] = 0.0f;
   v[9] =  s; v[10] = s; v[11] = s;
   v[12] = -s; v[13] = s; v[14] = 0.0f;
   v[15] = -s; v[16] = s; v[17] = s;
   v[18] = s; v[19] = -s; v[20] = 0.0f;
   v[21] = s; v[22] = -s; v[23] = s;
   object->Lines(LINE_SEGMENTS, mID, v, 8, TRUE);
   v[0] = 0.0f; v[1] = 0.0f; v[2] = s*1.6f;
   v[3] = 0.0f; v[4] = 0.0f; v[5] = 0.0f;
   v[6] = 0.0f; v[7] = -s*1.6f; v[8] = 0.0f;
   object->Lines(OPEN_POLYLINE, mID, v, 3, TRUE);

   object->SetRenderOption(LIGHTING, FALSE);
}


/*------------------
  set zup or yup
  C.Wang 0115, 2014
 -------------------*/
BOOL4 TbSetZUpFunction(char *arg)
{
   if (!tbBeEditCam) return FALSE;

   FnObject object;
   object.ID(tbTestPlaneID);

   if (FyCheckWord(arg, "Z")) {
      object.Rotate(X_AXIS, 0.0f, REPLACE);
      tbCameraRef.Translate(0.0f, 0.0f, tbMainCamUp, REPLACE);
      tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
      tbCamera.Rotate(X_AXIS, 75.0f, GLOBAL);
      tbBeZUp = TRUE;
   }
   else if (FyCheckWord(arg, "Y")) {
      object.Rotate(X_AXIS, 90.0f, REPLACE);
      tbCameraRef.Translate(0.0f, tbMainCamUp, 0.0f, REPLACE);
      tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
      tbCamera.Rotate(X_AXIS, -25.0f, GLOBAL);
      tbBeZUp = FALSE;
   }
   else {
      return FALSE;
   }
   return TRUE;
}


#ifdef FLY2_RENDER
/*-------------------------------------
  render ambient occlusion on vertices
  C.Wang 1125, 2014
 --------------------------------------*/
void TbGenerateVAO()
{
   FnRenderAO renderAO(tbAORenderID);

   // clean the objects
   renderAO.CleanObjects();

   // insert the objects
   int i, numG, count, numO = tbCurScene->GetObjectNumber();
   OBJECTid objID, *objList = (OBJECTid *)FyAllocateBuffer(sizeof(OBJECTid)*numO);
   FnObject model;

   count = 0;
   for (i = 0; i < numO; i++) {
      objID = tbCurScene->GetObjectID(i);
      model.ID(objID);
      numG = model.GetGeometryNumber();
      if (numG > 0) {
         objList[count] = objID;
         count++;
      }
   }
   renderAO.AddObjects(objList, count);

   // release buffers
   FyReleaseBuffer(objList);

   // build the rendering primitives
   renderAO.Build();

   // render the AO
}
#endif


/*-------------------
  load a script file
  C.Wang 0814, 2012
 --------------------*/
BOOL4 TbLoadScript(char *arg)
{
   // find the new texture path
   char string[256], *key;
   BOOL4 beOK = FALSE;
   char fileName[256], path[128], name[128];
   FILE *fp;

   if (arg != NULL) {
      strcpy(string, arg);
      key = strtok(string, " ");
   }
   else {
      key = NULL;
   }

   if (key != NULL) {
      strcpy(fileName, key);
      FyCheckFileFormat(fileName, path, name, NULL);
      if (strlen(path) == 0) strcpy(path, ".");
      sprintf(fileName, "%s\\%s.cw2", path, name);
      beOK = TRUE;
   }
   else {
      // get current directory
      char dir[128];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "Testbed3 Script {*.cw2}\0*.cw2\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "cw2";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         FySetDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      // restore the current directory
      SetCurrentDirectory(dir);
   }

   if (beOK) {
      int fileSize;
      BYTE *pData;

      // open the file
      fp = fopen(fileName, "rb");
      if (fp != NULL) {
         // check file length
         fseek(fp, 0, SEEK_END);
         fileSize = ftell(fp);
         fseek(fp, 0, SEEK_SET);

         // load the data
         pData = (BYTE *)FyAllocateBuffer(fileSize + 100);
         fread(pData, sizeof(BYTE), fileSize, fp);

         // close the file
         fclose(fp);
         fp = NULL;

         // copy the script to system string
         TbCopySystemString((char *) pData, fileSize);

         // free the buffer
         FyReleaseBuffer(pData);
      }
      else {
         beOK = FALSE;
      }
   }
   
   return beOK;
}


/*------------------
  end a loop
  C.Wang 0814, 2012
 -------------------*/
BOOL4 TbEndLoopCmd(char *arg)
{
   if (tbCurLoopStackID < 0) {
      return FALSE;
   }

   // check ending
   TBScript *script;
   int incV = tbLoopStack[tbCurLoopStackID].incV;
   int endV = tbLoopStack[tbCurLoopStackID].endV;
   int curV = tbLoopStack[tbCurLoopStackID].curV;
   if (curV >= endV) {
      // end the loop
      tbCurLoopStackID--;
      if (tbCurLoopStackID < 0) tbCurLoopStackID = FY_NONE;
      return TRUE;
   }

   // make the script pointer back to the head of the loop
   script = &tbScript[tbCurScript];
   if (script->beRun) {
      script->length = tbLoopStack[tbCurLoopStackID].loopEntry;
   }

   // increment the counter
   tbLoopStack[tbCurLoopStackID].curV += incV;
   if (incV > 0) {
      if (tbLoopStack[tbCurLoopStackID].curV > endV) {
         tbLoopStack[tbCurLoopStackID].curV = endV;
      }
   }
   else {
      if (tbLoopStack[tbCurLoopStackID].curV < endV) {
         tbLoopStack[tbCurLoopStackID].curV = endV;
      }
   }

   // set the variable
   char value[256];
   sprintf(value, "= %d", tbLoopStack[tbCurLoopStackID].curV);
   TbSetVariableValue(tbLoopStack[tbCurLoopStackID].index, value);

   return TRUE;
}


/*------------------
  begin a loop
  C.Wang 0814, 2012
 -------------------*/
BOOL4 TbBeginLoopCmd(char *arg)
{
   char string[256], buf[256], *keyV, *keyS, *keyE, *keyI;
   int iS, iE, iI;
   TBScript *script;

   strcpy(string, arg);
   keyV = strtok(string, " ");
   if (keyV != NULL) {
      keyS = strtok(NULL, " ");
      if (keyS != NULL) {
         keyE = strtok(NULL, " ");
         if (keyE != NULL) {
            keyI = strtok(NULL, " ");
            if (keyI != NULL) {
               // check value
               iS = atoi(keyS);
               iE = atoi(keyE);
               iI = atoi(keyI);
               if (iS <= iE && iI < 0) {
                  return FALSE;
               }
               if (iE <= iS && iI > 0) {
                  return FALSE;
               }

               // insert the local variable
               tbBeUseGlobals = FALSE;
               TbSetIntegerCmd(keyV, buf, NULL);
               tbBeUseGlobals = TRUE;

               // set initial value
               char value[256];
               sprintf(value, "= %s", keyS);
               TbSetVariableValue(keyV, value);

               // push the loop to loop stack
               tbCurLoopStackID++;
               tbLoopStack[tbCurLoopStackID].startV = iS;
               tbLoopStack[tbCurLoopStackID].curV = iS;
               tbLoopStack[tbCurLoopStackID].endV = iE;
               tbLoopStack[tbCurLoopStackID].incV = iI;
               strcpy(tbLoopStack[tbCurLoopStackID].index, keyV);

               script = &tbScript[tbCurScript];
               if (script->beRun) {
                  tbLoopStack[tbCurLoopStackID].loopEntry = script->length;
               }
               else {
                  tbLoopStack[tbCurLoopStackID].loopEntry = 0;
               }

               return TRUE;
            }
         }
      }
   }

   return FALSE;
}


/*-----------------------------
  initialize the script system
  C.Wang 0814, 2012
 ------------------------------*/
void TbInitScriptSystem()
{
   tbCurScript = FY_NONE;
   for (int i = 0; i < MAX_SCRIPTS; i++) {
      tbScript[i].beRun = FALSE;
      tbScript[i].length = 0;
      tbScript[i].maxLength = 0;
      tbScript[i].systemString = NULL;
   }

   // setup home directory
   GetCurrentDirectory(256, tbHomeDir);
   TbInitializeCriticalSection(&tbScriptCS);
}


/*--------------------------------
  de-initialize the script system
  C.Wang 0814, 2012
 ---------------------------------*/
void TbDeinitScriptSystem()
{
   tbCurScript = 0;
   for (int i = 0; i < MAX_SCRIPTS; i++) {
      tbScript[i].beRun = FALSE;
      tbScript[i].length = 0;
      tbScript[i].maxLength = 0;
      FyReleaseBuffer(tbScript[i].systemString);
      tbScript[i].systemString = NULL;
   }
   TbDeleteCriticalSection(&tbScriptCS);
}


/*-------------------------------
  update string in script buffer
  C.Wang 0814, 2012
 --------------------------------*/
void TbCopySystemString(char *string, int maxLen)
{
   TBScript *script;

   TbEnterCriticalSection(&tbScriptCS, 2);

   // find the available script buffer
   int counter, sID = tbCurScript;

   if (sID < 0) sID = 0;
   script = &tbScript[sID];
   counter = MAX_SCRIPTS;
   while ((script->beRun || script->systemString != NULL) && counter > 0) {
      sID += 1;
      if (sID >= MAX_SCRIPTS) sID = 0;
      script = &tbScript[sID];
      counter--;
   }
   if (counter <= 0) return;

   script->maxLength = maxLen; //(int) strlen(string) + 1;
   script->systemString = (char *)FyAllocateBuffer(sizeof(char)*script->maxLength);

   FyMemCopy(script->systemString, string, maxLen);
   script->length = 0;
   script->beRun = FALSE;

   TbLeaveCriticalSection(&tbScriptCS);
}


/*-----------------------------------------------
  get a character from a file or a script buffer
  C.Wang 0814, 2012
 ------------------------------------------------*/
int TbGetChar()
{
   TBScript *script;
   if (tbCurScript < 0) return EOF;

   script = &tbScript[tbCurScript];
   if (script->systemString == NULL) {
      script->beRun = FALSE;
      return EOF;
   }

   if (script->length >= script->maxLength) {
      script->beRun = FALSE;
      return EOF;
   }

   int a = (int) script->systemString[script->length];
   script->length++;
   return a;
}


/*----------------------------------
  get a new line from script buffer
  C.Wang 0814, 2012
 -----------------------------------*/
int TbGetNewLine(char *str)
{
   int count, nextC;
   BOOL4 beContinue;

   count = 0;
   beContinue = FALSE;
   while ((nextC = TbGetChar()) > EOF) {
      if (nextC == '\n' || nextC == '\0') {
         if (beContinue) {
            beContinue = FALSE;
         }
         else {
            break;
         }
      }
      else if (nextC == '\\') {
         beContinue = TRUE;
      }
      else if (nextC == '\t') {
         *(str + count++) = ' ';
      }
      else if (nextC == '/') {
         *(str + count++) = '\\';
      }
      else if (nextC == 13) {
         // nothing
      }
      else {
         *(str + count++) = nextC;
      }
   }

   if (count == 0) {
      if (nextC == '\n') {
         *(str) = '\0';
         return (0);
      }
      else if (nextC == '\0') {
         *(str) = '\0';
         return (0);
      }
      else {
         *(str) = '\0';
         return (-1);
      }
   }
   else {
     *(str + count) = '\0';
     return (count);
   }
}


/*------------------------------------
  Truncate All Spaces in "str" string
  Eric Fei 0128, 2013
 -------------------------------------*/
void TbTruncateAllSpaces(char * str)
{
   char result[256];
   char *tmp = NULL;

   result[0] = '\0';
   tmp = strtok(str, " ");
   while(tmp != NULL) {
      strcat(result, tmp);
      tmp = strtok(NULL, " ");
   }

   memcpy(str, result, strlen(result) + 1);
}


/*---------------------------
  execute the current script
  C.Wang 0814, 2012
 ----------------------------*/
void TbExecuteScript(int skip)
{
   TbEnterCriticalSection(&tbScriptCS, 2);

   char s[256];
   int i, len;
   TBScript *script;
   
   if (tbCurScript < 0) {
      for (i = 0; i < MAX_SCRIPTS; i++) {
         script = &tbScript[i];
         if (script->systemString != NULL) {
            tbCurScript = i;
            break;
         }
      }

      if (i >= MAX_SCRIPTS) return;
   }

   script = &tbScript[tbCurScript];

   // activate the script
   if (skip < 0) {
      if (!script->beRun) {
         if (script->length < 0) {
            script->length = 0;
         }
      }
   }

   if (!script->beRun) {
      if (script->length == 0) {
         script->beRun = TRUE;

         // clean local variables
         for (len = 0; len < MAX_LOCALS; len++) {
            tbLocals[len].beUsed = FALSE;
         }
         tbNumLocal = 0;
      }
   }
   else {
      if (script->length >= script->maxLength) {
         script->beRun = FALSE;
         script->length = FY_NONE;

         // clean local variables
         for (len = 0; len < MAX_LOCALS; len++) {
            tbLocals[len].beUsed = FALSE;
         }
         tbNumLocal = 0;
      }
   }

   if (script->beRun) {
      // scan the script and execute the command
      tbRunScript = TRUE;
      tbBeUseGlobals = FALSE;
      while ((len = TbGetNewLine(s)) >= 0) {
         FyTruncateStringSpace(s);
         len = (int)strlen(s);

         if (len > 0) {
            // check the break command
            if (FyCheckWord(s, "break", TRUE)) {
               break;
            }

            // parse the script command
            char *command, *key, argument[256], string[512];
            strcpy(string, s);
            command = strtok(string, " ");
            key = strtok(NULL, " ");
            argument[0] = '\0';
            while (key != NULL) {
               strcat(argument, key);
               key = strtok(NULL, " ");
               if (key != NULL) {
                  strcat(argument, " ");
               }
            }
            TbCommandParser(command, argument, FALSE);
         }
      }
      tbBeUseGlobals = TRUE;

      if (len < 0) {
         script->beRun = FALSE;

         // clear the script
         FyReleaseBuffer(script->systemString);
         script->systemString = NULL;

         // check next script
         int i;
         for (i = 0; i < MAX_SCRIPTS; i++) {
            tbCurScript++;
            if (tbCurScript >= MAX_SCRIPTS) tbCurScript = 0;
            if (tbScript[tbCurScript].systemString != NULL) break;
         }
         if (i >= MAX_SCRIPTS) {
            tbCurScript = FY_NONE;
         }
      }
      tbRunScript = FALSE;
   }

   TbLeaveCriticalSection(&tbScriptCS);
}


/*------------------
  check real number
  C.Wang 0814, 2012
 -------------------*/
BOOL4 TbCheckReal(char *string)
{
   int i, len, start;
   int dd;
   char str[256];

   strcpy(str, string);
   FyTruncateStringSpace(str);

   dd = 0;
   len = (int) strlen(str);

   if (*(str) == '-' || *(str) == '+') start = 1;
   else start = 0;

   for (i = start; i < len && *(str+i) != 13 && *(str+i) != 11; i++) {
      if (*(str+i) < '0' || *(str+i) > '9') {
         if (*(str+i) == '.') {
            if (dd) {
               return FALSE;
            }
            else {
               dd = 1;
            }
         }
         else {
            return FALSE;
         }
      }
   }

   return TRUE;
}


/*---------------------
  check integer number
  C.Wang 0814, 2012
 ----------------------*/
BOOL4 TbCheckInt(char *string)
{
   int i, len, start;
   int dd;
   char str[256];

   strcpy(str, string);
   FyTruncateStringSpace(str);

   dd = 0;
   len = (int) strlen(str);

   if (*(str) == '-' || *(str) == '+')
      start = 1;
   else
      start = 0;

   for (i = start; i < len && *(str+i) != 13 && *(str+i) != 11; i++) {
      if ( *(str+i) < '0' || *(str+i) > '9' ) {
         return FALSE;
      }
   }

   return TRUE;
}


/*------------------
  set variable
  C.Wang 0814, 2012
 -------------------*/
char *TbSetVariableValue(char *cmd, char *argument)
{
   // extract the value
   char *value, *key, string[256];

   strcpy(string, argument);
   key = strtok(string, " ");
   if (key == NULL) return NULL;
   value = strtok(NULL, " ");
   if (value == NULL) return NULL;

   // check data type
   DWORD type = STRING_VARIABLE;
   BOOL4 beValue = FALSE;
   if (TbCheckReal(value)) {
      type = FLOAT_VARIABLE;
   }
   else {
      if (TbCheckInt(value)) {
         type = INTEGER_VARIABLE;
      }
      else {
         if (FyCheckWord(value, "TRUE", TRUE)) {
            type = BOOLEAN_VARIABLE;
            beValue = TRUE;
         }
         else if (FyCheckWord(value, "FALSE", FALSE)) {
            type = BOOLEAN_VARIABLE;
            beValue = FALSE;
         }
      }
   }

   // search the variable
   int i;
   for (i = 0; i < tbNumLocal; i++) {
      if (tbLocals[i].beUsed) {
         if (FyCheckWord(tbLocals[i].name, cmd, TRUE)) {
            if (tbLocals[i].type == FLOAT_VARIABLE && (type == FLOAT_VARIABLE || type == INTEGER_VARIABLE)) {
               tbLocals[i].value.f = (float) atof(value);
               strcpy(tbLocals[i].raw, value);
               return tbLocals[i].raw;
            }
            else if (tbLocals[i].type == INTEGER_VARIABLE && type == FLOAT_VARIABLE) {
               tbLocals[i].value.i = (int) atof(value);
               strcpy(tbLocals[i].raw, value);
               return tbLocals[i].raw;
            }
            else if (tbLocals[i].type == INTEGER_VARIABLE && type == INTEGER_VARIABLE) {
               tbLocals[i].value.i = (int) atoi(value);
               strcpy(tbLocals[i].raw, value);
               return tbLocals[i].raw;
            }
            else if (type == BOOLEAN_VARIABLE) {
               tbLocals[i].value.b = beValue;
               strcpy(tbLocals[i].raw, value);
               return tbLocals[i].raw;
            }

            return NULL;
         }
      }
   }

   for (i = 0; i < tbNumGlobal; i++) {
      if (tbGlobals[i].beUsed) {
         if (FyCheckWord(tbGlobals[i].name, cmd, TRUE)) {
            if (tbGlobals[i].type == FLOAT_VARIABLE && (type == FLOAT_VARIABLE || type == INTEGER_VARIABLE)) {
               tbGlobals[i].value.f = (float) atof(value);
               strcpy(tbGlobals[i].raw, value);
               return tbGlobals[i].raw;
            }
            else if (tbGlobals[i].type == INTEGER_VARIABLE && type == FLOAT_VARIABLE) {
               tbGlobals[i].value.i = (int) atof(value);
               sprintf(tbGlobals[i].raw, "%d", tbGlobals[i].value.i);
               return tbGlobals[i].raw;
            }
            else if (tbGlobals[i].type == INTEGER_VARIABLE && type == INTEGER_VARIABLE) {
               tbGlobals[i].value.i = (int) atoi(value);
               strcpy(tbGlobals[i].raw, value);
               return tbGlobals[i].raw;
            }
            else if (tbGlobals[i].type == BOOLEAN_VARIABLE && type == BOOLEAN_VARIABLE) {
               tbGlobals[i].value.b = beValue;
               strcpy(tbGlobals[i].raw, value);
               return tbGlobals[i].raw;
            }

            return NULL;
         }
      }
   }

   return NULL;
}


/*----------------------------------------------
  check variable name is an array or a variable
  C.Wang 0814, 2012
 -----------------------------------------------*/
int TbCheckVariableName(char *v, char *head)
{
   int i, j, len, value;

   len = (int) strlen(v);
   for (i = 0; i < len; i++) {
      if (v[i] == '[') break;
      head[i] = v[i];
   }

   if (i >= len) {
      head[i] = '\0';
      return 0;
   }
   else {
      int k;
      char num[256];
      head[i] = '\0';
      for (j = i + 1, k = 0; j < len; j++, k++) {
         if (v[j] == ']') break;
         num[k] = v[j];
      }
      num[k] = '\0';
      value = atoi(num);
      return value;
   }
}


/*----------------------------
  declare an integer variable
  C.Wang 0814, 2012
 -----------------------------*/
BOOL4 TbSetIntegerCmd(char *arg, char *log, void *data)
{
   int i, id, nArray;
   char string[256], vName[256], name[256], *key1, *key2, *key3;

   strcpy(string, arg);
   key1 = strtok(string, " ");
   if (key1 != NULL) {
      if (key1[0] == '$') {
         // check if the variable is an array or not
         nArray = TbCheckVariableName(key1, vName);

         // check initial values
         key2 = strtok(NULL, " ");
         if (key2 != NULL) {
            key3 = strtok(NULL, " ");
            if (key3 != NULL) {
               if (nArray == 0) {
                  id = TbRegisterVariable(key1, key3, INTEGER_VARIABLE, 0, tbBeUseGlobals);
                  sprintf(log, "%s = %s", key1, key3);
                  return TRUE;
               }
               else {
                  for (i = 0; i < nArray; i++) {
                     sprintf(name, "%s[%d]", vName, i);
                     id = TbRegisterVariable(name, key3, INTEGER_VARIABLE, 0, tbBeUseGlobals);
                  }
                  sprintf(log, "Create an integer array, %s", vName);
                  return TRUE;
               }
            }
         }
         else {
            if (nArray == 0) {
               id = TbRegisterVariable(key1, "0", INTEGER_VARIABLE, 0, tbBeUseGlobals);
               sprintf(log, "%s = 0", key1);
               return TRUE;
            }
            else {
               for (i = 0; i < nArray; i++) {
                  sprintf(name, "%s[%d]", vName, i);
                  id = TbRegisterVariable(name, "0", INTEGER_VARIABLE, 0, tbBeUseGlobals);
               }
               sprintf(log, "Create an integer array, %s", vName);
               return TRUE;
            }
         }
      }
   }
   strcpy(log, "No integer value to set");
   return TRUE;
}


/*---------------------------
  declare a boolean variable
  C.Wang 0814, 2012
 ----------------------------*/
BOOL4 TbSetBooleanCmd(char *arg, char *log, void *data)
{
   int i, id, nArray;
   char string[256], vName[256], name[256], *key1, *key2, *key3;

   strcpy(string, arg);
   key1 = strtok(string, " ");
   if (key1 != NULL) {
      if (key1[0] == '$') {
         // check if the variable is an array or not
         nArray = TbCheckVariableName(key1, vName);

         // check initial values
         key2 = strtok(NULL, " ");
         if (key2 != NULL) {
            key3 = strtok(NULL, " ");
            if (key3 != NULL) {
               if (nArray == 0) {
                  id = TbRegisterVariable(key1, key3, BOOLEAN_VARIABLE, 0, TRUE);
                  sprintf(log, "%s = %s", key1, key3);
                  return TRUE;
               }
               else {
                  for (i = 0; i < nArray; i++) {
                     sprintf(name, "%s[%d]", vName, i);
                     id = TbRegisterVariable(name, key3, BOOLEAN_VARIABLE, 0, TRUE);
                  }
                  sprintf(log, "Create a boolean array, %s", vName);
                  return TRUE;
               }
            }
         }
         else {
            if (nArray == 0) {
               id = TbRegisterVariable(key1, "TRUE", BOOLEAN_VARIABLE, 0, TRUE);
               sprintf(log, "%s = TRUE", key1);
               return TRUE;
            }
            else {
               for (i = 0; i < nArray; i++) {
                  sprintf(name, "%s[%d]", vName, i);
                  id = TbRegisterVariable(name, "TRUE", BOOLEAN_VARIABLE, 0, TRUE);
               }
               sprintf(log, "Create a boolean array, %s", vName);
               return TRUE;
            }
         }
      }
   }
   strcpy(log, "No boolean value to set");
   return TRUE;
}


/*----------------------------------
  declare a floating-point variable
  C.Wang 0814, 2012
 -----------------------------------*/
BOOL4 TbSetFloatCmd(char *arg, char *log, void *data)
{
   int i, id, nArray;
   char string[256], vName[256], name[256], *key1, *key2, *key3;

   strcpy(string, arg);
   key1 = strtok(string, " ");
   if (key1 != NULL) {
      if (key1[0] == '$') {
         // check if the variable is an array or not
         nArray = TbCheckVariableName(key1, vName);

         // check initial values
         key2 = strtok(NULL, " ");
         if (key2 != NULL) {
            key3 = strtok(NULL, " ");
            if (key3 != NULL) {
               if (nArray == 0) {
                  id = TbRegisterVariable(key1, key3, FLOAT_VARIABLE, 0, TRUE);
                  sprintf(log, "%s = %s", key1, key3);
                  return TRUE;
               }
               else {
                  for (i = 0; i < nArray; i++) {
                     sprintf(name, "%s[%d]", vName, i);
                     id = TbRegisterVariable(name, key3, FLOAT_VARIABLE, 0, TRUE);
                  }
                  sprintf(log, "Create a floating-point array, %s", vName);
                  return TRUE;
               }
            }
         }
         else {
            if (nArray == 0) {
               id = TbRegisterVariable(key1, "0.0", FLOAT_VARIABLE, 0, TRUE);
               sprintf(log, "%s = 0.0", key1);
               return TRUE;
            }
            else {
               for (i = 0; i < nArray; i++) {
                  sprintf(name, "%s[%d]", vName, i);
                  id = TbRegisterVariable(name, "0.0", FLOAT_VARIABLE, 0, TRUE);
               }
               sprintf(log, "Create a floating-point array, %s", vName);
               return TRUE;
            }
         }
      }
   }
   strcpy(log, "No floating-point value to set");
   return TRUE;
}


/*--------------------
  register a variable
  C.Wang 0814, 2012
 ---------------------*/
int TbRegisterVariable(char *name, char *raw, DWORD type, int arrayLen, BOOL4 beGlobal)
{
   int i, iOne, num, maxNum, iGet;
   TBVariable *varTab;

   if (beGlobal) {
      num = tbNumGlobal;
      maxNum = MAX_GLOBALS;
      varTab = tbGlobals;
   }
   else {
      num = tbNumLocal;
      maxNum = MAX_LOCALS;
      varTab = tbLocals;
   }

   iOne = -1;
   for (i = 0; i < num; i++) {
      if (!varTab->beUsed) {
         if (iOne < 0) {
            iOne;
         }
      }
      else {
         if (FyCheckWord(varTab->name, name, TRUE)) return i;
      }

      varTab++;
   }

   if (iOne < 0) {
      if (num >= maxNum) return FY_NONE;

      if (beGlobal) {
         iOne = tbNumGlobal;
         varTab = &tbGlobals[tbNumGlobal];
         tbNumGlobal++;
      }
      else {
         iOne = tbNumLocal;
         varTab = &tbLocals[tbNumLocal];
         tbNumLocal++;
      }
   }

   strcpy(varTab->name, name);
   strcpy(varTab->raw, raw);

   // check data type
   iGet = FY_NONE;
   varTab->type = STRING_VARIABLE;
   if (TbCheckReal(raw) && type == FLOAT_VARIABLE) {
      varTab->type = FLOAT_VARIABLE;
      varTab->value.f = (float) atof(raw);
      varTab->beUsed = TRUE;
      iGet = iOne;
   }
   else {
      if (TbCheckInt(raw) && type == INTEGER_VARIABLE) {
         varTab->type = INTEGER_VARIABLE;
         varTab->value.i = atoi(raw);
         varTab->beUsed = TRUE;
         iGet = iOne;
      }
      else {
         if (FyCheckWord(raw, "TRUE", TRUE) && type == BOOLEAN_VARIABLE) {
            varTab->type = BOOLEAN_VARIABLE;
            varTab->value.b = TRUE;
            varTab->beUsed = TRUE;
            iGet = iOne;
         }
         else if (FyCheckWord(raw, "FALSE", FALSE) && type == BOOLEAN_VARIABLE) {
            varTab->type = BOOLEAN_VARIABLE;
            varTab->value.b = FALSE;
            varTab->beUsed = TRUE;
            iGet = iOne;
         }
      }
   }

   if (iGet == FY_NONE && type == STRING_VARIABLE) {
      iGet = iOne;
   }
   return iGet;
}


/*----------------------
  unregister a variable
  C.Wang 0814, 2012
 -----------------------*/
void TbUnregisterVariable(char *name, BOOL4 beGlobal)
{
   int i, num;
   TBVariable *varTab;

   if (beGlobal) {
      num = tbNumGlobal;
      varTab = tbGlobals;
   }
   else {
      num = tbNumLocal;
      varTab = tbLocals;
   }

   for (i = 0; i < num; i++) {
      if (varTab->beUsed) {
         if (FyCheckWord(varTab->name, name, TRUE)) {
            varTab->beUsed = FALSE;
            return;
         }
      }
      varTab++;
   }
}


/*-------------------
  the command parser
  C.Wang 0115, 2014
 --------------------*/
BOOL4 TbCommandParser(char *command, char *argument, BOOL4 bePasteCommand)
{
   BOOL4 beOK, beExeOK, beOKInsert = TRUE;
   char log[256], msg[256];

   tbLogOpacity = 255;
   beExeOK = FALSE;
   TbSetLogMessage("");

   if (FyCheckWord(command, "Tool") || FyCheckWord(command, "Tool")) {
      beExeOK = TbSwitchToolModeCmd(argument);
   }
   else if (FyCheckWord(command, "Quit") || FyCheckWord(command, "Q")) {
      // "Quit" command
      TbQuitCommand *quitIt = (TbQuitCommand *) new TbQuitCommand();
      beExeOK = FcDoCommand(quitIt);
   }
   else if (FyCheckWord(command, "Up") || FyCheckWord(command, "Up")) {
      // "Updir" command
      beExeOK = TbSetZUpFunction(argument);
   }
   else if (FyCheckWord(command, "FaceMask") || FyCheckWord(command, "FM")) {
      // "FaceMask" command
      TbLoadFaceMaskCommand *faceIt = (TbLoadFaceMaskCommand *) new TbLoadFaceMaskCommand();
      beExeOK = FcDoCommand(faceIt, argument);
   }
   else if (FyCheckWord(command, "AdjustFaceMask") || FyCheckWord(command, "AdjFM")) {
      // "AdjustFaceMask" command
      TbAdjustFaceMaskCommand *adjustfaceIt = (TbAdjustFaceMaskCommand *) new TbAdjustFaceMaskCommand();
      beExeOK = FcDoCommand(adjustfaceIt, argument);
   }
   else if (FyCheckWord(command, "SaveFaceMask") || FyCheckWord(command, "SaveFM")) {
      // "SaveFaceMask" command
      TbSaveFaceMaskCommand *saveIt = (TbSaveFaceMaskCommand *) new TbSaveFaceMaskCommand();
      beExeOK = FcDoCommand(saveIt, argument);
   }
   else if (FyCheckWord(command, "Model") || FyCheckWord(command, "M")) {
      // "Model" command
      TbLoadModelCommand *modelIt = (TbLoadModelCommand *) new TbLoadModelCommand();
      beExeOK = FcDoCommand(modelIt, argument);
   }
   else if (FyCheckWord(command, "CurrentModel") || FyCheckWord(command, "CM")) {
      // "CurrentModel" command
      TbCurrentModelCommand *cModel = (TbCurrentModelCommand *) new TbCurrentModelCommand();
      beExeOK = FcDoCommand(cModel, argument);
   }
   else if (FyCheckWord(command, "NextModel") || FyCheckWord(command, "NM")) {
      // "NextModel" command
      TbNextModelCommand *nextModel = (TbNextModelCommand *) new TbNextModelCommand();
      beExeOK = FcDoCommand(nextModel, argument);
   }
   else if (FyCheckWord(command, "PreviousModel") || FyCheckWord(command, "PM")) {
      // "PreviousModel" command
      TbPreviousModelCommand *previousModel = (TbPreviousModelCommand *) new TbPreviousModelCommand();
      beExeOK = FcDoCommand(previousModel, argument);
   }
   else if (FyCheckWord(command, "FindModel") || FyCheckWord(command, "FM")) {
      // "FindModel" command
      TbFindModelCommand *findModel = (TbFindModelCommand *) new TbFindModelCommand();
      beExeOK = FcDoCommand(findModel, argument);
   }
   else if (FyCheckWord(command, "DeleteModel") || FyCheckWord(command, "DM")) {
      // "DeleteModel" command
      TbDeleteModelCommand *deleteModel = (TbDeleteModelCommand *) new TbDeleteModelCommand();
      beExeOK = FcDoCommand(deleteModel, argument);
   }
   else if (FyCheckWord(command, "SaveModel") || FyCheckWord(command, "SaveM")) {
      // "SaveModel" command
      TbSaveModelCommand *saveIt = (TbSaveModelCommand *) new TbSaveModelCommand();
      beExeOK = FcDoCommand(saveIt, argument);
   }
   else if (FyCheckWord(command, "Rename") || FyCheckWord(command, "R")) {
      // "Rename" command
      // check the type
      char value[256];
      BOOL4 beOK = TbCheckCommandAttributeS(argument, "-Material", value, 256);
      if (beOK) {
         TbRenameMaterialCommand *rn = (TbRenameMaterialCommand *) new TbRenameMaterialCommand();
         beExeOK = FcDoCommand(rn, value);
      }

      beOK = TbCheckCommandAttributeS(argument, "-Action", value, 256);
      if (beOK) {
         TbRenameActionCommand *aa = (TbRenameActionCommand *) new TbRenameActionCommand();
         beExeOK = FcDoCommand(aa, value);
      }
   }
   else if(FyCheckWord(command, "SetTransform") || FyCheckWord(command, "TF")) {
      // "Transform" command
      TbSetTransformCommand *stf = (TbSetTransformCommand *) new TbSetTransformCommand();
      beExeOK = FcDoCommand(stf, argument);
   }
   else if (FyCheckWord(command, "SetMaterial") || FyCheckWord(command, "Mat")) {
      // "SetMaterial" command
      TbSetMaterialCommand *sm = (TbSetMaterialCommand *) new TbSetMaterialCommand();
      beExeOK = FcDoCommand(sm, argument);
   }
   else if (FyCheckWord(command, "UpdateMaterial") || FyCheckWord(command, "UM")) {
      // "UpdateMaterial" command
      TbUpdateMaterialCommand *um = (TbUpdateMaterialCommand *) new TbUpdateMaterialCommand();
      beExeOK = FcDoCommand(um, argument);
   }
   else if (FyCheckWord(command, "Character") || FyCheckWord(command, "C")) {
      // "Character" command
      TbLoadCharacterCommand *actorIt = (TbLoadCharacterCommand *) new TbLoadCharacterCommand();
      beExeOK = FcDoCommand(actorIt, argument);
   }
   else if (FyCheckWord(command, "Motion") || FyCheckWord(command, "M")) {
      // "Motion" command
      TbLoadCharacterMotionCommand *actorIt = (TbLoadCharacterMotionCommand *) new TbLoadCharacterMotionCommand();
      beExeOK = FcDoCommand(actorIt, argument);
   }
   else if (FyCheckWord(command, "CharacterScale") || FyCheckWord(command, "CScale")) {
      // "CharacterScale" command
      TbSetCharacterScaleCommand *actorIt = (TbSetCharacterScaleCommand *) new TbSetCharacterScaleCommand();
      beExeOK = FcDoCommand(actorIt, argument);
   }
   else if (FyCheckWord(command, "SaveCharacter") || FyCheckWord(command, "SaveC")) {
      // "SaveCharacter" command
      TbSaveCharacterCommand *saveIt = (TbSaveCharacterCommand *) new TbSaveCharacterCommand();
      beExeOK = FcDoCommand(saveIt, argument);
   }
   else if (FyCheckWord(command, "ExportCharacter") || FyCheckWord(command, "ExportC")) {
      // "ExportCharacter" command
      TbExportCharacterCommand *exportIt = (TbExportCharacterCommand *) new TbExportCharacterCommand();
      beExeOK = FcDoCommand(exportIt, argument);
   }
   else if (FyCheckWord(command, "SetCharacter") || FyCheckWord(command, "SetC")) {
      // "SetCharacter" command
      TbSetCharacterCommand *sa = (TbSetCharacterCommand *) new TbSetCharacterCommand();
      beExeOK = FcDoCommand(sa, argument);
   }
   else if (FyCheckWord(command, "SetBody") || FyCheckWord(command, "SD")) {
      // "SetBody" command
      TbSetBodyCommand *sd = (TbSetBodyCommand *) new TbSetBodyCommand();
      beExeOK = FcDoCommand(sd, argument);
   }
   else if (FyCheckWord(command, "SetBodyPose") || FyCheckWord(command, "SetBP")) {
      // "SetBodyPose" command
      TbSetBodyPoseCommand *sbp = (TbSetBodyPoseCommand *) new TbSetBodyPoseCommand();
      beExeOK = FcDoCommand(sbp, argument);
   }
   else if (FyCheckWord(command, "DeleteBodyPose") || FyCheckWord(command, "DBP")) {
      // "DeleteBodyPose" command
      TbDeleteBodyPoseCommand *dbp = (TbDeleteBodyPoseCommand *) new TbDeleteBodyPoseCommand();
      beExeOK = FcDoCommand(dbp, argument);
   }
   else if (FyCheckWord(command, "SelectAction") || FyCheckWord(command, "SelectA")) {
      // "SelectAction" command
      TbSelectActionCommand *sa = (TbSelectActionCommand *) new TbSelectActionCommand();
      beExeOK = FcDoCommand(sa, argument);
   }
   else if (FyCheckWord(command, "DeleteCharacter") || FyCheckWord(command, "DC")) {
      // "DeleteCharacterr" command
      TbDeleteCharacterCommand *deleteActor = (TbDeleteCharacterCommand *) new TbDeleteCharacterCommand();
      beExeOK = FcDoCommand(deleteActor, argument);
   }
   else if (FyCheckWord(command, "DeleteAction") || FyCheckWord(command, "DAA")) {
      // "DeleteAction" command
      TbDeleteActionCommand *deleteAction = (TbDeleteActionCommand *) new TbDeleteActionCommand();
      beExeOK = FcDoCommand(deleteAction, argument);
   }
   else if (FyCheckWord(command, "NewAction") || FyCheckWord(command, "AA")) {
      // "NewAction" command
      TbNewActionCommand *newAction = (TbNewActionCommand *) new TbNewActionCommand();
      beExeOK = FcDoCommand(newAction, argument);
   }
   else if (FyCheckWord(command, "EditAction") || FyCheckWord(command, "MA")) {
      // "EditAction" command
      TbEditActionCommand *editAction = (TbEditActionCommand *) new TbEditActionCommand();
      beExeOK = FcDoCommand(editAction, argument);
   }
   else if (FyCheckWord(command, "SelectFace") || FyCheckWord(command, "SF")) {
      // "SelectFace" command
      TbSelectFaceCommand *sf = (TbSelectFaceCommand *) new TbSelectFaceCommand();
      beExeOK = FcDoCommand(sf, argument);
   }
   else if (FyCheckWord(command, "SelectFaceMaterial") || FyCheckWord(command, "SFMat")) {
      // "SelectFaceMaterial" command
      TbSelectFaceMaterialCommand *sf = (TbSelectFaceMaterialCommand *) new TbSelectFaceMaterialCommand();
      beExeOK = FcDoCommand(sf, argument);
   }
   else if (FyCheckWord(command, "Wireframe") || FyCheckWord(command, "W")) {
      // "Wireframe" command
      TbSetWireframeCommand *wireframeIt = (TbSetWireframeCommand *) new TbSetWireframeCommand();
      beExeOK = FcDoCommand(wireframeIt, argument);
   }
   else if (FyCheckWord(command, "Alpha") || FyCheckWord(command, "A")) {
      // "Alpha" command
      TbSetAlphaCommand *alphaIt = (TbSetAlphaCommand *) new TbSetAlphaCommand();
      beExeOK = FcDoCommand(alphaIt, argument);
   }
   else if (FyCheckWord(command, "Show") || FyCheckWord(command, "S")) {
      // "Show" command
      TbSetShowCommand *showIt = (TbSetShowCommand *) new TbSetShowCommand();
      beExeOK = FcDoCommand(showIt, argument);
   }
   else if (FyCheckWord(command, "SelectPose") || FyCheckWord(command, "SP")) {
      // select pose for previewing
      beExeOK = TbSelectPoseCmd(argument);
   }
   else if (FyCheckWord(command, "PlayPose") || FyCheckWord(command, "PP")) {
      // play test pose
      beExeOK = TbPlayTestPoseCmd(argument);
   }
   else if (FyCheckWord(command, "Aux") || FyCheckWord(command, "Aux")) {
      // twiddle on/off aux view
      beExeOK = TbShowAuxViewCmd(argument);
   }
   else if (FyCheckWord(command, "Ortho") || FyCheckWord(command, "Ortho")) {
      // twiddle on/off orthogonal view
      beExeOK = TbShowOrthoViewCmd(argument);
   }
   else if (FyCheckWord(command, "Speed") || FyCheckWord(command, "s")) {
      // set play speed
      beExeOK = TbSetPlaySpeedCmd(argument);
   }
   else if (FyCheckWord(command, "SetRenderGroup") || FyCheckWord(command, "rg")) {
      // "SetRenderGroup" command
      TbSetRenderGroupCommand *setRGIt = (TbSetRenderGroupCommand *) new TbSetRenderGroupCommand();
      beExeOK = FcDoCommand(setRGIt, argument);
   }
   else if (FyCheckWord(command, "Smooth") || FyCheckWord(command, "sm")) {
      // smooth the vertex normals
      beExeOK = TbSmoothModelCmd(argument);
   }
   else if (FyCheckWord(command, "SetPath") || FyCheckWord(command, "sp")) {
      // "SetPath" command
      TbSetPathCommand *setPathIt = (TbSetPathCommand *) new TbSetPathCommand();
      beExeOK = FcDoCommand(setPathIt, argument);
   }
   else if (FyCheckWord(command, "MakeBaseMove") || FyCheckWord(command, "mbm")) {
      // make base move command - this command can not undo ...... Orz
      beExeOK = TbMakeBaseMoveCmd(argument);
   }
   else if (FyCheckWord(command, "Load") || FyCheckWord(command, "load")) {
      if (!tbRunScript) {
         // load a script
         beExeOK = TbLoadScript(argument);
      }
      else {
         beExeOK = TRUE;
      }
   }
   else if (FyCheckWord(command, "BeginLoop") || FyCheckWord(command, "bLoop")) {
      // begin the loop
      TbBeginLoopCmd(argument);
      beExeOK = TRUE;
   }
   else if (FyCheckWord(command, "EndLoop") || FyCheckWord(command, "eLoop")) {
      // end loop
      TbEndLoopCmd(argument);
      beExeOK = TRUE;
   }
   else if (FyCheckWord(command, "RestoreWorkDirectory") || FyCheckWord(command, "RWD")) {
      if (FyCheckWord(argument, "-OFF")) {
         tbBeRestoreCurrentDir = FALSE;
      }
      else {
         tbBeRestoreCurrentDir = TRUE;
      }
   }
   else if (FyCheckWord(command, "NewFacialExpression") || FyCheckWord(command, "NFE")) {
      // "NewFacialExpression" command
      TbNewFacialExpressionCommand *newFacialExp = (TbNewFacialExpressionCommand *) new TbNewFacialExpressionCommand();
      beExeOK = FcDoCommand(newFacialExp, argument);
   }
   else if (FyCheckWord(command, "RemoveFacialExpression") || FyCheckWord(command, "RemoveFE")) {
      // "RemoveFacialExpression" command
      TbRemoveFacialExpressionCommand *removeF = (TbRemoveFacialExpressionCommand *) new TbRemoveFacialExpressionCommand();
      beExeOK = FcDoCommand(removeF, argument);
   }
   else if (FyCheckWord(command, "FacialExpressionLength") || FyCheckWord(command, "FELen")) {
      // "FacialExpressionLength" command
      TbSetFacialExpressionLengthCommand *feLen = (TbSetFacialExpressionLengthCommand *) new TbSetFacialExpressionLengthCommand();
      beExeOK = FcDoCommand(feLen, argument);
   }
   else if (FyCheckWord(command, "UpdateFacialExpressionKey") || FyCheckWord(command, "updateFEKey")) {
      // "UpdateFacialExpressionKey" command
      TbUpdateFacialExpressionKeyCommand *updateF = (TbUpdateFacialExpressionKeyCommand *) new TbUpdateFacialExpressionKeyCommand();
      beExeOK = FcDoCommand(updateF, argument);
   }
   else if (FyCheckWord(command, "RemoveFacialExpressionKey") || FyCheckWord(command, "removeFEKey")) {
      // "RemoveFacialExpressionKey" command
      TbRemoveFacialExpressionKeyCommand *removeF = (TbRemoveFacialExpressionKeyCommand *) new TbRemoveFacialExpressionKeyCommand();
      beExeOK = FcDoCommand(removeF, argument);
   }
   else if (FyCheckWord(command, "AddFacialExpressionKey") || FyCheckWord(command, "addFEKey")) {
      // "AddFacialExpressionKey" command
      TbAddFacialExpressionKeyCommand *addF = (TbAddFacialExpressionKeyCommand *) new TbAddFacialExpressionKeyCommand();
      beExeOK = FcDoCommand(addF, argument);
   }
   else if (FyCheckWord(command, "MakeCurrentFacialExpression") || FyCheckWord(command, "MakeFE")) {
      // "MakeCurrentFacialExpression" command
      TbMakeCurrentFacialExpressionCommand *makeF = (TbMakeCurrentFacialExpressionCommand *) new TbMakeCurrentFacialExpressionCommand();
      beExeOK = FcDoCommand(makeF, argument);
   }
   else if (FyCheckWord(command, "RenameCurrentFacialExpression") || FyCheckWord(command, "RenameFE")) {
      // "RenameCurrentFacialExpression" command
      TbRenameCurrentFacialExpressionCommand *renameF = (TbRenameCurrentFacialExpressionCommand *) new TbRenameCurrentFacialExpressionCommand();
      beExeOK = FcDoCommand(renameF, argument);
   }
   else if (FyCheckWord(command, "NewGameFXSystem") || FyCheckWord(command, "NFXS")) {
      // "NewGameFXSystem" command
      TbNewGameFXSystemCommand *newGameFXS = (TbNewGameFXSystemCommand *) new TbNewGameFXSystemCommand();
      beExeOK = FcDoCommand(newGameFXS, argument);
   }
   else if (FyCheckWord(command, "SelectGameFXSystem") || FyCheckWord(command, "SFXS")) {
      // "SelectGameFXSystem" command
      TbSelectGameFXSystemCommand *selectGameFX = (TbSelectGameFXSystemCommand *) new TbSelectGameFXSystemCommand();
      beExeOK = FcDoCommand(selectGameFX, argument);
   }
   else if (FyCheckWord(command, "RenameGameFXSystem") || FyCheckWord(command, "RFXS")) {
      // "RenameGameFXSystem" command
      TbRenameGameFXSystemCommand *renameGameFX = (TbRenameGameFXSystemCommand *) new TbRenameGameFXSystemCommand();
      beExeOK = FcDoCommand(renameGameFX, argument);
   }
   else if (FyCheckWord(command, "LoadGameFXSystem") || FyCheckWord(command, "LFX")) {
      // "LoadGameFXSystem" command
      TbLoadGameFXSystemCommand *loadGameFX = (TbLoadGameFXSystemCommand *) new TbLoadGameFXSystemCommand();
      beExeOK = FcDoCommand(loadGameFX, argument);
   }
   else if (FyCheckWord(command, "DeleteGameFXSystem") || FyCheckWord(command, "DFX")) {
      // "DeleteGameFXSystem" command
      TbDeleteGameFXSystemCommand *deleteGameFX = (TbDeleteGameFXSystemCommand *) new TbDeleteGameFXSystemCommand();
      beExeOK = FcDoCommand(deleteGameFX, argument);
   }
   else if (FyCheckWord(command, "SaveGameFXSystem") || FyCheckWord(command, "SFX")) {
      // "SaveGameFXSystem" command
      TbSaveGameFXSystemCommand *saveGameFX = (TbSaveGameFXSystemCommand *) new TbSaveGameFXSystemCommand();
      beExeOK = FcDoCommand(saveGameFX, argument);
   }
   else if (FyCheckWord(command, "NewGameFX") || FyCheckWord(command, "NFX")) {
      // "NewGameFX" command
      TbNewGameFXCommand *newGameFX = (TbNewGameFXCommand *) new TbNewGameFXCommand();
      beExeOK = FcDoCommand(newGameFX, argument);
   }
   else if (FyCheckWord(command, "DeleteGameFX") || FyCheckWord(command, "DFX")) {
      // "DeleteGameFX" command
      TbDeleteGameFXCommand *deleteGameFX = (TbDeleteGameFXCommand *) new TbDeleteGameFXCommand();
      beExeOK = FcDoCommand(deleteGameFX, argument);
   }
   else if (FyCheckWord(command, "SelectGameFX") || FyCheckWord(command, "SFX")) {
      // "SelectGameFX" command
      TbSelectGameFXCommand *selectGameFX = (TbSelectGameFXCommand *) new TbSelectGameFXCommand();
      beExeOK = FcDoCommand(selectGameFX, argument);
   }
   else if (FyCheckWord(command, "ModifyFXKey") || FyCheckWord(command, "ModifyFXKey")) {
      // "ModifyFXKey" command
      TbModifyGameFXKeyCommand *modifyKey = (TbModifyGameFXKeyCommand *) new TbModifyGameFXKeyCommand();
      beExeOK = FcDoCommand(modifyKey, argument);
   }
   else if (FyCheckWord(command, "RemoveFXKey") || FyCheckWord(command, "RemoveFXKey")) {
      // "RemoveFXKey" command
      TbRemoveGameFXKeyCommand *removeKey = (TbRemoveGameFXKeyCommand *) new TbRemoveGameFXKeyCommand();
      beExeOK = FcDoCommand(removeKey, argument);
   }
   else if (FyCheckWord(command, "RenameGameFX") || FyCheckWord(command, "RFX")) {
      // "RenameGameFX" command
      TbRenameGameFXCommand *renameGameFX = (TbRenameGameFXCommand *) new TbRenameGameFXCommand();
      beExeOK = FcDoCommand(renameGameFX, argument);
   }
   else if (FyCheckWord(command, "SetFXParent") || FyCheckWord(command, "FXParent")) {
      // "SetFXParent" command
      TbSetFXParentCommand *setParentFX = (TbSetFXParentCommand *) new TbSetFXParentCommand();
      beExeOK = FcDoCommand(setParentFX, argument);
   }
   else if (FyCheckWord(command, "ChangePlateType") || FyCheckWord(command, "PlateType")) {
      // "ChangePlateType" command
      TbChangePlateTypeCommand *changeFXType = (TbChangePlateTypeCommand *) new TbChangePlateTypeCommand();
      beExeOK = FcDoCommand(changeFXType, argument);
   }
   else if (FyCheckWord(command, "LoadPlateTexture") || FyCheckWord(command, "LPT")) {
      // "LoadPlateTexture" command
      TbLoadPlateTextureCommand *loadPlateTexture = (TbLoadPlateTextureCommand *) new TbLoadPlateTextureCommand();
      beExeOK = FcDoCommand(loadPlateTexture, argument);
   }
   else if (FyCheckWord(command, "LoadPlateAlphamap") || FyCheckWord(command, "LPA")) {
      // "LoadPlateAlpha" command
      TbLoadPlateAlphamapCommand *loadPlateTexture = (TbLoadPlateAlphamapCommand *) new TbLoadPlateAlphamapCommand();
      beExeOK = FcDoCommand(loadPlateTexture, argument);
   }
   else if (FyCheckWord(command, "LoadGeometryTexture") || FyCheckWord(command, "LGT")) {
      // "LoadGeometryTexture" command
      TbLoadGeometryTextureCommand *loadGeometryTexture = (TbLoadGeometryTextureCommand *) new TbLoadGeometryTextureCommand();
      beExeOK = FcDoCommand(loadGeometryTexture, argument);
   }
   else if (FyCheckWord(command, "LoadGeometryAlphamap") || FyCheckWord(command, "LGA")) {
      // "LoadGeometryAlphamap" command
      TbLoadGeometryAlphamapCommand *loadGeometryTexture = (TbLoadGeometryAlphamapCommand *) new TbLoadGeometryAlphamapCommand();
      beExeOK = FcDoCommand(loadGeometryTexture, argument);
   }
   else if (FyCheckWord(command, "LoadParticleTexture") || FyCheckWord(command, "LParT")) {
      // "LoadParticleTexture" command
      TbLoadParticleTextureCommand *loadParticleTexture = (TbLoadParticleTextureCommand *) new TbLoadParticleTextureCommand();
      beExeOK = FcDoCommand(loadParticleTexture, argument);
   }
   else if (FyCheckWord(command, "LoadParticleAlphamap") || FyCheckWord(command, "LParA")) {
      // "LoadParticleAlphamap" command
      TbLoadParticleAlphamapCommand *loadParticleTexture = (TbLoadParticleAlphamapCommand *) new TbLoadParticleAlphamapCommand();
      beExeOK = FcDoCommand(loadParticleTexture, argument);
   }
   else if (FyCheckWord(command, "RemovePlateTexture") || FyCheckWord(command, "RPT")) {
      // "RemovePlateTexture" command
      TbRemovePlateTextureCommand *removePlateTexture = (TbRemovePlateTextureCommand *) new TbRemovePlateTextureCommand();
      beExeOK = FcDoCommand(removePlateTexture, argument);
   }
   else if (FyCheckWord(command, "RemoveGeometryTexture") || FyCheckWord(command, "RGT")) {
      // "RemoveGeometryTexture" command
      TbRemoveGeometryTextureCommand *removeGeometryTexture = (TbRemoveGeometryTextureCommand *) new TbRemoveGeometryTextureCommand();
      beExeOK = FcDoCommand(removeGeometryTexture, argument);
   }
   else if (FyCheckWord(command, "RemoveParticleTexture") || FyCheckWord(command, "RParT")) {
      // "RemoveParticleTexture" command
      TbRemoveParticleTextureCommand *removeParticleTexture = (TbRemoveParticleTextureCommand *) new TbRemoveParticleTextureCommand();
      beExeOK = FcDoCommand(removeParticleTexture, argument);
   }
   else if (FyCheckWord(command, "ChangePlateBlendType") || FyCheckWord(command, "PlateBT")) {
      // "ChangePlateBlendType" command
      TbChangePlateBlendTypeCommand *changeFXBlendType = (TbChangePlateBlendTypeCommand *) new TbChangePlateBlendTypeCommand();
      beExeOK = FcDoCommand(changeFXBlendType, argument);
   }
   else if (FyCheckWord(command, "ChangeParticleBlendType") || FyCheckWord(command, "ParticleBT")) {
      // "ChangeParticleBlendType" command
      TbChangeParticleBlendTypeCommand *changeFXBlendType = (TbChangeParticleBlendTypeCommand *) new TbChangeParticleBlendTypeCommand();
      beExeOK = FcDoCommand(changeFXBlendType, argument);
   }
   else if (FyCheckWord(command, "ModifyBaseFXPosition") || FyCheckWord(command, "FXPosition")) {
      // "ModifyBaseFXPosition" command
      TbModifyBaseFXPositionCommand *changeFXPosition = (TbModifyBaseFXPositionCommand *) new TbModifyBaseFXPositionCommand();
      beExeOK = FcDoCommand(changeFXPosition, argument);
   }
   else if (FyCheckWord(command, "ModifyBaseFXRotation") || FyCheckWord(command, "FXRotation")) {
      // "ModifyBaseFXRotation" command
      TbModifyBaseFXRotationCommand *changeFXRotation = (TbModifyBaseFXRotationCommand *) new TbModifyBaseFXRotationCommand();
      beExeOK = FcDoCommand(changeFXRotation, argument);
   }
   else if (FyCheckWord(command, "ModifyBaseFXVisibility") || FyCheckWord(command, "FXShow")) {
      // "ModifyBaseFXVisibility" command
      TbModifyBaseFXVisibilityCommand *showFX = (TbModifyBaseFXVisibilityCommand *) new TbModifyBaseFXVisibilityCommand();
      beExeOK = FcDoCommand(showFX, argument);
   }
   else if (FyCheckWord(command, "ModifyBaseFXLife") || FyCheckWord(command, "FXLife")) {
      // "ModifyBaseFXLife" command
      TbModifyBaseFXLifeCommand *lifeFX = (TbModifyBaseFXLifeCommand *) new TbModifyBaseFXLifeCommand();
      beExeOK = FcDoCommand(lifeFX, argument);
   }
   else if (FyCheckWord(command, "ModifyPlateSize") || FyCheckWord(command, "PlateSize")) {
      // "ModifyPlateSize" command
      TbModifyPlateSizeCommand *changePlateSize = (TbModifyPlateSizeCommand *) new TbModifyPlateSizeCommand();
      beExeOK = FcDoCommand(changePlateSize, argument);
   }
   else if (FyCheckWord(command, "ModifyPlateColor") || FyCheckWord(command, "PlateColor")) {
      // "ModifyPlateColor" command
      TbModifyPlateColorCommand *changePlateColor = (TbModifyPlateColorCommand *) new TbModifyPlateColorCommand();
      beExeOK = FcDoCommand(changePlateColor, argument);
   }
   else if (FyCheckWord(command, "ModifyParticleLife") || FyCheckWord(command, "ParticleLife")) {
      // "ModifyParticleLife" command
      TbModifyParticleLifeCommand *changeParticleLife = (TbModifyParticleLifeCommand *) new TbModifyParticleLifeCommand();
      beExeOK = FcDoCommand(changeParticleLife, argument);
   }
   else if (FyCheckWord(command, "ModifyParticleTextureAnimation") || FyCheckWord(command, "ParticleTexAnim")) {
      // "ModifyParticleTextureAnimation" command
      TbModifyParticleTexAnimCommand *changeParticleTexAnim = (TbModifyParticleTexAnimCommand *) new TbModifyParticleTexAnimCommand();
      beExeOK = FcDoCommand(changeParticleTexAnim, argument);
   }
   else if (FyCheckWord(command, "ModifyParticleSize") || FyCheckWord(command, "ParticleSize")) {
      // "ModifyParticleSize" command
      TbModifyParticleSizeCommand *changeParticleSize = (TbModifyParticleSizeCommand *) new TbModifyParticleSizeCommand();
      beExeOK = FcDoCommand(changeParticleSize, argument);
   }
   else if (FyCheckWord(command, "ModifyParticleVelocity") || FyCheckWord(command, "ParticleVelocity")) {
      // "ModifyParticleVelocity" command
      TbModifyParticleVelocityCommand *changeParticleVelocity = (TbModifyParticleVelocityCommand *) new TbModifyParticleVelocityCommand();
      beExeOK = FcDoCommand(changeParticleVelocity, argument);
   }
   else if (FyCheckWord(command, "ModifyParticleBorn") || FyCheckWord(command, "ParticleBorn")) {
      // "ModifyParticleBorn" command
      TbModifyParticleBornCommand *changeParticleBorn = (TbModifyParticleBornCommand *) new TbModifyParticleBornCommand();
      beExeOK = FcDoCommand(changeParticleBorn, argument);
   }
   else if (FyCheckWord(command, "ChangeForceParticle") || FyCheckWord(command, "ChangeForceParticle")) {
      // "ChangeForceParticle" command
      TbChangeParticleUsedCommand *changeForceParticleUsed = (TbChangeParticleUsedCommand *) new TbChangeParticleUsedCommand();
      beExeOK = FcDoCommand(changeForceParticleUsed, argument);
   }
   else if (FyCheckWord(command, "ModifyForceParam") || FyCheckWord(command, "ModifyForce")) {
      // "ModifyForceParam" command
      TbModifyForceParamCommand *changeForce = (TbModifyForceParamCommand *) new TbModifyForceParamCommand();
      beExeOK = FcDoCommand(changeForce, argument);
   }
   else if (FyCheckWord(command, "LoadGeometryFXModel") || FyCheckWord(command, "LFXModel")) {
      // "LoadGeometryFXModel" command
      TbLoadGeometryFXModelCommand *loadModel = (TbLoadGeometryFXModelCommand *) new TbLoadGeometryFXModelCommand();
      beExeOK = FcDoCommand(loadModel, argument);
   }
   else if (FyCheckWord(command, "ChangeGeometryBlendType") || FyCheckWord(command, "GeometryBT")) {
      // "ChangeGeometryBlendType" command
      TbChangeGeometryBlendTypeCommand *changeFXBlendType = (TbChangeGeometryBlendTypeCommand *) new TbChangeGeometryBlendTypeCommand();
      beExeOK = FcDoCommand(changeFXBlendType, argument);
   }
   else if (FyCheckWord(command, "LoadGameFXAudio") || FyCheckWord(command, "GXAudLoad")) {
      // "LoadGameFXAudio" command
      TbGameFXAudioLoadFileCommand *loadAudio = (TbGameFXAudioLoadFileCommand *) new TbGameFXAudioLoadFileCommand();
      beExeOK = FcDoCommand(loadAudio, argument);
   }
   else if (FyCheckWord(command, "ChangeGameFXAudioStartTime") || FyCheckWord(command, "GXAudStart")) {
      // "ChangeGameFXAudioStartTime" command
      TbGameFXAudioChangeStartCommand *changeStart = (TbGameFXAudioChangeStartCommand *) new TbGameFXAudioChangeStartCommand();
      beExeOK = FcDoCommand(changeStart, argument);
   }
   else if (FyCheckWord(command, "ChangeGameFXSwordCharacter") || FyCheckWord(command, "GXSwdChar")) {
      // "ChangeGameFXSwordCharacter" command
      TbGameFXSwordChangeCharacterCommand *changeChar = (TbGameFXSwordChangeCharacterCommand *) new TbGameFXSwordChangeCharacterCommand();
      beExeOK = FcDoCommand(changeChar, argument);
   }
   else if (FyCheckWord(command, "ChangeGameFXSwordDummy") || FyCheckWord(command, "GXSwdDmy")) {
      // "ChangeGameFXSwordDummy" command
      TbGameFXSwordChangeDummyCommand *changeDmy = (TbGameFXSwordChangeDummyCommand *) new TbGameFXSwordChangeDummyCommand();
      beExeOK = FcDoCommand(changeDmy, argument);
   }
   else if (FyCheckWord(command, "ChangeGameFXSwordParam") || FyCheckWord(command, "GXSwdParam")) {
      // "ChangeGameFXSwordParam" command
      TbGameFXSwordChangeParamCommand *changeParam = (TbGameFXSwordChangeParamCommand *) new TbGameFXSwordChangeParamCommand();
      beExeOK = FcDoCommand(changeParam, argument);
   }
   else if (FyCheckWord(command, "ChangeGameFXSwordTexture") || FyCheckWord(command, "GXSwdTex")) {
      // "ChangeGameFXSwordTexture" command
      TbGameFXSwordChangeTextureCommand *changeTex = (TbGameFXSwordChangeTextureCommand *) new TbGameFXSwordChangeTextureCommand();
      beExeOK = FcDoCommand(changeTex, argument);
   }
   else if (FyCheckWord(command, "ChangeGameFXSwordAlphamap") || FyCheckWord(command, "GXSwdAlpha")) {
      // "ChangeGameFXSwordAlphamap" command
      TbGameFXSwordChangeAlphamapCommand *changeAlpha = (TbGameFXSwordChangeAlphamapCommand *) new TbGameFXSwordChangeAlphamapCommand();
      beExeOK = FcDoCommand(changeAlpha, argument);
   }
#ifdef FLY2_RENDER
   else if (FyCheckWord(command, "generateVAO") || FyCheckWord(command, "vao")) {
      // generate ambient occlusion on vertices
      TbGenerateVAO();
   }
#endif
   else if (FyCheckWord(command, "Undo") || FyCheckWord(command, "UD")) {
      // for undo command
      beOK = FcUndoLastCommand(log);
      if (beOK) {
         sprintf(msg, "Undo : %s", log); 
         TbSetLogMessage(msg);
      }
      else {
         TbSetLogMessage("Nothing undo");
      }
      beExeOK = TRUE;
   }
   else if (FyCheckWord(command, "Redo") || FyCheckWord(command, "RD")) {
      // for redo command
      beOK = FcRedoCommand(log);
      if (beOK) {
         sprintf(msg, "Redo : %s", log); 
         TbSetLogMessage(msg);
      }
      else {
         TbSetLogMessage("Nothing re-do");
      }
      beExeOK = TRUE;
   }
   else if (FyCheckWord(command, "#") || command[0] == '#') {
      beOKInsert = FALSE;
      beExeOK = TRUE;
   }
   else {
      beOKInsert = FALSE;
      beExeOK = FALSE;
   }

   if (!beExeOK) {
      TbSetLogMessage("Failed to execute commands!");
   }

   // insert the command into the history
   if (beOKInsert) {
      if (bePasteCommand) {
         if (argument != NULL && strlen(argument) > 0) {
            sprintf(tbCommandHistory[tbCurCommand], "%s %s", command, argument);
         }
         else {
            strcpy(tbCommandHistory[tbCurCommand], command);
         }
      }

      if (tbCurCommand == tbNumCommandHistory - 1) {
         // the current command slot is the last one on the history list
         int i;
         for (i = 0; i < tbCurCommand; i++) {
            strcpy(tbCommandHistory[i], tbCommandHistory[i + 1]);
         }
      }
      else {
         tbCurCommand++;
         tbCurCommandPtr = tbCommandHistory[tbCurCommand];
      }
   }

   // check command queue for undo/redo management
   beOK = FcRedoCommand(log, TRUE);
   if (beOK) {
      // enable the redo memu
   }
   else {
      // disable the redo memu
   }

   beOK = FcUndoLastCommand(log, TRUE);
   if (beOK) {
      // enable the undo memu
   }
   else {
      // disable the undo memu
   }

   return beExeOK;
}


/*----------------------------
  command input function keys
  C.Wang 0814, 2012
 -----------------------------*/
void TbCommandFunKey(BYTE code, BOOL4 value)
{
   if (value) {
      if (code == FY_UP) {   // upword
         if (tbCurCommand > 0) {
            tbCurCommand--;
            tbCurCommandPtr = tbCommandHistory[tbCurCommand];
            tbCommandCursor = (int) strlen(tbCurCommandPtr);
         }
      }
      else if (code == FY_DOWN) {   // downword
         if (tbCurCommand < tbNumCommandHistory - 1) {
            if (strlen(tbCommandHistory[tbCurCommand]) > 0) {
               tbCurCommand++;
               tbCurCommandPtr = tbCommandHistory[tbCurCommand];
               tbCommandCursor = (int) strlen(tbCurCommandPtr);
            }
         }
      }
      else if (code == FY_RIGHT) {   // right arrow
         if (tbCommandCursor < (int) strlen(tbCurCommandPtr)) {
            tbCommandCursor++;
         }
      }
      else if (code == FY_LEFT) {   // left arrow
         if (tbCommandCursor > 0) {
            tbCommandCursor--;
         }
      }
   }
}


/*-----------------------
  command input callback
  C.Wang 0814, 2012
 ------------------------*/
void TbCommandInput(WORD code)
{
   if (code == 8) {
      if (tbCommandCursor <= 0) return;

      // BackSpace
      tbCommandCursor--;
      tbCurCommandPtr[tbCommandCursor] = '\0';
   }
   else if (code == 13 || code == 27) {
      // "Return"
      if (code == 13) {
         char *command, *key, argument[256], string[256];

         strcpy(string, tbCurCommandPtr);
         command = strtok(string, " ");
         key = strtok(NULL, " ");
         argument[0] = '\0';
         while (key != NULL) {
            strcat(argument, key);
            key = strtok(NULL, " ");
            if (key != NULL) {
               strcat(argument, " ");
            }
         }
         TbCommandParser(command, argument, FALSE);
      }

      // clear the input command
      tbCommandCursor = 0;
      tbCurCommandPtr[tbCommandCursor] = '\0';
   }
   else {
      if (tbCommandCursor >= MAX_STRING_CHARACTERS - 1) return;
      tbCurCommandPtr[tbCommandCursor] = (char) code;
      tbCommandCursor++;
      tbCurCommandPtr[tbCommandCursor] = '\0';
   }
}


/*--------------------------------
  check and get boolean attribute
  C.Wang 0814, 2012
 ---------------------------------*/
BOOL4 TbCheckCommandAttributeB(char *string, char *attribute, BOOL4 *value, int maxLen)
{
   int i;
   char localString[512], *key, *key2;

   strcpy(localString, string);

   key = strtok(localString, " ");
   while (key != NULL) {
      if (FyCheckWord(key, attribute, TRUE)) {
         if (value != NULL) {
            for (i = 0; i < maxLen; i++) {
               key2 = strtok(NULL, " ");
               if (key2 == NULL) return FALSE;
               value[i] = (BOOL4) atoi(key2);
            }
         }
         return TRUE;
      }

      // next word
      key = strtok(NULL, " ");
   }

   return FALSE;
}


/*-----------------------------------
  check and get the string attribute
  C.Wang 0814, 2012
 ------------------------------------*/
BOOL4 TbCheckCommandAttributeS(char *string, char *attribute, char *value, int maxLen)
{
   char localString[512], bufString[256], *key, *key2;

   strcpy(localString, string);

   key = strtok(localString, " ");
   while (key != NULL) {
      if (FyCheckWord(key, attribute, TRUE)) {
         if (value != NULL) {
            key2 = strtok(NULL, " ");
            if (key2 == NULL) return FALSE;

            // check the string buffer length
            strcpy(bufString, key2);
            if ((int) strlen(bufString) >= maxLen - 1) {
               bufString[maxLen - 1] = '\0';
            }
            strcpy(value, bufString);
         }
         return TRUE;
      }

      // next word
      key = strtok(NULL, " ");
   }

   return FALSE;
}


/*--------------------------------
  check and get integer attribute
  C.Wang 0814, 2012
 ---------------------------------*/
BOOL4 TbCheckCommandAttributeI(char *string, char *attribute, int *value, int maxLen)
{
   int i;
   char localString[512], *key, *key2;

   strcpy(localString, string);

   key = strtok(localString, " ");
   while (key != NULL) {
      if (FyCheckWord(key, attribute, TRUE)) {
         if (value != NULL) {
            for (i = 0; i < maxLen; i++) {
               key2 = strtok(NULL, " ");
               if (key2 == NULL) return FALSE;
               value[i] = atoi(key2);
            }
         }
         return TRUE;
      }

      // next word
      key = strtok(NULL, " ");
   }

   return FALSE;
}


/*------------------------------
  check and get float attribute
  C.Wang 0814, 2012
 -------------------------------*/
BOOL4 TbCheckCommandAttributeF(char *string, char *attribute, float *value, int maxLen)
{
   int i;
   char localString[512], *key, *key2;

   strcpy(localString, string);

   key = strtok(localString, " ");
   while (key != NULL) {
      if (FyCheckWord(key, attribute, TRUE)) {
         if (value != NULL) {
            for (i = 0; i < maxLen; i++) {
               key2 = strtok(NULL, " ");
               if (key2 == NULL) return FALSE;
               value[i] = (float) atof(key2);
            }
         }
         return TRUE;
      }

      // next word
      key = strtok(NULL, " ");
   }

   return FALSE;
}


/*------------------
  set log message
  C.Wang 0814, 2012
 -------------------*/
void TbSetLogMessage(char *log)
{
   strcpy(tbLogMessage, log);
}


/*---------------------------------------------------------
  create a new scene object and register it to scene table
  C.Wang 0814, 2012
 ----------------------------------------------------------*/
TBScene *TbNewScene(SCENEid sID, OBJECTid cRefID, OBJECTid cID, OBJECTid l1ID, OBJECTid l2ID)
{
   int i;

   // new a TB scene
   TBScene *scene = new TBScene();
   scene->ID(sID);

   // assign default cameras & lights
   scene->AssignCamera(cRefID, cID);
   scene->AssignLights(l1ID, l2ID);

   // create the Ui button

   if (tbSceneList == NULL) {
      tbMaxScene = 32;
      tbSceneList = (TBScene **) FyMalloc(sizeof(TBScene *)*tbMaxScene);
      for (i = 0; i < tbMaxScene; i++) {
         tbSceneList[i] = NULL;
      }
      tbNumScene = 0;
   }

   for (i = 0; i < tbNumScene; i++) {
      if (tbSceneList[i] == NULL) {
         break;
      }
   }
   if (i == tbNumScene) {
      if (tbNumScene + 1 > tbMaxScene) {
         tbMaxScene += 32;
         tbSceneList = (TBScene **) FyRealloc(tbSceneList, sizeof(TBScene *)*tbMaxScene);
         for (int j = tbNumScene; j < tbMaxScene; j++) {
            tbSceneList[j] = NULL;
         }
      }
      tbNumScene++;
   }

   tbSceneList[i] = scene;
   return scene;
}


/*-------------------------------
  hotkey callback for debug used
  C.Wang 0122, 2013
 --------------------------------*/
void TbDebugKey(BYTE code, BOOL4 value)
{
   if (code == FY_F10) {
      if (value) {
         int w, h;
         tbWorkSpaceVP.GetSize(&w, &h);

         float aspect = (float)w/(float)h;

         h = 600;
         w = (int)(h*aspect);
         tbWorkSpaceVP.SetSize(w, h);
         tbWorkSpaceVP.SetupHDREffect(w/4, h/4, 1.0f, 0.3f, 3.0f, 0.8f);
      }
   }
   if (code == FY_F9) {
      if (value) {
         tbWorkSpaceVP.SetSize(tbWorkSpaceVPWidth, tbWorkSpaceVPHeight);
         tbWorkSpaceVP.SetupHDREffect(tbWorkSpaceVPWidth/4, tbWorkSpaceVPHeight/4, 1.0f, 0.3f, 3.0f, 0.8f);
      }
   }
}


/*------------------------------------
  hotkey callback for reseting camera
  C.Wang 0120, 2014
 -------------------------------------*/
void TbResetCamera(BYTE code, BOOL4 value)
{
   if (!tbBeEditCam) return;

   if (code == FY_F1) {
      if (value) {
         if (tbBeZUp) {
            tbCameraRef.Translate(0.0f, 0.0f, tbMainCamUp, REPLACE);
            tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
            tbCamera.Rotate(X_AXIS, 75.0f, GLOBAL);
         }
         else {
            tbCameraRef.Translate(0.0f, tbMainCamUp, 0.0f, REPLACE);
            tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
            tbCamera.Rotate(X_AXIS, -25.0f, GLOBAL);
         }

         tbOrthScaleFactor = 1.0f;
         tbOrthoCamera.Translate(0.5f, 0.5f, 300.0f, REPLACE);
         tbOrthoCamera.SetScaleFactor(tbOrthScaleFactor);
      }
   }
   else if (code == FY_F2) {
      if (value) {
         // find the center/extreme of the scene
         float pos[3], radius;
         tbCurScene->FindCenter(pos, &radius);

         if (radius > 0.0f) {
            // pan the camera to scene center
            tbCameraRef.SetPosition(pos, TRUE);

            // zoom the camera
            float fov = tbCamera.GetFov();
            float dist = radius/(float)tan(fov*3.14159/360.0)*1.3f;

            float sss[3];
            tbCamera.GetPosition(sss, FALSE);
            FyNormalizeVector3(sss);
            sss[0] *= dist;
            sss[1] *= dist;
            sss[2] *= dist;
            tbCamera.SetPosition(sss, FALSE);
         }
         else {
            if (tbBeZUp) {
               tbCameraRef.Translate(0.0f, 0.0f, tbMainCamUp, REPLACE);
               tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
               tbCamera.Rotate(X_AXIS, 75.0f, GLOBAL);
            }
            else {
               tbCameraRef.Translate(0.0f, tbMainCamUp, 0.0f, REPLACE);
               tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
               tbCamera.Rotate(X_AXIS, -25.0f, GLOBAL);
            }
         }
      }
   }
}


/*------------------
  load init file
  C.Wang 0815, 2012
 -------------------*/
void TbLoadInitFile()
{
   FILE *fp;

   // open the Fly2 configuration file
   fp = fopen("FyViewer.cfg", "rt");
   if (fp == NULL) return;
   
   int len, w, h, x;
   char s[256], *key, *key1, *key2;

   // read the lines
   while ((len = FyReadNewLine(fp, s)) >= 0) {
      FyTruncateStringSpace(s);
      len = (int) strlen(s);

      if (len > 0 && s[0] != '#') {
         // check the break command
         key = strtok(s, " ");
         if (FyCheckWord(key, "Resolution", TRUE)) {
            key1 = strtok(NULL, " ");
            if (key1 != NULL) {
               if (FyCheckWord(key1, "Full", TRUE)) {
                  FyWin32GetDesktopSize(&tbTempTopWidth, &tbTempTopHeight);
               }
               else {
                  FyWin32GetDesktopSize(&w, &h);

                  x = atoi(key1);
                  if (x < 1024) tbTempTopWidth = 1024;
                  else if (x > w) tbTempTopWidth = w;
                  else tbTempTopWidth = x;

                  key2 = strtok(NULL, " ");
                  if (key2 != NULL) {
                     x = atoi(key2);
                     if (x < 768) tbTempTopHeight = 768;
                     else if (x > h) tbTempTopHeight = h;
                     else tbTempTopHeight = x;
                  }
               }
            }
         }
         else if (FyCheckWord(key1, "Shader", TRUE)) {
            key2 = strtok(NULL, " ");
            FySetShaderPath(key2);
         }
      }
   }

   // close the file
   fclose(fp);
}


/*-----------------------------------------
  resize/re-layout the UI and viewport
  (w, h) is the inside range of the window
  C.Wang 0930, 2014
 ------------------------------------------*/
void TbResizeWindow(int w, int h, BOOL4 beWin)
{
   // reset the world size
   tbWorldWidth = w;
   tbWorldHeight = h;

   // recalculate all UI position and size
   TbViewportLayout(tbWorldWidth, tbWorldHeight);

   // update graphics DDK rendering buffer size
   FyResizeFlyWin32World(w, h, FALSE, beWin);

   // adjust the main viewport
   tbWSBottomVP.SetPosition(tbBottomVPOX, tbBottomVPOY);
   tbWSBottomVP.SetSize(tbBottomVPWidth, tbBottomVPHeight);
   tbWorkSpaceVP.SetPosition(tbWorkSpaceVPOX, tbWorkSpaceVPOY);
   tbWorkSpaceVP.SetSize(tbWorkSpaceVPWidth, tbWorkSpaceVPHeight);

   // adjust mini viewport
   tbMiniPost.SetPosition(tbWorkSpaceVPOX + 10, tbWorkSpaceVPOY + tbWorkSpaceVPHeight - 100);

   // adjust the camera aspect ratio
   tbCamera.SetAspectRatio((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);
   tbAuxCamera.SetAspectRatio((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);
   tbAux2Camera.SetAspectRatio((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);
   tbOrthoCamera.SetAspectRatio((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);

   // setup post-processing
   tbWorkSpaceVP.SetupBloomEffect(tbWorkSpaceVPWidth/4, tbWorkSpaceVPHeight/4, 1.2f, 0.3f);
   tbWorkSpaceVP.SetupHDREffect(tbWorkSpaceVPWidth/4, tbWorkSpaceVPHeight/4, 1.0f, 0.3f, 3.0f, 0.8f);
   tbWorkSpaceVP.SetupSSAOEffect(tbSSAOSampleRange, tbSSAODistance);
   
   // adjust the menu bar UIs
   int ox, oy;
   FnUiPlate uiP;
   tbMenuBarUI.MoveSize(tbMenuBarOX, tbMenuBarOY, tbMenuBarWidth, tbMenuBarHeight);
   uiP.ID(tbMenuBarPlateID);
   uiP.MoveSize(0, 0, tbMenuBarWidth - 1, tbMenuBarHeight - 1);

   ox = tbMenuBarWidth - BUTTON_WIDTH - 5;
   oy = 5;
   uiP.ID(tbExitButtonID);
   uiP.Move(ox, oy);

   ox = tbMenuBarWidth - BUTTON_WIDTH*2 - 5 - 3;
   uiP.ID(tbAuxButtonID);
   uiP.Move(ox, oy);

   ox = tbMenuBarWidth - BUTTON_WIDTH*3 - 5 - 6;
   uiP.ID(tbOrthoButtonID);
   uiP.Move(ox, oy);

   // adjust side menu UIs
   tbSideMenuUI.MoveSize(tbSideMenuOX, tbSideMenuOY, tbSideMenuWidth, tbSideMenuHeight);
   uiP.ID(tbSideMenuPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);
   uiP.ID(tbSideMenuSystemPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);
   uiP.ID(tbSideMenuScriptPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);
   uiP.ID(tbSideMenuModelPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);
   uiP.ID(tbSideMenuCharacterPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);
   uiP.ID(tbSideMenuFXPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);
   uiP.ID(tbSideMenuEnvironmentPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);
   uiP.ID(tbSideMenuPostProcessingPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);
   uiP.ID(tbSideMenuFaceToolPlateID);
   uiP.MoveSize(0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1);

   // adjust the bottom menu UIs
   tbMenuUI.MoveSize(tbMenuOX, tbMenuOY, tbMenuWidth, tbMenuHeight);
   uiP.ID(tbControlPanelPlateUI);
   uiP.MoveSize(0, 0, tbMenuWidth - 1, tbMenuHeight - 1);

   // create the play/stop buttons
   ox = tbMenuWidth - tbSideMenuWidth + 50;
   oy = 10;
   uiP.ID(tbStopID);
   uiP.Move(ox, oy);

   ox += SHORT_BUTTON_WIDTH + 3;
   uiP.ID(tbPlayID);
   uiP.Move(ox, oy);

   oy -= 5;
   ox += SHORT_BUTTON_WIDTH + 3;
   uiP.ID(tbPlaySpeedID);
   uiP.Move(ox, oy);

   ox += 125;
   uiP.ID(tbDebugTypeinID);
   uiP.Move(ox, oy);

   ox = tbMenuWidth - tbSideMenuWidth + 50 + (SHORT_BUTTON_WIDTH + 3)*2 + 10;
   oy = 10 + 30;
   uiP.ID(tbSelectPlayModeID);
   uiP.Move(ox, oy);

   ox -= (180 + 10);
   oy -= 5;
   uiP.ID(tbCurFrameID);
   uiP.Move(ox, oy);

   ox = tbMenuWidth - tbSideMenuWidth + 50 - 200;
   oy = 10;
   uiP.ID(tbSelectCameraID);
   uiP.Move(ox, oy);

   // adjust dialogue UIs
   tbWarnWidth = 250;
   tbWarnHeight = 120;
   tbWarnOX = tbBottomVPOX + (tbBottomVPWidth - tbWarnWidth)/2;
   tbWarnOY = tbBottomVPOY + (tbBottomVPHeight - tbWarnHeight)/2;
   tbWarnUI.MoveSize(tbWarnOX, tbWarnOY, tbWarnWidth, tbWarnHeight);

   tbAddActionW = 400;
   tbAddActionH = 250;
   tbAddActionOX = tbBottomVPOX + (tbBottomVPWidth - tbAddActionW)/2;
   tbAddActionOY = tbBottomVPOY + (tbBottomVPHeight - tbAddActionH)/2;
   tbAddActionUI.MoveSize(tbAddActionOX, tbAddActionOY, tbAddActionW, tbAddActionH);

   tbAddFacialExpressionW = 400;
   tbAddFacialExpressionH = 250;
   tbAddFacialExpressionOX = tbBottomVPOX + (tbBottomVPWidth - tbAddFacialExpressionW)/2;
   tbAddFacialExpressionOY = tbBottomVPOY + (tbBottomVPHeight - tbAddFacialExpressionH)/2;
   tbAddFacialExpressionUI.MoveSize(tbAddFacialExpressionOX, tbAddFacialExpressionOY,
                                    tbAddFacialExpressionW, tbAddFacialExpressionH);

   tbAddGameFXW = 400;
   tbAddGameFXH = 250;
   tbAddGameFXOX = tbBottomVPOX + (tbBottomVPWidth - tbAddGameFXW)/2;
   tbAddGameFXOY = tbBottomVPOY + (tbBottomVPHeight - tbAddGameFXH)/2;
   tbAddGameFXUI.MoveSize(tbAddGameFXOX, tbAddGameFXOY, tbAddGameFXW, tbAddGameFXH);

   tbAddGameFXSystemW = 400;
   tbAddGameFXSystemH = 250;
   tbAddGameFXSystemOX = tbBottomVPOX + (tbBottomVPWidth - tbAddGameFXSystemW)/2;
   tbAddGameFXSystemOY = tbBottomVPOY + (tbBottomVPHeight - tbAddGameFXSystemH)/2;
   tbAddGameFXSystemUI.MoveSize(tbAddGameFXSystemOX, tbAddGameFXSystemOY, tbAddGameFXSystemW, tbAddGameFXSystemH);

   tbGameFXKeyframeW = 400;
   tbGameFXKeyframeH = 250;
   tbGameFXKeyframeOX = tbBottomVPOX + tbBottomVPWidth - tbGameFXKeyframeW;
   tbGameFXKeyframeOY = tbBottomVPOY;
   tbGameFXKeyframeUI.MoveSize(tbGameFXKeyframeOX, tbGameFXKeyframeOY, tbGameFXKeyframeW, tbGameFXKeyframeH);
}


/*---------------------------
  drop a file to the program
  C.Wang 0821, 2012
 ----------------------------*/
void TbDropAFile(char *fullFileName)
{
   // check file format
   char path[256], name[256], ext[64];
   int format = FyCheckFileFormat(fullFileName, path, name, ext);

   if (format == CWA || format == CWC || format == CWK) {
      TbCommandParser("Character", fullFileName, FALSE);
   }
   else if (format == CW3 || format == CW4 || format == CWN || format == WOBJ) {
      TbCommandParser("Model", fullFileName, FALSE);
   }
   else {
      TbCommandParser("Load", fullFileName, FALSE);
   }
}


/*----------------------------------------------------
  create a mini scene and viewport for axis rendering
  C.Wang 0821, 2012
 -----------------------------------------------------*/
void TbCreateMiniScene()
{
   // create mini port
   tbMiniPortID = FyCreateViewport(tbWorkSpaceVPOX + 10, tbWorkSpaceVPOY + tbWorkSpaceVPHeight - 60, 50, 50);
   tbMiniPost.ID(tbMiniPortID);
   tbMiniPost.SetBackgroundColor(1.0f, 0.0f, 0.0f);

   // create mini scene and objects
   float v[6], color[3];
   MATERIALid mIDR, mIDG, mIDB;
   tbMiniSceneID = FyCreateScene(1);
   FnScene scene(tbMiniSceneID);

   tbMiniAxisID = scene.CreateObject(OBJECT);
   FnObject object(tbMiniAxisID);

   v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f;
   v[3] = 10.0f; v[4] = 0.0f; v[5] = 0.0f;
   color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f;
   mIDR = FyCreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mIDR, v, 2, TRUE);

   v[3] = 0.0f; v[4] = 10.0f; v[5] = 0.0f;
   color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f;
   mIDG = FyCreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mIDG, v, 2, TRUE);

   v[3] = 0.0f; v[4] = 0.0f; v[5] = 10.0f;
   color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f;
   mIDB = FyCreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mIDB, v, 2, TRUE);

   tbMiniCamID = scene.CreateObject(CAMERA);
   FnCamera camera(tbMiniCamID);
   camera.ID(tbMiniCamID);
   camera.SetAspectRatio(1.0f);
   camera.SetNearPlane(3.0f);
   camera.SetFarPlane(1000.0f);

   camera.Translate(0.0f, 0.0f, tbMiniCamDist, LOCAL);
   if (tbBeZUp) {
      camera.Rotate(X_AXIS, 75.0f, GLOBAL);
   }
   else {
      camera.Rotate(X_AXIS, -25.0f, GLOBAL);
   }
}


/*------------------
  the main program
  C.Wang 1121, 2014
 -------------------*/
void FyMain(int argc, char *argv[])
{
   int i;

   // echo the execution file to Fly2Tmp folder
   FILE *fp = fopen("\\Fly2Tmp\\Fly2ViewerLocation.cfg", "wt");
   if (fp == NULL) {
      CreateDirectory("\\Fly2Tmp", NULL);
      Sleep(1);
      fp = fopen("\\Fly2Tmp\\Fly2ViewerLocation.cfg", "wt");
   }
   else {
      fclose(fp);
      //DeleteFile("\\Fly2Tmp\\Fly2ViewerLocation.cfg");
      fp = fopen("\\Fly2Tmp\\Fly2ViewerLocation.cfg", "wt");
   }

   int len;
   char tCmd[256], *key, cmd[256];
   strcpy(tCmd, argv[0]);
   key = strtok(tCmd, "\"");

   strcpy(cmd, key);
   len = (int) strlen(cmd);
   if (!FyCheckWord(cmd + len - 3, "exe")) {
      for (i = len; i > 0; i--) {
         if (cmd[i] == ' ') {
            cmd[i] = '\0';
            break;
         }
      }
      fprintf(fp, "%s", cmd);
   }
   else {
      fprintf(fp, "%s", key);
   }
   fclose(fp);

   // initialize Fly2 and create the main world
   FyWin32GetDesktopSize(&tbTempTopWidth, &tbTempTopHeight);

   // initialize the script system
   TbInitScriptSystem();

   // load initial file
   TbLoadInitFile();

   //tbTempTopWidth = 1400;
   //tbTempTopHeight = 1000;

   strcpy(tbTmpShaderPath, ".");
   tbWorldWidth = tbTempTopWidth - tbWindowFrameInWidth;
   tbWorldHeight = tbTempTopHeight - tbWindowFrameInHeight;

   char caption[256];
#ifdef X64
#ifdef Q3D
   sprintf(caption, "%s %03d (64-bit Q3D version)", tbCaption, tbBuildNumber);
#else
   sprintf(caption, "%s %03d (64-bit version)", tbCaption, tbBuildNumber);
#endif
#else
#ifdef Q3D
   sprintf(caption, "%s %03d (32-bit Q3D version)", tbCaption, tbBuildNumber);
#else
   sprintf(caption, "%s %03d (32-bit version)", tbCaption, tbBuildNumber);
#endif
#endif

   BOOL4 beReady = FyStartFlyWin32(caption, 0, 0, tbWorldWidth, tbWorldHeight, FALSE,
                                   TRUE, tbAALevel, 0, NULL, TRUE);
   if (!beReady) {
      // failed to create the world, just quit the program
      MessageBox(NULL, "Failed to initialize Fly Toolkit", "Error!", MB_OK);
      return;
   }

   // setup the basic working environment
   FySetCharacterPath("Data\\Characters");
   FySetTexturePath("Data\\Textures");
   FySetModelPath("Data\\Models");
   FySetGameFXPath("Data\\GameFXs");
   FySetShaderPath("Data\\Shaders");
   FySetDataPath(".");
   //FySetSystemShaderPath("System\\Shaders", TRUE);

   // load cursors
   tbArrowCursor = LoadCursor(NULL, IDC_ARROW);
   tbWaitCursor = LoadCursor(NULL, IDC_WAIT);

   // calculate the system layout
   TbViewportLayout(tbWorldWidth, tbWorldHeight);

   // create the UIs
   tbNumUISys = 0;
   for (i = 0; i < MAX_UI_SYSTEMS; i++) {
      tbAllUISysID[i] = FAILED_ID;
   }
   TbCreateMenuBarUI();
   TbCreateMenuUI();
   TbCreateSideMenuUI();
   TbCreateDialogues();

   // create the viewports for workspace
   tbWorkSpaceBTID = FyCreateViewport(tbBottomVPOX, tbBottomVPOY, tbBottomVPWidth, tbBottomVPHeight);
   tbWSBottomVP.ID(tbWorkSpaceBTID);
   tbWSBottomVP.SetBackgroundColor(0.5f, 0.5f, 0.5f);

   tbWorkSpaceID = FyCreateViewport(tbWorkSpaceVPOX, tbWorkSpaceVPOY, tbWorkSpaceVPWidth, tbWorkSpaceVPHeight);
   tbWorkSpaceVP.ID(tbWorkSpaceID);
   
   tbWorkSpaceVP.SetBackgroundColor(0.2f, 0.2f, 0.2f);
   tbWorkSpaceVP.SetupBloomEffect(tbWorkSpaceVPWidth/4, tbWorkSpaceVPHeight/4, 1.2f, 0.3f);
   tbWorkSpaceVP.SetupHDREffect(tbWorkSpaceVPWidth/4, tbWorkSpaceVPHeight/4, 1.0f, 0.3f, 3.0f, 0.8f);
   tbWorkSpaceVP.SetupDOFEffect(300.0f, 150.0f, 200.0f);
   tbWorkSpaceVP.SetDOFFocus(300.0f);

   tbWorkSpaceVP.SetupSSAOEffect(tbSSAOSampleRange, tbSSAODistance);

   // create the main scene
   tbMainSceneID = FyCreateScene(tbRenderGroup);
   tbScene.ID(tbMainSceneID);
   tbScene.SetAmbientLights(0.84f, 0.84f, 0.84f, 0.44f, 0.44f, 0.44f);

   // create the auxiliary scenes
   tbAuxSceneID = FyCreateScene(2);
   tbAuxScene.ID(tbAuxSceneID);
   tbAux2SceneID = FyCreateScene(2);
   tbAux2Scene.ID(tbAux2SceneID);

   // create the scene for texture space
   tbOrthoSceneID = FyCreateScene(2);
   tbOrthoScene.ID(tbOrthoSceneID);
   tbOrthoScene.SetAmbientLights(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

   // create the scene for texture filter shaders
   tbSquareSceneID = FyCreateScene(1);

   // create the main camera for 3D
   tbMainCamRefID = tbScene.CreateObject();
   tbCameraRef.ID(tbMainCamRefID);
   tbMainCamID = tbScene.CreateObject(CAMERA);
   tbCamera.ID(tbMainCamID);
   tbCamera.SetParent(tbMainCamRefID);
   tbCamera.SetAspectRatio((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);
   tbCamera.SetNearPlane(3.0f);
   tbCamera.SetFarPlane(10000000.0f);
   tbCamera.SetName("MainCam");

   tbExclusiveSaveList[tbNumExclusiveSave] = tbMainCamRefID;
   tbNumExclusiveSave++;
   tbExclusiveSaveList[tbNumExclusiveSave] = tbMainCamID;
   tbNumExclusiveSave++;

   if (tbBeZUp) {
      tbCameraRef.Translate(0.0f, 0.0f, tbMainCamUp, REPLACE);
      tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
      tbCamera.Rotate(X_AXIS, 75.0f, GLOBAL);
   }
   else {
      tbCameraRef.Translate(0.0f, tbMainCamUp, 0.0f, REPLACE);
      tbCamera.Translate(0.0f, 0.0f, tbMainCamDist, REPLACE);
      tbCamera.Rotate(X_AXIS, -25.0f, GLOBAL);
   }
   tbBeEditCam = TRUE;
   tbRenderCamID = tbMainCamID;

   // create the main light source
   tbMainLightID = tbScene.CreateObject(LIGHT);
   tbMainLight.ID(tbMainLightID);
   tbMainLight.SetLightType(PARALLEL_LIGHT);
   tbMainLight.Rotate(X_AXIS, 60.0f, LOCAL);
   tbMainLight.Rotate(Z_AXIS, -120.0f, LOCAL);
   tbMainLight.Translate(0.0, 300.0f, 0.0f, LOCAL);
   tbMainLight.SetColor(1.0f, 1.0f, 1.0f);
   tbMainLight.SetName("MainLight");
   tbMainLight.SetIntensity(1.0f);
   tbMainLight.SetRange(5000.0f);

   tbExclusiveSaveList[tbNumExclusiveSave] = tbMainLightID;
   tbNumExclusiveSave++;

   float fDir[3];
   fDir[0] = -40.0f; fDir[1] = 40.0f; fDir[2] = -100.0f;
   FyNormalizeVector3(fDir);
   tbMainLight.SetDirection(fDir, NULL, TRUE);

   // create the auxiliary camera
   tbAuxCamID = tbAuxScene.CreateObject(CAMERA);
   tbAuxCamera.ID(tbAuxCamID);
   tbAuxCamera.SetAspectRatio((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);
   tbAuxCamera.SetNearPlane(3.0f);
   tbAuxCamera.SetFarPlane(100000.0f);

   tbAux2CamID = tbAux2Scene.CreateObject(CAMERA);
   tbAux2Camera.ID(tbAux2CamID);
   tbAux2Camera.SetAspectRatio((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);
   tbAux2Camera.SetNearPlane(3.0f);
   tbAux2Camera.SetFarPlane(100000.0f);

   // create the ortho camera
   tbOrthScaleFactor = 1.0f;
   tbOrthoCamID = tbOrthoScene.CreateObject(CAMERA);
   tbOrthoCamera.ID(tbOrthoCamID);
   tbOrthoCamera.Translate(0.0f, 0.0f, 300.0f, REPLACE);
   tbOrthoCamera.SetProjectionType(ORTHOGONAL);
   tbOrthoCamera.SetScaleFactor(tbOrthScaleFactor);
   tbOrthoCamera.SetAspectRatio((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight*tbTexAspect);
   tbOrthoCamera.Translate(0.5f, 0.5f, 0.0f, GLOBAL);

   // create the axis
   TbCreateAuxObjects();

   // create a new scene object
   tbCurScene = TbNewScene(tbMainSceneID, tbMainCamRefID, tbMainCamID, tbMainLightID, tbAuxLightID);

   // create a small scene
   TbCreateMiniScene();

   // create fonts
   tbSmallEFontID = FyCreateText("Trebuchet MS", 18, FALSE, FALSE);
   tbSmallEFont.ID(tbSmallEFontID);
   tbTinyEFontID = FyCreateText("Trebuchet MS", 16, FALSE, FALSE);
   tbTinyEFont.ID(tbTinyEFontID);

   // create a timer for 30fps
   FyBindTimer(LOOP_TIMER, 30.0f, TbMainLoop, TRUE);
   FyBindTimer(RENDER_TIMER, 30.0f, TbRenderIt, TRUE);
   FyBindTimer(BLINK_TIMER, 2.0f, TbBlinkControl, TRUE);

   // bind mouse callbacks
   FyBindMouseFunction(LEFT_MOUSE, TbLeftMousePress, TbLeftMouseDrag, TbLeftMouseRelease, TbLeftMouseDoubleClick);
   FyBindMouseFunction(RIGHT_MOUSE, TbRightMousePress, TbRightMouseDrag, TbRightMouseRelease, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, TbMiddleMousePress, TbMiddleMouseDrag, TbMiddleMouseRelease, NULL);
   FyBackupMouseMoveFunctionForUI(TbMouseMove);

   // bind quit function
   FyBindQuitFunction(TbQuitFun);

   // bind window resize callback
   FyBindWindowResizeFunction(TbResizeWindow);

   // bind the drop file callback
   FyBindDropFileFunction(TbDropAFile);

   // bind keyboard input function
   if (tbShowCommand) {
      FyBindInputFunction(TbCommandInput);
      FyDefineHotKey(FY_LEFT, TbCommandFunKey, TRUE);
      FyDefineHotKey(FY_UP, TbCommandFunKey);
      FyDefineHotKey(FY_RIGHT, TbCommandFunKey, TRUE);
      FyDefineHotKey(FY_DOWN, TbCommandFunKey);
   }
   FyDefineHotKey(FY_F1, TbResetCamera, FALSE);
   FyDefineHotKey(FY_F2, TbResetCamera, FALSE);
   FyDefineHotKey(FY_SHIFT, TbShowGizmo, TRUE);
   FyDefineHotKey(FY_ESC, TbCancelGizmoProcess, FALSE);

   // hotkeys for debug
   FyDefineHotKey(FY_F9, TbDebugKey, FALSE);
   FyDefineHotKey(FY_F10, TbDebugKey, FALSE);

   // init command queue max 200 commands
   FcSetupCommandQueue(tbNumCommandSlots);

   // create command history buffer
   tbCommandHistory = (char **) FyMalloc(sizeof(char *)*tbNumCommandHistory);
   for (i = 0; i < tbNumCommandHistory; i++) {
      tbCommandHistory[i] = (char *) FyMalloc(MAX_STRING_CHARACTERS);
      *tbCommandHistory[i] = '\0';
   }
   tbCurCommandPtr = tbCommandHistory[0];
   tbCurCommand = 0;

   // initialize the job system to maximum 16 jobs concurrent
   FjUseJobSystem(16);

   // open system menu
   FnUiRadioButton uiRB;
   uiRB.ID(tbSystemButtonID);
   uiRB.ChangeState(TRUE);
   TbSwitchMenuBarRadios(tbSystemButtonID, TRUE);

   // echo the welcome message
   // load a default script
#ifdef ZEROPLUS
   TbSetLogMessage("Welcome to ZeroFly2");
   TbCommandParser("Load", "ZeroFly2", FALSE);
#elif Q3D
   TbSetLogMessage("Welcome to Fly2 Toolkit v 1.0 in Q3D Environment");
   TbCommandParser("Load", "Q3DViewer", FALSE);
#elif NMA
   TbSetLogMessage("Welcome to Fly2 Toolkit v 1.0 in NMA Environment");
   TbCommandParser("Load", "NMA", FALSE);
#elif DCI9
   TbSetLogMessage("Welcome to Fly2 Toolkit v 1.0 in DCI 9th Environment");
   TbCommandParser("Load", "DCI_9th", FALSE);
#else
   TbSetLogMessage("Welcome to Fly2 Toolkit v 1.0");
#endif

   // check argv[1] for a file to import immediately
   if (argc > 1) {
      // backup working directories
      char modelPath[256], texturePath[256], actorPath[256], fileName[256];
      char path[256], name[256], ext[256];
      
      strcpy(modelPath, FyGetModelPath());
      strcpy(texturePath, FyGetTexturePath());
      strcpy(actorPath, FyGetCharacterPath());

      // get/reset file name and working directories
      strcpy(fileName, argv[1]);
      FyCheckFileFormat(fileName, path, name, ext);
      FySetModelPath(path);
      FySetTexturePath(path);

      // drop the file to toolkit
      TbDropAFile(fileName);

      // restore the paths
      FySetModelPath(modelPath);
      FySetTexturePath(texturePath);
      FySetCharacterPath(actorPath);
   }

   // initialize Face Tool
   tbFaceModelName[0] = '\0';

   // initialize the ambient occlusion renderer
#ifdef FLY2_RENDER
   tbAORenderID = FrCreateRenderEngine(AMBIENT_OCCLUSION_VERTEX);
#endif

   // invoke the Fly
   FyInvokeFly(TRUE);
}