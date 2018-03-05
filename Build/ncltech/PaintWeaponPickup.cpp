#include "PaintWeaponPickup.h"
#include "CommonUtils.h"
#include "CuboidCollisionShape.h"
#include "CommonMeshes.h"
#include "ScreenPicker.h"
#include <nclgl\OBJMesh.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <functional>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>

using namespace nclgl::Maths;


PaintWeaponPickup::PaintWeaponPickup(const std::string& name,
	const nclgl::Maths::Vector3& pos,
	const nclgl::Maths::Vector3& halfdims,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const nclgl::Maths::Vector4& color)
{
	RenderNodeBase* rnode = RenderNodeFactory::Instance()->MakeRenderNode();

	RenderNodeBase* dummy = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Cube(), color);
	dummy->SetTransform(Matrix4::Scale(halfdims));
	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(halfdims.Length());

	PhysicsNode* pnode = NULL;
	if (physics_enabled)
	{
		pnode = new PhysicsNode();
		pnode->SetPosition(pos);
		pnode->SetInverseMass(inverse_mass);
		float rad = sqrt(halfdims.x*halfdims.x + halfdims.y*halfdims.y + halfdims.z*halfdims.z);
		pnode->SetColRadius(rad*1.5f);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pnode->SetInverseInertia(CuboidCollisionShape(halfdims).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
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


PaintWeaponPickup::~PaintWeaponPickup()
{
}

void PaintWeaponPickup::effect(Player* player) {
	player->equipPaintWeapon((*renderNode->GetChildIteratorStart())->GetColour());
	AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"pickweapon.wav", false);
}
