#pragma once

namespace DUOLGraphicsEngine
{
 /**

     @class   ByteBuffer
     @brief   가변적으로 데이터를 저장할 수 있는 바이트버퍼입니다.
     @details ~
     @author  KyungMin Oh

 **/
	class ByteBuffer
	{
	public:
		ByteBuffer();

		~ByteBuffer();

		//Non Copyable. 복사는 불가능하도록한다.
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
