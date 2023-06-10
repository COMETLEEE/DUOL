#pragma once

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/ECS/Component/Projectile.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{

	inline void Attack_Close(DUOLClient::Enemy* enemy)
	{
		std::vector<DUOLPhysics::RaycastHit> boxcastHits;

		const auto pos = enemy->GetTransform()->GetWorldPosition();

		const auto look = enemy->GetTransform()->GetLook();

		const DUOLMath::Quaternion boxRotation = DUOLMath::Quaternion::Identity;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapBoxAll(pos + look * 2, DUOLMath::Vector3(2, 2, 2), boxRotation, boxcastHits))
		{
			for (auto hited : boxcastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				if (gameObject->GetTag() == TEXT("Player"))
				{
					auto player = gameObject->GetComponent<DUOLClient::CharacterBase>();

					if (enemy->Attack(player, enemy->GetDamage(), AttackType::HeavyAttack))
					{
						auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 0.7f);

						DUOLMath::Vector3 randYOffset = DUOLMath::Vector3(0, DUOLMath::MathHelper::RandF(1.0f, 1.5f), 0);

						particleRenderer->GetTransform()->SetPosition(player->GetTransform()->GetWorldPosition() + randYOffset);
					}
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

		projectile->FireProjectile(tr->GetParent()->GetLook(), 20, enemy->GetGameObject(), enemy->GetDamage(), TEXT("Player"), true, 0.1f);
	}

	inline void Attack_Charge(DUOLClient::Enemy* enemy)
	{

		const auto projectile = EnemyManager::GetInstance()->Pop<Projectile>(TEXT("NoneMeshProjectile"), 1.2f);

		if (!projectile)
			return;

		auto tr = enemy->GetTransform();

		auto startPos = tr->GetWorldPosition() + DUOLMath::Vector3::Up * 2.3f;

		DUOLClient::EnemyManager::GetInstance()->GetMainCameraController()->SetCameraShake(0.5f, DUOLMath::Vector2(6.0f, 6.0f), tr);


		projectile->GetTransform()->SetPosition(startPos);

		projectile->FireProjectile(tr->GetParent()->GetLook(), 20, enemy->GetGameObject(), enemy->GetDamage(), TEXT("Player"), false, 0.4f);

		auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::FistWide, 1.f);

		auto particleTr = particleRenderer->GetTransform();

		DUOLMath::Quaternion rot = DUOLMath::Quaternion::CreateFromAxisAngle(tr->GetRight(), DUOLMath::MathHelper::DegreeToRadian(-90.f));

		particleTr->SetRotation(rot);

		particleTr->SetPosition(startPos + tr->GetLook() * 3);
	}

	inline void JumpAttackStart(DUOLClient::Enemy* enemy)
	{
		enemy->SetNavOffRigidbodyOn();

		enemy->GetParentTransform()->LookAt(enemy->GetTarget()->GetTransform());

		auto targetPosition = enemy->GetTarget()->GetTransform()->GetWorldPosition();

		auto tr = enemy->GetParentTransform();

		auto dir = targetPosition - tr->GetWorldPosition();

		dir.y = 7.0f;

		dir *= 0.8f;

		enemy->GetRigidbody()->AddImpulse(dir);
	}
	inline void JumpAttackEnd(DUOLClient::Enemy* enemy)
	{

		enemy->SetNavOnRigidbodyOff();

		auto tr = enemy->GetParentTransform();

		auto enemyPos = enemy->GetParentTransform()->GetWorldPosition();

		DUOLClient::EnemyManager::GetInstance()->GetMainCameraController()->SetCameraShake(
			0.7f, DUOLMath::Vector2(8.0f, 8.0f),
			tr);

		auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::Crack, 5.f);

		auto particleTranform = particleRenderer->GetTransform();

		std::vector<DUOLPhysics::RaycastHit> hit;

		DUOLGameEngine::PhysicsManager::GetInstance()->RaycastAll(enemyPos, DUOLMath::Vector3(0, -1, 0), 10, hit);

		for (auto& iter : hit)
		{
			if (static_cast<DUOLGameEngine::GameObject*>(iter._userData)->GetLayer() == TEXT("Obstacle"))
			{
				enemyPos.y = iter._hitPosition.y + 0.3f;
				break;
			}
		}

		particleTranform->SetPosition(enemyPos, DUOLGameEngine::Space::World);

		auto enemyLook = tr->GetLook();

		enemyLook.y = 0;

		particleTranform->LookAt(enemyPos + enemyLook);

		hit.clear();

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapSphereAll(enemyPos, 4, hit))
		{
			for (auto hited : hit)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				if (gameObject == enemy->GetGameObject()) continue;

				if (gameObject->GetTag() == TEXT("Player"))
				{
					auto player = gameObject->GetComponent<DUOLClient::CharacterBase>();

					if (enemy->Attack(player, enemy->GetDamage(), AttackType::HeavyAttack))
					{
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

					if (enemy->Attack(enemyOther, 0, AttackType::HeavyAttack))
					{
						auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 0.7f);

						DUOLMath::Vector3 randYOffset = DUOLMath::Vector3(0, DUOLMath::MathHelper::RandF(1.0f, 1.5f), 0);

						particleRenderer->GetTransform()->SetPosition(enemyOther->GetTransform()->GetWorldPosition() + randYOffset);

					}
				}
			}
		}

	}

	inline void RushAndHit(DUOLClient::Enemy* enemy)
	{
		enemy->SetNavOnRigidbodyOff();
		enemy->GetNavMeshAgent()->SetMaxSpeed(20.0f);

		auto tr = enemy->GetParentTransform();
		auto nav = enemy->GetNavMeshAgent();
		nav->SetDestination(tr->GetWorldPosition() + tr->GetLook());

		std::vector<DUOLPhysics::RaycastHit> boxcastHits;

		const DUOLMath::Quaternion boxRotation = tr->GetWorldRotation();

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapBoxAll(tr->GetWorldPosition() + DUOLMath::Vector3(0, 1.5f, 0), DUOLMath::Vector3(1.5f, 1.5f, 1.5f), boxRotation, boxcastHits))
		{
			for (auto hited : boxcastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				if (gameObject == enemy->GetGameObject()) continue;

				if (gameObject->GetTag() == TEXT("Player"))
				{
					auto player = gameObject->GetComponent<DUOLClient::CharacterBase>();

					if (player->GetIsInvincible())
						continue;

					// 다단히트 어떻게 할까..!!!
					// auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 0.7f);

					// DUOLMath::Vector3 randYOffset = DUOLMath::Vector3(0, DUOLMath::MathHelper::RandF(1.0f, 1.5f), 0);

					// particleRenderer->GetTransform()->SetPosition(player->GetTransform()->GetWorldPosition() + randYOffset);

					if (enemy->Attack(player, enemy->GetDamage(), AttackType::HeavyAttack))
					{
						auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 0.7f);
						// 약한 앨리트 몬스터 다단히트 어떻게 할까...! 일단 막아두자...!
						if (particleRenderer)
						{
							DUOLMath::Vector3 randYOffset = DUOLMath::Vector3(0, DUOLMath::MathHelper::RandF(1.0f, 1.5f), 0);

							particleRenderer->GetTransform()->SetPosition(player->GetTransform()->GetWorldPosition() + randYOffset);
						}
						enemy->SetParameter(TEXT("IsRushHit_Target"), true);
					}
				}
				else if (gameObject->GetTag() == TEXT("Enemy"))
				{
					auto enemyOther = gameObject->GetComponent<DUOLClient::CharacterBase>();

					if (enemyOther->GetIsInvincible())
						continue;

					if (enemy->Attack(enemyOther, 0, AttackType::HeavyAttack))
					{
						auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 0.7f);
						// 약한 앨리트 몬스터 다단히트 어떻게 할까...! 일단 막아두자...!
						if (particleRenderer)
						{
							DUOLMath::Vector3 randYOffset = DUOLMath::Vector3(0, DUOLMath::MathHelper::RandF(1.0f, 1.5f), 0);

							particleRenderer->GetTransform()->SetPosition(enemyOther->GetTransform()->GetWorldPosition() + randYOffset);
						}
					}

				}
			}
		}

		std::vector<DUOLPhysics::RaycastHit> raycastHits;

		DUOLMath::Vector3 rayStartPos = tr->GetWorldPosition() + DUOLMath::Vector3(0, 1.5f, 0);

		if (DUOLGameEngine::PhysicsManager::GetInstance()->RaycastAll(rayStartPos,
			tr->GetLook(),
			1.5f,
			raycastHits
		)) // 정확하게 앞에 있는 오브젝트만 판별하기 위해서는 레이 캐스트가 낫다고 판별
		{
			for (auto hited : raycastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				if (gameObject == enemy->GetGameObject()) continue;

				if (gameObject->GetLayer() == TEXT("Obstacle"))
				{
					auto condition = tr->GetLook().Dot(hited._hitNormal);

					//if (condition < 0) // 부딪혔다...!
					//{
					enemy->GetAnimator()->SetBool(TEXT("IsGroggy"), true);
					//	break;
					//}
				}
			}
		}

	}
}
