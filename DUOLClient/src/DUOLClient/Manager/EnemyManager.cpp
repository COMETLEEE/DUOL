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
#include "DUOLClient/ECS/Component/Enemy/EnemyEventFunctions.h"

#include "DUOLGameEngine/ECS/Component/SphereCollider.h"
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

	void EnemyManager::InsertHitFunc(DUOLCommon::tstring key,
		std::function<void(DUOLClient::Enemy*, CharacterBase*, float, AttackType)> func)
	{
		if (_enemyHits.contains(key))
			DUOL_TRACE(DUOL_CONSOLE, "The Hit function already exists. {0}", DUOLCommon::StringHelper::ToString(key))
		else
			_enemyHits.insert({ key,func });
	}

	std::function<void(DUOLClient::Enemy*, CharacterBase*, float, AttackType)> EnemyManager::GetHitFunc(DUOLCommon::tstring key)
	{
		if (_enemyHits.contains(key))
			return _enemyHits[key];

		return nullptr;
	}

	void EnemyManager::InsertEventFunc(DUOLCommon::tstring key, std::function<void(DUOLClient::Enemy*)> func)
	{
		if (_enemyEventFuncs.contains(key))
			DUOL_TRACE(DUOL_CONSOLE, "The Attack function already exists. {0}", DUOLCommon::StringHelper::ToString(key))
		else
			_enemyEventFuncs.insert({ key,func });
	}

	std::function<void(DUOLClient::Enemy*)> EnemyManager::GetEventFunc(DUOLCommon::tstring key)
	{
		if (_enemyEventFuncs.contains(key))
			return _enemyEventFuncs[key];

		return nullptr;
	}

	void EnemyManager::CreateEnemy(EnemyCode enemyCode) // 일반 몬스터를 생성하기위한 함수.
	{
		auto data = GetEnemy(enemyCode);

		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		auto gameObj = scene->CreateFromFBXModel(data->_fbxModelName);

		gameObj->SetName(data->_name);

		auto enemyGameObj = scene->CreateEmpty();

		enemyGameObj->GetTransform()->SetParent(gameObj->GetTransform());

		auto enemyBasic = enemyGameObj->AddComponent<Enemy>();

		enemyGameObj->AddComponent<AI_EnemyBasic>();

		for (auto& iter : data->_boolParameterInitDatas)
			enemyBasic->AddParameter(iter.first, iter.second);

		for (auto& iter : data->_floatParameterInitDatas)
			enemyBasic->AddParameter(iter.first, iter.second);

		enemyBasic->SetEnemyHitFunc(GetHitFunc(data->_hitFuncKey));

		enemyBasic->SetEnemyCode(data);

		for (auto& iter : data->_eventFuncKey)
			enemyBasic->EnemyAddEventFunc(iter, std::bind(GetEventFunc(iter), enemyBasic));

		/*for (auto& iter : data->_eventFuncKey)
		data->_additionalEventFunc;*/

		// Elite Monster => Lock On Target .. 플레이어 락온 기능을 위함
		if (enemyCode == EnemyCode::WeakElite || enemyCode == EnemyCode::Elite)
		{
			auto lockOnTarget = scene->CreateEmpty();

			lockOnTarget->SetName(TEXT("LockOnTarget"));

			lockOnTarget->SetLayer(TEXT("Enemy"));

			lockOnTarget->SetTag(TEXT("LockOnTarget"));

			lockOnTarget->GetTransform()->SetParent(gameObj->GetTransform());

			lockOnTarget->GetTransform()->SetLocalPosition(DUOLMath::Vector3(0.f, 1.35f, 0.f));

			// 이거 안 하면 날라가던데 ?
			lockOnTarget->AddComponent<DUOLGameEngine::SphereCollider>()->SetIsTrigger(true);
		}

		PushBack(data->_name, gameObj);
	}

	void EnemyManager::CreateCloseEnemy()
	{
		_closeEnemyCount--;

		CreateEnemy(EnemyCode::Close);
	}

	void EnemyManager::CreateFarEnemy()
	{
		_farEnemyCount--;

		CreateEnemy(EnemyCode::Far);
	}

	void EnemyManager::CreateWeakEliteEnemy()
	{
		CreateEnemy(EnemyCode::WeakElite);
	}

	void EnemyManager::CreateEliteEnemy()
	{
		CreateEnemy(EnemyCode::Elite);
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

	void EnemyManager::Initialize_RegisteEventFuncs()
	{
		InsertEventFunc(TEXT("Attack_Close"), Attack_Close);
		InsertEventFunc(TEXT("Attack_Far"), Attack_Far);
		InsertEventFunc(TEXT("ComboAttack1"), Attack_Close);
		InsertEventFunc(TEXT("ComboAttack2"), Attack_Close);
		InsertEventFunc(TEXT("ComboAttack3"), Attack_Close);
		InsertEventFunc(TEXT("SmashAttack"), Attack_Close);
		InsertEventFunc(TEXT("JumpAttackStart"), JumpAttackStart);
		InsertEventFunc(TEXT("JumpAttackEnd"), JumpAttackEnd);
		InsertEventFunc(TEXT("RushAndHit"), RushAndHit);
		InsertEventFunc(TEXT("SeriousPunch"), Attack_Close);

		InsertEventFunc(TEXT("RushParticlePlay"), RushParticlePlay);
		InsertEventFunc(TEXT("StopAnimator"), StopAnimator);
		InsertEventFunc(TEXT("SetBool_IsWakeUpToIdle_True"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsWakeUpToIdle"), true));
		InsertEventFunc(TEXT("SetNavOnRigidbodyOff"), SetNavOnRigidbodyOff);
		InsertEventFunc(TEXT("SetNavOffRigidbodyOn"), SetNavOffRigidbodyOn);
		InsertEventFunc(TEXT("LerpLookTarget"), LerpLookTarget);
		InsertEventFunc(TEXT("StandingCryParticle"), StandingCryParticle);

		InsertEventFunc(TEXT("SetBool_IsHeavyAttack_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsHeavyAttack"), false));
		InsertEventFunc(TEXT("SetBool_IsAttack_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsAttack"), false));
		InsertEventFunc(TEXT("SetBool_IsSeriousPunch_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsSeriousAttack"), false));
		InsertEventFunc(TEXT("SetBool_IsJumpAttack_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsJumpAttack"), false));
		InsertEventFunc(TEXT("SetBool_IsComboAttack_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsComboAttack"), false));
		InsertEventFunc(TEXT("SuperArmorOff_OnTimer"), SuperArmorOff_OnTimer);

	}

	void EnemyManager::Initialize_RegisteHitFuncs()
	{
		InsertHitFunc(TEXT("NormalEnemyHit"), NormalEnemyHit);
		InsertHitFunc(TEXT("WeakEliteEnemyHit"), WeakEliteEnemyHit);
		InsertHitFunc(TEXT("EliteEnemyHit"), EliteEnemyHit);
	}

	void EnemyManager::Initialize_RegisteObejctCreateFunc()
	{
		_objectCreateFuncs.insert({ TEXT("EnemyNear"),std::bind(&EnemyManager::CreateCloseEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("EnemyFar"),std::bind(&EnemyManager::CreateFarEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("Projectile"),std::bind(&EnemyManager::CreateProjectile,this) });

		_objectCreateFuncs.insert({ TEXT("WeakEnemyElite"),std::bind(&EnemyManager::CreateWeakEliteEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("EnemyElite"),std::bind(&EnemyManager::CreateEliteEnemy,this) });
	}

	void EnemyManager::Initialize_MonsterData()
	{
		for (auto& iter : _enemyDatas)
			delete iter;

		// 지금은 몬스터가 몇 없으니 하드코딩을 하지만 더 늘어난다면 json이나 툴에서 값을 저장하고 불러오도록 하자..!
		_enemyDatas.resize(static_cast<unsigned int>(EnemyCode::Count));

		// -------------------------------Close Enemy-----------------------------------------
		{
			EnemyData* data = new EnemyData();

			data->_name = TEXT("EnemyNear");
			data->_behaviorTreeName = "MainTree_CloseEnemy";
			data->_fbxModelName = TEXT("monster");
			data->_enemyCode = EnemyCode::Close;

			data->_boolParameterInitDatas.push_back({ TEXT("IsHit"), false });
			data->_boolParameterInitDatas.push_back({ TEXT("IsToken"), false });

			data->_floatParameterInitDatas.push_back({ TEXT("PatrolOffset"), 6.0f }); // 배회 범위
			data->_floatParameterInitDatas.push_back({ TEXT("LookRange"), 30.0f }); // 시야 거리.
			data->_floatParameterInitDatas.push_back({ TEXT("MaxSpeed"), 3.5f }); // 최대 속력.
			data->_floatParameterInitDatas.push_back({ TEXT("AttackRange"), 2.0f }); // 공격 사정거리.
			data->_floatParameterInitDatas.push_back({ TEXT("AttackCancelTime"), 0.8f }); // 백 점프시 공격 캔슬 타임.
			data->_floatParameterInitDatas.push_back({ TEXT("AttackDelayTime"), 4.0f }); // 몇 초 동안 사정거리안에 있을시 공격할 것인가.
			data->_floatParameterInitDatas.push_back({ TEXT("ChaseRange"), 20.0f }); // 어느정도 거리 안으로 들어왔을 때 추적을 시작할 것인가.
			data->_floatParameterInitDatas.push_back({ TEXT("Damage"), 10.0f }); // 기본 공격 대미지.
			data->_floatParameterInitDatas.push_back({ TEXT("MaxHp"), 100.0f }); // 최대 체력
			data->_floatParameterInitDatas.push_back({ TEXT("MaxAcceleration"), 3.5f }); // 최대 가속도
			data->_floatParameterInitDatas.push_back({ TEXT("PaperBurnOffset"), 0.0f }); // 페이퍼 번 옵셋 값.

			data->_animControllerName = TEXT("Monster_AnimatorController");
			data->_capsuleCenter = DUOLMath::Vector3(0, 1.0f, 0);
			data->_capsuleRadius = 0.5f;
			data->_navBaseOffset = DUOLMath::Vector3(0, -0.3f, 0);
			data->_height = 1.0f;

			data->_eventFuncKey.push_back(TEXT("Attack_Close"));
			data->_eventFuncKey.push_back(TEXT("StopAnimator"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsWakeUpToIdle_True"));

			data->_hitFuncKey = TEXT("NormalEnemyHit");

			_enemyDatas[static_cast<unsigned int>(data->_enemyCode)] = data;
		}
		// ---------------------------------------------------------------------------------------
		// -------------------------------Far Enemy---------------------------------------------
		{
			EnemyData* data = new EnemyData();

			data->_name = TEXT("EnemyFar");
			data->_behaviorTreeName = "MainTree_FarEnemy";
			data->_fbxModelName = TEXT("monster");
			data->_enemyCode = EnemyCode::Far;

			data->_boolParameterInitDatas.push_back({ TEXT("IsHit"), false });
			data->_boolParameterInitDatas.push_back({ TEXT("IsToken"), false });

			data->_floatParameterInitDatas.push_back({ TEXT("PatrolOffset"), 15.0f }); // 배회 범위
			data->_floatParameterInitDatas.push_back({ TEXT("LookRange"), 30.0f }); // 시야 거리.
			data->_floatParameterInitDatas.push_back({ TEXT("MaxSpeed"), 3.0f }); // 최대 속력.
			data->_floatParameterInitDatas.push_back({ TEXT("AttackRange"), 10.0f }); // 공격 사정거리.
			data->_floatParameterInitDatas.push_back({ TEXT("AttackDelayTime"), 20.0f }); // 몇 초 동안 사정거리안에 있을시 공격할 것인가.
			data->_floatParameterInitDatas.push_back({ TEXT("ChaseRange"), 20.0f }); // 어느정도 거리 안으로 들어왔을 때 추적을 시작할 것인가.
			data->_floatParameterInitDatas.push_back({ TEXT("Damage"), 10.0f }); // 기본 공격 대미지.
			data->_floatParameterInitDatas.push_back({ TEXT("MaxHp"), 100.0f }); // 최대 체력
			data->_floatParameterInitDatas.push_back({ TEXT("MaxAcceleration"), 3.5f }); // 최대 가속도
			data->_floatParameterInitDatas.push_back({ TEXT("PaperBurnOffset"), 0.0f }); // 페이퍼 번 옵셋 값.

			data->_animControllerName = TEXT("Monster_AnimatorController_Far");
			data->_capsuleCenter = DUOLMath::Vector3(0, 1.0f, 0);
			data->_capsuleRadius = 0.5f;
			data->_navBaseOffset = DUOLMath::Vector3(0, -0.3f, 0);
			data->_height = 1.0f;

			data->_eventFuncKey.push_back(TEXT("Attack_Far"));
			data->_eventFuncKey.push_back(TEXT("StopAnimator"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsWakeUpToIdle_True"));

			data->_hitFuncKey = TEXT("NormalEnemyHit");

			_enemyDatas[static_cast<unsigned int>(data->_enemyCode)] = data;
		}
		// ---------------------------------------------------------------------------------------
		// ---------------------------------Elite Enemy-----------------------------------------
		{
			EnemyData* data = new EnemyData();

			data->_name = TEXT("EnemyElite");
			data->_behaviorTreeName = "MainTree_EliteEnemy";
			data->_fbxModelName = TEXT("monster_elite");
			data->_enemyCode = EnemyCode::Elite;

			data->_boolParameterInitDatas.push_back({ TEXT("IsHit"), false });
			data->_boolParameterInitDatas.push_back({ TEXT("IsToken"), false });
			data->_boolParameterInitDatas.push_back({ TEXT("IsSuperArmor"), false });
			data->_boolParameterInitDatas.push_back({ TEXT("IsCanSuperArmor"), true });
			data->_boolParameterInitDatas.push_back({ TEXT("IsRushHit_Target"), false }); // 러쉬 패턴 중 타겟에게 명중하였는지.
			data->_boolParameterInitDatas.push_back({ TEXT("IsOnSuperArmorEvent"), false }); // 슈퍼아머 활성화 이벤트를 실행했는가.


			data->_floatParameterInitDatas.push_back({ TEXT("PatrolOffset"), 4.0f }); // 배회 범위
			data->_floatParameterInitDatas.push_back({ TEXT("ChaseRange"), 20.0f }); // 추적 범위

			data->_floatParameterInitDatas.push_back({ TEXT("LookRange"), 30.0f }); // 시야 거리.
			data->_floatParameterInitDatas.push_back({ TEXT("MaxSpeed"), 3.5f }); // 최대 속력.
			data->_floatParameterInitDatas.push_back({ TEXT("Damage"), 10.0f }); // 기본 공격 대미지.
			data->_floatParameterInitDatas.push_back({ TEXT("MaxHp"), 1000.0f }); // 최대 체력
			data->_floatParameterInitDatas.push_back({ TEXT("MaxAcceleration"), 100.0f }); // 최대 가속도
			data->_floatParameterInitDatas.push_back({ TEXT("SuperArmorMaxGauge"), 100.0f }); // 슈퍼 아머 최대 충전치.
			data->_floatParameterInitDatas.push_back({ TEXT("SuperArmorTime"), 4.0f }); // Elite Enemy의 슈퍼아머 지속 시간은 전체 시간이 아닌 패턴이 종료 된 후 남는 시간이다.
			data->_floatParameterInitDatas.push_back({ TEXT("SuperArmorCoolTime"), 6.0f }); // 슈퍼 아머의 쿨타임
			data->_floatParameterInitDatas.push_back({ TEXT("CurrentSuperArmorGauge"), 0.0f }); // 현재 슈퍼아머 게이지 충전량
			data->_floatParameterInitDatas.push_back({ TEXT("AttackRange"), 15.0f }); // 사정거리 밖으로 벗어나면 돌진 패턴을 실행한다.
			data->_floatParameterInitDatas.push_back({ TEXT("PaperBurnOffset"), 0.0f }); // 페이퍼 번 옵셋 값.

			data->_animControllerName = TEXT("Monster_AnimatorController_Elite");
			data->_capsuleCenter = DUOLMath::Vector3(0, 2.0f, 0);
			data->_capsuleRadius = 1.0f;
			data->_navBaseOffset = DUOLMath::Vector3(0, -0.3f, 0);
			data->_height = 1.3f;

			data->_eventFuncKey.push_back(TEXT("ComboAttack1"));
			data->_eventFuncKey.push_back(TEXT("ComboAttack2"));
			data->_eventFuncKey.push_back(TEXT("ComboAttack3"));
			data->_eventFuncKey.push_back(TEXT("SmashAttack"));
			data->_eventFuncKey.push_back(TEXT("StopAnimator"));
			data->_eventFuncKey.push_back(TEXT("JumpAttackStart"));
			data->_eventFuncKey.push_back(TEXT("JumpAttackEnd"));
			data->_eventFuncKey.push_back(TEXT("RushAndHit"));
			data->_eventFuncKey.push_back(TEXT("StandingCryParticle"));

			data->_eventFuncKey.push_back(TEXT("SeriousPunch"));
			data->_eventFuncKey.push_back(TEXT("RushParticlePlay"));
			data->_eventFuncKey.push_back(TEXT("LerpLookTarget"));

			data->_eventFuncKey.push_back(TEXT("SetNavOffRigidbodyOn"));
			data->_eventFuncKey.push_back(TEXT("SetNavOnRigidbodyOff"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsHeavyAttack_False"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsAttack_False"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsSeriousPunch_False"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsJumpAttack_False"));
			data->_eventFuncKey.push_back(TEXT("SuperArmorOff_OnTimer"));

			data->_hitFuncKey = TEXT("EliteEnemyHit");

			_enemyDatas[static_cast<unsigned int>(data->_enemyCode)] = data;
		}
		// ---------------------------------------------------------------------------------------
		// ---------------------------------Weak Elite Enemy-----------------------------------------
		{
			EnemyData* data = new EnemyData();

			data->_name = TEXT("WeakEnemyElite");
			data->_behaviorTreeName = "MainTree_WeakEliteEnemy";
			data->_fbxModelName = TEXT("monster_elite");
			data->_enemyCode = EnemyCode::WeakElite;

			data->_boolParameterInitDatas.push_back({ TEXT("IsHit"), false });
			data->_boolParameterInitDatas.push_back({ TEXT("IsToken"), false });
			data->_boolParameterInitDatas.push_back({ TEXT("IsSuperArmor"), false });
			data->_boolParameterInitDatas.push_back({ TEXT("IsCanSuperArmor"), true });

			data->_floatParameterInitDatas.push_back({ TEXT("PatrolOffset"), 6.0f }); // 배회 범위
			data->_floatParameterInitDatas.push_back({ TEXT("LookRange"), 30.0f }); // 시야 거리.
			data->_floatParameterInitDatas.push_back({ TEXT("MaxSpeed"), 4.0f }); // 최대 속력.
			data->_floatParameterInitDatas.push_back({ TEXT("AttackRange"), 3.0f }); // 공격 사정거리.
			data->_floatParameterInitDatas.push_back({ TEXT("AttackDelayTime"), 2.0f }); // 몇 초 동안 사정거리안에 있을시 공격할 것인가.
			data->_floatParameterInitDatas.push_back({ TEXT("ChaseRange"), 20.0f }); // 어느정도 거리 안으로 들어왔을 때 추적을 시작할 것인가.
			data->_floatParameterInitDatas.push_back({ TEXT("Damage"), 10.0f }); // 기본 공격 대미지.
			data->_floatParameterInitDatas.push_back({ TEXT("MaxHp"), 300.0f }); // 최대 체력
			data->_floatParameterInitDatas.push_back({ TEXT("MaxAcceleration"), 3.5f }); // 최대 가속도

			data->_floatParameterInitDatas.push_back({ TEXT("SuperArmorMaxGauge"), 100.0f }); // 슈퍼 아머 최대 충전치.
			data->_floatParameterInitDatas.push_back({ TEXT("SuperArmorTime"), 6.0f }); // Elite Enemy의 슈퍼아머 지속 시간은 전체 시간이 아닌 패턴이 종료 된 후 남는 시간이다.
			data->_floatParameterInitDatas.push_back({ TEXT("SuperArmorCoolTime"), 6.0f }); // 슈퍼 아머의 쿨타임
			data->_floatParameterInitDatas.push_back({ TEXT("CurrentSuperArmorGauge"), 0.0f }); // 현재 슈퍼아머 게이지 충전량
			data->_floatParameterInitDatas.push_back({ TEXT("PaperBurnOffset"), 0.0f }); // 페이퍼 번 옵셋 값.

			data->_animControllerName = TEXT("Monster_AnimatorController_WeakElite");
			data->_capsuleCenter = DUOLMath::Vector3(0, 1.6f, 0);
			data->_capsuleRadius = 1.0f;
			data->_navBaseOffset = DUOLMath::Vector3(0, -0.3f, 0);
			data->_height = 1.8f;

			data->_eventFuncKey.push_back(TEXT("ComboAttack1"));
			data->_eventFuncKey.push_back(TEXT("ComboAttack2"));
			data->_eventFuncKey.push_back(TEXT("ComboAttack3"));
			data->_eventFuncKey.push_back(TEXT("SmashAttack"));
			data->_eventFuncKey.push_back(TEXT("StopAnimator"));
			data->_eventFuncKey.push_back(TEXT("LerpLookTarget"));

			data->_eventFuncKey.push_back(TEXT("SetNavOffRigidbodyOn"));
			data->_eventFuncKey.push_back(TEXT("SetNavOnRigidbodyOff"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsComboAttack_False"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsAttack_False"));
			data->_eventFuncKey.push_back(TEXT("SetBool_IsAttack_False"));


			data->_hitFuncKey = TEXT("WeakEliteEnemyHit");

			_enemyDatas[static_cast<unsigned int>(data->_enemyCode)] = data;
		}
	}

	void EnemyManager::Initialize_ObjectQueue()
	{
		for (int i = 0; i < _closeEnemyAwakeCount; i++)
			CreateCloseEnemy();

		for (int i = 0; i < _farEnemyAwakeCount; i++)
			CreateFarEnemy();

		for (int i = 0; i < _projectileAwakeCount; i++)
			CreateProjectile();

		CreateWeakEliteEnemy();

		CreateEliteEnemy();
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
		Initialize_RegisteEventFuncs();

		Initialize_RegisteHitFuncs();

		Initialize_RegisteObejctCreateFunc();

		Initialize_MonsterData();

		Initialize_ObjectQueue();
	}

	EnemyManager* EnemyManager::GetInstance()
	{
		if (!_instance)
			_instance = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->AddComponent<EnemyManager>();

		return _instance;
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
