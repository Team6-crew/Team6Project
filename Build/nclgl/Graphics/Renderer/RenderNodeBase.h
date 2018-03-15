#pragma once
#include <ncltech\Tags.h>
#include <vector>
#include <nclgl\Vector4.h>

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
	inline bool     HasTag(Tags t) { return t == tag; }
	void SetTag(Tags t) { tag = t; }
	// Transforms
	virtual void							SetTransform(const nclgl::Maths::Matrix4 &matrix) = 0;
	virtual const nclgl::Maths::Matrix4&	GetTransform() const = 0;
	virtual const nclgl::Maths::Matrix4&	GetWorldTransform() const = 0;
	virtual void							SetWorldTransform(const nclgl::Maths::Matrix4 &matrix) = 0;

	virtual void							SetModelScale(const nclgl::Maths::Vector3& scale) = 0;
	virtual const nclgl::Maths::Vector3&	GetModelScale() const = 0;

	virtual nclgl::Maths::Vector4&			GetColour() = 0;
	virtual void							SetColour(const nclgl::Maths::Vector4 &c) = 0;
	// Mesh
	inline MeshBase*						GetMesh() { return mesh; }
	inline void								SetMesh(MeshBase* newMesh) { mesh = newMesh; } 

	inline bool								IsRenderable() { return (this->mesh != nullptr); }

	// Children
	void	AddChild(RenderNodeBase* s);
	bool	RemoveChild(RenderNodeBase* s, bool recursive = true);
	void	RemoveChildren();
	std::vector<RenderNodeBase*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	std::vector<RenderNodeBase*>::const_iterator GetChildIteratorEnd() { return children.end(); }

	// Culling
	static bool		CompareByCameraDistance(RenderNodeBase*a, RenderNodeBase*b);
	static bool		CompareByZ(RenderNodeBase*a, RenderNodeBase*b);

	float			GetBoundingRadius() const { return boundingRadius; }
	void			SetBoundingRadius(float f) { boundingRadius = f; }

	float			GetCameraDistance() const { return distanceFromCamera; }
	void			SetCameraDistance(float f) { distanceFromCamera = f; }

	float			GetPaintPercentage() const { return paintPercentage; }
	void			SetPaintPercentage(float p) { paintPercentage = p; }

	nclgl::Maths::Vector4	GetColourFromPlayer() const { return colourFromPlayer; }
	void			SetColourFromPlayer(nclgl::Maths::Vector4 c) { colourFromPlayer = c; }

	nclgl::Maths::Vector3	GetHalfDims() const { return halfDims; }
	void			SetHalfDims(nclgl::Maths::Vector3 c) { halfDims = c; }

	bool			GetBeingPainted() const { return beingPainted; }
	void			SetBeingPainted(bool p) { beingPainted = p; }

	float			GetCost() { return cost; }
	void			SetCost(float c) { cost = c; }
protected:
	std::vector<RenderNodeBase*> children;
	RenderNodeBase*	parent = nullptr;
	Tags tag;
	MeshBase*	mesh;
	float paintPercentage;
	bool beingPainted;
	nclgl::Maths::Vector4 colourFromPlayer;
	nclgl::Maths::Vector3 halfDims;
	float		boundingRadius = 100.0f;
	float		distanceFromCamera = 0.0f;
	float		cost;
};

