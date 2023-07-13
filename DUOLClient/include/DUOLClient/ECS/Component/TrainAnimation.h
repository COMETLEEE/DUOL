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
	 * \brief 씬이 시작할 때 OnStart 보다 이전에 호출됩니다.
	 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
	 */
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime);

		DUOLMath::Vector3 _startPos;

		DUOLMath::Vector3 _endPos;

		float _moveTime;

		float _generationTime;

		// 아래는 private한 숫자
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