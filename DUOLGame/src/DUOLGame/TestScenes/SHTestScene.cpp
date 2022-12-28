#include "DUOLGame/TestScenes/SHTestScene.h"

#include "DUOLGame/TestScripts/CoroutineLogTest.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"

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

	// ----------- Coroutine Logger -----------
	DUOLGameEngine::GameObject* testObject = CreateEmpty();

	testObject->AddComponent<DUOLGame::CoroutineLogTest>();


	CreateFromParticleData(_T("blendTest.dfx"));
	auto spark = CreateFromParticleData(_T("Spark.dfx"));
	spark->GetTransform()->SetPosition(DUOLMath::Vector3(40,0,0));
	auto fire = CreateFromParticleData(_T("fire.dfx"));
	fire->GetTransform()->SetPosition(DUOLMath::Vector3(-30,0,0));
	auto blueFire = CreateFromParticleData(_T("FireBlue.dfx"));
	blueFire->GetTransform()->SetPosition(DUOLMath::Vector3(-40,0,0));

	__super::Awake();
}
