#pragma once
#include "WeaponPickup.h"

class PaintWeaponPickup :
	public WeaponPickup
{
public:
	PaintWeaponPickup(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& halfdims,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~PaintWeaponPickup();

	virtual void effect(Player* player);
};

