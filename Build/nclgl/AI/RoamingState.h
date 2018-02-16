#pragma once

//#include "Messenger.h"
#include "iostream"
#include "fstream"
#include "State.h"

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
	virtual void enter(StateMachine* sOwner, GameObject* owner);
	virtual void update(StateMachine* sOwner, GameObject* owner);
	virtual void exit(StateMachine* sOwner, GameObject* owner)
	{
		std::cout << "Exiting Roaming State \n";
	};
	void addNodesToList(nclgl::Maths::Vector3 Node) { nodesList.push_back(Node); }
	std::vector<nclgl::Maths::Vector3> getNodes() { return nodesList; }
	nclgl::Maths::Vector3 seek(GameObject* owner, nclgl::Maths::Vector3 TargetPos);
	nclgl::Maths::Vector3 followPath(StateMachine* sOwner, GameObject* owner);

private:
	State * chasingState;
	int CurrentNode = 0;
	nclgl::Maths::Vector3 goal;
	std::vector<nclgl::Maths::Vector3> nodesList;
};

