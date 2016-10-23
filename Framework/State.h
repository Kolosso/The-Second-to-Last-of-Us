#ifndef __STATE_H__
#define __STATE_H__
template <class character_type>
class State
{
public:
	virtual void Enter(character_type*) = 0;
	virtual void Execute(character_type*) = 0;
	virtual void Exit(character_type*) = 0;
	virtual ~State(){}
};

#endif //__STATE_H__