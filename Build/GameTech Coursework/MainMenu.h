#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\OcTree.h>
#include "../ncltech/ScreenPicker.h"
#include "Menu.h"

#include <ncltech\GameObject.h>
#include <ncltech\CommonMeshes.h>
#include <ncltech\CommonUtils.h>

#include <nclgl\Graphics\TextureBase.h>
#include <nclgl\Graphics\Renderer\RenderNodeBase.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <nclgl\Graphics\Renderer\TextureFactory.h>
#include <nclgl\Graphics\MeshBase.h>
#include <nclgl\Graphics\Renderer\OpenGL\OGLMesh.h>

class MainMenu : public Scene
{
public:
	int numOfPlayers;
	int numOfAi;


	MainMenu(const std::string& friendly_name)
		: Scene(friendly_name)
	{
		//std::cout << glGetError() << std::endl;
		numOfPlayers = 0;
		numOfAi = 0;

		// Start Screen - Main Menu
		mainMenu = new Menu();
		mainMenu->visible = true;
		mainMenu->AddMenuItem("Single Player");
		mainMenu->AddMenuItem("MultiPlayer");
		mainMenu->AddMenuItem("Options");
		mainMenu->AddMenuItem("Exit");
		activeMenu = mainMenu;

		// Multi Player Menu
		multiPlayerMenu = new Menu();
		multiPlayerMenu->visible = false;
		multiPlayerMenu->AddMenuItem("Split Screen");
		multiPlayerMenu->AddMenuItem("Host LAN Server");
		multiPlayerMenu->AddMenuItem("Join LAN Server");
		multiPlayerMenu->AddMenuItem("Back");

		// Split Screen Menu
		splitScreenMenu = new Menu();
		splitScreenMenu->visible = false;
		splitScreenMenu->AddMenuItem("< Player 1 - Press 1 >");
		splitScreenMenu->AddMenuItem("< Player 2 - Press 2 >");
		splitScreenMenu->AddMenuItem("< Player 3 - Press 3 >");
		splitScreenMenu->AddMenuItem("< Player 4 - Press 4 >");
		splitScreenMenu->AddMenuItem("Start Game");
		splitScreenMenu->AddMenuItem("Back");

		//addPlayerMenu = new Menu();
		//addPlayerMenu->visible = false;
		//addPlayerMenu->AddMenuItem("Add Human Player");
		//addPlayerMenu->AddMenuItem("Add AI Player");
		//addPlayerMenu->AddMenuItem("Back");

		// Options Menu
		optionsMenu = new Menu();
		optionsMenu->visible = false;
		optionsMenu->AddMenuItem("Resolution");
		optionsMenu->AddMenuItem("Sound");
		optionsMenu->AddMenuItem("Controls");
		optionsMenu->AddMenuItem("Back");
		optionsMenu->setSelection(0);

		// Resolution Menu
		resolutionMenu = new Menu();
		resolutionMenu->visible = false;
		resolutionMenu->AddMenuItem("1920 x 1080");
		resolutionMenu->AddMenuItem("1600 x 900");
		resolutionMenu->AddMenuItem("1366 x 768");
		resolutionMenu->AddMenuItem("1280 x 720");
		resolutionMenu->AddMenuItem("Back");
		resolutionMenu->setSelection(0);

		// Sound Menu
		soundMenu = new Menu();
		soundMenu->visible = false;
		soundMenu->AddMenuItem("Volume");
		soundMenu->AddMenuItem("Back");

		// Controls Menu
		controlsMenu = new Menu();
		controlsMenu->visible = false;
		controlsMenu->AddMenuItem("Player 1 Controls");
		controlsMenu->AddMenuItem("Player 2 Controls");
		controlsMenu->AddMenuItem("Player 3 Controls");
		controlsMenu->AddMenuItem("Player 4 Controls");
		controlsMenu->AddMenuItem("Back");


	}

	virtual ~MainMenu()
	{
	}

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
		GraphicsPipeline::Instance()->CreateNewCamera();
		cam = RenderNodeFactory::Instance()->MakeRenderNode();
		cam->SetTransform(nclgl::Maths::Matrix4::Translation(nclgl::Maths::Vector3(0, 10, 25)));
		
	}


	virtual void OnUpdateScene(float dt) override
	{

		Scene::OnUpdateScene(dt);
		Camera * camera = GraphicsPipeline::Instance()->GetCamera();

		float yaw = camera->GetYaw();
		float pitch = camera->GetPitch();

		pitch = 60.0f;

		float rotation = 0.0f;

		float power = 15.0f;


		//Show Menus
		activeMenu->ShowMenu();


		//Navigate choices
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_UP))
		{
			activeMenu->MoveUp();
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_DOWN))
		{
			activeMenu->MoveDown();
		}

		if (activeMenu == splitScreenMenu) {
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
			{
				numOfPlayers += 1;
				activeMenu->replaceMenuItem(0, "Player 1 Ready");
			}
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
			{
				numOfPlayers += 1;
				activeMenu->replaceMenuItem(1, "Player 2 Ready");
			}
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3))
			{
				numOfPlayers += 1;
				activeMenu->replaceMenuItem(2, "Player 3 Ready");
			}
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_4))
			{
				numOfPlayers += 1;
				activeMenu->replaceMenuItem(3, "Player 4 Ready");
			}
		}
		//Change Menus
		if (activeMenu->getSelection() == 0 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == mainMenu) {
				
				GraphicsPipeline::Instance()->ChangeScene();
				SceneManager::Instance()->JumpToScene("Team Project");
				
				//activeMenu->setSelection(-1);
				activeMenu = NULL;
				
			}
			else if (activeMenu == multiPlayerMenu) {
				activeMenu = splitScreenMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == optionsMenu) {
				activeMenu = resolutionMenu;
			}
			else if (activeMenu == resolutionMenu) {
				std::cout << "1920 x 1080 set.";
			}
			else if (activeMenu == soundMenu) {
				std::cout << "volume set.";
			}
			else if (activeMenu == controlsMenu) {
				std::cout << "Player 1 controls set.";
			}
			

		}

		else if (activeMenu->getSelection() == 1 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == mainMenu) {
				activeMenu = multiPlayerMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == multiPlayerMenu) {
				std::cout << "Host LAN server set.";
			}
			else if (activeMenu == optionsMenu) {
				activeMenu = soundMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == resolutionMenu) {
				std::cout << "1600 x 900 set.";
			}
			else if (activeMenu == soundMenu) {
				activeMenu = optionsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == controlsMenu) {
				std::cout << "Player 2 controls set.";
			}
			else if (activeMenu == splitScreenMenu) {
				std::cout << "3 Players.";
			}
		}

		else if (activeMenu->getSelection() == 2 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == mainMenu) {
				activeMenu = optionsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == multiPlayerMenu) {
				std::cout << "Join LAN server set.";
			}
			else if (activeMenu == optionsMenu) {
				activeMenu = controlsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == resolutionMenu) {
				std::cout << "1366 x 768 set.";
			}
			else if (activeMenu == controlsMenu) {
				std::cout << "Player 3 controls set.";
			}
			else if (activeMenu == splitScreenMenu) {
				std::cout << "4 Players.";
			}
		}

		else if (activeMenu->getSelection() == 3 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == mainMenu) {
				exit(0);
			}
			else if (activeMenu == multiPlayerMenu) {
				activeMenu = mainMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == optionsMenu) {
				activeMenu = mainMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == resolutionMenu) {
				std::cout << "1280 x 720 set.";
			}
			else if (activeMenu == controlsMenu) {
				std::cout << "Player 4 controls set.";
			}
			else if (activeMenu == splitScreenMenu) {
				activeMenu = multiPlayerMenu;
			}
		}

		else if (activeMenu->getSelection() == 4 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == resolutionMenu) {
				activeMenu = optionsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == controlsMenu) {
				activeMenu = optionsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == splitScreenMenu) {
				GameLogic::Instance()->setnumOfPlayersMp(numOfPlayers);
				GraphicsPipeline::Instance()->ChangeScene();
				SceneManager::Instance()->JumpToScene("Team Project");
				
				activeMenu = NULL;
			}
		}
		else if (activeMenu->getSelection() == 5 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			activeMenu = multiPlayerMenu;
		}
	}
private:
	TextureBase*	tex;

	OGLMesh* backgroundMesh;

	RenderNodeBase * cam;
	RenderNodeBase * backTexture;

	// Menus definition
	Menu * mainMenu;
	Menu * optionsMenu;
	Menu * resolutionMenu;
	Menu * soundMenu;
	Menu * multiPlayerMenu;
	Menu * controlsMenu;
	Menu * splitScreenMenu;
	Menu * addPlayerMenu;

	Menu * activeMenu;
	Menu * activeSubmenu;
};

