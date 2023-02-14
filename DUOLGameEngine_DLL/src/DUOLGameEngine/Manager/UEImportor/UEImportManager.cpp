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
        // ����, �� ���� ���� SceneManager�� �ͼӵǾ� Ȱ���մϴ�.
	}

	void UEImportManager::UnInitialize()
	{
        // ����, �� ���� ���� SceneManager�� �ͼӵǾ� Ȱ���մϴ�.
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

        // �𸮾� ������ ��Ÿ���ϴ�.
        GameObject* _root = scene->CreateEmpty();

        _root->SetName(_fileName);

        // 1. Static Mesh Component
        auto _staticMeshActors = _level.GetActorsHasComponent("StaticMeshComponent");
        for (auto a : _staticMeshActors)
        {
            auto _components = a.GetComponentsOf("StaticMeshComponent");
            auto _c = _components[0];

            auto _staticMeshComponent = UELevelParser::Component_StaticMeshComponent::Parse(_c);
            if (_staticMeshComponent.meshFileNameWithoutExtension.empty())
                continue;

            const DUOLCommon::tstring _meshName = DUOLCommon::StringHelper::ToTString(_staticMeshComponent.meshFileNameWithoutExtension);

            Vector3& _worldPosition = _staticMeshComponent.relativeLocation;
            Vector3 _worldRotation = _staticMeshComponent.relativeRotation;
            Vector3& _worldScale = _staticMeshComponent.relativeScale3D;

            // �� .. �ϳ��� ȣ��� ���͸���� �޽�
            auto _go = scene->CreateFromFBXMesh(_meshName);

            _go->SetName(DUOLCommon::StringHelper::ToTString(a.name));

            auto transform = _go->GetTransform();

            // �𸮾� �� ���� �Ľ��� ��Ÿ�� �ϳ��� ��Ʈ ������Ʈ�� �־��ݴϴ�.
            transform->SetParent(_root->GetTransform());

            // ��ǥ�� ��ȯ
            transform->SetLocalScale(_worldScale);
            transform->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::Up, -90.0f) * Quaternion::CreateFromYawPitchRoll(_worldRotation.x, _worldRotation.y, _worldRotation.z), Space::Self);
            transform->SetPosition(_worldPosition, Space::Self);
        }




















        // ���� ��Ʈ�� ��ε�
        buffer.clear();

        _fs.close();
	}
}