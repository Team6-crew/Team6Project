/******************************************************************************
Class: Object
Implements:
Author:
Pieran Marris      <p.marris@newcastle.ac.uk> and YOU!
Description:
This is the base class for all of the objects in your game/scenes. It
will automatically be managed by any Scene instance which it is added to,
firing OnRenderObject() and OnUpdateObject(float dt) each frame.

It can also optionally be attached to a PhysicsNode component which will
automatically update the object's world transform based off it's physical
position/orientation each frame.

*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <nclgl\Matrix4.h>
#include "GraphicsPipeline.h"
#include "PhysicsEngine.h"
#include "PhysicsNode.h"
#include <vector>
#include <functional>
#include "Tags.h"
#include <nclgl\Graphics\Renderer\RenderNodeBase.h>


class Scene;
class PhysicsEngine;

class GameObject
{
	friend class Scene;			//Can set the private variables scene 
	friend class PhysicsEngine;	//Can notionally set private variable m_pPhysicsObject to NULL if it was deleted elsewhere

public:
	GameObject(const std::string& name = "")
		: friendlyName(name)
		, renderNode(NULL)
		, physicsNode(NULL)
	    , tag(Default){
	}

	GameObject(const std::string& name, RenderNodeBase* renderNde, PhysicsNode* physicsNde = NULL)
		: friendlyName(name)
		, renderNode(renderNde)
		, physicsNode(physicsNde)
		, tag(Default)
	{  
		RegisterPhysicsToRenderTransformCallback();
	}

	virtual ~GameObject()
	{
		if (renderNode)  GraphicsPipeline::Instance()->RemoveRenderNode(renderNode);
		if (physicsNode) PhysicsEngine::Instance()->RemovePhysicsObject(physicsNode);

		SAFE_DELETE(renderNode);
		SAFE_DELETE(physicsNode);
	}


	//<------- Scene Interation ------>
	// A good place to handle callback setups etc if you extend GameObject class (See ObjectPlayer for an example)
	virtual void OnAttachedToScene()	{};
	virtual void OnDetachedFromScene()	{};

	//<------- Object Parameters ------>
	inline const std::string& GetName()		{ return friendlyName; }
	inline const Scene* GetScene() const	{ return scene; }
	inline		 Scene* GetScene()			{ return scene; }
	inline bool	 HasTag(Tags t)				{ return t == tag; }
	void SetTag(Tags t)						{ tag = t; }

	
	//<---------- PHYSICS ------------>
	inline bool  HasPhysics() const					{ return (physicsNode != NULL); }
	inline const PhysicsNode*	Physics() const		{ return physicsNode; }
	inline		 PhysicsNode*	Physics()			{ return physicsNode; }
	
	inline void  SetPhysics(PhysicsNode* node)
	{
		if (physicsNode)
		{
			UnregisterPhysicsToRenderTransformCallback(); //Unregister old callback listener
			physicsNode->SetParent(NULL);
		}

		physicsNode = node;

		if (physicsNode)
		{
			physicsNode->SetParent(this);
			RegisterPhysicsToRenderTransformCallback();   //Register new callback listener
		}
	}


	//<---------- GRAPHICS ------------>
	inline bool  HasRender() const						{ return (renderNode != NULL); }
	inline const RenderNodeBase*	Render() const		{ return renderNode; }
	inline		 RenderNodeBase*	Render()			{ return renderNode; }

	inline void  SetRender(RenderNodeBase* node)
	{
		if (renderNode != node)
		{
			if (scene && renderNode) GraphicsPipeline::Instance()->RemoveRenderNode(node);
			
			renderNode = node;
			RegisterPhysicsToRenderTransformCallback();

			if (scene) GraphicsPipeline::Instance()->AddRenderNode(node);
		}
	}


	//---------- SOUND (?) ------------>




	//<---------- UTILS ------------>
	inline void RegisterPhysicsToRenderTransformCallback()
	{
		if (physicsNode && renderNode)
		{
			physicsNode->SetOnUpdateCallback(
				std::bind(
					&RenderNodeBase::SetTransform,		// Function to call
					renderNode,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
					std::placeholders::_1)			// Variable parameter(s) that will be set by the callback function
			);
		}
	}

	inline void UnregisterPhysicsToRenderTransformCallback()
	{
		if (physicsNode)
		{
			physicsNode->SetOnUpdateCallback([](const nclgl::Maths::Matrix4&) {});
		}
	}

	void setDynamic(bool isDynamic) {
		physicsNode->setDynamic(isDynamic);
	}

public:
	PhysicsNode * physicsNode;

	
	
protected:
	//Scene  
	std::string					friendlyName;
	Tags						tag;
	Scene*						scene;

	//Components
	RenderNodeBase*					renderNode;

};