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

	int humanOrAi[4] = { 0 };
	int playerSelection = 0;

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
		mainMenu->set_id(1);
		// Multi Player Menu
		multiPlayerMenu = new Menu();
		multiPlayerMenu->visible = false;
		multiPlayerMenu->AddMenuItem("Split Screen");
		multiPlayerMenu->AddMenuItem("Host LAN Server");
		multiPlayerMenu->AddMenuItem("Join LAN Server");
		multiPlayerMenu->AddMenuItem("Back");
		mainMenu->addToMenu(multiPlayerMenu, 1);
		multiPlayerMenu->addToMenu(mainMenu, 3);
		multiPlayerMenu->set_id(2);
		// Split Screen Menu
		splitScreenMenu = new Menu();
		splitScreenMenu->visible = false;
		splitScreenMenu->AddMenuItem(" Player 1 None > ");
		splitScreenMenu->AddMenuItem(" Player 2 None > ");
		splitScreenMenu->AddMenuItem(" Player 3 None > ");
		splitScreenMenu->AddMenuItem(" Player 4 None > ");
		splitScreenMenu->AddMenuItem("Start Game");
		splitScreenMenu->AddMenuItem("Back");
		multiPlayerMenu->addToMenu(splitScreenMenu, 0);
		splitScreenMenu->addToMenu(multiPlayerMenu, 5);
		splitScreenMenu->set_id(3);

		// Options Menu
		optionsMenu = new Menu();
		optionsMenu->visible = false;
		optionsMenu->AddMenuItem("Resolution");
		optionsMenu->AddMenuItem("Sound");
		optionsMenu->AddMenuItem("Controls");
		optionsMenu->AddMenuItem("Back");
		optionsMenu->setSelection(0);
		mainMenu->addToMenu(optionsMenu, 2);
		optionsMenu->addToMenu(mainMenu, 3);
		optionsMenu->set_id(4);
		// Resolution Menu
		resolutionMenu = new Menu();
		resolutionMenu->visible = false;
		resolutionMenu->AddMenuItem("1920 x 1080");
		resolutionMenu->AddMenuItem("1600 x 900");
		resolutionMenu->AddMenuItem("1366 x 768");
		resolutionMenu->AddMenuItem("1280 x 720");
		resolutionMenu->AddMenuItem("Back");
		resolutionMenu->setSelection(0);
		optionsMenu->addToMenu(resolutionMenu, 0);
		resolutionMenu->addToMenu(optionsMenu, 4);
		resolutionMenu->set_id(5);
		// Sound Menu
		soundMenu = new Menu();
		soundMenu->visible = false;
		soundMenu->AddMenuItem("Volume");
		soundMenu->AddMenuItem("Back");
		optionsMenu->addToMenu(soundMenu, 1);
		soundMenu->addToMenu(optionsMenu, 1);
		soundMenu->set_id(6);
		// Controls Menu
		controlsMenu = new Menu();
		controlsMenu->visible = false;
		controlsMenu->AddMenuItem("Player 1 Controls");
		controlsMenu->AddMenuItem("Player 2 Controls");
		controlsMenu->AddMenuItem("Player 3 Controls");
		controlsMenu->AddMenuItem("Player 4 Controls");
		controlsMenu->AddMenuItem("Back");
		optionsMenu->addToMenu(controlsMenu, 2);
		controlsMenu->addToMenu(controlsMenu, 2);
		controlsMenu->set_id(7);
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

		if (activeMenu->get_id() == 3 && activeMenu->getSelection() < 4) {
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT)) { humanOrAi[activeMenu->getSelection()] = max(humanOrAi[activeMenu->getSelection()] - 1, 0); }
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) { humanOrAi[activeMenu->getSelection()] = min(humanOrAi[activeMenu->getSelection()] + 1, 2); }
			if (humanOrAi[activeMenu->getSelection()] == 0) { activeMenu->replaceMenuItem(activeMenu->getSelection(), " Player " + std::to_string(activeMenu->getSelection() + 1) + " None > "); }
			else if (humanOrAi[activeMenu->getSelection()] == 1) { activeMenu->replaceMenuItem(activeMenu->getSelection(), "< Human Player " + std::to_string(activeMenu->getSelection() + 1) + " > "); }
			else if (humanOrAi[activeMenu->getSelection()] == 2) { activeMenu->replaceMenuItem(activeMenu->getSelection(), "< CPU Player " + std::to_string(activeMenu->getSelection() + 1) + " "); }
		}
		// MainMenu : 1
		// MutiplayerMenu : 2
		// SplitScreen : 3
		// Main menu selection 0 is 100 (id = 1 & selection = 00)
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN)) {
			int id = activeMenu->get_id() * 100 + activeMenu->getSelection();
			if (activeMenu->onMenuSelect() != NULL) {
				activeMenu = activeMenu->onMenuSelect();
				activeMenu->setSelection(0);
			}
			switch (id) {
				case (100): {
					GameLogic::Instance()->setnumOfPlayersMp(numOfPlayers);
					GraphicsPipeline::Instance()->ChangeScene();
					SceneManager::Instance()->JumpToScene("Team Project");
					break;
				}
				case (103): {
					exit(0);
					break;
				}
				case (304): { // change to reflect how many players are given
					for (int i = 0; i < sizeof(humanOrAi) / sizeof(humanOrAi[0]); ++i) {
						if (humanOrAi[i] == 1) {
							numOfPlayers += pow (2,i);

						}
						else if (humanOrAi[i] == 2) {
							numOfAi++;
						}
					}
					if (numOfPlayers != 0) {
						GameLogic::Instance()->setnumOfPlayersMp(numOfPlayers);
						GraphicsPipeline::Instance()->ChangeScene();
						SceneManager::Instance()->JumpToScene("Team Project");
						break;
					}
					else {
						break;
					}
				}
				case (500): {
					std::cout << "1920 x 1080 set.";
					break;
				}
				case (501): {
					std::cout << "1600 x 900 set.";
					break;
				}
				case (502): {
					std::cout << "1366 x 768 set.";
					break;
				}
				case (503): {
					std::cout << "1280 x 720 set.";
					break;
				}
				case (600): {
					std::cout << "Volume set.";
					break;
				}
			}

		}
		//Change Menus
		//if (activeMenu->getSelection() == 0 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		//{

		//	//if (activeMenu == resolutionMenu) {
		//	//	std::cout << "1920 x 1080 set.";
		//	//}
		//	//if (activeMenu == soundMenu) {
		//	//	std::cout << "volume set.";
		//	//}
		//	//else if (activeMenu == controlsMenu) {
		//	//	std::cout << "Player 1 controls set.";
		//	//}
		//	//else if (activeMenu == splitScreenMenu)
		//	//{
		//		
		//	//}

		//}

		//else if (activeMenu->getSelection() == 1 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		//{

		//	if (activeMenu == multiPlayerMenu) {
		//		std::cout << "Host LAN server set.";
		//	}

		//	else if (activeMenu == resolutionMenu) {
		//		std::cout << "1600 x 900 set.";
		//	}
		//	else if (activeMenu == controlsMenu) {
		//		std::cout << "Player 2 controls set.";
		//	}
		//	else if (activeMenu == splitScreenMenu) {
		//		std::cout << "3 Players.";
		//	}
		//}

		//else if (activeMenu->getSelection() == 2 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		//{
		//	if (activeMenu == multiPlayerMenu) {
		//		std::cout << "Join LAN server set.";
		//	}
		//	else if (activeMenu == resolutionMenu) {
		//		std::cout << "1366 x 768 set.";
		//	}
		//	else if (activeMenu == controlsMenu) {
		//		std::cout << "Player 3 controls set.";
		//	}
		//	else if (activeMenu == splitScreenMenu) {
		//		std::cout << "4 Players.";
		//	}
		//}

		//else if (activeMenu->getSelection() == 3 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		//{
		///*	if (activeMenu == mainMenu) {
		//		exit(0);
		//	}*/
		//	if (activeMenu == resolutionMenu) {
		//		std::cout << "1280 x 720 set.";
		//	}
		//	else if (activeMenu == controlsMenu) {
		//		std::cout << "Player 4 controls set.";
		//	}
		//}

		//else if (activeMenu->getSelection() == 4 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		//{

		//	//if (activeMenu == splitScreenMenu) {
		//	//	GameLogic::Instance()->setnumOfPlayersMp(numOfPlayers);
		//	//	GraphicsPipeline::Instance()->ChangeScene();
		//	//	SceneManager::Instance()->JumpToScene("Team Project");
		//	//	
		//	//	activeMenu = NULL;
		//	//}
		//}

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

