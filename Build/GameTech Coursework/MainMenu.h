#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\OcTree.h>
#include "../ncltech/ScreenPicker.h"
#include "Menu.h"
#include <map>
#include <ncltech\GameObject.h>
#include <ncltech\CommonMeshes.h>
#include <ncltech\CommonUtils.h>

#include <nclgl\Graphics\TextureBase.h>
#include <nclgl\Graphics\Renderer\RenderNodeBase.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <nclgl\Graphics\Renderer\TextureFactory.h>
#include <nclgl\Graphics\MeshBase.h>
#include <nclgl\Graphics\Renderer\OpenGL\OGLMesh.h>
#include <fstream>

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
		waitsInput = false;
		mapKeys();
		//std::cout << glGetError() << std::endl;
		numOfPlayers = 0;
		numOfAi = 0;
		player_c = -1;
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
		controlsMenu->addToMenu(optionsMenu, 4);
		controlsMenu->set_id(7);

		getControls();

		// Player 1 Menu
		playerControlsMenu = new Menu();
		playerControlsMenu->AddMenuItem("Foward");
		playerControlsMenu->AddMenuItem("Backward");
		playerControlsMenu->AddMenuItem("Turn Left");
		playerControlsMenu->AddMenuItem("Turn Right");
		playerControlsMenu->AddMenuItem("Jump");
		playerControlsMenu->AddMenuItem("Shoot");
		playerControlsMenu->AddMenuItem("Back");
		controlsMenu->addToMenu(playerControlsMenu, 0);
		controlsMenu->addToMenu(playerControlsMenu, 1);
		controlsMenu->addToMenu(playerControlsMenu, 2);
		controlsMenu->addToMenu(playerControlsMenu, 3);
		playerControlsMenu->addToMenu(controlsMenu, 6);
		playerControlsMenu->set_id(8);

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
		if (waitsInput && !Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN)) {
			for (std::map<int, std::string>::iterator it = KEYS.begin(); it != KEYS.end(); ++it) {
				if (Window::GetKeyboard()->KeyTriggered((KeyboardKeys)it->first)) {
					playerControlsMenu->replaceMenuItem(activeMenu->getSelection(), con[activeMenu->getSelection()]
						+ "	" + it->second);
					replaceControl(it->first, player_c * 4 + activeMenu->getSelection());
					waitsInput = false;
				}
			}
		}

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
				case (700): {
					player_c = 0;
					show_controls();
					break;
				}
				case (701): {
					player_c = 1;
					show_controls();
					break;
				}
				case (702): {
					player_c = 2;
					show_controls();
					break;
				}
				case (703): {
					player_c = 3;
					show_controls();
					break;
				}
				case (800): case (801):	case (802):	case (803):	case (804):	case (805):
				{
					waitsInput = !waitsInput;
					if (waitsInput == false) playerControlsMenu->replaceMenuItem(activeMenu->getSelection(), con[activeMenu->getSelection()] 
						+ "	" + KEYS[GameLogic::Instance()->getControls(player_c, activeMenu->getSelection())]);
					else
						playerControlsMenu->replaceMenuItem(activeMenu->getSelection(), "");
					break;
				}
				
			}

		}


	}
private:
	TextureBase*	tex;

	OGLMesh* backgroundMesh;
	std::string con[6] = {"Forward", "Backward", "Turn Left", "Turn Right", "Jump", "Shoot"};
	RenderNodeBase * cam;
	RenderNodeBase * backTexture;
	int player_c;
	// Menus definitions
	Menu * mainMenu;
	Menu * optionsMenu;
	Menu * resolutionMenu;
	Menu * soundMenu;
	Menu * multiPlayerMenu;
	Menu * controlsMenu;
	Menu * splitScreenMenu;
	Menu * addPlayerMenu;
	bool waitsInput;
	Menu * activeMenu;
	Menu * activeSubmenu;
	Menu * playerControlsMenu;
	std::map <int, std::string> KEYS;

	void replaceControl(int con, int line_num) {
		std::string line;
		std::ifstream filein("../../Data/controls.txt");
		std::ofstream myfile("../../Data/temp.txt");
		int i = 0;
		std::string strTemp;
		if (myfile.is_open())
		{
			while (filein >> strTemp)
			{
				if (i == line_num) {
					strTemp = std::to_string(con);
				}
				strTemp += "\n";
				myfile << strTemp;
				i++;
			}
			myfile.close();
			filein.close();
		}
		else std::cout << "Unable to open file";
		if (remove("../../Data/controls.txt") != 0)
			perror("Error deleting file");
		else
			puts("File successfully deleted");
		if (rename("../../Data/temp.txt","../../Data/controls.txt") != 0)
			perror("Error deleting file");
		else
			puts("File successfully deleted");
	}
	int getControls() {
		std::string line;
		std::ifstream myfile("../../Data/controls.txt");
		int i = 0;
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				GameLogic::Instance()->setControls(i/6,i%6,(KeyboardKeys)(stoi(line)));
				i++;
			}
			myfile.close();
		}

		else std::cout << "Unable to open file";

		return 0;
	}

	void show_controls() {
		std::string line;
		std::ifstream myfile("../../Data/controls.txt");
		int i = 0;
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				if (i / 6 == player_c) {
					playerControlsMenu->replaceMenuItem(i%6, con[i % 6]+"	"+KEYS[stoi(line)]);
				}
				i++;
			}
			myfile.close();
		}

		else std::cout << "Unable to open file";
	}
	void mapKeys() {
		KEYS[8] = "BACKSPACE";
		KEYS[9] = "TAB";
		KEYS[19] = "PAUSE";
		KEYS[20] = "CAPS LOCK";
		KEYS[32] = "SPACE";
		KEYS[33] = "PAGE UP";
		KEYS[34] = "PAGE DOWN";
		KEYS[35] = "END";
		KEYS[36] = "HOME";
		KEYS[37] = "LEFT";
		KEYS[38] = "UP";
		KEYS[39] = "RIGHT";
		KEYS[40] = "DOWN";
		KEYS[44] = "PRT SCN";
		KEYS[45] = "INSERT";
		KEYS[46] = "DELETE";
		KEYS[48] = "0";
		KEYS[49] = "1";
		KEYS[50] = "2";
		KEYS[51] = "3";
		KEYS[52] = "4";
		KEYS[53] = "5";
		KEYS[54] = "6";
		KEYS[55] = "7";
		KEYS[56] = "8";
		KEYS[57] = "9";
		KEYS[65] = "A";
		KEYS[66] = "B";
		KEYS[67] = "C";
		KEYS[68] = "D";
		KEYS[69] = "E";
		KEYS[70] = "F";
		KEYS[71] = "G";
		KEYS[72] = "H";
		KEYS[73] = "I";
		KEYS[74] = "J";
		KEYS[75] = "K";
		KEYS[76] = "L";
		KEYS[77] = "M";
		KEYS[78] = "N";
		KEYS[79] = "O";
		KEYS[80] = "P";
		KEYS[81] = "Q";
		KEYS[82] = "R";
		KEYS[83] = "S";
		KEYS[84] = "T";
		KEYS[85] = "U";
		KEYS[86] = "V";
		KEYS[87] = "W";
		KEYS[88] = "X";
		KEYS[89] = "Y";
		KEYS[90] = "Z";

		KEYS[96] = "NUM 0";
		KEYS[97] = "NUM 1";
		KEYS[98] = "NUM 2";
		KEYS[99] = "NUM 3";
		KEYS[100] = "NUM 4";
		KEYS[101] = "NUM 5";
		KEYS[102] = "NUM 6";
		KEYS[103] = "NUM 7";
		KEYS[104] = "NUM 8";
		KEYS[105] = "NUM 9";
		KEYS[106] = "*";
		KEYS[107] = "NUM +";
		KEYS[108] = "|";
		KEYS[109] = "NUM -";
		KEYS[110] = "DOT";
		KEYS[111] = "/";

		KEYS[112] = "F1";
		KEYS[113] = "F2";
		KEYS[114] = "F3";
		KEYS[115] = "F4";
		KEYS[116] = "F5";
		KEYS[117] = "F6";
		KEYS[118] = "F7";
		KEYS[119] = "F8";
		KEYS[120] = "F9";
		KEYS[121] = "F10";
		KEYS[122] = "F11";
		KEYS[123] = "F12";

		KEYS[144] = "NUM LOCK";
		KEYS[145] = "SCROLL LOCK";
		KEYS[160] = "L SHIFT";
		KEYS[161] = "R SHIFT";
		KEYS[162] = "L CTRL";
		KEYS[163] = "R CTRL";
		KEYS[164] = "L ALT";
		KEYS[165] = "R ALT";

		KEYS[187] = "+";
		KEYS[188] = ",";
		KEYS[189] = "-";
		KEYS[190] = ".";

	}
};

