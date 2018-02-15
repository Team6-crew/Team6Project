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


//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	
	float rotation = 0.0f;
	EmptyScene(const std::string& friendly_name) 
		: Scene(friendly_name)
	{
	}

	virtual ~EmptyScene()
	{
		delete player1;
	}

	//WorldPartition *wsp;

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
	
		
		GameLogic::Instance()->addPlayers(4);
		//Add player to scene
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers();i++) {
			this->AddGameObject(GameLogic::Instance()->getPlayer(i));
			this->AddGameObject(GameLogic::Instance()->getPlayer(i)->getBody());
		}

		player1->SetPhysics(player1->Physics());*/

		player1 = new Player("player",
			Vector3(0.0f, 1.f, 0.0f),
			1.0f,
			true,
			1.0f,
			true,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		player1->SetPhysics(player1->Physics());

		//Add player to scene
		this->AddGameObject(player1);
		//Also add body which is used for camera manipulation
		this->AddGameObject(player1->getBody());

		player1->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L, KEYBOARD_SPACE);
		//Who doesn't love finding some common ground?
		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			nclgl::Maths::Vector3(0.0f, -1.5f, 0.0f),
			nclgl::Maths::Vector3(20.0f, 1.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		GLuint trail = SOIL_load_OGL_texture(
			TEXTUREDIR"trail.jpg",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		(*ground->Render()->GetChildIteratorStart())->GetMesh()->SetTexture(trail);
		this->AddGameObject(ground);
		ground->SetTag(Tags::TGround);
		(*ground->Render()->GetChildIteratorStart())->SetTag(Tags::TGround);
		/*this->AddGameObject(CommonUtils::BuildCuboidObject(
			"pickup",
			Vector3(10.0f, 1.f, 0.0f),
			Vector3(0.6f, 0.2f, 0.2f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));*/
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
		NCLDebug::AddHUD(Vector4(0.0f, 0.0f, 0.0f, 1.0f), "Score: " + to_string(Score));
		GameObject *pickup = FindGameObject("pickup");
		rotation = 0.1f;
		if(pickup)
		(*pickup->Render()->GetChildIteratorStart())->SetTransform(nclgl::Maths::Matrix4::Rotation(rotation, 
			nclgl::Maths::Vector3(0, 1, 0))*(*pickup->Render()->GetChildIteratorStart())->GetTransform());
		
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
			GameLogic::Instance()->getPlayer(i)->move(dt);
		}

	}
};