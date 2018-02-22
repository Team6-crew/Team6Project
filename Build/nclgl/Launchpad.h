#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>

class Player;
class Launchpad : public GameObject
{
public:
	Launchpad(
		const std::string& name, 
		const Vector3& pos, 
		const Vector3& scale,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const Vector4& color);

	~Launchpad();

	void Launch(Player* player);
	

private:

};