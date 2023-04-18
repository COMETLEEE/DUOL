#pragma once

#include "DUOLClient/ECS/Component/Projectile.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

namespace DUOLClient
{

	inline void Attack_Close(DUOLClient::Enemy* enemy)
	{
		std::vector<DUOLPhysics::RaycastHit> boxcastHits;

		const auto pos = enemy->GetTransform()->GetWorldPosition();

		const auto look = enemy->GetTransform()->GetLook();

		const DUOLMath::Quaternion boxRotation = DUOLMath::Quaternion::Identity;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapBoxAll(pos + look * 3, DUOLMath::Vector3(2, 2, 2), boxRotation, boxcastHits))
		{
			for (auto hited : boxcastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				if (gameObject->GetTag() == TEXT("Player"))
				{
					auto player = gameObject->GetComponent<DUOLClient::CharacterBase>();

					enemy->Attack(player, enemy->GetDamage(), AttackType::HeavyAttack);
				}
			}
		}
	}

	inline void Attack_Far(DUOLClient::Enemy* enemy)
	{
		const auto projectile = EnemyManager::GetInstance()->Pop<Projectile>(TEXT("Projectile"), 2.0f);

		if (!projectile)
			return;

		auto tr = enemy->GetTransform();

		projectile->GetTransform()->SetPosition(tr->GetWorldPosition() + DUOLMath::Vector3::Up * 1.5f);

		projectile->FireProjectile(tr->GetParent()->GetLook(), 20, enemy->GetGameObject(), 5, TEXT("Player"), true, 0.1f);
	}
}