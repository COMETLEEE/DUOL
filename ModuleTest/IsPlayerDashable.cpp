#include "IsPlayerDashable.h"
#include "InputManager.h"

IsPlayerDashable::IsPlayerDashable() :
	ConditionNode(_T("PlayerDashable"))
{

}

bool IsPlayerDashable::ConditionCheck()
{
	int velocityX = GetDataFromBlackBoard<int>(_T("PlayerVelocityX"));
	int velocityY = GetDataFromBlackBoard<int>(_T("PlayerVelocityY"));

	if (velocityX == 0 && velocityY == 0)
		return false;

	if (InputManager::GetInstance()->GetInputState(VK_SHIFT, KeyState::DOWN) == true)
		return true;

	return false;
}