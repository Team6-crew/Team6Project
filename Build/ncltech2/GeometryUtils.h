#pragma once
#include <nclgl\Vector3.h>
#include <nclgl\Plane.h>
#include <list>
#include <vector>

namespace GeometryUtils
{
	struct Edge
	{
		Edge() : _v0(0.0f, 0.0f, 0.0f) , _v1(0.0f, 0.0f, 0.0f) {}
		Edge(nclgl::Maths::Vector3 start, nclgl::Maths::Vector3 end) : _v0(start) , _v1(end) {}


		nclgl::Maths::Vector3 _v0;
		nclgl::Maths::Vector3 _v1;
	};



	// Gets the closest point x on the line (edge) to point (pos)
	nclgl::Maths::Vector3 GetClosestPoint(
		const nclgl::Maths::Vector3& pos,
		const Edge& edge);

	// Iterates through all edges in polygon (defined as a line-loop list
	// of vertices) and returns the closest point X to point (pos) that 
	// resides on any of the given edges of the polygon.
	nclgl::Maths::Vector3 GetClosestPointPolygon(
		const nclgl::Maths::Vector3& pos,
		const std::list<nclgl::Maths::Vector3>& polygon);

	// Iterates through all edges returning the the point X which is the closest
	//   point along any of the given edges to the provided point A as possible.
	nclgl::Maths::Vector3 GetClosestPoint(
		const nclgl::Maths::Vector3& pos,
		std::vector<Edge>& edges);


	// Performs a plane/edge collision test, if an intersection does occur then
	//    it will return the point on the line where it intersected the given plane.
	bool PlaneEdgeIntersection(
		const nclgl::Maths::Plane& plane,
		const nclgl::Maths::Vector3& start,
		const nclgl::Maths::Vector3& end,
		nclgl::Maths::Vector3& out_point);

	//Performs sutherland hodgman clipping algorithm to clip the provided polygon
	// in regards to each of the provided clipping planes.
	// https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
	void SutherlandHodgmanClipping(
		const std::list<nclgl::Maths::Vector3>& input_polygon,
		int num_clip_planes,
		const nclgl::Maths::Plane* clip_planes,
		std::list<nclgl::Maths::Vector3>* out_polygon,
		bool removeNotClipToPlane);
};