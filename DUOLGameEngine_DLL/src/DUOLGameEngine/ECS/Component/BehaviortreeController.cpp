#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"

DUOLGameEngine::BehaviortreeController::BehaviortreeController(DUOLGameEngine::GameObject* owner)
	:ComponentBase(owner, TEXT("BehaviorTreeController"))
{
}

DUOLGameEngine::BehaviortreeController::~BehaviortreeController()
{
}
