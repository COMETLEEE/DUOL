#include "DUOLEditor/TestScripts/MoveController.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLMath/DUOLMath.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Manager/EventManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLEditor::MoveController>("MoveController")
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("MoveSpeed", &DUOLEditor::MoveController::GetMoveSpeed, &DUOLEditor::MoveController::SetMoveSpeed)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("IsWorldMode",&DUOLEditor::MoveController::GetIsWorldMode, &DUOLEditor::MoveController::SetIsWorldMode)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLEditor
{
	MoveController::MoveController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _moveSpeed(3.f)
		, _isWorldMode(true)
	{
	}

	MoveController::~MoveController()
	{
	}

	float MoveController::GetMoveSpeed() const
	{
		return _moveSpeed;
	}

	void MoveController::SetMoveSpeed(float value)
	{
		_moveSpeed = value;
	}

	bool MoveController::GetIsWorldMode() const
	{
		return _isWorldMode;
	}

	void MoveController::SetIsWorldMode(bool value)
	{
		_isWorldMode = value;
	}

	void MoveController::OnLeftFoot()
	{
		DUOL_TRACE(DUOL_CONSOLE, "OnLeftFoot ..!");
	}

	void MoveController::OnRightFoot()
	{
		DUOL_TRACE(DUOL_CONSOLE, "OnRightFoot ..!");
	}

	void MoveController::OnAwake()
	{
		_animator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

		AddEventFunction(TEXT("OnLeftFoot"), std::bind(&MoveController::OnLeftFoot, this));

		AddEventFunction(TEXT("OnRightFoot"), std::bind(&MoveController::OnRightFoot, this));
	}

	void MoveController::OnUpdate(float deltaTime)
	{
		static bool isMove = false;

		DUOLGameEngine::Transform* transform = GetTransform();

		if (_isWorldMode)
		{
			// World Translation
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::UpArrow))
			{
				transform->Translate(DUOLMath::Vector3::Forward * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);

				isMove = true;

				_animator->SetBool(TEXT("TrueIsIdle"), false);
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::LeftArrow))
			{
				transform->Translate(DUOLMath::Vector3::Left * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);

				isMove = true;

				_animator->SetBool(TEXT("TrueIsIdle"), false);
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::DownArrow))
			{
				transform->Translate(DUOLMath::Vector3::Backward * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);

				isMove = true;

				_animator->SetBool(TEXT("TrueIsIdle"), false);
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::RightArrow))
			{
				transform->Translate(DUOLMath::Vector3::Right * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);
				isMove = true;

				_animator->SetBool(TEXT("TrueIsIdle"), false);
			}

			if (!isMove)
			{
				_animator->SetBool(TEXT("TrueIsIdle"), true);
			}

			isMove = false;

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::Z))
				transform->Translate(DUOLMath::Vector3::Down * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::C))
				transform->Translate(DUOLMath::Vector3::Up * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);
		}
		else if (!_isWorldMode)
		{
			// Local Translation
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::UpArrow))
				transform->Translate(DUOLMath::Vector3::Forward * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::LeftArrow))
				transform->Translate(DUOLMath::Vector3::Left * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::DownArrow))
				transform->Translate(DUOLMath::Vector3::Backward * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::RightArrow))
				transform->Translate(DUOLMath::Vector3::Right * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::Z))
				transform->Translate(DUOLMath::Vector3::Down * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::C))
				transform->Translate(DUOLMath::Vector3::Up * deltaTime * _moveSpeed);
		}

		// Rotation Test
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::X))
			transform->Rotate(DUOLMath::Vector3(0.f, 90.f, 0.f) * deltaTime);

		//// Animator Transition Test
		//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::V))
		//{
		//	_animator->SetBool(TEXT("TrueIsIdle"), true);
		//}
		//else if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::B))
		//{
		//	_animator->SetBool(TEXT("TrueIsIdle"), false);
		//}
	}
}
