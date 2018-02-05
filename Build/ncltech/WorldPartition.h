#pragma once
#include <ncltech\AABB.h>
#include <ncltech\CollisionShape.h>
#include <ncltech\PhysicsNode.h>
#include <nclgl\NCLDebug.h>
#include <vector>

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

	bool insert(PhysicsNode* p);

	std::vector<Partition> leaves;
	
	void debugDraw();

	static void enable() { isActive = true; }

	static void disable() { isActive = false; }

	static void toggle() { isActive = !isActive; }

	static bool isEnabled() { return isActive; }

	//returns a pointer to a vector of pointers to physics objects that the given physics object could possibly be colliding with.
	static std::vector<PhysicsNode*>* getPossibleCollisions(PhysicsNode* node);

private:

	static bool isActive;

	void subdivide(vector<Partition>& leaves);

};

