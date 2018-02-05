#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\OcTree.h>
#include <ncltech\Tags.h>

//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	
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

		
		player1 = new Player();
		//Who doesn't love finding some common ground?
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, -1.5f, 0.0f),
			Vector3(20.0f, 1.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"pickup",
			Vector3(10.0f, 1.f, 0.0f),
			Vector3(1.0f, 1.0f, 1.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

		//Add player to scene
		this->AddGameObject(player1->getBall());	
		this->AddGameObject(player1->getBody());
		
		player1->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L);
		GameObject *pickup = FindGameObject("pickup");
		pickup->SetTag(Tags::TPickup);
	
		
	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);

		GameObject *pickup = FindGameObject("pickup");
		rotation += 0.1f;
		if(pickup)
		(*pickup->Render()->GetChildIteratorStart())->SetTransform(Matrix4::Rotation(rotation, Vector3(0, 1, 0)));
		
		player1->move();

	}
};