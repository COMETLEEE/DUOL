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
#include "DUOLGameEngine/Manager/TimeManager.h"
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
	);

}

namespace DUOLClient
{
	EnemyManager* EnemyManager::_instance = nullptr;

	EnemyManager::EnemyManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name), _isStart(false)
	{
		Initialize();
	}

	EnemyManager::~EnemyManager()
	{
		_instance = nullptr;

		for (auto& iter : _enemyDatas)
			delete iter;
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
			data->_patrolOffset = 13.0f;
			data->_lookRange = 20.0f;
			data->_maxSpeed = 3.5f;

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
			data->_attackRange = 13.0f;
			data->_patrolOffset = 15.0f;
			data->_lookRange = 20.0f;
			data->_maxSpeed = 3.0f;

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
		for (int i = 0; i < 20; i++)
		{
			auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			auto gameObj = scene->CreateFromFBXModel(TEXT("monster"));

			auto enemyBasic = gameObj->AddComponent<Enemy>();

			gameObj->AddComponent<AI_EnemyBasic>();

			enemyBasic->SetEnemyCode(GetEnemy(EnemyCode::Close));

			PushBack(TEXT("BasicEnemy_Close"), enemyBasic);
		}
		for (int i = 0; i < 20; i++)
		{
			auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			auto gameObj = scene->CreateFromFBXModel(TEXT("monster"));

			auto enemyBasic = gameObj->AddComponent<Enemy>();

			gameObj->AddComponent<AI_EnemyBasic>();

			enemyBasic->SetEnemyCode(GetEnemy(EnemyCode::Far));

			PushBack(TEXT("BasicEnemy_Far"), enemyBasic);
		}
		for (int i = 0; i < 20; i++)
		{
			auto projectileObject = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();

			auto projectile = projectileObject->AddComponent<Projectile>();

			PushBack(TEXT("Projectile"), projectile);
		}

	}

	void EnemyManager::Initialize()
	{
		_objectQueueGameObject = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();

		_objectQueueGameObject->SetName(TEXT("ObjectQueue"));

		_objectQueueGameObject->GetTransform()->SetParent(GetTransform());

		Initialize_MonsterData();

		Initialize_ObjectQueue();
	}

	DUOLGameEngine::CoroutineHandler EnemyManager::AutoReturnObejct(void* typeID, DUOLCommon::tstring key,
		DUOLGameEngine::MonoBehaviourBase* object, float timer)
	{

		co_yield std::make_shared<DUOLGameEngine::WaitForFrames>(2);

		auto timeManager = DUOLGameEngine::TimeManager::GetInstance();

		while (true)
		{
			if (timer != std::numeric_limits<float>::max())
				timer -= timeManager->GetDeltaTime();

			if (timer <= 0 ||
				!object->GetGameObject()->GetIsActive())
			{
				PushBack(key, typeID, object);
				co_return;
			}

			co_yield nullptr;
		}
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



	void EnemyManager::OnStart()
	{
		if (!_instance)
		{
			_instance = this;
		}
		else
			Destroy(this);
	}

	void EnemyManager::OnUpdate(float deltaTime)
	{
		if (!_isStart)
		{
			_isStart = true;

			_instance->GetGameObject()->SetName(TEXT("EnemyManager"));
		}
	}
}
