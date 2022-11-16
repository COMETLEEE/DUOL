#include <iostream>
#include "PlayerMove.h"
#include "InputManager.h"

PlayerMove::PlayerMove() :
	ActionNode(_T("PlayerMove"))
{

}

NodeState PlayerMove::Tick()
{
	bool isMove = false;
	int velocityX = GetDataFromBlackBoard<int>(_T("PlayerVelocityX"));
	int velocityY = GetDataFromBlackBoard<int>(_T("PlayerVelocityY"));

	int* playerLocation = GetDataFromBlackBoard<int*>(_T("PlayerLocation"));

	int moveRange = GetDataFromBlackBoard<int>(_T("MoveRange"));

	/* - Move Range ~ Move Range*/
	if (velocityX != 0)
	{
		if (-moveRange < velocityX + playerLocation[0] && velocityX + playerLocation[0] < moveRange)
		{
			playerLocation[0] += velocityX;
			velocityX -= velocityX / abs(velocityX);
			isMove = true;
		}
		else
		{
			velocityX = 0;

			std::cout << "X Collided" << std::endl;
		}
	}

	if (velocityY != 0)
	{
		if (-moveRange < velocityY + playerLocation[1] && velocityY + playerLocation[1] < moveRange)
		{
			playerLocation[1] += velocityY;
			velocityY -= velocityY / abs(velocityY);
			isMove = true;
		}
		else
		{
			velocityY = 0;

			std::cout << "Y Collided" << std::endl;
		}
	}

	PushDataToBlackBoard<int>(velocityX, _T("PlayerVelocityX"));
	PushDataToBlackBoard<int>(velocityY, _T("PlayerVelocityY"));

	if (isMove == true)
	{
		std::cout << "Player Move x : " << playerLocation[0] << ", y : " << playerLocation[1] << std::endl;

		return NodeState::SUCCESS;
	}

	return NodeState::FAILURE;
}

void PlayerMove::Stop()
{

}