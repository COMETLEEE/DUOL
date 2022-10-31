#include "DUOLGameEngine/Util/Coroutine/WaitForSecondsRealtime.h"

#include "DUOLGameEngine/Manager/TimeManager.h"

namespace DUOLGameEngine
{
	WaitForSecondsRealtime::WaitForSecondsRealtime(float waitTime) :
		YieldInstructionBase(YieldInstructionType::WAIT_FOR_SECONDS_REALTIME)
		, _waitTime(waitTime)
	{

	}

	WaitForSecondsRealtime::~WaitForSecondsRealtime()
	{

	}

	void WaitForSecondsRealtime::UpdateInstruction(float deltaTime)
	{
		// Time scale과 관련이 없는 실제 시간의 흐름으로 확인합니다.
		_waitTime -= DUOLGameEngine::TimeManager::GetInstance()->GetUnscaledDeltaTime();
	}

	bool WaitForSecondsRealtime::CanResume()
	{
		return _waitTime < 0 ? true : false;
	}
}
