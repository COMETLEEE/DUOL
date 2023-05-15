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

		// ���� �������� ��Ÿ Ÿ�� ����� ���� �غ� �մϴ�.
		_prevTimePoint = _currTimePoint;
	}

	void TimeManager::UpdateDeltaTime()
	{
		// TimeScale�� 0�̶�� �ð��� �帣�� �ʴ� ������ üũ�Ѵ�.
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

		// _unscaledDeltaTime => ������ ��ȯ�� �ɸ� �ð�
		// ����, 1�ʸ� �� ���� ������ �� ������ ���� FPS ����
		_fps = static_cast<uint32_t>(1.f / _unscaledDeltaTime);
	}

	void TimeManager::SetTimeScale(float scale)
	{
		// Scale�� �����̸� �߸��� �����̴�.
		assert(scale < 0.f);

		_timeScale = scale;
	}
}
