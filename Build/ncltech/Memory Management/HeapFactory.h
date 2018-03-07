#pragma once

#include <nclgl/TSingleton.h>

#include "HeapMemory.h"

class HeapFactory : public TSingleton<HeapFactory>
{
	friend class TSingleton<HeapFactory>;
	static const int MAX_HEAPS = 10;

public:
	//Heap* GetDefaultHeap();
	Heap * GetHeap(const char* name);
	int PrintDebugInfo();
private:
	Heap * FindHeap(const char* name);
	HeapFactory();
	~HeapFactory();
	Heap heapList[MAX_HEAPS];

	// Attempts to add a new heap to the heapList returns nullptr
	// if size of heapList is already MAX_HEAPS. 
	// Does NOT check for double entries of name (Use FindHeap())
	Heap* CreateHeap(const char* name);
};

