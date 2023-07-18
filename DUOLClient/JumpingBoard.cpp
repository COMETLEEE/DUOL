#include "JumpingBoard.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"

DUOLClient::JumpingBoard::JumpingBoard(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name)
{
}

DUOLClient::JumpingBoard::~JumpingBoard()
{

}

void DUOLClient::JumpingBoard::CalculateDistance()
{
	auto myTransform = GetTransform();
	auto targetTransform = _targetJumpingBoard->GetTransform();

	auto myWorldPos = myTransform->GetWorldPosition();
	auto targetWorldPos = targetTransform->GetWorldPosition();

	_distanceVector = targetWorldPos - myWorldPos;
	auto heightOffset = _distanceVector.y;
	_distanceVector.y = 0;
	auto length = _distanceVector.Length();

	//포물선의 방정식
	// x^2 = -4 * a * y
	// a = (x)^2 / (-4 * (y - maxHeight))
	//36 / 20
	//6, 5

	//float foucsPoint =  / (-4 * (heightOffset - _maxHeight));

}
