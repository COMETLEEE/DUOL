#include "DUOLGame/TestScenes/SHTestScene.h"

#include "DUOLGame/TestScripts/CoroutineLogTest.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"

#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

SHTestScene::SHTestScene() :
	Scene(TEXT("SHTestScene"))
{
}

void SHTestScene::Awake()
{
	auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

	treeFactory->Initialize();

	// ----------- Main Camera -----------
	DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

	mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

	mainCamObject->AddComponent<DUOLGameEngine::Camera>();

	mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

	// ----------- Coroutine Logger -----------
	DUOLGameEngine::GameObject* testObject = CreateEmpty();

	testObject->AddComponent<DUOLGame::CoroutineLogTest>();

	// ----------- BehaviorTree -----------

	auto tree = treeFactory->CreateTree("Test");

	testObject->AddComponent<DUOLGameEngine::BehaviortreeController>()->Initialize(std::move(tree));
	__super::Awake();
}
