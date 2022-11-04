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

		static float _delayTime; // ���� �ӵ� ���� �ð�.

		static float _gameSpeed; // ���� �ӵ�
		//�ð��� ����?
		UINT64 m_Frequency;

		//���ݵ��� ī��Ʈ Ƚ��
		static UINT64 m_PrevCount;

		// ���� �����ӿ��� ���� �����ӱ��� ����� �ð�
		static float m_DeltaTime;

		// �ʴ� �� ���� ������Ʈ�� �̷�������� üũ�ϱ� ���� ����
		UINT32 m_FrameCount;

		float m_FrameTime;

		// ��� �ʴ� ������ ��
		static float m_FPS;

		static std::random_device rd;

		static std::mt19937 gen;

		static std::list<std::tuple<std::function<void(std::shared_ptr<GameObject>)>, float, std::shared_ptr<GameObject>>> _invokeList;

	public:
		bool _isUpdateStart = false; // ������Ʈ�� ó���� �� ���� �ð��� ������ �� ������ üũ.

		static void Start();

		void Update();

		static void SetGameSpeed(float speed, float delayTime);

		static float GetFPS();

		static float GetGameDeltaTime() { return GetDeltaTime() * _gameSpeed; }

		static float GetGameSpeed() { return _gameSpeed; }

		static float GetDeltaTime() { return m_DeltaTime > 1.0f ? 1.0f : m_DeltaTime; }

		static void Invoke(std::function<void(std::shared_ptr<GameObject>)> func, float time, std::shared_ptr<GameObject> target); // ������ ���� ������ �Լ�? ���� �ð��ڿ� �Լ��� �����Ѵ�.

		static void ClearInvokeList(); // �� ����� ��� �Լ��� ������Ų��.


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