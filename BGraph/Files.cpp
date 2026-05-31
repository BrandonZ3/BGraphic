#include "Files.h"  

bool Files::FileExists(char* fileName) //does this work
{
	bool exists = false;
	FILE* ptr = NULL;
	fopen_s(&ptr, fileName, "a");

	if (NULL != ptr)
	{
		exists = true;
		fclose(ptr);
	}
	return exists;
}

DBuffer* Files::ReadFile(char* fileName)
{
	FILE* ptr = NULL;
	DBuffer* file = new DBuffer(1000000);

	fopen_s(&ptr, fileName, "rb");
	if (NULL != ptr)
	{
		unsigned char buffer[4000];
		int cnt = 0;
		do
		{
			cnt = fread(buffer, 1, 4000, ptr);
			file->Add((unsigned char*)buffer, cnt);
		} while (!feof(ptr) && cnt == 4000);
		fclose(ptr);
	}
	else
	{
		delete file;
		return NULL;
	}

	return file;
}

int Files::ReadFile(FILE* file, DBuffer* buffer)
{
	if (buffer != NULL && file != NULL)
	{
		unsigned char buff[4000];
		int good = fseek(file, 0, SEEK_SET);
		if (good == 0)
			while (true)
			{
				uint64_t size = fread(&buff, 1, 4000, file);

				buffer->Add(buff, size);

				if (size < 4000)
					break;
			}
		else
			return -1;
		return 0;
	}
}

int Files::ReadFile(FILE* file, DBuffer* buffer, uint64_t byteLength, long byteOffset)
{
	if (buffer != NULL && file != NULL)
	{
		unsigned char buff[4000];
		int good = fseek(file, byteOffset, SEEK_SET);

		if (good == 0)
			while (true)
			{
				uint64_t size = fread(&buff, 1, 4000, file);

				if (byteLength < size)
				{
					buffer->Add(buff, byteLength);
					byteLength = 0;
				}
				else
				{
					buffer->Add(buff, size);
					byteLength -= 4000;
				}

				if (size < 4000 || byteLength == 0)
					break;
			}
		else
			return -1;
	}
	else
		return -1;
	return 0;
}

int Files::WriteFile(char* fileName, struct DBuffer* file) // does this work
{
	FILE* ptr = NULL;

	fopen_s(&ptr, fileName, "wb+");
	if (ptr != NULL)
	{
		size_t bytes = fwrite(file->DataPointer(0), sizeof(unsigned char), file->count, ptr);

		fclose(ptr);

		if (bytes == file->count)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
}

int Files::WriteFile(FILE* file, DBuffer* buffer)
{
	uint64_t size = fwrite((const void*)buffer->DataPointer(0), 1, buffer->count, file);
	if (size == buffer->count)
		return 0;
	return -1;
}

size_t Files::AppendFile(char* filepath, DBuffer* buffer)
{
	size_t total = 0;
	FILE* file;

	fopen_s(&file, filepath, "a+");

	if (file)
	{
		total = fwrite(buffer->DataPointer(0), 1, buffer->count, file);
	}

	fclose(file);

	return total;
}

unsigned long long Files::GetFileSize(FILE* file) // Check ME PLS
{
	char buffer[1024];

	int len = 1;

	unsigned long long filesize = 0;

	fpos_t currentPosition;

	fgetpos(file, &currentPosition);

	fseek(file, 0, SEEK_SET);

	while (len > 0)
	{
		len = fread_s(&buffer, 1024, 1, 1024, file);
		if (len > 0)
			filesize += len;
	}

	fseek(file, currentPosition, SEEK_SET);

	return filesize;
}

char* Files::GetFileType(const char* path) // Check ME PLS
{

	int length = strlen(path);
	int queryLoc = Strings::IndexOf(path, "?", 0);
	int dotLoc = -1;

	char* output = NULL;

	if (queryLoc != -1)
	{
		// Not Tested
		for (int i = queryLoc; i > 0; i--)
		{
			if (path[i] == '.')
			{
				dotLoc = i;
				break;
			}
		}

		output = (char*)malloc(queryLoc - dotLoc);
		output[queryLoc - dotLoc] = 0;
		int pos = 0;

		for (int i = dotLoc + 1; i < queryLoc; i++)
		{
			output[pos] = path[i];
			pos++;
		}
	}
	else
	{
		for (int i = length - 1; i > 0; i--)
		{
			if (path[i] == '.')
			{
				dotLoc = i;
				break;
			}
		}

		int nsiz = length - dotLoc;

		output = (char*)malloc(nsiz);
		output[nsiz - 1] = 0;
		int pos = 0;

		for (int i = dotLoc + 1; i < length; i++)
		{
			output[pos] = path[i];
			pos++;
		}
	}

	return output;
}