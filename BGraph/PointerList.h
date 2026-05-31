#pragma once 

class PointerList
{
private:
	uint64_t size;

	char* Clone(const char* string);

public:
	void** items;
	uint64_t count;
	PointerList(PointerList&&) = delete;
	PointerList();

	bool AddPointer(void* ItemAddress);

	bool RemovePointer(uint64_t index);

	PointerList* CloneEverything(int startIndex, int count);

	static PointerList* SplitString(const char* string, const char* seperator);

	void FreePointers();

	void FreeListBuffer();

	void FreeEverything();

	~PointerList();
};
