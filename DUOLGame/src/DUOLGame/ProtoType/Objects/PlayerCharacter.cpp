#include "DUOLGame/ProtoType/Objects/PlayerCharacter.h"

#include "DUOLGame/ProtoType/Scripts/PlayerState.h"
#include "DUOLGame/ProtoType/Scripts/CameraController.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/SphereCollider.h"

namespace DUOLGame
{
	PlayerCharacter::PlayerCharacter(GameObject* entity) :
		_entity(entity)
		, _cameraController(nullptr)
		, _cameraMatrix(DUOLMath::Matrix::Identity)
	{
		_entity->SetTag(_T("Player"));

		//_entity->AddComponent<MeshFilter>()->
		//	SetMesh(ResourceManager::GetInstance()->GetMesh(_T("Maria_J_J_Ong")));
		//
		//_entity->AddComponent<MeshRenderer>()->
		//	AddMaterial(ResourceManager::GetInstance()->GetMaterial(_T("Material")));

		_entity->AddComponent<Rigidbody>();

		_entity->GetComponent<Rigidbody>()->SetIsFreezeRotation(true);

		_entity->AddComponent<CapsuleCollider>();

		//_entity->AddComponent<BoxCollider>();
		//_entity->AddComponent<SphereCollider>();

		_entity->AddComponent<CameraController>();

		_entity->AddComponent<PlayerState>();

		_entity->GetComponent<DUOLGameEngine::Animator>()->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("Idle")));

		auto children = _entity->GetTransform()->GetChildren();

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
			else
				continue;
		}
	}

	PlayerCharacter::~PlayerCharacter()
	{

	}

	void PlayerCharacter::AttachCamera(GameObject* camera)
	{
		_cameraController = camera;

		camera->GetComponent<Transform>()->SetParent(_entity->GetComponent<Transform>());
	}

	void PlayerCharacter::SetPlayerActive(bool value)
	{
		_entity->GetComponent<PlayerState>()->SetActive(value);

		_entity->GetComponent<CameraController>()->SwitchingCameraMode();

		auto children = _cameraController->GetTransform()->GetChildGameObjects();

		Transform* cameraTransform = nullptr;

		for (auto& child : children)
		{
			if (child->GetName().compare(_T("MainCamera")) == 0)
			{
				cameraTransform = child->GetTransform();

				break;
			}
		}

		if (value == true)
			cameraTransform->GetTransform()->SetLocalTM(_cameraMatrix);
		else
			_cameraMatrix = cameraTransform->GetTransform()->GetLocalMatrix();
	}

	GameObject* PlayerCharacter::GetCameraController()
	{
		return _cameraController;
	}
}