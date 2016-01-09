/*========================================================================
  Fly SDK Version 2 - Header  Build 1004 (Since 01/16, 2013)
  
  The Media : Streaming Data Playback and Control using DirectShow (on PC)

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0220, 2013 by Chuan-Chang Wang

  Last Updated : 0220, 2013
 =========================================================================*/
#if !defined(_FLY2_MEDIA_H_INCLUDED_)
#define _FLY2_MEDIA_H_INCLUDED_

#include "Fly.h"

#define MEDIA_UNKNOWN                               0
#define MEDIA_STOPPED                               1
#define MEDIA_PAUSED                                2
#define MEDIA_RUNNING                               3

typedef unsigned int MEDIAid;                       // media object ID


/*----------------------------
  Media Player Function Class
 -----------------------------*/
class FnMedia : public FnFunction
{
public:
   FnMedia() {};
  ~FnMedia() {};

   BOOL4 Play(int type);
   BOOL4 Stop();
   BOOL4 Pause();
   DWORD GetState();
   void SetVolume(float);
   BOOL4 IsPlaying();

   int QueryBufferSize();
};


extern BOOL4 FyBeginMedia(const char *path, int num);
extern void FyEndMedia();
extern MEDIAid FyCreateMediaPlayer(char *fileName, int ox, int oy, int w, int h, BOOL4 beAutoPlay = TRUE);
extern void FyDeleteMediaPlayer(MEDIAid mID);

#endif //_FLY2_MEDIA_H_INCLUDED_