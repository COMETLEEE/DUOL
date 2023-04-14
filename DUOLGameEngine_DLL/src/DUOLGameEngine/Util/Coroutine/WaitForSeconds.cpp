#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

namespace DUOLGameEngine
{
	WaitForSeconds::WaitForSeconds(float waitTime) :
		YieldInstructionBase(YieldInstructionType::WAIT_FOR_SECONDS)
		, _waitTime(waitTime)
	{

	}

	WaitForSeconds::~WaitForSeconds()
	{

	}

	void WaitForSeconds::UpdateInstruction(float deltaTime)
	{
		// Scaled DeltaTime.
		_waitTime -= deltaTime;
	}

	bool WaitForSeconds::CanResume()
	{
		return _waitTime <= 0.f ? true : false;
	}
}
