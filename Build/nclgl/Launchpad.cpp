#include"Launchpad.h"
#include <ncltech\CommonMeshes.h>

Launchpad::Launchpad(const std::string& name,
	const Vector3& pos,
	const Vector3& scale,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{
	RenderNode* rnode = new RenderNode();

	RenderNode* dummy = new RenderNode(CommonMeshes::Cube(), color);
	dummy->SetTransform(Matrix4::Scale(scale));
	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
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
			pnode->SetInverseInertia(CuboidCollisionShape(scale).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new CuboidCollisionShape(scale);
			pnode->SetCollisionShape(pColshape);
			pnode->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	friendlyName = name;
	renderNode = rnode;
	physicsNode = pnode;

	setDynamic(false);

	tag = Tags::TLaunch;

	RegisterPhysicsToRenderTransformCallback();
}


Launchpad::~Launchpad()
{
}

void Launchpad::Launch(Player* player)
{
	player->physicsNode->SetLinearVelocity(Vector3(0, 30.f, 0));
}