#include "KeyPointerPair.h"  

KeyPointerPair::KeyPointerPair(char* key, void* pointer)
{
	this->key = key;
	this->pointer = pointer;
}

KeyPointerPair* KeyPointerPair::GetKeyValuePointer(PointerList* pointerList, const char* key)
{
	for (int i = 0; i < pointerList->count; i++)
	{
		KeyPointerPair* kpp = (KeyPointerPair*)pointerList->items[i];
		if (Strings::Compare(key, kpp->key))
			return kpp;
	}
	return NULL;
}