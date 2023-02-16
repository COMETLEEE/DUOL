#include "DUOLGameEngine/Manager/UnityMigrator/UnityMigrator.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include <filesystem>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/SphereCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"


// - 유니티 마이그레이터 제한사항
// 1. 스태틱 메쉬 오브젝트의 경우 게임 오브젝트의 이름이 사용하는 메쉬와 이름이 같아야한다.
//  => 머터리얼도 이미 제작된, 완성된 세트의 경우에만 똑같이 올려서 사용할 수 있다.
// 2. 트랜스폼, 콜라이더, 메쉬, 라이팅, 카메라에 대해서만 


namespace DUOLGameEngine
{
	UnityMigrator::UnityMigrator() :
		_yamlNodeList({})
		, _fileIDList({})
	{
		
	}

	UnityMigrator::~UnityMigrator()
	{
	}

	std::shared_ptr<DUOLGameEngine::Scene> UnityMigrator::MigrateUnitySceneFile(const DUOLCommon::tstring& filePath)
	{
		std::filesystem::path p(DUOLCommon::StringHelper::ToString(filePath));

		// 파일 이름을 씬 이름으로 지정합니다.
		DUOLGameEngine::Scene* scene = new Scene(DUOLCommon::StringHelper::ToTString(p.stem().c_str()));

		std::string stringPath = DUOLCommon::StringHelper::ToString(filePath);

		_yamlNodeList = YAML::LoadAllFromFile(stringPath);

		// 각 유니티 오브젝트 별 파일 아이디를 읽어서 저장한다. 오픈한다.
		FILE* file;

		fopen_s(&file, stringPath.c_str(), "r");

		while (!feof(file))
		{
			fgets(_nLine, 256, file);

			int ch_Index = 0;

			// --- 로 시작하는 경우에 대해서 진행합니다.
			if (_nLine[ch_Index] == '-')
			{
				// &가 나올 때까지 문자 포인터를 이동한다.
				while (_nLine[ch_Index] != '&')
				{
					ch_Index++;
				}

				// 숫자를 읽어오기 위해 간다.
				ch_Index++;

				DUOLCommon::tstring fileID = TEXT("");

				// 숫자들을 모두 읽어 오브젝트 아이디를 만든다.
				while (_nLine[ch_Index] >= '0' && _nLine[ch_Index] <= '9')
				{
					fileID += _nLine[ch_Index];
					ch_Index++;
				}

				_fileIDList.emplace_back(fileID);
			}
		}

		// 다 읽었다. 닫자.
		fclose(file);

		// 게임 오브젝트 생성
		for (int i = 0 ; i < _yamlNodeList.size() ; i++)
		{
			if (_yamlNodeList[i]["GameObject"])
			{
				DUOLGameEngine::GameObject* newGO = scene->CreateEmpty();

				YAML::Node goNode = _yamlNodeList[i]["GameObject"];
				
				newGO->SetName(DUOLCommon::StringHelper::ToTString(goNode["m_Name"].as<std::string>()));

				newGO->SetTag(DUOLCommon::StringHelper::ToTString(goNode["m_TagString"].as<std::string>()));

				newGO->SetLayer(goNode["m_Layer"].as<int>());

				newGO->SetIsActive(goNode["m_IsActive"].as<int>());

				_fileIDObjectMap.insert({ _fileIDList[i], newGO });
			}
		}

		// 컴포넌트 생성
		for (int i = 0; i < _yamlNodeList.size(); i++)
		{
			// Transform Component
			if (_yamlNodeList[i]["Transform"])
			{
				YAML::Node transformNode = _yamlNodeList[i]["Transform"];

				// 오너 게임 오브젝트의 노드
				YAML::Node gameObjectNode = transformNode["m_GameObject"];

				DUOLCommon::tstring ownerGameObject = DUOLCommon::StringHelper::ToTString(gameObjectNode["fileID"].as<std::string>());

				// 해당 오너 게임 오브젝트의 트랜스폼에 대한 정보입니다.
				DUOLGameEngine::Transform* transform = reinterpret_cast<DUOLGameEngine::GameObject*>(_fileIDObjectMap.at(ownerGameObject))->GetComponent<DUOLGameEngine::Transform>();

				// 자식 트랜스폼들
				YAML::Node childrenNode = transformNode["m_Children"];

				std::vector<DUOLCommon::tstring> childrenIDList{};

				for (int i = 0; i < childrenNode.size(); i++)
				{
					childrenIDList.push_back(DUOLCommon::StringHelper::ToTString(childrenNode[i]["fileID"].as<std::string>()));
				}

				// 해당 트랜스폼의 자식 트랜스폼 아이디들을 모아서 일단 저장해놓는다.
				_childrenTransformIDList.push_back({ transform, childrenIDList });

				// Position
				YAML::Node m_LocalPosition = transformNode["m_LocalPosition"];

				transform->SetLocalPosition(Vector3(m_LocalPosition["x"].as<float>(), m_LocalPosition["y"].as<float>(), m_LocalPosition["z"].as<float>()));

				// Rotation
				YAML::Node m_LocalRotation = transformNode["m_LocalRotation"];

				transform->SetRotation(Quaternion(m_LocalRotation["x"].as<float>(), m_LocalRotation["y"].as<float>(), m_LocalRotation["z"].as<float>(), m_LocalRotation["w"].as<float>()));

				// Scale
				YAML::Node m_LocalScale = transformNode["m_LocalScale"];

				transform->SetLocalScale(Vector3(m_LocalScale["x"].as<float>(), m_LocalScale["y"].as<float>(), m_LocalScale["z"].as<float>()));

				_fileIDObjectMap.insert({ _fileIDList[i], transform });
			}
			// Mesh Filter Component => 스태틱 메쉬라는 뜻입니다.
			else if (_yamlNodeList[i]["MeshFilter"])
			{
				// 오너 게임 오브젝트의 '이름과 동일한' 메쉬를 장착해준다. 머터리얼 또한 마찬가지이다.
				YAML::Node meshFilterNode = _yamlNodeList[i]["MeshFilter"];

				// 오너 게임 오브젝트의 노드
				YAML::Node gameObjectNode = meshFilterNode["m_GameObject"];

				DUOLCommon::tstring ownerGameObject = DUOLCommon::StringHelper::ToTString(gameObjectNode["fileID"].as<std::string>());

				DUOLGameEngine::GameObject* owner = reinterpret_cast<DUOLGameEngine::GameObject*>(_fileIDObjectMap.at(ownerGameObject));

				// 게임 오브젝트와 똑같은 이름의 메쉬를 장착한다.
				DUOLGameEngine::Mesh* engineMesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(owner->GetName());

				owner->AddComponent<DUOLGameEngine::MeshFilter>()->SetMesh(engineMesh);

				_fileIDObjectMap.insert({ _fileIDList[i], owner->GetComponent<DUOLGameEngine::MeshFilter>()});
			}
			else if (_yamlNodeList[i]["MeshRenderer"])
			{
				YAML::Node meshRendererNode = _yamlNodeList[i]["MeshRenderer"];

				// 오너 게임 오브젝트의 노드
				YAML::Node gameObjectNode = meshRendererNode["m_GameObject"];

				DUOLCommon::tstring ownerGameObject = DUOLCommon::StringHelper::ToTString(gameObjectNode["fileID"].as<std::string>());

				DUOLGameEngine::GameObject* owner = reinterpret_cast<DUOLGameEngine::GameObject*>(_fileIDObjectMap.at(ownerGameObject));

				DUOLGameEngine::MeshRenderer* meshRenderer = owner->AddComponent<DUOLGameEngine::MeshRenderer>();

				// 해당하는 메쉬 (이미 제작된, 완성된) 에 맞는 머터리얼을 장착해준다.
				DUOLGameEngine::Mesh* engineMesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(owner->GetName());

				for (int subMeshIndex = 0; subMeshIndex < engineMesh->GetPrimitiveMesh()->GetSubMeshCount(); subMeshIndex++)
				{
					meshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(engineMesh->GetPrimitiveMesh()->GetSubMesh(subMeshIndex)->_materialName));
				}
			}
			// Light Component
			else if (_yamlNodeList[i]["Light"])
			{
				YAML::Node lightNode = _yamlNodeList[i]["Light"];

				// 오너 게임 오브젝트의 노드
				YAML::Node gameObjectNode = lightNode["m_GameObject"];

				DUOLCommon::tstring ownerGameObject = DUOLCommon::StringHelper::ToTString(gameObjectNode["fileID"].as<std::string>());

				DUOLGameEngine::GameObject* owner = reinterpret_cast<DUOLGameEngine::GameObject*>(_fileIDObjectMap.at(ownerGameObject));

				DUOLGameEngine::Light* light = owner->AddComponent<Light>();

				// Light Type
				int lightType = lightNode["m_Type"].as<int>();

				switch (lightType)
				{
					case 0:
					{
						light->SetLightType(LightType::Spot);
						break;
					}

					case 1:
					{
						light->SetLightType(LightType::Directional);
						break;	
					}

					case 2:
					{
						light->SetLightType(LightType::Point);
						break;
					}
				}

				// Enable
				light->SetIsEnabled(lightNode["m_Enabled"].as<int>());

				// Color
				YAML::Node m_Color = lightNode["m_Color"];

				light->SetColor(Vector3(m_Color["r"].as<float>(), m_Color["g"].as<float>(), m_Color["b"].as<float>()));

				// Intensity
				light->SetIntensity(lightNode["m_Intensity"].as<float>());

				// Range
				light->SetRange(lightNode["m_Range"].as<float>());

				// TODO : 추가 예쁜 디자인을 위해서 기능이 추가되어야 합니다.
			}
			// Box Collider Component
			else if (_yamlNodeList[i]["BoxCollider"])
			{
				YAML::Node boxColliderNode = _yamlNodeList[i]["BoxCollider"];

				// 오너 게임 오브젝트의 노드
				YAML::Node gameObjectNode = boxColliderNode["m_GameObject"];

				DUOLCommon::tstring ownerGameObject = DUOLCommon::StringHelper::ToTString(gameObjectNode["fileID"].as<std::string>());

				DUOLGameEngine::GameObject* owner = reinterpret_cast<DUOLGameEngine::GameObject*>(_fileIDObjectMap.at(ownerGameObject));

				DUOLGameEngine::BoxCollider* boxCollider = owner->AddComponent<DUOLGameEngine::BoxCollider>();

				// Enable
				boxCollider->SetIsEnabled(boxColliderNode["m_Enabled"].as<int>());

				// Trigger
				boxCollider->SetIsTrigger(boxColliderNode["m_IsTrigger"].as<int>());

				// Size
				YAML::Node m_Size = boxColliderNode["m_Size"];

				boxCollider->SetSize(Vector3(m_Size["x"].as<float>(), m_Size["y"].as<float>(), m_Size["z"].as<float>()));

				// Center
				YAML::Node m_Center = boxColliderNode["m_Center"];

				boxCollider->SetCenter(Vector3(m_Center["x"].as<float>(), m_Center["y"].as<float>(), m_Center["z"].as<float>()));
			}
			// Sphere Collider Component
			else if (_yamlNodeList[i]["SphereCollider"])
			{
				YAML::Node sphereColliderNode = _yamlNodeList[i]["SphereCollider"];

				// 오너 게임 오브젝트의 노드
				YAML::Node gameObjectNode = sphereColliderNode["m_GameObject"];

				DUOLCommon::tstring ownerGameObject = DUOLCommon::StringHelper::ToTString(gameObjectNode["fileID"].as<std::string>());

				DUOLGameEngine::GameObject* owner = reinterpret_cast<DUOLGameEngine::GameObject*>(_fileIDObjectMap.at(ownerGameObject));

				DUOLGameEngine::SphereCollider* sphereCollider = owner->AddComponent<DUOLGameEngine::SphereCollider>();
			}
			// Capsule Collider Component
			else if (_yamlNodeList[i]["CapsuleCollider"])
			{
				YAML::Node capsuleColliderNode = _yamlNodeList[i]["CapsuleCollider"];

				// 오너 게임 오브젝트의 노드
				YAML::Node gameObjectNode = capsuleColliderNode["m_GameObject"];

				DUOLCommon::tstring ownerGameObject = DUOLCommon::StringHelper::ToTString(gameObjectNode["fileID"].as<std::string>());

				DUOLGameEngine::GameObject* owner = reinterpret_cast<DUOLGameEngine::GameObject*>(_fileIDObjectMap.at(ownerGameObject));

				DUOLGameEngine::CapsuleCollider* capsuleCollider = owner->AddComponent<DUOLGameEngine::CapsuleCollider>();

				// Enable
				capsuleCollider->SetIsEnabled(capsuleColliderNode["m_Enabled"].as<int>());

				// Trigger
				capsuleCollider->SetIsTrigger(capsuleColliderNode["m_IsTrigger"].as<int>());

				// Radius
				capsuleCollider->SetRadius(capsuleColliderNode["m_Radius"].as<float>());

				// Height
				capsuleCollider->SetHeight(capsuleColliderNode["m_Height"].as<float>());

				// Center
				YAML::Node m_Center = capsuleColliderNode["m_Center"];

				capsuleCollider->SetCenter(DUOLMath::Vector3{ m_Center["x"].as<float>(), m_Center["y"].as<float>(), m_Center["z"].as<float>() });

				// Direction
				int direction = capsuleColliderNode["m_Direction"].as<int>();

				capsuleCollider->SetDirection(static_cast<DUOLGameEngine::CapsuleDirection>(direction));
			}
			// Camera Component
			else if (_yamlNodeList[i]["Camera"])
			{
				YAML::Node cameraNode = _yamlNodeList[i]["Camera"];

				// 오너 게임 오브젝트의 노드
				YAML::Node gameObjectNode = cameraNode["m_GameObject"];

				DUOLCommon::tstring ownerGameObject = DUOLCommon::StringHelper::ToTString(gameObjectNode["fileID"].as<std::string>());

				DUOLGameEngine::GameObject* owner = reinterpret_cast<DUOLGameEngine::GameObject*>(_fileIDObjectMap.at(ownerGameObject));

				DUOLGameEngine::Camera* camera = owner->AddComponent<DUOLGameEngine::Camera>();

				// Enabled
				camera->SetIsEnabled(cameraNode["m_Enabled"].as<int>());

				// Near
				camera->SetNear(cameraNode["near clip plane"].as<float>());

				// Far
				camera->SetFar(cameraNode["far clip plane"].as<float>());

				// FOV
				camera->SetFOV(cameraNode["field of view"].as<float>());

				// Orthographic
				camera->SetIsOrthographic(cameraNode["orthographic"].as<int>());
			}
			// Reflection class by name.
			else
			{
				
			}
		}





		// Transform children mapping.
		for (auto pair : _childrenTransformIDList)
		{
			DUOLGameEngine::Transform* transform = pair.first;

			std::vector<DUOLCommon::tstring> childrenID = pair.second;

			for (int i = 0 ; i < childrenID.size(); i++)
			{
				if (_fileIDObjectMap.contains(childrenID[i]))
				{
					DUOLGameEngine::Transform* child = reinterpret_cast<DUOLGameEngine::Transform*>(_fileIDObjectMap.at(childrenID[i]));

					child->SetParent(transform);
				}
			}
		}










		// 마지막으로, 사용한 자료 구조들 정리
		_fileIDObjectMap.clear();

		_yamlNodeList.clear();

		_fileIDList.clear();

		return std::shared_ptr<DUOLGameEngine::Scene>(scene);
	}

	std::shared_ptr<DUOLGameEngine::AnimatorController> UnityMigrator::MigrateUnityAnimatorController(const DUOLCommon::tstring& filePath)
	{
		return nullptr;
	}
}
