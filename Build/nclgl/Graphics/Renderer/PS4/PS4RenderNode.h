#ifdef PSTATION4
#pragma once

#include "nclPS4Interface.h"

#include "PS4Mesh.h"

#include "../RenderNodeBase.h"
#include <nclgl\Vector4.h>


class MeshBase;

namespace nclgl
{
	namespace Maths
	{
		class Matrix4;
		class Vector3;
		class Vector4;
	}
};

class PS4RenderNode : public RenderNodeBase
{
public:
	PS4RenderNode(MeshBase* m = NULL, nclgl::Maths::Vector4 colour = nclgl::Maths::Vector4(1, 1, 1, 1));
	~PS4RenderNode();

	void	Update(float msec);
	void	Draw();

	// Transforms
	void							SetTransform(const nclgl::Maths::Matrix4 &matrix) override;
	const nclgl::Maths::Matrix4		GetTransform() const override;
	const nclgl::Maths::Matrix4		GetWorldTransform() const override;
	void							SetWorldTransform(const nclgl::Maths::Matrix4 &matrix) override;

	void							SetModelScale(const nclgl::Maths::Vector3& scale) override;
	const nclgl::Maths::Vector3		GetModelScale() const override;

	nclgl::Maths::Vector4			GetColour() override;
	void							SetColour(const nclgl::Maths::Vector4 &c) override;


	inline void SetGraphicsContext(sce::Gnmx::GnmxGfxContext* context) { mesh->SetGraphicsContext(context); }

protected:
	sce::Vectormath::Scalar::Aos::Matrix4			worldTransform;
	sce::Vectormath::Scalar::Aos::Matrix4			transform;

	sce::Vectormath::Scalar::Aos::Vector4			colour;
	sce::Vectormath::Scalar::Aos::Vector3			modelScale;

	// Temp - should be a MeshBase* inherited from RenderNoseBase
	PS4Mesh*	mesh;
};

#endif