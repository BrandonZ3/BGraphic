#pragma once 

class Strings
{
public:
	static uint64_t Length(const char* string);

	static char* Concat(const char* string1, const char* string2);

	static bool Contains(const char* string, const char* content);

	static int64_t IndexOf(const char* string, const char* content, uint64_t iteration);

	static int64_t CountOf(const char* string, const char* content);

	static int64_t CharCount(const char* string, char character);

	static char* Substring(const char* string, uint64_t startIndex, uint64_t length);

	static bool Compare(const char* string1, const char* string2);

	static bool CompareCaseInsensitive(const char* string1, const char* string2); // Check ME PLS

	static char* Replace(const char* string, const char* content, const char* replacementContent);

	static char* ReplaceChar(const char* string, char oldchar, char newchar); //Check ME PLS

	static bool IsNumeric(const char* string); //Check ME PLS

	static char* Insert(const char* string, const char* content, uint64_t index);

	static char* UintToString(uint64_t value);

	static char* UintToHex(uint64_t value);

	static uint64_t StringToUint(const char* string);

	static unsigned char StringToUnsignedChar(const char* string);

	static unsigned short StringToUnsignedWord(const char* string);

	static unsigned long StringToUnsignedDword(const char* string);

	static char* Trim(const char* string); //Check ME PLS

	static char* ToLowerString(const char* string); // Check ME PLS

	static void Free(char* string);

	static char* Clone(const char* string);

	static void FreeAndAssign(char** strref, char* string);

	static bool IsIp(const char* string);

	static ULONG IPToDwordNetwork(const char* ipAddress);

	static bool IsHexString(const char* string, size_t length);

	static char* StringToHexString(const char* string);

	DBuffer* BufferFromHexString(const char* string);

	static char* CreateIndent(int depth);

	static char* FloatToString(float value);

	static float StringToFloat(char*);
};
