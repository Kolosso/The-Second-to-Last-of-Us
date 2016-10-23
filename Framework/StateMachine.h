#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__
#include "State.h"

template <class character_type>
class StateMachine
{
	//Member Methods
public:
	StateMachine(character_type* owner):mp_owner(owner),
										mp_currentState(0),
										mp_previousState(0),
										mp_globalState(0)
	{}

	void SetCurrentState(State<character_type>* state)
	{
		mp_currentState = state;
	}

	void SetGlobalState(State<character_type>* state)
	{
		mp_globalState = state;
	}

	void SetPreviousState(State<character_type>* state)
	{
		mp_previousState = state;
	}
	
	void Update() const
	{
		if (mp_globalState)
		{
			mp_globalState->Execute(mp_owner);
		}

		if (mp_currentState)
		{
			mp_currentState->Execute(mp_owner);
		}
	}

	void ChangeState(State<character_type>* p_NewState)
	{
		assert(p_NewState &&
			"<StateMachine::ChangeState>: trying to change to a null state");
		mp_previousState = mp_currentState;
		mp_currentState->Exit(mp_owner);
		mp_currentState = p_NewState;
		mp_currentState->Enter(mp_owner);
	}

	void RevertToPreviousState()
	{
		ChangeState(mp_previousState);
	}

	State<character_type>* GetCurrentState() const
	{
		return mp_currentState;
	}

	State<character_type>* GetGlobalState() const
	{
		return mp_globalState;
	}

	State<character_type>* GetPreviousState() const
	{
		return mp_previousState;

	}
protected:

private:

	//Member Data
public:
	bool isInState(const State<character_type>& state)const;

protected:

private:
	//pointer to owner of instance
	character_type* mp_owner;
	State<character_type>* mp_currentState;
	State<character_type>* mp_previousState;
	State<character_type>* mp_globalState;
};

#endif __STATEMACHINE_H__