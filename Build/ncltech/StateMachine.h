#pragma once

#include <stdlib.h>
#include <iostream>
#include "State.h"
#include <ncltech\GameObject.h>

class StateMachine
{
public:

	StateMachine() {}
	~StateMachine() {}
	State* getCurrentState() { return currentState; }
	void setCurrentState(State* state) { currentState = state; }
	State* getPreviousState() { return previousState; }
	void setOwnerOfStateMachine(GameObject* stateOwner) { Owner = stateOwner; }


private:

	GameObject * Owner;
	State *currentState;
	State *previousState;
	State *roamingState;
	State *chasingState;


};
