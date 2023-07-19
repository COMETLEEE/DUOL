#include "DUOLClient/ECS/Component/Enemy/BossEnemy_Weapon_AreaWave.h"
#include "DUOLClient/ECS/Component/CharacterBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/SphereCollider.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include <rttr/registration>

#include "DUOLClient/Manager/ParticleManager.h"


using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::BossEnemy_Weapon_AreaWave>("BossEnemy_Weapon_AreaWave")
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
	BossEnemy_Weapon_AreaWave::BossEnemy_Weapon_AreaWave(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name),
		_owner(nullptr),
		_rigidbody(nullptr),
		_sphereCollider(nullptr),
		_startRadius(),
		_endRadius(),
		_waveTime(0),
		_timer(0),
		_startPos()
	{

	}
	BossEnemy_Weapon_AreaWave::~BossEnemy_Weapon_AreaWave()
	{
	}

	void BossEnemy_Weapon_AreaWave::SetOwner(DUOLClient::Enemy* owner)
	{
		_owner = owner;
		_owner->AddParameter(TEXT("AreaWave"), static_cast<void*>(this));
	}

	void BossEnemy_Weapon_AreaWave::StartWave(
		const DUOLMath::Vector3& startPosCenter,
		const float& startRadius,
		const float& endRadius,
		float waveTime, float damage, float downPoint)
	{
		_startRadius = startRadius;

		_endRadius = endRadius;

		_waveTime = waveTime;

		_timer = 0;

		_startPos = startPosCenter;

		_damage = damage;

		_downPoint = downPoint;

		_enterCheck.clear();

		GetGameObject()->SetIsActiveSelf(true);
	}

	void BossEnemy_Weapon_AreaWave::EndWave()
	{
		GetGameObject()->SetIsActiveSelf(false);
	}

	void BossEnemy_Weapon_AreaWave::OnAwake()
	{
		EndWave();
	}

	void BossEnemy_Weapon_AreaWave::OnStart()
	{

	}

	void BossEnemy_Weapon_AreaWave::OnUpdate(float deltaTime)
	{
		_timer += deltaTime;

		float t = _timer / _waveTime;

		float radius = std::lerp(_startRadius, _endRadius, std::min(t, 1.0f));


		std::vector<DUOLPhysics::RaycastHit> hit;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapSphereAll(_startPos, radius, hit))
		{
			for (auto hited : hit)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				if (_enterCheck.contains(gameObject)) continue;

				if (gameObject == _owner->GetGameObject()) continue;

				if (gameObject->GetTag() == TEXT("Player"))
				{
					auto player = gameObject->GetComponent<DUOLClient::CharacterBase>();

					if (player->GetIsInvincible())
						continue;

					if (_owner->Attack(player, _damage, AttackType::HeavyAttack, _downPoint))
					{
						_enterCheck.insert(gameObject);

						auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 0.7f);

						DUOLMath::Vector3 randYOffset = DUOLMath::Vector3(0, DUOLMath::MathHelper::RandF(1.0f, 1.5f), 0);

						particleRenderer->GetTransform()->SetPosition(player->GetTransform()->GetWorldPosition() + randYOffset);
					}

				}
				else if (gameObject->GetTag() == TEXT("Enemy"))
				{
					auto enemyOther = gameObject->GetComponent<DUOLClient::CharacterBase>();

					if (enemyOther->GetIsInvincible())
						continue;

					if (_owner->Attack(enemyOther, 0, AttackType::HeavyAttack))
					{
						_enterCheck.insert(gameObject);

						auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 0.7f);

						DUOLMath::Vector3 randYOffset = DUOLMath::Vector3(0, DUOLMath::MathHelper::RandF(1.0f, 1.5f), 0);

						particleRenderer->GetTransform()->SetPosition(enemyOther->GetTransform()->GetWorldPosition() + randYOffset);

					}
				}
			}
		}


		if (t > 1.0f)
			EndWave();
	}

}
