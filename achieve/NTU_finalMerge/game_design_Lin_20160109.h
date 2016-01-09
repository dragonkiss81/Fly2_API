#include <vector>
#include <math.h>

// Distance section
#define SHORTDIST 500
#define LONGDIST 600
#define HEIGHTBOUND 500
#define HEIGHTOFFSET 70

// Camera related
#define MAXCAMANGLE -0.990
#define CAMERASPEED 10.0f
#define CALLIBRATION 1.0f
#define CAMERAPROBE 5

// Actor behaviors
#define SMOOTHINESS 16
#define TURNSPEED 2
#define RUNSPEED 15.0f
#define ACTORPROBE 5

// Population related
#define NUM_OF_BADGUYS 35
#define NUM_OF_BOSS 1
#define NUM_OF_GENERATOR 7

// Attack attributes

#define HATKRANGE 100
#define NATK1RANGE 100
#define NATK2RANGE 150

#define HATKOFFSET 100
#define NATK1OFFSET 10 
#define NATK2OFFSET 100

#define HATKDAMAGE 100
#define NATK1DAMAGE 200
#define NATK2DAMAGE 150

// Enemies
#define GUY_SPEED 3.0f
#define GUY_ATTACK_DIST 60.0f
#define GUY_SCOPE 1000
#define SEP_WEIGHT 60.0f
#define GUY_ATTACK 30
#define GUY_HEAVY_ATTACK 100

// IDtypes
#define LYUBU 0
#define DONZO 1
#define ROBBER 2
#define AMA001 3
#define AMA002 4
#define AMA003 5
#define AMA004 6
#define AMA005 7
#define AMA006 8

class ProduceBadguys;
class ACTNUM;
int nextlevel_exp(int level);
int HpMp(int level);
void levelup(ACTNUM &actsystem);
int abilities(int level);
void badguylevelup(int level, ACTNUM &actsystem);

class ProduceBadguys{

public:
	//Create bad guys
	void produce(float* pos, float* uDir, FnScene scene, int generatorNUM, int totalgenNUM, int level);
	//Generation attributes
	void react();
	float range = 100.0;
	int in;
	int double_in;

};

class ACTNUM {
public:
	ACTNUM(){
		blood_total = 1000;
		blood_remain = 1000;
		mana_total = 1000;
		mana_remain = 1000;
		attack = 20;
		level = 1;
		exp_total = nextlevel_exp(level);
		exp_cur = 0;
		idtype = 0;
	};
	void reset();
	CHARACTERid actorID;
	int blood_total;
	int blood_remain;
	int mana_total;
	int mana_remain;
	int attack;
	int exp_total;
	int exp_cur;
	int level;
	int idtype;
	int alive = 0; // To control the population of bad guys
};

void ACTNUM::reset()
{
	blood_total = 500;
	blood_remain = 500;
	mana_total = 500;
	mana_remain = 500;
	attack = 20;
	level = 1;
	exp_total = nextlevel_exp(level);
	exp_cur = 0;
	idtype = 0;
}


void computeSeparation(ACTNUM meGuy, std::vector<ACTNUM> otherGuy, float* vec)
{
	FnCharacter meActor;
	FnCharacter otherActor;
	float mePos[3];
	float otherPos[3];

	meActor.ID(meGuy.actorID);
	meActor.GetPosition(mePos);

	int neighborCount = 0;
	for (int i = 0; i < otherGuy.size(); i++)
	{
		if (meGuy.actorID != otherGuy[i].actorID)
		{
			otherActor.ID(otherGuy[i].actorID);
			otherActor.GetPosition(otherPos);

			if (FyDistance(mePos, otherPos) < GUY_SCOPE)
			{
				vec[0] += (otherPos[0] - mePos[0]);
				vec[1] += (otherPos[1] - mePos[1]);
				neighborCount++;
			}
		}
	}

	if (neighborCount != 0)
	{
		//vec[0] /= neighborCount;
		//vec[1] /= neighborCount;
		vec[0] *= -1;
		vec[1] *= -1;

		float len = sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
		vec[0] = vec[0] / len;
		vec[1] = vec[1] / len;
	}
}

bool MoveToTargetLocation(ACTNUM &chase, ACTNUM &target, std::vector<ACTNUM> &otherGuy, FnObject terrain){

	FnCharacter actorChase, actorTarget;

	actorChase.ID(chase.actorID);
	actorTarget.ID(target.actorID);

	float chPos[3], chFDir[3], chUDir[3];
	float tarPos[3], tarFDir[3], tarUDir[3];
	float newFDir[3];
	float probePos[3];
	float probeDir[3];
	float separation[2] = { 0 };

	actorChase.GetPosition(chPos);
	actorTarget.GetPosition(tarPos);
	actorChase.GetDirection(chFDir, chUDir);
	actorTarget.GetDirection(tarFDir, tarUDir);

	computeSeparation(chase, otherGuy, separation);

	newFDir[0] = tarPos[0] - chPos[0] + separation[0] * SEP_WEIGHT;
	newFDir[1] = tarPos[1] - chPos[1] + separation[1] * SEP_WEIGHT;
	newFDir[2] = 0;

	probePos[0] = chPos[0];
	probePos[1] = chPos[1];
	probePos[2] = chPos[2] + HEIGHTOFFSET;// Need a height to probe

	probeDir[0] = chFDir[0] / ACTORPROBE;
	probeDir[1] = chFDir[1] / ACTORPROBE;
	probeDir[2] = -1.0f;

	actorChase.SetDirection(newFDir, chUDir);

	if (terrain.HitTest(probePos, probeDir) > 0 && FyDistance(chPos, tarPos) > GUY_ATTACK_DIST)
	{
		actorChase.MoveForward(GUY_SPEED, FALSE, FALSE, 0, FALSE);
		return 1;
	}
	
	else if (terrain.HitTest(probePos, probeDir) <= 0)
	{
		probeDir[0] = -chFDir[1] / ACTORPROBE;
		probeDir[1] = chFDir[0] / ACTORPROBE;
		if (terrain.HitTest(probePos, probeDir) > 0)
		{
			actorChase.MoveRight(-GUY_SPEED, FALSE, FALSE, 0, FALSE);
		}
		else
		{
			probeDir[0] = chFDir[1] / ACTORPROBE;
			probeDir[1] = -chFDir[0] / ACTORPROBE;
			if (terrain.HitTest(probePos, probeDir) > 0)
			{
				actorChase.MoveRight(GUY_SPEED, FALSE, FALSE, 0, FALSE);
			}
		}
		return 1;
	}
	
	else
		return 0;
}

void ActorGen(FnScene scene, ROOMid terrainRoomID, ACTNUM &actsystem, char* actor_name, char* action, float* pos, float* fDir, float* uDir)
{
	FnCharacter cur_actor;
	ACTIONid cur_actor_idleID, cur_actor_curPoseID;

	actsystem.actorID = scene.LoadCharacter(actor_name);
	actsystem.alive = 1;

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

BOOL ActAction(ACTNUM &actsystem, char* act, int damage_num)
{
	FnCharacter cur_actor;
	ACTIONid cur_actor_idleID, cur_actor_curPoseID; // two actions
	cur_actor.ID(actsystem.actorID);

	if (actsystem.alive)
		actsystem.blood_remain = actsystem.blood_remain - damage_num;

	if (actsystem.blood_remain > 0)
	{
		if (act == "Donzo")
			act = "idle";
		cur_actor_curPoseID = cur_actor.GetBodyAction(NULL, act);
		cur_actor.SetCurrentAction(NULL, 0, cur_actor_curPoseID);
		return 1; // Still alive
	}
	else
	{
		if (act = "Donzo")
		{
			float fDir[3], uDir[3];
			fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
			uDir[0] = 1.0f; uDir[1] = 1.0f; uDir[2] = 0.0f;
			cur_actor.SetDirection(fDir, uDir);
		}
		cur_actor_curPoseID = cur_actor.GetBodyAction(NULL, "Die");
		cur_actor.SetCurrentAction(NULL, 0, cur_actor_curPoseID);
		actsystem.alive = 0;
		return 0; // Dead
	}
}

BOOL InArea(float* position, float* central, float radius)
{
	if (abs(position[0] - central[0]) < radius && abs(position[1] - central[1]) < radius)
		return 1;
	else
		return 0;
}

void SetValues(float* variable, float x_val, float y_val, float z_val)
{
	variable[0] = x_val;
	variable[1] = y_val;
	variable[2] = z_val;
}

int nextlevel_exp(int level)
{	
	int stage = level / 5;
	float gap_factor = 1 + 0.15 * (stage);
	float expon = 1.5 - (float)(level - 1) / 150;
	int nxt = 100 * pow(gap_factor, 2) * pow(expon, level);
	return nxt - nxt % (int)pow(10, level/10 + 1);
}

BOOL chk_levelup(ACTNUM &actsystem)
{
	if (actsystem.exp_cur >= actsystem.exp_total){
		levelup(actsystem);
		return 1;
	}
	else
		return 0;
}

void levelup(ACTNUM &actsystem)
{
	actsystem.exp_cur -= actsystem.exp_total;
	actsystem.level++;
	actsystem.exp_total = nextlevel_exp(actsystem.level);
	actsystem.blood_total = HpMp(actsystem.level);
	actsystem.blood_remain = HpMp(actsystem.level);
	actsystem.mana_total = HpMp(actsystem.level);
	actsystem.mana_remain = HpMp(actsystem.level);
	actsystem.attack = abilities(actsystem.level);
}

int HpMp(int level)
{
	return pow(level + 10, 3) / 4 + 667;
}

int abilities(int level)
{
	return 20 + 10 * level * log10(level);
}

void badguylevelup(int level, ACTNUM &actsystem)
{
	actsystem.blood_total = 500 + 200 * level;
	actsystem.blood_remain = 500 + 200 * level;
	actsystem.attack = 10 + 10 * level;
	actsystem.exp_cur = 50 * (1 + 0.2 * level) + 40 * 2 * ((level - 1) / 5);
}


char* get_monster_act(ACTNUM &actsystem)
{
	switch (actsystem.idtype)
	{
		case DONZO:
			return "Donzo";
			break;
		case ROBBER:
			return "Damage1";
			break;
		case AMA001:
			return "AMA001";
			break;
		default:
			break;
	}
}

char* get_monster_atk(ACTNUM &actsystem)
{
	switch (actsystem.idtype)
	{
	case DONZO:
		return "HeavyAttack";
		break;
	case ROBBER:
		return "NormalAttack2";
		break;
	case AMA001:
		return "Attack";
		break;
	default:
		return "Attack";
		break;
	}
}