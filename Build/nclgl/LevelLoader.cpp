#include "LevelLoader.h"
#include <fstream>
#include <sstream>
#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <nclgl\GameLogic.h>
#include <nclgl\ResourceManager.h>

using namespace std;
using namespace nclgl::Maths;

LevelLoader::LevelLoader()
{
}

LevelLoader::~LevelLoader()
{
}

bool LevelLoader::Load(const std::string& filename)
{
	string info;
	ifstream levelfile(filename);
	if (levelfile.is_open()) {
		cout << "File opened" << endl;

		while (getline(levelfile, info)) {
			HandleInput(info);
			//AddMapObject(info);
		}
		levelfile.close();
		return true;
	}
	else {
		cout << "File failed to be read" << endl;
		return false;
	}
}

void LevelLoader::AddMapObject(const std::string& line)
{
	stringstream ss(line);
	MapObject obj;

	ss >> obj.name;

	std::string temp;
	ss >> temp;
	obj.type = StringToType(temp);

	float x, y, z;
	ss >> x >> y >> z;
	obj.position = nclgl::Maths::Vector3(x, y, z);

	ss >> x >> y >> z;
	obj.scale = nclgl::Maths::Vector3(x, y, z);

	ss >> obj.inverseMass;

	float r, g, b, a;
	ss >> r >> g >> b >> a;
	obj.colour = nclgl::Maths::Vector4(r, g, b, a);
	ss >> x >> y >> z;

	obj.rotAxis = nclgl::Maths::Vector3(x, y, z);
	float rot;
	ss >> rot;
	obj.rotationDegrees = rot;

	mapObjects.push_back(obj);
}

Type LevelLoader::StringToType(const std::string& line)
{
	if (line == "GEOMETRY_CUBOID")
	{
		return GEOMETRY_CUBOID;
	}
	if (line == "GEOMETRY_GROUND_CUBOID")
	{
		return GEOMETRY_GROUND_CUBOID;
	}
	if (line == "PAINTABLE_CUBE")
	{
		return PAINTABLE_CUBE;
	}
	else if (line == "GEOMETRY_SPHERE")
	{
		return GEOMETRY_SPHERE;
	}
	else if (line == "GEOMETRY_RAMP")
	{
		return GEOMETRY_RAMP;
	}
	else if (line == "PLAYER_SPAWN")
	{
		return PLAYER_SPAWN;
	}
}



void  LevelLoader::BuildLevel(const std::string& filename, Scene* scene)
{
	Load(filename);

	float step = mapObjects.size()/100.0f;
	float frame = 0.0f;
	int paintable_counter = 0;
	for (auto& object : mapObjects)
	{
		GameObject * geometry;
		switch (object.type)
		{
		case GEOMETRY_GROUND_CUBOID:
			geometry = CommonUtils::BuildGroundCuboidObject(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				false,
				object.colour);

			scene->AddGameObject(geometry);
			if (object.name == "Ground")
			{
				geometry->SetTag(Tags::TGround);
				(*geometry->Render()->GetChildIteratorStart())->SetTag(Tags::TGround);
			}
			break;

		case GEOMETRY_CUBOID:
			geometry = CommonUtils::BuildCuboidObject(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				false,
				object.colour);
			geometry->SetTag(Tags::TCubes);
			(*geometry->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"wall.jpg"), 0);
			scene->AddGameObject(geometry);
			break;

		case PAINTABLE_CUBE:
			geometry = CommonUtils::BuildPaintableCube(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				false,
				object.colour);

			geometry->SetTag(Tags::TPaintable);
			(*geometry->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"paintbox1.jpg"), 0);
			(*geometry->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->MakeTexture("transparent_" + std::to_string(paintable_counter), Texture::COLOUR, 1024, 1024), 1); // increment 'transparent number1' number by 1
			(*geometry->Render()->GetChildIteratorStart())->SetTag(Tags::TPaintable);
		
			scene->AddGameObject(geometry);
			++paintable_counter;
			GameLogic::Instance()->SetPlayerCapturedObject(geometry, -1);
			break;


		case GEOMETRY_SPHERE:
			geometry = CommonUtils::BuildSphereObject(
				object.name,
				object.position,
				object.scale.x,
				true,
				object.inverseMass,
				true,
				false,
				object.colour);

			scene->AddGameObject(geometry);
			break;

		case GEOMETRY_RAMP:
			geometry = CommonUtils::BuildRampObject(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				false,
				object.colour,
				object.rotAxis,
				object.rotationDegrees);

			geometry->SetTag(Tags::TRamp);
			(*geometry->Render()->GetChildIteratorStart())->SetTag(Tags::TRamp);
			(*geometry->Render()->GetChildIteratorStart())->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"ramp.jpg"), 0);
			//geometry->Physics()->SetOrientation(Quaternion::AxisAngleToQuaterion(nclgl::Maths::Vector3(1.0f, 0.0f, 0.0f), 45.0));

			scene->AddGameObject(geometry);
			break;
		}
		frame += step;
		GraphicsPipeline::Instance()->LoadingScreen(frame);
	}


	//scene->OnInitializeScene();
	mapObjects.clear();
	
}


void LevelLoader::HandleInput(const std::string& line)
{
	stringstream ss(line);
	std::string name;
	std::string temp;
	ss >> name;
	ss >> temp;
	Type type = StringToType(temp);
	switch (type)
	{
	case PLAYER_SPAWN:
		AddPlayers(line);
		break;
	default:
		AddMapObject(line);
	}
}

void LevelLoader::AddPlayers(const std::string& line)
{
	stringstream ss(line);
	std::string name;
	ss >> name;

	std::string temp;
	ss >> temp;

	int numPlayers;
	ss >> numPlayers;


	Vector3* spawnPoint = new Vector3[numPlayers];
	for (int i = 0; i < numPlayers; ++i)
	{
		float x, y, z;
		ss >> x >> y >> z;
		spawnPoint[i] = Vector3(x, y, z);
	}


	//GameLogic::Instance()->addPlayer(numPlayers, spawnPoint);
	
	delete[] spawnPoint;
	

}