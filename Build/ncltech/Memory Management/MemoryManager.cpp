#include "MemoryManager.h"

#include "HeapFactory.h"
#include "HeapMemory.h"

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
	std::size_t bytesRequested = size + sizeof(AllocHeader);
	char* memory = (char*)malloc(bytesRequested);

	// Setup header
	AllocHeader* header = (AllocHeader*)memory;
	header->heap = heap;
	header->sizeBytes = size;

	// Setup memory
	heap->AddAllocation(size);
	void* memoryStart = memory + sizeof(AllocHeader);
	return memoryStart;
}
void operator delete(void* memory)
{
	
	AllocHeader* header = (AllocHeader*)((char*) memory- sizeof(AllocHeader));
	header->heap->RemoveAllocation(header->sizeBytes);
	free(header);
}

// Version of new to allow dynamic memory allocation without using custom heap
void* operator new(std::size_t size)
{
	
	return operator new(size, GetDefaultHeap());
	//return malloc(size);
}

