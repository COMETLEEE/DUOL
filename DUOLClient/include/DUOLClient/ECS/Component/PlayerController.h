/**

    @file      PlayerController.h
    @brief     Core player control.
    @details   Control player character in DUOL game.
    @author    COMETLEE
    @date      20.02.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class CapsuleCollider;
	class Rigidbody;

	class Animator;
}

namespace DUOLClient
{
	 /**
	     @class   PlayerController
	     @brief   DUOL 의 플레이어 조작을 담당하는 클래스입니다.
	     @details 주로 플레이어에 등록된 모듈들의 조건을 조정하여 컨트롤하는 중추 역할을 수행합니다.

	 **/
	class DUOL_CLIENT_API PlayerController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		PlayerController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("PlayerController"));

		virtual ~PlayerController();

	private:
		float _forcePerInputFrame;

		float _speed;

		float _maxSpeed;

		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::CapsuleCollider* _capsuleCollider;

	private:
		void MoveUpdate(float deltaTime);

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnFixedUpdate(float fixedTimeStep) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
}