#pragma once
#include <string>
#include <nclgl\Vector3.h>
#include <nclgl\Vector4.h>

#include <vector>


class Scene;

enum Type {
	GEOMETRY_CUBOID,
	GEOMETRY_SPHERE,
	GEOMETRY_RAMP,
};


class LevelLoader
{
	struct MapObject
	{
		std::string name;
		Type		type;
		nclgl::Maths::Vector3		position;
		nclgl::Maths::Vector3		scale;
		float		inverseMass;
		float		rotationDegrees;
	};

public:
	LevelLoader();
	~LevelLoader();

	void BuildLevel(const std::string& filename, Scene* scene);

protected:
	void AddMapObject(const std::string& line);
	bool Load(const std::string& filename);

	std::vector<MapObject> mapObjects;

protected:
	Type StringToType(const std::string& line);
};

