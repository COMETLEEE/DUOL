#pragma once

#include "EnemyHits.h"
#include "EnemyAttacks.h"


namespace DUOLClient
{
	inline void StopAnimator(DUOLClient::Enemy* enemy)
	{
		enemy->GetAnimator()->SetSpeed(0);
	}

	inline void SetBool_IsWakeUpToIdle_True(DUOLClient::Enemy* enemy)
	{
		enemy->GetAnimator()->SetBool(TEXT("IsWakeUpToIdle"), true);
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
		DUOLMath::Vector3 myPosition = enemy->GetParentTransform()->GetWorldPosition();

		DUOLMath::Vector3 targetPos = enemy->GetTarget()->GetTransform()->GetWorldPosition();

		targetPos.y = myPosition.y;

		auto look = targetPos - myPosition;

		look.Normalize();

		look = DUOLMath::Vector3::Lerp(enemy->GetParentTransform()->GetLook(), look, 0.3f);

		enemy->GetParentTransform()->LookAt(myPosition + look);
	}
}