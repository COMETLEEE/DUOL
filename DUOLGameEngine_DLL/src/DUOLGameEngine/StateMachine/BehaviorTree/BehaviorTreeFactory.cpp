#include "DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTreeFactory.h"

// _factory.addDescriptionToManifest();
// _factory.builders();
// _factory.builtinNodes();
// _factory.clearRegisteredBehaviorTrees();
// _factory.createTree();
// _factory.instantiateTreeNode();
// _factory.manifests();
// _factory.registerBehaviorTreeFromFile();
// _factory.registerBehaviorTreeFromText();
// _factory.registerBuilder();
// _factory.registeredBehaviorTrees();
// _factory.registerFromPlugin();
// _factory.registerFromROSPlugins();
//_factory.unregisterBuilder();

//_factory.registerScriptingEnum();
//_factory.registerScriptingEnums();

namespace DUOLGameEngine
{
	BehaviorTreeFactory::BehaviorTreeFactory()
	{
	}

	BehaviorTreeFactory::~BehaviorTreeFactory()
	{
	}

	void BehaviorTreeFactory::RegisterSimpleAction(const std::string& ID, const BT::SimpleActionNode::TickFunctor& tick_functor, BT::PortsList ports)
	{
		_factory.registerSimpleAction(ID, tick_functor, ports);
	}

	void BehaviorTreeFactory::RegisterSimpleCondition(const std::string& ID, const BT::SimpleConditionNode::TickFunctor& tick_functor, BT::PortsList ports)
	{
		_factory.registerSimpleCondition(ID, tick_functor, ports);
	}

	void BehaviorTreeFactory::RegisterSimpleDecorator(const std::string& ID, const BT::SimpleDecoratorNode::TickFunctor& tick_functor, BT::PortsList ports)
	{
		_factory.registerSimpleDecorator(ID, tick_functor, ports);
	}

	BT::Tree BehaviorTreeFactory::CreateTreeFromText(const std::string& file_path, BT::Blackboard::Ptr blackboard)
	{
		// 이곳에서 예외 발생시 노드를 모두 제대로 생성하였는지 확인해 주십쇼..!
		return _factory.createTreeFromText(file_path, blackboard);
	}

	BT::Tree BehaviorTreeFactory::CreateTreeFromFile(const std::string& file_path, BT::Blackboard::Ptr blackboard)
	{
		// 이곳에서 예외 발생시 노드를 모두 제대로 생성하였는지 확인해 주십쇼..!
		return _factory.createTreeFromFile(file_path, blackboard);
	}

}