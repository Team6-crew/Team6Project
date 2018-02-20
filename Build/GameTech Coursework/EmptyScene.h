#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\OcTree.h>
#include "MainMenu.h"
//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:

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
	}

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
		OcTree *space = new OcTree(new AABB(Vector3(0, 20, 0), 20));

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

		this->AddGameObject(CommonUtils::BuildSphereObject("ball",
			Vector3(0.0f, 1.0f, 0.0f),				//Position leading to 0.25 meter overlap on faces, and more on diagonals
			1.0f,									//Half dimensions
			true,									//Has Physics Object
			1.0f,									//Mass
			true,									//Has Collision Shape
			true,									//Dragable by the user
			CommonUtils::GenColor(0.45f, 0.5f)));	//Color

		this->AddGameObject(CommonUtils::BuildCuboidObject("body",
			Vector3(0.0f, 2.0f, 0.0f),				//Position leading to 0.25 meter overlap on faces, and more on diagonals
			Vector3(0.5, 0.5, 0.5),					//Half dimensions
			false,									//Has Physics Object
			0.0f,									//Mass
			false,									//Has Collision Shape
			false,									//Dragable by the user
			CommonUtils::GenColor(0.45f, 0.5f)));	//Color

		cam = new RenderNode();
		cam->SetTransform(Matrix4::Translation(Vector3(0, 10, 25)));
		body = FindGameObject("body");
		ball = FindGameObject("ball");
		(*body->Render()->GetChildIteratorStart())->AddChild(cam);
		(*body->Render()->GetChildIteratorStart())->SetMesh(NULL);
	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);

		NCLDebug::AddHUD(Vector4(0.0f, 0.0f, 0.0f, 1.0f), "Score: " + to_string(Score));

		Vector3 ball_pos = ball->Physics()->GetPosition();
		Camera * camera = GraphicsPipeline::Instance()->GetCamera();

		Vector3 forward = (camera->GetPosition() - ball_pos).Normalise();

		Matrix4 worldTr = (*body->Render()->GetChildIteratorStart())->GetWorldTransform();
		worldTr.SetPositionVector(ball_pos + Vector3(0, 2, 0));
		(*body->Render()->GetChildIteratorStart())->SetTransform(worldTr);

		float yaw = camera->GetYaw();
		float pitch = camera->GetPitch();


		ball->Physics()->SetForce(Vector3(0, 0, 0));

		float rotation = 0.0f;

		float power = 15.0f;


		//Ball Controls
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
		{
			ball->Physics()->SetForce(-forward * power);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
		{
			ball->Physics()->SetForce(forward * power);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
		{
			rotation = 0.5f;
			camera->SetYaw(yaw + rotation);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
		{
			rotation = -0.5f;
			camera->SetYaw(yaw + rotation);
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C))
		{
			Score += 1.0f; 
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
		else if (pauseMenu->getSelection() == 1 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			cout << "Sound";
		}
		else if (pauseMenu->getSelection() == 2 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			cout << "Controls";
		}
		else if (pauseMenu->getSelection() == 3 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			pauseMenu->visible = false;
			PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
			SceneManager::Instance()->JumpToScene("Main Menu");
			
		}
		else if (pauseMenu->getSelection() == 4 && Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN))
		{
			exit(0);
		}

		camera->SetPosition(cam->GetWorldTransform().GetPositionVector());

	}

private:

	RenderNode * cam;

	GameObject *body;
	GameObject *ball;

	Menu * pauseMenu;

	float Score = 0.0f;

};