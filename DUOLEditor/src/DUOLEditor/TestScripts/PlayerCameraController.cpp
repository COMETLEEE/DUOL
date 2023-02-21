#include "DUOLEditor/TestScripts/PlayerCameraController.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLEditor::PlayerCameraController>("PlayerCameraController")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr	
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLEditor
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

		// �÷��̾� ĳ���� �� Ʈ������ �ּ� ĳ��
		for (auto& gameObject : gameObjects)
			if (gameObject->GetName() == TEXT("PlayerCharacter"))
				_playerCharacter = gameObject;

		if (_playerCharacter != nullptr)
			_playerCharacterTransform = _playerCharacter->GetTransform();

		// �ش� ī�޶� ������Ʈ �� Ʈ������ �ּ� ĳ��
		_playerCamera = GetGameObject();

		_playerCameraTransform = _playerCamera->GetTransform();

		// ����Ʈ ������
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