#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\OcTree.h>
#include <ncltech\WorldPartition.h>
#include <algorithm>

//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	
	Player* player1;
	EmptyScene(const std::string& friendly_name) 
		: Scene(friendly_name)
	{
	}

	virtual ~EmptyScene()
	{
		delete player1;
	}

	WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
		wsp = new WorldPartition(new AABB(Vector3(0, 20, 0), 20) , 2);

		
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

		//Add player to scene
		this->AddGameObject(player1->getBall());	
		this->AddGameObject(player1->getBody());	
		player1->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L);

		wsp->insert(m_vpObjects);

	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);
		
		player1->move();

	}
};