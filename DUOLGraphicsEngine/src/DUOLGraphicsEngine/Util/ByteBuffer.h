#pragma once

namespace DUOLGraphicsEngine
{
 /**

     @class   ByteBuffer
     @brief   ���������� �����͸� ������ �� �ִ� ����Ʈ�����Դϴ�.
     @details ~
     @author  KyungMin Oh

 **/
	class ByteBuffer
	{
	public:
		ByteBuffer();

		~ByteBuffer();

		//Non Copyable. ����� �Ұ����ϵ����Ѵ�.
		ByteBuffer(const ByteBuffer&) = delete;

		ByteBuffer& operator = (const ByteBuffer&) = delete;

	public:
		void* GetBufferStartPoint();

		unsigned int GetBufferSize();

		void WriteData(void* srcData, int srcDataSize, int bufferOffset = 0);

		void SetBufferSize(int bufferSize);

	private:
		unsigned int _byteSize;

		char* _buffer;
	};
}
