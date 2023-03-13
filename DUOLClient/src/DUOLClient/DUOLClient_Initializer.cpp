#include "DUOLClient/DUOLClient_Initializer.h"

#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsGroupCheck.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInAttackRange.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsLookTarget.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"

namespace DUOLClient
{
	bool DUOLClient_Initializer::_isInit = false;

	void DUOLClient_Initializer::DUOLClient_Initialize()
	{
		if (_isInit) return;

		_isInit = true;

		DUOLCommon::LogHelper::Initialize();

		BehaviorTreeFactory_Initialize();
	}

	void DUOLClient_Initializer::BehaviorTreeFactory_Initialize()
	{
		auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

		treeFactory->RegisterNodeType<Condition_IsInAttackRange>("Condition_IsInAttackRange");
		treeFactory->RegisterNodeType<Condition_IsLookTarget>("Condition_IsLookTarget");
		treeFactory->RegisterNodeType<Condition_IsGroupCheck>("Condition_IsGroupCheck");

		treeFactory->Initialize();
	}
}
