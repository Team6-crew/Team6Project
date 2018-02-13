#pragma once
#include "State.h"

class ChasingState : public State
{
public:
	static State * GetInstance()
	{
		static ChasingState theInstance;
		return &theInstance;
	}
public:
	virtual void enter(GameObject* owner) {};
	virtual void update(GameObject* owner) {};
	virtual void exit(GameObject* owner) {};
private:
};


