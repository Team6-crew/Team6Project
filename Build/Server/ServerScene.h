#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\PlayerSoftBody.h>
#include <ncltech\OcTree.h>
#include <nclgl\Launchpad.h>
#include <nclgl\Portal.h>

#include <ncltech\Tags.h>
#include <ncltech\Paintbomb.h>
#include <ncltech\Paintbomb.h>
#include <ncltech\Washingzone.h>

#include <ncltech\WorldPartition.h>
#include <algorithm>
#include <nclgl/GameLogic.h>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include <ncltech/Menu.h>
#include <nclgl\ResourceManager.h>

//Fully striped back scene to use as a template for new scenes.
class ServerScene : public Scene
{
public:

	float rotation = 0.0f;
	ServerScene(const std::string& friendly_name)
		: Scene(friendly_name)
	{

		// Pause Menu
		pauseMenu = new Menu();
		pauseMenu->visible = false;
		pauseMenu->AddMenuItem("Resume");
		pauseMenu->AddMenuItem("Sound");
		pauseMenu->AddMenuItem("Controls");
		pauseMenu->AddMenuItem("Back to Main Menu");
		pauseMenu->AddMenuItem("Exit Game");
		pauseMenu->setSelection(-1);
	}

	virtual ~ServerScene()
	{
		delete player1;
	}

	//WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{
		float step = 6.7f;
		float frame = 0.0f;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		Scene::OnInitializeScene();

		int num_p = GameLogic::Instance()->getnumOfPlayersMp();
		if (num_p & 0b0001) GameLogic::Instance()->addPlayer(0);
		if (num_p & 0b0010) GameLogic::Instance()->addPlayer(1);
		if (num_p & 0b0100) GameLogic::Instance()->addPlayer(2);
		if (num_p & 0b1000) GameLogic::Instance()->addPlayer(3);
		//Add player to scene
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
			this->AddGameObject(GameLogic::Instance()->getPlayer(i));
			this->AddGameObject(GameLogic::Instance()->getPlayer(i)->getBody());
		}

		frame += step * 2.0f;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		//GameLogic::Instance()->addSoftPlayers(1);
		////Add player to scene
		//for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers();i++) {
		//	this->AddSoftBody(GameLogic::Instance()->getSoftPlayer(i)->getBall());
		//	//this->AddGameObject(GameLogic::Instance()->getPlayer(i));
		//	this->AddGameObject(GameLogic::Instance()->getSoftPlayer(i)->getBody());
		//}


		//Who doesn't love finding some common ground?
		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			nclgl::Maths::Vector3(0.0f, -1.0f, 0.0f),
			nclgl::Maths::Vector3(WORLD_SIZE, 1.0f, WORLD_SIZE),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		this->AddGameObject(ground);
		ground->SetTag(Tags::TGround);
		(*ground->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"dirt.jpg"), 0);
		(*ground->Render()->GetChildIteratorStart())->SetTag(Tags::TGround);
		
		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		
		GameObject* testItem = CommonUtils::BuildPaintableCube(
			"Ground",
			nclgl::Maths::Vector3(-5.0f, 5.5f, -5.0f),
			nclgl::Maths::Vector3(2.0f, 2.0f, 2.0f),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		(*testItem->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"pickup.png"), 0);
		(*testItem->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->MakeTexture("transparent_1", Texture::COLOUR, 1024, 1024), 1);
		this->AddGameObject(testItem);
		testItem->SetTag(Tags::TPaintable);
		(*testItem->Render()->GetChildIteratorStart())->SetTag(Tags::TPaintable);
		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		GameObject* testItem2 = CommonUtils::BuildPaintableCube(
			"Ground",
			nclgl::Maths::Vector3(10.0f, 5.5f, -5.0f),
			nclgl::Maths::Vector3(2.0f, 10.0f, 2.0f),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		(*testItem2->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"pickup.png"), 0);
		(*testItem2->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->MakeTexture("transparent_2", Texture::COLOUR, 1024, 1024), 1);
		this->AddGameObject(testItem2);
		testItem2->SetTag(Tags::TPaintable);
		(*testItem2->Render()->GetChildIteratorStart())->SetTag(Tags::TPaintable);

		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		
		
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		Paintbomb* paintbomb = new Paintbomb("paintbomb",
			nclgl::Maths::Vector3(-10.0f, 1.f, 0.0f),
			0.5f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.4f, 0.5f, 1.0f, 1.0f));
		paintbomb->SetPhysics(paintbomb->Physics());
		this->AddGameObject(paintbomb);
		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		Launchpad* launchpad = new Launchpad(
			"launchpad",
			nclgl::Maths::Vector3(-10.0f, -0.3f, -30.0f),
			nclgl::Maths::Vector3(1.0f, 0.1f, 1.0f),
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.7f, 0.5f, 0.7f, 1.0f));
		launchpad->SetPhysics(launchpad->Physics());
		this->AddGameObject(launchpad);
		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		//portal a1
		Portal* portal_a1 = new Portal(
			"portal_a1",
			nclgl::Maths::Vector3(-10.0f, 0.5f, 10.0f),
			true,
			100.0f,
			true,
			nclgl::Maths::Vector4(1.f, 0.f, 0.f, 1.0f));
		portal_a1->setDynamic(false);
		portal_a1->SetPhysics(portal_a1->Physics());
		portal_a1->SetTag(Tags::TPortal_A1);
		portal_a1->Physics()->SetOnCollisionCallback(
			std::bind(&ServerScene::collisionCallback_a1,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(portal_a1);
		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		//portal a2
		Portal* portal_a2 = new Portal(
			"portal_a2",
			nclgl::Maths::Vector3(-10.2f, 0.5f, 10.0f),
			true,
			100.0f,
			true,
			nclgl::Maths::Vector4(1.f, 0.f, 0.f, 1.0f));
		portal_a2->setDynamic(false);
		portal_a2->SetPhysics(portal_a2->Physics());
		portal_a2->SetTag(Tags::TPortal_A2);
		portal_a2->Physics()->SetOnCollisionCallback(
			std::bind(&ServerScene::collisionCallback_a2,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(portal_a2);
		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		//portal b1
		Portal* portal_b1 = new Portal(
			"portal_b1",
			nclgl::Maths::Vector3(10.0f, 0.5f, -10.0f),
			true,
			100.0f,
			true,
			nclgl::Maths::Vector4(0.f, 1.f, 0.f, 1.0f));
		portal_b1->setDynamic(false);
		portal_b1->SetPhysics(portal_b1->Physics());
		portal_b1->SetTag(Tags::TPortal_B1);
		portal_b1->Physics()->SetOnCollisionCallback(
			std::bind(&ServerScene::collisionCallback_b1,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(portal_b1);
		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		//portal b2
		Portal* portal_b2 = new Portal(
			"portal_b2",
			nclgl::Maths::Vector3(10.2f, 0.5f, -10.0f),
			true,
			100.0f,
			true,
			nclgl::Maths::Vector4(0.f, 1.f, 0.f, 1.0f));
		portal_b2->setDynamic(false);
		portal_b2->SetPhysics(portal_b2->Physics());
		portal_b2->SetTag(Tags::TPortal_B2);
		portal_b2->Physics()->SetOnCollisionCallback(
			std::bind(&ServerScene::collisionCallback_b2,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(portal_b2);



		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
		Washingzone* washingzone = new Washingzone("washingzone",
			nclgl::Maths::Vector3(10.0f, -0.389f, 15.0f),
			nclgl::Maths::Vector3(5.0f, 0.01f, 3.0f),
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.6f, 0.5f, 1.0f, 1.0f));
		washingzone->SetTag(Tags::TWash);
		washingzone->SetPhysics(washingzone->Physics());
		this->AddGameObject(washingzone);

		//add world part
		
		
	}


	virtual void OnUpdateScene(float dt) override
	{
		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			if (GameLogic::Instance()->getSoftPlayer(i)->getBall())
				GameLogic::Instance()->getSoftPlayer(i)->getBall()->RemoveRender();
		}
		Scene::OnUpdateScene(dt);
		NCLDebug::AddHUD(nclgl::Maths::Vector4(0.0f, 0.0f, 0.0f, 1.0f), "Score: " + std::to_string(Score));
		GameObject *pickup = FindGameObject("pickup");
		rotation = 0.1f;
		if(pickup)
		(*pickup->Render()->GetChildIteratorStart())->SetTransform(nclgl::Maths::Matrix4::Rotation(rotation, 
			nclgl::Maths::Vector3(0, 1, 0))*(*pickup->Render()->GetChildIteratorStart())->GetTransform());
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); ++i)
			GameLogic::Instance()->getPlayer(i)->moveServer(dt);

	}


	bool collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TCanKiLL))
		{
			GameObject *kill_ob = (GameObject*)otherNode->GetParent();
			PhysicsEngine::Instance()->DeleteAfter(kill_ob,0.0f);
		}	
		return true;
	}

	bool collisionCallback_a1(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TPlayer))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			otherNode->SetPosition(portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
		}
		return true;
	}

	bool collisionCallback_a2(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TPlayer))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			otherNode->SetPosition(portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
		}
		return true;
	}

	bool collisionCallback_b1(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TPlayer))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			otherNode->SetPosition(portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
		}
		return true;
	};

	bool collisionCallback_b2(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TPlayer))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			otherNode->SetPosition(portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
		}
		return true;
	};

private:
	RenderNode * cam;

	GameObject *body;
	GameObject *ball;

	Menu * pauseMenu;

	float Score = 0.0f;
};