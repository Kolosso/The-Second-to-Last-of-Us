#define _USE_MATH_DEFINES

#include "SteeringBehaviours.h"
#include "Zombie.h"
#include "Tile.h"
#include <vector>
#include <math.h>


SteeringBehaviours::SteeringBehaviours(Character* character)
: mp_character(character)
, mf_wanderRadius(30)
, mf_wanderDistance(30)
, mf_wanderJitter(40)
, mi_flags(0)
, mp_targetCharacter1(nullptr)
, mp_targetCharacter2(nullptr)
, mv_target(nullptr)
, mf_weightWander(1.0)
, mf_weightSeek(1.0)
, mf_weightFlee(1.0)
, mf_weightArrive(1.0)
, mf_weightPursuit(1.0)
, mf_weightEvade(0.01)
, mf_weightWallAvoidance(10.0)
, m_deceleration(NORAML)
, m_feelers(3)
, mf_wallDetectionFeelerLength(10)
{
	double theta = static_cast<float>(rand()) / (RAND_MAX + 1);
	mv_wanderTarget = b2Vec2(mf_wanderRadius * cos(theta),
	                         mf_wanderRadius * sin(theta));
	mv_lastTarget = b2Vec2(0, 0);
}

SteeringBehaviours::~SteeringBehaviours()
{
}

b2Vec2
SteeringBehaviours::Calculate()
{
	mv_steeringForce.SetZero();
	mv_steeringForce = CalculatePrioritized();
	return mv_steeringForce;
}

void 
SteeringBehaviours::SetTarget(b2Vec2* target)
{
	mv_target = target;
}

b2Vec2* SteeringBehaviours::GetTarget()
{
	return mv_target;
}

void
SteeringBehaviours::SetLastTarget(b2Vec2 target)
{
	mv_lastTarget = target;
}

b2Vec2
SteeringBehaviours::GetLastTarget()
{
	return mv_lastTarget;
}

bool 
SteeringBehaviours::On(behavior_type bt)
{
	return (mi_flags & bt) == bt;
}

bool
SteeringBehaviours::AccumulateForce(b2Vec2 &steeringForce, b2Vec2 forceToAdd)
{
	float magnitudeSoFar = steeringForce.Length();
	float magnitudeRemaining = mp_character->GetMaxForce() - magnitudeSoFar;
	if (magnitudeRemaining <= 0.0)
	{
		return false;
	}
	float magnitudeToAdd = forceToAdd.Length();
	if (magnitudeToAdd < magnitudeRemaining)
	{
		steeringForce += forceToAdd;
	}
	else
	{
		forceToAdd.Normalize();
		steeringForce += magnitudeRemaining * forceToAdd;
	}
	return true;
}

float
SteeringBehaviours::RandomClamped()
{
	return static_cast<float>(rand()) / (RAND_MAX + 1.0) - static_cast<float>(rand()) / (RAND_MAX + 1.0);
}

b2Vec2
SteeringBehaviours::Seek(b2Vec2 TargetPos)
{
	b2Vec2 temp(TargetPos - mp_character->GetPos());
	temp.Normalize();
	b2Vec2 desiredVelocity = mp_character->GetMaxSpeed() * temp;
	return (desiredVelocity - mp_character->GetVelocity());
}

b2Vec2
SteeringBehaviours::Flee(b2Vec2 TargetPos)
{
	//only flee if the target is within 'panic distance'. Work in distance
	//squared space.
	const float panicDistanceSquared = 100.0 * 100.0;
	if (b2DistanceSquared(mp_character->GetPos(), TargetPos) > panicDistanceSquared)
	{
		return b2Vec2(0, 0);
	}

	b2Vec2 temp(mp_character->GetPos() - TargetPos);
	temp.Normalize();
	b2Vec2 desiredVelocity = mp_character->GetMaxSpeed() * temp;
	return (desiredVelocity - mp_character->GetVelocity());
}

b2Vec2
SteeringBehaviours::Arrive(b2Vec2 TargetPos, Deceleration deceleration)
{
	b2Vec2 toTarget(TargetPos - mp_character->GetPos());
	float dist = toTarget.Length();
	if (dist > 0)
	{
		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration.
		const float decelerationTweaker = 0.3f;

		float speed = dist / static_cast<float>(deceleration) * decelerationTweaker;

		speed = b2Min(speed, mp_character->GetMaxSpeed());

		b2Vec2 desiredVelocity(speed / dist * toTarget);
		return (desiredVelocity - mp_character->GetVelocity());
	}
	return b2Vec2(0, 0);
}

b2Vec2
SteeringBehaviours::Pursuit(Character* evader)
{
	b2Vec2 toEvader = evader->GetPos() - mp_character->GetPos();
	float relativeHeading = b2Dot(mp_character->GetHeading(), evader->GetHeading());
	if (b2Dot(toEvader, mp_character->GetHeading()) > 0 &&
		(relativeHeading < -0.95))
	{
		return Seek(evader->GetPos());
	}
	float lookAheadTime = toEvader.Length() / (evader->GetVelocity().Length() + mp_character->GetMaxSpeed());
	return Seek(evader->GetPos() + lookAheadTime * evader->GetVelocity());
}

b2Vec2
SteeringBehaviours::Evade(Character* pursuer)
{
	b2Vec2 toPursuer = pursuer->GetPos() - mp_character->GetPos();
	float lookAheadTime = toPursuer.Length() / (pursuer->GetVelocity().Length() + mp_character->GetMaxSpeed());
	return Seek(pursuer->GetPos() + lookAheadTime * pursuer->GetVelocity());
}

b2Vec2
SteeringBehaviours::Wander()
{
	mv_wanderTarget += b2Vec2(mf_wanderJitter * RandomClamped(),
		mf_wanderJitter * RandomClamped());

	//reproject this new vector back onto a unit circle
	mv_wanderTarget.Normalize();

	//increase the length of the vector to the same as the radius
	//of the wander circle
	mv_wanderTarget *= mf_wanderRadius;

	//move the target into a position WanderDist in front of the agent
	b2Vec2 targetLocal(mv_wanderTarget + b2Vec2(mf_wanderDistance, 0));

	//project the target into world space
	b2Vec2 targetWorld(mp_character->GetBox2DBody()->GetWorldPoint(targetLocal));

	return targetWorld - mp_character->GetBox2DBody()->GetPosition();
}

#define __SEONG_TEST__

#ifdef __SEONG_TEST__
b2Vec2
SteeringBehaviours::WallAvoidance()
{
	CreateFeelers();

	b2Vec2 steeringForce(0, 0);

	std::vector<b2Vec2> intersectionPoints;
	b2Vec2 p1(mp_character->GetBox2DBody()->GetPosition());

	float closestFraction = 1;
	b2Vec2 intersectionNormal(0, 0);

	for (int feeler = 0; feeler < m_feelers.size(); ++feeler)
	{
		b2Vec2 p2(p1 + 10 * m_feelers[feeler]);
		b2RayCastInput input;
		input.p1 = p1;
		input.p2 = p2;
		input.maxFraction = 1;
		for (b2Body* b = mp_character->GetWorld()->GetBodyList(); b; b = b->GetNext())
		{
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				if (f->GetFilterData().categoryBits == 0x0001)
				{
					b2RayCastOutput output;
					if (!f->RayCast(&output, input, 0))
					{
						continue;
					}
					if (output.fraction < closestFraction)
					{
						closestFraction = output.fraction;
						intersectionNormal = output.normal;
					}
				}
			}
		}

		b2Vec2 intersectionPoint(p1 + closestFraction * (p2 - p1));
		intersectionPoints.push_back(intersectionPoint);

		b2Vec2 normalEnd(intersectionPoint + intersectionNormal);

		steeringForce += normalEnd;
	}
	return steeringForce;
}

void
SteeringBehaviours::CreateFeelers()
{
	m_feelers[0] = mp_character->GetBox2DBody()->GetPosition() + mf_wallDetectionFeelerLength * mp_character->GetHeading();

	b2Vec2 temp(b2Mul(b2Transform(mp_character->GetBox2DBody()->GetPosition(), b2Rot(M_PI / 2 * 3.5f)), mp_character->GetHeading()));
	m_feelers[1] = mp_character->GetBox2DBody()->GetPosition() + mf_wallDetectionFeelerLength / 2.0f * temp;

	temp = b2Mul(b2Transform(mp_character->GetBox2DBody()->GetPosition(), b2Rot(M_PI / 2 * 0.5f)), mp_character->GetHeading());
	m_feelers[2] = mp_character->GetBox2DBody()->GetPosition() + mf_wallDetectionFeelerLength / 2.0f * temp;
}
#endif __SEONG_TEST__

b2Vec2
SteeringBehaviours::CalculatePrioritized()
{ 
	b2Vec2 force;
	if (On(WALL_AVOIDANCE))
	{
		force = mf_weightWallAvoidance * WallAvoidance();
		if (!AccumulateForce(mv_steeringForce, force))
		{
			return mv_steeringForce;
		}
	}
	//if (On(OBSTACLE_AVOIDANCE))
	//{
	//	//force = ObstacleAvoidance(mp_zombie->GetWorld()->GetObstacles()) *
	//	//	mf_MultObstacleAvoidance;
	//	//if (!AccumulateForce(mv_steeringForce, force))
	//	//{
	//	//	return mv_steeringForce;
	//	//}
	//}
	//if (On(EVADE))
	//{
	//	assert(mp_targetCharacter1 && "Evade target not assigned");
	//	force = mf_weightEvade * Evade(mp_targetCharacter1);
	//	if (!AccumulateForce(mv_steeringForce, force))
	//	{
	//		return mv_steeringForce;
	//	}
	//}
	//if (On(FLEE))
	//{
	//	force = mf_weightFlee * Flee(mp_character->GetWorld()->GetCrosshair());
	//	if (!AccumulateForce(mv_steeringForce, force))
	//	{
	//		return mv_steeringForce;
	//	}
	//}
	//if (On(SEPARATION))
	//{
	//	force = Separation(mp_zombie->GetWorld()->GetEntities()) *
	//		mf_MultSeparation;
	//	if (!AccumulateForce(mv_steeringForce, force))
	//	{
	//		return mv_steeringForce;
	//	}
	//}
	if (On(SEEK))
	{
		force = Seek(*mv_target);
			if (!AccumulateForce(mv_steeringForce, force))
			{
				return mv_steeringForce;
			}
	}
	if (On(ARRIVE))
	{
		force = mf_weightArrive * Arrive(mv_lastTarget, m_deceleration);
			if (!AccumulateForce(mv_steeringForce, force))
			{
				return mv_steeringForce;
			}
	}
	if (On(WANDER))
	{
		force = mf_weightWander * Wander();
		if (!AccumulateForce(mv_steeringForce, force))
		{
			return mv_steeringForce;
		}
	}
	//if (On(PURSUIT))
	//{
	//	assert(mp_targetCharacter1 && "pursuit target not assigned");
	//	force = mf_weightPursuit * Pursuit(mp_targetCharacter1);
	//	if (!AccumulateForce(mv_steeringForce, force))
	//	{
	//		return mv_steeringForce;
	//	}
	//}

	return mv_steeringForce;
}

void
SteeringBehaviours::WanderOn()
{
	mi_flags |= WANDER;
}

void
SteeringBehaviours::WanderOff()
{
	mi_flags ^= WANDER;
}

void 
SteeringBehaviours::SeekOn()
{
	mi_flags |= SEEK;
}

void
SteeringBehaviours::SeekOff()
{
	mi_flags ^= SEEK;
}

void
SteeringBehaviours::ArriveOn()
{
	mi_flags |= ARRIVE;
}

void
SteeringBehaviours::ArriveOff()
{
	mi_flags ^= ARRIVE;
}

void
SteeringBehaviours::WallAvoidanceOn()
{
	mi_flags |= WALL_AVOIDANCE;
}

void
SteeringBehaviours::WallAvoidanceOff()
{
	mi_flags ^= WALL_AVOIDANCE;
}
