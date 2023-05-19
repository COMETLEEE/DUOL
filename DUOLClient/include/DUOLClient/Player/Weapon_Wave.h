/**

    @file      Weapon_Wave.h
    @brief     웨이브 계열 공격에서 날리는 오브젝트
    @details   ~
    @author    COMETLEE
    @date      13.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
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
	class Weapon_Wave : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
        Weapon_Wave(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Weapon_Wave"));

        virtual ~Weapon_Wave() override;

	private:
        DUOLClient::Player* _player;

        DUOLGameEngine::Rigidbody* _rigidbody;

        DUOLGameEngine::BoxCollider* _boxCollider;

        DUOLMath::Vector3 _velocity;

        DUOLMath::Vector3 _deltaHalfExtentsPerSecond;

        float _waveTime;

	public:
        void StartWave(const DUOLMath::Vector3& startPosCenter, const DUOLMath::Vector3& startHalfExtents, const DUOLMath::Vector3& deltaHalfExtentsPerSecond, 
            const DUOLMath::Vector3& velocity, const DUOLMath::Quaternion& boxWorldRotation, float waveTime);

        void EndWave();

	public:
        virtual void OnAwake() override;

        virtual void OnStart() override;

        virtual void OnFixedUpdate(float fixedTimeStep) override;

        virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

#pragma region FRIEND_CLASS
        friend class Player;
#pragma endregion
	};
}