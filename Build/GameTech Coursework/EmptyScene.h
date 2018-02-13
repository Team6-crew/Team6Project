#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\OcTree.h>
#include <ncltech\WorldPartition.h>
#include <algorithm>
#include <ncltech\BallAI.h>
#include <ncltech\StateMachine.h>
//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	
	Player* player1;
	EmptyScene(const std::string& friendly_name) 
		: Scene(friendly_name)
	{
	}
	BallAI * AIBall;
	

	virtual ~EmptyScene()
	{
		delete player1;
		delete AIBall;
	}

	//WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
	
		
		player1 = new Player();
		AIBall = new BallAI();
		


		//Who doesn't love finding some common ground?
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, -1.5f, 0.0f),
			Vector3(80.0f, 1.0f, 80.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

		

		//Who doesn't love finding some common ground?
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"test",
			Vector3(5.0f, 1.5f, 0.0f),
			Vector3(2.0f, 1.0f, 2.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

		//Add player to scene
		this->AddGameObject(player1->getBall());	
		this->AddGameObject(player1->getBody());	
		
		player1->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L);

		this->AddGameObject(AIBall->getBall());

		//add world part
		PhysicsEngine::Instance()->GetWorldPartition()->insert(m_vpObjects);

	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);
		
		player1->move();
		AIBall->move();

	}
};