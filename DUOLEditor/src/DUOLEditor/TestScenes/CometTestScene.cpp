#include "DUOLEditor/TestScenes/CometTestScene.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"


#include "DUOLEditor/TestScripts/EnableTest.h"
#include "DUOLEditor/TestScripts/CoroutineLogTest.h"
#include "DUOLEditor/TestScripts/ModelShooter.h"
#include "DUOLEditor/TestScripts/PhysicsEventTest.h"
#include "DUOLEditor/TestScripts/MoveController.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

namespace DUOLEditor
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
		DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

		mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

		// ----------- Coroutine Logger -----------
		DUOLGameEngine::GameObject* testObject = CreateEmpty();

		testObject->AddComponent<CoroutineLogTest>();

		// Joy
		{
			DUOLGameEngine::GameObject* joyObject = CreateFromFBXModel(TEXT("Firing Rifle"));

			joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(5.f, 3.f, 0.f));

			joyObject->GetComponent<DUOLGameEngine::Animator>()->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("mixamo.com")));

			// joyObject->AddComponent<DUOLEditor::MoveController>();

			auto children = joyObject->GetTransform()->GetChildren();

			for (auto& child : children)
			{
				if (child->GetGameObject()->GetName() == _T("Boy01_Scarf_Geo"))
					child->GetGameObject()->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Scarf_MAT")));
				else if (child->GetGameObject()->GetName() == _T("Boy01_Hair_Geo"))
					child->GetGameObject()->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Hair_MAT")));
				else if (child->GetGameObject()->GetName() == _T("Boy01_Hands_Geo"))
					child->GetGameObject()->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Hands_MAT")));
				else if (child->GetGameObject()->GetName() == _T("Boy01_Head_Geo"))
					child->GetGameObject()->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Head_MAT")));
				else if (child->GetGameObject()->GetName() == _T("Boy01_LowerBody_Geo"))
					child->GetGameObject()->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_LowerBody_MAT")));
				else if (child->GetGameObject()->GetName() == _T("Boy01_Shoes_Geo"))
					child->GetGameObject()->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_Shoes_MAT")));
				else if (child->GetGameObject()->GetName() == _T("Boy01_UpperBody_Geo"))
					child->GetGameObject()->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Boy01_UpperBody_MAT")));
			}

		}

		DUOLGameEngine::GameObject* boxObject = CreateFromFBXModel(TEXT("Box"));

		boxObject->AddComponent<DUOLEditor::MoveController>();

		boxObject->SetName(TEXT("ParentBox"));

		DUOLGameEngine::GameObject* boxObject2 = CreateFromFBXModel(TEXT("Box"));

		boxObject2->SetName(TEXT("ChildBox"));

		boxObject2->GetTransform()->SetParent(boxObject->GetTransform());

		boxObject2->GetTransform()->SetPosition(DUOLMath::Vector3{ 5.f, 0.f, 0.f });

		__super::Awake();
	}
}