#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\OcTree.h>
#include "../nclgl/OGLRenderer.h"
#include "../ncltech/ScreenPicker.h"
#include "../nclgl/Menu.h"

#include <ncltech\GameObject.h>
#include <ncltech\CommonMeshes.h>
#include <ncltech\CommonUtils.h>


class MainMenu : public Scene
{
public:
	
	MainMenu(const std::string& friendly_name)
		: Scene(friendly_name)
	{
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

		//Background
		tex = SOIL_load_OGL_texture(
			TEXTUREDIR"target.tga",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	virtual ~MainMenu()
	{
	}

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
		
		cam = new RenderNode();
		cam->SetTransform(Matrix4::Translation(Vector3(0, 10, 25)));
		
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
		if (activeMenu == mainMenu) {
			mainMenu->ShowMenu();
		}
		else if (activeMenu == optionsMenu) {
			optionsMenu->ShowMenu();
		}
		else if (activeMenu == resolutionMenu) {
			resolutionMenu->ShowMenu();
		}
		else if (activeMenu == controlsMenu) {
			controlsMenu->ShowMenu();
		}
		else if (activeMenu == soundMenu) {
			soundMenu->ShowMenu();
		}
		else if (activeMenu == multiPlayerMenu) {
			multiPlayerMenu->ShowMenu();
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


		//Change Menus
		if (activeMenu->getSelection() == 0 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == mainMenu) {
				SceneManager::Instance()->JumpToScene("Team Project");
			}
			else if (activeMenu == multiPlayerMenu) {
				cout << "Split-Screen set.";
			}
			else if (activeMenu == optionsMenu) {
				activeMenu = resolutionMenu;
			}
			else if (activeMenu == resolutionMenu) {
				cout << "1920 x 1080 set.";
			}
			else if (activeMenu == soundMenu) {
				cout << "volume set.";
			}
			else if (activeMenu == controlsMenu) {
				cout << "Player 1 controls set.";
			}

		}

		if (activeMenu->getSelection() == 1 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == mainMenu) {
				activeMenu = multiPlayerMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == multiPlayerMenu) {
				cout << "Host LAN server set.";
			}
			else if (activeMenu == optionsMenu) {
				activeMenu = soundMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == resolutionMenu) {
				cout << "1600 x 900 set.";
			}
			else if (activeMenu == soundMenu) {
				activeMenu = optionsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == controlsMenu) {
				cout << "Player 2 controls set.";
			}
		}

		if (activeMenu->getSelection() == 2 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == mainMenu) {
				activeMenu = optionsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == multiPlayerMenu) {
				cout << "Join LAN server set.";
			}
			else if (activeMenu == optionsMenu) {
				activeMenu = controlsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == resolutionMenu) {
				cout << "1366 x 768 set.";
			}
			else if (activeMenu == controlsMenu) {
				cout << "Player 3 controls set.";
			}
		}

		if (activeMenu->getSelection() == 3 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
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
				cout << "1280 x 720 set.";
			}
			else if (activeMenu == controlsMenu) {
				cout << "Player 4 controls set.";
			}
		}

		if (activeMenu->getSelection() == 4 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			if (activeMenu == resolutionMenu) {
				activeMenu = optionsMenu;
				activeMenu->setSelection(0);
			}
			else if (activeMenu == controlsMenu) {
				activeMenu = optionsMenu;
				activeMenu->setSelection(0);
			}
		}
		
		//GameObject* target1 = CommonUtils::BuildCuboidObject(
		//	"Target1",										// Optional: Name
		//	Vector3(0.0f, 2.0f, -3.5f),						// Position
		//	Vector3(15.0f, 10.0f, 1.0f),					// Half-Dimensions
		//	false,											// Physics Enabled?
		//	0.0f,											// Physical Mass (must have physics enabled)
		//	false,											// Physically Collidable (has collision shape)
		//	false,											// Dragable by user?
		//	Vector4(0.0f, 0.0f, 0.0f, 1.0f));				// Render color
		//this->AddGameObject(target1);
		//(*target1->Render()->GetChildIteratorStart())->GetMesh()->SetTexture(tex);


		//backgroundMesh = Mesh::GenerateQuad();
		//backgroundMesh = new Mesh(*image);
		//backgroundMesh->SetTexture(tex);

		RenderNode* background = new RenderNode();
		background->SetMesh(backgroundMesh);
		background->SetTransform(Matrix4::Translation(Vector3(0.0f, 2.0f, -3.5f)) * Matrix4::Scale(Vector3(1.0f, 1.0f, 1.0f)));
		background->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		this->AddGameObject(new GameObject("Target", background, NULL));

	}
private:
	GLuint	tex;

	Mesh* backgroundMesh;

	RenderNode * cam;
	RenderNode * backTexture;

	// Menus definition
	Menu * mainMenu;
	Menu * optionsMenu;
	Menu * resolutionMenu;
	Menu * soundMenu;
	Menu * multiPlayerMenu;
	Menu * controlsMenu;
	Menu * activeMenu;
};

