#include "ByteBuffer.h"

#include <cstring>

namespace DUOLGraphicsEngine
{
	ByteBuffer::ByteBuffer():
		_byteSize(8384)
		, _buffer(nullptr)
	{
		//����Ʈ������� ������ �ϵ��ڵ� ���̴�.
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
			//�ٽ� �Ҵ��Ѵ�.
			//�̹���ܿ� �ٸ� ����� ������?
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
