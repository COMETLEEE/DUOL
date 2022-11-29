#include "DUOLGame/TestScenes/CometTestScene.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include "DUOLGame/TestScripts/EnableTest.h"
#include "DUOLGame/TestScripts/CoroutineLogTest.h"
#include "DUOLGame/TestScripts/ModelShooter.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	CometTestScene::CometTestScene() :
		Scene(TEXT("CometTestScene"))
	{

	}

	CometTestScene::~CometTestScene()
	{

	}
	
	void CometTestScene::Awake()
	{
		// ----------- Main Camera -----------
		std::shared_ptr<DUOLGameEngine::GameObject> mainCamObject = CreateEmpty();

		mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

		// ----------- Coroutine Logger -----------
		std::shared_ptr<DUOLGameEngine::GameObject> testObject = CreateEmpty();

		testObject->AddComponent<CoroutineLogTest>();

#pragma region SHOOTING_EXPERIMENT
		//std::vector<std::shared_ptr<DUOLGameEngine::Rigidbody>> rigids;

		//{
		//	// ----------- Marias -----------
		//	for (int i = 0; i < 30; i++)
		//	{
		//		std::shared_ptr<DUOLGameEngine::GameObject> joyObject = CreateEmpty();

		//		joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(i * 10.f, 0.f, 0.f));

		//		joyObject->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		//		joyObject->AddComponent<DUOLGameEngine::MeshFilter>()->
		//			SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Maria WProp J J Ong")));

		//		joyObject->AddComponent<DUOLGameEngine::MeshRenderer>();

		//		joyObject->AddComponent<DUOLGameEngine::CapsuleCollider>();

		//		joyObject->AddComponent<DUOLGameEngine::Rigidbody>();

		//		auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("MariaMat"));

		//		joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);
		//	}

		//	// ----------- Marias -----------
		//	for (int i = 0; i < 30; i++)
		//	{
		//		std::shared_ptr<DUOLGameEngine::GameObject> joyObject = CreateEmpty();

		//		joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(i * 10.f, 5.f, 20.f));

		//		joyObject->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		//		joyObject->AddComponent<DUOLGameEngine::MeshFilter>()->
		//			SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Maria WProp J J Ong")));

		//		joyObject->AddComponent<DUOLGameEngine::MeshRenderer>();

		//		joyObject->AddComponent<DUOLGameEngine::CapsuleCollider>();

		//		joyObject->AddComponent<DUOLGameEngine::Rigidbody>();

		//		auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("MariaMat"));

		//		joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);
		//	}

		//	// ----------- Marias -----------
		//	for (int i = 0; i < 5; i++)
		//	{
		//		std::shared_ptr<DUOLGameEngine::GameObject> joyObject = CreateEmpty();

		//		joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(i * 10.f, 15.f, 40.f));

		//		joyObject->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		//		joyObject->AddComponent<DUOLGameEngine::MeshFilter>()->
		//			SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Maria WProp J J Ong")));

		//		joyObject->AddComponent<DUOLGameEngine::MeshRenderer>();

		//		joyObject->AddComponent<DUOLGameEngine::CapsuleCollider>();

		//		rigids.push_back(joyObject->AddComponent<DUOLGameEngine::Rigidbody>());

		//		auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("MariaMat"));

		//		joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);
		//	}
		//}

		//{
		//	// ----------- Shooting Maria -----------
		//	std::shared_ptr<DUOLGameEngine::GameObject> shootObject = CreateEmpty();

		//	shootObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(-15.f, 0.f, 10.f));

		//	shootObject->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		//	shootObject->AddComponent<DUOLGameEngine::BoxCollider>();

		//	shootObject->AddComponent<DUOLGame::ModelShooter>()->_rigids = rigids;

		//	shootObject->AddComponent<DUOLGameEngine::MeshFilter>()->
		//		SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Maria WProp J J Ong")));

		//	shootObject->AddComponent<DUOLGameEngine::MeshRenderer>();

		//	auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("MariaMat"));

		//	shootObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);
		//}
#pragma endregion

		// ----------- Marias -----------
		for (int i = 0; i < 100; i++)
		{
			std::shared_ptr<DUOLGameEngine::GameObject> joyObject = CreateEmpty();

			joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(i * 10.f, 0.f, 0.f));

			joyObject->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

			joyObject->AddComponent<DUOLGameEngine::MeshFilter>()->
				SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Maria WProp J J Ong")));

			joyObject->AddComponent<DUOLGameEngine::MeshRenderer>();

			joyObject->AddComponent<DUOLGameEngine::BoxCollider>();

			joyObject->AddComponent<DUOLGameEngine::Rigidbody>();

			auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("MariaMat"));

			joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);
		}

		// 이거 어떻게 밖으로 빼낼지 고려해야합니다 ..
		__super::Awake();
	}
}