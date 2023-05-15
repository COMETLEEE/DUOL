#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLCommon/Log/LogHelper.h"

namespace DUOLGameEngine
{
	TimeManager::TimeManager() :
		_prevTimePoint(std::chrono::high_resolution_clock::now())
		, _currTimePoint(std::chrono::high_resolution_clock::now())
		, _frameCount(0)
		, _fps(0)
		, _realtimeSinceStartup(0.f)
		, _deltaTime(0.016f)
		, _unscaledDeltaTime(0.016f)
		, _timeScale(1.f)
		, _realtimeScaledSinceStartup(0)
	{

	}

	TimeManager::~TimeManager()
	{
		int _temp = 5;
	}

	void TimeManager::Initialize()
	{
		DUOL_INFO(DUOL_FILE, "TimeManager Initialize Success !");
	}

	void TimeManager::UnInitialize()
	{

	}

	void TimeManager::Update()
	{
		_currTimePoint = std::chrono::high_resolution_clock::now();

		UpdateFPS();

		UpdateDeltaTime();

		// 다음 프레임의 델타 타임 계산을 위한 준비를 합니다.
		_prevTimePoint = _currTimePoint;
	}

	void TimeManager::UpdateDeltaTime()
	{
		// TimeScale이 0이라면 시간은 흐르지 않는 것으로 체크한다.
		if (_timeScale < DUOLGameEngine::ENGINE_EPSILON)
		{
			_deltaTime = 0.f;
		}
		else
		{
			_unscaledDeltaTime = std::chrono::duration<float>(_currTimePoint - _prevTimePoint).count();

			_realtimeSinceStartup += _unscaledDeltaTime;

			_deltaTime = _unscaledDeltaTime * _timeScale;

			_realtimeScaledSinceStartup += _deltaTime;
		}
	}

	void TimeManager::UpdateFPS()
	{
		_frameCount += 1;

		// _unscaledDeltaTime => 프레임 전환에 걸린 시간
		// 따라서, 1초를 이 수로 나누면 현 프레임 기준 FPS 나옴
		_fps = static_cast<uint32_t>(1.f / _unscaledDeltaTime);
	}

	void TimeManager::SetTimeScale(float scale)
	{
		// Scale이 음수이면 잘못된 동작이다.
		assert(scale < 0.f);

		_timeScale = scale;
	}
}
