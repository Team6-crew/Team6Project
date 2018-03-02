#include "PlayerSoftBody.h"
#include <algorithm>


PlayerSoftBody::PlayerSoftBody(const std::string& name,
	nclgl::Maths::Vector3& pos,
	float radius,
	float inverse_mass,
	nclgl::Maths::Vector4& color)
{
	speed = 20.0f;
	ball = new Softbody("ball",
		pos,	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		radius,				//Half dimensions
		true,									//Has Physics Object
		inverse_mass,									//Mass
		true,									//Has Collision Shape
		false,									//Dragable by the user
		color);

	ball->buildsoftbody();

	body = CommonUtils::BuildCuboidObject("body",
		nclgl::Maths::Vector3(0.0f, 2.0f, 0.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		nclgl::Maths::Vector3(0.5, 0.5, 0.5),				//Half dimensions
		false,									//Has Physics Object
		0.0f,									//Mass
		false,									//Has Collision Shape
		false,									//Dragable by the user
		CommonUtils::GenColor(0.45f, 0.5f));

	bodyRenderNode = (*body->Render()->GetChildIteratorStart());

	camera = new Camera();
	camera->SetYaw(0.f);
	camera->SetPitch(-20.0f);

	camera_transform = RenderNodeFactory::Instance()->MakeRenderNode();
	camera_transform->SetTransform(nclgl::Maths::Matrix4::Translation(nclgl::Maths::Vector3(0, 10, 25)));

	(*body->Render()->GetChildIteratorStart())->AddChild(camera_transform);
	(*body->Render()->GetChildIteratorStart())->SetMesh(NULL);

	tempPitch = camera->GetPitch();
	tempYaw = camera->GetYaw();
}


PlayerSoftBody::~PlayerSoftBody()
{
}

GameObject* PlayerSoftBody::getTop() {
	int k = 0;
	top = ball->softball[0];
	for (int i = 1; i < 182; ++i) {
		if (ball->softball[i]->Physics()->GetPosition().y > top->Physics()->GetPosition().y) {
			top = ball->softball[i];
			++k;
		}
	}

	if (k == 0)
		bottom = ball->softball[181];
	else if (k == 181)
		bottom = ball->softball[0];
	else if (((k % 18) > 9) || (k % 18) == 0)
		bottom = ball->softball[k - 9];
	else
		bottom = ball->softball[k + 9];

	return top;
}

GameObject* PlayerSoftBody::getBottom() {
	getTop();
	return bottom;
}

GameObject* PlayerSoftBody::getFront() {
	int k = 0;
	front = ball->softball[0];
	for (int i = 1; i < 182; ++i) {
		if (ball->softball[i]->Physics()->GetPosition().z < top->Physics()->GetPosition().z) {
			front = ball->softball[i];
			++k;
		}
	}

	if (k == 0)
		back = ball->softball[181];
	else if (k == 181)
		back = ball->softball[0];
	else if (((k % 18) > 9) || (k % 18) == 0)
		back = ball->softball[k - 9];
	else
		back = ball->softball[k + 9];

	return front;
}

void PlayerSoftBody::setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump, KeyboardKeys shoot) {
	move_up = up;
	move_down = down;
	move_left = left;
	move_right = right;
	move_jump = jump;
	move_shoot = shoot;
}

void PlayerSoftBody::resetCamera(float dt) {
	if (sensitivity > 0) {
		sensitivity -= dt * 7;
		if (sensitivity < 0.0f) sensitivity = 0.0f;
	}
	else if (sensitivity < 0) {
		sensitivity += dt * 7;
		if (sensitivity > 0.0f) sensitivity = 0.0f;
	}
}

bool PlayerSoftBody::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode) {
	return true;
}

void PlayerSoftBody::shoot() {

}

void PlayerSoftBody::equipStunWeapon(nclgl::Maths::Vector4 colour) {

}

void PlayerSoftBody::equipPaintWeapon(nclgl::Maths::Vector4 colour) {

}

void PlayerSoftBody::handleInput(float dt) {
	nclgl::Maths::Vector3 jump(0, 20, 0);
	float rotation = 0.0f;
	float yaw = camera->GetYaw();
	float pitch = camera->GetPitch();
	nclgl::Maths::Vector3 up = nclgl::Maths::Vector3(0, 1, 0);
	nclgl::Maths::Vector3 right = nclgl::Maths::Vector3::Cross(forward, up);

	getTop();
	getFront();

	if (Window::GetKeyboard()->KeyDown(move_up))
	{
		if (forward.z > 0) {
			top->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, (ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, -5 - forward.z * speed));
			bottom->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, -(ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, -5 - forward.z * speed));
			front->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, -forward.y * speed, -speed - forward.z * speed));
			back->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, forward.y * speed, -speed - forward.z * speed));
		}
		else {
			top->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, (ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, 5 - forward.z * speed));
			bottom->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, -(ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, 5 - forward.z * speed));
			front->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, -forward.y * speed, speed - forward.z * speed));
			back->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, forward.y * speed, speed - forward.z * speed));
		}
	}

	if (Window::GetKeyboard()->KeyDown(move_down))
	{
		if (forward.z > 0) {
			top->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, (ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, 5 + forward.z * speed));
			bottom->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, -(ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, 5 + forward.z * speed));
			front->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, -forward.y * speed, speed + forward.z * speed));
			back->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, forward.y * speed, speed + forward.z * speed));
		}
		else {
			top->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, (ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, -5 + forward.z * speed));
			bottom->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, -(ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, -5 + forward.z * speed));
			front->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, -forward.y * speed, -speed + forward.z * speed));
			back->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, forward.y * speed, -speed + forward.z * speed));
		}
	}
	if (Window::GetKeyboard()->KeyDown(move_left))
	{
		for (int i = 0; i < 182; ++i)
		{
			getBall()->softball[i]->Physics()->SetForce(
				getBall()->softball[i]->Physics()->GetForce() + right * getBall()->softball[i]->Physics()->GetForce().Length() * 0.00005f);
		}
		increaseSensitivity(dt);
		camera->SetYaw(yaw + sensitivity);
	}

	else if (Window::GetKeyboard()->KeyDown(move_right))
	{
		for (int i = 0; i < 182; ++i)
		{
			getBall()->softball[i]->Physics()->SetForce(
				getBall()->softball[i]->Physics()->GetForce() - right * getBall()->softball[i]->Physics()->GetForce().Length() * 0.00005f);
		}
		decreaseSensitivity(dt);
		camera->SetYaw(yaw + sensitivity);
	}

	else {
		resetCamera(dt);
		camera->SetYaw(yaw + sensitivity);
	}

	if ((Window::GetKeyboard()->KeyTriggered(move_jump)))
	{
		if (canjump == true) {
			for (int i = 0; i < 182; ++i) {
				ball->softball[i]->Physics()->SetLinearVelocity(ball->softball[i]->Physics()->GetLinearVelocity() + jump);
			}
			canjump = false;
		}
	}

	if ((Window::GetKeyboard()->KeyTriggered(move_shoot)))
	{
		shoot();
	}
}

void PlayerSoftBody::move(float dt) {
	nclgl::Maths::Vector3 ball_pos = nclgl::Maths::Vector3((ball->softball[0]->Physics()->GetPosition().x + ball->softball[181]->Physics()->GetPosition().x) / 2,
		(ball->softball[0]->Physics()->GetPosition().y + ball->softball[181]->Physics()->GetPosition().y) / 2,
		(ball->softball[0]->Physics()->GetPosition().z + ball->softball[181]->Physics()->GetPosition().z) / 2);

	forward = (camera->GetPosition() - ball_pos).Normalise();

	nclgl::Maths::Matrix4 worldTr = bodyRenderNode->GetWorldTransform();
	worldTr.SetPositionVector(ball_pos + nclgl::Maths::Vector3(0, 2, 0));
	bodyRenderNode->SetTransform(worldTr);

	
	handleInput(dt);
	bodyRenderNode->SetTransform(bodyRenderNode->GetTransform()*nclgl::Maths::Matrix4::Rotation(sensitivity, nclgl::Maths::Vector3(0, 1, 0)));

	camera->SetPosition(camera_transform->GetWorldTransform().GetPositionVector());

	if (bottom->Physics()->GetPosition().y >5) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}

	if (bottom->Physics()->GetLinearVelocity().z < -12) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}

	else if (bottom->Physics()->GetLinearVelocity().z > 12) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}

	else if (bottom->Physics()->GetLinearVelocity().x > 12) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}

	else if (bottom->Physics()->GetLinearVelocity().x < -12) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}
	else
		return;
}