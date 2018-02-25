#include "HeapFactory.h"

#include <cstring>
#include <iostream>
#include <iomanip>      // std::setw

HeapFactory::HeapFactory()
{

}


HeapFactory::~HeapFactory()
{
}

Heap* HeapFactory::FindHeap(const char* name)
{
	for (int i = 0; i < MAX_HEAPS; ++i)
	{
		if (!strcmp(heapList[i].GetName(), name))
		{
			return &heapList[i];
		}
	}
	return nullptr;
}

Heap* HeapFactory::GetHeap(const char* name)
{
	if (Heap* heap = FindHeap(name))
	{
		return heap;
	}
	else
	{
		heap = CreateHeap(name);
		return heap;
	}
}


Heap* HeapFactory::CreateHeap(const char* name)
{
	for (int i = 0; i < MAX_HEAPS; ++i)
	{
		if (!strcmp(heapList[i].GetName(), ""))
		{
			heapList[i].SetName(name);
			return &heapList[i];
		}
	}
	std::cout << "Failed to create heap: " << name << std::endl;
	return nullptr;
}

void HeapFactory::PrintDebugInfo()
{
	std::cout << std::setw(10) << std::left << "Heap"
		<< std::setw(10) << std::left << "Bytes"
		<< std::setw(10) << std::left << "Peak"
		<< std::setw(10) << std::left << "Net Allocations"
		<< std::endl;
	
	for (int i = 0; i < MAX_HEAPS; ++i)
	{
		if (!strcmp(heapList[i].GetName(), ""))
		{
			break;
		}
		else
		{
			std::cout << std::setw(10) << std::left << heapList[i].GetName() 
					  << std::setw(10) << std::left << heapList[i].GetBytes() 
					  << std::setw(10) << std::left << heapList[i].GetPeakBytes() 
					  << std::setw(10) << std::left << heapList[i].GetNetAllocations() 
					  << std::endl;
		}
	}
}
//Heap* HeapFactory::GetDefaultHeap()
//{
//	return GetHeap("Default");
//}