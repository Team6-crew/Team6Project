#include "OGLRenderNode.h"

#include <nclgl\Graphics\MeshBase.h>

using std::vector;
using namespace nclgl::Maths;

DEFINE_HEAP(OGLRenderNode, "Graphics");

OGLRenderNode::OGLRenderNode(MeshBase* mesh, Vector4 colour) :
	RenderNodeBase()
{
	this->mesh			= mesh;
	this->colour		= colour;
	modelScale			= Vector3(1,1,1);
}

OGLRenderNode::~OGLRenderNode(void)	{
	for(unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void OGLRenderNode::Draw()
{
	if (this->mesh)
		this->mesh->Draw();
}

void	OGLRenderNode::Update(float msec)	 {
	RenderNodeBase::Update(msec);
}



void			OGLRenderNode::SetTransform(const Matrix4 &matrix)
{
	transform = matrix;
}

const Matrix4&	OGLRenderNode::GetTransform() const
{
	return transform;
}

const Matrix4&	OGLRenderNode::GetWorldTransform() const
{
	return worldTransform;
}

void			OGLRenderNode::SetWorldTransform(const Matrix4 &matrix)
{
	worldTransform = matrix;
}

Vector4&		OGLRenderNode::GetColour()
{
	return colour;
}

void OGLRenderNode::SetModelScale(const Vector3 &s)
{
	modelScale = s;
}

const Vector3& OGLRenderNode::GetModelScale() const
{
	return modelScale;
}

void			OGLRenderNode::SetColour(const Vector4 &c)
{
	colour = c;
}