#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class Player;

	enum class MainCameraState
	{
		// 기본 상태
		FOLLOW_PLAYER

		// UI 선택 상태
		, UI_SELECT

		// 카메라 시퀀스 재생 상태
		, CAMERA_SEQUENCE
	};

	/**
	 * \brief Main Camera Controller. 3인칭 액션 뷰
	 */
	class MainCameraController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MainCameraController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("MainCameraController"));

		virtual ~MainCameraController();

	private:
		MainCameraState _mainCameraState;

		MainCameraState _preMainCameraState;

		int _obstacleLayer;

		/**
		 * \brief Third person camera movement target.
		 */
		Player* _player;

		DUOLGameEngine::Transform* _followTransform;

		DUOLGameEngine::Transform* _playerTransform;

		/**
		 * \brief Third person camera view target.
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

		// Camera actions.
		float _cameraShakeTime;

		float _maxShakeTime;

		DUOLMath::Vector2 _shakePower;

		DUOLMath::Vector2 _maxShakePower;

		float _maxShakeDistance;
	private:
		void UpdateCameraShake(float deltaTime);

		void UpdateRotationValue(float deltaTime);

		void OnFollowPlayerState(float deltaTime);

		void SetLockRotationByMouse(bool value);

	public:
		// 카메라 쉐이킹 예약함수.
		void SetCameraShake(float shakeTime, const DUOLMath::Vector2& shakePower, DUOLGameEngine::Transform* startTr = nullptr);

		// 따라갈 트랜스폼을 받습니다.
		void SetFollowTransform(DUOLGameEngine::Transform* followTransform);

		// 바라볼 트랜스폼을 받습니다.
		void SetViewTransform(DUOLGameEngine::Transform* viewTransform);

		void SetCameraState(MainCameraState state);

		void SetCameraInitPos();

		inline MainCameraState GetCameraState() { return _mainCameraState; }
		/**
		 * \brief 초기 설정을 진행합니다.
		 */
		virtual void OnStart() override;

		virtual void OnLateUpdate(float deltaTime) override;

		/**
		 * \brief 일단 흔들림의 방지를 위해 OnFixedUpdate 에서 위치 동기 맞춰주자.
		 * \param deltaTime
		 */
		virtual void OnFixedUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class Player;

		friend class PlayerStateBase;
#pragma endregion
	};
}