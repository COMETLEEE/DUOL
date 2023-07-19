#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
/**
 * \brief ������
 */

namespace DUOLGameEngine
{
	class Animator;
}

namespace DUOLClient
{
	class Player;

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
		 * \brief ��ü�� ó�������� ��������Ʈ
		 */
		DUOLMath::Vector3 _startPoint;

		DUOLMath::Vector3 _distanceVector;

		float _currentHorizontalSpeed;

		float _currentVerticalSpeed;

		float _currentGravity;

		float _deltaT;

		float _currentCoolTime;

		float _currentStepOnTime;

		DUOLClient::Player* _usingPlayer;

		DUOLGameEngine::Animator* _jumpBoardAnimator;

		bool _isUsingJumpingBoard;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
}