#include "Player.h"
#include "CommonUtils.h"
#include "SphereCollisionShape.h"
#include "CuboidCollisionShape.h"
#include "CommonMeshes.h"
#include "ScreenPicker.h"
#include <nclgl\OBJMesh.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <functional>
#include <ncltech\StunProjectile.h>
#include <ncltech\PaintProjectile.h>
#include <ncltech\SceneManager.h>
#include <nclgl\Launchpad.h>

#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>

using namespace nclgl::Maths;

DEFINE_HEAP(Player, "GameObjects");

Player::Player(const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{   
	speed = 20.0f;
	canpaint = true;
	time = 0;
	time = 0.0f;
	stunDuration = 0.0f;

	stunEffect = true;
	sensitivity = 0.0f;
	equippedStunWeapon = NULL;
	equippedPaintWeapon = NULL;
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
		pnode->SetElasticity(0.2f);

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

	RegisterPhysicsToRenderTransformCallback();

	tag = Tags::TPlayer;

	physicsNode->SetOnCollisionCallback(
		std::bind(
			&Player::collisionCallback,		// Function to call
			this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
			std::placeholders::_1,
			std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
	);

	setDynamic(true);

	body = CommonUtils::BuildCuboidObject("body",
		Vector3(0.0f, 2.0f, 0.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		Vector3(0.5, 0.5, 0.5),				//Half dimensions
		false,									//Has Physics Object
		0.0f,									//Mass
		false,									//Has Collision Shape
		false,									//Dragable by the user
		CommonUtils::GenColor(0.45f, 0.5f));

	bodyRenderNode = (*body->Render()->GetChildIteratorStart());
	//camera = GraphicsPipeline::Instance()->GetCamera1();
	camera = new Camera();
	camera->SetYaw(0.f);
	camera->SetPitch(-20.0f);

	camera_transform = RenderNodeFactory::Instance()->MakeRenderNode();
	camera_transform->SetTransform(Matrix4::Translation(Vector3(0, 10, 25)));

	(*body->Render()->GetChildIteratorStart())->AddChild(camera_transform);
	(*body->Render()->GetChildIteratorStart())->SetMesh(NULL);

	tempPitch = camera->GetPitch();
	tempYaw = camera->GetYaw();
}


Player::~Player()
{

}

void Player::setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump, KeyboardKeys shoot) {
	move_up = up;
	move_down = down;
	move_left = left;
	move_right = right;
	move_jump = jump;
	move_shoot = shoot;
}

void Player::move(float dt) {
	if (!stun(dt)) {
		Vector3 ball_pos = physicsNode->GetPosition();
		forward = (camera->GetPosition() - ball_pos).Normalise();
		forward = Vector3(forward.x, 0.0f, forward.z);


		Matrix4 worldTr = bodyRenderNode->GetWorldTransform();
		worldTr.SetPositionVector(ball_pos + Vector3(0, 2, 0));
		bodyRenderNode->SetTransform(worldTr);

		physicsNode->SetForce(Vector3(0, 0, 0));
		handleInput(dt);

		bodyRenderNode->SetTransform(bodyRenderNode->GetTransform()*Matrix4::Rotation(sensitivity, Vector3(0, 1, 0)));
		camera->SetPosition(camera_transform->GetWorldTransform().GetPositionVector());
	}


}

void Player::handleInput(float dt) {
	Vector3 jump(0, 15, 0);
	float yaw = camera->GetYaw();
	float pitch = camera->GetPitch();
	Vector3 up = Vector3(0, 1, 0);
	Vector3 right = Vector3::Cross(forward, up);

	if (justJumped) justJumped = !justJumped;

	if (Window::GetKeyboard()->KeyDown(move_up))
	{    
		if (canjump) {
			physicsNode->SetForce(Vector3(-forward.x, -1.5f, -forward.z) * speed);
		}
		else {
			physicsNode->SetForce(Vector3(-forward.x, 0.0f, -forward.z) * speed);
		}
		
	}

	if (Window::GetKeyboard()->KeyDown(move_down))
	{
		if (canjump) {
			physicsNode->SetForce(Vector3(forward.x, -1.5f, forward.z) * speed);
		}
		else {
			physicsNode->SetForce(Vector3(forward.x, 0.0f, forward.z) * speed);
		}
	}
	if (Window::GetKeyboard()->KeyDown(move_left))
	{
		physicsNode->SetForce(physicsNode->GetForce() + right * physicsNode->GetForce().Length() * 0.6f);
		increaseSensitivity(dt);
		camera->SetYaw(yaw + sensitivity);
	}
	else if (Window::GetKeyboard()->KeyDown(move_right))
	{
		physicsNode->SetForce(physicsNode->GetForce() - right * physicsNode->GetForce().Length() * 0.6f);
		decreaseSensitivity(dt);
		camera->SetYaw(yaw + sensitivity);
	}
	else {
		resetCamera(dt);
		camera->SetYaw(yaw + sensitivity);
	}
	if ((Window::GetKeyboard()->KeyTriggered(move_jump)))
	{
		if (canjump == true) {
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"jump2.wav", false);
			physicsNode->SetLinearVelocity(jump + physicsNode->GetLinearVelocity());
			justJumped = true;
			canjump = false;
		}
	}

	if ((Window::GetKeyboard()->KeyTriggered(move_shoot)))
	{
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"shoot.wav", false);
		shoot();
	}
}

void Player::equipStunWeapon(Vector4 colour) {
	if (equippedPaintWeapon) {
		(*body->Render()->GetChildIteratorStart())->RemoveChild(equippedPaintWeapon);
		delete equippedPaintWeapon;
		equippedPaintWeapon = NULL;
	}
	equippedStunWeapon = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Cube(), colour);
	equippedStunWeapon->SetTransform(Matrix4::Scale(Vector3(0.3f,0.3f,1.5f))*Matrix4::Translation(Vector3(5.0f, -8.0f, 3.0f)));

	(*body->Render()->GetChildIteratorStart())->AddChild(equippedStunWeapon);
}

void Player::equipPaintWeapon(Vector4 colour) {
	if (equippedStunWeapon) {
		(*body->Render()->GetChildIteratorStart())->RemoveChild(equippedStunWeapon);
		delete equippedStunWeapon;
		equippedStunWeapon = NULL;
	}
	equippedPaintWeapon = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Cube(), colour);
	equippedPaintWeapon->SetTransform(Matrix4::Scale(Vector3(0.3f, 0.3f, 1.5f))*Matrix4::Translation(Vector3(5.0f, -8.0f, 3.0f)));

	(*body->Render()->GetChildIteratorStart())->AddChild(equippedPaintWeapon);
}

bool Player::stun(float dt) {
	if (stunDuration > 0.0f) {
		stunEffect = true;
		time += dt;
		stunDuration -= dt;
		// CC
		physicsNode->SetLinearVelocity(Vector3(0, 0, 0));
		if (stunDuration > 2.0f)
		{	// Shake
			camera->SetYaw(camera->GetYaw() + 360.0f * 0.0005f * ((rand() % 200 - 100.0f) / 100.0f));
			camera->SetPitch(camera->GetPitch() + 360.0f * 0.0005f * ((rand() % 200 - 100.0f) / 100.0f));
			return true;
		}
		else {
			camera->SetYaw(tempYaw);
			camera->SetPitch(tempPitch);
		}
		return true;
	}
	else {
		return false;
	}
}

void Player::resetCamera(float dt) {
	if (sensitivity > 0) {
		sensitivity -= dt * 7;
		if (sensitivity < 0.0f) sensitivity = 0.0f;
	}
	else if (sensitivity < 0) {
		sensitivity += dt * 7;
		if (sensitivity > 0.0f) sensitivity = 0.0f;
	}
}



bool Player::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode) {
	if (otherNode->GetParent()->HasTag(Tags::TPickup)) {
		Pickup* pickup = (Pickup*)otherNode->GetParent();
		pickup->effect(this);
		PhysicsEngine::Instance()->DeleteAfter(pickup,0.0f);
		return false;
	}
	else if (otherNode->GetParent()->HasTag(Tags::TLaunch))
	{
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"duang.wav", false);
		Launchpad* launchpad = (Launchpad*)otherNode->GetParent();
		launchpad->Launch(this);
		canjump = false;
		return false;
	}
	else if (otherNode->GetParent()->HasTag(Tags::TPaintable)) {
		RenderNodeBase* otherRenderNode = (*otherNode->GetParent()->Render()->GetChildIteratorStart());
		Vector4 col1 = otherRenderNode->GetColourFromPlayer();
		Vector4 col2 = (*thisNode->GetParent()->Render()->GetChildIteratorStart())->GetColour();
		if (col1.x != col2.x || col1.y!=col2.y || col1.z != col2.z) {
			otherRenderNode->SetColourFromPlayer((*thisNode->GetParent()->Render()->GetChildIteratorStart())->GetColour());
			otherRenderNode->SetBeingPainted(true);
			otherRenderNode->SetPaintPercentage(0.0f);
		}
	}
	if (otherNode->GetParent()->HasTag(Tags::TWash)) {
		Washingzone* wash = (Washingzone*)otherNode->GetParent();
		wash->effect(this);
		return false;
	}
	else if (otherNode->GetParent()->HasTag(Tags::TGround))
	{    if(!justJumped) canjump = true;
	}
	return true;
};

void Player::shoot() {
	if (equippedStunWeapon) {
		Vector3 up = Vector3(0, 1, 0);
		Vector3 right = Vector3::Cross(forward, up);
		Vector3 pos = physicsNode->GetPosition() + Vector3(0, 3, 0) - right*1.5f - forward*2.0f;
		StunProjectile* projectile = new StunProjectile("p",pos,0.3f,true,0.5f,true, (*renderNode->GetChildIteratorStart())->GetColour());
		projectile->Physics()->SetLinearVelocity(-forward*40.0f);
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile);
		PhysicsEngine::Instance()->DeleteAfter(projectile, 3.0f);
	}
	else if (equippedPaintWeapon) {
		Vector3 up = Vector3(0, 1, 0);
		Vector3 right = Vector3::Cross(forward, up);
		Vector3 pos = physicsNode->GetPosition() + Vector3(0, 3, 0) - right * 1.5f - forward * 2.0f;
		PaintProjectile* projectile = new PaintProjectile("p", pos, 0.3f, true, 0.5f, true, (*renderNode->GetChildIteratorStart())->GetColour());
		projectile->Physics()->SetLinearVelocity(-forward * 40.0f);
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile);
		PhysicsEngine::Instance()->DeleteAfter(projectile, 3.0f);
	}
}
