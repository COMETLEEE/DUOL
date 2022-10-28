#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

namespace DUOLGameEngine
{
	WaitForSeconds::WaitForSeconds(double waitTime) :
		YieldInstructionBase(YieldInstructionType::WAIT_FOR_SECONDS)
		, _waitTime(waitTime)
	{

	}

	WaitForSeconds::~WaitForSeconds()
	{

	}
}