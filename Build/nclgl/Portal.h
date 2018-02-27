#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>

class Player;
class Portal : public GameObject
{
public:
	Portal(
		const std::string& name,
		const Vector3& pos,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const Vector4& color);

	~Portal();


private:

};