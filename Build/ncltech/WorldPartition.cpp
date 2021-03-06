#include "OcTree.h"
#include "WorldPartition.h"
#include <ncltech\GameObject.h>

using namespace std;
using namespace nclgl::Maths;

std::vector<PhysicsNode*> WorldPartition::getPossibleCollisions(PhysicsNode * node)
{
	vector<PhysicsNode*> possibleCollisions;

	for (uint i = 0; (uint)i < leaves.size(); ++i) {
		if (leaves.at(i).boundary.containsObject(node)) {//if object is inside the leaves aabb add it to list of objects in that partition
			possibleCollisions.insert(possibleCollisions.end(), leaves.at(i).objectsIn.begin(), leaves.at(i).objectsIn.end());
		}
	}

	std::set<PhysicsNode*> set(possibleCollisions.begin(), possibleCollisions.end());
	if (set.find(node) != set.end()) {
		set.erase(set.find(node));
	}
	possibleCollisions.clear();
	possibleCollisions.insert(possibleCollisions.end(),set.begin(), set.end());

	return possibleCollisions;
}


void WorldPartition::insert(vector<GameObject*> &nodes)
{	
	for (uint i = 0; (uint)i < nodes.size();++i) {
		if (nodes.at(i)->HasPhysics()) {
			this->insert(nodes.at(i)->physicsNode);
		}
	}
}

void WorldPartition::insert(PhysicsNode * p)
{
	for (uint i = 0; (uint)i < leaves.size(); ++i) {
		if (leaves.at(i).boundary.containsObject(p)) {//if object is inside the leaves aabb add it to list of objects in that partition
			leaves.at(i).objectsIn.push_back(p);
		}
	}
}


void WorldPartition::debugDraw()
{
	for (uint i = 0; i < leaves.size(); ++i) {
		if (leaves.at(i).objectsIn.size() > 0) {//draw partitions with an object inside them
			float d = 2 * leaves.at(i).boundary.halfdim;
			Vector3 c1 = leaves.at(i).boundary.corner1;
			Vector3 c2 = c1 + Vector3(-d, 0, 0);
			Vector3 c3 = c2 + Vector3(0, 0, d);
			Vector3 c4 = c3 + Vector3(d, 0, 0);

			Vector3 c5 = leaves.at(i).boundary.corner2;
			Vector3 c6 = c5 + Vector3(d, 0, 0);
			Vector3 c7 = c6 + Vector3(0, 0, -d);
			Vector3 c8 = c7 + Vector3(-d, 0, 0);

			NCLDebug::DrawThickLine(c1, c2, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c2, c3, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c3, c4, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c1, c4, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

			NCLDebug::DrawThickLine(c5, c6, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c6, c7, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c7, c8, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c5, c8, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

			NCLDebug::DrawThickLine(c1, c7, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c4, c6, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c2, c8, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			NCLDebug::DrawThickLine(c3, c5, 0.05f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}
}

void WorldPartition::subdivide(vector<Partition> &leaves) {
	vector<Partition> copy = leaves;
	leaves.clear();

	for (uint i = 0; i < (uint)copy.size(); ++i) {
		Vector3 cur_center = copy.at(i).boundary.center;
		float half_cur_dim = copy.at(i).boundary.halfdim / 2.0f;
		leaves.push_back(Partition(AABB(Vector3(cur_center.x - half_cur_dim, cur_center.y - half_cur_dim, cur_center.z - half_cur_dim), half_cur_dim))); //topLeft ( down)
		leaves.push_back(Partition(AABB(Vector3(cur_center.x + half_cur_dim, cur_center.y - half_cur_dim, cur_center.z - half_cur_dim), half_cur_dim))); //topRight (down)
		leaves.push_back(Partition(AABB(Vector3(cur_center.x - half_cur_dim, cur_center.y - half_cur_dim, cur_center.z + half_cur_dim), half_cur_dim))); //botLeft (down)
		leaves.push_back(Partition(AABB(Vector3(cur_center.x + half_cur_dim, cur_center.y - half_cur_dim, cur_center.z + half_cur_dim), half_cur_dim))); //botRight (down)
		leaves.push_back(Partition(AABB(Vector3(cur_center.x - half_cur_dim, cur_center.y + half_cur_dim, cur_center.z - half_cur_dim), half_cur_dim))); //topLeft (up)
		leaves.push_back(Partition(AABB(Vector3(cur_center.x + half_cur_dim, cur_center.y + half_cur_dim, cur_center.z - half_cur_dim), half_cur_dim))); //topRight (up)
		leaves.push_back(Partition(AABB(Vector3(cur_center.x - half_cur_dim, cur_center.y + half_cur_dim, cur_center.z + half_cur_dim), half_cur_dim))); //botLeft (up)
		leaves.push_back(Partition(AABB(Vector3(cur_center.x + half_cur_dim, cur_center.y + half_cur_dim, cur_center.z + half_cur_dim), half_cur_dim))); //botRight (up)
	}

}
