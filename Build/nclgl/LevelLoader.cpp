#include "LevelLoader.h"

#include <fstream>
#include <sstream>

#include <ncltech\Scene.h>

using namespace std;

#include <ncltech\CommonUtils.h>

LevelLoader::LevelLoader()
{
}


LevelLoader::~LevelLoader()
{
}

bool LevelLoader::Load(const std::string& filename)
{
	string info;
	ifstream levelfile("SimpleLevel.txt");
	if (levelfile.is_open()) {
		cout << "File opened" << endl;
		while (getline(levelfile, info)) {
			AddMapObject(info);
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

	obj.rotationDegrees;

	

	mapObjects.push_back(obj);
}

Type LevelLoader::StringToType(const std::string& line)
{
	if (line == "GEOMETRY_CUBOID")
	{
		return GEOMETRY_CUBOID;
	}
	else if (line == "GEOMETRY_SPHERE")
	{
		return GEOMETRY_SPHERE;
	}
	else if (line == "GEOMETRY_RAMP")
	{
		return GEOMETRY_RAMP;
	}
}

void  LevelLoader::BuildLevel(const std::string& filename, Scene* scene)
{
	Load(filename);
	
	for (auto& object : mapObjects)
	{
		GameObject * geometry;
		switch (object.type)
		{
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

			scene->AddGameObject(geometry);
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
				object.rotationDegrees
				);

			scene->AddGameObject(geometry);
			break;
		}
	}
	scene->OnInitializeScene();
	mapObjects.clear();
}


