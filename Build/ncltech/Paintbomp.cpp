#include "Paintbomb.h"
#include "CommonUtils.h"
#include "SphereCollisionShape.h"
#include "CuboidCollisionShape.h"
#include "CommonMeshes.h"
#include "ScreenPicker.h"
#include <nclgl\OBJMesh.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <functional>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>

using namespace nclgl::Maths;

Paintbomb::Paintbomb(const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{

	//Due to the way SceneNode/RenderNode's were setup, we have to make a dummy node which has the mesh and scaling transform
	// and a parent node that will contain the world transform/physics transform
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
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
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


Paintbomb::~Paintbomb()
{
}

void Paintbomb::effect(Player* player) {
	player->setadd_rad(0.05f);
	AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"paintbomb.wav", false);
}