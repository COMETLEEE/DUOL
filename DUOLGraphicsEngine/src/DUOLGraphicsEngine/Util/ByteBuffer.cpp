#include "ByteBuffer.h"

#include <cstring>

namespace DUOLGraphicsEngine
{
	ByteBuffer::ByteBuffer():
		_byteSize(8384)
		, _buffer(nullptr)
	{
		//바이트사이즈는 임의의 하드코딩 값이다.
		_buffer = new char[_byteSize];
	}

	ByteBuffer::~ByteBuffer()
	{
		delete[] _buffer;
	}

	void* ByteBuffer::GetBufferStartPoint()
	{
		return _buffer;
	}

	unsigned ByteBuffer::GetBufferSize()
	{
		return _byteSize;
	}

	void ByteBuffer::WriteData(void* srcData, int srcDataSize, int bufferOffset)
	{
		if(_byteSize < srcDataSize + bufferOffset)
		{
			//다시 할당한다.
			//이방법외에 다른 방법은 없을까?
			char* newBuffer = new char[srcDataSize + bufferOffset];

			memcpy_s(newBuffer, srcDataSize+bufferOffset, _buffer, _byteSize);
			delete[] _buffer;

			_buffer = newBuffer;
			_byteSize = srcDataSize + bufferOffset;
		}

		memcpy_s(_buffer + bufferOffset, _byteSize - bufferOffset, srcData, srcDataSize);
	}

	void ByteBuffer::SetBufferSize(int bufferSize)
	{
		delete[] _buffer;

		_buffer = new char[bufferSize];
		_byteSize = bufferSize;
	}
}
