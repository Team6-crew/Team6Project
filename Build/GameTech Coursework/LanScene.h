#pragma once

#include <nclgl/MySocket.h>
#include <ncltech/Network.h>
#include <ncltech/NetworkBase.h>
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
#include <ncltech/Menu.h>
#include <nclgl\ResourceManager.h>
#include <nclgl\LevelLoader.h>
//Fully striped back scene to use as a template for new scenes.
class LanScene : public Scene
{

private:
	vector <vector<nclgl::Maths::Vector3>> forces;
	int scene_iterator;
	bool backgroundSoundPlaying;
public:

	float rotation = 0.0f;
	int volumelevel = 5;
	int tempvolumelevel = 0;
	bool isPaused = false;
	LanScene(const std::string& friendly_name)
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
		listen = Network::Instance()->getListen();
	}

	virtual ~LanScene()
	{

	}

	//WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{
		scene_iterator = 0;
		Scene::OnInitializeScene();

		int num_p = GameLogic::Instance()->getnumOfPlayersMp();
		if (num_p & 0b0001) GameLogic::Instance()->addSoftPlayer(0);
		if (num_p & 0b0010) GameLogic::Instance()->addSoftPlayer(1);
		if (num_p & 0b0100) GameLogic::Instance()->addSoftPlayer(2);
		if (num_p & 0b1000) GameLogic::Instance()->addSoftPlayer(3);

		//Add player to scene
		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			this->AddSoftBody(GameLogic::Instance()->getSoftPlayer(i)->getBall());
			this->AddGameObject(GameLogic::Instance()->getSoftPlayer(i)->getBody());
		}
		
		num_p = GameLogic::Instance()->getnumOfNetPlayers();
		if (num_p & 0b0001) GameLogic::Instance()->addNetPlayer(0);
		if (num_p & 0b0010) GameLogic::Instance()->addNetPlayer(1);
		if (num_p & 0b0100) GameLogic::Instance()->addNetPlayer(2);
		if (num_p & 0b1000) GameLogic::Instance()->addNetPlayer(3);
		for (int i = 0; i < GameLogic::Instance()->getNumNetPlayers(); i++) {
			vector<nclgl::Maths::Vector3> tempForce;
			for (int i = 0; i < 4; ++i)
				tempForce.push_back(nclgl::Maths::Vector3(.0f, .0f, .0f));
			forces.push_back(tempForce);
			this->AddSoftBody(GameLogic::Instance()->getNetPlayer(i)->getBall());
			this->AddGameObject(GameLogic::Instance()->getNetPlayer(i)->getBody());
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
		(*ground->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"dirt.jpg"), 0);
		(*ground->Render()->GetChildIteratorStart())->SetTag(Tags::TGround);

		backgroundSoundPlaying = false;

		RandomPickup* pickup1 = new RandomPickup("pickup",
			nclgl::Maths::Vector3(-5.0f, 3.f, -50.0f),
			1.0f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		pickup1->SetPhysics(pickup1->Physics());
		this->AddGameObject(pickup1);


		RandomPickup* pickup2 = new RandomPickup("pickup",
			nclgl::Maths::Vector3(0.0f, 3.f, -50.0f),
			1.0f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		pickup2->SetPhysics(pickup2->Physics());
		this->AddGameObject(pickup2);


		RandomPickup* pickup3 = new RandomPickup("pickup",
			nclgl::Maths::Vector3(5.0f, 3.f, -50.0f),
			1.0f,
			true,
			0.0f,
			true,
			nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));
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
		(*wz->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"washingzone.jpg"), 0);
		this->AddGameObject(wz);
		//frame += step;
		//GraphicsPipeline::Instance()->LoadingScreen(frame);
		LevelLoader loader;
		loader.BuildLevel("Level1.txt", this);
		
		
		myPlayerNum = GameLogic::Instance()->getMyNetNum();
		
	}


	virtual void OnUpdateScene(float dt) override
	{

		for (int i = 0; i < GameLogic::Instance()->getNumNetPlayers(); i++) {
			GameLogic::Instance()->getNetPlayer(i)->setAxisSpheres();
			GameLogic::Instance()->getNetPlayer(i)->getTop()->Physics()->SetForce(forces[i][0]);
			GameLogic::Instance()->getNetPlayer(i)->getBottom()->Physics()->SetForce(forces[i][1]);
			GameLogic::Instance()->getNetPlayer(i)->getFront()->Physics()->SetForce(forces[i][2]);
			GameLogic::Instance()->getNetPlayer(i)->getBack()->Physics()->SetForce(forces[i][3]);
		}
		auto callback = std::bind(
			&LanScene::ProcessNetworkEvent,	// Function to call
			this,								// Associated class instance
			std::placeholders::_1);				// Where to place the first parameter
		listen->ServiceNetwork(dt, callback);
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
				nclgl::Maths::Vector4(0.2f, 0.5f, 1.0f, 1.0f));
			pickup->SetPhysics(pickup->Physics());
			pickup->Physics()->SetElasticity(0.0f);
			this->AddGameObject(pickup);

		}

		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			if (GameLogic::Instance()->getSoftPlayer(i)->getBall())
				GameLogic::Instance()->getSoftPlayer(i)->getBall()->RemoveRender();
		}
		for (int i = 0; i < GameLogic::Instance()->getNumNetPlayers(); i++) {
			if (GameLogic::Instance()->getNetPlayer(i)->getBall())
				GameLogic::Instance()->getNetPlayer(i)->getBall()->RemoveRender();
		}

		Scene::OnUpdateScene(dt);

		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); ++i)
			GameLogic::Instance()->getPlayer(i)->move(dt);
		for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
			GameLogic::Instance()->getSoftPlayer(i)->getBall()->RenderSoftbody();
			GameLogic::Instance()->getSoftPlayer(i)->move(dt);
		}
		if (GameLogic::Instance()->getJustJumped()) {
			GameLogic::Instance()->setJustJumped(false);
			sendPacketJump();
		}
		for (int i = 0; i < GameLogic::Instance()->getNumNetPlayers(); i++) {
			GameLogic::Instance()->getNetPlayer(i)->getBall()->RenderSoftbody();
		}
		for (int j = 0; j < GameLogic::Instance()->getNumAIPlayers(); ++j)
			GameLogic::Instance()->getAIPlayer(j)->move(dt);


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
				//PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
				activeMenu = NULL;

				//Delete objects from the scene
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
	void sendVelocityUpdate() {

		MySocket velocity("INFO");
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getTop()->Physics()->GetForce().x));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getTop()->Physics()->GetForce().y));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getTop()->Physics()->GetForce().z));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBottom()->Physics()->GetForce().x));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBottom()->Physics()->GetForce().y));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBottom()->Physics()->GetForce().z));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getFront()->Physics()->GetForce().x));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getFront()->Physics()->GetForce().y));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getFront()->Physics()->GetForce().z));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBack()->Physics()->GetForce().x));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBack()->Physics()->GetForce().y));
		velocity.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBack()->Physics()->GetForce().z));
		
		velocity.BroadcastPacket(listen->m_pNetwork);
	}

	void sendPacketJump() {

		MySocket jump("JUMP");
		jump.BroadcastPacket(listen->m_pNetwork);
	}
	RenderNode * cam;
	NetworkBase * listen;
	GameObject *body;
	GameObject *ball;
	int myPlayerNum;
	Menu * pauseMenu;
	Menu * activeMenu;
	Menu * soundMenu;
	float Score = 0.0f;
	void ProcessNetworkEvent(const ENetEvent& evnt) {

		if (evnt.type == ENET_EVENT_TYPE_CONNECT)
		{
			printf("Connected to Server\n");
		}
		else if (evnt.type == ENET_EVENT_TYPE_RECEIVE) {
			MySocket Received(evnt.packet);
			if (Received.GetPacketId() == "FIXX") {

				//vector <float> updates;
				//for (int i = 0; i < GameLogic::Instance()->getNumAllPlayers(); i++) {
				//	for (int j = 0; j < 6; j++) {
				//		updates.push_back(stof(Received.TruncPacket(i * 6 + j)));
				//	}
				//}

				//int cntNet = 0;
				//for (int i = 0; i < GameLogic::Instance()->getNumAllPlayers(); i++) {
				//	if (i == myPlayerNum) {
				//		////GameLogic::Instance()->getPlayer(0)->Physics()->SetPosition(nclgl::Maths::Vector3(updates[i * 6 + 3], updates[i * 6 + 4], updates[i * 6 + 5]));
				//		//GameLogic::Instance()->getPlayer(0)->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(updates[i * 6], updates[i * 6 + 1], updates[i * 6 + 2]));
				//	}
				//	else {
				//		////GameLogic::Instance()->getNetPlayer(cntNet)->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(updates[i * 6], updates[i * 6 + 1], updates[i * 6 + 2]));
				//		////GameLogic::Instance()->getNetPlayer(cntNet)->Physics()->SetPosition(nclgl::Maths::Vector3(updates[i * 6 + 3], updates[i * 6 + 4], updates[i * 6 + 5]));
				//		cntNet++;
				//	}

				//}
				sendVelocityUpdate();
			}
			if (Received.GetPacketId() == "NEXT" && GameLogic::Instance()->gameHasStarted()) {
				vector <float> updates;
				for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers() + GameLogic::Instance()->getNumNetPlayers(); i++) {
					for (int j = 0; j < 12; j++) {
						updates.push_back(stof(Received.TruncPacket(i * 12 + j)));
					}
				}
				int cntNet = 0;
				for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers() + GameLogic::Instance()->getNumNetPlayers(); i++) {
					if (i == myPlayerNum) {
				//		//GameLogic::Instance()->getPlayer(0)->Physics()->SetPosition(nclgl::Maths::Vector3(updates[i * 6], updates[i * 6 + 1], updates[i * 6 + 2]));
				//		//GameLogic::Instance()->getPlayer(0)->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(updates[i * 6], updates[i * 6 + 1], updates[i * 6 + 2]));
					}
					else {
						
						forces[cntNet][0] = nclgl::Maths::Vector3(updates[i * 12], updates[i * 12 + 1], updates[i * 12 + 2]);
						forces[cntNet][1] = nclgl::Maths::Vector3(updates[i * 12 + 3], updates[i * 12 + 4], updates[i * 12 + 5]);
						forces[cntNet][2] = nclgl::Maths::Vector3(updates[i * 12 + 6], updates[i * 12 + 7], updates[i * 12 + 8]);
						forces[cntNet][3] = nclgl::Maths::Vector3(updates[i * 12 + 9], updates[i * 12 + 10], updates[i * 12 + 11]);
						//GameLogic::Instance()->getNetPlayer(cntNet)->Physics()->SetPosition(nclgl::Maths::Vector3(updates[i * 6 + 3], updates[i * 6 + 4], updates[i * 6 + 5]));
						cntNet++;
					}

				}
				sendVelocityUpdate();
			}
			else if (Received.GetPacketId() == "JMPP") {
				int cntNet = 0;
				for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers() + GameLogic::Instance()->getNumNetPlayers(); i++) {
					if (i == myPlayerNum) {
						//		//GameLogic::Instance()->getPlayer(0)->Physics()->SetPosition(nclgl::Maths::Vector3(updates[i * 6], updates[i * 6 + 1], updates[i * 6 + 2]));
						//		//GameLogic::Instance()->getPlayer(0)->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(updates[i * 6], updates[i * 6 + 1], updates[i * 6 + 2]));
					}
					else {
						if (i == stoi(Received.TruncPacket(0))) {
							nclgl::Maths::Vector3 jump(0, 10, 0);
							for (int j = 0; j < 182; ++j) {
								GameLogic::Instance()->getNetPlayer(cntNet)->getBall()->softball[j]->Physics()->SetLinearVelocity(GameLogic::Instance()->getNetPlayer(cntNet)->getBall()->softball[j]->Physics()->GetLinearVelocity() + jump);
							}
						}
						cntNet++;
					}

				}
			}
			else if (Received.GetPacketId() == "SDFX") {
				MySocket positionUpdate("PSUD");
				positionUpdate.AddVar(to_string(myPlayerNum));
				positionUpdate.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBottom()->Physics()->GetPosition().x));
				positionUpdate.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBottom()->Physics()->GetPosition().y));
				positionUpdate.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(0)->getBottom()->Physics()->GetPosition().z));
				positionUpdate.BroadcastPacket(listen->m_pNetwork);
			}
			else if (Received.GetPacketId() == "SVUD") {
				int cntNet = 0;
				for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers() + GameLogic::Instance()->getNumNetPlayers(); i++) {
					if (i == myPlayerNum) {
						//		//GameLogic::Instance()->getPlayer(0)->Physics()->SetPosition(nclgl::Maths::Vector3(updates[i * 6], updates[i * 6 + 1], updates[i * 6 + 2]));
						//		//GameLogic::Instance()->getPlayer(0)->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(updates[i * 6], updates[i * 6 + 1], updates[i * 6 + 2]));
					}
					else {
						if (i == stoi(Received.TruncPacket(0))) {
							nclgl::Maths::Vector3 posDifference = nclgl::Maths::Vector3(stof(Received.TruncPacket(1)), stof(Received.TruncPacket(2)), stof(Received.TruncPacket(3))) - GameLogic::Instance()->getNetPlayer(cntNet)->getBottom()->Physics()->GetPosition();
							for (int i = 0; i < 182; ++i) {
								GameLogic::Instance()->getNetPlayer(cntNet)->getBall()->softball[i]->Physics()->SetPosition(GameLogic::Instance()->getNetPlayer(cntNet)->getBall()->softball[i]->Physics()->GetPosition() + posDifference);
							}
						}
						cntNet++;
					}

				}
			}
			enet_packet_destroy(evnt.packet);
		}
		else if (ENET_EVENT_TYPE_DISCONNECT) {
			printf("Disconnected from server");
		}
	};
};