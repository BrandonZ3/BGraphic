#include "PointerList.h"  

char* PointerList::Clone(const char* string)
{
	char* out = NULL;
	uint64_t stringLength = strlen(string);

	out = (char*)MemoryManipulation::MemoryAllocate(stringLength + 1);

	MemoryManipulation::MoveBytes((uint64_t)string, (uint64_t)out, stringLength + 1);

	return out;
}

PointerList::PointerList()
{
	this->count = 0;
	this->size = (sizeof(void*) * 20);
	this->items = (void**)MemoryManipulation::MemoryAllocate(this->size);
}

bool PointerList::AddPointer(void* ItemAddress)
{
	if (((this->count + 1) * sizeof(void*) >= this->size))
	{
		this->items = (void**)MemoryManipulation::MemoryReallocate(this->items, this->size, this->size * 2);
		this->size *= 2;
	}

	if (((this->count + 1) * sizeof(void*) < this->size))
	{
		this->items[this->count] = ItemAddress;
		this->count++;
		return true;
	}
	return false;
}

bool PointerList::RemovePointer(uint64_t index)
{
	if (index < this->count)
	{
		uint64_t bytesToMove = (this->count - (index + 1)) * (sizeof(void*));

		uint64_t startPosition = ((uint64_t)this->items) + ((index + 1) * (sizeof(void*)));

		uint64_t endPosition = ((uint64_t)this->items) + (index * (sizeof(void*)));

		MemoryManipulation::MoveBytes((uint64_t)startPosition, (uint64_t)endPosition, bytesToMove);

		this->count -= 1;
		return true;
	}
	return false;
}

PointerList* PointerList::CloneEverything(int startIndex, int count)
{
	if (startIndex + count <= this->count)
	{
		PointerList* clone = new PointerList();
		clone->count = count;

		int pos = 0;

		for (int i = startIndex; i < startIndex + count; i++)
		{
			clone->items[pos] = Clone((char*)this->items[i]);
			pos++;
		}
		return clone;
	}
	else
		return NULL;
}

PointerList* PointerList::SplitString(const char* string, const char* seperator)
{
	PointerList* stringArray = new PointerList();

	int stringLength = strlen(string);
	int seperatorLength = strlen(seperator);

	int start = -1;
	int end = -1;

	for (int i = 0; i < stringLength; i++)
	{
		bool found = false;

		for (int x = 0; x < seperatorLength; x++)
		{
			if (string[i + x] == seperator[x])
			{
				if (x == seperatorLength - 1)
				{
					found = true;
					break;
				}
				else
					continue;
			}
			else
			{
				break;
			}
		}

		if (found && end == -1)
		{
			if (start == -1)
			{
				i += seperatorLength - 1;
				continue;
			}
			else
			{
				end = i;

				int length = end - start;

				char* newstring = (char*)malloc(length + 1);
				newstring[length] = 0;

				int pos = 0;

				for (int n = start; n < end; n++)
				{
					newstring[pos] = string[n];
					pos++;
				}

				stringArray->AddPointer(newstring);

				i += seperatorLength - 1;
				start = -1;
				end = -1;
			}
		}

		if (start == -1 && !found)
			start = i;

		if (start != -1 && end == -1 && i == stringLength - 1)
		{
			end = i + 1;

			int length = end - start;

			char* newstring = (char*)malloc(length + 1);
			newstring[length] = 0;

			int pos = 0;

			for (int n = start; n < end; n++)
			{
				newstring[pos] = string[n];
				pos++;
			}

			stringArray->AddPointer(newstring);

			i += seperatorLength - 1;
			start = -1;
			end = -1;
		}
	}

	return stringArray;
}

void PointerList::FreePointers()
{
	for (int i = 0; i < this->count; i++)
	{
		free(this->items[i]);
	}
}

void PointerList::FreeListBuffer()
{
	if (this->items != NULL)
	{
		MemoryManipulation::MemoryFree(this->items, this->size);
		this->items = NULL;
	}
}

void PointerList::FreeEverything()
{
	this->FreePointers();
	this->FreeListBuffer();
}

PointerList::~PointerList()
{
	this->FreeListBuffer();
}