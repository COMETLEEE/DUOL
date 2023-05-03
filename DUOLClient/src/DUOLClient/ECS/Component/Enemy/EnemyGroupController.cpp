#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::EnemyGroupController>("EnemyGroupController")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_radius",&DUOLClient::EnemyGroupController::_radius)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_tokkenCount", &DUOLClient::EnemyGroupController::_tokkenCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	).property("_cohesion", &DUOLClient::EnemyGroupController::_cohesion)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	).property("_alignment", &DUOLClient::EnemyGroupController::_alignment)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	).property("_separation", &DUOLClient::EnemyGroupController::_separation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_farEnemyCount", &DUOLClient::EnemyGroupController::_farEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_closeEnemyCount", &DUOLClient::EnemyGroupController::_closeEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	);
}



DUOLClient::EnemyGroupController::EnemyGroupController(DUOLGameEngine::GameObject* owner,
	const DUOLCommon::tstring& name) :MonoBehaviourBase(owner, name),
	_enemys(), _radius(0),
	_farEnemyCount(0), _closeEnemyCount(0),
	_tokkenCount(0),
	_targetPos(), _isGroupCheck(false),
	_cohesion(1.0f), _alignment(1.0f), _separation(1.0f),
	_isOnceGroupCenter(false)
{
}


const std::unordered_map<DUOLCommon::UUID, DUOLClient::AI_EnemyBasic*>& DUOLClient::EnemyGroupController::GetGroupEnemys()
{
	return _enemys;
}

bool DUOLClient::EnemyGroupController::GetIsGroupCheck()
{
	return _isGroupCheck;
}

void DUOLClient::EnemyGroupController::CreateEnemy()
{
	StartCoroutine(&EnemyGroupController::CreateEnemyCoroutine);
}

DUOLGameEngine::CoroutineHandler DUOLClient::EnemyGroupController::CreateEnemyCoroutine()
{
	_isGroupCheck = false;

	auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

	_targetPos = GetTransform()->GetWorldPosition();

	auto look = GetGameObject()->GetTransform()->GetLook();


	for (int i = 0; i < _closeEnemyCount; i++)
	{
		const auto gameObject = EnemyManager::GetInstance()->Pop<DUOLGameEngine::GameObject>(TEXT("BasicEnemy_Close"));

		Enemy* enemy = nullptr;

		for (auto& iter : gameObject->GetTransform()->GetChildGameObjects())
		{
			if (!enemy)
				enemy = iter->GetComponent<Enemy>();
			else
				break;
		}

		enemy->InitializeData();

		_enemys[enemy->GetGameObject()->GetUUID()] = enemy->GetAIController();

		DUOLMath::Vector3 randVec = DUOLMath::Vector3(
			DUOLMath::MathHelper::RandF(-_radius, _radius),
			2.0f,
			DUOLMath::MathHelper::RandF(-_radius, _radius));

		enemy->SetPosition(_targetPos + randVec);

		_enemys[enemy->GetGameObject()->GetUUID()]->SetGroupController(this);

		gameObject->GetTransform()->LookAt(gameObject->GetTransform()->GetWorldPosition() + look);
		co_yield std::make_shared<DUOLGameEngine::WaitForFrames>(1);
	}

	for (int i = 0; i < _farEnemyCount; i++)
	{
		const auto gameObject = EnemyManager::GetInstance()->Pop<DUOLGameEngine::GameObject>(TEXT("BasicEnemy_Far"));

		Enemy* enemy = nullptr;

		for (auto& iter : gameObject->GetTransform()->GetChildGameObjects())
		{
			if (!enemy)
				enemy = iter->GetComponent<Enemy>();
			else
				break;
		}

		enemy->InitializeData();

		_enemys[enemy->GetGameObject()->GetUUID()] = enemy->GetAIController();

		DUOLMath::Vector3 randVec = DUOLMath::Vector3(
			DUOLMath::MathHelper::RandF(-_radius, _radius),
			2.0f, // 조금 위에서 떨어져야 충돌 체크를 할 수 있다...!
			DUOLMath::MathHelper::RandF(-_radius, _radius));

		enemy->SetPosition(_targetPos + randVec);

		gameObject->GetTransform()->LookAt(gameObject->GetTransform()->GetWorldPosition() + look);

		_enemys[enemy->GetGameObject()->GetUUID()]->SetGroupController(this);
		co_yield std::make_shared<DUOLGameEngine::WaitForFrames>(1);
	}
}

void DUOLClient::EnemyGroupController::EraseEnemy(DUOLCommon::UUID uuid)
{
	if (_enemys.contains(uuid))
		_enemys.erase(uuid);
}

DUOLMath::Vector3 DUOLClient::EnemyGroupController::GetGroupCenterPos()
{
	if (!_isOnceGroupCenter)
	{
		_isOnceGroupCenter = true;

		_enemyGroupCenterPos = DUOLMath::Vector3::Zero;

		const auto& groupEnemys = GetGroupEnemys();

		for (auto& [key, iter] : groupEnemys)
		{
			const auto otherPos = iter->GetGameObject()->GetTransform()->GetWorldPosition();

			_enemyGroupCenterPos += otherPos;
		}

		_enemyGroupCenterPos /= groupEnemys.size();
	}

	return _enemyGroupCenterPos;
}

void DUOLClient::EnemyGroupController::OnAwake()
{
}

void DUOLClient::EnemyGroupController::OnStart()
{
}

void DUOLClient::EnemyGroupController::OnUpdate(float deltaTime)
{
	for (auto& [key, value] : _enemys)
	{
		if (_tokkenCount <= 0) break;

		if (!value->GetIsToken())
		{
			value->TakeToken();
			_tokkenCount--;
			continue;
		}
	}
}

void DUOLClient::EnemyGroupController::OnLateUpdate(float deltaTime)
{
	_isOnceGroupCenter = false;
}
