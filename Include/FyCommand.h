/*========================================================================
  Fly2 - Command Management

  (C)2012-2015 BlackSmith Technology, All Rights Reserved
  Created : 0619, 2012 by Chuan-Chang Wang

  Last Updated : 0619, 2012
 =========================================================================*/
#if !defined(_THE_FLY2_COMMAND_H_INCLUDED_)
#define _THE_FLY2_COMMAND_H_INCLUDED_

#include "Fly.h"


class FcCommandBase
{
private:
   BOOL4 m_bBeDone;                // Be done ?
   BOOL4 m_bCanBeUndo;             // Can be undo ?
   BOOL4 m_b1stDo;                 // be 1st do ?
   char m_aName[64];              // command name
   char m_aArgument[256];         // argument
   FcCommandBase *m_pNextCommand; // next command

public:
   FcCommandBase() { m_bBeDone = FALSE;
                     m_bCanBeUndo = TRUE;
                     m_b1stDo = TRUE;
                     m_aName[0] = '\0';
                     m_aArgument[0] = '\0';
                     m_pNextCommand = NULL; };

   virtual ~FcCommandBase() { };

   inline void NextCommand(FcCommandBase *cmd) { m_pNextCommand = cmd; };
   inline FcCommandBase *NextCommand() { return m_pNextCommand; };
   inline void BeDone(BOOL4 beDone) { m_bBeDone = beDone; };
   inline BOOL4 IsDone() { return m_bBeDone; };
   inline void BeUndo(BOOL4 beDone) { m_bCanBeUndo = beDone; };
   inline BOOL4 IsUndo() { return m_bCanBeUndo; };
   inline char *GetCommandName() { return m_aName; };
   inline void SetCommandName(char *name) { strcpy(m_aName, name); };
   inline void SetArgument(char *string) { strcpy(m_aArgument, string); };
   inline char *GetArgument() { return m_aArgument; };
   inline BOOL4 Is1stDo() { return m_b1stDo; };
   inline void Be1stDo(BOOL4 beDo) { m_b1stDo = beDo; };

   virtual void Init() { };
   virtual void DeInit() { };
   virtual BOOL4 Do(char *arg) { return FALSE; };
   virtual void Undo() { };
};


void FcSetupCommandQueue(int maxCmd);
void FcDeleteCommand(int iNum);
BOOL4 FcDoCommand(FcCommandBase *cmd, char *argument = NULL, BOOL4 beNoUndo = FALSE, BOOL4 beClearCmd = FALSE);
BOOL4 FcRedoCommand(char *cmdName, BOOL4 beTest = FALSE);
BOOL4 FcUndoLastCommand(char *cmdName, BOOL4 beTest = FALSE);
FcCommandBase *FcQueryCommand(int backPtr);
void FcClearCommand(void);

#endif ////_THE_FLY2_COMMAND_H_INCLUDED_
