#include "Player.h"

#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
GameObject* Player::ball;

Player::Player()
{   
	speed = 20.0f;
	ball = CommonUtils::BuildSphereObject("ball",
		Vector3(0.0f, 1.0f, 0.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		1.0f,				//Half dimensions
		true,									//Has Physics Object
		1.0f,									//Mass
		true,									//Has Collision Shape
		true,									//Dragable by the user
		CommonUtils::GenColor(0.45f, 0.5f));

	ball->setDynamic(true);
	

	body = CommonUtils::BuildCuboidObject("body",
		Vector3(0.0f, 2.0f, 0.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		Vector3(0.5, 0.5, 0.5),				//Half dimensions
		false,									//Has Physics Object
		0.0f,									//Mass
		false,									//Has Collision Shape
		false,									//Dragable by the user
		CommonUtils::GenColor(0.45f, 0.5f));

	camera = GraphicsPipeline::Instance()->GetCamera();

	camera_transform = RenderNodeFactory::Instance()->MakeRenderNode();
	camera_transform->SetTransform(Matrix4::Translation(Vector3(0, 10, 25)));

	(*body->Render()->GetChildIteratorStart())->AddChild(camera_transform);
	(*body->Render()->GetChildIteratorStart())->SetMesh(NULL);
}


Player::~Player()
{

}

void Player::setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right) {
	move_up = up;
	move_down = down;
	move_left = left;
	move_right = right;
}

void Player::move() {

	
	Vector3 ballPos = ball->Physics()->GetPosition();
	Vector3 forward = (camera->GetPosition() - ballPos).Normalise();

	RenderNodeBase* bodyRenderNode = (*body->Render()->GetChildIteratorStart());
	Matrix4 worldTr = bodyRenderNode->GetWorldTransform();
	worldTr.SetPositionVector(ballPos + Vector3(0, 2, 0));
	
	bodyRenderNode->SetTransform(worldTr);

	float yaw = camera->GetYaw();
	float pitch = camera->GetPitch();

	ball->Physics()->SetForce(Vector3(0, 0, 0));

	float rotation = 0.0f;

	if (Window::GetKeyboard()->KeyDown(move_up))
	{
		ball->Physics()->SetForce(-forward * speed);
	}

	if (Window::GetKeyboard()->KeyDown(move_down))
	{
		ball->Physics()->SetForce(forward * speed);
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



	bodyRenderNode->SetTransform(Matrix4::Rotation(rotation, Vector3(0, 1, 0))*bodyRenderNode->GetTransform());

	camera->SetPosition(camera_transform->GetWorldTransform().GetPositionVector());

}
