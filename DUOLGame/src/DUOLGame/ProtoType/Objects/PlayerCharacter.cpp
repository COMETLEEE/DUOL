#include "DUOLGame/ProtoType/Objects/PlayerCharacter.h"

#include "DUOLGame/ProtoType/Scripts/CharacterController.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/SphereCollider.h"

namespace DUOLGame
{
	PlayerCharacter::PlayerCharacter(GameObject* entity) :
		_entity(entity)
	{
		_entity->GetComponent<Transform>()->SetPosition({ 0.0f, 40.0f, 0.0f });
		_entity->GetComponent<Transform>()->SetLocalScale({ 0.1f, 0.1f, 0.1f });

		_entity->AddComponent<MeshFilter>()->
			SetMesh(ResourceManager::GetInstance()->GetMesh(_T("Maria_J_J_Ong")));

		_entity->AddComponent<MeshRenderer>()->
			AddMaterial(ResourceManager::GetInstance()->GetMaterial(_T("Material")));

		_entity->AddComponent<Rigidbody>();

		_entity->GetComponent<Rigidbody>()->SetIsFreezeRotation(true);

		_entity->AddComponent<CapsuleCollider>();
		//_entity->AddComponent<BoxCollider>();
		//_entity->AddComponent<SphereCollider>();

		_entity->AddComponent<CharacterController>();
	}

	PlayerCharacter::~PlayerCharacter()
	{

	}

	void PlayerCharacter::AttachCamera(GameObject* camera)
	{
		camera->GetComponent<Transform>()->SetParent(_entity->GetComponent<Transform>());
	}
}