#include "DUOLClient/Manager/EnemyManager.h"
#include <rttr/registration>

#include "DUOLClient/ECS/Component/ParticleData.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyAttacks.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"

#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::EnemyManager>("EnemyManager")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_closeEnemyAwakeCount",&DUOLClient::EnemyManager::_closeEnemyAwakeCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_farEnemyAwakeCount",&DUOLClient::EnemyManager::_farEnemyAwakeCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_projectileAwakeCount",&DUOLClient::EnemyManager::_projectileAwakeCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_closeEnemyCount",&DUOLClient::EnemyManager::_closeEnemyCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_farEnemyCount",&DUOLClient::EnemyManager::_farEnemyCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_projectileCount",&DUOLClient::EnemyManager::_projectileCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	);
}

namespace DUOLClient
{
	EnemyManager* EnemyManager::_instance = nullptr;

	EnemyManager::EnemyManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name), _isComplete(false),
		_playerCharacterGameObject(nullptr),
		_closeEnemyAwakeCount(0),
		_farEnemyAwakeCount(0),
		_projectileAwakeCount(0),
		_closeEnemyCount(0),
		_farEnemyCount(0),
		_projectileCount(0)
	{
	}

	EnemyManager::~EnemyManager()
	{
		_instance = nullptr;

		for (auto& iter : _enemyDatas)
			delete iter;
	}

	void EnemyManager::CreateCloseEnemy()
	{
		_closeEnemyCount--;

		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		auto gameObj = scene->CreateFromFBXModel(TEXT("monster"));

		auto enemyGameObj = scene->CreateEmpty();

		enemyGameObj->GetTransform()->SetParent(gameObj->GetTransform());

		auto enemyBasic = enemyGameObj->AddComponent<Enemy>();

		enemyGameObj->AddComponent<AI_EnemyBasic>();

		enemyBasic->SetEnemyCode(GetEnemy(EnemyCode::Close));

		PushBack(TEXT("BasicEnemy_Close"), gameObj);
	}

	void EnemyManager::CreateFarEnemy()
	{
		_farEnemyCount--;

		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		auto gameObj = scene->CreateFromFBXModel(TEXT("monster"));

		auto enemyGameObj = scene->CreateEmpty();

		enemyGameObj->GetTransform()->SetParent(gameObj->GetTransform());

		auto enemyBasic = enemyGameObj->AddComponent<Enemy>();

		enemyGameObj->AddComponent<AI_EnemyBasic>();

		enemyBasic->SetEnemyCode(GetEnemy(EnemyCode::Far));

		PushBack(TEXT("BasicEnemy_Far"), gameObj);
	}

	void EnemyManager::CreateProjectile()
	{
		_projectileCount--;

		auto projectileObject = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();

		auto projectile = projectileObject->AddComponent<Projectile>();

		auto meshFilter = projectileObject->AddComponent<DUOLGameEngine::MeshFilter>();

		meshFilter->SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Building_ShopD.003")));

		auto meshRenderer = projectileObject->AddComponent<DUOLGameEngine::MeshRenderer>();

		meshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("M_paint_indigo.006")));

		PushBack(TEXT("Projectile"), projectile);
	}

	void EnemyManager::Initialize_RegisteObejctCreateFunc()
	{
		_objectCreateFuncs.insert({ TEXT("BasicEnemy_Close"),std::bind(&EnemyManager::CreateCloseEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("BasicEnemy_Far"),std::bind(&EnemyManager::CreateFarEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("Projectile"),std::bind(&EnemyManager::CreateProjectile,this) });
	}

	void EnemyManager::Initialize_MonsterData()
	{
		for (auto& iter : _enemyDatas)
			delete iter;

		// 지금은 몬스터가 3마리 밖에 없으니 하드코딩을 하지만 더 늘어난다면 json이나 툴에서 값을 저장하고 불러오도록 하자..!
		_enemyDatas.resize(static_cast<unsigned int>(EnemyCode::Count));

		// --------------------------------------------------------------------------------------
		{
			EnemyData* data = new EnemyData();

			data->_name = TEXT("EnemyNear");
			data->_behaviorTreeName = "Enemy_MainTree";

			data->_damage = 10.0f;
			data->_maxHp = 100.0f;
			data->_attackRange = 3.0f;
			data->_patrolOffset = 10.0f;
			data->_lookRange = 30.0f;
			data->_maxSpeed = 3.5f;
			data->_attackDelayTime = 2.0f;
			data->_attackCancelTime = 0.8f;
			data->_chaseRange = 20.0f;


			data->_animControllerName = TEXT("Monster_AnimatorController");
			data->_capsuleCenter = DUOLMath::Vector3(0, 1.0f, 0);
			data->_navBaseOffset = DUOLMath::Vector3(0, -0.3f, 0);
			data->_height = 1.0f;

			data->_attackFuncs.insert({ TEXT("Attack_Close"), Attack_Close });

			_enemyDatas[static_cast<unsigned int>(EnemyCode::Close)] = data;
		}
		// ---------------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------------
		{
			EnemyData* data = new EnemyData();

			data->_name = TEXT("EnemyFar");
			data->_behaviorTreeName = "Enemy_MainTree";

			data->_damage = 10.0f;
			data->_maxHp = 100.0f;
			data->_attackRange = 10.0f;
			data->_patrolOffset = 15.0f;
			data->_lookRange = 30.0f;
			data->_maxSpeed = 3.0f;
			data->_attackDelayTime = 8.0f;
			data->_attackCancelTime = 0.1f;
			data->_chaseRange = 20.0f;

			data->_animControllerName = TEXT("Monster_AnimatorController_Far");
			data->_capsuleCenter = DUOLMath::Vector3(0, 1.0f, 0);
			data->_navBaseOffset = DUOLMath::Vector3(0, -0.3f, 0);
			data->_height = 1.0f;

			data->_attackFuncs.insert({ TEXT("Attack_Far"), Attack_Far });

			_enemyDatas[static_cast<unsigned int>(EnemyCode::Far)] = data;
		}
		// ---------------------------------------------------------------------------------------
	}

	void EnemyManager::Initialize_ObjectQueue()
	{
		for (int i = 0; i < _closeEnemyAwakeCount; i++)
			CreateCloseEnemy();
		for (int i = 0; i < _farEnemyAwakeCount; i++)
			CreateFarEnemy();
		for (int i = 0; i < _projectileAwakeCount; i++)
			CreateProjectile();
	}

	void EnemyManager::Initialize()
	{
		_objectQueueGameObject = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();

		_objectQueueGameObject->SetName(TEXT("ObjectQueue"));

		_objectQueueGameObject->GetTransform()->SetParent(GetTransform());

		auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : allGameObjects)
		{
			if (gameObject->GetTag() == TEXT("Player"))
			{
				_playerCharacterGameObject = gameObject;
			}
			else if (gameObject->GetTag() == TEXT("EnemyGroupController"))
			{
				if (_enemyGroupControllers.contains(gameObject->GetName()))
					assert(false); // 그룹 컨트롤러의 이름이 겹칩니다.
				_enemyGroupControllers.insert({ gameObject->GetName(),gameObject->GetComponent<EnemyGroupController>() });
			}
		}
		Initialize_RegisteObejctCreateFunc();

		Initialize_MonsterData();

		Initialize_ObjectQueue();
	}

	EnemyManager* EnemyManager::GetInstance()
	{
		if (_instance)
		{
			return _instance;
		}
		else
		{
			_instance = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->AddComponent<EnemyManager>();
			return _instance;
		}
	}

	EnemyData* EnemyManager::GetEnemy(EnemyCode enemyCode)
	{
		return _enemyDatas[static_cast<unsigned int>(enemyCode)];
	}

	DUOLGameEngine::GameObject* EnemyManager::GetPlayerCharacterGameObject()
	{
		if (!_playerCharacterGameObject)
		{
			auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			for (auto gameObject : allGameObjects)
			{
				if (gameObject->GetTag() == TEXT("Player"))
				{
					_playerCharacterGameObject = gameObject;
					break;
				}
			}
		}

		return _playerCharacterGameObject;
	}

	EnemyGroupController* EnemyManager::GetEnemyGroupController(DUOLCommon::tstring name)
	{
		if (_enemyGroupControllers.contains(name))
			return _enemyGroupControllers.at(name);
		return nullptr;
	}


	void EnemyManager::OnAwake()
	{
		if (!_instance)
		{
			_instance = this;

			Initialize();
		}
		else
			Destroy(this);
	}

	void EnemyManager::OnStart()
	{
		_instance->GetGameObject()->SetName(TEXT("EnemyManager"));
	}

	void EnemyManager::OnUpdate(float deltaTime)
	{
		if (_isComplete) return;

		if (_closeEnemyCount <= 0 && _farEnemyCount <= 0 && _projectileCount <= 0)
			_isComplete = true;

		if (_closeEnemyCount > 0)
		{
			CreateCloseEnemy();
			return;
		}

		if (_farEnemyCount > 0)
		{
			CreateFarEnemy();
			return;
		}

		if (_projectileCount > 0)
		{
			CreateProjectile();
			return;
		}
	}
}
