#include "HeapMemory.h"



#include <iostream>
Heap::Heap(const char* inName)
{
	for (int i = 0; i < NAME_LENGTH; ++i)
	{
		name[i] = inName[i];
	}
	name[NAME_LENGTH - 1] = '\0';

	numAllocations = 0;
	totalBytes = 0;
	peakBytes = 0;
}


void Heap::AddAllocation(std::size_t numBytes)
{
	++numAllocations;
	totalBytes += numBytes;
	if (totalBytes > peakBytes)
	{
		peakBytes = totalBytes;
	}
	//std::cout << this->name << ": " << numAllocations << " " << totalBytes << std::endl;
}

void Heap::RemoveAllocation(std::size_t numBytes)
{
	--numAllocations;
	totalBytes -= numBytes;
	//std::cout << this->name << ": " << numAllocations << " " << totalBytes << std::endl;
}

const char* Heap::GetName() const
{
	return name;
}

void Heap::SetName(const char* inName)
{
	for (int i = 0; i < NAME_LENGTH; ++i)
	{
		name[i] = inName[i];
	}
	name[NAME_LENGTH - 1] = '\0';
}