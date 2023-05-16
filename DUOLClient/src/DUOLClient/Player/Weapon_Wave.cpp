#include "DUOLClient/Player/Weapon_Wave.h"

#include "DUOLClient/ECS/Component/CharacterBase.h"
#include "DUOLClient/ECS/Component/ParticleData.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLClient/Player/Player.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

namespace DUOLClient
{
	Weapon_Wave::Weapon_Wave(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name)
		: MonoBehaviourBase(owner, name)
	{

	}

	Weapon_Wave::~Weapon_Wave()
	{

	}

	void Weapon_Wave::StartWave(const DUOLMath::Vector3& startPosCenter, const DUOLMath::Vector3& halfExtents,
		const DUOLMath::Vector3& velocity, const DUOLMath::Quaternion& boxWorldRotation)
	{
		GetGameObject()->SetIsActiveSelf(true);

		GetTransform()->SetPosition(startPosCenter, DUOLGameEngine::Space::World);

		GetTransform()->SetRotation(boxWorldRotation, DUOLGameEngine::Space::World);

		_velocity = velocity;

		_boxCollider->SetSize(halfExtents);
	}

	void Weapon_Wave::EndWave()
	{
		GetGameObject()->SetIsActiveSelf(false);
	}

	void Weapon_Wave::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		DUOLGameEngine::GameObject* owner = GetGameObject();

		owner->SetLayer(TEXT("Weapon"));

		_rigidbody = owner->AddComponent<DUOLGameEngine::Rigidbody>();

		_rigidbody->SetIsFreezeXRotation(true);
		_rigidbody->SetIsFreezeYRotation(true);
		_rigidbody->SetIsFreezeZRotation(true);

		_rigidbody->SetMass(1.f);

		_boxCollider = owner->AddComponent<DUOLGameEngine::BoxCollider>();

		_boxCollider->SetIsTrigger(true);
	}

	void Weapon_Wave::OnStart()
	{
		GetGameObject()->SetIsActiveSelf(false);
	}

	void Weapon_Wave::OnFixedUpdate(float fixedTimeStep)
	{
		MonoBehaviourBase::OnFixedUpdate(fixedTimeStep);

		_rigidbody->SetLinearVelocity(_velocity);
	}

	void Weapon_Wave::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		DUOLGameEngine::GameObject* other =
			reinterpret_cast<DUOLGameEngine::GameObject*>(trigger.get()->_other);

		if (other != nullptr && other->GetTag() == TEXT("Enemy"))
		{
			DUOLClient::CharacterBase* enemy = other->GetComponent<DUOLClient::CharacterBase>();

			DUOLGameEngine::Transform* otherTransform = other->GetTransform();

			_player->Attack(enemy, _player->_currentDamage, AttackType::HeavyAttack);

			auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 1.0f);

			if (particleData != nullptr)
			{
				DUOLMath::Vector3 randOffset = DUOLMath::Vector3::Transform(DUOLMath::Vector3(DUOLMath::MathHelper::RandF(-1.f, 1.f), DUOLMath::MathHelper::RandF(0.f, 2.f), DUOLMath::MathHelper::RandF(-0.5f, 0.5f)),
					DUOLMath::Matrix::CreateFromQuaternion(otherTransform->GetWorldRotation()));

				// Enemy의 랜덤 위치로 파티클 이펙트 생성
				particleData->GetTransform()->SetPosition(otherTransform->GetWorldPosition()  + randOffset, DUOLGameEngine::Space::World);
			}

			if (!_player->_isOverdriveSwordMode && !_player->_isOverdriveFistMode)
				_player->_currentOverdrivePoint += OVERDRIVE_POINT_PER_FIST;
		}
	}
}