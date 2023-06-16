#include "DUOLClient/Manager/EnemyManager.h"
#include <rttr/registration>

#include <DUOLJson/JsonReader.h>

#include "DUOLClient/ECS/Component/ParticleData.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyAttacks.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/BossEnemy_Weapon_Sword.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyEventFunctions.h"

#include "DUOLGameEngine/ECS/Component/SphereCollider.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLClient/Camera/MainCameraController.h"

#include "DUOLClient/Manager/EnemyAudioManager.h"

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
	.property("_projectileAwakeCount",&DUOLClient::EnemyManager::_projectileAwakeCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_bossEnemyAwakeCount",&DUOLClient::EnemyManager::_bossEnemyAwakeCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_bossEnemyAwakeCount",&DUOLClient::EnemyManager::_bossEnemyAwakeCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_chargeAttackAwakeCount",&DUOLClient::EnemyManager::_chargeAttackAwakeCount)
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
		_projectileCount(0),
		_chargeAttackAwakeCount(1),
		_enemyAudioManager(nullptr)
	{
	}

	EnemyManager::~EnemyManager()
	{
		_instance = nullptr;

		for (auto& iter : _enemyDatas)
			delete iter;

		delete _enemyAudioManager;
		_enemyAudioManager = nullptr;
	}

	void EnemyManager::InsertHitFunc(DUOLCommon::tstring key,
		std::function<bool(DUOLClient::Enemy*, CharacterBase*, float, AttackType)> func)
	{
		if (_enemyHits.contains(key))
			DUOL_TRACE(DUOL_CONSOLE, "The Hit function already exists. {0}", DUOLCommon::StringHelper::ToString(key))
		else
			_enemyHits.insert({ key,func });
	}

	std::function<bool(DUOLClient::Enemy*, CharacterBase*, float, AttackType)> EnemyManager::GetHitFunc(DUOLCommon::tstring key)
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

	DUOLGameEngine::GameObject* EnemyManager::CreateEnemy(EnemyCode enemyCode) // 일반 몬스터를 생성하기위한 함수.
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
		if (enemyCode == EnemyCode::WeakElite || enemyCode == EnemyCode::Elite || enemyCode == EnemyCode::Boss)
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

		return gameObj;
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

	void EnemyManager::CreateBossEnemy()
	{
		auto enemyObject = CreateEnemy(EnemyCode::Boss);

		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		auto sword = scene->CreateFromFBXModel(TEXT("player_sword"));

		auto swrodComponent = sword->AddComponent<BossEnemy_Weapon_Sword>();

		auto allObect = enemyObject->GetTransform()->GetAllChildGameObjects();

		for (auto& iter : allObect)
		{
			auto enemy = iter->GetComponent<Enemy>();

			if (enemy)
			{
				swrodComponent->SetOwner(enemy);
				break;
			}
		}

	}

	void EnemyManager::CreateChargeAttack()
	{
		auto projectileObject = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();

		auto projectile = projectileObject->AddComponent<Projectile>();

		PushBack(TEXT("NoneMeshProjectile"), projectile);
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
		InsertEventFunc(TEXT("SeriousPunch"), Attack_Charge);

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
		InsertEventFunc(TEXT("GroggyOff_OnTimer"), GroggyOff_OnTimer);

		InsertEventFunc(TEXT("SetBool_IsFormChange_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsFormChange"), false));
		InsertEventFunc(TEXT("SetBool_IsSwordPattern1_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsSwordPattern1"), false));
		InsertEventFunc(TEXT("SetBool_IsSwordPattern2_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsSwordPattern2"), false));
		InsertEventFunc(TEXT("SetBool_IsSwordPattern3_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsSwordPattern3"), false));
		InsertEventFunc(TEXT("SetBool_IsFistPattern1_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsFistPattern1"), false));
		InsertEventFunc(TEXT("SetBool_IsFistPattern2_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsFistPattern2"), false));
		InsertEventFunc(TEXT("SetBool_IsFistPattern3_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsFistPattern3"), false));
		InsertEventFunc(TEXT("SetBool_IsDash_False"), std::bind(EventSetBool, std::placeholders::_1, TEXT("IsDash"), false));
		InsertEventFunc(TEXT("RandomLookAt"), RandomLookAt);
		InsertEventFunc(TEXT("HoldSword"), HoldSword);
		InsertEventFunc(TEXT("HouseSword"), HouseSword);
	}

	void EnemyManager::Initialize_RegisteHitFuncs()
	{
		InsertHitFunc(TEXT("NormalEnemyHit"), NormalEnemyHit);
		InsertHitFunc(TEXT("WeakEliteEnemyHit"), WeakEliteEnemyHit);
		InsertHitFunc(TEXT("EliteEnemyHit"), EliteEnemyHit);
		InsertHitFunc(TEXT("BossEnemyHit"), BossEnemyHit);
	}

	void EnemyManager::Initialize_RegisteObejctCreateFunc()
	{
		_objectCreateFuncs.insert({ TEXT("EnemyNear"),std::bind(&EnemyManager::CreateCloseEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("EnemyFar"),std::bind(&EnemyManager::CreateFarEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("Projectile"),std::bind(&EnemyManager::CreateProjectile,this) });
		_objectCreateFuncs.insert({ TEXT("NoneMeshProjectile"),std::bind(&EnemyManager::CreateChargeAttack,this) });

		_objectCreateFuncs.insert({ TEXT("WeakEnemyElite"),std::bind(&EnemyManager::CreateWeakEliteEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("EnemyElite"),std::bind(&EnemyManager::CreateEliteEnemy,this) });
		_objectCreateFuncs.insert({ TEXT("EnemyBoss"),std::bind(&EnemyManager::CreateBossEnemy,this) });
	}

	void EnemyManager::Initialize_MonsterData()
	{
		for (auto& iter : _enemyDatas)
			delete iter;

		_enemyDatas.clear();

		LoadEnemyTable();
	}

	void EnemyManager::Initialize_ObjectQueue()
	{
		for (int i = 0; i < _closeEnemyAwakeCount; i++)
			CreateCloseEnemy();

		for (int i = 0; i < _farEnemyAwakeCount; i++)
			CreateFarEnemy();

		for (int i = 0; i < _projectileAwakeCount; i++)
			CreateProjectile();

		//for (int i = 0; i < _bossEnemyAwakeCount; i++)
		//	CreateBossEnemy();

		for (int i = 0; i < _chargeAttackAwakeCount; i++)
			CreateChargeAttack();

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

		_enemyAudioManager = new EnemyAudioManager();
		_enemyAudioManager->Initialize();

		Initialize_RegisteEventFuncs();

		Initialize_RegisteHitFuncs();

		Initialize_RegisteObejctCreateFunc();

		Initialize_MonsterData();

		Initialize_ObjectQueue();
	}

	void EnemyManager::LoadEnemyTable()
	{
		auto jsonLoader = DUOLJson::JsonReader::GetInstance();

		const DUOLCommon::tstring enemyDataTable(_T("Asset/DataTable/EnemyDataTable.json"));

		auto enemyDatas = jsonLoader->LoadJson(enemyDataTable);

		const TCHAR* name = _T("Name");
		const TCHAR* behaviorTreeName = _T("BehaviorTreeName");
		const TCHAR* fbxModelName = _T("FBXModelName");
		const TCHAR* enemyCode = _T("EnemyCode");
		const TCHAR* floatParameterInitData_Str = _T("FloatParameterInitData_Str");
		const TCHAR* floatParameterInitData_Value = _T("FloatParameterInitData_Value");
		const TCHAR* boolParameterInitData_Str = _T("BoolParameterInitData_Str");
		const TCHAR* boolParameterInitData_Value = _T("BoolParameterInitData_Value");
		const TCHAR* height = _T("Height");
		const TCHAR* animControllerName = _T("AnimControllerName");
		const TCHAR* capsuleCenter = _T("CapsuleCenter");
		const TCHAR* capsuleRadius = _T("CapsuleRadius");
		const TCHAR* navBaseOffset = _T("NavBaseOffset");
		const TCHAR* eventFuncKey = _T("EventFuncKey");
		const TCHAR* hitFuncKey = _T("HitFuncKey");

		for (auto& enemyData : enemyDatas->GetArray())
		{
			EnemyData* data = new EnemyData();

			auto funcLoadString = [&](const TCHAR* c_str) {if (enemyData.HasMember(c_str)) return enemyData[c_str].GetString(); return _T(""); };

			auto funcLoadFloat = [&](const TCHAR* c_str) {if (enemyData.HasMember(c_str)) return enemyData[c_str].GetFloat(); return 0.0f; };

			auto funcLoadVector3 = [&](const TCHAR* c_str) {DUOLMath::Vector3 newVector3; if (enemyData.HasMember(c_str)) { newVector3.x = std::stof(enemyData[c_str].GetArray()[0].GetString()); newVector3.y = std::stof(enemyData[c_str].GetArray()[1].GetString()); newVector3.z = std::stof(enemyData[c_str].GetArray()[2].GetString()); return newVector3; }; };

			data->_name = funcLoadString(name);

			data->_behaviorTreeName = DUOLCommon::StringHelper::ToString(funcLoadString(behaviorTreeName));

			data->_fbxModelName = funcLoadString(fbxModelName);

			if (enemyData.HasMember(enemyCode))
				data->_enemyCode = static_cast<EnemyCode>(enemyData[enemyCode].GetInt());

			if (enemyData.HasMember(floatParameterInitData_Str))
			{
				data->_floatParameterInitDatas.reserve(enemyData[floatParameterInitData_Str].Size());

				for (auto& iter : enemyData[floatParameterInitData_Str].GetArray())
					data->_floatParameterInitDatas.push_back({ iter.GetString(),0 });
			}

			if (enemyData.HasMember(floatParameterInitData_Value))
			{
				int i = 0;

				for (auto& iter : enemyData[floatParameterInitData_Value].GetArray())
					data->_floatParameterInitDatas[i++].second = std::stof(iter.GetString());
			}

			if (enemyData.HasMember(boolParameterInitData_Str))
			{
				data->_boolParameterInitDatas.reserve(enemyData[boolParameterInitData_Str].Size());

				for (auto& iter : enemyData[boolParameterInitData_Str].GetArray())
					data->_boolParameterInitDatas.push_back({ iter.GetString(),0 });
			}

			if (enemyData.HasMember(boolParameterInitData_Value))
			{
				int i = 0;

				for (auto& iter : enemyData[boolParameterInitData_Value].GetArray())
				{
					DUOLCommon::tstring boolean_str = iter.GetString();

					if (boolean_str == _T("True") || boolean_str == _T("true") || boolean_str == _T("TRUE"))
						data->_boolParameterInitDatas[i++].second = true;
					else
						data->_boolParameterInitDatas[i++].second = false;
				}
			}

			data->_height = funcLoadFloat(height);

			data->_animControllerName = funcLoadString(animControllerName);

			data->_capsuleCenter = funcLoadVector3(capsuleCenter);

			data->_capsuleRadius = funcLoadFloat(capsuleRadius);

			data->_navBaseOffset = funcLoadVector3(navBaseOffset);

			if (enemyData.HasMember(eventFuncKey))
			{
				data->_eventFuncKey.reserve(enemyData[eventFuncKey].Size());

				for (auto& iter : enemyData[eventFuncKey].GetArray())
					data->_eventFuncKey.push_back(iter.GetString());
			}

			data->_hitFuncKey = funcLoadString(hitFuncKey);

			_enemyDatas.push_back(data);
		}

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
				if (gameObject->GetTag() == TEXT("MainCamera"))
				{
					// Main Camera Controller 는 여기에 달려있습니다.
					_mainCamController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
				}
				else if (gameObject->GetTag() == TEXT("Player"))
				{
					_playerCharacterGameObject = gameObject;
					break;
				}
			}
		}

		return _playerCharacterGameObject;
	}

	DUOLClient::MainCameraController* EnemyManager::GetMainCameraController()
	{
		if (!_mainCamController)
		{
			auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			for (auto gameObject : allGameObjects)
			{
				if (gameObject->GetTag() == TEXT("MainCamera"))
				{
					_mainCamController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
					break;
				}
			}
		}

		return _mainCamController;
	}

	EnemyGroupController* EnemyManager::GetEnemyGroupController(DUOLCommon::tstring name)
	{
		if (_enemyGroupControllers.contains(name))
			return _enemyGroupControllers.at(name);
		return nullptr;
	}


	DUOLGameEngine::AudioClip* EnemyManager::GetAudioClip(EnemyAudioEnum enemySoundEnum)
	{
		return _enemyAudioManager->GetAudioClip(enemySoundEnum);
	}

	void EnemyManager::RetrunAudioClip(EnemyAudioEnum enemySoundEnum)
	{
		_enemyAudioManager->ReturnAudioClip(enemySoundEnum);
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
