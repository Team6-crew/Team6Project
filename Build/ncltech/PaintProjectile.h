#pragma once
#include "Projectile.h"
class PaintProjectile : public Projectile
{
public:
	PaintProjectile(const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~PaintProjectile();
	bool collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode);
};

