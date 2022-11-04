#pragma once
#include <random>

namespace Muscle
{
	class GameObject;

	class CTime
	{
	public:
		CTime();
		virtual ~CTime();

	private:

		static float _delayTime; // 게임 속도 복귀 시간.

		static float _gameSpeed; // 게임 속도
		//시간의 단위?
		UINT64 m_Frequency;

		//간격동안 카운트 횟수
		static UINT64 m_PrevCount;

		// 이전 프레임에서 현재 프레임까지 경과된 시간
		static float m_DeltaTime;

		// 초당 몇 번의 업데이트가 이루어지는지 체크하기 위한 도구
		UINT32 m_FrameCount;

		float m_FrameTime;

		// 평균 초당 프레임 수
		static float m_FPS;

		static std::random_device rd;

		static std::mt19937 gen;

		static std::list<std::tuple<std::function<void(std::shared_ptr<GameObject>)>, float, std::shared_ptr<GameObject>>> _invokeList;

	public:
		bool _isUpdateStart = false; // 업데이트를 처음할 때 이전 시간과 오차가 좀 있으니 체크.

		static void Start();

		void Update();

		static void SetGameSpeed(float speed, float delayTime);

		static float GetFPS();

		static float GetGameDeltaTime() { return GetDeltaTime() * _gameSpeed; }

		static float GetGameSpeed() { return _gameSpeed; }

		static float GetDeltaTime() { return m_DeltaTime > 1.0f ? 1.0f : m_DeltaTime; }

		static void Invoke(std::function<void(std::shared_ptr<GameObject>)> func, float time, std::shared_ptr<GameObject> target); // 스레드 같은 느낌의 함수? 일정 시간뒤에 함수를 실행한다.

		static void ClearInvokeList(); // 씬 변경시 모든 함수를 삭제시킨다.


		template<class T>
		static T GetRand(T start, T end);

		template<class T>
		static T GetFloatRand(T start, T end);
	};
	template<class T>
	inline T CTime::GetRand(T start, T end)
	{
		if (start >= end)
			return 0;
		std::uniform_int_distribution<T> dis(start, end);
		return dis(gen);
	}
	template<class T>
	inline T CTime::GetFloatRand(T start, T end)
	{
		std::uniform_real_distribution<T> dis(start, end);
		return dis(gen);
	}

}