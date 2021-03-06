/******************************************************************************
Class: 
Namespace: CommonUtils
Implements:
Author:
	Pieran Marris <p.marris@newcastle.ac.uk>
Description:
	Common Utilities/functions to help in quickly building up a scene with as little code as possible.
	Contains compounded factory functions to automatically construct GameObject's that fit the parameters
	provided.

	Everything here, probably shouldn't be bundled with the physics engine library... and in honesty
	should have been built better. Though for the sake of simplicity (and for the love of ridiculously long
	parameter lists) it's here, and here to stay. =]

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameObject.h"

namespace CommonUtils
{
	void DragableObjectCallback(GameObject* obj, float dt, const nclgl::Maths::Vector3& newWsPos, 
								const nclgl::Maths::Vector3& wsMovedAmount, bool stopDragging);

	void SelectableObjectCallBack(GameObject* obj, nclgl::Maths::Vector3* pos);

	//Generates a unique color based on scalar parameter in the range of 0-1.
	// - Works off HSV color format, in this case Saturation and Value are 100% and Hue is dependant on 'scalar'
	nclgl::Maths::Vector4 GenColor(float scalar, float alpha = 1.0f);

	//Similar as above, but full HSV rather than just hue
	// - All hsv values range from 0-1 (including hue!)
	nclgl::Maths::Vector4 GenHSVColor(const nclgl::Maths::Vector3& hsv, float alpha = 1.0f);

	//Generates a default Sphere object with the parameters specified.
	GameObject* BuildSphereObject(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	GameObject* BuildSoftSphereObject(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	//Generates a default Cuboid object with the parameters specified
	GameObject* BuildCuboidObject(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& halfdims,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a default Cuboid object with the parameters specified - GROUND SPECIFIC
	GameObject* BuildGroundCuboidObject(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& halfdims,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a default Cuboid object with the parameters specified
	GameObject* BuildPaintableCube(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& halfdims,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	GameObject* BuildCuboidObjectNoTexture(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& halfdims,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	
	//For soft objects
	GameObject* BuildNonRenderObject(
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	GameObject* BuildRampObject( // RAMP SPECIFIC
		const std::string& name,
		const nclgl::Maths::Vector3& pos,
		const nclgl::Maths::Vector3& halfdims,
		bool physics_enabled = true,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		const nclgl::Maths::Vector3& axis = nclgl::Maths::Vector3(0.0f, 1.0f, 0.0f),
		float degrees = 90.0f);
};