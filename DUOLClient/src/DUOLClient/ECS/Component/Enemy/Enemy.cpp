#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLCommon/MetaDataType.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"

#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLClient/ECS/Component/ParticleData.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyAirborneCheck.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyParentObjectObserver.h"
#include "DUOLGameEngine/ECS/Object/Material.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Enemy>("Enemy")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_isHit",&DUOLClient::Enemy::_isHit)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool))
	.property("_attackRange",&DUOLClient::Enemy::_attackRange)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float))
	.property("_patrolOffset",&DUOLClient::Enemy::_patrolOffset)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float))
	.property("_lookRange",&DUOLClient::Enemy::_lookRange)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float));

}

namespace DUOLClient
{
	Enemy::Enemy(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		CharacterBase(owner, name),
		_isHit(false),
		_attackRange(0),
		_patrolOffset(0),
		_lookRange(0),
		_animator(nullptr),
		_navMeshAgent(nullptr),
		_capsuleCollider(nullptr),
		_rigidbody(nullptr),
		_transform(nullptr),
		_enemyAirborneCheck(nullptr),
		_parentCapsuleCollider(nullptr),
		_parentObserver(nullptr),
		_skinnedMeshRenderer(nullptr),
		_isOriginMaterial(true),
		_attackDelayTime(2.0f)
	{
		_hitEnum = static_cast<HitEnum>(DUOLMath::MathHelper::Rand(0, 1));
	}

	Enemy::~Enemy()
	{

	}

	void Enemy::InitializeData()
	{
		_ai->SetAnimConditionReset();

		_animator->SetFloat(TEXT("RandOffset"), DUOLMath::MathHelper::RandF(0, 1.0f));

		SetHP(_enemyData->_maxHp);

		SetDamage(_enemyData->_damage);

		_rigidbody->SetUseGravity(true);

		_navMeshAgent->SetMaxSpeed(_maxSpeed);

		_animator->SetSpeed(1.0f);

		ChangeMaterial(false);
	}

	void Enemy::SetPosition(DUOLMath::Vector3 pos)
	{
		_navMeshAgent->SetIsEnabled(false);

		_transform->SetPosition(pos);

		_navMeshAgent->SetIsEnabled(true);
	}

	void Enemy::SetPosition(const float& x, const float& y, const float& z)
	{
		SetPosition(DUOLMath::Vector3(x, y, z));
	}

	void Enemy::SetEnemyCode(EnemyData* enemyData)
	{
		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		_enemyData = enemyData;

		GetGameObject()->SetLayer(TEXT("Enemy"));
		// ------------------------ Add & Get Components ---------------------------------

		if (!_parentGameObject) // 부모 오브젝트에서 물리와 트랜스 폼 관련 처리를 한다.
		{
			_parentGameObject = GetTransform()->GetParent()->GetGameObject();
			_parentGameObject->SetLayer(TEXT("EnemyRigidbody"));
		}

		if (!_navMeshAgent)
			_navMeshAgent = _parentGameObject->AddComponent<DUOLGameEngine::NavMeshAgent>();

		if (!_animator)
			_animator = _parentGameObject->GetComponent<DUOLGameEngine::Animator>();

		if (!_transform)
			_transform = _parentGameObject->GetTransform();

		if (!_rigidbody)
			_rigidbody = _parentGameObject->AddComponent<DUOLGameEngine::Rigidbody>();

		if (!_parentCapsuleCollider)
			_parentCapsuleCollider = _parentGameObject->AddComponent<DUOLGameEngine::CapsuleCollider>();

		if (!_capsuleCollider)
			_capsuleCollider = GetGameObject()->AddComponent<DUOLGameEngine::CapsuleCollider>();

		if (!_parentObserver)
			_parentObserver = _parentGameObject->AddComponent<EnemyParentObjectObserver>();

		if (!_enemyAirborneCheck)
		{
			const auto airborneCheckObject = scene->CreateEmpty();

			airborneCheckObject->GetTransform()->SetParent(GetTransform());

			_enemyAirborneCheck = airborneCheckObject->AddComponent<EnemyAirborneCheck>();

			_enemyAirborneCheck->Initialize(0.4f, 0);
		}
		// ------------------------ Add & Get Components ---------------------------------

		_attackRange = _enemyData->_attackRange;

		_patrolOffset = _enemyData->_patrolOffset;

		_lookRange = _enemyData->_lookRange;

		_maxSpeed = _enemyData->_maxSpeed;

		_attackDelayTime = _enemyData->_attackDelayTime;

		_attackCancelTime = _enemyData->_attackCancelTime;

		_chaseRange = _enemyData->_chaseRange;

		_animator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(_enemyData->_animControllerName));

		_rigidbody->SetUseGravity(true);
		_rigidbody->SetIsKinematic(true);
		_rigidbody->SetIsFreezeXRotation(true);
		_rigidbody->SetIsFreezeYRotation(true);
		_rigidbody->SetIsFreezeZRotation(true);

		_parentCapsuleCollider->SetCenter(DUOLMath::Vector3(_enemyData->_capsuleCenter));

		_capsuleCollider->SetHeight(_enemyData->_height);

		_capsuleCollider->SetCenter(DUOLMath::Vector3(_enemyData->_capsuleCenter));

		_capsuleCollider->SetHeight(_enemyData->_height);

		_navMeshAgent->SetBaseOffset(_enemyData->_navBaseOffset);

		_navMeshAgent->SetMaxSpeed(_maxSpeed);

		_parentObserver->_enemyOwner = this;

		for (auto& iter : _enemyData->_attackFuncs)
		{
			_parentObserver->AddEventFunction(iter.first, std::bind(iter.second, this));
		}

		auto test = [&]()
		{
			_animator->SetSpeed(0);
		};

		_parentObserver->AddEventFunction(TEXT("Die"), test);

		GetGameObject()->SetName(_enemyData->_name);

		if (!_ai)
			_ai = GetGameObject()->GetComponent<AI_EnemyBasic>();

		_ai->Initialize();


		/// TEST Code
		if (!_skinnedMeshRenderer)
		{
			for (auto& iter : _parentGameObject->GetTransform()->GetChildGameObjects())
			{
				_skinnedMeshRenderer = iter->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>();
				if (_skinnedMeshRenderer) break;
			}

			_originMaterials = _skinnedMeshRenderer->GetMaterials();
		}

		InitializeData();
	}

	const EnemyData* Enemy::GetEnemyData()
	{
		return _enemyData;
	}


	bool Enemy::GetIsAirBorne()
	{
		return _enemyAirborneCheck->GetIsAirborne();
	}

	AI_EnemyBasic* Enemy::GetAIController()
	{
		return _ai;
	}

	void Enemy::ChangeMaterial(bool isDie)
	{
		if (isDie)
		{
			if (_isOriginMaterial)
			{
				_skinnedMeshRenderer->DeleteAllMaterial();
				for (auto& iter : _originMaterials)
				{
					_skinnedMeshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(iter->GetName() + _T("PaperBurn")));
				}
			}
			_isOriginMaterial = false;
		}
		else
		{
			if (!_isOriginMaterial)
			{
				_skinnedMeshRenderer->DeleteAllMaterial();
				for (auto& iter : _originMaterials)
				{
					_skinnedMeshRenderer->AddMaterial(iter);
				}
			}
			_isOriginMaterial = true;
		}
	}

	void Enemy::OnEnable()
	{
		// 임시 코드
		if (_skinnedMeshRenderer)
		{
			_skinnedMeshRenderer->GetSkinnedMeshInfo().SetOffset(0);
		}
	}

	void Enemy::Attack(CharacterBase* target, float damage, AttackType attackType)
	{
		target->OnHit(this, damage, attackType);
	}

	void Enemy::OnHit(CharacterBase* other, float damage, AttackType attackType)
	{
		if (!GetIsDie())
			_ai->SetAnimConditionReset();

		_animator->SetFloat(TEXT("RandOffset"), DUOLMath::MathHelper::RandF(0, 0.5f));

		_isHit = true;

		_hp -= damage;

		_navMeshAgent->SetIsEnabled(false); // 바닥에 닿았을 때 다시 켜줘야한다.

		_rigidbody->SetIsKinematic(false);

		switch (_hitEnum)
		{
		case HitEnum::Front:
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Front"), true);
			_hitEnum = HitEnum::Back;
			break;
		case HitEnum::Back:
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Back"), true);
			_hitEnum = HitEnum::Front;
			break;
		default:
			break;
		}

		//auto dir = GetTransform()->GetWorldPosition() - other->GetTransform()->GetWorldPosition();

		//dir.Normalize();

		//auto height = DUOLMath::MathHelper::RandF(2.0f, 10.0f);

		//dir = dir * 4 + DUOLMath::Vector3(0, height, 0);

		//_rigidbody->AddImpulse(dir * 5.0f);

		//if (GetIsAirBorne()) // 공중에 떠 있다면 무조건 공중 히트 애니메이션으로..!
		//{
		//	auto rand = static_cast<HitEnum>(DUOLMath::MathHelper::Rand(
		//		static_cast<int>(HitEnum::Air_1),
		//		static_cast<int>(HitEnum::Air_3)));
		//	switch (rand)
		//	{
		//	case HitEnum::Air_1:

		//		_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_1"), true);
		//		break;
		//	case HitEnum::Air_2:
		//		_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_2"), true);
		//		break;
		//	case HitEnum::Air_3:
		//		_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_3"), true);
		//		break;

		//	default:
		//		break;
		//	}
		//}
		//else
		//{
		//	if (height > 5.0f)
		//	{
		//		auto rand = static_cast<HitEnum>(DUOLMath::MathHelper::Rand(2, 4));
		//		switch (rand)
		//		{
		//		case HitEnum::Air_1:
		//			_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_1"), true);
		//			break;
		//		case HitEnum::Air_2:
		//			_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_2"), true);
		//			break;
		//		case HitEnum::Air_3:
		//			_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_3"), true);
		//			break;

		//		default:
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		switch (_hitEnum)
		//		{
		//		case HitEnum::Front:
		//			_ai->GetAnimator()->SetBool(TEXT("IsHit_Front"), true);
		//			_hitEnum = HitEnum::Back;
		//			break;
		//		case HitEnum::Back:
		//			_ai->GetAnimator()->SetBool(TEXT("IsHit_Back"), true);
		//			_hitEnum = HitEnum::Front;
		//			break;
		//		default:
		//			break;
		//		}
		//	}
		//}



		if (GetIsDie())
		{
			ChangeMaterial(true);
		}
	}

	void Enemy::OnAwake()
	{

	}

	void Enemy::OnUpdate(float deltaTime)
	{
		if (GetIsDie())
		{
			constexpr float speed = 0.2f;
			_skinnedMeshRenderer->GetSkinnedMeshInfo().SetOffset(_skinnedMeshRenderer->GetSkinnedMeshInfo().GetOffset() + speed * deltaTime);

		}
	}
}
