#include "IsPlayerMoveable.h"
#include "InputManager.h"

IsPlayerMoveable::IsPlayerMoveable() :
	ConditionNode(_T("PlayerMoveable"))
{

}

bool IsPlayerMoveable::ConditionCheck()
{
	int velocityX = GetDataFromBlackBoard<int>(_T("PlayerVelocityX"));
	int velocityY = GetDataFromBlackBoard<int>(_T("PlayerVelocityY"));
	int playerSpeed = GetDataFromBlackBoard<int>(_T("PlayerSpeed"));

	if (InputManager::GetInstance()->GetInputState(VK_LEFT, KeyState::STAY) == true)
		velocityX -= playerSpeed;

	if (InputManager::GetInstance()->GetInputState(VK_RIGHT, KeyState::STAY) == true)
		velocityX += playerSpeed;

	if (InputManager::GetInstance()->GetInputState(VK_UP, KeyState::STAY) == true)
		velocityY += playerSpeed;

	if (InputManager::GetInstance()->GetInputState(VK_DOWN, KeyState::STAY) == true)
		velocityY -= playerSpeed;

	PushDataToBlackBoard<int>(velocityX, _T("PlayerVelocityX"));
	PushDataToBlackBoard<int>(velocityY, _T("PlayerVelocityY"));

	return velocityX != 0 || velocityY != 0;
}