#include "OcTree.h"
#include "WorldPartition.h"

bool WorldPartition::insert(PhysicsNode * p)
{
	return false;
}

void WorldPartition::debugDraw()
{
	for (uint i = 0; i < leaves.size(); ++i) {
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

std::vector<PhysicsNode*>* WorldPartition::getPossibleCollisions(PhysicsNode * node)
{
	return nullptr;
}
