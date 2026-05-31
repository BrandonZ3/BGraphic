#pragma once 

class DBuffer
{
private:
	unsigned char* _buffer = NULL;
	uint64_t size = 0;
public:
	uint64_t count = 0;
	DBuffer(DBuffer&&) = delete;
	DBuffer();

	DBuffer(uint64_t initialSize);

	~DBuffer();

	void Add(uint8_t value);

	void Add(unsigned char* buffer, uint64_t byteLength);

	void Add(DBuffer* buffer);

	unsigned char* BufferOut(uint64_t startIndex, uint64_t byteLength);

	void Clear();

	unsigned char* DataPointer(uint64_t index);

	void Insert(uint64_t startIndex, unsigned char value);

	void Insert(uint64_t startIndex, unsigned char* buffer, uint64_t byteLength);

	bool MatchBytes(unsigned char* source1, uint64_t byteLength1, uint64_t offset);

	void Remove(uint64_t index);

	void Remove(uint64_t startIndex, uint64_t byteLength);

	DBuffer* Slice(uint64_t startIndex, uint64_t byteLength);
};
