#pragma once

#include "EnemyHits.h"
#include "EnemyAttacks.h"


namespace DUOLClient
{
	inline void StopAnimator(DUOLClient::Enemy* enemy)
	{
		enemy->GetAnimator()->SetSpeed(0);
	}

	inline void SetNavOnRigidbodyOff(DUOLClient::Enemy* enemy)
	{
		enemy->SetNavOnRigidbodyOff();
	}
	inline void SetNavOffRigidbodyOn(DUOLClient::Enemy* enemy)
	{
		enemy->SetNavOffRigidbodyOn();
	}

	inline void LerpLookTarget(DUOLClient::Enemy* enemy)
	{
		if (enemy->GetAIController()->GetIsGroupCheck())
			enemy->LerpLookTarget();
	}
	inline void SuperArmorOff_OnTimer(DUOLClient::Enemy* enemy)
	{
		enemy->GetAIController()->SetSuperArmor(false, enemy->GetAIController()->GetSuperArmorTime());
	}
	inline void EventSetBool(DUOLClient::Enemy* enemy, DUOLCommon::tstring name, bool isBool)
	{
		enemy->GetAnimator()->SetBool(name, isBool);
	}

	inline void RushParticlePlay(DUOLClient::Enemy* enemy)
	{
		auto particle = ParticleManager::GetInstance()->Pop(ParticleEnum::BigFootRushDustEffect, 1);

		if (!particle) return;

		auto particleTr = particle->GetTransform();

		auto enemyTr = enemy->GetTransform();

		particleTr->SetPosition(enemyTr->GetWorldPosition());

		particleTr->SetRotation(enemyTr->GetWorldRotation());
	}

	inline void StandingCryParticle(DUOLClient::Enemy* enemy)
	{
		auto particle = ParticleManager::GetInstance()->Pop(ParticleEnum::Shouting, 2.0f);

		auto particleTr = particle->GetTransform();

		auto enemyTr = enemy->GetTransform();

		particleTr->SetPosition(enemyTr->GetWorldPosition() + DUOLMath::Vector3(0, 2.5f, 1.0f));

		particleTr->SetRotation(enemyTr->GetWorldRotation());
	}
}