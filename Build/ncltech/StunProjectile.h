#pragma once
#include "Projectile.h"
class StunProjectile : public Projectile
{
public:
	StunProjectile(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~StunProjectile();

	bool collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);
};

