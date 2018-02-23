#pragma once
#include "WeaponPickup.h"
class StunWeaponPickup :
	public WeaponPickup
{
public:
	StunWeaponPickup(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& halfdim,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~StunWeaponPickup();

	virtual void effect(Player* player);
};

