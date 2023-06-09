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
#include "DUOLGameEngine/ECS/Object/Mesh.h"
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
	);

}

namespace DUOLClient
{
	Enemy::Enemy(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		CharacterBase(owner, name),
		_animator(nullptr),
		_navMeshAgent(nullptr),
		_capsuleCollider(nullptr),
		_rigidbody(nullptr),
		_transform(nullptr),
		_enemyAirborneCheck(nullptr),
		_parentCapsuleCollider(nullptr),
		_parentObserver(nullptr),
		_skinnedMeshRenderer(nullptr)
	{
		_hitEnum = static_cast<HitEnum>(DUOLMath::MathHelper::Rand(0, 1));
	}

	Enemy::~Enemy()
	{

	}

	void Enemy::InitializeData()
	{
		_ai->Reset();

		_animator->SetFloat(TEXT("RandOffset"), DUOLMath::MathHelper::RandF(0, 1.0f));

		for (const auto& iter : _enemyData->_boolParameterInitDatas)
			SetParameter(iter.first, iter.second);

		for (const auto& iter : _enemyData->_floatParameterInitDatas)
			SetParameter(iter.first, iter.second);

		SetHP(GetParameter<float>(TEXT("MaxHp")));

		SetDamage(GetParameter<float>(TEXT("Damage")));

		_rigidbody->SetUseGravity(true);

		_navMeshAgent->SetMaxSpeed(GetParameter<float>(TEXT("MaxSpeed")));

		_navMeshAgent->SetMaxAcceleration(GetParameter<float>(TEXT("MaxAcceleration")));

		_animator->SetSpeed(1.0f);

		_skinnedMeshRenderer->SetRimPower(0.5f);

		ChangeMaterial(EnemyMaterial::APPEAR);

		SetColiiderEnable(true);
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

		if (!_parentGameObject) // �θ� ������Ʈ���� ������ Ʈ���� �� ���� ó���� �Ѵ�.
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
		{
			_rigidbody = _parentGameObject->AddComponent<DUOLGameEngine::Rigidbody>();

			// TODO : �� ������ ���ƾ� �� ���ư��� .. �� �𸣰ڵ� ..
			_rigidbody->SetMass(1.f);

			// TODO : ���ʹ� Ű�� �°� �������� �ʿ䰡 �������� ?
			_rigidbody->SetCenterOfMass(DUOLMath::Vector3(0.f, 1.f, 0.f));
		}

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

		_skinnedMeshRenderer->SetRimColor(DUOLMath::Vector3(1.0f, 0, 0));

		InitializeData();
	}

	void Enemy::EnemyAddEventFunc(const DUOLCommon::tstring& eventName, std::function<void()> functor)
	{
		_parentObserver->AddEventFunction(eventName, functor);
	}

	void Enemy::SetEnemyHitFunc(
		std::function<bool(DUOLClient::Enemy*, CharacterBase*, float, AttackType)> func)
	{
		_hitFunc = func;
	}

	DUOLGameEngine::SkinnedMeshRenderer* Enemy::GetSkinnedMeshRenderer()
	{
		return _skinnedMeshRenderer;
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
			_skinnedMeshRenderer->GetSkinnedMeshInfo().SetPaperBurnColor(DUOLMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), DUOLMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

			SetParameter<float>(TEXT("PaperBurnOffset"), 0);
			_skinnedMeshRenderer->GetSkinnedMeshInfo().SetOffset(0);
		}
		break;
		case EnemyMaterial::APPEAR:
		{
			for (auto& iter : _originMaterials)
				_skinnedMeshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(iter->GetName() + _T("PaperBurn_DownUp")));

			_skinnedMeshRenderer->GetSkinnedMeshInfo().SetPaperBurnColor(DUOLMath::Vector4(0.3f, 1.0f, 0.8f, 1.0f), DUOLMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

			SetParameter<float>(TEXT("PaperBurnOffset"), 0);
			_skinnedMeshRenderer->GetSkinnedMeshInfo().SetOffset(0);
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
		SetNavEnable(true);
		if (!_rigidbody->GetIsKinematic())
			_rigidbody->SetIsKinematic(true);

	}
	void DUOLClient::Enemy::SetNavOffRigidbodyOn()
	{
		SetNavEnable(false);

		if (_rigidbody->GetIsKinematic())
		{
			_rigidbody->SetIsKinematic(false);

			_rigidbody->SetLinearVelocity(DUOLMath::Vector3(0, 0, 0));
		}
	}

	void Enemy::SetNavEnable(bool isBool)
	{
		if (_navMeshAgent->GetIsEnabled() != isBool)
			_navMeshAgent->SetIsEnabled(isBool);
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
		_rigidbody->SetIsKinematic(!isBool);

		if (!isBool)
			_rigidbody->SetLinearVelocity(DUOLMath::Vector3(0, 0, 0));

		_capsuleCollider->SetIsEnabled(isBool);
		_parentCapsuleCollider->SetIsEnabled(isBool);
	}

	void Enemy::LerpLookTarget()
	{
		DUOLMath::Vector3 myPosition = GetParentTransform()->GetWorldPosition();

		DUOLMath::Vector3 targetPos = GetTarget()->GetTransform()->GetWorldPosition();

		targetPos.y = myPosition.y;

		auto look = targetPos - myPosition;

		look.Normalize();

		look = DUOLMath::Vector3::Lerp(GetParentTransform()->GetLook(), look, 10.0f * DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());

		GetParentTransform()->LookAt(myPosition + look);
	}

	void Enemy::PushedOut()
	{
		if (!_navMeshAgent->GetIsEnabled()) return;
		if (!GetParameter<bool>(TEXT("IsPushable"))) return;

		DUOLMath::Vector3 myPosition = GetParentTransform()->GetWorldPosition();

		DUOLMath::Vector3 playerPos = GetTarget()->GetTransform()->GetWorldPosition();

		playerPos.y = myPosition.y;

		auto look = myPosition - playerPos;

		look.Normalize();

		_navMeshAgent->SetDestination(myPosition + look);
	}

	void Enemy::OnEnable()
	{
	}

	bool Enemy::Attack(CharacterBase* target, float damage, AttackType attackType)
	{
		return target->OnHit(this, damage, attackType);
	}

	bool Enemy::OnHit(CharacterBase* other, float damage, AttackType attackType)
	{
		return _hitFunc(this, other, damage, attackType);
	}

	bool Enemy::GetIsInvincible()
	{
		if (GetIsDie())
			return true;
		if (_ai->GetAnimator()->GetBool(TEXT("IsAirBorne")))
			return true;

		return false;
	}

	void Enemy::OnAwake()
	{

	}

	void Enemy::OnUpdate(float deltaTime)
	{
		//DUOL_TRACE(DUOL_CONSOLE, "CurrentState : {0}", DUOLCommon::StringHelper::ToString(_animator->GetCurrentStateName()));

		switch (_currentMaterial)
		{
		case EnemyMaterial::DIE:
		{
			constexpr float speed = 0.2f;

			float offset = GetParameter<float>(TEXT("PaperBurnOffset"));

			offset += speed * deltaTime;

			SetParameter(TEXT("PaperBurnOffset"), offset);

			_skinnedMeshRenderer->GetSkinnedMeshInfo().SetOffset(offset);
		}
		break;
		case EnemyMaterial::APPEAR:
		{
			const float startHeight = GetTransform()->GetWorldPosition().y;

			const float maxHeight = GetTransform()->GetWorldPosition().y + _skinnedMeshRenderer->GetSkinnedMesh()->GetHeight();

			const float speed = 0.2f * (maxHeight - startHeight);

			float offset = GetParameter<float>(TEXT("PaperBurnOffset"));

			offset += speed * deltaTime;

			SetParameter(TEXT("PaperBurnOffset"), offset);

			_skinnedMeshRenderer->GetSkinnedMeshInfo().SetOffset(startHeight + offset);

			if (_skinnedMeshRenderer->GetSkinnedMeshInfo().GetOffset() >= maxHeight)
				ChangeMaterial(EnemyMaterial::NORMAL);
		}
		break;

		default:
			break;
		}
	}
}
