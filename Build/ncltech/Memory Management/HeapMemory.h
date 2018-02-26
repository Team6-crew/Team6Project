#pragma once
#include <cstddef>
class Heap
{
	static const int NAME_LENGTH = 15;
public:
	Heap(const char* name = "");

	const char* GetName() const;
	void SetName(const char* name);
	inline unsigned int GetBytes()		    { return totalBytes; }
	inline unsigned int GetPeakBytes()		{ return peakBytes; }
	inline  int			GetNetAllocations() { return numAllocations; }

	void AddAllocation(std::size_t numBytes);
	void RemoveAllocation(std::size_t numBytes);
private:
	char name[NAME_LENGTH];

	int numAllocations = 0;
	unsigned int totalBytes = 0;
	unsigned int peakBytes = 0;
};

// Header for each memory allocation e.g. so delete can get heap information
struct AllocHeader
{
	Heap* heap;
	int	  sizeBytes;
	int	  signature;
};
