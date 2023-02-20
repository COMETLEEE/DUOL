/**

    @file      PlayerCameraController.h
    @brief     
    @details   ~ 
    @author    COMETLEE
    @date      20.02.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Camera;
}

namespace DUOLEditor
{
	/**

	 @class   PlayerCameraController
	 @brief   DUOL 의 플레이어 촬영을 담당하는 클래스입니다.
	 @details 주로 플레이어의 움직임을 따라가면서 카메라를 움직입니다.

	**/
	class PlayerCameraController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		PlayerCameraController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("PlayerCameraController"));

		virtual ~PlayerCameraController();

	private:
		DUOLGameEngine::GameObject* _playerCharacter;

		DUOLGameEngine::Transform* _playerCharacterTransform;

		DUOLGameEngine::GameObject* _playerCamera;

		DUOLGameEngine::Transform* _playerCameraTransform;

		DUOLMath::Vector3 _defaultOffset;

	private:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnLateUpdate(float deltaTime) override;
	};
}
