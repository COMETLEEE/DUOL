#include "DUOLClient/Camera/MainCameraController.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

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
		, _followSpeed(8.f)
		, _sensitivity(20.f)
		, _clampAngle(30.f)
		, _minDistance(1.f)
		, _maxDistance(8.f)
		, _smoothness(10.f)
		, _isLockRotationByMouse(false)
		, _followTransform(nullptr)
		, _viewTransform(nullptr)
		, _cameraShakeTime(0.f)
		, _shakePower(DUOLMath::Vector2(0.f, 0.f))
		, _maxShakeDistance(30.0f)

	{
		_obstacleLayer = DUOLGameEngine::PhysicsManager::GetInstance()->GetLayerNumber(TEXT("Obstacle"));
	}

	MainCameraController::~MainCameraController()
	{

	}

	void MainCameraController::UpdateCameraShake(float deltaTime)
	{
		// Camera Shake time check.
		if (_cameraShakeTime <= 0.f)
			return;

		// ������ �� �� �� �߰�
		// TODO : ��� ���� ���� �ʴٰ� �����ؼ� ����.
		//{
		//	// _cameraTransform->Translate(DUOLMath::Vector3(0.f, -_shakePower.y, 0.f) * _cameraShakeTime * deltaTime * 10.f);

		//	_cameraTransform->Rotate(-_cameraTransform->GetRight(), -30.f * _shakePower.y * _cameraShakeTime * deltaTime * 3.f, DUOLGameEngine::Space::World);
		//}
		// �ð��� ���� �����ϵ���..!
		_shakePower = DUOLMath::Vector2::Lerp(DUOLMath::Vector2::Zero, _maxShakePower, _cameraShakeTime / _maxShakeTime);
		// Shake !
		float xShake = DUOLMath::MathHelper::RandF(-_shakePower.x * deltaTime, _shakePower.x * deltaTime);

		float yShake = DUOLMath::MathHelper::RandF(-_shakePower.y * deltaTime, _shakePower.y * deltaTime);

		// float zShake = DUOLMath::MathHelper::RandF(-_shakePower.y * deltaTime, _shakePower.y * deltaTime);

		_cameraTransform->Translate(DUOLMath::Vector3(xShake, yShake, 0.f), DUOLGameEngine::Space::Self);

		_cameraShakeTime -= deltaTime;
	}

	void MainCameraController::UpdateRotationValue(float deltaTime)
	{
		if (_isLockRotationByMouse)
			return;

		const DUOLMath::Quaternion& prevCameraRotation = _cameraTransform->GetWorldRotation();

		const DUOLMath::Vector2& prevMousePosition = DUOLGameEngine::InputManager::GetInstance()->GetPrevMousePosition();

		const DUOLMath::Vector2& currMousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition();

		const DUOLMath::Vector2 deltaMouseMove = 0.15f * (prevMousePosition - currMousePosition);

		_rotX += -deltaMouseMove.y * _sensitivity * deltaTime;

		_rotY += -deltaMouseMove.x * _sensitivity * deltaTime;

		// Y ������ ȸ���� �ּ�, �ִ� ������ �Ӵϴ�.
		_rotX = std::clamp(_rotX, -_clampAngle, _clampAngle);

		DUOLMath::Quaternion rot = DUOLMath::Quaternion::CreateFromEulerAngle(DUOLMath::MathHelper::DegreeToRadian(_rotX), DUOLMath::MathHelper::DegreeToRadian(_rotY), 0);

		//_cameraTransform->SetRotation(rot, DUOLGameEngine::Space::World);
		_cameraTransform->SetRotation(DUOLMath::Quaternion::Slerp(prevCameraRotation, rot, deltaTime * _smoothness), DUOLGameEngine::Space::World);
	}

	void MainCameraController::OnFollowPlayerState(float deltaTime)
	{
		const DUOLMath::Vector3& camPosition = _cameraTransform->GetWorldPosition();

		// _followTransform �� ������.
		DUOLMath::Vector3 dirToFollow = (_followTransform->GetWorldPosition() - camPosition);

		float lengthToFollow = (dirToFollow).Length();

		_cameraTransform->SetPosition(camPosition + dirToFollow.Normalized() * std::min(lengthToFollow * _followSpeed * deltaTime, lengthToFollow));

		// View Transform �� �����Ǿ� ������ �ٶ󺸴� ���� ���� �����̴� ..!
		if (_viewTransform != nullptr)
		{
			// _cameraTransform->LookAt(_viewTransform);

			const DUOLMath::Quaternion& currentRot = _cameraTransform->GetWorldRotation();

			// ȸ���� �����ֱ� ���ؼ� ȸ�� ���� ������.
			DUOLMath::Quaternion lookAtQuat =
				DUOLMath::Quaternion::CreateFromRotationMatrix(DUOLMath::Matrix::CreateLookAt(_cameraTransform->GetWorldPosition(), _viewTransform->GetWorldPosition(), DUOLMath::Vector3::Up));

			lookAtQuat.Inverse(lookAtQuat);

			_cameraTransform->SetRotation(DUOLMath::Quaternion::Slerp(currentRot, lookAtQuat, deltaTime * _smoothness), DUOLGameEngine::Space::World);
		}

		const DUOLMath::Matrix& worldMat = _cameraTransform->GetWorldMatrix();

		// ���� �ִ� ���� �� �Ÿ��� ���ÿ��� ����� �������ݴϴ�.
		_finalDir = DUOLMath::Vector3::TransformNormal(_dirNormalized, worldMat);

		_finalDir *= _maxDistance;

		DUOLPhysics::RaycastHit hit;

		// ��ֹ��� ���� ������ �� ������ �����ݴϴ�.
		if (DUOLGameEngine::PhysicsManager::GetInstance()->Raycast(_realCameraTransform->GetWorldPosition(), _finalDir.Normalized(), _finalDir.Length(), _obstacleLayer, hit))
		{
			DUOLGameEngine::GameObject* hittedObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hit._userData);

			if ((hittedObject != nullptr) /*&& (hittedObject->GetTag() == TEXT("Obstacle"))*/)
				_finalDistance = std::clamp(hit._hitDistance, _minDistance, _maxDistance);
		}
		else
		{
			_finalDistance = _maxDistance;
		}

		_realCameraTransform->SetLocalPosition(DUOLMath::Vector3::Lerp(_realCameraTransform->GetLocalPosition(), _dirNormalized * _finalDistance, std::clamp(_smoothness * deltaTime, 0.f, 1.f)));
	}

	void MainCameraController::SetLockRotationByMouse(bool value)
	{
		_isLockRotationByMouse = value;
	}

	void MainCameraController::SetCameraShake(float shakeTime, const DUOLMath::Vector2& shakePower, DUOLGameEngine::Transform* startTr)
	{
		_cameraShakeTime = shakeTime;

		_maxShakeTime = _cameraShakeTime;

		float distance = 0;

		if (startTr)
			distance = DUOLMath::Vector3::Distance(startTr->GetWorldPosition(), _cameraTransform->GetWorldPosition());

		float distanceOffset = 1 - std::min(1.0f, distance / _maxShakeDistance);

		_shakePower = shakePower * distanceOffset;

		_maxShakePower = _shakePower;
	}

	void MainCameraController::SetFollowTransform(DUOLGameEngine::Transform* followTransform)
	{
		_followTransform = followTransform;
	}

	void MainCameraController::SetViewTransform(DUOLGameEngine::Transform* viewTransform)
	{
		_viewTransform = viewTransform;

		// �� Ʈ�������� ������ ���콺 ȸ���� �ƴ� ������ �̿��� ī�޶��� ȸ���� �����Ѵ�.
		_viewTransform != nullptr ? SetLockRotationByMouse(true) : SetLockRotationByMouse(false);
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
			{
				_followTransform = gameObject->GetTransform();
			}
		}

		// Third Person
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
		// ���콺 �Է¿� ���� ī�޶� ȸ����
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

		case MainCameraState::CAMERA_SEQUENCE:
		{

			break;
		}
		}

		// ī�޶� ����ũ�� ���� �������� ������Ʈ����.
		UpdateCameraShake(deltaTime);
	}

	void MainCameraController::OnFixedUpdate(float deltaTime)
	{
		//// ���콺 �Է¿� ���� ī�޶� ȸ����
		//UpdateRotationValue(deltaTime);

		//switch (_mainCameraState)
		//{
		//	case MainCameraState::FOLLOW_PLAYER:
		//	{
		//		OnFollowPlayerState(deltaTime);

		//		break;
		//	}

		//	case MainCameraState::UI_SELECT:
		//	{

		//		break;
		//	}

		//	case MainCameraState::CAMERA_SEQUENCE:
		//	{

		//		break;
		//	}
		//}

		//// ī�޶� ����ũ�� ���� �������� ������Ʈ����.
		//UpdateCameraShake(deltaTime);
	}
}