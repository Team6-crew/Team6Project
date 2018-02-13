#pragma once
#include <ncltech\CommonUtils.h>
#include <vector>
//#include <ncltech\StateMachine.h>
//#include <ncltech\State.h>

class BallAI
{
public:
	BallAI();
	~BallAI();

	std::vector<Vector3> getNodes() { return Nodes; }
	void setNodes(Vector3 Node) { Nodes.push_back(Node); }
	Vector3 seek(Vector3 TargetPos);
	Vector3 followPath();
	void chasePlayer();
	static GameObject* getBall() {return AIBall;}  

	void move();

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }

private:
	static GameObject* AIBall;
	Vector3 goal;
	int CurrentNode = 0;
	std::vector<Vector3> Nodes;
	float speed;

};

