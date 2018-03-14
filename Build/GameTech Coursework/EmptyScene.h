#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <ncltech\PlayerSoftBody.h>
#include <ncltech\OcTree.h>
#include <nclgl\AI\BallAI.h>
#include <nclgl\Launchpad.h>
#include <nclgl\Portal.h>

#include <ncltech\Tags.h>
#include <ncltech\RandomPickup.h>
#include <ncltech\WeaponPickup.h>
#include <ncltech\StunWeaponPickup.h>
#include <ncltech\PaintWeaponPickup.h>
#include <ncltech\Paintbomb.h>
#include <ncltech\Paintbomb.h>
#include <ncltech\Washingzone.h>

#include <ncltech\WorldPartition.h>
#include <algorithm>
#include <nclgl/GameLogic.h>
#include <nclgl/MapNavigation.h>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include "MainMenu.h"
#include <nclgl\ResourceManager.h>
#include <ncltech\AABB.h>

//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{

private:
	int scene_iterator;
	bool backgroundSoundPlaying;
public:
	
	float rotation = 0.0f;
	int volumelevel = 5;
	int tempvolumelevel = 0;
	bool isPaused = false;
	AABB* walls[4];
	EmptyScene(const std::string& friendly_name)
		: Scene(friendly_name)
	{

		// Pause Menu
		pauseMenu = new Menu();
		pauseMenu->visible = false;
		pauseMenu->AddMenuItem("Resume");
		pauseMenu->AddMenuItem("Sound");
		pauseMenu->AddMenuItem("Back to Main Menu");
		pauseMenu->AddMenuItem("Exit Game");
		pauseMenu->setSelection(0);
		pauseMenu->set_id(1);
		activeMenu = NULL;
		// Sound Menu
		soundMenu = new Menu();
		soundMenu->visible = false;
		soundMenu->AddMenuItem("Volume " + std::to_string(volumelevel));
		soundMenu->AddMenuItem("Back");
		soundMenu->setSelection(0);
		soundMenu->set_id(2);
	}

	virtual ~EmptyScene()
	{
		
	}

	//WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{   
		
		walls[0] = new AABB(nclgl::Maths::Vector3(200, 0, 0), 100);
		walls[1] = new AABB(nclgl::Maths::Vector3(-200, 0, 0), 100);
		walls[2] = new AABB(nclgl::Maths::Vector3(0, 0, 200), 100);
		walls[3] = new AABB(nclgl::Maths::Vector3(0, 0, -200), 100);

		scene_iterator = 0;
		Scene::OnInitializeScene();

		int num_p = GameLogic::Instance()->getnumOfPlayersMp();
		if (num_p & 0b0001) GameLogic::Instance()->addSoftPlayer(0);
		if (num_p & 0b0010) GameLogic::Instance()->addSoftPlayer(1);
		if (num_p & 0b0100) GameLogic::Instance()->addSoftPlayer(2);
		if (num_p & 0b1000) GameLogic::Instance()->addSoftPlayer(3);
		//Add player to scene

		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
			this->AddGameObject(GameLogic::Instance()->getPlayer(i));
			this->AddGameObject(GameLogic::Instance()->getPlayer(i)->getBody());
		}

		//Add player to scene
		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			this->AddSoftBody(GameLogic::Instance()->getSoftPlayer(i)->getBall());
			this->AddGameObject(GameLogic::Instance()->getSoftPlayer(i)->getBody());
		}
		num_p = GameLogic::Instance()->getnumAI();
		if (num_p & 0b0001) BallAI::addBallAIPlayers(0);
		if (num_p & 0b0010) BallAI::addBallAIPlayers(1);
		if (num_p & 0b0100) BallAI::addBallAIPlayers(2);
		if (num_p & 0b1000) BallAI::addBallAIPlayers(3);

		for (int j = 0; j < GameLogic::Instance()->getNumAIPlayers(); j++) {
			this->AddGameObject(GameLogic::Instance()->getAIPlayer(j));
			this->AddGameObject(GameLogic::Instance()->getAIPlayer(j)->getBody());
		}
		
		//Who doesn't love finding some common ground?
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
		(*ground->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"ground.jpg"), 0);
		(*ground->Render()->GetChildIteratorStart())->SetTag(Tags::TGround);
		
		backgroundSoundPlaying = false;

		RandomPickup* pickup1 = new RandomPickup("pickup",
			nclgl::Maths::Vector3(-5.0f, 3.f, -50.0f),
			1.0f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		pickup1->SetPhysics(pickup1->Physics());
		(*pickup1->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"randompick.jpg"), 0);
		this->AddGameObject(pickup1);
		

		RandomPickup* pickup2 = new RandomPickup("pickup",
			nclgl::Maths::Vector3(0.0f, 3.f, -50.0f),
			1.0f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		pickup2->SetPhysics(pickup2->Physics());
		this->AddGameObject(pickup2);
		

		RandomPickup* pickup3 = new RandomPickup("pickup",
			nclgl::Maths::Vector3(5.0f, 3.f, -50.0f),
			1.0f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		pickup3->SetPhysics(pickup3->Physics());
		this->AddGameObject(pickup3);
		
		//testcube- test the texture
		Washingzone* wz = new Washingzone("washingzone",
			nclgl::Maths::Vector3(0.0f, 3.f, -40.0f),
			nclgl::Maths::Vector3(2.0f, 2.f, 1.0f),
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		wz->SetPhysics(wz->Physics());
		this->AddGameObject(wz);

		Launchpad* lp = new Launchpad("launchpad",
			nclgl::Maths::Vector3(0.0f, 1.5f, -30.0f),
			nclgl::Maths::Vector3(1.5f, 0.5f, 1.5f),
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		lp->SetPhysics(lp->Physics());
		this->AddGameObject(lp);

		Portal* a1  = new Portal("portal_a1",
			nclgl::Maths::Vector3(10.0f, 2.f, -40.0f),
			//nclgl::Maths::Vector3(2.0f, 2.f, 1.0f),
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		a1->SetPhysics(a1->Physics());
		a1->physicsNode->SetOnCollisionCallback(
			std::bind(
				&EmptyScene::collisionCallback_a1,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(a1);
		
		Portal* b1 = new Portal("portal_b1",
			nclgl::Maths::Vector3(-10.0f, 2.f, -40.0f),
			//nclgl::Maths::Vector3(2.0f, 2.f, 1.0f),
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		b1->SetPhysics(b1->Physics());
		b1->physicsNode->SetOnCollisionCallback(
			std::bind(
				&EmptyScene::collisionCallback_b1,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
		);
		this->AddGameObject(b1);
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

		if (GameLogic::Instance()->spawnPickup()) {
			RandomPickup* pickup = new RandomPickup("pickup",
				GameLogic::Instance()->getLastPickupPosition(),
				1.0f,
				true,
				0.5f,
				true,
				nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			pickup->SetPhysics(pickup->Physics());
			pickup->Physics()->SetElasticity(0.0f);
			(*pickup->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"randompick.jpg"), 0);
			this->AddGameObject(pickup);

		}

		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			if ((GameLogic::Instance()->getSoftPlayer(i)) && (GameLogic::Instance()->getSoftPlayer(i)->getIsBroken() == false))
				GameLogic::Instance()->getSoftPlayer(i)->getBall()->RemoveRender();
		}

		/*if (softplayer) {
			softplayer->getBall()->RemoveRender();
		}*/

		Scene::OnUpdateScene(dt);
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); ++i)
			GameLogic::Instance()->getPlayer(i)->move(dt);
		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			if (GameLogic::Instance()->getSoftPlayer(i)->getIsBroken() == false) {
				GameLogic::Instance()->getSoftPlayer(i)->getBall()->RenderSoftbody();
				GameLogic::Instance()->getSoftPlayer(i)->move(dt);
				for (int j = 0; j < 4; j++) {
					GameLogic::Instance()->getSoftPlayer(i)->cameraInWall(walls[j]);
				}
				
			}
		}
		/*if (softplayer) {
			softplayer->getBall()->RenderSoftbody();
			softplayer->move(dt);
		}*/
		for (int j = 0; j < GameLogic::Instance()->getNumAIPlayers(); ++j)
			GameLogic::Instance()->getAIPlayer(j)->move(dt);

		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); ++i) {
			if (GameLogic::Instance()->getSoftPlayer(i)->getIsBroken() == false) {
				if ((GameLogic::Instance()->getSoftPlayer(i)->getTop()->Physics()->GetPosition().y - GameLogic::Instance()->getSoftPlayer(i)->getBottom()->Physics()->GetPosition().y > 5)
					|| GameLogic::Instance()->getSoftPlayer(i)->getBack()->Physics()->GetPosition().z - GameLogic::Instance()->getSoftPlayer(i)->getFront()->Physics()->GetPosition().z > 5) {
					GameLogic::Instance()->getSoftPlayer(i)->setIsBroken(true);
					GameLogic::Instance()->getSoftPlayer(i)->getBall()->RemoveRender();
					//delete GameLogic::Instance()->getSoftPlayer(i);
					GameLogic::Instance()->repairSoftPlayer(i);
					/*softplayer = new PlayerSoftBody("SoftPlayer_" + i,
						nclgl::Maths::Vector3(3.0f * i, 10.f, 3.0f * i),
						1.0f,
						1.0f,
						GameLogic::Instance()->getColours(i),
						i);
					for (int j = 0; j < 182; ++j)
						softplayer->getBall()->softball[j]->SetPhysics(softplayer->getBall()->softball[j]->Physics());
					softplayer->setControls(GameLogic::Instance()->getControls(i, 0), GameLogic::Instance()->getControls(i, 1), GameLogic::Instance()->getControls(i, 2), 
						GameLogic::Instance()->getControls(i, 3), GameLogic::Instance()->getControls(i, 4), GameLogic::Instance()->getControls(i, 5));
					softplayer->setCamera(GraphicsPipeline::Instance()->GetCameras(i));
					*GameLogic::Instance()->getSoftPlayer(i) = *softplayer;*/
					this->AddSoftBody(GameLogic::Instance()->getSoftPlayer(i)->getBall());
					this->AddGameObject(GameLogic::Instance()->getSoftPlayer(i)->getBody());
				}
			}
		}
		
		// Pause Menu

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		{
			isPaused = !isPaused;
			if (activeMenu == NULL) {
				activeMenu = pauseMenu;
			}
			else if (activeMenu != NULL) {
				activeMenu = NULL;
			}
			PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
			tempvolumelevel = 0;
			if (!isPaused)
			{
				tempvolumelevel = volumelevel;
			}
			AudioFactory::Instance()->GetAudioEngine()->SetVolume(float(tempvolumelevel) / 10.0f);


			//if (pauseMenu->visible == false) {
			//	pauseMenu->setSelection(0);
			//	pauseMenu->visible = true;
			//}
			//else {
			//	pauseMenu->setSelection(-1);
			//	pauseMenu->visible = false;
			//}
		}
		if (activeMenu != NULL) {
			activeMenu->ShowPauseMenu();
			if (activeMenu->getSelection() == 0 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN)) {
				if (activeMenu == pauseMenu) {
					PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
					activeMenu = NULL;
					//tempvolumelevel = 0;
					isPaused = !isPaused;
					if (!isPaused)
					{
						tempvolumelevel = volumelevel;
					}
					AudioFactory::Instance()->GetAudioEngine()->SetVolume(float(tempvolumelevel) / 10.0f);
				}
			}
			else if (activeMenu->getSelection() == 0 && activeMenu == soundMenu) {
				if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT) && volumelevel > 0) {
					volumelevel -= 1;
					//AudioFactory::Instance()->GetAudioEngine()->SetVolume(float(volumelevel) / 10.0f);
					//AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"SmallScream.ogg", false);
					tempvolumelevel = volumelevel;
				}
				else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT) && volumelevel < 10) {
					volumelevel += 1;

					tempvolumelevel = volumelevel;
					//AudioFactory::Instance()->GetAudioEngine()->SetVolume(float(volumelevel) / 10.0f);
					//AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"SmallScream.ogg", false);
				}
				activeMenu->replaceMenuItem(activeMenu->getSelection(), "Volume " + std::to_string(volumelevel));
			}
			else if (pauseMenu->getSelection() == 1 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN)) {
				if (activeMenu == pauseMenu) {
					pauseMenu->visible = false;
					activeMenu = soundMenu;
					//activeMenu->ShowMenu();
				}
				else if (activeMenu == soundMenu) {
					activeMenu = pauseMenu;
				}
			}
			else if (pauseMenu->getSelection() == 2 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
			{
				pauseMenu->visible = false;
				SceneManager::Instance()->JumpToScene("Main Menu");
				activeMenu = NULL;

				//Delete objects from the scene
				GameLogic::Instance()->clearGameLogic();
				GraphicsPipeline::Instance()->clearGraphicsPipeline();
				DeleteAllGameObjects();
				m_UpdateCallbacks.clear();
				PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
			}
			else if (pauseMenu->getSelection() == 3 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
			{
				exit(0);
			}
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_UP)) { activeMenu->MoveUp(); }
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_DOWN)) { activeMenu->MoveDown(); }
			
		}		
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
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer1))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(0)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(0)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_a1")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer2))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(1)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(1)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_a1")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer3))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(2)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(2)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_a1")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer4))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(3)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(3)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_a1")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
			}
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
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer1))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(0)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(0)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_a2")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer2))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(1)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(1)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_a2")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer3))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(2)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(2)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_a2")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer4))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_b2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(3)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(3)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_a2")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
			}
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
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer1))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(0)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(0)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_b1")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer2))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(1)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(1)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_b1")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer3))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(2)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(2)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_b1")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer4))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a1");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(3)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(3)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_b1")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(2, 0, 0));
			}
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
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer1))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(0)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(0)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_b2")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer2))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(1)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(1)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_b2")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer3))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(2)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(2)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_b2")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
			}
		}
		else if (otherNode->GetParent()->HasTag(Tags::TSoftPlayer4))
		{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Portal.wav", false);
			GameObject *portal = FindGameObject("portal_a2");
			portal->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
			portal->physicsNode->SetForce(nclgl::Maths::Vector3(0, 0, 0));
			for (int i = 0; i < 182; ++i)
			{
				GameLogic::Instance()->getSoftPlayer(3)->getBall()->softball[i]->Physics()->SetPosition(
					GameLogic::Instance()->getSoftPlayer(3)->getBall()->softball[i]->Physics()->GetPosition()
					- FindGameObject("portal_b2")->Physics()->GetPosition()
					+ portal->physicsNode->GetPosition() + nclgl::Maths::Vector3(-2, 0, 0));
			}
		}
		return true;
	};
private:
	RenderNode * cam;
	GameObject *body;
	GameObject *ball;

	Menu * pauseMenu;
	Menu * activeMenu;
	Menu * activeSubmenu;
	Menu * soundMenu;
	PlayerSoftBody* softplayer;

	float Score = 0.0f;

};