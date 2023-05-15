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

	inline void EventSetBool(DUOLClient::Enemy* enemy, DUOLCommon::tstring name, bool isBool)
	{
		enemy->GetAnimator()->SetBool(name, isBool);
	}

	inline void RushParticlePlay(DUOLClient::Enemy* enemy)
	{
		auto particle = ParticleManager::GetInstance()->Pop(ParticleEnum::BigFootRushDustEffect, 1);

		auto particleTr = particle->GetTransform();

		auto enemyTr = enemy->GetTransform();

		particleTr->SetPosition(enemyTr->GetWorldPosition());

		particleTr->SetRotation(enemyTr->GetWorldRotation());
	}
}