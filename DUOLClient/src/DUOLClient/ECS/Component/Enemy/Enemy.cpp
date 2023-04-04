#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

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
		_capsuleCollider(nullptr)
	{
	}

	void Enemy::SetEnemyCode(EnemyCode enemyCode)
	{
		_enemyData = EnemyManager::GetInstance()->GetEnemy(enemyCode);

		// ------------------------ Add & Get Components ---------------------------------
		if (!_navMeshAgent)
			_navMeshAgent = GetGameObject()->AddComponent<DUOLGameEngine::NavMeshAgent>();

		if (!_animator)
			_animator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

		if (!_capsuleCollider)
			_capsuleCollider = GetGameObject()->AddComponent<DUOLGameEngine::CapsuleCollider>();
		// ------------------------ Add & Get Components ---------------------------------

		SetHP(_enemyData->_maxHp);

		SetDamage(_enemyData->_damage);

		_attackRange = _enemyData->_attackRange;

		_patrolOffset = _enemyData->_patrolOffset;

		_lookRange = _enemyData->_lookRange;

		_maxSpeed = _enemyData->_maxSpeed;

		_animator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(_enemyData->_animControllerName));

		_capsuleCollider->SetCenter(DUOLMath::Vector3(_enemyData->_capsuleCenter));

		_capsuleCollider->SetHeight(_enemyData->_height);

		_navMeshAgent->SetBaseOffset(_enemyData->_navBaseOffset);

		_navMeshAgent->SetMaxSpeed(_maxSpeed);
	}

	const EnemyData* Enemy::GetEnemyData()
	{
		return _enemyData;
	}


	void Enemy::Attack(CharacterBase* target, float damage)
	{
		target->OnHit(this, damage);
	}

	void Enemy::OnHit(CharacterBase* other, float damage)
	{
		_isHit = true;
		_hp -= damage;
	}

	void Enemy::OnAwake()
	{
	}
}
