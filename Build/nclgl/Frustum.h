#pragma once

#include "Plane.h"

namespace nclgl
{
	namespace Maths
	{
		class Matrix4;
	}
}

class RenderNodeBase;

class Frustum {
public:
	Frustum(void) {};
	~Frustum(void) {};
	
	void FromMatrix(const nclgl::Maths::Matrix4 & mvp);
	bool InsideFrustum(RenderNodeBase &n);

	nclgl::Maths::Plane& GetPlane(int i) {
		return planes[i];
	}

protected:
	nclgl::Maths::Plane planes[6];
};

