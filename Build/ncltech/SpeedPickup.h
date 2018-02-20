#pragma once
#include "Pickup.h"

class Pickup;
class SpeedPickup;

static SpeedPickup * speed1;

class SpeedPickup : public Pickup
{
public:
	SpeedPickup(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~SpeedPickup();
	static SpeedPickup* getPickup(int a) { if (a == 1) return speed1; };
	
	virtual void effect(Player* player);

private:

};

