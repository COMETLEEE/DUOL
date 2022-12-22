#include "DUOLGame/ProtoType/Scenes/ProtoTypeScene.h"

#include "DUOLGame/ProtoType/Objects/PlayerCharacter.h"
#include "DUOLGame/ProtoType/Scripts/CharacterController.h"

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
		// ----------- Main Camera -----------
		DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		//mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

		PlayerCharacter player(CreateFromFBXModel(_T("Capoeira")));

		player.AttachCamera(mainCamObject);

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