#define _USE_MATH_DEFINES

#include "Zombie.h"
#include "Character.h"

//#include "ZombieState.h"
#include "ZombieStates.h"
#include "sprite.h"


Zombie::Zombie()
	: Character()
, mf_timer(0)
, mf_rayAngle(0)
{
	mp_steering = new SteeringBehaviours(this);
	mp_stateMachine = new StateMachine<Zombie>(this);
	mp_stateMachine->SetCurrentState(Passive::Instance());
	mp_stateMachine->SetPreviousState(Passive::Instance());
	//mp_stateMachine->SetGlobalState(); //Set global state here

	mv_heading = b2Vec2(0, 1);
	mv_side = b2Vec2(-1, 0);
	//mv_velocity = b2Vec2(0, 0);
	mf_maxForce = 100;
	mf_maxSpeed = 8;
	mf_mass = 1;
	//mv_wanderTarget.x = mf_x + 65;
	//mv_wanderTarget.y = mf_y = 0;
	//mv_wanderTarget.x = mv_pos.x + 65;
	//mv_wanderTarget.y = mv_pos.y = 0;
	//md_wanderRadius = 20;
	//md_wanderJitter = 4;

	m_type = ZOMBIE;
}


Zombie::~Zombie()
{
	delete mp_stateMachine;
	mp_stateMachine = 0;

}

void
Zombie::Process(float deltaTime)
{
	mf_timeElapsed = deltaTime;

	mp_stateMachine->Update();
	for (b2ContactEdge* ce = mp_body->GetContactList(); ce != NULL; ce = ce->next)
	{

		Entity* tempBullet = static_cast<Entity*>(ce->other->GetUserData());
		if (tempBullet != NULL && tempBullet->GetType() == BULLET)
		{
			mb_dead = true;
			tempBullet->SetDead(true);
		}

		//TODO Check if agro state before attacking
		Entity* tempPlayer = static_cast<Entity*>(ce->other->GetUserData());
		if (tempPlayer != NULL && tempBullet->GetType() == PLAYER)
		{
			tempPlayer->SetDead(true);
		}
	}

	//calculate combined force(vector) from all steering behaviours exerted on the character.
	b2Vec2 steeringForce(mp_steering->Calculate());

	//Acceleration = Force/Mass
	b2Vec2 acceleration(steeringForce.x / mf_mass, steeringForce.y / mf_mass);

	//update velocity
	mv_velocity += deltaTime * acceleration;

	//make sure vehicle does not exceed maximum velocity
	if (mv_velocity.LengthSquared() > mf_maxSpeed)
	{
		mv_velocity.Normalize();
		mv_velocity *= mf_maxSpeed;
	}

	//// Rotation physics body why retaining position.
	//// Sprite rotation should be based on the angle of the physics body.
	//MoveToPoint(mv_velocity, mv_velocity.Length() * deltaTime);
	//Rotate(mv_heading);

	////update position

	mp_body->SetTransform(mp_body->GetPosition(), b2Atan2(mv_heading.y, mv_heading.x));
	mp_sprite->SetAngle(mp_body->GetAngle() * 180.0f / static_cast<float>(M_PI));
	mp_body->SetLinearVelocity(mv_velocity);
	mv_pos.x = static_cast<float>((mp_body->GetPosition().x * (mf_worldScaleSprite)) - (mp_sprite->GetWidth() / 2));
	mv_pos.y = static_cast<float>((mp_body->GetPosition().y * (mf_worldScaleSprite)) - (mp_sprite->GetHeight() / 2));
	mp_sprite->SetPos(mv_pos);

	//mv_pos.x -= mp_sprite->GetWidth()/2;
	//mv_pos.y -= mp_sprite->GetHeight()/2;
	//update the heading if the vehicle has a velocity greater than a very small
	//value
	if (mv_velocity.LengthSquared() > 0.00000001)
	{
		mv_heading = mv_velocity;
		mv_heading.Normalize();
		//set second params of b2Cross to 1 or -1 for perpendicular vector of first param.
		mv_side = b2Vec2(-mv_heading.y, mv_heading.x);
	}

	ZombieDetectPlayer();
	mb_playerDetected = DetectPlayer();
	//m_currentState->Execute(this);
	//UpdateState(deltaTime);
}

//void
//Zombie::ResetDirection()
//{
//	float r1 = ((float)rand() / (RAND_MAX + 1)) - ((float)rand() / (RAND_MAX + 1));
//	mv_wanderTarget.x += r1 * 360;
//
//	float r2 = ((float)rand() / (RAND_MAX + 1)) - ((float)rand() / (RAND_MAX + 1));
//	mv_wanderTarget.y += r2 * 360;
//
//	mv_wanderTarget.Normalize();
//
//	mv_wanderTarget.x *= md_wanderRadius;
//	mv_wanderTarget.y *= md_wanderRadius;
//
//	Rotate(mv_wanderTarget.x, mv_wanderTarget.y);
//}

void
Zombie::SetTimer(float timer)
{
	mf_timer = timer;
}

float
Zombie::GetTimer()
{
	return mf_timer;
}

void
Zombie::SetRayAngle(float angle)
{
	mf_rayAngle = angle;
}

float
Zombie::GetRayAngle()
{
	return mf_rayAngle;
}

void
Zombie::ChangeStateAgro()
{
	mp_stateMachine->ChangeState(Agro::Instance());
}

void
Zombie::ZombieDetectPlayer()
{
	float currentRayAngle = GetRayAngle();

	currentRayAngle += (360 / 1.0 / 60.0) * 3.14 / 180;

	//in Step() function
	SetRayAngle(currentRayAngle); //one revolution every 1 second

	//calculate points of ray
	float rayLength = 100; //long enough to hit the walls
	b2Vec2 p1 = GetPos(); //(zombie->GetPositionX(), zombie->GetPositionY()); //center of scene
	b2Vec2 p2 = p1 + rayLength * b2Vec2(sinf(currentRayAngle), cosf(currentRayAngle));

	b2RayCastInput input;
	input.p1 = p1;
	input.p2 = p2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	b2Vec2 intersectionNormal(0, 0);
	for (b2Body* b = GetWorld()->GetBodyList(); b; b = b->GetNext()) {
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {

			b2RayCastOutput output;
			if (!f->RayCast(&output, input, 0))
				continue;
			if (output.fraction < closestFraction) {
				closestFraction = output.fraction;
				intersectionNormal = output.normal;
			}
		}
	}

	b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);

	b2Vec2* t(GetSteering()->GetTarget());

	float checkX = intersectionPoint.x - GetSteering()->GetTarget()->x;
	float checkY = intersectionPoint.y - GetSteering()->GetTarget()->y;

	if (checkX <= 30 && checkX >= -30 && checkY <= 30 && checkY >= -30)
	{
		ChangeStateAgro();
	}
}

bool
Zombie::DetectPlayer()
{
	float currentRayAngle = mp_body->GetAngle();

	currentRayAngle += (360 / 1.0 / 60.0) * 3.14 / 180;

	//in Step() function
	SetRayAngle(currentRayAngle); //one revolution every 1 second
	
	//calculate points of ray
	float rayLength = 100; //long enough to hit the walls
	b2Vec2 p1 = GetPos(); //(zombie->GetPositionX(), zombie->GetPositionY()); //center of scene
	b2Vec2 p2 = p1 + rayLength * b2Vec2(sinf(currentRayAngle), cosf(currentRayAngle));

	b2RayCastInput input;
	input.p1 = p1;
	input.p2 = p2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	b2Vec2 intersectionNormal(0, 0);
	for (b2Body* b = GetWorld()->GetBodyList(); b; b = b->GetNext()) {
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {

			b2RayCastOutput output;
			if (!f->RayCast(&output, input, 0))
				continue;
			if (output.fraction < closestFraction) {
				closestFraction = output.fraction;
				intersectionNormal = output.normal;
			}
		}
	}

	b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);

	if (b2Distance(mv_pos, *GetSteering()->GetTarget()) <= 100)
	{
		if (b2Distance(mv_pos, intersectionPoint) <= rayLength)
		{
			return true;
		}
	}
	return false;
}

bool 
Zombie::PlayerDetected()
{
	return mb_playerDetected;
}
