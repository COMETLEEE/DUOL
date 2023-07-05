#include "DUOLClient/ECS/Component/Projectile.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/CharacterBase.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/SphereCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Projectile>("Projectile")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_speed",&DUOLClient::Projectile::_speed)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float))
	.property("_damage",&DUOLClient::Projectile::_damage)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float))
	.property("_isGravity",&DUOLClient::Projectile::_isGravity)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool))
	.property("_currentVelocity",&DUOLClient::Projectile::_currentVelocity)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3));
}

namespace DUOLClient
{
	Projectile::Projectile(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name),
		_damage(0), _speed(0),
		_transform(nullptr),
		_targetTag(TEXT("")),
		_isGravity(true),
		_currentVelocity(),
		_radius(0),
		_projectileOwner(nullptr)
	{
	}

	void Projectile::FireProjectile(
		DUOLMath::Vector3 direction,
		float speed,
		DUOLGameEngine::GameObject* owner
		, float damage,
		DUOLCommon::tstring targetTag, bool isGravity, float radius,
		bool isDistroy)
	{
		GetGameObject()->SetIsActiveSelf(true);

		_speed = speed;

		_damage = damage;

		_targetTag = targetTag;

		_isGravity = isGravity;

		_currentVelocity = direction * _speed;

		_radius = radius;

		_projectileOwner = owner;

		_collider->SetRadius(_radius);

		_rigidbody->SetUseGravity(_isGravity);

		_isDestory = isDistroy;
	}

	void Projectile::OnStart()
	{
		GetGameObject()->SetLayer(TEXT("EnemyProjectile"));

		_collider = GetGameObject()->GetComponent<DUOLGameEngine::SphereCollider>();

		_rigidbody = GetGameObject()->GetComponent<DUOLGameEngine::Rigidbody>();

		if (!_collider)
			_collider = GetGameObject()->AddComponent<DUOLGameEngine::SphereCollider>();

		if (!_rigidbody)
			_rigidbody = GetGameObject()->AddComponent<DUOLGameEngine::Rigidbody>();

		if (!_transform)
			_transform = GetTransform();

		_collider->SetIsTrigger(true);
		_rigidbody->SetIsFreezeXRotation(true);
		_rigidbody->SetIsFreezeYRotation(true);
		_rigidbody->SetIsFreezeZRotation(true);
	}

	void Projectile::OnUpdate(float deltaTime)
	{
		_transform->SetPosition(_currentVelocity * deltaTime + _transform->GetWorldPosition());
	}

	void Projectile::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		auto other = static_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		if (other == _projectileOwner)
			return;

		if (other->GetTag() == _targetTag)
		{
			other->GetComponent<CharacterBase>()->OnHit(nullptr, _damage, AttackType::LightAttack);
		}

		if (_isDestory)
		{
			GetGameObject()->SetIsActiveSelf(false);
		}

		auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 1.0f);

		if (particleData == nullptr)
			return;

		auto tr = particleData->GetTransform();
		tr->SetPosition(_transform->GetWorldPosition());
	}
}