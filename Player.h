#pragma once
#include <ncltech\CommonUtils.h>

class Player
{
public:
	Player();
	~Player();

	GameObject* getBody() { return body; }
	GameObject* getBall() { return ball; }

	void move();
	void setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump);

	float getSpeed() { return speed; }
	void setSpeed(float sp) { speed = sp; }

	//bool CallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);

private:

	GameObject* ball;
	GameObject* body;
	RenderNode* camera_transform;
	Camera* camera;

	float speed;
	bool canjump = 1;

	KeyboardKeys move_up, move_down, move_left, move_right,move_jump;
	
	
};

