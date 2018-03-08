#pragma once
#include <ncltech\GameObject.h>
#include <nclgl\AI\StateMachine.h>

class AIObject : public GameObject
{
public:
	StateMachine * getStateMachine() { return sOwner; }
	StateMachine * sOwner;
private:
	
};
