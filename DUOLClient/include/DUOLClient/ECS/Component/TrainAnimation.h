#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API TrainAnimation : public DUOLGameEngine::BehaviourBase
	{
	public:
		TrainAnimation(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("TrainAnimation"));

		virtual ~TrainAnimation() override;

	public:

		/**
	 * \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
	 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
	 */
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime);

		DUOLMath::Vector3 _startPos;

		DUOLMath::Vector3 _endPos;

		float _moveTime;

		float _generationTime;

		// �Ʒ��� private�� ����
	private:
		bool _isGeneratedTrain;

		float _currentGenerationTime;

		float _currentMovementTime;

		DUOLGameEngine::Transform* _trainTransform;

		DUOLGameEngine::Transform* _endlineTransform;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
}