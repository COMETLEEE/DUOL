#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include <filesystem>

#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_LookAt.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_MakeNoise.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_MoveDirectlyToward.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_MoveTo.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_PlayAnimation.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_PlaySound.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_SetAnimatorParameter.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_Wait.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Condition_Random.h"
#include "DUOLGameEngine/BehaviorTreeSampleNode/Decorator_KeepRunningUntilSuccess.h"

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
		_factory = new BT::BehaviorTreeFactory();
	}

	BehaviorTreeFactory::~BehaviorTreeFactory()
	{
	}

	void BehaviorTreeFactory::Initialize()
	{

		RegisterNodeType<Action_MakeNoise>("Action_MakeNoise");
		RegisterNodeType<Action_MoveDirectlyToward>("Action_MoveDirectlyToward");
		RegisterNodeType<Action_MoveTo>("Action_MoveTo");
		RegisterNodeType<Action_PlayAnimation>("Action_PlayAnimation");
		RegisterNodeType<Action_PlaySound>("Action_PlaySound");
		RegisterNodeType<Action_Wait>("Action_Wait");
		RegisterNodeType<Action_LookAt>("Action_LookAt");
		RegisterNodeType<Action_SetAnimatorParameter>("Action_SetAnimatorParameter");
		RegisterNodeType<Condition_Random>("Condition_Random");
		RegisterNodeType<Decorator_KeepRunningUntilSuccess>("Decorator_KeepRunningUntilSuccess");

		RegisterTreeFromFileInDirectory("Asset/BehaviorTree");
	}

	void BehaviorTreeFactory::UnInitialize()
	{
		delete _factory;
	}

	void BehaviorTreeFactory::RegisterSimpleAction(const std::string& ID, const BT::SimpleActionNode::TickFunctor& tick_functor, BT::PortsList ports)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Registe Simple Action : {0}", ID);
		_factory->registerSimpleAction(ID, tick_functor, ports);
	}

	void BehaviorTreeFactory::RegisterSimpleCondition(const std::string& ID, const BT::SimpleConditionNode::TickFunctor& tick_functor, BT::PortsList ports)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Registe Simple Condition : {0}", ID);
		_factory->registerSimpleCondition(ID, tick_functor, ports);
	}

	void BehaviorTreeFactory::RegisterSimpleDecorator(const std::string& ID, const BT::SimpleDecoratorNode::TickFunctor& tick_functor, BT::PortsList ports)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Registe Simple Decorator : {0}", ID);
		_factory->registerSimpleDecorator(ID, tick_functor, ports);
	}

	void BehaviorTreeFactory::RegisterTreeFromText(const std::string& xml_str)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Registe Tree From Text : {0}", xml_str);
		// 이곳에서 예외 발생시 노드를 모두 제대로 생성하였는지 확인해 주십쇼..!
		_factory->registerBehaviorTreeFromText(xml_str);
	}

	void BehaviorTreeFactory::RegisterTreeFromFile(const std::string& file_path)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Registe Tree Fomr File : {0}", file_path);
		// 이곳에서 예외 발생시 노드를 모두 제대로 생성하였는지 확인해 주십쇼..!
		_factory->registerBehaviorTreeFromFile(file_path);
	}

	void BehaviorTreeFactory::RegisterTreeFromFileInDirectory(const std::string& directory_path)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Register Tree From File In Directory : {0}", directory_path);

		using std::filesystem::directory_iterator;

		for (auto const& entry : directory_iterator(directory_path))
		{
			if (entry.path().extension() == ".xml")
			{
				RegisterTreeFromFile(entry.path().string());
			}
		}
	}

	BT::Tree BehaviorTreeFactory::CreateTreeFromText(const std::string& file_path, BT::Blackboard::Ptr blackboard)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Create Tree From Text : {0}", file_path);
		// 이곳에서 예외 발생시 노드를 모두 제대로 생성하였는지 확인해 주십쇼..!
		return _factory->createTreeFromText(file_path, blackboard);
	}

	BT::Tree BehaviorTreeFactory::CreateTreeFromFile(const std::string& file_path, BT::Blackboard::Ptr blackboard)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Create Tree From File : {0}", file_path);
		// 이곳에서 예외 발생시 노드를 모두 제대로 생성하였는지 확인해 주십쇼..!
		return _factory->createTreeFromFile(file_path, blackboard);
	}

	BT::Tree BehaviorTreeFactory::CreateTree(const std::string& ID, BT::Blackboard::Ptr blackboard)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Create Tree : {0}", ID);
		// 이곳에서 예외 발생시 노드를 모두 제대로 생성하였는지 확인해 주십쇼..!
		return _factory->createTree(ID, blackboard);
	}
}
