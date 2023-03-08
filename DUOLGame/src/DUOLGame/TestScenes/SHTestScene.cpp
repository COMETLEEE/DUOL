#include "DUOLGame/TestScenes/SHTestScene.h"

#include "DUOLGame/TestScripts/CoroutineLogTest.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"

SHTestScene::SHTestScene() :
	Scene(TEXT("SHTestScene"))
{
}

struct Position2D
{
	double x, y;
};

namespace BT
{
	template <>
	inline Position2D convertFromString(StringView str)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Converting string: \"{0}\"\n", str.data());

		auto parts = splitString(str, ';');
		if (parts.size() != 2)
		{
			throw RuntimeError("invalid input)");
		}
		else
		{
			Position2D output;
			output.x = convertFromString<double>(parts[0]);
			output.y = convertFromString<double>(parts[1]);
			return output;
		}
	}
}   // end namespace BT

class CalculateGoal : public BT::SyncActionNode
{
public:
	CalculateGoal(const std::string& name, const BT::NodeConfig& config) :
		SyncActionNode(name, config)
	{}

	BT::NodeStatus tick() override
	{
		Position2D mygoal = { 1.1, 2.3 };
		setOutput("goal", mygoal);
		return BT::NodeStatus::SUCCESS;
	}
	static BT::PortsList providedPorts()
	{
		return { BT::OutputPort<Position2D>("goal") };
	}
};

class PrintTarget : public BT::SyncActionNode
{
public:
	PrintTarget(const std::string& name, const BT::NodeConfig& config) :
		SyncActionNode(name, config)
	{}

	BT::NodeStatus tick() override
	{
		auto res = getInput<Position2D>("target");
		if (!res)
		{
			throw BT::RuntimeError("error reading port [target]:", res.error());
		}
		Position2D goal = res.value();

		DUOL_TRACE(DUOL_CONSOLE, "Target positions: [ {0}, {1}]\n", goal.x, goal.y);

		return BT::NodeStatus::SUCCESS;
	}

	static BT::PortsList providedPorts()
	{
		// Optionally, a port can have a human readable description
		const char* description = "Simply print the target on console...";
		return { BT::InputPort<Position2D>("target", description) };
	}
};


static const char* xml_text = R"(

 <root BTCPP_format="4" >
     <BehaviorTree ID="TreeWithPorts2">
		<ReactiveFallback>
		  <Inverter>
		    <CalculateGoal goal="{GoalPosition}"/>
		  </Inverter>
		  <Inverter>
		    <PrintTarget target="{GoalPosition}"/>
		  </Inverter>
		  <Inverter>
		    <Script code="OtherGoal:=&apos;-1;3&apos;"
		            _description="Using a Script, you can directly write into the blackboard"/>
		  </Inverter>
		  <PrintTarget target="{OtherGoal}"/>
		</ReactiveFallback>
	</BehaviorTree>
 </root>
 )";


void SHTestScene::Awake()
{
	// ----------- Main Camera -----------
	DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

	mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

	mainCamObject->AddComponent<DUOLGameEngine::Camera>();

	mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

	// ----------- Coroutine Logger -----------
	DUOLGameEngine::GameObject* testObject = CreateEmpty();

	testObject->AddComponent<DUOLGame::CoroutineLogTest>();

	auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

	treeFactory->RegisterNodeType<CalculateGoal>("CalculateGoal");
	treeFactory->RegisterNodeType<PrintTarget>("PrintTarget");

	treeFactory->RegisterTreeFromFileInDirectory("Asset/BehaviorTree");

	auto tree = treeFactory->CreateTree("TreeWithPorts2");

	testObject->AddComponent<DUOLGameEngine::BehaviortreeController>()->Initialize(std::move(tree));

	__super::Awake();
}
