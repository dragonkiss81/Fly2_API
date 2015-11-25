/*==============================================================
  Fly2 Computer Vision Extension

  (C)2014-2015 BlackSmith Technology, All Rights Reserved
  Created : 0806, 2014 by Chuan-Chang Wang

  Last Updated : 0806, 2014
 ===============================================================*/
#if !defined(_FLYCV_H_INCLUDED_)
#define _FLYCV_H_INCLUDED_

#include "Fly.h"


// a SIFT feature (key point) data structure
struct FSIFTFeature
{
   int octave;           // octave index
   int ix;               // keypoint integer X coordinate (unnormalized)
   int iy;               // keypoint integer Y coordinate (unnormalized)
   int is;               // keypoint integer scale indiex

   int w;

   float x;              // keypoint fractional X coordinate
   float y;              // keypoint fractional Y coordinate
   float s;              // keypoint fractional scale index

   float sigma;          // keypoint scale

   int numAngle;         // number of orientations
   float angle[4];       // orientation(s) of a keypoint

   int group;            // group ID
   int matchPrev;        // matched ID in previous frame
   int anglePrev[2];     // matched angle ID to/in previous frame
   int matchNext;        // matched ID in next frame
   int angleNext[2];     // matched angle ID to/in next frame
   float *descriptor[4]; // the descriptors, length of each = nDim*nDim*nHist
};


extern BOOL4 FyCVStart(int w, int h);
extern void FyCVEnd(void);
extern float *FyCVGetMonoImage(TEXTUREid tID, float *mono);
extern int FyCVMinEigenCornerFeaturesCPU(float *mono, int w, int h, FSIFTFeature *features, int num,
                                         float sigma = 2.0f, float preSmooth = 1.0f, float threshold = 0.005f,
                                         BOOL4 beDescriptor = FALSE);
extern void FyCVSIFTCPUDescriptor(FSIFTFeature *keys, int numKP, float **octaves, int firstOctave, float *gradient,
                                  int nDimD, int nBinsHist, int oW, int oH, int groupID = FY_NONE);
extern void FyCVSmoothFP32WithGaussian(float *dest, float *src, int width, int height, float sigma);
extern float FySIFTDescriptorSSD(FSIFTFeature *f1, int iAngle1, FSIFTFeature *f2, int iAngle2);

#endif
