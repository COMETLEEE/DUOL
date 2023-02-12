#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Animator;
}

namespace DUOLEditor
{
	class MoveController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MoveController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("MoveController"));

		virtual ~MoveController() override;

	private:
		DUOLGameEngine::Animator* _animator;

		float _moveSpeed;

		bool _isWorldMode;

	public:
		float GetMoveSpeed() const;

		void SetMoveSpeed(float value);

		bool GetIsWorldMode() const;

		void SetIsWorldMode(bool value);

	public:
		// 이벤트 테스트용입니다.
		void OnLeftFoot();

		// 이벤트 테스트용입니다.
		void OnRightFoot();

		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		// 다음과 같이 MonoBehaviourBase로의 클래스 계층도가 Reflection 될 수 있도록 지정해줍니다.
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}