#pragma once

namespace DUOLGraphicsLibrary
{
	enum class CommandBufferFlags
	{
		Deferred = ( 1 << 0),
		Multi = ( 1 << 1 ),
	};

	struct CommandBufferDesc
	{
		long flag = 0;

		int numBuffers;
	};


}
