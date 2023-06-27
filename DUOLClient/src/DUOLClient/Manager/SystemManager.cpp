#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/Manager/InputManager.h"


DUOLClient::SystemManager::SystemManager() :
	_isBStage(false)
	, _isBStageAllMonsterKill(false)
	, _isDoorMonsterKill(false)
	, _isOpenDoor(false)
	, _currentTime(0.0f)
	, _rimLightObject(nullptr)
	, _doorObject(nullptr)
	, _moveDoor(0.0f)
{
}

DUOLClient::SystemManager::~SystemManager()
{
}

void DUOLClient::SystemManager::OnAwake()
{

}

void DUOLClient::SystemManager::OnStart()
{
	auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

	for (auto gameObject : gameObjects)
	{
		if (gameObject->GetTag() == TEXT("RimObject"))
		{
			_rimLightObject = gameObject;
		}
		if (gameObject->GetTag() == TEXT("OpenDoor"))
		{
			_doorObject = gameObject;
		}
	}
}

void DUOLClient::SystemManager::OnUpdate(float deltaTime)
{
	//if(_isBStage)
	//{
	//	if (_isBStageAllMonsterKill)
	//		BSystem(deltaTime);
	//}

	BSystem(deltaTime);

}

void DUOLClient::SystemManager::DoorCollisionStay()
{
	//if (!_isBStageAllMonsterKill)
	//	return;

	//if (!_isDoorMonsterKill)
	//	return;

	//// UI ¶°¾ßÇÔ 

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F))
	{
		_isOpenDoor = true;
	}
}

void DUOLClient::SystemManager::BSystem(float deltaTime)
{
	// Door Monster Trigger active

	// RimLight 
	if (_isDoorMonsterKill)
		RimLightFromDeltaTime(deltaTime);

	// Open Door
	// CollisionStay Check

	if (_isOpenDoor)
		OpenDoor(deltaTime);
}

void DUOLClient::SystemManager::RimLightFromDeltaTime(float deltaTime)
{
	_currentTime += deltaTime;

	if (_rimLightObject == nullptr)
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("RimObject"))
			{
				_rimLightObject = gameObject;

				continue;
			}
		}
	}

	auto meshrender = _rimLightObject->GetComponent<DUOLGameEngine::MeshRenderer>();

}

void DUOLClient::SystemManager::OpenDoor(float deltaTime)
{
	_moveDoor += deltaTime;

	if (_doorObject == nullptr)
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("OpenDoor"))
			{
				_doorObject = gameObject;

				continue;
			}
		}

	}

	auto transform = _doorObject->GetComponent<DUOLGameEngine::Transform>();
	auto pos = transform->GetLocalPosition();

	if (1.5f * _moveDoor <= 6.0f)
	{
		pos.y += 1.5f * _moveDoor;

		transform->SetLocalPosition(pos);
	}
	else
		_isOpenDoor = false;
}
