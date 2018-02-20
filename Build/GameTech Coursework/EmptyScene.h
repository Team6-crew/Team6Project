#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\OcTree.h>
#include <nclgl\AI\BallAI.h>
#include <nclgl\AI\StateMachine.h>
#include <ncltech\Tags.h>
#include <ncltech\SpeedPickup.h>

#include <ncltech\WorldPartition.h>
#include <algorithm>


//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	BallAI * AIBall;
	float rotation = 0.0f;
	EmptyScene(const std::string& friendly_name) 
		: Scene(friendly_name)
	{
	}

	virtual ~EmptyScene()
	{
		delete AIBall;
		delete speed1;
	}

	//WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
		
		AIBall = new BallAI();
		this->AddGameObject(AIBall->getBall());

		player1 = new Player("player",
			nclgl::Maths::Vector3(0.0f, 1.f, 0.0f),
			1.0f,
			true,
			1.0f,
			true,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		player1->SetPhysics(player1->Physics());

		//Add player to scene
		this->AddGameObject(player1);
		//Also add body which is used for camera manipulation
		this->AddGameObject(player1->getBody());

		player1->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L, KEYBOARD_SPACE);
		//Who doesn't love finding some common ground?
		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			nclgl::Maths::Vector3(0.0f, -1.5f, 0.0f),
			nclgl::Maths::Vector3(160.0f, 1.0f, 160.0f),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		this->AddGameObject(ground);
		ground->SetTag(Tags::TGround);

		nclgl::Maths::Vector3 sp1location = nclgl::Maths::Vector3(30.0f, 1.f, 30.0f);

			speed1 = new SpeedPickup("pickup",
			sp1location,
			0.5f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		speed1->SetPhysics(speed1->Physics());
		this->AddGameObject(speed1);
		speed1->getPickup(1)->setLocation(sp1location); //have to get the pickup so we don't get a memory exception

		//add world part
		PhysicsEngine::Instance()->GetWorldPartition()->insert(m_vpObjects);
	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);

		GameObject *pickup = FindGameObject("pickup");
		rotation = 0.1f;
		if(pickup)
		(*pickup->Render()->GetChildIteratorStart())->SetTransform(nclgl::Maths::Matrix4::Rotation(rotation, 
			nclgl::Maths::Vector3(0, 1, 0))*(*pickup->Render()->GetChildIteratorStart())->GetTransform());
		
		player1->move();
		AIBall->move();

	}
};