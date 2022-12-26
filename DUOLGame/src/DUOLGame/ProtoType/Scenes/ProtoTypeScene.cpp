#include "DUOLGame/ProtoType/Scenes/ProtoTypeScene.h"

#include "DUOLGame/ProtoType/Objects/PlayerCharacter.h"
#include "DUOLGame/ProtoType/Scripts/CameraController.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/MeshCollider.h"

#include "DUOLGame/ProtoType/Scripts/Plane.h"

namespace DUOLGame
{
	ProtoTypeScene::ProtoTypeScene() :
		Scene(_T("ProtoTypeScene"))
	{

	}

	ProtoTypeScene::~ProtoTypeScene()
	{

	}

	DUOLMath::Matrix LookAt(const DUOLMath::Vector3& eye, const DUOLMath::Vector3& target)
	{
		auto look = target - eye;

		look.Normalize();

		auto up = DUOLMath::Vector3::Up;

		auto right = up;

		up.Cross(look, right);

		right.Normalize();

		look.Cross(right, up);

		up.Normalize();

		return Matrix{};
	}

	void ProtoTypeScene::Awake()
	{
		// ----------- Main Camera -----------
		DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->GetTransform()->SetPosition({ 0.0f, 1.0f, -5.0f }, Space::Self);

		//mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

		//PlayerCharacter player(CreateFromFBXModel(_T("Firing Rifle")));
		//
		//auto cameraController = CreateEmpty();
		//
		//mainCamObject->GetTransform()->LookAt(cameraController->GetTransform());
		//
		//mainCamObject->GetTransform()->SetParent(cameraController->GetTransform());
		//
		//cameraController->SetName(_T("CameraController"));
		//
		//player.AttachCamera(cameraController);

		DUOLGameEngine::GameObject* plane = CreateFromFBXModel(TEXT("Test1_DUOL"));

		plane->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(10.f, -15.f, 0.f));

		plane->AddComponent<BoxCollider>();
		plane->AddComponent<Plane>();

		auto&& children = plane->GetTransform()->GetChildren();

		for (auto& child : children)
		{
			child->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("WorldGridMaterial")));
		}

		__super::Awake();
	}
}