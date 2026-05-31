#pragma once 

class KeyPointerPair
{
public:
	char* key;
	void* pointer;
	KeyPointerPair(char* key, void* pointer);
	static KeyPointerPair* GetKeyValuePointer(PointerList* pointerList, const char* key);
};