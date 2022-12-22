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

		// ----------- Marias -----------
		//{

		//	DUOLGameEngine::GameObject* button = CreateEmpty();

		//	button->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(0.f, 5.f, 5.f));

		//	button->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		//	button->AddComponent<DUOLGameEngine::MeshFilter>()->
		//		SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("ButtonFloor")));

		//	button->AddComponent<DUOLGameEngine::MeshRenderer>();

		//	auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Material"));

		//	button->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);
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

#pragma region PHYSICS
		//DUOLGameEngine::GameObject* trigger = CreateEmpty();

		//trigger->AddComponent<DUOLGameEngine::Rigidbody>();

		//trigger->GetTransform()->SetPosition(DUOLMath::Vector3(25.f, 0.f, 0.f));

		//trigger->GetTransform()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		//// ??? ???????? ????? ??? ?????
		//trigger->AddComponent<DUOLGameEngine::MeshFilter>()->
		//	SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Maria_J_J_Ong")));

		//trigger->AddComponent<DUOLGameEngine::MeshRenderer>();

		//auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("MariaMat"));

		//trigger->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);

		//trigger->AddComponent<DUOLGameEngine::BoxCollider>();

		//trigger->AddComponent<DUOLGame::MoveController>();

		//trigger->AddComponent<DUOLGame::PhysicsEventTest>();

		//DUOLGameEngine::GameObject* trigger2 = CreateEmpty();

		//// ??? ??? Dynamic V.S. Dynamic (OnCollisionXXX)
		//// trigger2->AddComponent<DUOLGameEngine::Rigidbody>();

		//trigger2->GetTransform()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

		//// ??? ???????? ????? ??? ?????
		//trigger2->AddComponent<DUOLGameEngine::MeshFilter>()->
		//	SetMesh(DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Maria_J_J_Ong")));

		//trigger2->AddComponent<DUOLGameEngine::MeshRenderer>();

		//trigger2->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(mat);

		//trigger2->AddComponent<DUOLGameEngine::BoxCollider>();

		//trigger2->AddComponent<DUOLGame::PhysicsEventTest>();
#pragma endregion

#pragma region CAPOEIRA
		// Joy
		{
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
		}
#pragma endregion

#pragma region BUTTON_FLOOR
		// Joy
	/*	{
			DUOLGameEngine::GameObject* button = CreateFromFBXModel(TEXT("ButtonFloor"));
			button->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(10.f, 5.f, 0.f));
			button->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));

			auto&& children = button->GetTransform()->GetChildren();

			for (auto& child : children)
			{
				child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("button_metallic")));
				child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("base case")));
				child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("screen sign")));
				child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("glowing stripes")));
				child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("fixator_case")));
				child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("fixator_glass")));
			}
		}*/
#pragma endregion

#pragma region GRAY_BOXING
		/*DUOLGameEngine::GameObject* object1 = CreateFromFBXModel(TEXT("A_TEST"));

		object1->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(10.f, 5.f, 0.f));

		auto&& children = object1->GetTransform()->GetChildren();

		for (auto& child : children)
		{
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("steel")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("hwan")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("STONE")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("GLASS")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("BLOCK")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("stone.002")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("image.006")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("image.003")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("image.002")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("image.005")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("block.002")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("image.004")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("image")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("block.001")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("image.001")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("house1.001")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("house1")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("stone.001")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("stone.003")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("block")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("stone")));
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("wood")));
		}*/
#pragma endregion

		__super::Awake();
	}
}