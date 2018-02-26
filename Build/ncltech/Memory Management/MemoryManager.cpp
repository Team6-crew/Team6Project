#include "MemoryManager.h"

#include "HeapFactory.h"
#include "HeapMemory.h"

#include <cassert>
// Signature for memory allocated using this manager
const int MEMORY_SIGNATURE = 0xDEADBEEF;
const int END_MARKER = 0xBAADF00D;
//MemoryManager::MemoryManager()
//{
//}
//
//
//MemoryManager::~MemoryManager()
//{
//}
Heap* GetDefaultHeap()
{
	static Heap defaultHeap("Default");
	return &defaultHeap;
}
//TODO: IFDEF debug
// Overwritten global new and delete
void* operator new(std::size_t size, Heap* heap)
{
	// Allocate memory
	std::size_t bytesRequested = size + sizeof(AllocHeader) + sizeof(int);
	char* memory = (char*)malloc(bytesRequested);

	// Setup header
	AllocHeader* header = (AllocHeader*)memory;
	header->heap = heap;
	header->sizeBytes = size;
	header->signature = MEMORY_SIGNATURE;

	// Setup memory
	heap->AddAllocation(size);
	void* memoryStart = memory + sizeof(AllocHeader);

	// Setup End Marker
	int* endMarker = (int*)((char*)(memoryStart) + size);
	*endMarker = END_MARKER;

	return memoryStart;
}

void operator delete(void* memory)
{
	
	AllocHeader* header = (AllocHeader*)((char*) memory- sizeof(AllocHeader));
	// Guard against deleting memory not allocated using this manager
	if (header->signature == MEMORY_SIGNATURE)
	{
		header->heap->RemoveAllocation(header->sizeBytes);

		// Test if memory is being overwritten somewhere
		int* endMarker = (int*)((char*)(memory)+header->sizeBytes);
		assert(*endMarker == END_MARKER);

		free(header);
	}
	else
	{
		free(memory);
	}
}

// Version of new to allow dynamic memory allocation without using custom heap
void* operator new(std::size_t size)
{
	return operator new(size, GetDefaultHeap());
}

#include <iostream>

void * operator new[](std::size_t size, Heap* heap)
{
	// Allocate memory
	std::size_t bytesRequested = size + sizeof(AllocHeader) + sizeof(int);
	char* memory = (char*)malloc(bytesRequested);

	// Setup header
	AllocHeader* header = (AllocHeader*)memory;
	header->heap = heap;
	header->sizeBytes = size;
	header->signature = MEMORY_SIGNATURE;

	// Setup memory
	heap->AddAllocation(size);
	void* memoryStart = memory + sizeof(AllocHeader);

	// Setup End Marker
	int* endMarker = (int*)((char*)(memoryStart)+size);
	*endMarker = END_MARKER;

	return memoryStart;
}

void  operator delete[](void* memory) {
	AllocHeader* header = (AllocHeader*)((char*)memory - sizeof(AllocHeader));
	// Guard against deleting memory not allocated using this manager
	if (header->signature == MEMORY_SIGNATURE)
	{
		header->heap->RemoveAllocation(header->sizeBytes);

		// Test if memory is being overwritten somewhere
		int* endMarker = (int*)((char*)(memory)+header->sizeBytes);
		assert(*endMarker == END_MARKER);

		free(header);
	}
	else
	{
		free(memory);
	}
}