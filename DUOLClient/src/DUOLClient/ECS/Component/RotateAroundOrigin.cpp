#include "DUOLClient/ECS/Component/RotateAroundOrigin.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

namespace DUOLClient
{
	RotateAroundOrigin::RotateAroundOrigin(DUOLGameEngine::GameObject* owner) :
		DUOLGameEngine::MonoBehaviourBase(owner, TEXT("RotateAroundOrigin"))
		, _myTransform(nullptr)
		, _rotateSpeed(30.f)
	{
		_myTransform = GetGameObject()->GetTransform();
	}

	RotateAroundOrigin::~RotateAroundOrigin()
	{
	}

	void RotateAroundOrigin::OnUpdate(float deltaTime)
	{
		_myTransform->RotateAround(DUOLMath::Vector3::Zero, DUOLMath::Vector3::Up, _rotateSpeed * deltaTime);
	}

	float RotateAroundOrigin::GetRotateSpeed() const
	{
		return _rotateSpeed;
	}

	void RotateAroundOrigin::SetRotateSpeed(float rotateSpeed)
	{
		_rotateSpeed = rotateSpeed;
	}
}