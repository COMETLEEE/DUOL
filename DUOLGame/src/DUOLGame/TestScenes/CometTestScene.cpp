#include "DUOLGame/TestScenes/CometTestScene.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include "DUOLGame/TestScripts/EnableTest.h"
#include "DUOLGame/TestScripts/CoroutineLogTest.h"

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
		// -------- Main Camera --------
		std::shared_ptr<DUOLGameEngine::GameObject> mainCamObject = CreateEmpty();

		mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

		// -------- Coroutine Logger --------
		std::shared_ptr<DUOLGameEngine::GameObject> testObject = CreateEmpty();

		testObject->AddComponent<CoroutineLogTest>();

		// -------- Box --------
		std::shared_ptr<DUOLGameEngine::GameObject> boxObject = CreateEmpty();

		boxObject->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		boxObject->AddComponent<DUOLGameEngine::MeshFilter>()->
			SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Joy")));

		boxObject->AddComponent<DUOLGameEngine::MeshRenderer>();

		boxObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Scarf_MAT")));
		boxObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_LowerBody_MAT")));
		boxObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Hair_MAT")));
		boxObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Shoes_MAT")));
		boxObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_UpperBody_MAT")));
		boxObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Hands_MAT")));
		boxObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Head_MAT")));

		// 이거 어떻게 밖으로 빼낼지 고려해야합니다 ..
		__super::Awake();
	}
}