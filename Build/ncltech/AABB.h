#pragma once
#include <nclgl\Vector3.h>
#include <ncltech\CollisionShape.h>
#include <ncltech\PhysicsNode.h>
#include <nclgl\NCLDebug.h>

struct AABB {
	nclgl::Maths::Vector3 center;
	float				  halfdim;
	nclgl::Maths::Vector3 corner1;
	nclgl::Maths::Vector3 corner2;

	AABB() {
		center = nclgl::Maths::Vector3(0.0f, 0.0f, 0.0f);
		halfdim = 0.5f;
		corner1 = center + nclgl::Maths::Vector3(halfdim, halfdim, -halfdim);
		corner2 = center - nclgl::Maths::Vector3(halfdim, halfdim, -halfdim);
	}

	AABB(nclgl::Maths::Vector3 cnt, float hlf) {
		center = cnt;
		halfdim = hlf;
		corner1 = center + nclgl::Maths::Vector3(halfdim, halfdim, -halfdim);
		corner2 = center - nclgl::Maths::Vector3(halfdim, halfdim, -halfdim);
	}
	
    //Based on https://stackoverflow.com/questions/4578967/cube-sphere-intersection-test

	inline float squared(float v) { return v * v; }

	bool containsObject(PhysicsNode* obj) {
		nclgl::Maths::Vector3 obj_pos = obj->GetPosition();
		float d_squared = obj->GetColRadius() * obj->GetColRadius();

		if (obj_pos.x > corner1.x) d_squared -= squared(obj_pos.x - corner1.x);
		else if (obj_pos.x < corner2.x) d_squared -= squared(obj_pos.x - corner2.x);

		if (obj_pos.y > corner1.y) d_squared -= squared(obj_pos.y - corner1.y);
		else if (obj_pos.y < corner2.y) d_squared -= squared(obj_pos.y - corner2.y);

		if (obj_pos.z < corner1.z) d_squared -= squared(obj_pos.z - corner1.z);
		else if (obj_pos.z > corner2.z) d_squared -= squared(obj_pos.z - corner2.z);

		return d_squared > 0;
	}

	bool containsObject(nclgl::Maths::Vector3 obj_pos, float radius) {
		float d_squared = radius * radius;

		if (obj_pos.x > corner1.x) d_squared -= squared(obj_pos.x - corner1.x);
		else if (obj_pos.x < corner2.x) d_squared -= squared(obj_pos.x - corner2.x);

		if (obj_pos.y > corner1.y) d_squared -= squared(obj_pos.y - corner1.y);
		else if (obj_pos.y < corner2.y) d_squared -= squared(obj_pos.y - corner2.y);

		if (obj_pos.z < corner1.z) d_squared -= squared(obj_pos.z - corner1.z);
		else if (obj_pos.z > corner2.z) d_squared -= squared(obj_pos.z - corner2.z);

		return d_squared > 0;
	}

	void draw() {
		float d = 2 *halfdim;
		nclgl::Maths::Vector3 c1 = corner1;
		nclgl::Maths::Vector3 c2 = c1 + nclgl::Maths::Vector3(-d, 0, 0);
		nclgl::Maths::Vector3 c3 = c2 + nclgl::Maths::Vector3(0, 0, d);
		nclgl::Maths::Vector3 c4 = c3 + nclgl::Maths::Vector3(d, 0, 0);

		nclgl::Maths::Vector3 c5 =corner2;
		nclgl::Maths::Vector3 c6 = c5 + nclgl::Maths::Vector3(d, 0, 0);
		nclgl::Maths::Vector3 c7 = c6 + nclgl::Maths::Vector3(0, 0, -d);
		nclgl::Maths::Vector3 c8 = c7 + nclgl::Maths::Vector3(-d, 0, 0);

		NCLDebug::DrawThickLine(c1, c2, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c2, c3, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c3, c4, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c1, c4, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));

		NCLDebug::DrawThickLine(c5, c6, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c6, c7, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c7, c8, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c5, c8, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));

		NCLDebug::DrawThickLine(c1, c7, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c4, c6, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c2, c8, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		NCLDebug::DrawThickLine(c3, c5, 0.1f, nclgl::Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	}
};