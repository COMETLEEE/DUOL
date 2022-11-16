#include <iostream>
#include "PlayerDash.h"

PlayerDash::PlayerDash() :
	ActionNode(_T("PlayerDash"))
{

}

NodeState PlayerDash::Tick()
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
			isMove = true;
		}
		else
		{
			std::cout << "X Collided" << std::endl;
		}
	}

	if (velocityY != 0)
	{
		if (-moveRange < velocityY + playerLocation[1] && velocityY + playerLocation[1] < moveRange)
		{
			playerLocation[1] += velocityY;
			isMove = true;
		}
		else
		{
			std::cout << "Y Collided" << std::endl;
		}
	}

	if (isMove == true)
	{
		std::cout << "Player Dash x : " << playerLocation[0] << ", y : " << playerLocation[1] << std::endl;

		return NodeState::SUCCESS;
	}

	return NodeState::FAILURE;
}

void PlayerDash::Stop()
{

}