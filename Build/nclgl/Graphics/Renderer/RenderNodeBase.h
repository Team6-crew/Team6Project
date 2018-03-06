#pragma once

#include <vector>
#include "OBJMeshBase.h"
namespace nclgl
{
	namespace Maths
	{
		class Vector3;
		class Vector4;
		class Matrix4;
	}
}

class MeshBase;

class RenderNodeBase
{
public:
	RenderNodeBase();
	virtual ~RenderNodeBase();

	virtual void	Update(float msec);
	virtual void	Draw() = 0;

	// Transforms
	virtual void							SetTransform(const nclgl::Maths::Matrix4 &matrix) = 0;
	virtual const nclgl::Maths::Matrix4		GetTransform() const = 0;
	virtual const nclgl::Maths::Matrix4		GetWorldTransform() const = 0;
	virtual void							SetWorldTransform(const nclgl::Maths::Matrix4 &matrix) = 0;

	virtual void							SetModelScale(const nclgl::Maths::Vector3& scale) = 0;
	virtual const nclgl::Maths::Vector3		GetModelScale() const = 0;

	virtual nclgl::Maths::Vector4			GetColour() = 0;
	virtual void							SetColour(const nclgl::Maths::Vector4 &c) = 0;

	// Mesh
	virtual MeshBase*						GetMesh() = 0;
	virtual void								SetMesh(MeshBase* newMesh) = 0;

	//obj Mesh
	virtual OBJMeshBase*						GetOBJMesh() = 0;
	virtual void								SetOBJMesh(OBJMeshBase* newMesh) = 0;



	virtual bool								IsRenderable() = 0;

	// Children
	void	AddChild(RenderNodeBase* s);
	bool	RemoveChild(RenderNodeBase* s, bool recursive = true);

	std::vector<RenderNodeBase*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	std::vector<RenderNodeBase*>::const_iterator GetChildIteratorEnd() { return children.end(); }

	// Culling
	static bool		CompareByCameraDistance(RenderNodeBase*a, RenderNodeBase*b);
	static bool		CompareByZ(RenderNodeBase*a, RenderNodeBase*b);

	float			GetBoundingRadius() const { return boundingRadius; }
	void			SetBoundingRadius(float f) { boundingRadius = f; }

	float			GetCameraDistance() const { return distanceFromCamera; }
	void			SetCameraDistance(float f) { distanceFromCamera = f; }
protected:
	std::vector<RenderNodeBase*> children;
	RenderNodeBase*	parent = nullptr;


	float		boundingRadius = 100.0f;
	float		distanceFromCamera = 0.0f;
};

