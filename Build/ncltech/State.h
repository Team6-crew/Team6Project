#pragma once

#include <stdlib.h>
#include <iostream>
#include <ncltech\Player.h>
#include <ncltech\BallAI.h>


class State
{
public:
	void updateState( GameObject*);


private:
	State* currentState;
	State* previousState;
	State* roamingState;
	State* chasingState;
};
