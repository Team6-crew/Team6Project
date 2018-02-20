#pragma once

#include <stdlib.h>
#include <iostream>
#include "State.h"

class GameObject;

using namespace std;

class StateMachine
{
public:

	StateMachine(GameObject* owner) { stateOwner = owner; };
	~StateMachine() {}
	State* getCurrentState() { return currentState; }
	GameObject* getOwner(int a) { if(a == 1) return stateOwner; }
	void setDefaultState(State* dState) { currentState = dState; }
	void setCurrentState(StateMachine* sOwner, State* newState);
	void setPreviousState(State* pState) { previousState = pState; }
	State* getPreviousState() { return previousState; }

private:
	State * roamingState;
	State * chasingState;
	State * collectAndUsePickupState;
	State * currentState = nullptr;
	State * previousState;
	StateMachine* machineOwner;
	GameObject* stateOwner;
};
