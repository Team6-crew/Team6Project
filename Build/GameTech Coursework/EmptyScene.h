#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\OcTree.h>
#include <nclgl\Launchpad.h>
#include <nclgl\Portal.h>

#include <ncltech\Tags.h>
#include <ncltech\SpeedPickup.h>

#include <ncltech\WorldPartition.h>
#include <algorithm>


//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	Player* getPlayer() { return player1; }
	Player* player1;
	float rotation = 0.0f;
	EmptyScene(const std::string& friendly_name) 
		: Scene(friendly_name)
	{
	}

	virtual ~EmptyScene()
	{
		delete player1;
	}

	//WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
	
		
		/*player1 = new Player("player",
			Vector3(0.0f, 1.f, 0.0f),
			1.0f,
			true,
			1.0f,
			true,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		player1->SetPhysics(player1->Physics());*/

		player1 = new Player("player",
			Vector3(0.0f, 1.f, 0.0f),
			1.0f,
			true,
			1.0f,
			true,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		player1->SetPhysics(player1->Physics());

		//Add player to scene
		this->AddGameObject(player1);
		//Also add body which is used for camera manipulation
		this->AddGameObject(player1->getBody());

		player1->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L, KEYBOARD_SPACE);

		//Who doesn't love finding some common ground?
		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, -1.5f, 0.0f),
			Vector3(40.0f, 1.0f, 40.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		this->AddGameObject(ground);
		ground->SetTag(Tags::TGround);

		//add a killzone to remove the object out of ground
		GameObject* killzone = CommonUtils::BuildCuboidObject(
			"kill",
			Vector3(0.0f, -10.f, 0.0f),
			Vector3(200.0f, 0.5f, 200.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.7f, 0.7f, 1.0f, 0.0f));

		killzone->Physics()->SetOnCollisionCallback(
			std::bind(&EmptyScene::collisionCallback,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);

		this->AddGameObject(killzone);
		
		//the ball to test the killzone
		//GameObject* sphere = CommonUtils::BuildSphereObject(
		//	"",					// Optional: Name
		//	Vector3(0.0f, 5.f, 12.0f),				// Position
		//	0.5f,		     	// Half-Dimensions
		//	true,				// Physics Enabled?
		//	1.f,				// Physical Mass (must have physics enabled)
		//	true,				// Physically Collidable (has collision shape)
		//	false,				// Dragable by user?
		//	Vector4(0.5f, 0.5f, 1.0f, 1.0f));             // Render color
		//sphere->SetTag(Tags::TCanKiLL);
		//sphere->setDynamic(true);
		//this->AddGameObject(sphere);

		/*this->AddGameObject(CommonUtils::BuildCuboidObject(
			"pickup",
			Vector3(10.0f, 1.f, 0.0f),
			Vector3(0.6f, 0.2f, 0.2f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));*/
		SpeedPickup* pickup = new SpeedPickup("pickup",
			Vector3(10.0f, 1.f, 0.0f),
			0.5f,
			true,
			0.0f,
			true,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		pickup->SetPhysics(pickup->Physics());
		this->AddGameObject(pickup);

		Launchpad* launchpad = new Launchpad(
			"launchpad",
			Vector3(-10.0f, -0.3f, 0.0f),
			Vector3(1.0f,0.1f,1.0f),
			true,
			0.0f,
			true,
			Vector4(0.7f, 0.5f, 0.7f, 1.0f));
		launchpad->SetPhysics(launchpad->Physics());
		this->AddGameObject(launchpad);

		//portal a1
		Portal* portal_a1 = new Portal(
			"portal_a1",
			Vector3(-10.0f, 0.5f, 10.0f),
			true,
			100.0f,
			true,
			Vector4(0.7f, 0.4f, 0.8f, 1.0f));
		portal_a1->setDynamic(true);
		portal_a1->SetPhysics(portal_a1->Physics());
		portal_a1->SetTag(Tags::TPortal_A1);
		portal_a1->Physics()->SetOnCollisionCallback(
			std::bind(&EmptyScene::collisionCallback_a1,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(portal_a1);

		//portal a2
		Portal* portal_a2 = new Portal(
			"portal_a2",
			Vector3(-10.2f, 0.5f, 10.0f),
			true,
			100.0f,
			true,
			Vector4(0.7f, 0.4f, 0.8f, 1.0f));
		portal_a2->setDynamic(true);
		portal_a2->SetPhysics(portal_a2->Physics());
		portal_a2->SetTag(Tags::TPortal_A2);
		portal_a2->Physics()->SetOnCollisionCallback(
			std::bind(&EmptyScene::collisionCallback_a2,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(portal_a2);

		//portal b1
		Portal* portal_b1 = new Portal(
			"portal_b1",
			Vector3(10.0f, 0.5f, -10.0f),
			true,
			100.0f,
			true,
			Vector4(0.7f, 0.4f, 0.8f, 1.0f));
		portal_b1->setDynamic(true);
		portal_b1->SetPhysics(portal_b1->Physics());
		portal_b1->SetTag(Tags::TPortal_B1);
		portal_b1->Physics()->SetOnCollisionCallback(
			std::bind(&EmptyScene::collisionCallback_b1,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(portal_b1);

		//portal b2
		Portal* portal_b2 = new Portal(
			"portal_b2",
			Vector3(10.2f, 0.5f, -10.0f),
			true,
			100.0f,
			true,
			Vector4(0.7f, 0.4f, 0.8f, 1.0f));
		portal_b2->setDynamic(true);
		portal_b2->SetPhysics(portal_b2->Physics());
		portal_b2->SetTag(Tags::TPortal_B2);
		portal_b2->Physics()->SetOnCollisionCallback(
			std::bind(&EmptyScene::collisionCallback_b2,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(portal_b2);


		//add world part
		PhysicsEngine::Instance()->GetWorldPartition()->insert(m_vpObjects);
	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);

		GameObject *pickup = FindGameObject("pickup");
		rotation = 0.1f;
		if(pickup)
		(*pickup->Render()->GetChildIteratorStart())->SetTransform(Matrix4::Rotation(rotation, Vector3(0, 1, 0))*(*pickup->Render()->GetChildIteratorStart())->GetTransform());
		
		player1->move();

	}

	bool collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TCanKiLL))
		{			
			GameObject *kill_ob = (GameObject*)otherNode->GetParent();
			PhysicsEngine::Instance()->DeleteNextFrame(kill_ob);
		}	
		return true;
	};

	bool collisionCallback_a1(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TPlayer))
		{
			GameObject *portal = FindGameObject("portal_b1");
			portal->physicsNode->SetLinearVelocity(Vector3(0, 0, 0));
			portal->physicsNode->SetForce(Vector3(0, 0, 0));
			player1->physicsNode->SetPosition(portal->physicsNode->GetPosition()+Vector3(-2,0,0));
		}
		return true;
	};

	bool collisionCallback_a2(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TPlayer))
		{
			GameObject *portal = FindGameObject("portal_b2");
			portal->physicsNode->SetLinearVelocity(Vector3(0, 0, 0));
			portal->physicsNode->SetForce(Vector3(0, 0, 0));
			player1->physicsNode->SetPosition(portal->physicsNode->GetPosition() + Vector3(2, 0, 0));
		}
		return true;
	};

	bool collisionCallback_b1(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TPlayer))
		{
			GameObject *portal = FindGameObject("portal_a1");
			portal->physicsNode->SetLinearVelocity(Vector3(0, 0, 0));
			portal->physicsNode->SetForce(Vector3(0, 0, 0));
			player1->physicsNode->SetPosition(portal->physicsNode->GetPosition() + Vector3(2, 0, 0));
		}
		return true;
	};

	bool collisionCallback_b2(PhysicsNode* thisNode, PhysicsNode* otherNode)
	{
		if (otherNode->GetParent()->HasTag(Tags::TPlayer))
		{
			GameObject *portal = FindGameObject("portal_a2");
			portal->physicsNode->SetLinearVelocity(Vector3(0, 0, 0));
			portal->physicsNode->SetForce(Vector3(0, 0, 0));
			player1->physicsNode->SetPosition(portal->physicsNode->GetPosition() + Vector3(-2, 0, 0));
		}
		return true;
	};

	
};