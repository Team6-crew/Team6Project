#pragma once
#include "Pickup.h"


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
	
	virtual void eff_speed(Player* player);
	virtual void eff_paint(Player* player) ;
	virtual void eff_stun(Player* player) ;
};

