#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	enum class MainCameraState
	{
		FOLLOW_PLAYER
		, UI_SELECT
	};

	/**
	 * \brief Main Camera Controller. 3��Ī �׼� �� 
	 */
	class MainCameraController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MainCameraController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("MainCameraController"));

		virtual ~MainCameraController();

	private:
		MainCameraState _mainCameraState;

		/**
		 * \brief Third person camera movement information.
		 */
		DUOLGameEngine::Transform* _followTransform;

		/**
		 * \brief Third person camera view information.
		 */
		DUOLGameEngine::Transform* _viewTransform;

		float _followSpeed;

		float _sensitivity;

		float _clampAngle;


		// Third person mouse information.
		float _rotX;

		float _rotY;
	
		bool _isLockRotationByMouse;

		// Camera transform information
		DUOLGameEngine::Transform* _realCameraTransform;

		DUOLGameEngine::Transform* _cameraTransform;

		DUOLMath::Vector3 _dirNormalized;

		DUOLMath::Vector3 _finalDir;

		float _minDistance;

		float _maxDistance;

		float _finalDistance;

		float _smoothness;

	private:
		void UpdateRotationValue(float deltaTime);

		void OnFollowPlayerState(float deltaTime);

		void SetLockRotationByMouse(bool value);

	public:
		void SetFollowTransform(DUOLGameEngine::Transform* followTransform);

		// ���������� �ٶ� Ʈ�������� �޽��ϴ�.
		void SetViewTransform(DUOLGameEngine::Transform* viewTransform);

		/**
		 * \brief �ʱ� ������ �����մϴ�.
		 */
		virtual void OnStart() override;

		virtual void OnLateUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND

		friend class Player;

		friend class PlayerStateBase;
	};
}