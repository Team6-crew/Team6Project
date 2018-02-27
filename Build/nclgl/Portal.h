#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>
#include "Graphics\Renderer\RenderNodeBase.h"

class Player;
class Portal : public GameObject
{
public:
	Portal(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);

	~Portal();


private:

};