#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\OcTree.h>

#include <ncltech\Tags.h>
#include <ncltech\SpeedPickup.h>

#include <ncltech\WorldPartition.h>
#include <algorithm>
#include <nclgl/GameLogic.h>
#include "MainMenu.h"

//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:

	float rotation = 0.0f;
	EmptyScene(const std::string& friendly_name)
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

	virtual ~EmptyScene()
	{
		delete player1;
	}

	//WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{
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



		//Who doesn't love finding some common ground?
		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			nclgl::Maths::Vector3(0.0f, -1.5f, 0.0f),
			nclgl::Maths::Vector3(WORLD_SIZE, 1.0f, WORLD_SIZE),
			true,
			0.0f,
			true,
			false,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		this->AddGameObject(ground);
		ground->SetTag(Tags::TGround);
		(*ground->Render()->GetChildIteratorStart())->SetTag(Tags::TGround);

		SpeedPickup* pickup = new SpeedPickup("pickup",
			nclgl::Maths::Vector3(10.0f, 1.f, 0.0f),
			0.5f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		pickup->SetPhysics(pickup->Physics());
		this->AddGameObject(pickup);

		//add world part
		PhysicsEngine::Instance()->GetWorldPartition()->insert(m_vpObjects);
	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);
		NCLDebug::AddHUD(nclgl::Maths::Vector4(0.0f, 0.0f, 0.0f, 1.0f), "Score: " + std::to_string(Score));
		GameObject *pickup = FindGameObject("pickup");
		rotation = 0.1f;
		if (pickup)
			(*pickup->Render()->GetChildIteratorStart())->SetTransform(nclgl::Maths::Matrix4::Rotation(rotation,
				nclgl::Maths::Vector3(0, 1, 0))*(*pickup->Render()->GetChildIteratorStart())->GetTransform());

		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
			GameLogic::Instance()->getPlayer(i)->move(dt);
		}
		// Pause Menu
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		{

			PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
			if (pauseMenu->visible == false) {
				pauseMenu->setSelection(0);
				pauseMenu->visible = true;
			}
			else {
				pauseMenu->setSelection(-1);
				pauseMenu->visible = false;
			}
		}

		//Navigate choices
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_UP))
		{
			if (pauseMenu->visible == true) { pauseMenu->MoveUp(); }
			else { pauseMenu->MoveUp(); }
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_DOWN))
		{
			if (pauseMenu->visible == true) { pauseMenu->MoveDown(); }
			else { pauseMenu->MoveDown(); }
		}
		if (pauseMenu->visible == true) {
			pauseMenu->ShowMenu();
		}


		//Change Menus
		if (pauseMenu->getSelection() == 0 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
			pauseMenu->visible = false;
		}
		else if (pauseMenu->getSelection() == 3 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			pauseMenu->visible = false;
			SceneManager::Instance()->JumpToScene("Main Menu");
			PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
		}
		else if (pauseMenu->getSelection() == 4 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			exit(0);
		}

		//camera->SetPosition(cam->GetWorldTransform().GetPositionVector());

	}
private:
	RenderNode * cam;

	GameObject *body;
	GameObject *ball;

	Menu * pauseMenu;

	float Score = 0.0f;
};