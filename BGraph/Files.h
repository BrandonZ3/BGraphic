#pragma once 

class Files
{
public:
	static bool FileExists(char* fileName); //does this work

	static DBuffer* ReadFile(char* fileName);

	static int ReadFile(FILE* file, DBuffer* buffer);

	static int ReadFile(FILE* file, DBuffer* buffer, uint64_t byteLength, long byteOffset);

	static int WriteFile(char* fileName, struct DBuffer* file); // does this work

	static int WriteFile(FILE* file, DBuffer* buffer);

	static size_t AppendFile(char* filepath, DBuffer* buffer);

	static unsigned long long GetFileSize(FILE* file); // Check ME PLS

	static char* GetFileType(const char* path); // Check ME PLS
};
