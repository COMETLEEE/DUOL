#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::BehaviortreeController>("BehaviortreeController")
		.constructor()
		(
			rttr::policy::ctor::as_raw_ptr
		).constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
		(
			rttr::policy::ctor::as_raw_ptr
		);
}

DUOLGameEngine::BehaviortreeController::BehaviortreeController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name)
	:BehaviourBase(owner, name),
	_behaviorTree(), _isInit(false), _isPlay(true)
{
}

DUOLGameEngine::BehaviortreeController::~BehaviortreeController()
{
	_behaviorTree.rootBlackboard()->set<DUOLGameEngine::GameObject*>("GameObject", nullptr);
}

void DUOLGameEngine::BehaviortreeController::Initialize(const std::string& ID, BT::Blackboard::Ptr blackboard)
{
	auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

	blackboard->set<DUOLGameEngine::GameObject*>("GameObject", GetGameObject());

	_behaviorTree = treeFactory->CreateTree(ID, blackboard);

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