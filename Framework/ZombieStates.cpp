#include "ZombieStates.h"
#include <iostream>

//Passive
Passive* Passive::Instance()
{
	static Passive instance;
	return &instance;
}

void 
Passive::Enter(Zombie* zombie)
{
	std::cout << "Entering Passive State" << std::endl;
	zombie->GetSteering()->WanderOn();
	//zombie->ResetDirection();
}

void 
Passive::Execute(Zombie* zombie)
{
	//std::cout << "Executing Passive State" << std::endl;
	zombie->ZombieDetectPlayer();
	//zombie->Wander();

}

void 
Passive::Exit(Zombie* zombie)
{
	std::cout << "Exiting Passive State" << std::endl;
	zombie->GetSteering()->WanderOff();
}

//Alerted
Alerted* Alerted::Instance()
{
	static Alerted instance;
	return &instance;
}

void 
Alerted::Enter(Zombie* zombie)
{
	zombie->SetTimer(0);
}

void 
Alerted::Execute(Zombie* zombie)
{
	
}

void 
Alerted::Exit(Zombie* zombie)
{
	
}

//Agro
Agro* Agro::Instance()
{
	static Agro instance;
	return &instance;
}

void 
Agro::Enter(Zombie* zombie)
{
	zombie->GetSteering()->SeekOn();
	std::cout << "Entering Agro State" << std::endl;
}

void 
Agro::Execute(Zombie* zombie)
{
	//if (!zombie->PlayerDetected())
	//{
	//	zombie->GetSteering()->SetLastTarget(*zombie->GetSteering()->GetTarget());
	//	zombie->mp_stateMachine->ChangeState(Arrive::Instance());
	//}

	if (b2Distance(zombie->GetPos(), *zombie->GetSteering()->GetTarget()) > 300)
	{
		zombie->mp_stateMachine->ChangeState(Passive::Instance());
	}
	//zombie->Move(zombie->GetTargetX(), zombie->GetTargetY());
	//zombie->Rotate(zombie->GetTargetX(), zombie->GetTargetY());
}

void 
Agro::Exit(Zombie* zombie)
{
	zombie->GetSteering()->SeekOff();
	std::cout << "Exitting Agro State" << std::endl;
}

//Distracted
Distracted* Distracted::Instance()
{
	static Distracted instance;
	return &instance;
}

void 
Distracted::Enter(Zombie* zombie)
{
	
}

void 
Distracted::Execute(Zombie* zombie)
{
	
}

void 
Distracted::Exit(Zombie* zombie)
{
	
}

//Arrive
Arrive* Arrive::Instance()
{
	static Arrive instance;
	return &instance;
}

void
Arrive::Enter(Zombie* zombie)
{
	std::cout << "Entering Arrive State" << std::endl;
	zombie->GetSteering()->ArriveOn();
	zombie->GetSteering()->SetLastTarget(*zombie->GetSteering()->GetTarget());
}

void
Arrive::Execute(Zombie* zombie)
{
	if (b2Distance(zombie->GetPos(), zombie->GetSteering()->GetLastTarget()) < 50)
	{
		zombie->mp_stateMachine->ChangeState(Passive::Instance());
	}
}

void
Arrive::Exit(Zombie* zombie)
{
	std::cout << "Exiting Arrive State" << std::endl;
	zombie->GetSteering()->ArriveOff();
}