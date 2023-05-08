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
}