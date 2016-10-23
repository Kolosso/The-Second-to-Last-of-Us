#ifndef __Zombie_H__
#define __Zombie_H__

#include "Character.h"
#include "StateMachine.h"

class Zombie :
	public Character
{
	// Member Methods:
public:
	Zombie();
	~Zombie();

	void Process(float deltaTime);

	//void ResetDirection();
	//void Wander();

	void SetTimer(float timer);
	float GetTimer();
	void SetRayAngle(float angle);
	float GetRayAngle();

	//void SetTargetVec(float x, float y);
	//float GetTargetX();
	//float GetTargetY();

	void ChangeStateAgro();
	void ZombieDetectPlayer();
	bool DetectPlayer();
	bool PlayerDetected();
protected:

	
	/*void UpdateState(float deltaTime);

	void PassiveBehaviour();
	void AlertBehaviour(float deltaTime);
	void AgroBehaviour();
	void DistractedBehaviour(float deltaTime);*/

private:
	Zombie(const Zombie& Zombie);
	Zombie& operator = (const Zombie& Zombie);

	// Member Data:
public:
	StateMachine<Zombie>* mp_stateMachine;
	//ZombieState m_currentState;


	//double md_wanderRadius;
	//double md_wanderDistance;
	//double md_wanderJitter;

	//b2Vec2 mv_wanderTarget;
protected:
	//float mf_targetX;
	//float mf_targetY;

	float mf_timer;
	
	float mf_rayAngle;
	bool mb_playerDetected;
	float mf_timeElapsed;
	
private:

};

#endif //__Zombie_H__