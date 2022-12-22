#include "DUOLGame/TestScenes/YDTestScene.h"

#include "DUOLGame/TestScenes/CometTestScene.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"


#include "DUOLGame/TestScripts/EnableTest.h"
#include "DUOLGame/TestScripts/CoroutineLogTest.h"
#include "DUOLGame/TestScripts/ModelShooter.h"
#include "DUOLGame/TestScripts/PhysicsEventTest.h"
#include "DUOLGame/TestScripts/MoveController.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

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
		DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

		mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

		// Joy
		DUOLGameEngine::GameObject* joyObject = CreateFromFBXModel(TEXT("Capoeira"));

		joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(5.f, 3.f, 0.f));

		joyObject->GetComponent<DUOLGameEngine::Animator>()->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("mixamo.com")));

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

		// 이거 어떻게 밖으로 빼낼지 고려해야합니다 ..
		__super::Awake();
	}
}
