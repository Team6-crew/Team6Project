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
	
	virtual void Eff_Speed(Player* player) ;
	virtual void Eff_Paint(Player* player) ;
	virtual void Eff_Stun(Player* player) ;
	virtual void SoftEff_Speed(PlayerSoftBody* player) ;
	virtual void SoftEff_Paint(PlayerSoftBody* player) ;
	virtual void SoftEff_Stun(PlayerSoftBody* player) ;

	bool updown = true;
	float y;
	virtual void setupdowm(bool ud) { updown = ud; }
	virtual bool getupdown() { return updown; }
	void Updown();
};

