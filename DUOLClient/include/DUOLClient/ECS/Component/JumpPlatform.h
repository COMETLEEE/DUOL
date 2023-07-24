#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
/**
 * \brief 점프대
 */

namespace DUOLGameEngine
{
	class AudioSource;
	class AudioClip;
	class Animator;
}

namespace DUOLClient
{
	class Player;

	constexpr float JUMP_ANIM_TIME = 2.0f;

	class DUOL_CLIENT_API JumpPlatform : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		JumpPlatform(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("JumpingBoard"));

		virtual ~JumpPlatform() override;

		void CalculateDistance(float deltaT);

		void SetTarget(DUOLGameEngine::GameObject* object);

		void OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		void OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		void OnUpdate(float deltaTime) override;

		void OnAwake() override;

	private:
		float _maxHeight;

		DUOLMath::Vector3 _destinationPoint;

		float _moveTime;

		float _cooltime;

		float _stepOnTime;

		/**
		 * \brief 물체가 처음탔을때 시작포인트
		 */
		DUOLMath::Vector3 _startPoint;

		DUOLMath::Vector3 _distanceVector;

		float _currentHorizontalSpeed;

		float _currentVerticalSpeed;

		float _currentGravity;

		float _deltaT;

		float _currentCoolTime;

		float _currentStepOnTime;

		float _jumpAnimCount;

		DUOLClient::Player* _usingPlayer;

		DUOLGameEngine::Animator* _jumpBoardAnimator;

		DUOLGameEngine::AudioClip* _jumpBoardSoundClip;

		DUOLGameEngine::AudioSource* _jumpBoardSound;


		bool _isUsingJumpingBoard;
		bool _isJumpboardSoundPlay;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
		RTTR_REGISTRATION_FRIEND
	};
}
