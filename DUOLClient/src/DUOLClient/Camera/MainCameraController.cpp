#include "DUOLClient/Camera/MainCameraController.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::MainCameraController>("MainCameraController")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_followSpeed", &DUOLClient::MainCameraController::_followSpeed)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_sensitivity", &DUOLClient::MainCameraController::_sensitivity)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_clampAngle", &DUOLClient::MainCameraController::_clampAngle)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_minDistance", &DUOLClient::MainCameraController::_minDistance)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_maxDistance", &DUOLClient::MainCameraController::_maxDistance)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_smoothness", &DUOLClient::MainCameraController::_smoothness)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

namespace DUOLClient
{
	MainCameraController::MainCameraController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
		, _followSpeed(10.f)
		, _sensitivity(100.f)
		, _clampAngle(70.f)
		, _minDistance(2.f)
		, _maxDistance(5.f)
		, _smoothness(50.f)
	{

	}

	MainCameraController::~MainCameraController()
	{

	}

	void MainCameraController::UpdateRotationValue(float deltaTime)
	{
		const DUOLMath::Vector2& prevMousePosition = DUOLGameEngine::InputManager::GetInstance()->GetPrevMousePosition();

		const DUOLMath::Vector2& currMousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition();

		const DUOLMath::Vector2 deltaMouseMove = 0.15f * (prevMousePosition - currMousePosition);

		_rotX += -deltaMouseMove.y * _sensitivity * deltaTime;

		_rotY += -deltaMouseMove.x * _sensitivity * deltaTime;

		// Y 방향의 회전은 최소, 최대 제한을 둡니다.
		_rotX =	std::clamp(_rotX, -_clampAngle, _clampAngle);

		DUOLMath::Quaternion rot = DUOLMath::Quaternion::CreateFromEulerAngle(DUOLMath::MathHelper::DegreeToRadian(_rotX), DUOLMath::MathHelper::DegreeToRadian(_rotY), 0);

		_cameraTransform->SetRotation(rot, DUOLGameEngine::Space::World);
	}

	void MainCameraController::OnFollowPlayerState(float deltaTime)
	{
		const DUOLMath::Vector3& camPosition = _cameraTransform->GetWorldPosition();

		// _followTransform 을 따라가자.
		DUOLMath::Vector3 dirToFollow = (_followTransform->GetWorldPosition() - camPosition);

		float lengthToFollow = (dirToFollow).Length();

		_cameraTransform->SetPosition(camPosition + dirToFollow.Normalized() * std::min(lengthToFollow  * _followSpeed * deltaTime, lengthToFollow));

		const DUOLMath::Matrix& worldMat = _cameraTransform->GetWorldMatrix();

		// 최종 최대 방향 및 거리를 로컬에서 월드로 변경해줍니다.
		_finalDir = DUOLMath::Vector3::TransformNormal(_dirNormalized, worldMat);

		_finalDir *= _maxDistance;

		DUOLPhysics::RaycastHit hit;

		// 장애물이 막고 있으면 그 앞으로 땡겨줍니다.
		if (DUOLGameEngine::PhysicsManager::GetInstance()->Raycast(_realCameraTransform->GetWorldPosition(), _realCameraTransform->GetWorldPosition() + _finalDir, hit))
		{
			DUOLGameEngine::GameObject* hittedObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hit._userData);

			if ((hittedObject != nullptr) && (hittedObject->GetTag() == TEXT("Obstacle")))
				_finalDistance = std::clamp(hit._hitDistance, _minDistance, _maxDistance);
			else
				_finalDistance = _maxDistance;
		}
		else
		{
			_finalDistance = _maxDistance;
		}

		_realCameraTransform->SetLocalPosition(DUOLMath::Vector3::Lerp(_realCameraTransform->GetLocalPosition(), _dirNormalized * _finalDistance, std::clamp(_smoothness * deltaTime, 0.f, 1.f)));
	}

	void MainCameraController::OnStart()
	{
		MonoBehaviourBase::OnStart();

		_mainCameraState = MainCameraState::FOLLOW_PLAYER;

		// Follow (== Character)
		auto& allGOs = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : allGOs)
		{
			if (gameObject->GetTag() == TEXT("PlayerFollowRoot"))
				_followTransform = gameObject->GetTransform();
		}

		// Third Persom
		_rotX = GetTransform()->GetLocalEulerAngle().x;
		_rotY = GetTransform()->GetLocalEulerAngle().y;

		// Transform Information
		for (auto gameObject : allGOs)
		{
			if (gameObject->GetTag() == TEXT("MainCamera"))
				_realCameraTransform = gameObject->GetTransform();
		}

		_cameraTransform = GetTransform();
		_dirNormalized = _realCameraTransform->GetLocalPosition().Normalized();
		_finalDistance = _realCameraTransform->GetLocalPosition().Length();
	}

	void MainCameraController::OnLateUpdate(float deltaTime)
	{
		UpdateRotationValue(deltaTime);

		switch (_mainCameraState)
		{
			case MainCameraState::FOLLOW_PLAYER:
			{
				OnFollowPlayerState(deltaTime);

				break;
			}

			case MainCameraState::UI_SELECT:
			{

				break;
			}
		}
	}
}