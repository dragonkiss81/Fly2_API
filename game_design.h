#define SHORTDIST 400
#define LONGDIST 500
#define HEIGHTBOUND 500
#define HEIGHTOFFSET 70
#define ACTORPROBE 5
#define CAMERAPROBE 5
#define MAXCAMANGLE -0.990
#define RUNSPEED 15.0f
#define CAMERASPEED 10.0f
#define CALLIBRATION 1.0f
#define SMOOTHINESS 16
#define NUM_OF_BADGUYS 10
#define NUM_OF_BOSS 2 

#define HATKRANGE 100
#define NATK1RANGE 100
#define NATK2RANGE 150

#define HATKOFFSET 100
#define NATK1OFFSET 10 
#define NATK2OFFSET 100

#define HATKDAMAGE 100
#define NATK1DAMAGE 200
#define NATK2DAMAGE 150

class ACTNUM {
public:
	ACTNUM();
	CHARACTERid actorID;
	int blood_total;
	int blood_remain;
	int mana_total;
	int mana_remain;
}; 

ACTNUM::ACTNUM() {
	blood_total = 1024;
	blood_remain = 1024;
	mana_total = 1024;
	mana_remain = 1024;
}

void ActorGen(FnScene scene, ROOMid terrainRoomID, ACTNUM &actsystem, char* actor_name, char* action, float* pos, float* fDir, float* uDir)
{
	FnCharacter cur_actor;
	ACTIONid cur_actor_idleID, cur_actor_curPoseID; 

	actsystem.actorID = scene.LoadCharacter(actor_name);

	cur_actor.ID(actsystem.actorID);
	cur_actor.SetDirection(fDir, uDir);

	cur_actor.SetTerrainRoom(terrainRoomID, 10.0f);
	cur_actor.PutOnTerrain(pos);

	// Get two character actions pre-defined at Lyubu2
	cur_actor_idleID = cur_actor.GetBodyAction(NULL, action);

	// set the character to idle action
	cur_actor_curPoseID = cur_actor_idleID;
	cur_actor.SetCurrentAction(NULL, 0, cur_actor_curPoseID);
	cur_actor.Play(LOOP, 0.0f, FALSE, TRUE);
}

void ActAction(ACTNUM &actsystem, char* act, int damage_num)
{
	FnCharacter cur_actor;
	ACTIONid cur_actor_idleID, cur_actor_curPoseID; // two actions
	cur_actor.ID(actsystem.actorID);

	actsystem.blood_remain = actsystem.blood_remain - damage_num;

	if (actsystem.blood_remain > 0 )
	{
		if(act == "Donzo") act = "idle";
		cur_actor_curPoseID = cur_actor.GetBodyAction(NULL, act);
	}
	else
	{
		if(act != "Donzo")
		{
			float fDir[3], uDir[3];
			fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
			uDir[0] = 1.0f; uDir[1] = 1.0f; uDir[2] = 0.0f;
			cur_actor.SetDirection(fDir, uDir);
		}
		cur_actor_curPoseID = cur_actor.GetBodyAction(NULL, "Die");
	}
	cur_actor.SetCurrentAction(NULL, 0, cur_actor_curPoseID);
}


