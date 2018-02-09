#pragma once
#include "Pickup.h"


class SpeedPickup : public Pickup
{
public:
	SpeedPickup(const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const Vector4& color);
	~SpeedPickup();
	
	virtual void effect(Player* player);
	

};

