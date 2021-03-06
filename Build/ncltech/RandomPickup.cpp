#include "RandomPickup.h"
#include "CommonUtils.h"
#include "SphereCollisionShape.h"
#include "CommonMeshes.h"
#include "ScreenPicker.h"
#include <nclgl\OBJMesh.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <functional>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include <nclgl\GameLogic.h>

using namespace nclgl::Maths;

RandomPickup::RandomPickup(const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{
	enabled = true;
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

	setDynamic(false);
	SetTag(Tags::TRandomPickup);

	RegisterPhysicsToRenderTransformCallback();
}


RandomPickup::~RandomPickup()
{
}

void RandomPickup::Effect(PlayerSoftBody* player) {
	int prob = (rand() % 100);
	float temp;
	float tempscore = player->getscore();

	if (tempscore == 1)
	{
		temp = 0;
	}
	else if (tempscore < 0.4f)
	{
		temp = tempscore * 4;
	}
	else if (tempscore > 0.7f)
	{
		temp = tempscore * 10;
	}
	else
	{
		temp = tempscore * 7;
	}
	cout << temp << endl;
	if (enabled) {
		if (prob < (20-temp))
		{
			for (int i = 0; i < 182; ++i) {
				player->getBall()->softball[i]->Physics()->SetInverseInertia(nclgl::Maths::Matrix3(150, 0, 0, 150, 0, 0, 150, 0, 0));
			}
			player->setBuffTime(10.0f);
			player->setCurrentBuff(Tags::BSpeed);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"speedup.wav", false);
		}
		else if (prob <(40-temp*2))
		{
			player->setadd_rad(0.05f);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"paintbomb.wav", false);
		}
		else if (prob <(60-temp*3))
		{   
			player->equipStunWeapon(Vector4(1,0,0,1));
			player->setBuffTime(10.0f);
			player->setCurrentBuff(Tags::BStun);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"pickweapon.wav", false);
		}
		else if(prob < (80-temp*4))
		{
			player->equipPaintWeapon(player->getColour());
			player->setBuffTime(10.0f);
			player->setCurrentBuff(Tags::BPaint);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"pickweapon.wav", false);
		}
		else
		{
			player->setStunDuration(3.0f);
			//player->stun(0.2f);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"behit.wav", false);
		}
		enabled = false;
	}
}

void RandomPickup::Effect(BallAI* AIBall) {
	int prob = (rand() % 100);
	float temp;
	float tempscore = AIBall->getscore();

	if (tempscore == 1)
	{
		temp = 0;
	}
	else if (tempscore < 0.4f)
	{
		temp = tempscore * 4;
	}
	else if (tempscore > 0.7f)
	{
		temp = tempscore * 10;
	}
	else
	{
		temp = tempscore * 7;
	}

	if (enabled) {
		if (prob < (20-temp))
		{
			AIBall->getBall()->Physics()->SetInverseMass(100.f);
			AIBall->setBuffTime(10.0f);
			AIBall->setCurrentBuff(Tags::BSpeed);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"speedup.wav", false);
		}
		else if (prob <(40- temp*2))
		{
			AIBall->setadd_rad(0.05f);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"paintbomb.wav", false);
		}
		else if (prob <(60- temp*3))
		{
			AIBall->equipStunWeapon(Vector4(1, 0, 0, 1));
			AIBall->setBuffTime(10.0f);
			AIBall->setCurrentBuff(Tags::BStun);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"pickweapon.wav", false);
		}
		else if (prob < (80- temp*4))
		{
			AIBall->equipPaintWeapon(AIBall->getColour());
			AIBall->setBuffTime(10.0f);
			AIBall->setCurrentBuff(Tags::BPaint);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"pickweapon.wav", false);
		}
		else
		{
			AIBall->setStunDuration(3.0f);
			//AIBall->stun(0.2f);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"behit.wav", false);
		}
		enabled = false;
	}
}

