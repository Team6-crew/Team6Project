#pragma once
#include "Pickup.h"


class RandomPickup : public Pickup
{
public:
	RandomPickup(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~RandomPickup();
	
	virtual void Effect(PlayerSoftBody* player);
	virtual void Effect(BallAI* AIBall);
private:
	bool enabled;
};

