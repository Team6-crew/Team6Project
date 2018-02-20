#pragma once
#include <ncltech\AABB.h>
#include <ncltech\CollisionShape.h>
#include <ncltech\PhysicsNode.h>
#include <nclgl\NCLDebug.h>
#include <vector>
#include <algorithm>  
#include <set>

class Scene;

struct Partition {
	AABB boundary;
	std::vector<PhysicsNode*> objectsIn;
	Partition(AABB aabb) {
		boundary = aabb;
	}
};

class WorldPartition {
public:
	WorldPartition(AABB* worldSize, uint divisions) { 
		Partition base(*worldSize);
		leaves.push_back(base);
		for (uint i = 0; i < divisions; ++i) {
			subdivide(leaves);
		}
	}

	~WorldPartition() {
	
	}

	//insert vector of game objects into world partition
	void insert(std::vector<GameObject*> &objects);

	//used for inserting static nodes into the world partition, should be performed on loading of level
	void insert(PhysicsNode* p);

	//causes massive lag when drawing with high number of partitions so try to avoid using
	void debugDraw();

	void enable() { isActive = true; }

	void disable() { isActive = false; }

	void toggle() { isActive = !isActive; }

	bool isEnabled() { return isActive; }

	//returns a vector of pointers to physics objects that the given physics object could possibly be colliding with.
	std::vector<PhysicsNode*> getPossibleCollisions(PhysicsNode* node);

	//clear all objects from the world partition
	void clear() {
		leaves.clear();
	}

private:

	bool isActive = true;

	void subdivide(std::vector<Partition>& leaves);

	std::vector<Partition> leaves;

	std::vector<PhysicsNode*> movingObjects;
};

