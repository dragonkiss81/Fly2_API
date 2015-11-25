/*========================================================================
  The Fly2 Utility Header

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0804, 2012 by Kevin C. Wang

  Last Updated : 0805, 2014
 =========================================================================*/
#if !defined(_THE_FLY2_UTIL_H_INCLUDED_)
#define _THE_FLY2_UTIL_H_INCLUDED_

#define UNIFORM_B_SPLINE                 0

#define OPEN_CURVE                       1


// a dual-head linked list element
struct FuDualListElement
{
   void *content;
   struct FuDualListElement *pre;
   struct FuDualListElement *nxt;
};


// a dual-head linked list
class FuDualLinkedList
{
protected:
   int m_nCount;              // number of objects
   FuDualListElement *m_lHead;  // dual-head linked list (head)
   FuDualListElement *m_lTail;  // dual-head linked list (tail)

public:
   FuDualLinkedList();
  ~FuDualLinkedList();

   inline FuDualListElement *GetListHead() { return m_lHead; };
   inline FuDualListElement *GetListTail() { return m_lTail; };
   inline int GetCount() { return m_nCount; };
   void AddHead(FuDualListElement *, BOOL4);
   void AddTail(FuDualListElement *, BOOL4);
   BOOL4 ListIsEmpty();
   FuDualListElement *RemoveTail();
   void RemoveAt(FuDualListElement *);
   void Push(FuDualListElement *);
   void Pop(FuDualListElement *);
   FuDualListElement *GetPosition(void *);
};


// skin weight data structure
struct FuSkinWeight
{
   int boneID;
   float weight;
};


// curve definition & data structure
struct curvesegmentS
{
   float *a;
   float *ta;
   float *ca;
   struct curvesegmentS *nxt;
};
typedef struct curvesegmentS CURVESEGMENTs, *CURVESEGMENTptr;

typedef struct fycurveS
{
   int type;
   int open;
   int degree;
   int nseg;
   int ncntl;
   int nknot;
   float *knot;
   float *cntl;
   float *pt;
   CURVESEGMENTptr abcd;
   CURVESEGMENTptr labcd;
} FYCURVEs, *FYCURVEptr;


extern void FuGetShaderNameInFile(char *shaderName, int id, char *tName);
extern int FuCheckShaderNumberInFile(char *shaderName);
extern BOOL4 FuCheckExistFile(char *fullName, char *path, char *file, char *ext, char *path1, char *path2, const char *ext1, const char *ext2, const char *ext3, const char *ext4);
extern void FuGenerateDiskWeight(float *diskArray, int radius, int inc = 1);
extern void FuApplyBlendFilter(float *destRaw, float *src, int w, int h, float *filter, int size);
extern void FuSortSkinBoneID(char *finalBone, char *srcBone, OBJECTid skinID);

extern float *FuOpenUniformBSpline3(float *pt, int nspan, int type, int np, int *nbuf, float *buf, FYCURVEptr spline = NULL);
extern float *FuOpenCatmullRomSpline3(float *preP, float *pt, int nspan, float *postP, float *keyTable, int np, float *buf);
extern void FuCleanSplineData(FYCURVEptr spline);
extern float FuFindSplineX3(float *a, float t);

extern BOOL4 FuBakeSHCoeff(float *SH, int l, int m, SCENEid sID, float *pos, BOOL4 beO = FALSE);

#endif _THE_FLY2_UTIL_H_INCLUDED_