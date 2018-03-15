#pragma once

#include <stdlib.h>
#include <iostream>
#include "State.h"
#include <ncltech\Memory Management\MemoryManager.h>

class GameObject;

using namespace std;

class StateMachine
{
DECLARE_HEAP;
public:

	StateMachine(GameObject* owner) { stateOwner = owner; };
	~StateMachine() {}
	State* getCurrentState() { return currentState; }
	GameObject* getOwner() { return stateOwner; }
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
