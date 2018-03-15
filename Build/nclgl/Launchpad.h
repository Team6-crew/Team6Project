#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>
#include <string>
#include <vector>
#include <nclgl\Vector4.h>
#include <ncltech\PlayerSoftBody.h>
#include <nclgl/AI/BallAI.h>

class Player;
class PlayerSoftBody;
class BallAI;
class Launchpad : public GameObject
{
public:
	Launchpad(
		const std::string& name, 
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& scale,
		bool physics_enabled,
		float inverse_mass,
		bool collidable,
		const nclgl::Maths::Vector4& color);

	~Launchpad();

	void Launch(Player* player);
	void SoftLaunch(PlayerSoftBody* player);
	void AILaunch(BallAI* AIBall);
	

private:

};