#pragma once

#include <stdlib.h>
#include <iostream>
#include "State.h"
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>
#include <ncltech\BallAI.h>

using namespace std;

//GameObject* BallAI::AIBall;
//GameObject* Player::ball;

class StateMachine
{
public:

	StateMachine(GameObject* owner) { stateOwner = owner, currentState = nullptr, previousState = nullptr; }
	~StateMachine() {}
	State* getCurrentState() { return currentState; }
	State* getroamingState() { return roamingState; }
	void setDefaultState(State* dState) { currentState = dState; }
	void setCurrentState(State* newState)
	{
		//Vector3 ballPos = BallAI::getBall()->Physics()->GetPosition();
		//Vector3 playerPos = Player::getBall()->Physics()->GetPosition();
		// //Switch to length square

		//float distanceToPlayer = (ballPos - playerPos).Length();
		//if (distanceToPlayer < 30) 
		//{ currentState->exit(stateOwner);
		//cout << "leaving state " << currentState;
		//
		//} previousState = currentState; currentState = newState; newState->enter(stateOwner);
		//cout << "entering " << currentState;
	}
	State* getPreviousState() { return previousState; }


private:
	State * roamingState;
	GameObject* stateOwner;
	State *currentState;
	State *previousState;
};
