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

	camera = new Camera();
	camera->SetYaw(0.f);

	camera_transform = RenderNodeFactory::Instance()->MakeRenderNode();
	camera_transform->SetTransform(nclgl::Maths::Matrix4::Translation(nclgl::Maths::Vector3(0, 10, 25)));

	(*body->Render()->GetChildIteratorStart())->AddChild(camera_transform);
	(*body->Render()->GetChildIteratorStart())->SetMesh(NULL);
}


PlayerSoftBody::~PlayerSoftBody()
{
}

GameObject* PlayerSoftBody::getControl() {
	int k = 0;
	control = ball->softball[0];
	for (int i = 1; i < 182; ++i) {
		if (ball->softball[i]->Physics()->GetPosition().y > control->Physics()->GetPosition().y) {
			control = ball->softball[i];
			++k;
		}
	}

	if (k == 0)
		oppcontrol = ball->softball[181];
	else if (k == 181)
		oppcontrol = ball->softball[0];
	else if (((k % 18) > 9) || (k % 18) == 0)
		oppcontrol = ball->softball[k - 9];
	else
		oppcontrol = ball->softball[k + 9];

	return control;
}

void PlayerSoftBody::setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump, KeyboardKeys shoot) {
	move_up = up;
	move_down = down;
	move_left = left;
	move_right = right;
	move_jump = jump;
	move_shoot = shoot;
}

void PlayerSoftBody::move() {
	nclgl::Maths::Vector3 ball_pos = nclgl::Maths::Vector3((ball->softball[0]->Physics()->GetPosition().x + ball->softball[181]->Physics()->GetPosition().x) / 2,
		(ball->softball[0]->Physics()->GetPosition().y + ball->softball[181]->Physics()->GetPosition().y) / 2,
		(ball->softball[0]->Physics()->GetPosition().z + ball->softball[181]->Physics()->GetPosition().z) / 2);

	nclgl::Maths::Vector3 jump(0, 20, 0);

	nclgl::Maths::Vector3 forward = (camera->GetPosition() - ball_pos).Normalise();

	RenderNodeBase* bodyRenderNode = (*body->Render()->GetChildIteratorStart());
	nclgl::Maths::Matrix4 worldTr = bodyRenderNode->GetWorldTransform();
	worldTr.SetPositionVector(ball_pos + nclgl::Maths::Vector3(0, 2, 0));

	bodyRenderNode->SetTransform(worldTr);

	float yaw = camera->GetYaw();
	float pitch = camera->GetPitch();

	//ball->Physics()->SetForce(Vector3(0, 0, 0));

	float rotation = 0.0f;

	getControl();


	if (Window::GetKeyboard()->KeyDown(move_up))
	{
		control->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * (15), (ball->GetRadius() - (forward.z * (15)) * (forward.z * (15))) / 5, -5 - forward.z * (15)));
		oppcontrol->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * (15), -(ball->GetRadius() - (forward.z * (15)) * (forward.z * (15))) / 5, -5 - forward.z * (15)));
	}

	if (Window::GetKeyboard()->KeyDown(move_down))
	{
		control->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * (15), (ball->GetRadius() - (forward.z * (15)) * (forward.z * (15))) / 5, 5 + forward.z * (15)));
		oppcontrol->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * (15), -(ball->GetRadius() - (forward.z * (15)) * (forward.z * (15))) / 5, 5 + forward.z * (15)));
	}
	if (Window::GetKeyboard()->KeyDown(move_left))
	{
		rotation = 0.5f;
		camera->SetYaw(yaw + rotation);
	}

	if (Window::GetKeyboard()->KeyDown(move_right))
	{
		rotation = -0.5f;
		camera->SetYaw(yaw + rotation);
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

	bodyRenderNode->SetTransform(nclgl::Maths::Matrix4::Rotation(rotation, nclgl::Maths::Vector3(0, 1, 0))*bodyRenderNode->GetTransform());

	camera->SetPosition(camera_transform->GetWorldTransform().GetPositionVector());

}