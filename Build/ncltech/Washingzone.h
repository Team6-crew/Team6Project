#pragma once
#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>

class Player;
class Washingzone : public GameObject
{
public:
	Washingzone(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& scale,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);
	~Washingzone();

	void effect(Player* player);
	
private:

};