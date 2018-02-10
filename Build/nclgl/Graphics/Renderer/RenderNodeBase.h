#pragma once

#include <vector>

class Vector3;
class Vector4;
class Matrix4;

class MeshBase;

class RenderNodeBase
{
public:
	RenderNodeBase();
	virtual ~RenderNodeBase();

	virtual void	Update(float msec);
	virtual void	Draw() = 0;

	// Transforms
	virtual void			SetTransform(const Matrix4 &matrix) = 0;
	virtual const Matrix4&	GetTransform() const = 0;
	virtual const Matrix4&	GetWorldTransform() const = 0;
	virtual void			SetWorldTransform(const Matrix4 &matrix) = 0;

	virtual void			SetModelScale(const Vector3& scale) = 0;
	virtual const Vector3&	GetModelScale() const = 0;

	virtual Vector4&		GetColour() = 0; 
	virtual void			SetColour(const Vector4 &c) = 0;

	// Mesh
	inline MeshBase*		GetMesh() { return mesh; }
	inline void				SetMesh(MeshBase* newMesh) { mesh = newMesh; } 

	inline bool				IsRenderable() { return (this->mesh != nullptr); }

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

	MeshBase*	mesh;

	float		boundingRadius = 100.0f;
	float		distanceFromCamera = 0.0f;
};

