#pragma once

#include "Plane.h"

class Matrix4;
class RenderNodeBase;

class Frustum {
public:
	Frustum(void) {};
	~Frustum(void) {};
	
	void FromMatrix(const Matrix4 & mvp);
	bool InsideFrustum(RenderNodeBase &n);

	Plane& GetPlane(int i) {
		return planes[i];
	}

protected:
	Plane planes[6];
};

