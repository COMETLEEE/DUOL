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
		//���� �ð��� ����
		::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_Frequency));
		//�ð� ������ ī��Ʈ
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_PrevCount));

		// �ð� Ŭ�������� �����ϴ� ����...
		// �ð��� ���������� ���Ƽ�?


		static std::mt19937 gen;
	}

	CTime::~CTime()
	{
		// ���� �� ����.
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

		// ���� _currentCount���� �� m_PrevCount�� �� ����
		// m_Frequency�� �������ָ� ����� �ð�(��)�� ���´�.
		m_DeltaTime = (_currentCount - m_PrevCount) / static_cast<float>(m_Frequency);
		m_PrevCount = _currentCount; // preCount�� currentCount�� ����

		m_FrameCount++;				// Update���� 1�ʿ� �� �� ����Ǵ��� üũ�ϱ� ���ؼ� ���� !
		m_FrameTime += m_DeltaTime; // 1 �����ӿ� ��� �� �ʰ� �ɸ����� üũ�ϱ� ���ؼ� ���� !

		if (m_FrameTime > 1.f)		// 1�ʸ� �Ѿ�� ���� (�츰 �ʴ� �� ���� ������Ʈ == �������� ����Ǿ����� �ñ��ϴϱ� ..)
		{
			m_FPS = (float)m_FrameCount / m_FrameTime;  // 1�� �� Update���� �� �� ����Ǿ����� ���

			m_FrameTime = 0.f;		// 0���� �ʱ�ȭ
			m_FrameCount = 0;		// 0���� �ʱ�ȭ
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