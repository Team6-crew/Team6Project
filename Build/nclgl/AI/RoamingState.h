#pragma once

//#include "Messenger.h"
#include "iostream"
#include "fstream"
#include "State.h"
#include "BallAI.h"
#include "StateMachine.h"

#include <iostream>
#include <nclgl\Vector3.h>
#include <vector>

class GameObject;
class StateMachine;

class RoamingState : public State
{
	public:
	static State * GetInstance()
	{
		static RoamingState theInstance;
		return &theInstance;
	}
public:
	float maxVel = 20.0f;
	virtual void enter(StateMachine* sOwner);
	virtual void update(StateMachine* sOwner);
	virtual void exit(StateMachine* sOwner)
	{
				std::cout << "Exiting Roaming State \n";
	};
	nclgl::Maths::Vector3 seek(StateMachine* sOwner, nclgl::Maths::Vector3 TargetPos);
	nclgl::Maths::Vector3 followPath(StateMachine* sOwner);

private:
	State * chasingState;
};

