#include "Portal.h"
#include <ncltech\CommonMeshes.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
Portal::Portal(const std::string& name,
	const nclgl::Maths::Vector3& pos,
	const nclgl::Maths::Vector3& halfdims,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const nclgl::Maths::Vector4& color)
{
	RenderNodeBase* rnode = RenderNodeFactory::Instance()->MakeRenderNode();

	RenderNodeBase* dummy = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Cube(), color);
	dummy->SetTransform(nclgl::Maths::Matrix4::Scale(nclgl::Maths::Vector3(halfdims)));
	rnode->AddChild(dummy);

	rnode->SetTransform(nclgl::Maths::Matrix4::Translation(pos));
	rnode->SetBoundingRadius(0.5f);

	PhysicsNode* pnode = NULL;
	if (physics_enabled)
	{
		pnode = new PhysicsNode();
		pnode->SetPosition(pos);
		pnode->SetInverseMass(inverse_mass);
		pnode->SetColRadius(0.5f);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pnode->SetInverseInertia(CuboidCollisionShape(nclgl::Maths::Vector3(0.1f, 1.0f, 1.0f)).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new CuboidCollisionShape(nclgl::Maths::Vector3(0.1f, 1.0f, 1.0f));
			pnode->SetCollisionShape(pColshape);
			pnode->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	friendlyName = name;
	renderNode = rnode;
	physicsNode = pnode;

	setDynamic(false);

	//tag = Tags::TLaunch;

	RegisterPhysicsToRenderTransformCallback();
}

Portal::~Portal()
{
}
