/*============================================================
  The Fly 2 SDK - the memory management system header

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0328, 2012 by Chuan-Chang Wang

  Last Updated : 0328, 2012
 =============================================================*/

#if !defined(_FLY2_MEMORY_H_INCLUDED_)
#define _FLY2_MEMORY_H_INCLUDED_

extern BOOL4 FyBeginMemorySystem(int numBlock, float *blockLen, float tmpBlockLen = 5.0f, float defBlockLen = 5.0f);
extern void FyEndMemorySystem();
extern void *FyMalloc(int len);
extern void FyFree(void *ptr);
extern void *FyRealloc(void *ptr, int len);

extern void *FyAllocateBuffer(int len);
extern void FyReleaseBuffer(void *ptr);
extern void *FyReallocateBuffer(void *ptr, int len);

extern void *FyMemCopy(void *dest, void *src, int len);
extern void FyStringCopy(char *dest, const char *src);
extern void FySetMemoryValue(void *dest, int value, int size);

#endif //_FLY2_MEMORY_H_INCLUDED_

