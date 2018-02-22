#pragma once
#include <ncltech\CommonUtils.h>
#include <ncltech\GameObject.h>
#include <ncltech\Pickup.h>

#include <ncltech\Memory Management\MemoryManager.h>

class Pickup;
class Player : public GameObject
{
	DECLARE_HEAP;
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


	void move(float dt);
	void setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump);

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }
	void equipWeapon();

	bool collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);

	nclgl::Maths::Vector3 getRelativePosition() { return relative_position; 	}

	void setRelativePosition(nclgl::Maths::Vector3 rel_pos) {relative_position = rel_pos;	}

	float getRadius() { return rad; }

	void setRadius(float radius) { rad = radius; }

	void setCamera(Camera* c) { camera = c; }

	void increaseSensitivity(float dt) {
		sensitivity += dt * 4;
		if (sensitivity > 3.0f) sensitivity = 3.0f;
	}

	void decreaseSensitivity(float dt) {
		sensitivity -= dt * 4;
		if (sensitivity < -3.0f) sensitivity = -3.0f;
	}

	void resetCamera(float dt) {
		if (sensitivity > 0) {
			sensitivity -= dt * 6;
			if (sensitivity < 0.0f) sensitivity = 0.0f;
		}
		else if (sensitivity < 0) {
			sensitivity += dt * 6;
			if (sensitivity > 0.0f) sensitivity = 0.0f;
		}

	}

	Camera* getCamera() { return camera; }
private:

	GameObject* body;
	GameObject* cube;

	RenderNodeBase* camera_transform;
	RenderNodeBase* equippedItem;
	Camera* camera;
	nclgl::Maths::Vector3 relative_position;
	float speed;

	bool canjump = 1;

	KeyboardKeys move_up, move_down, move_left, move_right , move_jump;

	float sensitivity;
	
	float rad;
};

