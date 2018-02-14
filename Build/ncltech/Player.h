#pragma once
#include <ncltech\CommonUtils.h>
#include <ncltech\GameObject.h>
#include <ncltech\Pickup.h>

class Pickup;
class Player : public GameObject
{
public:

	Player(const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const Vector4& color);

	~Player();

	GameObject* getBody() { return body; }



	void move();
	void setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump);

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

	KeyboardKeys move_up, move_down, move_left, move_right , move_jump;
	
	
};

