#pragma once
#include <string>
#include <nclgl\Vector3.h>
#include <nclgl\Vector4.h>
#include <vector>


class Scene;

enum Type {
	GEOMETRY_CUBOID,
	GEOMETRY_ROTATABLE_CUBOID,
	GEOMETRY_GROUND_CUBOID,
	PAINTABLE_CUBE,
	GEOMETRY_SPHERE,
	LAUNCH_PAD,
	WASHING_ZONE,
	PLAYER_SPAWN
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
		nclgl::Maths::Vector3 rotAxis;
		float		rotationDegrees;
		nclgl::Maths::Vector4       colour;
		nclgl::Maths::Vector3 ramprotaxis;
		float ramprotdeg;
	};

public:
	LevelLoader();
	~LevelLoader();

	void BuildLevel(const std::string& filename, Scene* scene);
	inline void DeleteMapObjects() { mapObjects.clear(); }
protected:
	void AddMapObject(const std::string& line);
	void AddPlayers(const std::string& line);
	bool Load(const std::string& filename);

	
	std::vector<MapObject> mapObjects;

protected:
	Type StringToType(const std::string& line);
	void HandleInput(const std::string& line);
};

