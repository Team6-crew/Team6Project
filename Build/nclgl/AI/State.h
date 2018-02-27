#pragma once

#include <stdlib.h>
#include <iostream>

class StateMachine;
class GameObject;

class State
{
public:
	virtual void enter(StateMachine* sOwner) = 0;
	virtual void update(StateMachine* sOwner) = 0;
	virtual void exit(StateMachine* sOwner) = 0;

private:
};
