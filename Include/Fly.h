/*==============================================================
  Fly SDK Version 2 - Header  Build 1016 (Since 06/29, 2015)

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0327, 2012 by Chuan-Chang Wang

  Last Updated : 0625, 2015
 ===============================================================*/
#if !defined(_FLY2_H_INCLUDED_)
#define _FLY2_H_INCLUDED_

#ifdef WIN32
   #define WIN32_LEAN_AND_MEAN
   #pragma inline_depth(255)
   #pragma inline_recursion(on)

   #include <windows.h>
   #include <malloc.h>
#elif Sce
#elif LINUX_XWINDOW
    #include <X11/X.h>       // X11 constant (e.g. TrueColor)
    #include <X11/keysym.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/XKBlib.h>
#endif

#ifndef WIN32
typedef void *LPVOID;
typedef void (*LPTHREAD_START_ROUTINE)(LPVOID data);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// global constants
#ifndef NONE
   #define NONE               -1
#endif
#define FY_NONE               -1
#define ROOT                   0
#define FAILED_ID              0
#define FAILED_MATERIAL_ID     0
#define BACK_BUFFER            0


// 3D object types
#define NOTHING                0
#ifndef OBJECT
   #define OBJECT              1
#endif
#define MODEL                  1
#define CAMERA                 2
#define LIGHT                  3
#define TERRAIN                4    // reserved for backward compatibility to TheFly version 1
#define AUDIO                  5
#define FORCE                  6
#define SPRITE                 7
#define CHARACTER              100


// matrix operators
#define REPLACE                0
#define LOCAL                  1
#define GLOBAL                 2
#define AS_LOCAL               1
#define AS_GLOBAL              2


// axis IDs
#define X_AXIS                 0
#define Y_AXIS                 1
#define Z_AXIS                 2
#define MINUS_X_AXIS           3
#define MINUS_Y_AXIS           4
#define MINUS_Z_AXIS           5


// geometry element types
#define INDEXED_TRIANGLES      1
#define BILLBOARD              2
#define LINES                  3
#define NURBS_CURVE            4
#define NURBS_SURFACE          5
#define BILLBOARD_CLOUD        6
#define LABEL                  8


// line types
#define LINE_SEGMENTS          1
#define OPEN_POLYLINE          2
#define CLOSE_POLYLINE         3


// vertex types
#define XYZ                    0x00000100
#define XYZ_NORM               0x00000200
#define XYZ_RGB                0x00000300
#define XYZW_RGB               0x00000400         // no more supported in Fly2
#define XYZ_DIFFUSE            0x00000500         // no more supported in Fly2
#define XYZW_DIFFUSE           0x00000600         // no more supported in Fly2
#define XYZ_BBCLOUD            0x00000700


// room types
#define SIMPLE_ROOM            0x0001
#define CLONE_ROOM             0x0002
#define BSP_ROOM               0x0003


// camera projection types
#define PERSPECTIVE            1
#define ORTHOGONAL             2


// lighting types
#define POINT_LIGHT            1
#define PARALLEL_LIGHT         2
#define SPOT_LIGHT             3


// render modes for objects
#define SOLID                  0
#define WIREFRAME              2
#define POINT_CLOUD            3


// movement results for objects and characters
#define WALK                   0
#define BOUNDARY              -1
#define BLOCK                 -2
#define DO_NOTHING            -3


// texture effects for fixed function rendering pipeline
#define UNKNOWN                0x00
#define COLOR_MAP              0x01
#define LIGHT_MAP              0x02
#define SPECULAR_MAP           0x03
#define ENVIRONMENT_MAP        0x04   // not supported
#define BUMP_MAP               0x05   // not supported
#define MIX_MAP                0x06
#define CUSTOM_MAP             0x07


// texture filter types
#define FILTER_NONE            0
#define FILTER_POINT           1
#define FILTER_LINEAR          2 

#define FILTER_ANISOTROPIC     3
#define FILTER_FLAT_CUBIC      4
#define FILTER_GAUSSIAN_CUBIC  5


// texture address modes
#define WRAP_TEXTURE           0
#define MIRROR_TEXTURE         1
#define CLAMP_TEXTURE          2
#define BORDER_TEXTURE         3
#define MIRROR_ONCE_TEXTURE    4


// texture storage type (pool type)
#define VIDEO_MEMORY               1
#define MANAGED_MEMORY             2
#define SYSTEM_MEMORY              3

// texture pixel formats
#define TEXTURE_UNKNOWN            0
#define TEXTURE_32                 1   // true color : A8R8G8B8
#define TEXTURE_16                 2   // hi-color
#define TEXTURE_COMPRESSED_1       3   // DXT1
#define TEXTURE_COMPRESSED_3       4   // DXT3
#define TEXTURE_L16I               5   // Luminance 16 bit integer
#define TEXTURE_1F32               6   // R channel in 32bit float
#define TEXTURE_FP16               7   // R16B16G16A16 RGBA in 16bit floats
#define TEXTURE_2F16               8   // G & R channels in 16bit float each
#define TEXTURE_64                 9   // R16B16G16A16 RGBA in 16bit unsigned int
#define TEXTURE_32A                10  // true color : A8R8G8B8
#define TEXTURE_2F32               12  // G & R channels in 32bit float each
#define TEXTURE_COMPRESSED_5       13  // DXT5
#define TEXTURE_24                 14  // 24-bit RGB
#define TEXTURE_FP32               255 // R32B32G32A32 RGBA in 32bit float each


// rendering options for objects
#define Z_BUFFER               1
#define Z_BUFFER_WRITE         2
#define FOG                    4       // no more used in Fly2
#define SPECULAR               5       // no more used in Fly2
#define LIGHTING               6       // no more used in Fly2
#define SOURCE_BLEND_MODE      8
#define DESTINATION_BLEND_MODE 9
#define VIEWING_CHECK          12
#define CULLING_MODE           13
#define AMBIENT_OCCLUSION      14
#define Z_REVERSE              15


// culling mode
#define CULL_NONE              1
#define CULL_CW                2
#define CULL_CCW               3


// alpha blending modes
#define BLEND_ZERO             1
#define BLEND_ONE              2
#define BLEND_SRC_COLOR        3
#define BLEND_INV_SRC_COLOR    4
#define BLEND_SRC_ALPHA        5
#define BLEND_INV_SRC_ALPHA    6
#define BLEND_DEST_ALPHA       7
#define BLEND_INV_DEST_ALPHA   8
#define BLEND_DEST_COLOR       9
#define BLEND_INV_DEST_COLOR   10
#define BLEND_SRC_ALPHA_SAT    11


// motion data types
#define MOTION_UNKNOWN             0x00000000
#define MOTION_POSITION            0x00000001
#define MOTION_ROTATION            0x00000002
#define MOTION_QUATERNION          0x00000003
#define MOTION_SCALE               0x00000004
#define MOTION_SPEED               0x00000005
#define MOTION_ROTATION_YXZ        0x00000006


// action types
#define SIMPLE_ACTION              1
#define CROSS_BLEND_ACTION         2
#define FULL_BLEND_ACTION          3
#define CONNECT_ACTION             4


// particle emitter type
#define POINT_SPHERE               1
#define ON_VERTEX                  2
//#define ON_FACE                  3


// particle type
#define PLANE_BILLBOARD_CLOUD      1
#define SPHERICAL_BILLBOARD_CLOUD  2


// particle blending type
#define ALPHA_BLENDING             1
#define ADD_COLOR                  2
#define SUBTRACT_COLOR             3


// return values for motion playing
#define FAILED_PLAYING            -1
#define PLAYING_TO_THE_END        -2
#define NO_PLAYING                -3
#define IN_BETWEEN                -4


// playing methods
#define RESET                      1
#define START                      1
#define ONCE                       2
#define LOOP                       3
#define PAUSE                      4


// bone types
#define ANIMATION_BONE             0
#define DYNAMICS_BONE              1
#define IK_BONE                    2
#define PROCEDURAL_BONE            3


// billboard types
#define INDEPENDENT                0x00000000
#define DEPENDENT                  0x01000000


// force types
#define GRAVITY                    1
#define PARALLEL                   2
#define SINK                       3
#define SPHERE                     4
#define VORTEX                     5
#define VORTEX_2D                  6
#define VISCOSITY                  7
#define USER_DEFINED               8


// texture file format
#define FILE_BMP                   0
#define FILE_JPG                   1
#define FILE_PNG                   3
#define FILE_DDS                   4


// depth map type
#define UNIT_DEPTH_MAP             1
#define DEPTH_MAP                  2
#define DISTANCE_MAP               3


// game FX types
#define ERROR_FX                   0
#define BASE_FX                    1
#define PLATE_FX                   2
#define PARTICLES_FX               3
#define AUDIO_FX                   4
#define SWORD_FX                   5
#define FORCE_FX                   6
#define GEOMETRY_FX                7


// game FX lifes
#define DEAD_LIFE                  -1.0


// plate FX types
//#define BILLBOARD                2    // reserved
#define PLATE_ZUP                  3
#define PLATE_YUP                  4


// particle generation mode
#define RANDOM_EMITTER             0
#define EACH_EMITTER               1


// interpolation type for key framing
#define LINEAR_INTERPOLATION       1
#define SPLINE_INTERPOLATION       2
#define SLERP                      3
#define STEP_INTERPOLATION         4


// type of key framing
#define BY_VALUE                   1
#define BY_RATE                    2


// topology sorting flag
#define NO_SORTING                 0
#define BACK_TO_FRONT              1
#define FRONT_TO_BACK              2
#define DO_BACKFACE_REMOVE         0x10000000


// fog modes
#define LINEAR_FOG                 1
#define EXP_FOG                    2
#define EXP2_FOG                   3


// text alignment
#define CENTERED                          1
#define LEFT_TOP                          2
#define RIGHT_TOP                         3
#define CENTERED_TOP                      6
#define LEFT_CENTERED                     4
#define RIGHT_CENTERED                    5
#define LEFT_BOTTOM                       7
#define RIGHT_BOTTOM                      8
#define CENTERED_BOTTOM                   9


// file format
#define UNKNOWN_FORMAT               0x00 // unknown file type
#define MS_BITMAP                    0x01 // standard MS bitmap (no compression)
#define JPEG                         0x02 // JPEG image
#define TARGA                        0x03 // Targa image (*.tga)
#define DDS                          0x04 // DDS image (*.dds)
#define PNG                          0x05 // PNG image (*.png)
#define CW3                          0x0E // Chuan-Chang Wang's geometry data file (Fly)
#define CWC                          0x0F // Chuan-Chang Wang's old character file (Fly)
#define CWN                          0x10 // Chuan-Chang Wang's scene file (Fly)
#define CWK                          0x11 // Chuan-Chang Wang's skeleton file (Fly)
#define CWM                          0x12 // Chuan-Chang Wang's motion file (Fly)
#define CWS                          0x13 // Chuan-Chang Wang's shder description file (Fly)
#define CW4                          0x14 // Chuan-Chang Wang's universal data file in binary format (Fly)
#define MSX                          0x15 // Microsoft .x model file
#define CFX                          0x16 // Fly FX file format
#define WOBJ                         0x17 // Wavefront .obj model file
#define CWA                          0x19 // Chuan-Chang Wang's ASCII character file (Fly)
#define CWF                          0x1A // Chuan-Chang Wang's 2nd generation ASCII game FX file

// types of baking vertex animation
#define VA_FROM_ACTION               0
#define VA_FROM_POSE                 1

// hotkey codes
#ifdef LINUX_XWINDOW
#define FY_LBUTTON             0x01
#define FY_MBUTTON            0x02
#define FY_RBUTTON             0x03
#define FY_CANCEL              0x04
#define FY_BACK                XK_BackSpace
#define FY_TAB                   XK_Tab
#define FY_CLEAR              XK_Clear
#define FY_RETURN          XK_Return
#define FY_SHIFT_L           XK_Shift_L
#define FY_SHIFT_R           XK_Shift_R
#define FY_CTRL_L             XK_Control_L
#define FY_CTRL_R             XK_Control_R
#define FY_ALT_L                XK_Alt_L
#define FY_ALT_R                XK_Alt_R
#define FY_PAUSE               XK_Pause
#define FY_CAPITAL           XK_Caps_Lock
#define FY_ESCAPE             XK_Escape
#define FY_ESC                     XK_Escape
#define FY_SPACE               XK_space
#define FY_PRIOR               XK_Prior
#define FY_NEXT                 XK_Next
#define FY_END                   XK_End
#define FY_HOME               XK_Home
#define FY_LEFT                  XK_Left
#define FY_UP                     XK_Up
#define FY_RIGHT              XK_Right
#define FY_DOWN             XK_Down
#define FY_SELECT            XK_Select
#define FY_PRINT              XK_Print
#define FY_EXECUTE         XK_Execute
#define FY_SNAPSHOT            0x2C
#define FY_INSERT            XK_Insert
#define FY_DELETE            XK_Delete
#define FY_HELP                XK_Help
#define FY_0                   XK_0
#define FY_1                   XK_1
#define FY_2                   XK_2
#define FY_3                   XK_3
#define FY_4                   XK_4
#define FY_5                   XK_5
#define FY_6                   XK_6
#define FY_7                   XK_7
#define FY_8                   XK_8
#define FY_9                   XK_9
#define FY_A                   XK_a
#define FY_B                   XK_b
#define FY_C                   XK_c
#define FY_D                   XK_d
#define FY_E                   XK_e
#define FY_F                   XK_f
#define FY_G                   XK_g
#define FY_H                   XK_h
#define FY_I                   XK_i
#define FY_J                   XK_j
#define FY_K                   XK_k
#define FY_L                   XK_l
#define FY_M                   XK_m
#define FY_N                   XK_n
#define FY_O                   XK_o
#define FY_P                   XK_p
#define FY_Q                   XK_q
#define FY_R                   XK_r
#define FY_S                   XK_s
#define FY_T                   XK_t
#define FY_U                   XK_u
#define FY_V                   XK_v
#define FY_W                  XK_w
#define FY_X                   XK_x
#define FY_Y                   XK_y
#define FY_Z                   XK_z
#define FY_NUMPAD0             XK_KP_0
#define FY_NUMPAD1             XK_KP_1
#define FY_NUMPAD2             XK_KP_2
#define FY_NUMPAD3             XK_KP_3
#define FY_NUMPAD4             XK_KP_4
#define FY_NUMPAD5             XK_KP_5
#define FY_NUMPAD6             XK_KP_6
#define FY_NUMPAD7             XK_KP_7
#define FY_NUMPAD8             XK_KP_8
#define FY_NUMPAD9             XK_KP_9
#define FY_NUMLOCK             XK_Num_Lock
#define FY_MULTIPLY              XK_KP_Multiply
#define FY_ADD                          XK_KP_Add
#define FY_SEPARATOR           XK_KP_Separator
#define FY_SUBTRACT              XK_KP_Subtract
#define FY_DECIMAL                XK_KP_Decimal
#define FY_DIVIDE                    XK_KP_Divide
#define FY_F1                  XK_F1
#define FY_F2                  XK_F2
#define FY_F3                  XK_F3
#define FY_F4                  XK_F4
#define FY_F5                  XK_F5
#define FY_F6                  XK_F6
#define FY_F7                  XK_F7
#define FY_F8                  XK_F8
#define FY_F9                  XK_F9
#define FY_F10                XK_F10
#define FY_F11                XK_F11
#define FY_F12                XK_F12

#else
#define FY_LBUTTON             0x01
#define FY_RBUTTON             0x02
#define FY_CANCEL              0x03
#define FY_MBUTTON             0x04
#define FY_BACK                0x08
#define FY_TAB                 0x09
#define FY_CLEAR               0x0C
#define FY_RETURN              0x0D
#define FY_SHIFT               0x10
#define FY_CTRL                0x11
#define FY_ALT                 0x12
#define FY_PAUSE               0x13
#define FY_CAPITAL             0x14
#define FY_ESCAPE              0x1B
#define FY_ESC                 0x1B
#define FY_SPACE               0x20
#define FY_PRIOR               0x21
#define FY_NEXT                0x22
#define FY_END                 0x23
#define FY_HOME                0x24
#define FY_LEFT                0x25
#define FY_UP                  0x26
#define FY_RIGHT               0x27
#define FY_DOWN                0x28
#define FY_SELECT              0x29
#define FY_PRINT               0x2A
#define FY_EXECUTE             0x2B
#define FY_SNAPSHOT            0x2C
#define FY_INSERT              0x2D
#define FY_DELETE              0x2E
#define FY_HELP                0x2F
#define FY_0                   0x30
#define FY_1                   0x31
#define FY_2                   0x32
#define FY_3                   0x33
#define FY_4                   0x34
#define FY_5                   0x35
#define FY_6                   0x36
#define FY_7                   0x37
#define FY_8                   0x38
#define FY_9                   0x39
#define FY_A                   0x41
#define FY_B                   0x42
#define FY_C                   0x43
#define FY_D                   0x44
#define FY_E                   0x45
#define FY_F                   0x46
#define FY_G                   0x47
#define FY_H                   0x48
#define FY_I                   0x49
#define FY_J                   0x4A
#define FY_K                   0x4B
#define FY_L                   0x4C
#define FY_M                   0x4D
#define FY_N                   0x4E
#define FY_O                   0x4F
#define FY_P                   0x50
#define FY_Q                   0x51
#define FY_R                   0x52
#define FY_S                   0x53
#define FY_T                   0x54
#define FY_U                   0x55
#define FY_V                   0x56
#define FY_W                   0x57
#define FY_X                   0x58
#define FY_Y                   0x59
#define FY_Z                   0x5A
#define FY_LWIN                0x5B
#define FY_RWIN                0x5C
#define FY_APPS                0x5D
#define FY_NUMPAD0             0x60
#define FY_NUMPAD1             0x61
#define FY_NUMPAD2             0x62
#define FY_NUMPAD3             0x63
#define FY_NUMPAD4             0x64
#define FY_NUMPAD5             0x65
#define FY_NUMPAD6             0x66
#define FY_NUMPAD7             0x67
#define FY_NUMPAD8             0x68
#define FY_NUMPAD9             0x69
#define FY_MULTIPLY            0x6A
#define FY_ADD                 0x6B
#define FY_SEPARATOR           0x6C
#define FY_SUBTRACT            0x6D
#define FY_DECIMAL             0x6E
#define FY_DIVIDE              0x6F
#define FY_F1                  0x70
#define FY_F2                  0x71
#define FY_F3                  0x72
#define FY_F4                  0x73
#define FY_F5                  0x74
#define FY_F6                  0x75
#define FY_F7                  0x76
#define FY_F8                  0x77
#define FY_F9                  0x78
#define FY_F10                 0x79
#define FY_F11                 0x7A
#define FY_F12                 0x7B
#define FY_F13                 0x7C
#define FY_F14                 0x7D
#define FY_F15                 0x7E
#define FY_F16                 0x7F
#define FY_F17                 0x80
#define FY_F18                 0x81
#define FY_F19                 0x82
#define FY_F20                 0x83
#define FY_F21                 0x84
#define FY_F22                 0x85
#define FY_F23                 0x86
#define FY_F24                 0x87
#define FY_NUMLOCK             0x90
#define FY_SCROLL              0x91
#define FY_LSHIFT              0xA0
#define FY_RSHIFT              0xA1
#define FY_LCTRL               0xA2
#define FY_RCTRL               0xA3
#define FY_LALT                0xA4
#define FY_RALT                0xA5
#define FY_PROCESSKEY          0xE5
#define FY_ATTN                0xF6
#define FY_CRSEL               0xF7
#define FY_EXSEL               0xF8
#define FY_EREOF               0xF9
#define FY_PLAY                0xFA
#define FY_ZOOM                0xFB
#define FY_NONAME              0xFC
#define FY_PA1                 0xFD
#define FY_OEM_CLEAR           0xFE
#endif


// for system information query
//#define VIDEO_MEMORY                     1
#define TEXTURE_NUMBER_IN_VIDEO_MEMORY     2
#define TEXTURE_NUMBER_IN_MANAGED_MEMORY   3


#define LOGICAL_SCREEN         1
#define PHYSICAL_SCREEN        2


// cw4 file format
#define CW4_UNKNOWN                0
#define CW4_MODEL                  1
#define CW4_SCENE                  2
#define CW4_CHARACTER              3
#define CW4_SHADER                 4
//#define CW4_ACTOR                5    // no more used in Fly2


#define MAX_TIMERS                 8
#define MAX_GAMEPADS               8


// define gamepad buttons
#define FY_BUTTON_SELECT          0
#define FY_BUTTON_START           1
#define FY_BUTTON_UP              2
#define FY_BUTTON_RIGHT           3
#define FY_BUTTON_DOWN            4
#define FY_BUTTON_LEFT            5
#define FY_BUTTON_L_SHOULDER      6
#define FY_BUTTON_R_SHOULDER      7
#define FY_BUTTON_TRIANGLE        8
#define FY_BUTTON_CIRCLE          9
#define FY_BUTTON_CROSS           10
#define FY_BUTTON_SQUARE          11
#define FY_BUTTON_L_THUMB         12
#define FY_BUTTON_R_THUMB         13
#define FY_BUTTON_L_STICK         14
#define FY_BUTTON_R_STICK         15

#define FY_STICK_LEFT             0
#define FY_STICK_RIGHT            1

#define FY_TOUCH_FRONT_1          0
#define FY_TOUCH_FRONT_2          1
#define FY_TOUCH_FRONT_3          2
#define FY_TOUCH_FRONT_4          3
#define FY_TOUCH_FRONT_5          4
#define FY_TOUCH_FRONT_6          5
#define FY_TOUCH_FRONT_7          6
#define FY_TOUCH_FRONT_8          7
#define FY_TOUCH_FRONT_9          8
#define FY_TOUCH_FRONT_10         9
#define FY_TOUCH_BACK_1           10
#define FY_TOUCH_BACK_2           11
#define FY_TOUCH_BACK_3           12
#define FY_TOUCH_BACK_4           13
#define FY_TOUCH_BACK_5           14
#define FY_TOUCH_BACK_6           15
#define FY_TOUCH_BACK_7           16
#define FY_TOUCH_BACK_8           17
#define FY_TOUCH_BACK_9           18
#define FY_TOUCH_BACK_10          19

#define FY_MOTION_STATE            0
#define FY_INVALID                -1


#define FY_EPS                     0.000001
#define FY_EPS_DOUBLE              0.00000000000001


#ifndef WIN32
   typedef unsigned int DWORD;
#ifdef LINUX_XWINDOW
   typedef struct
   {
      Display *dpy;
      Window win;
   } XWinHand ;
   typedef XWinHand *HWND;
#else
   typedef unsigned int HWND;        // for _IOS_
#endif
   typedef unsigned char BYTE;
   typedef unsigned short WORD;
#endif


#ifndef BOOL4
   typedef unsigned int BOOL4;       // the boolean data type used in Fly2, length = 4 bytes
#endif

#ifndef TRUE
   #define TRUE                   1
#endif

#ifndef FALSE
   #define FALSE                  0
#endif


// global Data Types
typedef unsigned int SCENEid;                // scene ID
typedef unsigned int VIEWPORTid;             // viewport ID
typedef unsigned int OBJECTid;               // object ID
typedef          int GEOMETRYid;             // geometry element ID
typedef unsigned int PARTICLE_SYSTEMid;      // particle system ID
typedef unsigned int MATERIALid;             // material ID
typedef unsigned int TEXTUREid;              // texture ID
typedef unsigned int SHADERid;               // shader ID
typedef unsigned int ROOMid;                 // room ID
typedef unsigned int CHARACTERid;            // character ID
typedef unsigned int ACTIONid;               // action ID
typedef unsigned int TEXTid;                 // text object ID
typedef unsigned int AUDIOid;                // audio object ID
typedef unsigned int GAMEPADid;              // gamepad object ID
typedef unsigned int GAMEFX_SYSTEMid;        // game FX system object ID
typedef unsigned int GAMEFXid;               // game FX object ID


//#ifdef WIN32
typedef unsigned int THREADPOOLid;           // thread pool ID
//#endif


// macros
#define FYABS(A) ((A) > 0 ? (A) : -(A))
#define FYMAX(A,B) ((A) > (B) ? (A) : (B))
#define FYMIN(A,B) ((A) > (B) ? (B) : (A))


// system functions
extern BOOL4 FyStartFly(HWND gahWnd, int w, int h, BOOL4 beFullScreen, int aaLevel = 0, int numMemBlock = 0, float *blockLen = NULL, BOOL4 beUseThread = TRUE, float tM = 400.0f, float tmB = 200.0f);
extern void FyResizeWorld(int w, int h, BOOL4 beFullScreen);
extern BOOL4 FyCheckWorldSize(int *w, int *h, BOOL4 *beFS = NULL);
extern void FyQuitFly();
extern void FySwapBuffers();
extern void FyRestoreAll();
extern void FySetModelPath(const char *path);
extern void FySetAudioPath(const char *path);
extern void FySetScenePath(const char *path);
extern void FySetTexturePath(const char *path);
extern void FySetShaderPath(const char *path);
extern void FySetCharacterPath(const char *path);
extern void FySetGameFXPath(const char *path);
extern void FySetDataPath(const char *path);
extern void FySetFontPath(const char *path);
extern void FySetExtraWorkPath(const char *path);
extern char *FyGetModelPath();
extern char *FyGetAudioPath();
extern char *FyGetScenePath();
extern char *FyGetTexturePath();
extern char *FyGetShaderPath();
extern char *FyGetCharacterPath();
extern char *FyGetGameFXPath();
extern char *FyGetDataPath();
extern char *FyGetFontPath();
extern char *FyGetExtraWorkPath();
extern void FySetSystemShaderPath(const char *path);
extern char *FyGetSystemShaderPath();
extern void FyBindBatchRenderer(void (*fun0)());
extern void FyBindRestoreCallback(void (*fun0)());


// scene functions
extern SCENEid FyCreateScene(int numGroups = 12, BOOL4 beCreateMainLight = FALSE);
extern void FyDeleteScene(SCENEid sID);


// viewport functions
extern VIEWPORTid FyCreateViewport(int ox, int oy, int w, int h, BOOL4 BeInverseGLV = TRUE);
extern void FyDeleteViewport(VIEWPORTid vID);


// timer functions
extern void FyBindTimer(DWORD id, float fps, void (*fun)(int), BOOL4 beLock);
extern void FyTimerReset(DWORD id);
extern float FyTimerCheckTime(DWORD id);

// object functions
extern BOOL4 FyCheckObjectValidity(OBJECTid oID);

// material functions
extern MATERIALid FyCreateMaterial(float *amb = NULL, float *dif = NULL, float *spe = NULL, float shine = 100.0f, float *emi = NULL, float o = -1.0f);
extern void FyDeleteMaterial(MATERIALid mID);
extern void FySetNoAlphaCheckingForRenderTarget(BOOL4 beTexCheck);


// texture functions
extern TEXTUREid FyCreateRenderTargetTexture(const char *name, DWORD f, int w, int h, BOOL4 beA = FALSE, BOOL4 beZ = FALSE);
extern TEXTUREid FyCreateTexture(const char *name, DWORD f, int w, int h, BOOL4 beA = FALSE, DWORD pool = MANAGED_MEMORY);
extern BOOL4 FyDeleteTexture(TEXTUREid tID, BOOL4 beDeleteAnyway = FALSE);
extern void FySetDefaultMipmapLevel(int mmValue);


// math functions
extern void FyNormalizeVector3(float *v);
extern BOOL4 FyCross(float *v3, float *v1, float *v2);
extern float FyDot(float *v1, float *v2);
extern float FyDistance(float *p0, float *p1);
extern int FyRandomInteger(int minV, int maxV, int majorN);
extern float FyRandomNumber(float minV, float maxV);
extern void FyFormM12FromEulerAnglesXYZ(float *M, float rx, float ry, float rz);
extern void FyFormEulerAnglesXYZFromM12(float *rx, float *ry, float *rz, float *M);
extern void FyInverseM(float *M, float *IM, int dim);
extern void FyInverseMDouble(double *M, double *IM, int dim);
extern BOOL4 FyTriangulatePolygon(float *xyz, int np, int *inc, int **ans);
extern BOOL4 FyConvexPolygon(float *xyz3, int np, int *inc, int **ans);
extern BOOL4 FySolveSVD(double *a, int m, int n, double *u, double *w, double *v);
extern void FyCholeskyD(double *A, int n, double *L);
extern void FyMultiplyM16(float *C, float *A, float *B);
extern void FyMultiplyM12(float *C, float *A, float *B);
extern void FyHeapSort(int n, float *ra, int *rb);
extern void FyLinearFit(float *x, float *y, int nData, float *a, float *b, float *sig = NULL);
extern float FyStdNormalDistributionPDF(float x, float mu, float sigma);
extern void FyFindMeanSTD(float *data, int n, float *mu, float *std);
extern BOOL4 FyInverseKinematicsByCCD(float *link, int nLink, float *target, int iteration = 100, float eps = 0.001f, float dampW = -0.00001f, BOOL4 *lock = NULL);
extern BOOL4 FyCatmullRomSpline3(float *t, float *p0, float *p1, float *p2, float *p3, float u);
extern double FySHBasisFunD(int l, int m, double theta, double phi); 

// shader functions
extern SHADERid FyCreateShader(const char *cws, const char *tech);
extern void FyDeleteShader(SHADERid sID);


// text functions
extern TEXTid FyCreateText(const char *fontName, int size, BOOL4 beBold, BOOL4 beItalic, BOOL4 beMultiByte = FALSE, const char *fontPath = NULL, BOOL4 BeFTT = FALSE);
extern TEXTid FyUseText(const char *fontName, int size, BOOL4 beBold, BOOL4 beItalic, BOOL4 beMultiByte = FALSE, const char *fontPath = NULL, BOOL4 BeFTT = FALSE);
extern void FyDeleteText(TEXTid fID);


// audio functions
extern AUDIOid FyCreateAudio();
extern void FyDeleteAudio(AUDIOid aID);


// gamepad functions
extern int FyBeginGamepad();
extern void FyEndGamepad();
extern GAMEPADid FyCreateGamepad();
extern void FyDeleteGamepad(GAMEPADid mID);


// ETC
extern BOOL4 FyBeginMemorySystem(int numBlock, float *blockLen, float tmpBlockLen, float defBlockLen);
extern int FyReadNewLine(FILE *fp, char *str, BOOL4 beC = TRUE);
extern DWORD FyCheckCW4FileFormat(char *fileName);
extern BOOL4 FyCheckWord(const char *key1, const char *key2, BOOL4 beCase = TRUE, int lenLimit = FY_NONE);
extern void FyTruncateStringSpace(char *str);
extern int FyCheckFileFormat(const char *full, char *path, char *name, char *ext);
extern void FyRenderBenchMark(int *nT, int *nV);
extern int FyQuerySystem(DWORD type);
extern void FyBeRenderBoundingBox(BOOL4 beB);
extern double FyGetCurrentSystemTick();
extern void FyFillVertexColor(float *v, float *rgb, float opacity);


//#ifdef WIN32
extern THREADPOOLid FyGetSystemThreadPool(); 
//#endif


struct FDataChunkHead
{
   DWORD type;
   DWORD id;
   int len;
};


/*--------------------
  Base Function Class
 ---------------------*/
class FnFunction
{
protected:
   DWORD m_nID;  // id

public:
   FnFunction(DWORD id = FAILED_ID) { m_nID = id; };
  ~FnFunction() {};

   inline void ID(DWORD id) { m_nID = id; };
   inline void Object(DWORD id) { m_nID = id; };
   inline DWORD ID() { return m_nID; };
   inline DWORD Object() { return m_nID; };
};


/*------------------------
  Viewport Function Class
 -------------------------*/
class FnViewport : public FnFunction
{
public:
   FnViewport(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnViewport() {};

   void SetBackgroundColor(float r, float g, float b, float a = 1.0f);
   void GetSize(int *w, int *h);
   void GetPosition(int *ox, int *oy);
   void SetSize(int w, int h);
   void SetPosition(int ox, int oy);

   void Render3D(OBJECTid cID, BOOL4 beC = TRUE, BOOL4 beZ = TRUE, int nRT = 0, TEXTUREid *rtID = NULL);
   void RenderDepth(OBJECTid cID, DWORD type = UNIT_DEPTH_MAP, BOOL4 beAlpha = FALSE, TEXTUREid rtID = FAILED_ID);
   void RenderSprites(SCENEid sID, BOOL4 beC = TRUE, BOOL4 beZ = TRUE);

   void SetRenderTarget(int iOne, TEXTUREid tID);
   void SetRenderTarget(int iOne, MATERIALid mID, int slot, int layer);

   OBJECTid HitObject(OBJECTid *m, int numM, OBJECTid cam, int ix, int iy, OBJECTid *buf, int *numHit, BOOL4 beFFOnly = TRUE);
   BOOL4 CameraRay(OBJECTid cam, int x, int y, float *pos, float *vector, int axis, float value = 0.0f, float *dir = NULL, float *PtPos = NULL);
   GEOMETRYid HitPosition(OBJECTid target, OBJECTid camera, int ix, int iy, float *xyz, int *iHit = NULL);
   BOOL4 ComputeScreenPosition(OBJECTid cameraID, float *p2D, float *p3D, DWORD dwType, BOOL4 beZ);
   OBJECTid HitSprite(OBJECTid *spriteList, int numSprite, int x, int y);

   BOOL4 SetupBloomEffect(int w, int h, float eLevel = 2.0f, float bWeight = 0.6f, TEXTUREid inputID = FAILED_ID);
   void RenderBloomEffect(OBJECTid cam, BOOL4 beClear, BOOL4 beClearZ, int loop = 8, TEXTUREid rtID = BACK_BUFFER);

   BOOL4 SetupHDREffect(int w, int h, float eLevel = 1.3f, float bWeight = 0.6f, float white = 1.8f, 
                        float blurThreshold = 0.8f, TEXTUREid inputID = FAILED_ID);
   void RenderHDREffect(OBJECTid cID, BOOL4 beC, BOOL4 beCZ, int loop = 8, TEXTUREid rtID = BACK_BUFFER);

   void SetDOFFocus(float focus, float frontRange = FY_NONE, float rearRange = FY_NONE);
   BOOL4 SetupDOFEffect(float focus, float frontRange, float rearRange, BOOL4 beReset = FALSE, TEXTUREid inputID = FAILED_ID);
   void RenderDOFEffect(OBJECTid cID, BOOL4 beClear, BOOL4 beClearZ, TEXTUREid rtID = BACK_BUFFER);

   BOOL4 SetupSSAOEffect(float sampleRange = 18.0f, float dist = 6.0f, BOOL4 beReset = FALSE, TEXTUREid inputID = FAILED_ID);
   void RenderSSAOEffect(OBJECTid cID, BOOL4 beClear, BOOL4 beClearZ, TEXTUREid rtID = BACK_BUFFER);

   void UseReverseZ(BOOL4 beZR);
   void UseZBufferFrom(TEXTUREid texID, BOOL4 beD = FALSE);
};


/*---------------------
  Scene Function Class
 ----------------------*/
class FnScene : public FnFunction
{
public:
   FnScene(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnScene() {};

   int GetRenderGroupNumber();
   void SetCurrentRenderGroup(int id);
   int GetCurrentRenderGroup();

   int QueryObjectNumberInFile(char *file);
   int QueryCharacterNumberInFile(char *file);
   BOOL4 Load(char *fileName, OBJECTid *objList = NULL, int numO = 0, CHARACTERid *cList = NULL, int numC = 0);
   void Save(char *fileName, BOOL4 beInvisible = FALSE, BOOL4 beCharacter = FALSE, OBJECTid *exclusiveList = NULL, int numExclusive = 0);

   void SetAmbientLights(float sr, float sg, float sb, float gr, float gg, float gb);

   OBJECTid CreateObject(DWORD type = OBJECT, int rgID = FY_NONE);
   BOOL4 DeleteObject(OBJECTid oID);
   OBJECTid GetObjectByName(char *name);

   ROOMid CreateRoom(DWORD type, int maxEntity);
   void DeleteRoom(ROOMid roomID);

   PARTICLE_SYSTEMid CreateParticleSystem(char *name, int num, DWORD pType = SPHERICAL_BILLBOARD_CLOUD, DWORD bType = ALPHA_BLENDING);
   void DeleteParticleSystem(PARTICLE_SYSTEMid pID);

   CHARACTERid LoadCharacter(char *fileName, BOOL4 beGPU = FALSE, int maxBone = 50, void (*callBack)(char *) = NULL);
   void DeleteCharacter(CHARACTERid aID);

   GAMEFX_SYSTEMid CreateGameFXSystem();
   void DeleteGameFXSystem(GAMEFX_SYSTEMid gID);

   BOOL4 SetLightProbe(char *dfTex, char *sp100Tex = NULL, char *sp500Tex = NULL, char *sp1000Tex = NULL, char *refTex = NULL,
                      float scaleD = 1.0f, float scaleS = 1.0f, float scaleR = 0.5f);
   void SetLightProbeOffset(float u, float v = 0.0f);
   BOOL4 SetSkyDome(BOOL4 beON, char *name, int res = 10, int res1 = 10, float size = 400.0f, int w = 1280, int h = 720);
   void SetLightProbeCenter(float *center);
   void SetLightProbeRadius(float radius);
   TEXTUREid GetSkyDomeTexture();

   BOOL4 IsZUp();
   void BeZUp(BOOL4 beZ);

   int ObjectNumber();
   void GetObjects(OBJECTid *objList, int bSize);

   BOOL4 PlayAnimation(float skip, DWORD mode, BOOL4 beC = FALSE);
   BOOL4 PlayFrame(float frame, BOOL4 beC = FALSE);

   void SetSpriteWorldSize(int w, int h, int z = 1000);
   OBJECTid GetSpriteCamera();
};


/*--------------------
  Room Function Class
 ---------------------*/
class FnRoom : public FnFunction
{
public:
   FnRoom(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnRoom() {};

   void AddObject(OBJECTid oID);
   void RemoveObject(OBJECTid oID);

   void SetRoomSize(float mX, float MX, float mY, float MY, float mZ, float MZ);
};


/*--------------------------
  Clone Room Function Class
 ---------------------------*/
class FnCloneRoom : public FnRoom
{
public:
   FnCloneRoom(DWORD m = FAILED_ID) : FnRoom(m) {};
  ~FnCloneRoom() {};

   void Initialize(DWORD type, DWORD mother, int maxCloned);
   DWORD Spawn(BOOL4 beGeoCopy = FALSE);
};


/*------------------------
  BSP Room Function Class
 -------------------------*/
class FnBSPRoom : public FnRoom
{
public:
   FnBSPRoom(DWORD m = FAILED_ID) : FnRoom(m) {};
  ~FnBSPRoom() {};

  void Commit(OBJECTid cID);   
};


/*----------------------
  Object Function Class
 -----------------------*/
class FnObject : public FnFunction
{
public:
   FnObject(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnObject() {};

   DWORD GetType();
   SCENEid GetScene();
   void SetName(const char *name);
   char *GetName();
   void Translate(float x, float y, float z, DWORD op, BOOL4 beLocalOnly = FALSE);
   void Rotate(DWORD axis, float degree, DWORD op, BOOL4 beLocalOnly = FALSE);
   void Quaternion(float w, float x, float y, float z, DWORD op, BOOL4 beLocalOnly = FALSE);
   void Scale(float sx, float sy, float sz, DWORD op, BOOL4 beLocalOnly = FALSE);
   void SetMatrix(float *M12, DWORD op, BOOL4 beLocalOnly = FALSE);
   void GetMatrix(float *MFinal, BOOL4 beGlobal = FALSE, BOOL4 beApplyObjectMatrix = FALSE);

   void SetParent(OBJECTid objID);
   OBJECTid GetParent();
   void ChangeScene(SCENEid sceneID);
   void ChangeRenderGroup(int rgID);

   void Show(BOOL4 beShow);
   BOOL4 GetVisibility();
   void Twiddle(BOOL4 beON);

   void DropToTrashCan(BOOL4 beTrash);
   BOOL4 BeInTrashCan();

   void SetOpacity(float o);
   float GetOpacity();
   void SetRenderOption(DWORD item, DWORD value);
   void SetRenderMode(DWORD mode);
   DWORD GetRenderMode();
   int GetRenderGroup();
   void SetAlphaFlag(BOOL4 beAlpha);
   BOOL4 GetAlphaFlag();

   BOOL4 Load(const char *fileName, int levelLOD = FY_NONE);
   void Save(char *fileName);

   GEOMETRYid Lines(int type, MATERIALid mID, float *v, int nV, BOOL4 beUpdate = TRUE, BOOL4 beVC = FALSE, BOOL4 beVT = FALSE, int nRender = -1);
   GEOMETRYid Triangles(int vType, MATERIALid mID, int nVA, int nV, int nTex, int *texLen, float *v,
                        int nT, int *triTable, int tangent = 0, BOOL4 beUpdate = TRUE, BOOL4 *beRV = NULL, int nRender = -1);
   GEOMETRYid NurbsCurve(MATERIALid mID, int degree, float *cv, int nCV, float *knot, float *weight, int numP, BOOL4 beUpdate = TRUE);
   GEOMETRYid NurbsSurface(MATERIALid mID, int uDegree, int vDegree, float *cv, int nCU, int nCV, float *knotU, float *knotV, 
                           float *weight, int numPU, int numPV, BOOL4 beUpdate = TRUE);
   GEOMETRYid Billboard(float *pos, float *size, char *tex, int nTex = 0, float *color = NULL,
                        BOOL4 beKey = FALSE, BYTE *rgb = NULL, int type = INDEPENDENT, BOOL4 beUpdate = TRUE);
   GEOMETRYid Label(int label);
   GEOMETRYid GotoLabelNext(int label);
   GEOMETRYid GotoLabel(int label);

   int GetGeometryNumber();
   int GetGeometries(GEOMETRYid *gID, int numG);
   GEOMETRYid GetGeometryID(int iOne);
   void RemoveAllGeometries();
   BOOL4 RemoveGeometryBetweenLabel(int l1, int l2);
   void ShowGeometryBetweenLabel(int l1, int l2, BOOL4 beShow);
   BOOL4 RemoveGeometry(GEOMETRYid gID);

   GEOMETRYid HitTest(float *origin, float *dir, float *result = NULL, int *iHit = NULL, float *area = NULL, float *pNorm = NULL, float *t = NULL, GEOMETRYid lastGeo = FAILED_ID);

   int GetMaterialNumber();
   void GetMaterials(MATERIALid *mID, int numM = 0);

   BOOL4 CheckVertexAnimation();
   BOOL4 NextVertexArray(float skipFrame, DWORD playMode);
   void SetAlphaTest(BOOL4 beAT, DWORD refValue);

   void ShowBoundingBox(BOOL4 beShow);
   BOOL4 GetBoundingVolume(float *v, int *inc, BOOL4 beQuad = FALSE, float *center = NULL, float *radius = NULL, BOOL4 beReScan = FALSE);
   void GetLocalBoundingExtreme(float *mx, float *MX, float *my, float *MY, float *mz, float *MZ);
   void SetLocalBoundingExtreme(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

   void SetDirectionAlignment(DWORD f, DWORD u);
   void GetPosition(float *pos, BOOL4 beWorldSpace = TRUE);
   void SetPosition(float *pos, BOOL4 beWorldSpace = TRUE);
   void GetDirection(float *fDir, float *uDir, BOOL4 beWorldSpace = TRUE);
   void SetDirection(float *fDir, float *uDir, BOOL4 beWorldSpace = TRUE);

   void SetTerrainRoom(ROOMid rID, float hLimit, float angle = 75.0f, float front = 10.0f, float back = 10.0f);
   void SetCollisionRoom(ROOMid rID);
   BOOL4 PutOnTerrain(float *pos = NULL, float height = 100.0f, BOOL4 be3D = FALSE);
   int MoveForward(float dist, BOOL4 beTF = FALSE, BOOL4 be3D = FALSE, float offset = 0.0f, BOOL4 beCollide = FALSE);
   int MoveRight(float dist, BOOL4 beTF = FALSE, BOOL4 be3D = FALSE, float offset = 0.0f, BOOL4 beCollide = FALSE);
   int MoveUp(float dist, BOOL4 beCollide = FALSE);
   int TurnRight(float angle);

   BOOL4 PlayFrame(float frame, float *M12 = NULL);
   float GetCurrentFrame();
   int FrameNumber();

   BOOL4 NextTexture(int skip, DWORD mode);
   BOOL4 CheckTextureAnimation();
   BOOL4 SetCurrentTexture(int iFrame);

   void BindPostRenderFunction(void (*fun)(OBJECTid));
   void BindPreRenderFunction(void (*fun)(OBJECTid));

   void UseLOD(int numLevels);
   void SetLODData(int level, float distance);
   void GetLODData(int level, float *distance);
   void PerformLOD(float *cameraPos);
   int GetLODLevels();

   OBJECTid Clone(BOOL4 beGeoCopy = FALSE, BOOL4 beMatCopy = FALSE, BOOL4 beTexCopy = FALSE);

   GEOMETRYid Spawn(GEOMETRYid gID, BOOL4 beGeoCopy = FALSE, BOOL4 beMatCopy = FALSE, BOOL4 beTexCopy = FALSE);

   float *LockVertexBuffer(GEOMETRYid gID, int *nV, int *vLen, int iC = 0, BOOL4 beReadOnly = FALSE);
   void UnlockVertexBuffer(GEOMETRYid gID);
   int *LockIndexBuffer(GEOMETRYid gID, int *nT, BOOL4 *beR = NULL, BOOL4 beReadOnly = FALSE);
   void UnlockIndexBuffer(GEOMETRYid gID);
   float *GetBackupVertices(GEOMETRYid gID, int *nV, int *vLen);

   void *InitData(int lenByte);
   void *GetData();
   void ReleaseData();

   void GenerateTangentVectors(int numT = 1);
};


/*----------------------
  Camera Function Class
 -----------------------*/
class FnCamera : public FnObject
{
public:
   FnCamera(DWORD m = FAILED_ID) : FnObject(m) {};
  ~FnCamera() {};

   void SetProjectionType(DWORD type);
   void SetAspectRatio(float aspect);
   void SetFov(float fov);
   void SetNearPlane(float nearP);
   void SetFarPlane(float farP);
   void SetScaleFactor(float sf);
   void SetScreenRange(float x, float y, float w, float h);

   float GetFov();
   float GetNearPlane();
   float GetFarPlane();
   float GetAspectRatio();

   void GetProjectionMatrix(float *P);
   void ReplaceProjectionMatrix(float *P);
   void GetViewingMatrix(float *V);
   void ReplaceViewingMatrix(float *V);
   float GetScaleFactor();

   BOOL4 PlayFOV(float curFrame);
   BOOL4 IsFOVAnimation();

   void SetEyeShift(float dist, float focus);
};


/*---------------------
  Light Function Class
 ----------------------*/
class FnLight : public FnObject
{
public:
   FnLight(DWORD m = FAILED_ID) : FnObject(m) {};
  ~FnLight() {};

   void SetLightType(DWORD type);
   void SetColor(float r, float g, float b);
   void SetIntensity(float intensity);
   void SetRange(float range);
   void SetSpotCone(float iAngle, float oAngle, float fallOff);
   void TurnLight(BOOL4 beON);

   void UseShadowMap(int w, int h, int pcfLevel = 1);
};


/*----------------------
  Sprite Function Class
 -----------------------*/
class FnSprite : public FnObject
{
public:
   FnSprite(DWORD m = FAILED_ID) : FnObject(m) {};
  ~FnSprite() {};

   void SetPosition(int ox, int oy, int layer);
   void GetPosition(int *ox, int *oy, int *oz);
   void SetSize(int w, int h);
   void GetSize(int *w, int *h);
   void SetColor(float *rgb);
   void GetColor(float *rgb);
   void SetHotSpot(int *hotSpot);
   void GetHotSpot(int *hotSpot);
   BOOL4 UseMaterial(MATERIALid mID, float *uv = NULL, BOOL4 beAlpha = FALSE, BOOL4 beInvUV = FALSE, int resU = 1, int resV = 1);
   TEXTUREid SetImage(char *texHead, int nTex = 0, float *uv = NULL, BOOL4 beKey = FALSE, BYTE *kColor = NULL, 
                      DWORD poolType = MANAGED_MEMORY, BOOL4 beAlpha = FALSE, DWORD filter = FILTER_NONE, BOOL4 beInvUV = FALSE,
                      int resU = 1, int resV = 1, int layer = 0);
   TEXTUREid SetRawImage(int iTex, BYTE *raw, int size, int rawW, int rawH, DWORD format, float *uv = NULL, BOOL4 beKey = FALSE, BYTE *rgb = NULL,
                         DWORD poolType = MANAGED_MEMORY, BOOL4 beAlpha = FALSE, DWORD filter = FILTER_NONE, BOOL4 beInvUV = FALSE,
                         BOOL4 beRenderTarget = FALSE, int resU = 1, int resV = 1, int layer = 0, char *texName = NULL);
   void SetUV(int layer, float *uv, BOOL4 beRV = FALSE);
   void ReleaseVideoMemory();
   void ChangeCornerPositionUV(float *pos, int layer, float *uv, BOOL4 beRV = FALSE);
   TEXTUREid GetImage();

#ifdef FLY2_OPENGL
   void MakeSharper(BOOL4 BeSharper);
#endif
};


/*---------------------------
  Sprite Text Function Class
 ----------------------------*/
class FnSpriteText : public FnSprite
{
public:
   FnSpriteText(DWORD m = FAILED_ID) : FnSprite(m) {};
  ~FnSpriteText() {};

   TEXTid UseFont(const char *fontName, int size, BOOL4 beBold = FALSE, BOOL4 beItalic = FALSE, BOOL4 beMultiByte = FALSE);
   TEXTUREid SetArea(int w, int h);
   void Begin();
   int Write(int ox, int oy, char *string, BYTE r, BYTE g, BYTE b, BYTE a = 255);
   int WriteUTF8(int ox, int oy, char *string, BYTE r, BYTE g, BYTE b, BYTE a = 255);
   void End();
};


/*-------------------------------
  Particle System Function Class
 --------------------------------*/
class FnParticleSystem : public FnFunction
{
public:
   FnParticleSystem(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnParticleSystem() {};

   void Initialize(float *rawSize, float *color, char *texName, BOOL4 beShow, float defDensity = 0.15f, float fps = 30.0f);
   void SetupTextureAnimation(int uDiv, int vDiv, int numA);
   int Born(float *mass, float *life, float radius, float *initVel, float *speed, DWORD mode = EACH_EMITTER, int numP = 1, 
            float r = 1.0f, float density = 0.15f, int fadeRate = 0, float *initTexureFrame = NULL, float *texAnimSpeed = NULL);
   int NextEulerStep(float skip, BOOL4 beCollide, int samples = 1);
   OBJECTid GetEmitterObject();
   OBJECTid GetParticleObject();
   void AssignEmitterObject(OBJECTid oID, DWORD type);

   OBJECTid ApplyForce(DWORD type, float mag);
   void RemoveForce(OBJECTid fID, BOOL4 beD = TRUE);
   void SetCollisionRoom(ROOMid roomID);

   void Render(VIEWPORTid vID, OBJECTid cID, TEXTUREid tID, BOOL4 beZC = FALSE);
};


/*------------------------
  Material Function Class
 -------------------------*/
class FnMaterial : public FnFunction
{
public:
   FnMaterial(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnMaterial() {};

   char *GetName();
   void SetName(const char *name);
   void SetEmissive(float *emi);
   void SetAmbient(float *amb);
   void SetDiffuse(float *dif);
   void SetSpecular(float *spe);
   void SetShininess(float s);

   float *GetAmbient();
   float *GetDiffuse();
   float *GetSpecular();
   float *GetEmissive();
   float GetShininess();

   void InitializeTextures(int nSlot, int nLayer);
   TEXTUREid AddRenderTarget(int slot, int layer, const char *name, DWORD f, int w, int h, BOOL4 beA = FALSE, BOOL4 beZ = FALSE, int cubemap = 0);
   TEXTUREid LoadTexture(int slot, int layer, char *texName, BOOL4 beKey = FALSE, BYTE *key = NULL, int mipLevel = 0,
                         DWORD pool = MANAGED_MEMORY, BOOL4 beAlpha = FALSE);
   TEXTUREid CreateTexture(int slot, int layer, const char *name, BYTE *raw, int size, int w, int h, DWORD f, BOOL4 beAlpha = FALSE,
                           DWORD pool = MANAGED_MEMORY, BOOL4 beKey = FALSE, BYTE *key = NULL);
   TEXTUREid GetTexture(int slot, int layer);
   BOOL4 AddTexture(int slot, int layer, TEXTUREid texID);
   BOOL4 AddTexture(int slot, int layer, const char *texName);
   //BOOL4 AddRawTextureLayer(int slot, int layer, BYTE *raw, int size, int w, int h, const char *texName, DWORD format,
   //        BOOL4 beAlpha, DWORD poolType);
   BOOL4 RemoveTexture(int slot, int layer, BOOL4 beD = TRUE);
   BOOL4 NextTexture(int skipFrame, DWORD playMode);
   BOOL4 SetCurrentTexture(int iFrame);
   TEXTUREid GetCurrentTexture(int iLayer = 0);
   BYTE *LockTextureBuffer(int slot, int layer, int *len);
   void UnlockTextureBuffer(int slot, int layer, int w = 0, int h = 0);

   DWORD GetTextureFormat(int slot, int layer);
   BOOL4 GetTextureSize(int slot, int layer, int *w, int *h);
   BOOL4 CopyTexture(int ds, int dl, MATERIALid mID, int ss, int sl);
   int GetTextureSlotNumber();

   SHADERid AddShaderEffect(const char *cwsFile, const char *effectName = NULL);
   SHADERid AddShaderEffect(DWORD idShader, const char *effectName = NULL);
   SHADERid GetShaderEffect();
   SHADERid ReplaceShaderEffect(SHADERid newS, BOOL4 beD = TRUE);

   void RenderCubeMap(int slot, int layer, float *camPos, SCENEid sID, BOOL4 beUsedMaterial = FALSE, 
                      MATERIALid mID = FAILED_ID, OBJECTid noRenderObj = FAILED_ID);

   // material data
   int SetDataLength(int len);
   void SetData(int id, float v);
   float GetData(int id);
   void SetBooleanData(int id, BOOL4 v);
   void SetIntData(int id, int iValue);

   // clone
   MATERIALid Clone(BOOL4 beTexCopy = FALSE);

   // texture UV transformation
   void Scale(int iLayer, float su, float sv, DWORD op);
   void Rotate(int iLayer, float d, DWORD op);
   void Translate(int iLayer, float u, float v, DWORD op);

   // alpha test associated
   void SetAlphaTest(BOOL4 beAlphaTest, DWORD alphaRef = 10);

#ifdef WIN32
   void SaveTexture(int slot, int layer, char *name, DWORD fileType);
   void FillTexture(int, int, void (WINAPI *)(float *, const float *, const float *, void *));
#endif
};


/*----------------------
  Shader Function Class
 -----------------------*/
class FnShader : public FnFunction
{
public:
   FnShader(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnShader() {};

   void Render(VIEWPORTid vID, MATERIALid mID);
   void Render(VIEWPORTid vID, OBJECTid cID, int nObj, OBJECTid *obj, BOOL4 beC, BOOL4 beCZ, MATERIALid mID = FAILED_ID);
   void SetPasses(int num);
   char *GetSourceName();
   char *GetShaderEffectName();
   char *GetPath();

#ifdef WIN32
#ifdef FLY2_DX9
   int GetPassNumber();
   int GetDataNumber(int pass);
   char *GetDataVariableName(int pass, int i, int *id, int *len);
#endif
#endif
};


/*-----------------------
  Texture Function Class
 ------------------------*/
class FnTexture : public FnFunction
{
public:
   FnTexture(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnTexture() {};

   BOOL4 Save(char *name, DWORD format);
   char *GetPathName();
   DWORD GetFileFormat();
   void SetFilter(DWORD type);
   void SetAddressMode(DWORD mode);
   void GetTextureSize(int *w, int *h);

   BYTE *LockTextureBuffer(int *len);
   void UnlockTextureBuffer();
};


/*-------------------------
  Character Function Class
 --------------------------*/
class FnCharacter : public FnFunction
{
public:
   FnCharacter(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnCharacter() {};

   char *GetName();
   void ChangeScene(SCENEid sID);
   int DefinePose(const char *name, int startFrame, int endFrame, int cutFrame, char *motionName = NULL);
   void DefineBodyPose(const char *bodyName, const char *poseName, int startFrame, int endFrame, int cutFrame, char *motionName = NULL);
   BOOL4 DeleteBodyPose(const char *bodyName, const char *poseName);
   BOOL4 CheckBodyPose(const char *bodyName, const char *poseName);

   BOOL4 PlayFrame(float frame, BOOL4 beBase = FALSE, BOOL4 bePerform = FALSE, float step = 1.0f, char *mName = NULL);
   BOOL4 LoadMotion(const char *fileName, char *motionName, int *startFrame, int *endFrame, char *aName);
   BOOL4 RemoveMotion(char *mName);

   ACTIONid CreateAction(char *bodyName, char *aName, char *poseName, BOOL4 beD = TRUE);
   ACTIONid CreateCrossBlendAction(char *bodyName, char *aName, char *frontPoseName, char *rearPoseName, float length, BOOL4 beD = TRUE);
   ACTIONid CreateConnectAction(char *bodyName, char *aName, char *frontPoseName, char *rearPoseName, float length, BOOL4 beD = TRUE);
   ACTIONid CreateFullBlendAction(char *bodyName, char *aName, char *frontPoseName, char *rearPoseName, float w0, float w1, BOOL4 beD = TRUE);
   void DeleteAction(ACTIONid aID);
   int GetBodyTotalActionNumber(char *bodyName);
   int GetBodyAllActions(char *bodyName, ACTIONid *animTable, int maxLen);

   ACTIONid GetBodyAction(char *bodyName, char *actionName);
   ACTIONid SetCurrentAction(char *bodyName, int channel, ACTIONid animID, float bLength = 0.0f, BOOL4 bePlay = TRUE);
   ACTIONid GetCurrentAction(char *bodyName, int channel = 0);
   BOOL4 Play(DWORD mode, float frame, BOOL4 beBase = TRUE, BOOL4 beSkin = FALSE, BOOL4 beCollide = FALSE);

   int GetBlendShapeNumber(int bsID);
   OBJECTid GetBaseShape(int bsID);
   char *GetBlendShapeName(int bsID, int iOne);

   BOOL4 LoadFacialExpression(char *fileName);
   BOOL4 SaveFacialExpression(char *fileName);

   int FacialExpressionNumber();
   char *GetFacialExpressionName(int iOne);
   void SetFacialExpressionName(int iOne, char *name);
   int GetFacialExpressionLength(char *name);
   BOOL4 SetFacialExpressionLength(char *name, int length);
   void AddFacialExpression(char *name);
   void RemoveFacialExpression(char *name);

   BOOL4 MakeCurrentFacialExpression(char *name);
   char *GetCurrentFacialExpression();
   BOOL4 PlayFacialExpression(int mode, float frame, BOOL4 beAdd = FALSE);

   int FacialExpressionKeyNumber(char *name);
   void FacialExpressionMask(char *name, int id);
   float GetFacialExpressionKey(char *name, int iOne, char **shapeName, float *weight, int *len = NULL);
   BOOL4 ModifyFacialExpressionKey(char *name, float pos, char **shapeName, float *weight, int len = 1);
   void AddFacialExpressionKey(char *name, float pos, char **shapeName, float *weight, int len = 1);
   BOOL4 RemoveFacialExpressionKey(char *name, float pos);
   int CheckFacialExpressionKey(char *name, float pos);

   int GetBodyPoseNumber(const char *bodyName);
   BOOL4 QueryBodyPose(const char *bodyName, int iOne, char *poseName, int *startFrame, int *endFrame, char *mName = NULL);
   BOOL4 QueryBodyPoseDataWithName(const char *bodyName, char *poseName, int *startFrame, int *endFrame, char *mName = NULL);

   void Save(const char *fileName, BOOL4 beBinary = TRUE, void (*callback)(FILE *) = NULL);
   void ExportMayaOBJ(const char *fileName);

   OBJECTid GetBaseObject();

   int GetSkinNumber();
   OBJECTid GetSkin(int id);
   OBJECTid GetSkinByName(const char *name);
   int ApplySkin(OBJECTid obj);
   BOOL4 TakeOffSkin(OBJECTid oID);
   void UseSkin(int iOne, BOOL4 BeUse);

   int GetAttachmentNumber();
   OBJECTid GetAttachment(int id);
   OBJECTid GetAttachmentByName(const char *name);
   int ApplyAttachmentByBoneID(OBJECTid oID, int iBone);
   int ApplyAttachmentByBoneName(OBJECTid oID, char *name);
   void UseAttachment(int iOne, BOOL4 BeUse);
   int GetAttachmentParentBoneID(int iSeg);

   int GetBoneNumber();
   BOOL4 IsGeometryBone(int iOne);
   char *FindBoneName(int id);
   OBJECTid GetBoneObject(char *name);
   OBJECTid GetBoneObject(int iOne);
   OBJECTid GetRootBone(int iOne = 0);

   void SetTerrainRoom(ROOMid rID, float hLimit, float angle = 75.0f, float front = 10.0f, float back = 10.0f);
   void SetCollisionRoom(ROOMid rID);
   BOOL4 PutOnTerrain(float *pos = NULL, float height = 100.0f, BOOL4 be3D = FALSE);
   int MoveForward(float dist, BOOL4 beTF = FALSE, BOOL4 be3D = FALSE, float offset = 0.0f, BOOL4 beCollide = FALSE);
   int MoveRight(float dist, BOOL4 beTF = FALSE, BOOL4 be3D = FALSE, float offset = 0.0f, BOOL4 beCollide = FALSE);
   int MoveUp(float dist, BOOL4 beCollide = FALSE);
   int TurnRight(float angle);

   void SetTerrainParameters(float sF, float sR, float sU);
   void QueryLastMoveResult(int *f, int *r, int *u);

   void GetPosition(float *pos, BOOL4 beWorld = TRUE);
   void SetPosition(float *pos, BOOL4 beWorld = TRUE);
   void GetDirection(float *fDir, float *uDir, BOOL4 beWorld = TRUE);
   void SetDirection(float *fDir, float *uDir, BOOL4 beWorld = TRUE);

   void PerformLOD(float *cameraPos);
   void PerformSkinDeform();

   CHARACTERid Clone(BOOL4 beGeoDuplicate = FALSE, BOOL4 beMatDuplicate = FALSE, BOOL4 beTexDuplicate = FALSE);

   int GetBodyNumber();
   int SetCurrentBody(char *name);
   char *GetCurrentBodyName();
   char *GetBodyName(int iOne);
   char *GetRootBodyName(int iOne = 0);
   int CreateBody(char *bodyName, char *boneName);

   void MakeBaseMove(char *rootName, int iStart, int iEnd, BOOL4 be3D = FALSE, char *footName = NULL, int id = 0);

   void DropToTrashCan(BOOL4 beTrash);
   BOOL4 BeInTrashCan();

   void Show(BOOL4 beShowGeo, BOOL4 beShowSKin, BOOL4 beShowAttach, BOOL4 beShowBone);
   void SetLocalScale(float scaleFactor = 1.0f);
   float GetLocalScale();

   OBJECTid BakeCharacterToVertexAnimation(int iChannel, OBJECTid obj, DWORD type = VA_FROM_POSE, int sF = 0, int eF = FY_NONE, 
                                           BOOL4 beSkin = TRUE, BOOL4 beAtt = FALSE, BOOL4 beBone = FALSE);
};


/*----------------------
  Action Function Class
 -----------------------*/
class FnAction : public FnFunction
{
public:
   FnAction(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnAction() {};

   char *GetName();
   void SetName(const char *name);
   DWORD GetType();
   void SetCurrentPose(int iOne, char *name);
   char *GetCurrentPose(int iOne);
   float GetLength();
   void SetLength(float length);
   float GetPoseWeight(int iOne);
   void SetPoseWeight(int iOne, float w);

   float GetPoseCurrentFrame(int iOne);
   void SetPoseCurrentFrame(int iOne, float f);

   float GetTotalFrames();
};


/*-----------------------------
  Base geometry function class
 ------------------------------*/
class FnGeometry : public FnFunction
{
public:
   FnGeometry(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnGeometry() {};

   DWORD Type();
   void Show(BOOL4 beShow);

   float *LockVertexArray(int *vLen = NULL, int *nV = NULL, int vbID = 0, BOOL4 beReadOnly = FALSE, BOOL4 BeThreadSave = FALSE);
   void UnlockVertexArray();
   
   int *LockIndexArray(int *tLen = NULL, int *nT = NULL, BOOL4 beReadOnly = FALSE, BOOL4 *beRightHand = NULL, BOOL4 BeThreadSave = FALSE);
   void UnlockIndexArray();

   MATERIALid GetMaterial();
   void ReplaceMaterial(MATERIALid newMID, BOOL4 beDelete = TRUE);
};


/*---------------------------
  line object function class
 ----------------------------*/
class FnLine : public FnGeometry
{
public:
   FnLine(DWORD m = FAILED_ID) : FnGeometry(m) {};
  ~FnLine() {};

   void SetRenderLineCount(int nRender);
};


/*---------------------------------------
  indexed triangle object function class
 ----------------------------------------*/
class FnTriangle : public FnGeometry
{
public:
   FnTriangle(DWORD m = FAILED_ID) : FnGeometry(m) {};
  ~FnTriangle() {};

   int GetEdgeNumber(BOOL4 beD = FALSE);
   int GetEdges(float *edgeV, float *vNorm, int maxE, BOOL4 beD = FALSE);

   void GetEdgeVertices(int eID, int *v0, int *v1);
   void GetEdgeFaces(int eID, int *f0, int *f1);
   int GetSilhouetteEdges(OBJECTid camID, float *vOutline, BOOL4 beOutline, BOOL4 beCrease, float angle, BOOL4 beBoundary);
   float *GetBoundaryEdges(float *buffer, int bufferLen, int *returnedBufferLen, BOOL4 beD = FALSE);

   int GetTriangleNumber();
   int *CopyTriangleData(int *buffer, int bufferLen, int *returnedBufferLen);
   int GetVertexNumber(int *vLen);
   DWORD GetVertexType();
   float *GetVertexArray(float *buffer, int bufferLen, int *returnedBufferLen, BOOL4 beRaw = TRUE, int vID = 0);
   void SetTriangleNumberForRendering(int n);

   void AddVertexNormal();
   void AddTextureCoordinates(int len, BOOL4 beRV = TRUE);
   void GenerateTangent(int numTangent);

   void *GetSkinWeightData(int *nW = NULL);

   BOOL4 WeldVertex(float error = 0.01f);
};


/*--------------------------------------
  billboard cloud object function class
 ---------------------------------------*/
class FnBillboardCloud : public FnGeometry
{
public:
   FnBillboardCloud(DWORD m = FAILED_ID) : FnGeometry(m) {};
  ~FnBillboardCloud() {};

   void Resize(float *newSize);
};


/*--------------------------------
  billboard object function class
 ---------------------------------*/
class FnBillboard : public FnGeometry
{
public:
   FnBillboard(DWORD m = FAILED_ID) : FnGeometry(m) {};
  ~FnBillboard() {};

   void SetPositionSize(float *newPos, float *newSize);
   void GetLocalPosition(float *pos);
   void GetSize(float *size);
};


/*--------------------
  Text Function Class
 ---------------------*/
class FnText : public FnFunction
{
public:
   FnText(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnText() {};

   void Begin(VIEWPORTid vID);
   void End();
   int Write(char *string, int ox, int oy, BYTE r, BYTE g, BYTE b, BYTE a = 255, BOOL4 beC = FALSE, int length = 1024);
   int WriteFTT(char *string, int ox, int oy, BYTE r, BYTE g, BYTE b, BYTE a = 255, BOOL4 beC = FALSE, int length = 1024);
   int GetWidth(char *string, int *piHeight = NULL);
#ifdef FREE_TYPE
   void DisplayText();
#endif
};


/*-----------------------
  Gamepad Function Class
 ------------------------*/
class FnGamepad : public FnFunction
{
public:
   FnGamepad(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnGamepad() {};

   void BindButtonFunction(int bID, void (*func)(GAMEPADid, int, BOOL4, float), BOOL4 beRepeat, BYTE hotKeyCode = 0);
   void BindJoystickFunction(int stickID, void (*fun)(GAMEPADid gpID, int sID, float x, float y));
   void BindTouchFunction(int touchID, void (*funcPressed)(GAMEPADid, int, int*, int*, int*), void (*funcDrag)(GAMEPADid, int, int*, int*, int*), void (*funcReleased)(GAMEPADid, int, int*, int*, int*));
   void BindMotionFunction(int motionID, void (*fun)(GAMEPADid, int, float*, float*, float*));
   void Poll();
   void GetJoystickValue(int stickID, float *x, float *y);
   void GetButtonValue(int bID, BOOL4 *pressed);
};


/*---------------------
  Audio Function Class
 ----------------------*/
class FnAudio : public FnFunction
{
public:
   FnAudio(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnAudio() {};

   BOOL4 Load(char *file);
   BOOL4 LoadRawData(BYTE *pData, DWORD uiSize);
   BOOL4 CreateStremAudio(int inBuffer, BYTE** ppBuffers, int* piSize, int iFormat, int iFrequency, BOOL4 be3D);
   BOOL4 UpdateStremAudio(int iID, BYTE* (*fun)(int, int*, int* ));

   void Play(DWORD mode);
   void Stop();
   void Pause();
   void SetVolume(float v);
   AUDIOid Clone();
   BOOL4 IsPlaying();
};


/*------------------------------
  Game FX System Function Class
 -------------------------------*/
class FnGameFXSystem : public FnFunction
{
public:
   FnGameFXSystem(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnGameFXSystem() {};

   void SetName(const char *name);
   char *GetName();

   GAMEFXid NewFX(DWORD type, BOOL4 beCurrent = TRUE);
   BOOL4 DeleteFX(GAMEFXid gxID, BOOL4 beD = TRUE);
   void RestoreFX(GAMEFXid gxID);
   int GameFXNumber();
   GAMEFXid GetGameFX(int iOne);
   GAMEFXid GetGameFXByName(const char *name);
   GAMEFXid GetCurrentGameFX();
   void MakeCurrentGameFX(GAMEFXid gxID);

   BOOL4 Play(float skipFrame, DWORD mode);
   void Reset();
   void Pause();
   BOOL4 PlayFrame(float iFrame);

   int QueryFXNumberInFile(char *file);
   BOOL4 Load(char *name, BOOL4 beReset, GAMEFXid *gxArray = NULL, int numFX = 0);
   void Save(char *name);

   void SetPlayLocation(float *pos);
   void SetParentObjectForAll(OBJECTid pID);

   void DropToTrashCan(BOOL4 beT);
};


/*----------------------------
  Base Game FX Function Class
 -----------------------------*/
class FnBaseGameFX : public FnFunction
{
public:
   FnBaseGameFX(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnBaseGameFX() {};

   OBJECTid GetBaseObject();

   void SetName(const char *name);
   char *GetName();
   DWORD GetFXType();
   void SetInitialLife(float l);
   void AdjustLife(float l, BOOL4 beAdjustAuto = TRUE);
   float GetLife();

   void SetParentObject(OBJECTid pID, char *name = NULL);
   OBJECTid GetParentObject();
   char *GetParentObjectName();

   float GetVisibility();
   void SetVisibility(float v, BOOL4 beRender = FALSE);
   int GetVisibilityKeyNumber();
   void GetVisibilityKey(int iOne, int *iKey, float *v);
   void SetVisibilityKey(int iKey, float v);
   void RemoveVisibilityKey(int iKey);

   void GetPosition(float *pos);
   void SetPosition(float *pos);
   int GetPositionKeyNumber();
   void GetPositionKey(int iOne, int *iKey, float *v);
   void SetPositionKey(int iKey, float *v);
   void RemovePositionKey(int iKey);

   void GetRotation(float *rot);
   void SetRotation(float *rot);
   int GetRotationKeyNumber();
   void GetRotationKey(int iOne, int *iKey, float *v);
   void SetRotationKey(int iKey, float *v);
   void RemoveRotationKey(int iKey);
};


/*-----------------------------
  Plate Game FX Function Class
 ------------------------------*/
class FnPlateGameFX : public FnBaseGameFX
{
public:
   FnPlateGameFX(DWORD m = FAILED_ID) : FnBaseGameFX(m) {};
  ~FnPlateGameFX() {};

   DWORD GetPlateType();
   void GetTextureData(char *texName, int *numTex);
   void GetAlphamapData(char *texName);

   OBJECTid CreatePlate(DWORD type, float *size, float *color = NULL, char *texName = NULL, int numTex = 0, int rg = FY_NONE, char *alphaMap = NULL);
   OBJECTid GetPlateObject();
   BOOL4 LoadTextures(char *texName, int numTex);
   BOOL4 LoadAlphamaps(char *alphaName);

   DWORD GetBlendingType();
   void SetBlendingType(DWORD type);

   void GetPlateSize(float *size);
   void SetPlateSize(float *size);
   int GetPlateSizeKeyNumber();
   void GetPlateSizeKey(int iOne, int *iKey, float *v);
   void SetPlateSizeKey(int iKey, float *v);
   void RemovePlateSizeKey(int iKey);

   void GetPlateColor(float *color);
   void SetPlateColor(float *color);
   int GetPlateColorKeyNumber();
   void GetPlateColorKey(int iOne, int *iKey, float *v);
   void SetPlateColorKey(int iKey, float *v);
   void RemovePlateColorKey(int iKey);
};


/*---------------------------------
  Geometry Game FX Function Class
 ----------------------------------*/
class FnGeometryGameFX : public FnBaseGameFX
{
public:
   FnGeometryGameFX(DWORD m = FAILED_ID) : FnBaseGameFX(m) {};
  ~FnGeometryGameFX() {};

   DWORD GetBlendingType();
   void SetBlendingType(DWORD type);
   void GetTextureData(char *texName, int *numTex);
   void GetAlphamapData(char *texName);
   BOOL4 LoadTextures(char *texName, int numTex);
   BOOL4 LoadAlphamaps(char *alphaName);

   OBJECTid GetModelObject();
   OBJECTid Initialize(BOOL4 beD = TRUE);
   void SetModelObject(OBJECTid oID, char *path = NULL, BOOL4 beD = TRUE);
   char *GetModelFullName();
   void SetModelFullName(char *name);
};


/*---------------------------------
  Particles Game FX Function Class
 ----------------------------------*/
class FnParticlesGameFX : public FnBaseGameFX
{
public:
   FnParticlesGameFX(DWORD m = FAILED_ID) : FnBaseGameFX(m) {};
  ~FnParticlesGameFX() {};

   BOOL4 CreateParticles(int maxP, char *texName, float *size, DWORD bbType, DWORD bType, int numTex = 1, int *uv = NULL);

   DWORD GetBlendingType();
   void SetBlendingType(DWORD type);

   void SetParticleLife(float *life);
   void GetParticleLife(float *life);

   void SetParticleBornPerFrame(int n);
   int GetParticleBornPerFrame();

   void SetParticleSize(float *size);
   void GetParticleSize(float *size);

   void SetParticleVelocity(float *vel, float *velVar);
   void GetParticleVelocity(float *vel, float *velVar);

   void GetTextureData(char *texName);
   BOOL4 LoadTextures(char *texName);
   void GetAlphamapData(char *texName);
   BOOL4 LoadAlphamaps(char *texName);

   OBJECTid GetParticleObject();
   OBJECTid GetEmitterObject();

   int GetForceNumber();
   GAMEFXid GetForce(int iOne);
   BOOL4 ApplyForce(GAMEFXid fID);
   void RemoveForce(GAMEFXid fID);

   void SetupTextureAnimation(int num, int *uv);
   void GetTextureAnimation(int *num, int *uv);

   PARTICLE_SYSTEMid GetParticleSystem();
};


/*-----------------------------
  Force Game FX Function Class
 ------------------------------*/
class FnForceGameFX : public FnBaseGameFX
{
public:
   FnForceGameFX(DWORD m = FAILED_ID) : FnBaseGameFX(m) {};
  ~FnForceGameFX() {};

   void Initialize(DWORD type, float magnitude);
   DWORD GetForceType();
   void  SetForceType(DWORD type);
   float GetForceMagnitude();
   void  SetForceMagnitude(float f);
   float GetForceMinDistance();
   void  SetForceMinDistance(float dist);
   GAMEFXid UsedBy();
};


/*-----------------------------
  Sword Game FX Function Class
 ------------------------------*/
class FnSwordGameFX : public FnBaseGameFX
{
public:
   FnSwordGameFX(DWORD m = FAILED_ID) : FnBaseGameFX(m) {};
  ~FnSwordGameFX() {};

   BOOL4 CreateSword();
   BOOL4 GetState();
   void SetState(BOOL4 newState);
   void GetCharacterName(char *charName);
   void SetCharacterName(char *charName);
   void GetDummyObject1Name(char *dummy1Name);
   void GetDummyObject2Name(char *dummy2Name);
   OBJECTid GetDummyObject1();
   OBJECTid GetDummyObject2();
   void SetDummyObject1(OBJECTid oID1, char *dummy1Name = NULL);
   void SetDummyObject2(OBJECTid oID2, char *dummy2Name = NULL);
   int GetBladeMaxNum();
   void SetBladeMaxNum(int bladeMaxNum);
   int GetDivideNum();
   void SetDivideNum(int divideNum);
   int GetSegmentNum();
   void SetSegmentNum(int segmentNum);
   void GetTextureInfo(char *aTexName, int * nTex);
   void LoadTextures(char *aTexName, int nTex);
   void GetAlphamapInfo(char *aAlphaName);
   void LoadAlphamaps(char *aAlphaName);
   DWORD GetBlendingType();
   void SetBlendingType(DWORD type);
   void GetColor(float *fColor);
   void SetColor(float *fColor);
};


/*-----------------------------
  Audio Game FX Function Class
 ------------------------------*/
class FnAudioGameFX : public FnBaseGameFX
{
public:
   FnAudioGameFX(DWORD m = FAILED_ID) : FnBaseGameFX(m) {};
  ~FnAudioGameFX() {};

   BOOL4 CreateAudio(char * audioName);
   BOOL4 LoadAudio(char * audioName);
   void  GetAudioName(char * audioName);
   void  GetAudioStartFrame(float * startFrame);
   void  SetAudioStartFrame(float startFrame);
};


//#ifdef WIN32
/*---------------------------
  Thread Pool Function Class
 ----------------------------*/
class FnThreadPool : public FnFunction
{
public:
   FnThreadPool(DWORD m = FAILED_ID) : FnFunction(m) {};
  ~FnThreadPool() {};

   int ThreadNumber();
   int Run(LPTHREAD_START_ROUTINE pFunc, LPVOID pData, BOOL4 beHighPriority = TRUE);
   void WaitRunning(float sec);
};
//#endif

#endif //_FLY2_H_INCLUDED_
