#ifndef __ZOMBIESTATES_H__
#define __ZOMBIESTATES_H__

#include "State.h"
#include "Zombie.h"

// Forward Declarations:
class Zombie;

//All states should be added below.

/**
 * Passive State
 */
class Passive
	:public State<Zombie>
{
	//Member Methods
public:
	static Passive* Instance();

	virtual void Enter(Zombie* zombie);
	virtual void Execute(Zombie* zombie);
	virtual void Exit(Zombie* zombie);

protected:

private:
	Passive(){}

	Passive(const Passive&);
	Passive& operator=(const Passive&);
};

/**
 * Alerted
 */
class Alerted
	:public State<Zombie>
{
	//Member Methods
public:
	static Alerted* Instance();

	virtual void Enter(Zombie* zombie);
	virtual void Execute(Zombie* zombie);
	virtual void Exit(Zombie* zombie);
protected:

private:
	Alerted(){}

	Alerted(const Alerted&);
	Alerted& operator=(const Alerted&);
};

/**
 * Agro
 */
class Agro
	:public State<Zombie>
{
	//Member Methods
public:
	static Agro* Instance();

	virtual void Enter(Zombie* zombie);
	virtual void Execute(Zombie* zombie);
	virtual void Exit(Zombie* zombie);
protected:

private:
	Agro(){}

	Agro(const Agro&);
	Agro& operator=(const Agro&);
};

/**
 * Distracted
 */
class Distracted
	:public State<Zombie>
{
	//Member Methods
public:
	static Distracted* Instance();

	virtual void Enter(Zombie* zombie);
	virtual void Execute(Zombie* zombie);
	virtual void Exit(Zombie* zombie);
protected:

private:
	Distracted(){}

	Distracted(const Distracted&);
	Distracted& operator=(const Distracted&);
};

/**
* Arrive
*/
class Arrive
	:public State<Zombie>
{
	//Member Methods
public:
	static Arrive* Instance();

	virtual void Enter(Zombie* zombie);
	virtual void Execute(Zombie* zombie);
	virtual void Exit(Zombie* zombie);
protected:

private:
	Arrive(){}

	Arrive(const Arrive&);
	Arrive& operator=(const Arrive&);
};
#endif //__ZOMBIESTATES_H__