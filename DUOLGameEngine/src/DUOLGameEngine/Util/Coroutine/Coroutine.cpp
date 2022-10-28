#include "DUOLGameEngine/Util/Coroutine/Coroutine.h"

namespace DUOLGameEngine
{
	Coroutine::Coroutine(const CoroutineHandler& coroutineHandler) :
		YieldInstructionBase(YieldInstructionType::COROUTINE)
		, _coroutineHandler(coroutineHandler)
	{

	}

	void Coroutine::UpdateInstruction(double deltaTime)
	{
		
	}

	bool Coroutine::CanResume()
	{
		
	}
}