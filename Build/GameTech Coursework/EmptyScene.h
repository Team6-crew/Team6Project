#pragma once
#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\OcTree.h>
#include <nclgl\Launchpad.h>

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
			nclgl::Maths::Vector3(20.0f, 1.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		GameObject* topwall = CommonUtils::BuildCuboidObject(
			"TopWall",
			nclgl::Maths::Vector3(0.0f, -1.5f, -19.5f),
			nclgl::Maths::Vector3(20.0f, 10.0f, 1.0f),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		GameObject* backwall = CommonUtils::BuildCuboidObject(
			"BackWall",
			nclgl::Maths::Vector3(0.0f, -1.5f, 19.5f),
			nclgl::Maths::Vector3(20.0f, 10.0f, 1.0f),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		GameObject* rightwall = CommonUtils::BuildCuboidObject(
			"RightWall",
			nclgl::Maths::Vector3(19.5f, -1.5f, 0.0f),
			nclgl::Maths::Vector3(1.0f, 10.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		GameObject* leftwall = CommonUtils::BuildCuboidObject(
			"LeftWall",
			nclgl::Maths::Vector3(-19.5f, -1.5f, 0.0f),
			nclgl::Maths::Vector3(1.0f, 10.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		this->AddGameObject(ground);		
		ground->SetTag(Tags::TGround);
		
		this->AddGameObject(topwall);
		this->AddGameObject(backwall);
		this->AddGameObject(rightwall);
		this->AddGameObject(leftwall);

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
			nclgl::Maths::Vector3(10.0f, 1.f, 0.0f),
			0.5f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));
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
};