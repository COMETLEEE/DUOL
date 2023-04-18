/**

	@file      EnemyAirborneCheck.h
	@brief     몬스터가 공중에 있는지 확인하기 위한 클래스.
	@details   ~ 발에 충돌 콜라이더를 배치해 충돌중이라면 땅 충돌이 없다면 공중으로 판별한다.
	@author    SinSeongHyeon
	@date      18.04.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Rigidbody;
	class BoxCollider;
}

namespace DUOLClient
{
	class DUOL_CLIENT_API EnemyAirborneCheck : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		EnemyAirborneCheck(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("EnemyAirborneCheck"));

		virtual ~EnemyAirborneCheck() = default;

	private:
		std::vector<DUOLGameEngine::GameObject*> _grounds;

		DUOLGameEngine::BoxCollider* _boxCollider;

	public:
		bool GetIsAirborne();

		void Initialize(float boxSize, float boxHeight);

	public:
		virtual void OnDisable() override;

		virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		virtual void OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}

