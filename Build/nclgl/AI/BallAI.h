#pragma once
#include <ncltech\CommonUtils.h>
#include <vector>
#include "Messenger.h"
#include <ncltech\Pickup.h>
#include <nclgl\MapNavigation.h>
#include <ncltech\Memory Management\MemoryManager.h>

class StateMachine;

class BallAI : public GameObject
{
	DECLARE_HEAP;
public:
	BallAI(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~BallAI();
	GameObject* getBody() { return AIbody; }
	nclgl::Maths::Vector4 getColour() { return colour; }
	void setColour(nclgl::Maths::Vector4 a) { colour = a; }
	static void addBallAIPlayers(int i);
	std::vector<nclgl::Maths::Vector3> getNodeList() { return nodesList; }
	nclgl::Maths::Vector3 getNode(int a) { return nodesList[a]; }
	void addNodesToList(nclgl::Maths::Vector3 Node) { nodesList.push_back(Node); }
	GameObject* getBall() { return this; }
	void move(float dt);
	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }
	float getRadius() { return rad; }
	void setRadius(float radius) { rad = radius; }
	float getadd_rad() { return add_rad; }
	void setadd_rad(float add) { add_rad = add; }
	void setRelativePosition(nclgl::Maths::Vector3 rel_pos) { relative_position = rel_pos; }
	nclgl::Maths::Vector3 getRelativePosition() { return relative_position; }
	bool BallAI::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);
	void setStateMachine(StateMachine* a) { AIStateMachine = a; }
	StateMachine* getStateMachine() { return AIStateMachine; }
	void setMapNavigation(MapNavigation* a) { MapNav = a; }
	MapNavigation* getMapNavigation() { return MapNav; }
	int getCurrentNode() { return currentNode; }
	void increaseCurrentNode() { currentNode = (currentNode + 1) % nodesList.size(); }
	void resetCurrentNode() { currentNode = 0; }

	void equipStunWeapon(nclgl::Maths::Vector4 colour);
	void equipPaintWeapon(nclgl::Maths::Vector4 colour);
	int getDebuffTime() { return debuffTime; }
	void setDebuffTime(int t) { debuffTime = t; }
	void setBuffTime(float t) {
		buffTime = t;
		currentBuffTime = 0.0f;
	}

	float getCurrentBuffTime() { return currentBuffTime; }
	void setCurrentBuffTime(float t) { currentBuffTime = t; }

	void setCurrentBuff(Tags tag);
	Tags getCurrentBuff() { return currentBuff; }

	void updateBuffTime(float dt);
	void shoot();

	void setStunDuration(float st) {
		stunDuration = st;
	}
	bool stun(float dt);

	nclgl::Maths::Vector3 getForward() { return forward; }

	bool setcanpaint(bool canp) { canPaint = canp; return canPaint; }
	bool getcanpaint() { return canPaint; }

	void settime(float t) { time = t; }
	float gettime() { return time; }

	void setscore(float s) { score = s; }
	float getscore() { return score; }
private:
	GameObject * AIbody;
	nclgl::Maths::Vector4 colour;
	nclgl::Maths::Vector3 goal;
	int currentNode = 0;
	std::vector<nclgl::Maths::Vector3> nodesList;

	nclgl::Maths::Vector3 forward;

	nclgl::Maths::Vector3 relative_position;
	std::vector<StateMachine*> stateMachines;
	StateMachine * AIStateMachine;
	MapNavigation * MapNav;

	RenderNodeBase* bodyRenderNode;
	RenderNodeBase* equippedStunWeapon;
	RenderNodeBase* equippedPaintWeapon;

	void unequipPaintWeapon();
	void unequipStunWeapon();

	float stunDuration;
	bool stunEffect;
	float time;
	float sensitivity;
	float speed;
	float rad;
	float add_rad;
	int debuffTime;
	float buffTime;
	float currentBuffTime;
	bool canShoot;
	bool canPaint;
	Tags currentBuff;
	float score;
};

