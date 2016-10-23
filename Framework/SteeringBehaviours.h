#ifndef __STEERINGBEHAVIOURS_H__
#define __STEERINGBEHAVIOURS_H__

#include <Box2D/Common/b2Math.h>
#include "Tile.h"
#include <vector>

class Character;

typedef enum
{
	SLOW = 3,
	NORAML = 2,
	FAST = 1
}Deceleration;

typedef enum
{
	NONE = 0x00000,
	SEEK = 0x00002,
	FLEE = 0x00004,
	ARRIVE = 0x00008,
	WANDER = 0x00010,
	COHESION = 0x00020,
	SEPARATION = 0x00040,
	ALLIGNMENT = 0x00080,
	OBSTACLE_AVOIDANCE = 0x00100,
	WALL_AVOIDANCE = 0x00200,
	FOLLOW_PATH = 0x00400,
	PURSUIT = 0x00800,
	EVADE = 0x01000,
	INTERPOSE = 0x02000,
	HIDE = 0x04000,
	FLOCK = 0x08000,
	OFFSET_PURSUIT = 0x10000,
}behavior_type;

class SteeringBehaviours
{
	//Member methods
public:
	//SteeringBehaviours();
	SteeringBehaviours(Character* character);
	b2Vec2 CalculatePrioritized();
	void WanderOn();
	void WanderOff();
	void SeekOn();
	void SeekOff();
	void ArriveOn();
	void ArriveOff();
	void WallAvoidanceOn();
	void WallAvoidanceOff();
	b2Vec2 Seek(b2Vec2 TargetPos);
	b2Vec2 Flee(b2Vec2 TargetPos);
	b2Vec2 Arrive(b2Vec2 TargetPos, Deceleration deceleration);
	b2Vec2 Pursuit(Character* evader);
	b2Vec2 Evade(Character* pursuer);
	b2Vec2 Wander();
	float RandomClamped();
	b2Vec2 WallAvoidance();
	void CreateFeelers();
	b2Vec2 Calculate();
	void SetTarget(b2Vec2* target);
	b2Vec2* GetTarget();
	void SetLastTarget(b2Vec2 target);
	b2Vec2 GetLastTarget();
	~SteeringBehaviours();
protected:

private:
	bool On(behavior_type bt);
	bool AccumulateForce(b2Vec2& steeringForce, b2Vec2 forceToAdd);
	//Member Data
public:

protected:

private:

	Character* mp_character;
	float mf_wanderRadius;
	float mf_wanderDistance;
	float mf_wanderJitter;
	b2Vec2 mv_wanderTarget;
	int mi_flags;
	b2Vec2 mv_steeringForce;

	//keep track of friends, pursuers, or prey
	Character* mp_targetCharacter1;
	Character* mp_targetCharacter2;
	//current target
	b2Vec2* mv_target;
	b2Vec2 mv_lastTarget;

	float mf_weightWander;
	float mf_weightSeek;
	float mf_weightFlee;
	float mf_weightArrive;
	float mf_weightPursuit;
	float mf_weightEvade; 
	float mf_weightWallAvoidance;
	Deceleration m_deceleration;

	std::vector<b2Vec2> m_feelers;
	float mf_wallDetectionFeelerLength;
};

#endif //__STEERINGBEHAVIOURS_H__