#pragma once
#include "Pickup.h"

#include <string>

class Paintbomb : public Pickup
{
public:
	Paintbomb(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~Paintbomb();

	virtual void effect(Player* player);


};