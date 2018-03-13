#include "RenderNodeBase.h"

#include "../../Matrix4.h"

using namespace nclgl::Maths;

RenderNodeBase::RenderNodeBase()
{
}


RenderNodeBase::~RenderNodeBase()
{
}

bool RenderNodeBase::RemoveChild(RenderNodeBase* s, bool recursive) {

	for (auto i = children.begin(); i != children.end(); ++i) {
		if ((*i) == s) {
			i = children.erase(i);
			return true;
		}
	}

	if (recursive) {
		for (auto i = children.begin(); i != children.end(); ++i) {
			if ((*i)->RemoveChild(s, recursive)) {
				return true;
			}
		}
	}
	return false;
}

void RenderNodeBase::AddChild(RenderNodeBase* s) {
	children.push_back(s);
	s->parent = this;
}


void	RenderNodeBase::Update(float msec)
{
	if (parent)
	{
		SetWorldTransform(parent->GetWorldTransform() * GetTransform());
	}
	else {
		SetWorldTransform(GetTransform());
	}


	for (std::vector<RenderNodeBase*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}


bool	RenderNodeBase::CompareByCameraDistance(RenderNodeBase*a, RenderNodeBase*b) {
	return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
}

bool	RenderNodeBase::CompareByZ(RenderNodeBase*a, RenderNodeBase*b) {
	return (a->GetWorldTransform().GetPositionVector().z < b->GetWorldTransform().GetPositionVector().z) ? true : false;
}

void	RenderNodeBase::RemoveChildren()
{
	for (auto i = children.begin(); i != children.end(); ++i) {
		if (*i)
		{ 
			(*i)->RemoveChildren();
			SAFE_DELETE(*i);
		}
	}

}