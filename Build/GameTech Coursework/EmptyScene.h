#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\OcTree.h>
#include <ncltech\Tags.h>
#include <ncltech\SpeedPickup.h>

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

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
		OcTree *space = new OcTree(new AABB(Vector3(0, 20, 0), 20));

		
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

		player1->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L);
		//Who doesn't love finding some common ground?
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, -1.5f, 0.0f),
			Vector3(40.0f, 1.0f, 40.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

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
			1.0f,
			true,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		pickup->SetPhysics(pickup->Physics());
		this->AddGameObject(pickup);


	
		
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
};