#include "DUOLGameEngine/Util/Coroutine/WaitForSecondsRealtime.h"

namespace DUOLGameEngine
{
	WaitForSecondsRealtime::WaitForSecondsRealtime(double waitTime) :
		YieldInstructionBase(YieldInstructionType::WAIT_FOR_SECONDS_REALTIME)
		, _waitTime(waitTime)
	{

	}

	WaitForSecondsRealtime::~WaitForSecondsRealtime()
	{

	}
}