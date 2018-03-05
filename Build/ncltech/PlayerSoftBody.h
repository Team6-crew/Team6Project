#pragma once
#include <ncltech\CommonUtils.h>
#include <ncltech\Softbody.h>


class PlayerSoftBody
{
public:
	PlayerSoftBody(const std::string& name,
		nclgl::Maths::Vector3& pos,
		float radius,
		float inverse_mass,
		nclgl::Maths::Vector4& color);

	~PlayerSoftBody();

	GameObject* getBody() { return body; }
	Softbody* getBall() { return ball; }
	GameObject* getTop();
	GameObject* getBottom();
	GameObject* getFront();

	void move();
	void setRadius(float radius) { rad = radius; }
	float getRadius() { return rad; }
	void setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys space, KeyboardKeys shoot);

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }

	void setCamera(Camera* c) { camera = c; }
	Camera* getCamera() { return camera; }

	float getadd_rad() { return add_rad; }
	void setadd_rad(float add) { add_rad = add; }

	void setRelativePosition(nclgl::Maths::Vector3 rel_pos) { relative_position = rel_pos; }
	nclgl::Maths::Vector3 getRelativePosition() { return relative_position; }

private:

	Softbody * ball;
	GameObject* body;
	RenderNodeBase* camera_transform;
	Camera* camera;
	GameObject* top; // Top most sphere used to control soft body
	GameObject* bottom; // Bottom most sphere for movement and painting
	GameObject* front;
	GameObject* back;

	float speed;

	bool canjump = 1;

	KeyboardKeys move_up, move_down, move_left, move_right, move_jump, move_shoot;
	float add_rad;
	float rad;
	nclgl::Maths::Vector3 relative_position;
};
