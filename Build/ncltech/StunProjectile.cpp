#include "StunProjectile.h"
#include "CommonUtils.h"
#include "SphereCollisionShape.h"
#include "CommonMeshes.h"
#include "ScreenPicker.h"
#include <nclgl\OBJMesh.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <functional>
#include "Player.h"
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>

using namespace nclgl::Maths;


StunProjectile::StunProjectile(const std::string& name,
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

	RenderNodeBase* dummy = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::StaticSphere(), color);
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
	SetPhysics(physicsNode);
	tag = Tags::TProjectile;

	physicsNode->SetOnCollisionCallback(
		std::bind(
			&StunProjectile::collisionCallback,		
			this,					
			std::placeholders::_1,
			std::placeholders::_2)		
	);

	setDynamic(false);


	RegisterPhysicsToRenderTransformCallback();
}


StunProjectile::~StunProjectile()
{
}

bool StunProjectile::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode) {
	if (otherNode->GetParent()->HasTag(Tags::TPlayer)) {
		//AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"behit.wav", false);
		Player* player = (Player*)otherNode->GetParent();
		player->setStunDuration(3.0f);
		PhysicsEngine::Instance()->DeleteAfter(this, 0.0f);
	}
	return true;
};
