#pragma once
#include <ncltech\CommonUtils.h>
#include <vector>
class BallAI
{
public:
	BallAI();
	~BallAI();
	enum deceleration {
		slow = 3, normal = 2, fast = 1
	};

	std::vector<Vector3> getNodes() { return Nodes; }
	void setNodes(Vector3 Node) { Nodes.push_back(Node); }
	Vector3 seek(Vector3 TargetPos);
	Vector3 followPath();
	void chasePlayer();
	GameObject* getBall(int a) { if (a = 1){return AIBall;}  }

	void move();

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }

private:
	Vector3 goal;
	int CurrentNode = 0;
	std::vector<Vector3> Nodes;
	GameObject * AIBall;
	float speed;

};

