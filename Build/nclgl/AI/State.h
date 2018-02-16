#pragma once

#include <stdlib.h>
#include <iostream>

class StateMachine;
class GameObject;

class State
{
public:
	virtual void enter(StateMachine* sOwner,GameObject* owner) = 0;
	virtual void update(StateMachine* sOwner, GameObject* owner) = 0;
	virtual void exit(StateMachine* sOwner, GameObject* owner) = 0;

private:
};
