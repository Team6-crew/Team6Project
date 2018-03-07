#pragma once
#include <string>
#include <nclgl\Vector3.h>
#include <nclgl\Vector4.h>
#include <vector>


class Scene;

enum PickupType {
	PAINTBOMB_STUN_SPEED_PICKUP,
	LAUNCH_PAD,
	STUN_WEAPON,
	PAINT_WEAPON,
	WASHING_ZONE
};



class PickUpLoader
{
	struct PickUpObject
	{
		std::string name;
		PickupType		type;
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
	PickUpLoader();
	~PickUpLoader();

	void CreatePickUps(const std::string& filename, Scene* scene);

protected:
	void AddPickUpObject(const std::string& line);
	bool Load(const std::string& filename);

	std::vector<PickUpObject> pickupObjects;

protected:
	PickupType StringToType(const std::string& line);
	void HandlePickupInput(const std::string& line);
};

