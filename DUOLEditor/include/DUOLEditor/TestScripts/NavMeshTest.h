#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class NavMeshAgent;
	class Animator;
	class Rigidbody;
	class CapsuleCollider;
}

namespace DUOLEditor
{
	class NavMeshTest : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		NavMeshTest(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("NavMeshTest"));

		virtual ~NavMeshTest();

	private:
		float _forcePerInputFrame;

		float _speed;

		float _maxSpeed;

		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::CapsuleCollider* _capsuleCollider;

		DUOLGameEngine::NavMeshAgent* _navMeshAgent;

		DUOLGameEngine::GameObject* _playerGameObject;

		DUOLMath::Vector3 _prevPlayerPos;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
}
