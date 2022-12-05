#include "DUOLGame/TestScenes/YDTestScene.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"


#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include "DUOLGame/TestScripts/EnableTest.h"
#include "DUOLGame/TestScripts/CoroutineLogTest.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	YDTestScene::YDTestScene() :
		Scene(TEXT("YDTestScene"))
	{

	}

	YDTestScene::~YDTestScene()
	{
		
	}

	void YDTestScene::Awake()
	{
		// ----------- Main Camera -----------
		std::shared_ptr<DUOLGameEngine::GameObject> mainCamObject = CreateEmpty();

		mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

		// Joy
		{
			std::shared_ptr<DUOLGameEngine::GameObject> joyObject = CreateEmpty();

			joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(5.f, 3.f, 0.f));

			joyObject->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

			joyObject->AddComponent<DUOLGameEngine::MeshFilter>()->
				SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Joy")));

			joyObject->AddComponent<DUOLGameEngine::MeshRenderer>();

			joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Scarf_MAT")));
			joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_LowerBody_MAT")));
			joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Hair_MAT")));
			joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Shoes_MAT")));
			joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_UpperBody_MAT")));
			joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Hands_MAT")));
			joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Head_MAT")));
		}

		// ----------- Marias -----------
		//for (int i = 0; i < 10; i++)
		//{
		//	std::shared_ptr<DUOLGameEngine::GameObject> joyObject = CreateEmpty();

		//	joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(i * 10.f, 0.f, 0.f));

		//	joyObject->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		//	joyObject->AddComponent<DUOLGameEngine::MeshFilter>()->
		//		SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Maria WProp J J Ong")));

		//	joyObject->AddComponent<DUOLGameEngine::MeshRenderer>();

		//	joyObject->AddComponent<DUOLGameEngine::BoxCollider>();

		//	auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("MariaMat"));

		//	joyObject->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);
		//}

		// 이거 어떻게 밖으로 빼낼지 고려해야합니다 ..
		__super::Awake();
	}
}
