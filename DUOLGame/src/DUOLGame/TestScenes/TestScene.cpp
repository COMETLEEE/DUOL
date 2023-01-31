#include "DUOLGame/TestScenes/TestScene.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"


#include "DUOLGame/TestScripts/EnableTest.h"
#include "DUOLGame/TestScripts/CoroutineLogTest.h"
#include "DUOLGame/TestScripts/ModelShooter.h"
#include "DUOLGame/TestScripts/PhysicsEventTest.h"
#include "DUOLGame/TestScripts/MoveController.h"
#include "DUOLGame/TestScripts/RotateAroundOrigin.h"
#include "DUOLGame/TestScripts/ThirdPersonCamera.h"
#include "DUOLGameEngine/ECS/Component/MeshCollider.h"

namespace DUOLGame
{
	class ThirdPersonCamera;
}

DUOLGame::TestScene::TestScene() :
	Scene(TEXT("TestScene"))
{
}

DUOLGame::TestScene::~TestScene()
{
}

void DUOLGame::TestScene::Awake()
{
	// ----------- Main Camera -----------
	DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

	mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 0.f, 0.f));

	mainCamObject->AddComponent<DUOLGameEngine::Camera>();

	auto cameraComp = mainCamObject->AddComponent<DUOLGameEngine::TPFController>();
	//auto cameraComp = mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

	// ----------- Directional Light -----------
	DUOLGameEngine::GameObject* dirLight = CreateEmpty();

	DUOLGameEngine::Light* dirLightCom = dirLight->AddComponent<DUOLGameEngine::Light>();

	dirLightCom->SetLightType(DUOLGameEngine::LightType::Directional);
	dirLightCom->SetIntensity(5.f);
	dirLight->GetTransform()->LookAt({ -0.4f, -1.f, -0.4f });

	dirLightCom->SetColor(DUOLMath::Vector3{ 1.f, 1.f, 1.0f });

	{
		DUOLGameEngine::GameObject* b = CreateFromFBXModel(TEXT("B_Test"));

		//for(auto& child : b->GetTransform()->GetChildren())
		//{
		//	auto meshfilter = child->GetGameObject()->GetComponent<DUOLGameEngine::MeshFilter>();
		//	if(meshfilter != nullptr)
		//	{
		//		child->GetGameObject()->AddComponent<DUOLGameEngine::MeshCollider>()->SetMeshBuffer(meshfilter);
		//	}
		//}
	}

	{
		DUOLGameEngine::GameObject* sphere = CreateFromFBXModel(TEXT("Drunk Idle"));
		sphere->GetComponent<DUOLGameEngine::Animator>()
			->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(TEXT("TestAnimCon")));
		auto collider = sphere->AddComponent<DUOLGameEngine::CapsuleCollider>();
		//sphere->AddComponent<DUOLGameEngine::Rigidbody>();

		sphere->GetTransform()->SetPosition({ 2.f, 0.f, -10.f });
		//sphere->GetTransform()->SetRotation({ -0.7071068, 0, 0, 0.7071068 });
		//cameraComp->SetTargetObject(sphere);

		sphere = CreateFromFBXModel(TEXT("Sphere"));
		sphere->GetTransform()->SetPosition({ 2.f, 0.f, -5.f });

		sphere = CreateFromFBXModel(TEXT("Sphere2"));
		sphere->GetTransform()->SetPosition({ 0.f, 0.f, -5.f });

		sphere = CreateFromFBXModel(TEXT("Sphere3"));
		sphere->GetTransform()->SetPosition({ -2.f, 0.f, -5.f });

		sphere = CreateFromFBXModel(TEXT("Sphere4"));
		sphere->GetTransform()->SetPosition({ -4.f, 0.f, -5.f });

	}

	{
		DUOLGameEngine::GameObject* plain = CreateEmpty();
		
		plain->AddComponent<DUOLGameEngine::BoxCollider>();

	}

	auto testfx = CreateFromParticleData(_T("test.dfx"));

	__super::Awake();
}

void DUOLGame::TestScene::Update(float deltaTime)
{
	__super::Update(deltaTime);
}
