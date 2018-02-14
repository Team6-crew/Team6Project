#include "Player.h"
#include "CommonUtils.h"
#include "SphereCollisionShape.h"
#include "CuboidCollisionShape.h"
#include "CommonMeshes.h"
#include "ScreenPicker.h"
#include <nclgl\OBJMesh.h>
#include <nclgl\RenderNode.h>
#include <functional>



Player::Player(const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{   
	speed = 20.0f;

	//Due to the way SceneNode/RenderNode's were setup, we have to make a dummy node which has the mesh and scaling transform
	// and a parent node that will contain the world transform/physics transform
	RenderNode* rnode = new RenderNode();

	RenderNode* dummy = new RenderNode(CommonMeshes::Sphere(), color);
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

	//camera = GraphicsPipeline::Instance()->GetCamera1();
	camera = new Camera();
	camera->SetYaw(0.f);

	camera_transform = new RenderNode();
	camera_transform->SetTransform(Matrix4::Translation(Vector3(0, 10, 25)));

	(*body->Render()->GetChildIteratorStart())->AddChild(camera_transform);
	(*body->Render()->GetChildIteratorStart())->SetMesh(NULL);
}


Player::~Player()
{

}

void Player::setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump) {
	move_up = up;
	move_down = down;
	move_left = left;
	move_right = right;
	move_jump = jump;
}

void Player::move(float dt) {

	
	Vector3 ball_pos = physicsNode->GetPosition();
	Vector3 forward = (camera->GetPosition() - ball_pos).Normalise();
	Vector3 jump(0, 20, 0);

	RenderNode* bodyRenderNode = (*body->Render()->GetChildIteratorStart());
	Matrix4 worldTr = bodyRenderNode->GetWorldTransform();
	worldTr.SetPositionVector(ball_pos + Vector3(0, 2, 0));
	
	bodyRenderNode->SetTransform(worldTr);

	float yaw = camera->GetYaw();
	float pitch = camera->GetPitch();

	physicsNode->SetForce(Vector3(0, 0, 0));

	float rotation = 0.0f;

	if (Window::GetKeyboard()->KeyDown(move_up))
	{
		physicsNode->SetForce(-forward * speed);
	}

	if (Window::GetKeyboard()->KeyDown(move_down))
	{   
		forward.y = -forward.y;
		physicsNode->SetForce(forward * speed);
	}
	if (Window::GetKeyboard()->KeyDown(move_left))
	{
		rotation = dt*100.0f;
		camera->SetYaw(yaw + rotation);
	}

	if (Window::GetKeyboard()->KeyDown(move_right))
	{
		rotation = -dt*100.0f;
		camera->SetYaw(yaw + rotation);
	}

	if ((Window::GetKeyboard()->KeyTriggered(move_jump)) )
	{  
		if (canjump == true) {
			physicsNode->SetLinearVelocity(jump + physicsNode->GetLinearVelocity());
			canjump = false;
		}
		
	}

	bodyRenderNode->SetTransform(Matrix4::Rotation(rotation, Vector3(0, 1, 0))*bodyRenderNode->GetTransform());

	camera->SetPosition(camera_transform->GetWorldTransform().GetPositionVector());

}

bool Player::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode) {
	if (otherNode->GetParent()->HasTag(Tags::TPickup)) {
		Pickup* pickup = (Pickup*)otherNode->GetParent();
		pickup->effect(this);
		PhysicsEngine::Instance()->DeleteNextFrame(pickup);
		return false;
	}
	else if (otherNode->GetParent()->HasTag(Tags::TGround))
	{ 
		canjump = true;
	}
	return true;
};
