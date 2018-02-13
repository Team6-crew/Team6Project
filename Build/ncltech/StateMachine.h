#pragma once

#include <stdlib.h>
#include <iostream>
#include "State.h"
#include <ncltech\GameObject.h>


using namespace std;

class StateMachine
{
public:

	StateMachine(GameObject* owner) { stateOwner = owner, previousState = nullptr; }
	~StateMachine() {}
	State* getCurrentState() { return currentState; }
	State* getroamingState() { return roamingState; }
	void setDefaultState(State* dState) { currentState = dState; }
	void setCurrentState(State* newState)
	{
		Vector3 ballPos = BallAI::getBall()->Physics()->GetPosition();
		Vector3 playerPos = Player::getBall()->Physics()->GetPosition();
		// Switch to length square

		float distanceToPlayer = (ballPos - playerPos).Length();
		if (distanceToPlayer <= 10) 
		{ 
		cout << "leaving state  \n";
		//currentState->exit(stateOwner);
		//previousState = currentState;
		//currentState = chasingState;
		}//newState->enter(stateOwner);
		//cout << "entering " << currentState << "\n";
		if (distanceToPlayer > 11)
		{
			getCurrentState()->update(stateOwner);
			cout << "roaming around \n";
		}
	}
	State* getPreviousState() { return previousState; }


private:
	State * roamingState;
	State * chasingState;
	State *currentState;
	State *previousState;
	GameObject* stateOwner;
	

};
