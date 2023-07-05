#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class SphereCollider;

	class Rigidbody;
}

namespace DUOLClient
{
	class Enemy;
	/**
	 * \brief ���� ������ �����ϱ� ���� ����. ��, �޾ƿ��� �𵨸��� �����ϰ� ���õǾ� ����� ������ ���� ����.
	 */
	class DUOL_CLIENT_API BossEnemy_Weapon_AreaWave : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		BossEnemy_Weapon_AreaWave(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Weapon_Sword"));

		virtual ~BossEnemy_Weapon_AreaWave() override;

	private:
		DUOLClient::Enemy* _owner;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::SphereCollider* _sphereCollider;

		DUOLMath::Vector3 _startPos;

		float _startRadius;

		float _endRadius;

		float _timer;

		float _waveTime;

		std::set<DUOLGameEngine::GameObject*> _enterCheck;
	public:
		void SetOwner(DUOLClient::Enemy* owner);

		void StartWave(const DUOLMath::Vector3& startPosCenter,
			const float& startRadius,
			const float& endRadius,
			float waveTime);

		void EndWave();

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

	};
}

