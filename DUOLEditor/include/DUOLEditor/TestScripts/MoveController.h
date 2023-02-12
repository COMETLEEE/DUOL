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
		// �̺�Ʈ �׽�Ʈ���Դϴ�.
		void OnLeftFoot();

		// �̺�Ʈ �׽�Ʈ���Դϴ�.
		void OnRightFoot();

		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		// ������ ���� MonoBehaviourBase���� Ŭ���� �������� Reflection �� �� �ֵ��� �������ݴϴ�.
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}