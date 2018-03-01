#pragma once
#include <ncltech\CommonUtils.h>
#include <ncltech\GameObject.h>
#include <ncltech\Pickup.h>

class Pickup;
class Player : public GameObject
{
public:

	Player(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);

	~Player();

	GameObject* getBody() { return body; }



	void move();

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }

	bool collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);

	

private:

	GameObject* body;
	GameObject* cube;

	RenderNodeBase* camera_transform;
	Camera* camera;

	float speed;

	bool canjump = 1;

	
	
};

