#pragma once

#include "EnemyHits.h"
#include "EnemyAttacks.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"


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
	inline void GroggyOff_OnTimer(DUOLClient::Enemy* enemy)
	{
		auto func = [](Enemy* enemy)->DUOLGameEngine::CoroutineHandler
		{
			co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(enemy->GetParameter<float>(TEXT("GroggyTime")));

			enemy->GetAnimator()->SetBool(TEXT("IsGroggy"), false);
		};

		enemy->StartCoroutine_Manual(std::bind(func, enemy));
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

		DUOLClient::EnemyManager::GetInstance()->GetMainCameraController()->SetCameraShake(0.4f, DUOLMath::Vector2(3.0f, 3.0f), enemyTr);

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

	inline void RandomLookAt(DUOLClient::Enemy* enemy)
	{
		auto tr = enemy->GetParentTransform();

		auto targetPos = enemy->GetTarget()->GetTransform()->GetWorldPosition();

		auto dir = tr->GetWorldPosition() - targetPos;

		dir.y = 0;

		dir.Normalize();

		auto rad = DUOLMath::MathHelper::RandF(-1.57079f, 1.57079f);

		const float _cos = cosf(rad);
		const float _sin = sinf(rad);
		const float x = _cos * dir.x + (-_sin * dir.z);
		const float y = _sin * dir.x + _cos * dir.z;
		dir.x = x;
		dir.z = y;

		tr->LookAt(tr->GetWorldPosition() + dir);
	}
}
