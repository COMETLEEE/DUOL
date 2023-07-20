#include "DUOLClient/Player/Weapon_AreaWave.h"

#include "DUOLClient/ECS/Component/CharacterBase.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLClient/Player/Player.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Weapon_AreaWave>("Weapon_AreaWave")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_startSize", &DUOLClient::Weapon_AreaWave::_startSize)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("_deltaSizePerSecond", &DUOLClient::Weapon_AreaWave::_deltaSizePerSecond)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	);
}

namespace DUOLClient
{
	Weapon_AreaWave::Weapon_AreaWave(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
		, _rigidbody(nullptr)
		, _boxCollider(nullptr)
		, _player(nullptr)
		, _deltaSizePerSecond(DUOLMath::Vector3(20.f, 0.f, 20.f))
		, _startSize(DUOLMath::Vector3(1.f, 2.f, 1.f))
	{
	}

	Weapon_AreaWave::~Weapon_AreaWave()
	{
	}

	void Weapon_AreaWave::StartAreaWave(const DUOLMath::Vector3& startPosCenter,
		const DUOLMath::Quaternion& boxWorldRotation, float duration)
	{
		GetGameObject()->SetIsActiveSelf(true);

		GetTransform()->SetPosition(startPosCenter, DUOLGameEngine::Space::World);

		GetTransform()->SetRotation(boxWorldRotation, DUOLGameEngine::Space::World);

		_boxCollider->SetSize(_startSize);

		_currentAudioClip = PlayerSoundTable::NONE;

		_mobHitSoundCount = 0;
	}

	void Weapon_AreaWave::StartAreaWave(const DUOLMath::Vector3& startPosCenter,
		const DUOLMath::Quaternion& boxWorldRotation, float duration, PlayerSoundTable soundClip)
	{
		GetGameObject()->SetIsActiveSelf(true);

		GetTransform()->SetPosition(startPosCenter, DUOLGameEngine::Space::World);

		GetTransform()->SetRotation(boxWorldRotation, DUOLGameEngine::Space::World);

		_boxCollider->SetSize(_startSize);

		_currentAudioClip = soundClip;

		_mobHitSoundCount = 0;
	}

	void Weapon_AreaWave::EndAreaWave()
	{
		// 마지막 종료.
		_boxCollider->SetSize(_startSize);

		GetGameObject()->SetIsActiveSelf(false);
	}

	void Weapon_AreaWave::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		DUOLGameEngine::GameObject* owner = GetGameObject();

		owner->SetLayer(TEXT("Weapon"));

		_rigidbody = owner->AddComponent<DUOLGameEngine::Rigidbody>();

		_rigidbody->SetIsFreezeXRotation(true);
		_rigidbody->SetIsFreezeYRotation(true);
		_rigidbody->SetIsFreezeZRotation(true);

		_rigidbody->SetMass(1.f);
		_rigidbody->SetUseGravity(false);

		_boxCollider = owner->AddComponent<DUOLGameEngine::BoxCollider>();

		owner->SetIsActiveSelf(false);
	}

	void Weapon_AreaWave::OnUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnUpdate(deltaTime);

		_boxCollider->SetSize(_boxCollider->GetSize() + _deltaSizePerSecond * deltaTime);
	}

	void Weapon_AreaWave::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		MonoBehaviourBase::OnTriggerEnter(trigger);

		DUOLGameEngine::GameObject* other =
			reinterpret_cast<DUOLGameEngine::GameObject*>(trigger.get()->_other);

		if (other != nullptr && other->GetTag() == TEXT("Enemy"))
		{
			DUOLClient::CharacterBase* enemy = other->GetComponent<DUOLClient::CharacterBase>();

			DUOLGameEngine::Transform* otherTransform = other->GetTransform();

			int mobHitSoundCount = 0;

			if(_player->Attack(enemy, _player->_currentDamage, AttackType::HeavyAttack))
			{
				auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 1.0f);

				if (particleData != nullptr)
				{
					if (_mobHitSoundCount < MAX_SOUND_PLAYER)
					{
							_player->PlaySoundClipInModule(_currentAudioClip, _mobHitSoundCount, false);
							_mobHitSoundCount++;
					}

					DUOLMath::Vector3 randOffset = DUOLMath::Vector3::Transform(DUOLMath::Vector3(DUOLMath::MathHelper::RandF(-1.f, 1.f), DUOLMath::MathHelper::RandF(0.f, 2.f), DUOLMath::MathHelper::RandF(-0.5f, 0.5f)),
						DUOLMath::Matrix::CreateFromQuaternion(otherTransform->GetWorldRotation()));

					// Enemy의 랜덤 위치로 파티클 이펙트 생성
					particleData->GetTransform()->SetPosition(otherTransform->GetWorldPosition() + randOffset, DUOLGameEngine::Space::World);
				}

				if (!_player->_isOverdriveSwordMode && !_player->_isOverdriveFistMode)
					_player->AddOverdrivePoint(_player->_overdrivePointPerFist);
			}
		}
	}
}