#include "PaintProjectile.h"
#include "CommonUtils.h"
#include "SphereCollisionShape.h"
#include "CommonMeshes.h"
#include "ScreenPicker.h"
#include <nclgl\OBJMesh.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <functional>
#include "Player.h"

using namespace nclgl::Maths;


PaintProjectile::PaintProjectile(const std::string& name,
	const nclgl::Maths::Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const nclgl::Maths::Vector4& color)
{
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
			&PaintProjectile::collisionCallback,
			this,
			std::placeholders::_1,
			std::placeholders::_2)
	);

	setDynamic(false);


	RegisterPhysicsToRenderTransformCallback();
}


PaintProjectile::~PaintProjectile()
{
}

bool PaintProjectile::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode) {
	if (otherNode->GetParent()->HasTag(Tags::TGround)) {
		PhysicsEngine::Instance()->DeleteAfter(this, 0.0f);
	}
	else if (otherNode->GetParent()->HasTag(Tags::TPaintable)) {
		RenderNodeBase* otherRenderNode = (*otherNode->GetParent()->Render()->GetChildIteratorStart());
		Vector4 col1 = otherRenderNode->GetColourFromPlayer();
		Vector4 col2 = (*thisNode->GetParent()->Render()->GetChildIteratorStart())->GetColour();
		if (col1.x != col2.x || col1.y != col2.y || col1.z != col2.z) {
			//(*otherNode->GetParent()->Render()->GetChildIteratorStart())->SetColour((*renderNode->GetChildIteratorStart())->GetColour());
			otherRenderNode->SetColourFromPlayer((*thisNode->GetParent()->Render()->GetChildIteratorStart())->GetColour());
			otherRenderNode->SetBeingPainted(true);
			otherRenderNode->SetPaintPercentage(0.0f);
		}
		PhysicsEngine::Instance()->DeleteAfter(this, 0.0f);

	}
	return true;
};
