#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\OcTree.h>
#include <ncltech\WorldPartition.h>

//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	EmptyScene(const std::string& friendly_name) 
		: Scene(friendly_name)
	{
	}

	virtual ~EmptyScene()
	{
	}

	WorldPartition *space;

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
		space = new WorldPartition(new AABB(Vector3(0, 20, 0), 20) , 2);

		//Who doesn't love finding some common ground?
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, -1.5f, 0.0f),
			Vector3(20.0f, 1.0f, 20.0f),
			false,
			0.0f,
			false,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

		//this->AddGameObject(CommonUtils::BuildCuboidObject("NorthWall",
		//	Vector3(0.0f, 18.5f, -20.0f),	
		//	Vector3(20.f, 20.f, 0.5f),				
		//	true,									
		//	0.0f,									
		//	true,									
		//	false,									
		//	CommonUtils::GenColor(0.45f, 0.1f)));	

		//this->AddGameObject(CommonUtils::BuildCuboidObject("SouthWall",
		//	Vector3(0.0f, 18.5f, 20.0f),	
		//	Vector3(20.f, 20.f, 0.5f),				
		//	true,									
		//	0.0f,									
		//	true,									
		//	false,									
		//	CommonUtils::GenColor(0.45f, 0.1f)));	

		//
		//this->AddGameObject(CommonUtils::BuildCuboidObject("EastWall",
		//	Vector3(20.0f, 18.5f, 0.0f),	
		//	Vector3(0.5f, 20.f, 20.f),				
		//	true,									
		//	0.0f,									
		//	true,									
		//	false,									
		//	CommonUtils::GenColor(0.45f, 0.1f)));

		//this->AddGameObject(CommonUtils::BuildCuboidObject("WestWall",
		//	Vector3(-20.0f, 18.5f, 0.0f),	
		//	Vector3(0.5f, 20.f, 20.f),				
		//	true,									
		//	0.0f,									
		//	true,									
		//	false,									
		//	CommonUtils::GenColor(0.45f, 0.1f)));	

		//this->AddGameObject(CommonUtils::BuildCuboidObject(
		//	"Ceiling",
		//	Vector3(0.0f, 38.5f, 0.0f),
		//	Vector3(20.0f, 1.0f, 20.0f),
		//	false,
		//	0.0f,
		//	false,
		//	false,
		//	CommonUtils::GenColor(0.45f, 0.1f)));

		
	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);
		space->debugDraw();
	}
};