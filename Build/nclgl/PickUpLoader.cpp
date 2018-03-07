#include "PickUpLoader.h"
#include <fstream>
#include <sstream>
#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <nclgl\GameLogic.h>
#include <nclgl\ResourceManager.h>

#include <ncltech\SpeedPickup.h>
#include <nclgl\Launchpad.h>
#include <ncltech\Paintbomb.h>
#include <ncltech\StunWeaponPickup.h>
#include <ncltech\PaintWeaponPickup.h>
#include <ncltech\Washingzone.h>

using namespace std;
using namespace nclgl::Maths;

PickUpLoader::PickUpLoader()
{
}

PickUpLoader::~PickUpLoader()
{
}

bool PickUpLoader::Load(const std::string& filename)
{
	string info;
	ifstream pickupfile("PickUpFile2.txt"); // need to make generic so all files can be loaded
	if (pickupfile.is_open()) {
		cout << "File opened" << endl;

		while (getline(pickupfile, info)) {
			HandlePickupInput(info);
		}
		pickupfile.close();
		return true;
	}
	else {
		cout << "File failed to be read" << endl;
		return false;
	}
}

void PickUpLoader::AddPickUpObject(const std::string& line)
{
	stringstream ss(line);
	PickUpObject obj;

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

	/*obj.rotAxis = nclgl::Maths::Vector3(x, y, z);
	float rot;
	ss >> rot;
	obj.rotationDegrees = rot;*/

	pickupObjects.push_back(obj);
}

PickupType PickUpLoader::StringToType(const std::string& line)
{
	if (line == "PAINTBOMB_STUN_SPEED_PICKUP")
	{
		return PAINTBOMB_STUN_SPEED_PICKUP;
	}
	else if (line == "LAUNCH_PAD")
	{
		return LAUNCH_PAD;
	}
	else if (line == "STUN_WEAPON")
	{
		return STUN_WEAPON;
	}
	else if (line == "PAINT_WEAPON")
	{
		return PAINT_WEAPON;
	}
	else if (line == "WASHING_ZONE")
	{
		return WASHING_ZONE;
	}
	
	
}

void PickUpLoader::CreatePickUps(const std::string& filename, Scene* scene)
{
	Load(filename);

	float step = 6.7f;
	float frame = 0.0f;
	int paintable_counter = 0;
	for (auto& object : pickupObjects)
	{
		SpeedPickup* speed;
		StunWeaponPickup* stunweapon;
		PaintWeaponPickup* paintweapon;
		//PaintBomb* paintbomb;
		Launchpad* launchpad;
		Washingzone* washingzone;
		
		switch (object.type)
		{
		/*case PAINTBOMB_STUN_SPEED_PICKUP:
			pickup = CommonUtils::BuildGroundCuboidObject(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				false,
				object.colour);

			scene->AddGameObject(pickup);
			break;*/

		case LAUNCH_PAD:
			launchpad = new Launchpad(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				object.colour);

			launchpad->SetTag(Tags::TLaunch);
			launchpad->SetPhysics(launchpad->Physics());
			scene->AddGameObject(launchpad);
			/*++paintable_counter;*/
			break;	

		case STUN_WEAPON:
			stunweapon = new StunWeaponPickup(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				object.colour);

			stunweapon->SetPhysics(stunweapon->Physics());
			scene->AddGameObject(stunweapon);
			break;

		case PAINT_WEAPON:
			paintweapon = new PaintWeaponPickup(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				object.colour
			);

			paintweapon->SetPhysics(paintweapon->Physics());
			scene->AddGameObject(paintweapon);		
			break;

		case WASHING_ZONE:
			washingzone = new Washingzone(
				object.name,
				object.position,
				object.scale,
				true,
				object.inverseMass,
				true,
				object.colour
			);

			washingzone->SetPhysics(washingzone->Physics());
			scene->AddGameObject(washingzone);
			break;
		}
	}

	pickupObjects.clear();
}

void PickUpLoader::HandlePickupInput(const std::string& line)
{
	stringstream ss(line);
	std::string name;
	std::string temp;
	ss >> name;
	ss >> temp;
	PickupType type = StringToType(temp);
	switch (type)
	{	
	default:
		AddPickUpObject(line);
	}
}
