#include "DUOLClient/Manager/SystemManager.h"

#include <rttr/registration>

#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLClient/Manager/UIDataManager.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"

namespace  DUOLClient
{

	DUOLClient::SystemManager* DUOLClient::SystemManager::_instance = nullptr;

	using namespace rttr;

	RTTR_REGISTRATION
	{
		rttr::registration::class_<DUOLClient::SystemManager>("SystemManager")
		.constructor()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
		(
			rttr::policy::ctor::as_raw_ptr
		);
	}

		SystemManager::SystemManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _isBStage(true)
		, _isBStageAllMonsterKill(false)
		, _isDoorMonsterKill(false)
		, _isOpenDoor(false)
		, _currentTime(0.0f)
		, _rimLightObject(nullptr)
		, _doorObject(nullptr)
		, _moveDoor(0.0f)
		, _rimPower(0.f)
	{
	}

	SystemManager* DUOLClient::SystemManager::GetInstance()
	{
		if (_instance == nullptr)
			_instance = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->AddComponent<SystemManager>();

		return _instance;
	}

	DUOLClient::SystemManager::~SystemManager()
	{
		_instance = nullptr;

	}

	void DUOLClient::SystemManager::OnAwake()
	{
		if (!_instance)
		{
			_instance = this;

		}
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
		if (_isBStage)
			BSystem(deltaTime);

	}

	void DUOLClient::SystemManager::DoorCollisionStay()
	{
		//// UI ¶°¾ßÇÔ 
		//DUOLClient::UIDataManager::GetInstance()->

		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F))
		{
			_isOpenDoor = true;
		}
	}

	void DUOLClient::SystemManager::BSystem(float deltaTime)
	{
		// Door Monster Trigger active
		auto control1 = EnemyManager::GetInstance()->GetEnemyGroupController(L"MonsterSpawnPoint_B_01_Gun05_Normal40");
		auto control2 = EnemyManager::GetInstance()->GetEnemyGroupController(L"Spawn1_MonsterSpawnPoint_B_02_Gun06_Normal40 // Spawn2_WeakEliteMonster2");

		if (control1 && control2)
		{
			if (control1->GetIsClearGroup() && control2->GetIsClearGroup())
				_isDoorMonsterKill = true;
			else
				return;
		}

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
			if (_rimLightObject == nullptr)
				return;
		}

		auto meshrender = _rimLightObject->GetComponent<DUOLGameEngine::MeshRenderer>();
		meshrender->SetEmissive(DUOLMath::Vector3(253, 253, 0));

		// Dark
		if (5.f <= _currentTime)
		{
			if (10.f <= _currentTime)
				_currentTime = 0.f;

			_rimPower -= (deltaTime * 5);

			if (_rimPower <= 1.5f)
				_rimPower = 1.5f;

			meshrender->SetEmissivePower(_rimPower);
		}
		else
		{
			_rimPower += (deltaTime * 5);

			if (_rimPower <= 1.5f)
				_rimPower = 1.5f;

			meshrender->SetEmissivePower(_rimPower);
		}

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
}