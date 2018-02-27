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
	GameObject* getControl();

	void move();
	void setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys space);

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }

	void setCamera(Camera* c) { camera = c; }

	Camera* getCamera() { return camera; }

private:

	Softbody * ball;
	GameObject* body;
	RenderNodeBase* camera_transform;
	Camera* camera;
	GameObject* control;
	GameObject* oppcontrol;

	float speed;

	bool canjump = 1;

	KeyboardKeys move_up, move_down, move_left, move_right, move_jump;


};
