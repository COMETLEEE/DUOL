#include "DUOLGameEngine/Manager/UEImportor/UEImportManager.h"

#include <filesystem>

#include "DUOLGameEngine/Manager/UEImportor/UELevelParser.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGameEngine
{
	UEImportManager::UEImportManager()
	{
		
	}

	void UEImportManager::Initialize()
	{
        // 딱히, 할 일은 없고 SceneManager에 귀속되어 활용합니다.
	}

	void UEImportManager::UnInitialize()
	{
        // 딱히, 할 일은 없고 SceneManager에 귀속되어 활용합니다.
	}

	UEImportManager::~UEImportManager()
	{
	}

	void UEImportManager::Import(DUOLGameEngine::Scene* scene, const DUOLCommon::tstring& filePath)
	{
        const DUOLCommon::tstring _fileName = std::filesystem::path{ filePath }.filename();

        std::ifstream _fs(filePath);
        std::stringstream buffer;
        buffer << _fs.rdbuf();

        UELevelParser::Level _level = UELevelParser::Parse(buffer);

        // 언리얼 레벨을 나타냅니다.
        GameObject* _root = scene->CreateEmpty();

        _root->SetName(_fileName);

        // StaticMeshComponent
        auto _staticMeshActors = _level.GetActorsHasComponent("StaticMeshComponent");
        for (auto a : _staticMeshActors)
        {
            auto _components = a.GetComponentsOf("StaticMeshComponent");
            auto _c = _components[0];

            auto _staticMeshComponent = UELevelParser::Component_StaticMeshComponent::Parse(_c);
            if (_staticMeshComponent.meshFileNameWithoutExtension.empty())
                continue;

            const DUOLCommon::tstring _fileName = DUOLCommon::StringHelper::ToTString(_staticMeshComponent.meshFileNameWithoutExtension) + TEXT(".fbx");

            Vector3& _worldPosition = _staticMeshComponent.relativeLocation;
            Vector3 _worldRotation = _staticMeshComponent.relativeRotation;
            Vector3& _worldScale = _staticMeshComponent.relativeScale3D;

            auto _go = scene->CreateFromFBXModel(_fileName);

            _go->SetName(DUOLCommon::StringHelper::ToTString(a.name));

            auto transform = _go->GetTransform();

            transform->SetParent(_root->GetTransform());
            transform->SetLocalScale(_worldScale);
            transform->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::Up, -90.0f) * Quaternion::CreateFromYawPitchRoll(_worldRotation.x, _worldRotation.y, _worldRotation.z), Space::Self);
            transform->SetPosition(_worldPosition, Space::Self);
        }

        buffer.clear();

        _fs.close();
	}
}