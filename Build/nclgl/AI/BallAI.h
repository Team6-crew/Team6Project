#pragma once
#include <ncltech\CommonUtils.h>
#include <vector>
#include "Messenger.h"

class StateMachine;

class BallAI : public GameObject
{
public:
	BallAI(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~BallAI();
	static void addBallAIPlayers(int numBallAI);
	std::vector<nclgl::Maths::Vector3> getNodes() { return Nodes; }
	void setNodes(nclgl::Maths::Vector3 Node) { Nodes.push_back(Node); }
	GameObject* getBall() { return this; }
	void move();
	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }
	float getRadius() { return rad; }
	void setRadius(float radius) { rad = radius; }
	void setRelativePosition(nclgl::Maths::Vector3 rel_pos) { relative_position = rel_pos; }
	nclgl::Maths::Vector3 getRelativePosition() { return relative_position; }
	bool BallAI::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);
	StateMachine* AIStateMachine;

private:
	nclgl::Maths::Vector3 goal;
	int CurrentNode = 0;
	std::vector<nclgl::Maths::Vector3> Nodes;
	float speed;
	float rad;
	nclgl::Maths::Vector3 relative_position;
	
};

