#include "IsPlayerJumpable.h"
#include "InputManager.h"

IsPlayerJumpable::IsPlayerJumpable() :
	ConditionNode(_T("PlayerJumpable"))
{

}

bool IsPlayerJumpable::ConditionCheck()
{
	return InputManager::GetInstance()->GetInputState(VK_SPACE, KeyState::DOWN);
}