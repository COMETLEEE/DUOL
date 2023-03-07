#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"

DUOLGameEngine::BehaviortreeController::BehaviortreeController(DUOLGameEngine::GameObject* owner)
	:ComponentBase(owner, TEXT("BehaviorTreeController")),
	_behaviorTree(), _isInit(false), _isPlay(true)
{
}

DUOLGameEngine::BehaviortreeController::~BehaviortreeController()
{
}

void DUOLGameEngine::BehaviortreeController::Initialize(BT::Tree&& tree)
{
	_behaviorTree = std::move(tree);
	_isInit = true;
}

void DUOLGameEngine::BehaviortreeController::StopBehaviorTree()
{
	_isPlay = false;
}

void DUOLGameEngine::BehaviortreeController::PlayBehaviorTree()
{
	_isPlay = true;
}

void DUOLGameEngine::BehaviortreeController::OnUpdate(float deltaTime)
{
	if (_isInit && _isPlay)
		_behaviorTree.tickOnce();
}
