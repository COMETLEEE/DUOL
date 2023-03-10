#include "DUOLClient/ECS/Component/PlayerController.h"

#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::PlayerController>("PlayerController")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLClient
{
	DUOLClient::PlayerController::PlayerController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _forcePerInputFrame(100.f)
		, _speed(0.f)
		, _maxSpeed(5.f)
	{

	}

	PlayerController::~PlayerController()
	{
	}

	void PlayerController::MoveUpdate(float deltaTime)
	{
		float horizontal = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Horizontal);

		float vertical = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Vertical);

		DUOLMath::Vector3 normalizeInputAxis;

		DUOLMath::Vector3(horizontal, 0.f, vertical).Normalize(normalizeInputAxis);

		_rigidbody->AddForce(normalizeInputAxis * _forcePerInputFrame);

		DUOLMath::Vector3 linearVelo = _rigidbody->GetLinearVelocity();

		// 스피드는 Y축으로의 속도를 제외한 길이이다.
		DUOLMath::Vector2 speedWithoutY = DUOLMath::Vector2(linearVelo.x, linearVelo.z);

		_speed = speedWithoutY.Length();

		// 움직였다면 방향을 전환해줍니다.
		if (_speed >= 0.1f)
		{
			DUOLMath::Vector3 velo = _rigidbody->GetLinearVelocity();

			velo.Normalize(velo);

			// y축은 없앤다 ..!
			velo.y = 0.f;

			DUOLMath::Vector3 lookWay = _transform->GetWorldPosition() + velo;

			// 에러 제거
			if (lookWay != _transform->GetWorldPosition())
				_transform->LookAt(lookWay);
		}

		// 최대 속력
		if (speedWithoutY.Length() > _maxSpeed)
		{
			DUOLMath::Vector3 normalOfVelocity;

			linearVelo.Normalize(normalOfVelocity);

			_rigidbody->SetLinearVelocity(normalOfVelocity * _maxSpeed);
		}

		_animator->SetFloat(TEXT("Speed"), _speed);
	}

	void PlayerController::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		_transform = reinterpret_cast<DUOLGameEngine::Transform*>(GetGameObject()->GetComponent(TEXT("Transform")));

		_animator = reinterpret_cast<DUOLGameEngine::Animator*>(GetGameObject()->GetComponent(TEXT("Animator")));

		_rigidbody = reinterpret_cast<DUOLGameEngine::Rigidbody*>(GetGameObject()->GetComponent(TEXT("Rigidbody")));

		_capsuleCollider = reinterpret_cast<DUOLGameEngine::CapsuleCollider*>(GetGameObject()->GetComponent(TEXT("CapsuleCollider")));
	}

	void PlayerController::OnStart()
	{
		MonoBehaviourBase::OnStart();

		if (_rigidbody != nullptr)
			_rigidbody->SetIsFreezeRotation(true);

		if(_capsuleCollider != nullptr)
			_rigidbody->SetCenterOfMass(_capsuleCollider->GetCenter());
	}

	void PlayerController::OnFixedUpdate(float fixedTimeStep)
	{
		MonoBehaviourBase::OnFixedUpdate(fixedTimeStep);

		MoveUpdate(fixedTimeStep);
	}
}