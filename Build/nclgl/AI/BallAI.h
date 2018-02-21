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
	static GameObject* getBall() {return AIBall;}
	void move();
	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }
	float getRadius() { return rad; }
	void setRadius(float radius) { rad = radius; }
	void setRelativePosition(nclgl::Maths::Vector3 rel_pos) { relative_position = rel_pos; }
	nclgl::Maths::Vector3 getRelativePosition() { return relative_position; }

private:
	static GameObject* AIBall;
	nclgl::Maths::Vector3 goal;
	
	int CurrentNode = 0;
	std::vector<nclgl::Maths::Vector3> Nodes;
	float speed;
	float rad;
	nclgl::Maths::Vector3 relative_position;
	StateMachine* AIStateMachine;

};

