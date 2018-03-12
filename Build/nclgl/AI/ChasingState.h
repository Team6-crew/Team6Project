#pragma once
#include "State.h"
#include "Messenger.h"
#include "ncltech\Player.h"
#include <ncltech\GameObject.h>
#include <iostream>
class StateMachine;
class GameObject;
class ChasingState : public State
{
public:
	static State * GetInstance()
	{
		static ChasingState theInstance;
		return &theInstance;
	}

public:
	virtual void enter(StateMachine* sOwner);
	virtual void update(StateMachine* sOwner);
	virtual void exit(StateMachine* sOwner);
private:
	float maxVel = 20;

	//State* collectAndUsePickupState;
	State* roamingState;
};


