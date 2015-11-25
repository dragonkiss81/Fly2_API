/*========================================================================
  Fly2 - Job Management

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0328, 2012 by Chuan-Chang Wang

  Last Updated : 1220, 2012
 =========================================================================*/
#if !defined(_THE_FLY_JOB_H_INCLUDED_)
#define _THE_FLY_JOB_H_INCLUDED_

//#include "FlyWin32.h"

#define UNLIMIT             -1.0f

class FjJobBase
{
private:
   int m_nID;             // job ID in job table
   float m_fLife;         // job's life
   BOOL4 m_bInitDo;       // do the job once when initializing ? or not
   FjJobBase *m_pNextJob; // next job

public:
   FjJobBase() { m_nID = FY_NONE;
                 m_fLife = UNLIMIT;
                 m_pNextJob = NULL;
                 m_bInitDo = FALSE; };

  ~FjJobBase() { };

   inline int ID() { return m_nID; };
   inline void ID(int id) { m_nID = id; };
   inline void NextJob(FjJobBase *job) { m_pNextJob = job; };
   inline FjJobBase *NextJob() { return m_pNextJob; };
   inline BOOL4 CheckInitDo() { return m_bInitDo; };
   inline void SetInitDo(BOOL4 b) { m_bInitDo = b; };

   void Life(float);
   void AddLife(float);
   void CheckLife();

   virtual void Init() { };
   virtual void DeInit() { };
   virtual void Do(int skip) { };
};


extern void FjUseJobSystem(int);
extern int FjRegisterJob(FjJobBase *);
extern void FjUnregisterJob(FjJobBase *);
extern void FjDoJobs(int);
extern BOOL4 FjCheckJobSystem();

#endif //_THE_FLY_JOB_H_INCLUDED_