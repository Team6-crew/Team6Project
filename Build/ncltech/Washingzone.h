#pragma once
#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>
#include <ncltech\PlayerSoftBody.h>

class Player;
class PlayerSoftBody;
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

	void Effect(Player* player);
	void SoftEffect(PlayerSoftBody* player);
	
private:

};