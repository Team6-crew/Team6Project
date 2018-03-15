#include"Launchpad.h"
#include <ncltech\CommonMeshes.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <nclgl\ResourceManager.h>

Launchpad::Launchpad(const std::string& name,
	const nclgl::Maths::Vector3& pos,
	const nclgl::Maths::Vector3& scale,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const nclgl::Maths::Vector4& color)
	 {
	RenderNodeBase * rnode = RenderNodeFactory::Instance()->MakeRenderNode();
	
	RenderNodeBase * dummy = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Sphere(), color);
	dummy->SetTransform(nclgl::Maths::Matrix4::Scale(scale));
	dummy->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"launchpad.jpg"), 0);
	rnode->AddChild(dummy);
	rnode->SetTransform(nclgl::Maths::Matrix4::Translation(pos));
	rnode->SetBoundingRadius(0.5f);
	PhysicsNode * pnode = NULL;
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
			CollisionShape * pColshape = new CuboidCollisionShape(scale);
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
	player->physicsNode->SetLinearVelocity(nclgl::Maths::Vector3(0, 20.f, 0));
}

void Launchpad::SoftLaunch(PlayerSoftBody* player)
{
	for (int i = 0; i < 182; ++i)
		player->getBall()->softball[i]->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(player->getBall()->softball[i]->Physics()->GetLinearVelocity().x,
			20.f, player->getBall()->softball[i]->Physics()->GetLinearVelocity().z));
}
void Launchpad::AILaunch(BallAI* AIBall)
{
		AIBall->Physics()->SetLinearVelocity(AIBall->getBall()->Physics()->GetLinearVelocity()
			+ nclgl::Maths::Vector3(0, 20.f, 0));
}