#pragma once
#include "nclgl\AI\State.h"
#include "ncltech\Player.h"
#include <ncltech\GameObject.h>
#include <iostream>
#include <nclgl\AI\State.h>

class GameObject;
class StateMachine;


class CollectandUsePickupState: public State
{
public:
	static State * GetInstance()
	{
		static CollectandUsePickupState theInstance;
		return &theInstance;
	}

public:
	virtual void enter(StateMachine* sOwner, GameObject* owner);
	virtual void update(StateMachine* sOwner, GameObject* owner);
	virtual void exit(StateMachine* sOwner, GameObject* owner);
private:
	float maxVel = 20;
};


