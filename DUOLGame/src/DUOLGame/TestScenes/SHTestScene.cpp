#include "DUOLGame/TestScenes/SHTestScene.h"

#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include "DUOLClient/ECS/Component/CoroutineLogTest.h"
#include "DUOLClient/ECS/Component/PlayerController.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"

#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

SHTestScene::SHTestScene() :
	Scene(TEXT("SHTestScene"))
{
}

void SHTestScene::Awake()
{

	// ----------- Main Camera -----------
	DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

	mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

	mainCamObject->AddComponent<DUOLGameEngine::Camera>();

	mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

	//// ----------- Coroutine Logger -----------
	DUOLGameEngine::GameObject* testObject = CreateEmpty();
	testObject->AddComponent<DUOLGameEngine::CapsuleCollider>();
	testObject->AddComponent<DUOLGameEngine::Rigidbody>();
	//testObject->AddComponent<DUOLClient::PlayerController>();

	//testObject->AddComponent<DUOLClient::CoroutineLogTest>();
	testObject->AddComponent<DUOLClient::AI_Enemy>();


	//// ----------- BehaviorTree -----------

	//auto tree = treeFactory->CreateTree("Test");

	//testObject->AddComponent<DUOLGameEngine::BehaviortreeController>()->Initialize(std::move(tree));
	__super::Awake();
}
