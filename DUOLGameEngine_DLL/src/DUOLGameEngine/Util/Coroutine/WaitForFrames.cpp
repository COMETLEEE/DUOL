#include "DUOLGameEngine/Util/Coroutine/WaitForFrames.h"

namespace DUOLGameEngine
{
	WaitForFrames::WaitForFrames(int frameCount) :
		YieldInstructionBase(YieldInstructionType::WAIT_FOR_FRAMES)
		, _waitFrame(frameCount)
	{
	}

	WaitForFrames::~WaitForFrames()
	{
	}

	void WaitForFrames::UpdateInstruction(float deltaTime)
	{
		_waitFrame--;
	}

	bool WaitForFrames::CanResume()
	{
		return _waitFrame <= 0 ? true : false;
	}
}