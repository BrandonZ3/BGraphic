#include "DBuffer.h"  

DBuffer::DBuffer()
{
	this->_buffer = (unsigned char*)malloc(1000);
	if (this->_buffer != NULL)
	{
		size = 1000;
	}
}

DBuffer::DBuffer(uint64_t initialSize)
{
	this->_buffer = (unsigned char*)malloc(initialSize);
	if (this->_buffer != NULL)
	{
		size = initialSize;
	}
}

DBuffer::~DBuffer()
{
	free(this->_buffer);
}

void DBuffer::Add(uint8_t value)
{
	if (this->count + 1 < this->size)
	{
		this->_buffer[this->count] = value;
		this->count++;
	}
	else
	{
		unsigned char* newbuffer = (unsigned char*)realloc(this->_buffer, this->size * 2);
		if (newbuffer != NULL)
		{
			this->_buffer = newbuffer;
			this->size = this->size * 2;
			this->_buffer[this->count] = value;
			this->count++;
		}
	}
}

void DBuffer::Add(unsigned char* buffer, uint64_t byteLength)
{
	if (this->count + byteLength < this->size)
	{
		for (uint64_t i = 0; i < byteLength; i++)
		{
			this->_buffer[this->count + i] = buffer[i];
		}
		this->count += byteLength;
	}
	else
	{
		unsigned char* newbuffer = (unsigned char*)realloc(this->_buffer, (this->size * 2) + byteLength);
		if (newbuffer != NULL)
		{
			this->_buffer = newbuffer;
			this->size = (this->size * 2) + byteLength;

			for (uint64_t i = 0; i < byteLength; i++)
			{
				this->_buffer[this->count + i] = buffer[i];
			}
			this->count += byteLength;
		}
	}
}

void DBuffer::Add(DBuffer* buffer)
{
	if (this->count + buffer->count < this->size)
	{
		unsigned char* tempbuf = buffer->DataPointer(0);
		for (uint64_t i = 0; i < buffer->count; i++)
		{
			this->_buffer[this->count + i] = tempbuf[i];
		}
		this->count += buffer->count;
	}
	else
	{
		unsigned char* newbuffer = (unsigned char*)realloc(this->_buffer, (this->size * 2) + buffer->count);
		if (newbuffer != NULL)
		{
			this->_buffer = newbuffer;
			this->size = (this->size * 2) + buffer->count;

			unsigned char* tempbuf = buffer->DataPointer(0);
			for (uint64_t i = 0; i < buffer->count; i++)
			{
				this->_buffer[this->count + i] = tempbuf[i];
			}
			this->count += buffer->count;
		}
	}
}

unsigned char* DBuffer::BufferOut(uint64_t startIndex, uint64_t byteLength)
{
	unsigned char* returnbuffer = NULL;
	if (startIndex < this->count && byteLength > 0 && startIndex + byteLength <= this->count)
	{
		returnbuffer = (unsigned char*)malloc(byteLength);

		if (returnbuffer != NULL)
		{
			MemoryManipulation::MoveBytes((uint64_t)this->_buffer + startIndex, (uint64_t)returnbuffer, byteLength);
		}
	}
	return returnbuffer;
}

void DBuffer::Clear()
{
	this->count = 0;
}

unsigned char* DBuffer::DataPointer(uint64_t index)
{
	if (index < this->count)
		return (unsigned char*)((uint64_t)this->_buffer + index);
	return NULL;
}

void DBuffer::Insert(uint64_t startIndex, unsigned char value)
{
	uint64_t remaining = this->count - startIndex;
	if (startIndex < this->count)
	{
		if (this->count + 1 < this->size)
		{
			MemoryManipulation::MoveBytes((uint64_t)this->_buffer + startIndex, (uint64_t)this->_buffer + startIndex + 1, remaining);
			this->_buffer[startIndex] = value;
			this->count++;
		}
		else
		{
			unsigned char* newbuffer = (unsigned char*)realloc(this->_buffer, this->size * 2);
			if (newbuffer != NULL)
			{
				this->_buffer = newbuffer;
				this->size = this->size * 2;
				MemoryManipulation::MoveBytes((uint64_t)this->_buffer + startIndex, (uint64_t)this->_buffer + startIndex + 1, remaining);
				this->_buffer[startIndex] = value;
				this->count++;
			}
		}
	}
}

void DBuffer::Insert(uint64_t startIndex, unsigned char* buffer, uint64_t byteLength)
{
	uint64_t remaining = this->count - startIndex;

	if (startIndex < this->count && byteLength > 0)
	{
		if (this->count + byteLength < this->size)
		{
			MemoryManipulation::MoveBytes((uint64_t)this->_buffer + startIndex, (uint64_t)this->_buffer + startIndex + byteLength, remaining);
			MemoryManipulation::MoveBytes((uint64_t)buffer, (uint64_t)this->_buffer + startIndex, byteLength);
			this->count += byteLength;
		}
		else
		{
			unsigned char* newbuffer = (unsigned char*)realloc(this->_buffer, this->size * 2);
			if (newbuffer != NULL)
			{
				this->_buffer = newbuffer;
				this->size = this->size * 2;
				MemoryManipulation::MoveBytes((uint64_t)this->_buffer + startIndex, (uint64_t)this->_buffer + startIndex + byteLength, remaining);
				MemoryManipulation::MoveBytes((uint64_t)buffer, (uint64_t)this->_buffer + startIndex, byteLength);
				this->count += byteLength;
			}
		}
	}
}

bool DBuffer::MatchBytes(unsigned char* source1, uint64_t byteLength1, uint64_t offset)
{
	if (offset < this->count && offset + byteLength1 <= this->count)
		return Bytes::MatchBytes(source1, byteLength1, (unsigned char*)(this->_buffer + offset));
	return false;
}

void DBuffer::Remove(uint64_t index)
{
	if (index < this->count)
	{
		uint64_t remaining = this->count - (index + 1);
		if (remaining > 0)
			MemoryManipulation::MoveBytes((uint64_t)this->_buffer + index + 1, (uint64_t)this->_buffer + index, remaining);
		this->count--;
	}
}

void DBuffer::Remove(uint64_t startIndex, uint64_t byteLength)
{
	if (startIndex < this->count && byteLength > 0 && startIndex + byteLength <= this->count)
	{
		uint64_t remaining = this->count - (startIndex + byteLength);
		if (remaining > 0)
			MemoryManipulation::MoveBytes((uint64_t)this->_buffer + startIndex + byteLength, (uint64_t)this->_buffer + startIndex, remaining);
		this->count -= byteLength;
	}
}

DBuffer* DBuffer::Slice(uint64_t startIndex, uint64_t byteLength)
{
	DBuffer* returnbuffer = NULL;
	if (startIndex < this->count && byteLength > 0 && startIndex + byteLength <= this->count)
	{
		returnbuffer = new DBuffer(byteLength);

		if (returnbuffer != NULL)
		{
			returnbuffer->Add(this->_buffer + startIndex, byteLength);
		}
	}
	return returnbuffer;
}