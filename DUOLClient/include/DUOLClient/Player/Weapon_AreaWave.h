#pragma once
#include "Player.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Rigidbody;
	class BoxCollider;
}

namespace DUOLClient
{
	class Player;
}

namespace DUOLClient
{
	/**
	 * \brief 오버드라이브 소드 마지막 타격 공격을 구현합니다.
	 */
	class Weapon_AreaWave : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Weapon_AreaWave(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Weapon_AreaWave"));

		virtual ~Weapon_AreaWave() override;

	private:
		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::BoxCollider* _boxCollider;

		DUOLClient::Player* _player;

		DUOLMath::Vector3 _deltaSizePerSecond;

		DUOLMath::Vector3 _startSize;

		PlayerSoundTable _currentAudioClip;

		int _mobHitSoundCount;

	public:
		void StartAreaWave(const DUOLMath::Vector3& startPosCenter, const DUOLMath::Quaternion& boxWorldRotation, float duration);

		void StartAreaWave(const DUOLMath::Vector3& startPosCenter, const DUOLMath::Quaternion& boxWorldRotation, float duration, PlayerSoundTable soundClip);

		void EndAreaWave();

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

#pragma region FRIEND_CLASS
		friend class Player;
#pragma endregion

		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}