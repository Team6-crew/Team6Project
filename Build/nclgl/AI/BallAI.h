#pragma once
#include <ncltech\CommonUtils.h>
#include <vector>
#include "Messenger.h"
#include <nclgl\MapNavigation.h>

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
	std::vector<nclgl::Maths::Vector3> getNodeList() { return nodesList; }
	nclgl::Maths::Vector3 getNode(int a) { return nodesList[a]; }
	void addNodesToList(nclgl::Maths::Vector3 Node) { nodesList.push_back(Node); }
	GameObject* getBall() { return this; }
	void move();
	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }
	float getRadius() { return rad; }
	void setRadius(float radius) { rad = radius; }
	void setRelativePosition(nclgl::Maths::Vector3 rel_pos) { relative_position = rel_pos; }
	nclgl::Maths::Vector3 getRelativePosition() { return relative_position; }
	bool BallAI::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);
	void setStateMachine(StateMachine* a) { AIStateMachine = a; }
	StateMachine* getStateMachine() { return AIStateMachine; }
	void setMapNavigation(MapNavigation* a) { MapNav = a; }
	MapNavigation* getMapNavigation() { return MapNav; }
	int getCurrentNode() { return currentNode; }
	void increaseCurrentNode() { currentNode = (currentNode + 1) % nodesList.size();; }
	void resetCurrentNode() { currentNode = 0; }


private:
	nclgl::Maths::Vector3 goal;
	int currentNode = 0;
	std::vector<nclgl::Maths::Vector3> nodesList;
	float speed;
	float rad;
	nclgl::Maths::Vector3 relative_position;
	std::vector<StateMachine*> stateMachines;
	StateMachine * AIStateMachine;
	MapNavigation * MapNav;
	
};

