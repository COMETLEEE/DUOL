#include "DUOLClient/ECS/Component/PlayerCameraController.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::PlayerCameraController>("PlayerCameraController")
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
	PlayerCameraController::PlayerCameraController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _playerCharacter(nullptr)
		, _defaultOffset(DUOLMath::Vector3::Zero)
	{
	}

	PlayerCameraController::~PlayerCameraController()
	{
	}

	void PlayerCameraController::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		auto& gameObjects =	DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		// 플레이어 캐릭터 및 트랜스폼 주소 캐싱
		for (auto& gameObject : gameObjects)
			if (gameObject->GetName() == TEXT("PlayerCharacter"))
				_playerCharacter = gameObject;

		if (_playerCharacter != nullptr)
			_playerCharacterTransform = _playerCharacter->GetTransform();

		// 해당 카메라 오브젝트 및 트랜스폼 주소 캐싱
		_playerCamera = GetGameObject();

		_playerCameraTransform = _playerCamera->GetTransform();

		// 디폴트 오프셋
		_defaultOffset = DUOLMath::Vector3(0.f, 10.f, -10.f);
	}

	void PlayerCameraController::OnStart()
	{
		MonoBehaviourBase::OnStart();
	}

	void PlayerCameraController::OnLateUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnLateUpdate(deltaTime);

		_playerCameraTransform->SetPosition(_playerCharacterTransform->GetWorldPosition() + _defaultOffset, DUOLGameEngine::Space::World);

		_playerCameraTransform->LookAt(_playerCharacterTransform->GetWorldPosition());
	}
}