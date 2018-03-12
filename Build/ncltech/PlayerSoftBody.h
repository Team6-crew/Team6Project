#pragma once
#include <ncltech\CommonUtils.h>
#include <ncltech\Softbody.h>
#include <ncltech\Pickup.h>
#include <ncltech\Washingzone.h>


class PlayerSoftBody
{
public:
	PlayerSoftBody(const std::string& name,
		nclgl::Maths::Vector3& pos,
		float radius,
		float inverse_mass,
		nclgl::Maths::Vector4& color,
		int tg);

	~PlayerSoftBody();

	GameObject* getBody() { return body; }
	Softbody* getBall() { return ball; }
	GameObject* getTop();
	GameObject* getBottom();
	GameObject* getFront();

	
	void setRadius(float radius) { rad = radius; }
	float getRadius() { return rad; }
	
	void setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys space, KeyboardKeys shoot);

	void setCamera(Camera* c) { camera = c; }
	Camera* getCamera() { return camera; }
	void resetCamera(float dt);

	bool collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }

	nclgl::Maths::Vector4 getColour() { return colour; }
	

	float getadd_rad() { return add_rad; }
	void setadd_rad(float add) { add_rad = add; }

	void setRelativePosition(nclgl::Maths::Vector3 rel_pos) { relative_position = rel_pos; }
	nclgl::Maths::Vector3 getRelativePosition() { return relative_position; }

	void shoot();
	void equipStunWeapon(nclgl::Maths::Vector4 colour);
	void equipPaintWeapon(nclgl::Maths::Vector4 colour);

	void setStunDuration(float st) {
		stunDuration = st;
		tempYaw = camera->GetYaw();
		tempPitch = camera->GetPitch();
	}

	void increaseSensitivity(float dt) {
		sensitivity += dt * 5;
		if (sensitivity > 2.0f) sensitivity = 2.0f;
	}

	void decreaseSensitivity(float dt) {
		sensitivity -= dt * 5;
		if (sensitivity < -2.0f) sensitivity = -2.0f;
	}

	bool setcanpaint(bool canp) { canpaint = canp; return canpaint; }
	bool getcanpaint() { return canpaint; }

	void settime(float t) { time = t; }
	float gettime() { return time; }

	bool stun(float dt);

	int getDebuffTime() { return debuffTime; }
	void setDebuffTime(int t) { debuffTime = t; }

	float getBuffTime() { return buffTime; }
	void setBuffTime(float t) { 
		buffTime = t;
		currentBuffTime = 0.0f;
	}

	float getCurrentBuffTime() { return currentBuffTime; }
	void setCurrentBuffTime(float t) { currentBuffTime = t; }

	void setCurrentBuff(Tags tag);
	Tags getCurrentBuff() { return currentBuff; }


	void move(float dt);

	void speedLimit();
	void wallLimit();
	void jumpSlow();
	int getIndex() { return index; }
	void setIndex(int idx) { index = idx; }

private:
	void handleInput(float dt);
	void unequipPaintWeapon();
	void unequipStunWeapon();

	void updateBuffTime(float dt);

	Softbody * ball;
	GameObject* body;

	KeyboardKeys move_up, move_down, move_left, move_right, move_jump, move_shoot;

	GameObject* top; // Top most sphere used to control soft body
	GameObject* bottom; // Bottom most sphere for movement and painting
	GameObject* front; // Front sphere
	GameObject* back; // You get the idea

	Camera* camera;
	RenderNodeBase* camera_transform;
	RenderNodeBase* bodyRenderNode;
	RenderNodeBase* equippedStunWeapon;
	RenderNodeBase* equippedPaintWeapon;
	
	float speed;


	float add_rad;
	float rad;

	nclgl::Maths::Vector3 relative_position;
	nclgl::Maths::Vector3 forward;

	float time;
	int tag;

	float sensitivity;
	bool canpaint;

	bool canjump = 1;
	float stunDuration;
	bool stunEffect;
	float tempYaw, tempPitch;
	int debuffTime;
	float buffTime;
	float currentBuffTime;

	Tags currentBuff;

	nclgl::Maths::Vector4 colour;
	int index;
};
