#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

using namespace rttr;

RTTR_REGISTRATION
{

	rttr::registration::class_<DUOLClient::EnemyCreateInfo>("EnemyCreateInfo")
	.constructor<>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_closeEnemyCount",&DUOLClient::EnemyCreateInfo::_closeEnemyCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_farEnemyCount",&DUOLClient::EnemyCreateInfo::_farEnemyCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_weakEliteEnemyCount",&DUOLClient::EnemyCreateInfo::_weakEliteEnemyCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_eliteEnemyCount",&DUOLClient::EnemyCreateInfo::_eliteEnemyCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_createWaitForSeconds",&DUOLClient::EnemyCreateInfo::_createWaitForSeconds)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);


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
	).property("_initIdleSoundTimer", &DUOLClient::EnemyGroupController::_initIdleSoundTimer)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);



}



DUOLClient::EnemyGroupController::EnemyGroupController(DUOLGameEngine::GameObject* owner,
	const DUOLCommon::tstring& name) :MonoBehaviourBase(owner, name),
	_enemys(), _radius(0),
	_tokkenCount(0),
	_isGroupCheck(false),
	_cohesion(1.0f), _alignment(1.0f), _separation(2.0f),
	_isOnceGroupCenter(false),
	_isCreateEnemy(false),
	_initIdleSoundTimer(1.0f),
	_idleSoundTimer(_initIdleSoundTimer),
	_triggerCount(0),
	_isPrevHit(false),
	_hitTimer(0),
	_isWaveCodition(false)
{
}


DUOLClient::Enemy* DUOLClient::EnemyGroupController::PopEnemy(DUOLCommon::tstring name,
	const DUOLMath::Vector3& targetPos,
	float radius,
	float rotateDegree)
{

	const auto gameObject = EnemyManager::GetInstance()->Pop<DUOLGameEngine::GameObject>(name);

	Enemy* enemy = nullptr;

	for (auto& iter : gameObject->GetTransform()->GetChildGameObjects())
	{
		if (!enemy)
			enemy = iter->GetComponent<Enemy>();
		else
			break;
	}

	DUOLMath::Vector3 randVec = DUOLMath::Vector3(
		DUOLMath::MathHelper::RandF(-radius, radius),
		2.0f,
		DUOLMath::MathHelper::RandF(-radius, radius));

	enemy->SetPosition(targetPos + randVec);

	enemy->InitializeData();

	_enemys[enemy->GetGameObject()->GetUUID()] = enemy->GetAIController();

	_enemys[enemy->GetGameObject()->GetUUID()]->SetGroupController(this);

	gameObject->GetTransform()->SetRotation(DUOLMath::Quaternion::CreateFromAxisAngle(DUOLMath::Vector3(0, 1, 0), DUOLMath::XMConvertToRadians(rotateDegree)));

	return enemy;
}

const std::unordered_map<DUOLCommon::UUID, DUOLClient::AI_EnemyBasic*>& DUOLClient::EnemyGroupController::GetGroupEnemys()
{
	return _enemys;
}

bool DUOLClient::EnemyGroupController::GetIsGroupCheck()
{
	return _isGroupCheck;
}

void DUOLClient::EnemyGroupController::SetCreateInfo(const EnemyCreateInfo& firstInfo,
	const EnemyCreateInfo& secondInfo)
{
	_firstAndSecondInfo.push_back({ firstInfo ,secondInfo });
}

void DUOLClient::EnemyGroupController::CreateEnemy()
{
	_isGroupCheck = false;

	_triggerCount--;

	StartCoroutine(&EnemyGroupController::CreateEnemyCoroutine);
}

DUOLGameEngine::CoroutineHandler DUOLClient::EnemyGroupController::CreateEnemyCoroutine()
{

	while (_isCreateEnemy) // busy Wait
	{
		co_yield nullptr;
	}

	_isCreateEnemy = true;

	co_yield nullptr;


	if (!_firstAndSecondInfo.empty())
	{
		auto createInfoPair = _firstAndSecondInfo.back();

		_firstAndSecondInfo.pop_back();

		auto particle = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterSummon, 4);

		particle->GetTransform()->SetPosition(createInfoPair.first._createPos);

		particle->GetTransform()->SetLocalScale(DUOLMath::Vector3(_radius / 2.0f, 1, _radius / 2.0f));

		switch (createInfoPair.first._enemyCreateType)
		{
		case EnemyCreateType::Normal:
		{
			//co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);

			// 코루틴 안에서 다른 코루틴을 호출할 수 없다...! 하드 코딩으로 하자..!
			for (int i = 0; i < createInfoPair.first._bossEnemyCount; i++)
			{
				// Todo : 보스 생성할 때 Warning 치기.
				auto boss = PopEnemy(TEXT("EnemyBoss"), createInfoPair.first._createPos, 0, createInfoPair.first._rotateDegree);
				boss->GetAnimator()->SetBool(TEXT("IsFormChange"), true);
				boss->GetAnimator()->SetBool(TEXT("IsSwordForm"), true);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.first._eliteEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyElite"), createInfoPair.first._createPos, 0, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.first._weakEliteEnemyCount; i++)
			{
				PopEnemy(TEXT("WeakEnemyElite"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.first._closeEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyNear"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.first._farEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyFar"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._bossEnemyCount; i++)
			{
				auto boss = PopEnemy(TEXT("EnemyBoss"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				boss->GetAnimator()->SetBool(TEXT("IsFormChange"), true);
				boss->GetAnimator()->SetBool(TEXT("IsSwordForm"), true);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._eliteEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyElite"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._weakEliteEnemyCount; i++)
			{
				PopEnemy(TEXT("WeakEnemyElite"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._closeEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyNear"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._farEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyFar"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}
		}
		break;
		case EnemyCreateType::Wave:
		{
			//co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);

			for (int j = 0; j < createInfoPair.first._waveCount; j++)
			{
				// 코루틴 안에서 다른 코루틴을 호출할 수 없다...! 하드 코딩으로 하자..!
				for (int i = 0; i < createInfoPair.first._bossEnemyCount; i++)
				{
					// Todo : 보스 생성할 때 Warning 치기.
					auto boss = PopEnemy(TEXT("EnemyBoss"), createInfoPair.first._createPos, 0, createInfoPair.first._rotateDegree);
					boss->GetAnimator()->SetBool(TEXT("IsFormChange"), true);
					boss->GetAnimator()->SetBool(TEXT("IsSwordForm"), true);
				}
				for (int i = 0; i < createInfoPair.first._eliteEnemyCount; i++)
					PopEnemy(TEXT("EnemyElite"), createInfoPair.first._createPos, 0, createInfoPair.first._rotateDegree);
				for (int i = 0; i < createInfoPair.first._weakEliteEnemyCount; i++)
					PopEnemy(TEXT("WeakEnemyElite"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				for (int i = 0; i < createInfoPair.first._closeEnemyCount; i++)
					PopEnemy(TEXT("EnemyNear"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				for (int i = 0; i < createInfoPair.first._farEnemyCount; i++)
					PopEnemy(TEXT("EnemyFar"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int j = 0; j < createInfoPair.second._waveCount; j++)
			{
				for (int i = 0; i < createInfoPair.second._bossEnemyCount; i++)
				{
					auto boss = PopEnemy(TEXT("EnemyBoss"), createInfoPair.second._createPos, 0, createInfoPair.first._rotateDegree);
					boss->GetAnimator()->SetBool(TEXT("IsFormChange"), true);
					boss->GetAnimator()->SetBool(TEXT("IsSwordForm"), true);
				}

				for (int i = 0; i < createInfoPair.second._eliteEnemyCount; i++)
					PopEnemy(TEXT("EnemyElite"), createInfoPair.second._createPos, 0, createInfoPair.first._rotateDegree);

				for (int i = 0; i < createInfoPair.second._weakEliteEnemyCount; i++)
					PopEnemy(TEXT("WeakEnemyElite"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);

				for (int i = 0; i < createInfoPair.second._closeEnemyCount; i++)
					PopEnemy(TEXT("EnemyNear"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);

				for (int i = 0; i < createInfoPair.second._farEnemyCount; i++)
					PopEnemy(TEXT("EnemyFar"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

		}
		break;
		case EnemyCreateType::AllDied:

			while (!_enemys.empty()) // busy Wait
			{
				co_yield nullptr;
			}

			//co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);

			// 코루틴 안에서 다른 코루틴을 호출할 수 없다...! 하드 코딩으로 하자..!
			for (int i = 0; i < createInfoPair.first._bossEnemyCount; i++)
			{
				// Todo : 보스 생성할 때 Warning 치기.
				auto boss = PopEnemy(TEXT("EnemyBoss"), createInfoPair.first._createPos, 0, createInfoPair.first._rotateDegree);
				boss->GetAnimator()->SetBool(TEXT("IsFormChange"), true);
				boss->GetAnimator()->SetBool(TEXT("IsSwordForm"), true);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.first._eliteEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyElite"), createInfoPair.first._createPos, 0, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.first._weakEliteEnemyCount; i++)
			{
				PopEnemy(TEXT("WeakEnemyElite"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.first._closeEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyNear"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.first._farEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyFar"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._bossEnemyCount; i++)
			{
				// Todo : 보스 생성할 때 Warning 치기.
				auto boss = PopEnemy(TEXT("EnemyBoss"), createInfoPair.second._createPos, 0, createInfoPair.first._rotateDegree);
				boss->GetAnimator()->SetBool(TEXT("IsFormChange"), true);
				boss->GetAnimator()->SetBool(TEXT("IsSwordForm"), true);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._eliteEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyElite"), createInfoPair.second._createPos, 0, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._weakEliteEnemyCount; i++)
			{
				PopEnemy(TEXT("WeakEnemyElite"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._closeEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyNear"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}

			for (int i = 0; i < createInfoPair.second._farEnemyCount; i++)
			{
				PopEnemy(TEXT("EnemyFar"), createInfoPair.second._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.second._createWaitForSeconds);
			}
			break;
		case EnemyCreateType::ConditionWave:
		{
			while (!_isWaveCodition)
			{
				for (int i = 0; i < createInfoPair.first._bossEnemyCount; i++)
				{
					// Todo : 보스 생성할 때 Warning 치기.
					auto boss = PopEnemy(TEXT("EnemyBoss"), createInfoPair.first._createPos, 0, createInfoPair.first._rotateDegree);
					boss->GetAnimator()->SetBool(TEXT("IsFormChange"), true);
					boss->GetAnimator()->SetBool(TEXT("IsSwordForm"), true);
				}
				for (int i = 0; i < createInfoPair.first._eliteEnemyCount; i++)
					PopEnemy(TEXT("EnemyElite"), createInfoPair.first._createPos, 0, createInfoPair.first._rotateDegree);
				for (int i = 0; i < createInfoPair.first._weakEliteEnemyCount; i++)
					PopEnemy(TEXT("WeakEnemyElite"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				for (int i = 0; i < createInfoPair.first._closeEnemyCount; i++)
					PopEnemy(TEXT("EnemyNear"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				for (int i = 0; i < createInfoPair.first._farEnemyCount; i++)
					PopEnemy(TEXT("EnemyFar"), createInfoPair.first._createPos, _radius, createInfoPair.first._rotateDegree);
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(createInfoPair.first._createWaitForSeconds);
			}

			_isWaveCodition = false;

			break;
		}
		}
	}

	_isCreateEnemy = false;
}

void DUOLClient::EnemyGroupController::EraseEnemy(DUOLCommon::UUID uuid)
{
	if (_enemys.contains(uuid))
		_enemys.erase(uuid);
}

float DUOLClient::EnemyGroupController::GetIdleSoundTimer()
{
	return _idleSoundTimer;
}

void DUOLClient::EnemyGroupController::SetIdleSoundTimer(float value)
{
	_idleSoundTimer = value;
}

void DUOLClient::EnemyGroupController::ResetIdleSoundTimer()
{
	_idleSoundTimer += DUOLMath::MathHelper::RandF(0.85f, _initIdleSoundTimer);
}

void DUOLClient::EnemyGroupController::IncreaseTriggerCount()
{
	_triggerCount++;
}

bool DUOLClient::EnemyGroupController::GetIsClearGroup()
{
	if (_triggerCount > 0) return false;
	if (!_firstAndSecondInfo.empty()) return false;
	if (!_enemys.empty()) return false;

	return true;
}

void DUOLClient::EnemyGroupController::SetisPrevHit()
{
	_isPrevHit = true;
	_hitTimer = 1.0f;
}

void DUOLClient::EnemyGroupController::AllDie()
{
	for (auto& iter : _enemys)
	{
		iter.second->GetEnemy()->OnHit(nullptr, 10000.0f, AttackType::LightAttack);
	}
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

		if (groupEnemys.size() > 0)
			_enemyGroupCenterPos /= groupEnemys.size();
	}

	return _enemyGroupCenterPos;
}

void DUOLClient::EnemyGroupController::SetTrueWaveCondition()
{
	_isWaveCodition = true;
}

void DUOLClient::EnemyGroupController::OnAwake()
{
}

void DUOLClient::EnemyGroupController::OnStart()
{
}

void DUOLClient::EnemyGroupController::OnUpdate(float deltaTime)
{
	_idleSoundTimer -= deltaTime;
	_hitTimer -= deltaTime;

	if (_hitTimer < 0)
		_isPrevHit = false;
}

void DUOLClient::EnemyGroupController::OnLateUpdate(float deltaTime)
{
	_isOnceGroupCenter = false;
}
