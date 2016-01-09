/*========================================================  
  The Media : Streaming Data Playback and Control

  - Using DirectShow

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0220, 2013 by Chuan-Chang Wang

  Last Updated : 0220, 2013
 =========================================================*/

#if !defined(_FLY2_STREAM_H_INCLUDED_)
#define _FLY2_STREAM_H_INCLUDED_

#include "FyMedia.h"


class iFSyMedia
{
protected:
   int m_nID;                         // ID
   FILE *m_pFile;                     // the media file
   DWORD m_nType;                     // media type
   BOOL4 m_bPlay;                     // be playing
   int m_fOx, m_fOy, m_fW, m_fH;      // size of rendering area
   float m_fVolume;                   // volume size
   int m_nDDKID;                      // DDK ID
   BOOL4 m_bBeAutoPlay;               // be rendered usign DShow ?

public:
   iFSyMedia(BOOL4); 
  ~iFSyMedia();

   inline void ID(int id) { m_nID = id; };      // set ID
   inline int ID() { return m_nID; };           // get ID
   BOOL4 Open(char *);                          // open a media file by name
   BOOL4 Play(int);                             // play media
   BOOL4 Stop();                                // stop playing
   BOOL4 Pause();                               // pause playing
   inline BOOL4 IsPlaying() { return m_bPlay; } // check if playing
   void SetDrawRectangle(int, int, int, int);   // set playing rectangle range
   BOOL4 CheckMediaEvent();                     // check media event
   DWORD GetState();                            // get playing states
   void SetVolume(float);                       // set volume size

   int QueryCurrentBufferSize();                // get current buffer size
};

extern MEDIAid FYRegisterMedia(iFSyMedia *m);
extern void FYUnregisterMedia(iFSyMedia *m);
extern iFSyMedia *FYGetMedia(MEDIAid im);

#endif //_FLY2_STREAM_H_INCLUDED_
