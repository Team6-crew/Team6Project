// Heap class used to logically group sets of memory allocations together
#pragma once

#include <string>
#include <nclgl\TSingleton.h>
#include "HeapFactory.h"

class Heap;


// Overwritten global new and delete
void* operator new(std::size_t size, Heap* heap);
void operator delete(void* memory);

// Version of new to allow dynamic memory allocation without using custom heap
void* operator new(std::size_t size);



//class MemoryManager
//{
//public:
//	MemoryManager();
//	~MemoryManager();
//};
//

#define DECLARE_HEAP \
public:\
static void* operator new(std::size_t size); \
static void operator delete(void* memory); \
static void* operator new[](std::size_t size); \
static void operator delete[](void* memory); \
private:\
static Heap* heap;


#define DEFINE_HEAP(classType, heapName) \
Heap* classType::heap = nullptr; \
void* classType::operator new(std::size_t size) \
{ \
	if (!heap) \
	{ \
		heap = HeapFactory::Instance()->GetHeap(heapName);\
	} \
	return ::operator new(size, heap); \
} \
void classType::operator delete(void* memory) \
{ \
	::operator delete(memory); \
}\
void* classType::operator new[](std::size_t size) \
{ \
if (!heap) \
{ \
heap = HeapFactory::Instance()->GetHeap(heapName);\
} \
return ::operator new[](size, heap); \
} \
void classType::operator delete[](void* memory) \
{ \
::operator delete[](memory); \
}