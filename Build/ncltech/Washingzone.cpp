#include "Washingzone.h"
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
#include <nclgl\ResourceManager.h>

using namespace nclgl::Maths;

Washingzone::Washingzone(const std::string& name,
	const Vector3& pos,
	const Vector3& scale,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{

	//Due to the way SceneNode/RenderNode's were setup, we have to make a dummy node which has the mesh and scaling transform
	// and a parent node that will contain the world transform/physics transform
	RenderNodeBase* rnode = RenderNodeFactory::Instance()->MakeRenderNode();

	RenderNodeBase* dummy = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Cube(), color);
	dummy->SetTransform(Matrix4::Scale(Vector3(scale.x, scale.y, scale.z)));
	dummy->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture(TEXTUREDIR"washingzone.jpg"), 0);
	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(3.0f);

	PhysicsNode* pnode = NULL;
	if (physics_enabled)
	{
		pnode = new PhysicsNode();
		pnode->SetPosition(pos);
		pnode->SetInverseMass(inverse_mass);
		pnode->SetColRadius(3.0f);

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

	setDynamic(true);
	SetTag(Tags::TWash);

	RegisterPhysicsToRenderTransformCallback();
}


Washingzone::~Washingzone()
{
}

void Washingzone::Effect(Player* player) 
{
	player->setDebuffTime(210);
	player->setcanpaint(false);
	player->settime(0.0f);
}

void Washingzone::SoftEffect(PlayerSoftBody* player)
{
	if (player->getcanpaint())
	{
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"washingzone.wav", false);
	}
	player->setDebuffTime(210);
	player->setcanpaint(false);
	player->settime(0.0f);
}

void Washingzone::AIEffect(BallAI* AIBall)
{
	if (AIBall->getcanpaint())
	{
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"washingzone.wav", false);
	}
	AIBall->setDebuffTime(210);
	AIBall->setcanpaint(false);
	AIBall->settime(0.0f);
}

