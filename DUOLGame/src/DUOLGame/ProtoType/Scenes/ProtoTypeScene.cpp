#include "DUOLGame/ProtoType/Scenes/ProtoTypeScene.h"

#include "DUOLGame/ProtoType/Objects/PlayerCharacter.h"
#include "DUOLGame/ProtoType/Scripts/CameraController.h"
#include "DUOLGame/ProtoType/Scripts/SceneController.h"

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

	void ProtoTypeScene::Awake()
	{
		auto sceneController = CreateEmpty()->AddComponent<SceneController>();

		// ----------- Main Camera -----------
		DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

		mainCamObject->SetName(_T("MainCamera"));

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->GetTransform()->SetPosition({ 0.0f, 0.0f, -10.0f }, Space::Self);
		
		/* Camera Controller */
		auto cameraController = CreateEmpty();
		
		mainCamObject->GetTransform()->LookAt(cameraController->GetTransform());
		
		mainCamObject->GetTransform()->SetParent(cameraController->GetTransform());
		
		cameraController->SetName(_T("CameraController"));
		 
		/* Player */
		PlayerCharacter* player = new PlayerCharacter(CreateFromFBXModel(_T("JoyIdle")));

		sceneController->SetPlayer(player);
		
		player->AttachCamera(cameraController);

		/* Dummy */
		//auto dummy = CreateFromFBXModel(_T("JoyIdle"));
		//
		//dummy->AddComponent<MeshCollider>()->SetMeshBuffer(_T("JoyIdle"));
		//
		//dummy->GetTransform()->SetPosition({ 10.0f, 0.0f, 0.0f });

		/* Map */
		DUOLGameEngine::GameObject* plane = CreateFromFBXModel(TEXT("Test1_DUOL"));

		plane->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(10.f, -10.f, 0.f));

		plane->AddComponent<BoxCollider>();
		plane->AddComponent<Plane>();

		//plane->AddComponent<MeshCollider>()->SetMeshBuffer(_T("Test1_DUOL"));

		__super::Awake();
	}
}