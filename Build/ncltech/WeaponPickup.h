#pragma once
#include "Pickup.h"

class WeaponPickup: public Pickup
{
public:
	WeaponPickup(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& halfdim,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);

	~WeaponPickup();

	virtual void effect(Player* player);
};

