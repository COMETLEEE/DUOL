#include "DUOLClient/DUOLClient_Initializer.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"

#include "DUOLClient/BehaviorTreeNode/Action/Action_AroundPatrol.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_BoidsMoveTo.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_Die.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_Hit.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_UseTokenAttack.h"

#include "DUOLClient/BehaviorTreeNode/Condition/Condition_CurrentAnimStateCheck.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_HasToken.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsClose.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsGroupCheck.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInAttackRange.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsLookTarget.h"

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
		treeFactory->RegisterNodeType<Condition_IsClose>("Condition_IsClose");
		treeFactory->RegisterNodeType<Condition_HasToken>("Condition_HasToken");

		treeFactory->RegisterNodeType<Condition_CurrentAnimStateCheck>("Condition_CurrentAnimStateCheck");

		treeFactory->RegisterNodeType<Action_AroundPatrol>("Action_AroundPatrol");
		treeFactory->RegisterNodeType<Action_UseTokenAttack>("Action_UseTokenAttack");
		treeFactory->RegisterNodeType<Action_Die>("Action_Die");
		treeFactory->RegisterNodeType<Action_Hit>("Action_Hit");
		treeFactory->RegisterNodeType<Action_BoidsMoveTo>("Action_BoidsMoveTo");


		treeFactory->Initialize();
	}
}
