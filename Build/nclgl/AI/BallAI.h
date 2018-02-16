#pragma once
#include <ncltech\CommonUtils.h>
#include <vector>
#include "Messenger.h"

class StateMachine;

class BallAI
{
public:
	BallAI();
	~BallAI();
	std::vector<nclgl::Maths::Vector3> getNodes() { return Nodes; }
	void setNodes(nclgl::Maths::Vector3 Node) { Nodes.push_back(Node); }
	nclgl::Maths::Vector3 seek(nclgl::Maths::Vector3 TargetPos);
	nclgl::Maths::Vector3 followPath();
	void chasePlayer();
	static GameObject* getBall() {return AIBall;}

	void move();

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }

private:
	static GameObject* AIBall;
	nclgl::Maths::Vector3 goal;
	int CurrentNode = 0;
	std::vector<nclgl::Maths::Vector3> Nodes;
	float speed;
	StateMachine* AIStateMachine;
};

