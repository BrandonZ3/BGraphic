#include "Strings.h"  

uint64_t Strings::Length(const char* string)
{
	uint64_t i = 0;
	for (; string[i] != 0x0; i++) {}

	return i; //this value is not including the null termination character
}

char* Strings::Concat(const char* string1, const char* string2)
{
	uint64_t string1Length = Length(string1);
	uint64_t string2Length = Length(string2);

	uint64_t totalLength = string1Length + string2Length + 1;

	char* finalString = (char*)MemoryManipulation::MemoryAllocate(totalLength);

	MemoryManipulation::MoveBytes((uint64_t)string1, (uint64_t)finalString, string1Length);
	MemoryManipulation::MoveBytes((uint64_t)string2, (uint64_t)(finalString + string1Length), string2Length + 1);

	return finalString;
}

bool Strings::Contains(const char* string, const char* content)
{
	uint64_t stringLength = Length(string);
	uint64_t contentLength = Length(content);

	bool found = false;

	uint64_t x = 0;

	for (uint64_t i = 0; i < stringLength; i++)
	{
		if (string[i] == content[x])
		{
			x++;
			if (x == contentLength)
				return true;
		}
		else
		{
			if (x > 0)
				i--;
			x = 0;
		}
	}
	return false;
}

int64_t Strings::IndexOf(const char* string, const char* content, uint64_t iteration)
{
	int64_t index = -1;
	uint64_t stringLength = Length(string);
	uint64_t contentLength = Length(content);

	bool found = false;

	uint64_t x = 0;

	if (iteration > 0)
	{
		for (uint64_t i = 0; i < stringLength; i++)
		{
			if (string[i] == content[x])
			{
				x++;
				if (x == contentLength)
				{
					if (iteration > 1)
					{
						x = 0;
					}
					iteration--;
					if (iteration == 0)
					{
						index = i - (contentLength - 1);
						return index;
					}
				}
			}
			else
			{
				if (x > 0)
					i--;
				x = 0;
			}
		}
	}
	return index;
}

int64_t Strings::CountOf(const char* string, const char* content)
{
	int64_t count = 0;
	uint64_t stringLength = Length(string);
	uint64_t contentLength = Length(content);

	bool found = false;

	uint64_t x = 0;

	for (uint64_t i = 0; i < stringLength; i++)
	{
		if (string[i] == content[x])
		{
			x++;
			if (x == contentLength)
			{
				count++;
				x = 0;
			}
		}
		else
		{
			if (x > 0)
				i--;
			x = 0;
		}
	}

	return count;
}

int64_t Strings::CharCount(const char* string, char character)
{
	int64_t output = 0;
	int64_t stringLen = strlen(string);

	for (int64_t i = 0; i < stringLen; i++)
	{
		if (string[i] == character)
			output++;
	}
	return output;
}

char* Strings::Substring(const char* string, uint64_t startIndex, uint64_t length)
{
	char* newString = (char*)MemoryManipulation::MemoryAllocate(length + 1);
	newString[length] = 0;
	MemoryManipulation::MoveBytes((uint64_t)(string + startIndex), (uint64_t)newString, length);
	return newString;
}

bool Strings::Compare(const char* string1, const char* string2)
{
	uint64_t len1 = Length(string1);
	uint64_t len2 = Length(string2);

	if (len1 == len2)
	{
		for (uint64_t i = 0; i < len1; i++)
		{
			if (string1[i] != string2[i])
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool Strings::CompareCaseInsensitive(const char* string1, const char* string2) // Check ME PLS
{
	int strLen1 = strlen(string1);
	int strLen2 = strlen(string2);

	if (strLen1 != strLen2)
		return false;

	for (int i = 0; i < strLen1; i++)
	{
		unsigned char char1 = string1[i];
		unsigned char char2 = string2[i];

		if (char1 > 0x40 && char1 < 0x5B)
		{
			char1 += 0x20;
		}

		if (char2 > 0x40 && char2 < 0x5B)
		{
			char2 += 0x20;
		}

		if (char1 != char2)
		{
			return false;
		}
	}

	return true;
}

char* Strings::Replace(const char* string, const char* content, const char* replacementContent)
{
	uint64_t 	stringLength = Length(string);
	uint64_t	contentLength = Length(content);
	uint64_t 	repLength = Length(replacementContent);

	uint64_t count = CountOf(string, content);

	uint64_t newLength = (stringLength - (contentLength * count)) + (repLength * count);

	char* out = (char*)MemoryManipulation::MemoryAllocate(newLength + 1);

	uint64_t stringPosition = (uint64_t)string;
	uint64_t outPosition = (uint64_t)out;
	out[newLength] = 0;

	for (int i = 0; i < count; i++)
	{
		uint64_t positionOfContent = IndexOf((char*)stringPosition, content, 1);
		MemoryManipulation::MoveBytes((uint64_t)(stringPosition), (uint64_t)(outPosition), positionOfContent);
		stringPosition += positionOfContent + contentLength;
		outPosition += positionOfContent;

		MemoryManipulation::MoveBytes((uint64_t)(replacementContent), (uint64_t)(outPosition), repLength);
		outPosition += repLength;
	}

	MemoryManipulation::MoveBytes((uint64_t)(stringPosition), (uint64_t)(outPosition), Length((char*)stringPosition));

	return out;
}

char* Strings::ReplaceChar(const char* string, char oldchar, char newchar) //Check ME PLS
{
	int stringLength = strlen(string);

	char* newString = (char*)malloc(stringLength + 1);

	newString[stringLength] = 0;

	for (int i = 0; i < stringLength; i++)
	{
		if (string[i] == oldchar)
			newString[i] = newchar;
		else
			newString[i] = string[i];
	}

	return newString;
}

bool Strings::IsNumeric(const char* string) //Check ME PLS
{
	size_t stringLength = strlen(string);

	for (size_t i = 0; i < stringLength; i++)
	{
		if (string[i] > 0x39 || string[i] < 0x30)
			return false;
	}

	return true;
}

char* Strings::Insert(const char* string, const char* content, uint64_t index)
{
	uint64_t	stringLength = Length(string);
	uint64_t 	contentLength = Length(content);

	uint64_t 	newLength = contentLength + stringLength;

	char* out = (char*)MemoryManipulation::MemoryAllocate(newLength + 1);

	MemoryManipulation::MoveBytes((uint64_t)string, (uint64_t)out, index);
	MemoryManipulation::MoveBytes((uint64_t)content, (uint64_t)(out + index), contentLength);
	MemoryManipulation::MoveBytes((uint64_t)(string + index), (uint64_t)(out + index + contentLength), (stringLength - index) + 1);

	return out;
}

char* Strings::UintToString(uint64_t value)
{
	char* out;
	char values[100];

	for (int i = 0; i < 100; i++)
	{
		values[i] = 0;
	}

	int i = 0;

	if (value != 0)
	{
		while (value != 0)
		{
			uint8_t mod = value % 10;
			values[i] = mod + 0x30;
			value /= 10;
			i++;
		}

		out = (char*)MemoryManipulation::MemoryAllocate(i + 1);
		out[i] = 0;

		//i should be the count now
		for (int x = (i - 1); x > -1; x--)
		{
			out[(i - 1) - x] = values[x];
		}
	}
	else
	{
		out = (char*)MemoryManipulation::MemoryAllocate(2);
		out[0] = 0x30;
		out[1] = 0x0;
	}

	return out;
}

char* Strings::UintToHex(uint64_t value)
{
	char* out;
	char values[100];

	for (int i = 0; i < 100; i++)
	{
		values[i] = 0;
	}

	int i = 0;

	if (value != 0)
	{
		while (value != 0)
		{
			uint8_t val = (value & 0xF);
			if (val > 0x9)
			{
				values[i] = (val - 10) + 0x41;
			}
			else
			{
				values[i] = val + 0x30;
			}
			value >>= 4;
			i++;
		}

		out = (char*)MemoryManipulation::MemoryAllocate(i + 1);
		out[i] = 0;

		//i should be the count now
		for (int x = (i - 1); x > -1; x--)
		{
			out[(i - 1) - x] = values[x];
		}
	}
	else
	{
		out = (char*)MemoryManipulation::MemoryAllocate(2);
		out[0] = 0x30;
		out[1] = 0x0;
	}


	return out;
}

uint64_t Strings::StringToUint(const char* string)
{
	char* item = Clone(string);
	uint64_t output = 0;
	bool badItem = false;
	bool hexItem = false;

	if (Contains(item, (char*)"0x"))
	{
		FreeAndAssign(&item, Substring(item, 2, Length(item) - 2));
		hexItem = true;
	}

	for (int i = 0; i < Length(item); i++)
	{

		if ((item[i] >= 0x41 && item[i] <= 0x46) || (item[i] >= 0x61 && item[i] < 0x66))
		{
			hexItem = true;
		}
		else if (item[i] >= 0x30 && item[i] <= 0x39)
		{

		}
		else
		{
			badItem = true;
		}
	}

	if (!badItem)
	{
		if (hexItem)
		{
			for (int i = 0; i < Length(item); i++)
			{
				char currentByte;

				if ((item[i] >= 0x61 && item[i] < 0x66))
				{
					currentByte = (item[i] - 0x61) + 0xA;
				}
				else if ((item[i] >= 0x41 && item[i] <= 0x46))
				{
					currentByte = (item[i] - 0x41) + 0xA;
				}
				else
				{
					currentByte = item[i] - 0x30;
				}

				output |= currentByte;

				if (i < Length(item) - 1)
					output <<= 4;
			}
		}
		else
		{
			for (int i = 0; i < Length(item); i++)
			{
				char currentByte = item[i] - 0x30;

				output += currentByte;

				if (i < Length(item) - 1)
					output *= 10;
			}
		}

	}
	else
	{
		output = 0;
	}

	Free(item);

	return output;
}

unsigned char Strings::StringToUnsignedChar(const char* string)
{
	unsigned char output = 0;
	size_t stringLength = strlen(string);

	if (!IsNumeric(string))
	{
		return output;
	}

	int size = 0;

	for (size_t i = 0; i < stringLength; i++)
	{
		if (size == 3)
			break;
		output += string[i] - 0x30;
		if (i == stringLength - 1)
			break;
		output *= 10;
		size++;
	}

	return output;
}

unsigned short Strings::StringToUnsignedWord(const char* string)
{
	unsigned short output = 0;
	size_t stringLength = strlen(string);

	if (!IsNumeric(string))
	{
		return output;
	}

	int size = 0;

	for (size_t i = 0; i < stringLength; i++)
	{
		if (size == 5)
			break;
		output += string[i] - 0x30;
		if (i == stringLength - 1)
			break;
		output *= 10;
		size++;
	}

	return output;
}

unsigned long Strings::StringToUnsignedDword(const char* string)
{
	unsigned long output = 0;
	size_t stringLength = strlen(string);

	if (!IsNumeric(string))
	{
		return output;
	}

	int size = 0;

	for (size_t i = 0; i < stringLength; i++)
	{
		if (size == 11)
			break;
		output += string[i] - 0x30;
		if (i == stringLength - 1)
			break;
		output *= 10;
		size++;
	}

	return output;
}

char* Strings::Trim(const char* string) //Check ME PLS
{
	char* result = NULL;
	int stringLength = strlen(string);

	int start = -1;
	int end = -1;

	for (int i = 0; i < stringLength; i++)
	{
		if (start == -1 && string[i] != 0x20)
		{
			start = i;
			break;
		}
	}

	for (int i = stringLength - 1; i >= 0; i--)
	{
		if (end == -1 && string[i] != 0x20)
		{
			end = i;
			break;
		}
	}

	if (start != -1 && end != -1)
	{
		int length = end - start;
		result = (char*)malloc(length + 2);

		int pos = 0;

		for (int i = start; i <= end; i++)
		{
			result[pos] = string[i];
			pos++;
		}

		result[length + 1] = 0;
		return result;
	}
	else if (start == end && start == -1)
	{
		result = (char*)malloc(1);
		result[0] = 0;
		return result;
	}
}

char* Strings::ToLowerString(const char* string) // Check ME PLS
{
	int stringLength = strlen(string);

	char* newString = (char*)malloc(stringLength + 1);

	newString[stringLength] = 0;

	for (int i = 0; i < stringLength; i++)
	{
		if (string[i] > 0x40 && string[i] < 0x5B)
			newString[i] = string[i] + 0x20;
		else
			newString[i] = string[i];
	}

	return newString;
}

void Strings::Free(char* string)
{
	MemoryManipulation::MemoryFree(string, Length(string) + 1);
}

char* Strings::Clone(const char* string)
{
	char* out = NULL;
	uint64_t stringLength = Length(string);

	out = (char*)MemoryManipulation::MemoryAllocate(stringLength + 1);

	MemoryManipulation::MoveBytes((uint64_t)string, (uint64_t)out, stringLength + 1);

	return out;
}

void Strings::FreeAndAssign(char** strref, char* string)
{
	Free(strref[0]);
	strref[0] = string;
}

bool Strings::IsIp(const char* string)
{
	size_t sep = CharCount(string, '.');

	if (sep == 3)
	{
		PointerList* myArr = PointerList::SplitString(string, ".");

		for (size_t i = 0; i < myArr->count; i++)
		{
			if (StringToUnsignedWord(string) > 256)
			{
				myArr->FreeEverything();
				delete myArr;
				return false;
			}
		}

		myArr->FreeEverything();
		delete myArr;
	}
	else
		return false;
	return true;
}

ULONG Strings::IPToDwordNetwork(const char* ipAddress)
{
	ULONG result;
	unsigned char parts[4];
	PointerList* ipItems = PointerList::SplitString(ipAddress, ".");

	if (ipItems->count == 4)
	{
		for (size_t i = 0; i < ipItems->count; i++)
		{
			parts[i] = StringToUnsignedChar((char*)ipItems->items[i]);
		}
		ipItems->FreeEverything();
		delete ipItems;
	}
	else
	{
		ipItems->FreeEverything();
		delete ipItems;
		return -1;
	}

	((char*)(&result))[0] = parts[0];
	((char*)(&result))[1] = parts[1];
	((char*)(&result))[2] = parts[2];
	((char*)(&result))[3] = parts[3];

	return result;
}

bool Strings::IsHexString(const char* string, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		if (!((string[i] >= 0x30 && string[i] <= 0x39) || (string[i] >= 0x41 && string[i] <= 0x46) || (string[i] >= 0x61 && string[i] <= 0x66)))
		{
			return false;
		}
	}
	return true;
}

char* Strings::StringToHexString(const char* string)
{
	char* output = NULL;

	size_t stringLen = strlen(string);
	size_t pos = 0;

	output = (char*)malloc((stringLen * 2) + 1);
	output[stringLen * 2] = 0;

	for (size_t i = 0; i < stringLen; i++)
	{
		unsigned char val = string[i];

		unsigned char low = val % 16;
		unsigned char high = (val / 16) % 16;

		if (low < 10)
		{
			low += 0x30;
		}
		else
		{
			low += (0x61 - 10);
		}

		if (high < 10)
		{
			high += 0x30;
		}
		else
		{
			high += (0x61 - 10);
		}

		output[pos] = high;
		output[pos + 1] = low;
		pos += 2;
	}

	return output;
}

DBuffer* Strings::BufferFromHexString(const char* string)
{
	DBuffer* buf = new DBuffer(4096);

	size_t stringLen = strlen(string);

	if (stringLen % 2 == 0 && IsHexString(string, stringLen))
	{
		for (size_t i = 0; i < stringLen; i += 2)
		{
			unsigned char val = 0;
			if ((string[i] >= 0x30 && string[i] <= 0x39))
			{
				val += (string[i] - 0x30) * 16;
			}
			else if (string[i] >= 0x41 && string[i] <= 0x46)
			{
				val += ((string[i] - 0x41) + 10) * 16;
			}
			else if (string[i] >= 0x61 && string[i] <= 0x66)
			{
				val += ((string[i] - 0x61) + 10) * 16;
			}

			if ((string[i + 1] >= 0x30 && string[i + 1] <= 0x39))
			{
				val += (string[i + 1] - 0x30);
			}
			else if (string[i + 1] >= 0x41 && string[i + 1] <= 0x46)
			{
				val += (string[i + 1] - 0x41) + 10;
			}
			else if (string[i + 1] >= 0x61 && string[i + 1] <= 0x66)
			{
				val += (string[i + 1] - 0x61) + 10;
			}

			buf->Add(val);
		}
	}

	return buf;
}

char* Strings::CreateIndent(int depth)
{
	int length = depth;
	char* tabbed = (char*)malloc(length + 1);
	tabbed[length] = 0;

	for (int i = 0; i < length; i++)
	{
		tabbed[i] = '\t';
	}
	return tabbed;
}


char* Strings::FloatToString(float value)
{
	DBuffer* buf = new DBuffer();

	uint32_t exponent = (*((uint32_t*)&value) & 0x7F800000) >> 23;
	bool sign = (*((uint32_t*)&value) & 0x80000000) == 1;
	uint32_t mantissa = 0;

	if (exponent >= 127)
		mantissa = ((*((uint32_t*)&value) & 0x007FFFFF) | 0x00800000) << (exponent - 127);
	else
		mantissa = ((*((uint32_t*)&value) & 0x007FFFFF) | 0x00800000) >> (127 - exponent);

	bool firstValue = false;
	while (mantissa != 0)
	{
		unsigned char window = (mantissa >> 23 & 0xFF);
		uint32_t value = window % 10;
		buf->Add(value);
		mantissa *= 10;
	}

	int z = 0;
	delete buf;
	return NULL;
}

static float StringToFloat(char*)
{

}