#include "SpeedPickup.h"
#include "CommonUtils.h"
#include "SphereCollisionShape.h"
#include "CommonMeshes.h"
#include "ScreenPicker.h"
#include <nclgl\OBJMesh.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <functional>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>

using namespace nclgl::Maths;

SpeedPickup::SpeedPickup(const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{
	RenderNodeBase* rnode = RenderNodeFactory::Instance()->MakeRenderNode();

	RenderNodeBase* dummy = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Sphere(), color);
	dummy->SetTransform(Matrix4::Scale(Vector3(radius, radius, radius)));
	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(radius);

	PhysicsNode* pnode = NULL;
	if (physics_enabled)
	{
		pnode = new PhysicsNode();
		pnode->SetPosition(pos);
		pnode->SetInverseMass(inverse_mass);
		pnode->SetColRadius(radius);

		if (!collidable)
		{			
			pnode->SetInverseInertia(SphereCollisionShape(radius).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new SphereCollisionShape(radius);
			pnode->SetCollisionShape(pColshape);
			pnode->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	friendlyName = name;
	renderNode = rnode;
	physicsNode = pnode;

	setDynamic(false);	

	RegisterPhysicsToRenderTransformCallback();
}


SpeedPickup::~SpeedPickup()
{
}

void SpeedPickup::Effect(Player* player)
{
}

void SpeedPickup::SoftEffect(PlayerSoftBody* player) {
	for (int i = 0; i < 182; ++i)
	{
		player->getBall()->softball[i]->Physics()->SetInverseInertia(nclgl::Maths::Matrix3(150, 0, 0, 150, 0, 0, 150, 0, 0));
	}
}	

void SpeedPickup::eff_speed(Player* player)
{
	player->setSpeed(50.0f);
	AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"speedup.wav", false);
}
void SpeedPickup::eff_paint(Player* player)
{
	player->setadd_rad(0.05f);
	AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"paintbomb.wav", false);
}

void SpeedPickup::eff_stun(Player* player)
{
	player->setStunDuration(3.0f);
	player->stun(0.2f);
	AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"stun.wav", false);
}

void SpeedPickup::soft_eff_speed(PlayerSoftBody* player)
{
	player->setSpeed(50.0f);
	AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"speedup.wav", false);
}
void SpeedPickup::soft_eff_paint(PlayerSoftBody* player)
{
	player->setadd_rad(0.05f);
	AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"paintbomb.wav", false);
}

void SpeedPickup::soft_eff_stun(PlayerSoftBody* player)
{
	player->setStunDuration(3.0f);
	player->stun(0.2f);
	AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"stun.wav", false);
}

	
