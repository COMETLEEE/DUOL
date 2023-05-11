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
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLClient/ECS/Component/ParticleData.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyAirborneCheck.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyParentObjectObserver.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

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
		_attackDelayTime(2.0f),
		_isSuperArmor(false)
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

		ChangeMaterial(EnemyMaterial::NORMAL);

		SetColiiderEnable(true);

		_isSuperArmor = false;

		_currentSuperArmorGauge = 0;

		_isCanSuperArmor = true;
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

			airborneCheckObject->AddComponent<DUOLGameEngine::BoxCollider>();

			_enemyAirborneCheck = airborneCheckObject->AddComponent<EnemyAirborneCheck>();

			_enemyAirborneCheck->Initialize(0.4f, -0.1f);
		}
		// ------------------------ Add & Get Components ---------------------------------

		_attackRange = _enemyData->_attackRange;

		_patrolOffset = _enemyData->_patrolOffset;

		_lookRange = _enemyData->_lookRange;

		_maxSpeed = _enemyData->_maxSpeed;

		_attackDelayTime = _enemyData->_attackDelayTime;

		_attackCancelTime = _enemyData->_attackCancelTime;

		_chaseRange = _enemyData->_chaseRange;

		_superArmorTime = _enemyData->_superArmorTime;

		_maxSuperArmorGauge = _enemyData->_superArmorMaxGauge;

		_superArmorCoolTime = _enemyData->_superArmorCoolTime;

		_animator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(_enemyData->_animControllerName));

		_rigidbody->SetUseGravity(true);
		_rigidbody->SetIsKinematic(true);
		_rigidbody->SetIsFreezeXRotation(true);
		_rigidbody->SetIsFreezeYRotation(true);
		_rigidbody->SetIsFreezeZRotation(true);

		_parentCapsuleCollider->SetHeight(_enemyData->_height);

		_parentCapsuleCollider->SetCenter(DUOLMath::Vector3(_enemyData->_capsuleCenter));

		_parentCapsuleCollider->SetRadius(_enemyData->_capsuleRadius);

		_capsuleCollider->SetHeight(_enemyData->_height);

		_capsuleCollider->SetCenter(DUOLMath::Vector3(_enemyData->_capsuleCenter));

		_capsuleCollider->SetRadius(_enemyData->_capsuleRadius);

		_navMeshAgent->SetBaseOffset(_enemyData->_navBaseOffset);

		_navMeshAgent->SetMaxSpeed(_maxSpeed);

		_navMeshAgent->SetMaxAcceleration(_enemyData->_maxAcceleration);

		_parentObserver->_enemyOwner = this;

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

	void Enemy::SetIsHit(bool isHit)
	{
		_isHit = isHit;
	}

	void Enemy::EnemyAddEventFunc(const DUOLCommon::tstring& eventName, std::function<void()> functor)
	{
		_parentObserver->AddEventFunction(eventName, functor);
	}

	void Enemy::SetEnemyHitFunc(
		std::function<void(DUOLClient::Enemy*, CharacterBase*, float, AttackType)> func)
	{
		_hitFunc = func;
	}

	void Enemy::SetSuperArmor(bool isSuperArmor)
	{
		_isSuperArmor = isSuperArmor;
	}

	void Enemy::AddSuperArmorGauge(float addGauge)
	{
		_currentSuperArmorGauge += addGauge;
	}

	float Enemy::GetCurrentSuperArmorGauge() const
	{
		return _currentSuperArmorGauge;
	}

	float Enemy::GetMaxSuperArmorGauge() const
	{
		return _maxSuperArmorGauge;
	}

	float Enemy::GetSuperArmorTime() const
	{
		return _superArmorTime;
	}

	const EnemyData* Enemy::GetEnemyData()
	{
		return _enemyData;
	}


	bool Enemy::GetIsAirBorne()
	{
		return _enemyAirborneCheck->GetIsAirborne();
	}

	bool Enemy::GetIsSuperArmor()
	{
		return _isSuperArmor;
	}

	void Enemy::SetIsCanSuperArmor(bool isBool)
	{
		_isCanSuperArmor = isBool;

		if (!isBool)
		{
			auto lamdafunc = [](Enemy* enemy, float time)->DUOLGameEngine::CoroutineHandler
			{
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(time);

				enemy->SetIsCanSuperArmor(true);
			};
			std::function<DUOLGameEngine::CoroutineHandler()> func = std::bind(lamdafunc, this, _superArmorCoolTime);

			StartCoroutine(func);
		}
	}

	bool Enemy::GetIsCanSuperArmor()
	{
		return _isCanSuperArmor;
	}

	AI_EnemyBasic* Enemy::GetAIController()
	{
		return _ai;
	}

	DUOLGameEngine::Rigidbody* Enemy::GetRigidbody() const
	{
		return _rigidbody;
	}

	DUOLGameEngine::Animator* Enemy::GetAnimator() const
	{
		return _animator;
	}

	DUOLGameEngine::NavMeshAgent* Enemy::GetNavMeshAgent() const
	{
		return _navMeshAgent;
	}

	HitEnum Enemy::GetHitEnum() const
	{
		return _hitEnum;
	}

	void Enemy::SetHitEnum(HitEnum hitEnum)
	{
		_hitEnum = hitEnum;
	}

	void Enemy::ChangeMaterial(EnemyMaterial enemyMat)
	{
		if (_currentMaterial == enemyMat)
			return;

		_skinnedMeshRenderer->DeleteAllMaterial();

		switch (enemyMat)
		{
		case EnemyMaterial::WHITE:
			for (auto& iter : _originMaterials)
				_skinnedMeshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("SkinnedDefault_WHITE")));
			break;

		case EnemyMaterial::RED:
			for (auto& iter : _originMaterials)
				_skinnedMeshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("SkinnedDefault_RED")));
			break;

		case EnemyMaterial::DIE:
		{
			for (auto& iter : _originMaterials)
				_skinnedMeshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(iter->GetName() + _T("PaperBurn")));
		}
		break;

		case EnemyMaterial::NORMAL:
			for (auto& iter : _originMaterials)
				_skinnedMeshRenderer->AddMaterial(iter);
			break;

		default:
			break;
		}

		_currentMaterial = enemyMat;

	}

	void Enemy::ChangeMaterialOnHit()
	{
		std::function<DUOLGameEngine::CoroutineHandler(void)> func =
			std::bind([](Enemy* enemy)->DUOLGameEngine::CoroutineHandler
				{
					enemy->ChangeMaterial(EnemyMaterial::RED);

					co_yield std::make_shared<DUOLGameEngine::WaitForFrames>(1);

					enemy->ChangeMaterial(EnemyMaterial::WHITE);

					co_yield std::make_shared<DUOLGameEngine::WaitForFrames>(1);

					enemy->ChangeMaterial(EnemyMaterial::NORMAL);
				},
				this);

		StartCoroutine(func);


		std::function<DUOLGameEngine::CoroutineHandler(void)> setSpeed =
			std::bind([](Enemy* enemy)->DUOLGameEngine::CoroutineHandler
				{
					enemy->GetAnimator()->SetSpeed(0.2f);
					co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(0.2f);
					enemy->GetAnimator()->SetSpeed(1.0f);
				},
				this);

		StartCoroutine(setSpeed);
	}

	void DUOLClient::Enemy::SetNavOnRigidbodyOff()
	{
		if (_navMeshAgent->GetIsEnabled()) return;

		_navMeshAgent->SetIsEnabled(true);

		_rigidbody->SetIsKinematic(true);
	}
	void DUOLClient::Enemy::SetNavOffRigidbodyOn()
	{
		if (!_navMeshAgent->GetIsEnabled()) return;

		_navMeshAgent->SetIsEnabled(false);

		_rigidbody->SetIsKinematic(false);

		_rigidbody->SetLinearVelocity(DUOLMath::Vector3(0, 0, 0));
	}

	DUOLGameEngine::GameObject* Enemy::GetTarget() const
	{
		return _target;
	}

	DUOLGameEngine::Transform* Enemy::GetParentTransform() const
	{
		return _transform;
	}

	void Enemy::SetColiiderEnable(bool isBool)
	{
		if (!isBool)
			SetNavOnRigidbodyOff();
		_capsuleCollider->SetIsEnabled(isBool);
		_parentCapsuleCollider->SetIsEnabled(isBool);
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
		_hitFunc(this, other, damage, attackType);
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
