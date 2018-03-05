#pragma once
#include <ncltech\CommonUtils.h>
#include <ncltech\GameObject.h>
#include <ncltech\Pickup.h>

#include <ncltech\Memory Management\MemoryManager.h>

#include <ncltech\Washingzone.h>
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
	void shoot();
	
	float getSpeed() { return speed; }
	nclgl::Maths::Vector3 getRelativePosition() { return relative_position; }
	float getRadius() { return rad; }

	void equipStunWeapon(nclgl::Maths::Vector4 colour);
	void equipPaintWeapon(nclgl::Maths::Vector4 colour);

	bool collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);

	void setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump, KeyboardKeys shoot);
	void setRelativePosition(nclgl::Maths::Vector3 rel_pos) { relative_position = rel_pos; }
	void setRadius(float radius) { rad = radius; }
	void setCamera(Camera* c) { camera = c; }
	void setStunDuration(float st) { stunDuration = st;
		tempYaw = camera->GetYaw();
		tempPitch = camera->GetPitch();
	}
	void setSpeed(float sp) { speed = sp; }

	void increaseSensitivity(float dt) {
		sensitivity += dt * 5;
		if (sensitivity > 2.0f) sensitivity = 2.0f;
	}

	void decreaseSensitivity(float dt) {
		sensitivity -= dt * 5;
		if (sensitivity < -2.0f) sensitivity = -2.0f;
	}

	void resetCamera(float dt);
	bool getcanpaint() { return canpaint; }
	float gettime() { return time; }
	Camera* getCamera() { return camera; }

	void handleInput(float dt);
	float getadd_rad() { return add_rad; }
	void setadd_rad(float add) { add_rad = add; }
	bool setcanpaint(bool canp) { canpaint = canp; return canpaint; }
	void settime(float t) { time = t; }

	void setscore(float s) { score = s; }
	float getscore() { return score; }

	bool stun(float dt);

	int getDebuffTime() { return debuffTime; }
	void setDebuffTime(int t) { debuffTime = t; }
private:

	GameObject* body;
	RenderNodeBase* bodyRenderNode;
	GameObject* cube;

	RenderNodeBase* camera_transform;
	RenderNodeBase* equippedStunWeapon;
	RenderNodeBase* equippedPaintWeapon;
	Camera* camera;
	nclgl::Maths::Vector3 relative_position;
	float speed;
	nclgl::Maths::Vector3 forward;
	bool canjump = 1;

	KeyboardKeys move_up, move_down, move_left, move_right , move_jump, move_shoot;
	float time;
	float score;

	float sensitivity;
	bool canpaint;
	
	float rad;
	float stunDuration;
	bool stunEffect;
	float tempYaw, tempPitch;
	float add_rad;
	int debuffTime;
};

