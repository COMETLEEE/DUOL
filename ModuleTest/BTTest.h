#pragma once
#include <iostream>
#include <time.h>
#include "PlayerBehaviorTree.h"
#include "InputManager.h"

void BTTestCode()
{
	PlayerBehaviorTree pbt;

	pbt.Init();

	time_t prev = GetTickCount64();

	while (true)
	{
		if (GetTickCount64() - prev >= 500)
		{
			prev = GetTickCount64();

			InputManager::GetInstance()->Update();

			if (InputManager::GetInstance()->GetInputState('Q', KeyState::STAY) == true)
				break;

			pbt.Update(1.0f);

			std::cout << std::endl;

			InputManager::GetInstance()->LateUpdate();
		}
	}
}