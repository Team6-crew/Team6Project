#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\PlayerSoftBody.h>
#include <ncltech\OcTree.h>
#include <nclgl\Launchpad.h>
#include <nclgl\Portal.h>

#include <ncltech\Tags.h>
#include <ncltech\SpeedPickup.h>
#include <ncltech\WeaponPickup.h>
#include <ncltech\StunWeaponPickup.h>
#include <ncltech\PaintWeaponPickup.h>
#include <ncltech\Paintbomb.h>
#include <ncltech\Paintbomb.h>
#include <ncltech\Washingzone.h>

#include <ncltech\WorldPartition.h>
#include <algorithm>
#include <nclgl/GameLogic.h>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include "MainMenu.h"
#include <nclgl\ResourceManager.h>

//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{

private:
	int scene_iterator;
	bool backgroundSoundPlaying;
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
		scene_iterator = 0;
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



		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			nclgl::Maths::Vector3(0.0f, 0.0f, 0.0f),
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
		
		backgroundSoundPlaying = false;

	}


	virtual void OnUpdateScene(float dt) override
	{
		if (GameLogic::Instance()->getTotalTime() >= 3.0f) {
			GameLogic::Instance()->setGameHasStarted(true);
			if (!backgroundSoundPlaying) {
				AudioFactory::Instance()->GetAudioEngine()->SetBackgroundSound(SOUNDSDIR"WonderfulLights.ogg");
				backgroundSoundPlaying = true;
			}

		}
		if (scene_iterator > 0) {
			GameLogic::Instance()->setLevelIsLoaded(true);
		}
		else scene_iterator = 1;

		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			if (GameLogic::Instance()->getSoftPlayer(i)->getBall())
				GameLogic::Instance()->getSoftPlayer(i)->getBall()->RemoveRender();
		}

		Scene::OnUpdateScene(dt);
		NCLDebug::AddHUD(nclgl::Maths::Vector4(0.0f, 0.0f, 0.0f, 1.0f), "Score: " + std::to_string(Score));
		
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); ++i)
			GameLogic::Instance()->getPlayer(i)->move(dt);
		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			GameLogic::Instance()->getSoftPlayer(i)->getBall()->RenderSoftbody();
			GameLogic::Instance()->getSoftPlayer(i)->move();
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