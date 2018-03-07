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
	
	virtual void Effect(Player* player);
	virtual void SoftEffect(PlayerSoftBody* player);
	

};

