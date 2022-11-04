#include "pch.h"
#include "CTime.h"
#include "GameObject.h"
#include <memory.h>
#include <tuple>
namespace Muscle
{
	std::random_device CTime::rd;

	std::mt19937 CTime::gen(rd());

	float CTime::m_FPS = 0;

	float CTime::m_DeltaTime = 0;

	float CTime::_delayTime = 0;

	float CTime::_gameSpeed = 1;

	std::list<std::tuple<std::function<void(std::shared_ptr<GameObject>)>, float, std::shared_ptr<GameObject>>> CTime::_invokeList;
	//std::list<std::tuple<std::function<std::shared_ptr<GameObject>>, float, std::shared_ptr<GameObject>>>();

	UINT64 CTime::m_PrevCount = NULL;
	CTime::CTime() :
		m_Frequency(NULL),
		m_FrameCount(NULL),
		m_FrameTime(NULL)
	{
		//기준 시간을 저장
		::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_Frequency));
		//시간 간격의 카운트
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_PrevCount));

		// 시간 클래스에서 관리하는 이유...
		// 시간과 관련있을것 같아서?


		static std::mt19937 gen;
	}

	CTime::~CTime()
	{
		// 딱히 할 없다.
	}

	void CTime::Start()
	{
		ClearInvokeList();
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_PrevCount));
		m_DeltaTime = 0;
	}

	void CTime::Update()
	{

		UINT64 _currentCount;

		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currentCount));

		// 현재 _currentCount에서 전 m_PrevCount를 뺀 값을
		// m_Frequency로 나누어주면 경과된 시간(초)이 나온다.
		m_DeltaTime = (_currentCount - m_PrevCount) / static_cast<float>(m_Frequency);
		m_PrevCount = _currentCount; // preCount에 currentCount를 저장

		m_FrameCount++;				// Update문이 1초에 몇 번 실행되는지 체크하기 위해서 누적 !
		m_FrameTime += m_DeltaTime; // 1 프레임에 평균 몇 초가 걸리는지 체크하기 위해서 누적 !

		if (m_FrameTime > 1.f)		// 1초를 넘어가는 순간 (우린 초당 몇 번의 업데이트 == 프레임이 진행되었는지 궁금하니까 ..)
		{
			m_FPS = (float)m_FrameCount / m_FrameTime;  // 1초 간 Update문이 몇 번 실행되었는지 계산

			m_FrameTime = 0.f;		// 0으로 초기화
			m_FrameCount = 0;		// 0으로 초기화
		}

		if (_delayTime < 0)
			_gameSpeed = 1;
		else
			_delayTime -= m_DeltaTime;

		auto iter = _invokeList.begin();

		while (iter != _invokeList.end())
		{
			if (std::get<1>(*iter) > 0)
			{
				std::get<1>(*iter) -= GetGameDeltaTime();
				iter++;
			}
			else
			{
				std::get<0>(*iter)(std::get<2>(*iter));
				iter = _invokeList.erase(iter);
				if (iter == _invokeList.end())
					break;
			}
		}
	}

	void CTime::SetGameSpeed(float speed, float delayTime)
	{
		_gameSpeed = speed;

		_delayTime = delayTime;
	}

	float CTime::GetFPS()
	{
		return m_FPS;
	}

	void CTime::Invoke(std::function<void(std::shared_ptr<GameObject>)> func, float time, std::shared_ptr<GameObject> target)
	{
		_invokeList.push_back({ func ,time,target });
	}

	void CTime::ClearInvokeList()
	{
		for (auto& iter : _invokeList)
		{
			std::get<2>(iter).reset();
		}
		_invokeList.clear();
	}

}