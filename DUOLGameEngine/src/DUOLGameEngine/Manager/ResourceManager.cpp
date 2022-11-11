#include "DUOLGameEngine/Manager/ResourceManager.h"

#include <DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h>

#include "DUOLJson/JsonReader.h"

namespace DUOLGameEngine
{
	ResourceManager::ResourceManager()
	{
		
	}

	ResourceManager::~ResourceManager()
	{

	}

	void ResourceManager::LoadMeshTable(const DUOLCommon::tstring& path)
	{
		auto jsonReader = DUOLJson::JsonReader::GetInstance();

		auto meshTable = jsonReader->LoadJson(path);

		const TCHAR* id = TEXT("ID");

		const TCHAR* resourcePath = TEXT("ResourcePath");

		for (auto& mesh : meshTable->GetArray())
		{
			if (mesh.HasMember(id) && mesh.HasMember(resourcePath))
			{
				const DUOLCommon::tstring& meshStringID = mesh[id].GetString();

				const DUOLCommon::tstring& meshPath = mesh[resourcePath].GetString();

				// Graphics�� CreateMesh ȣ�� / ���� �ش� �����Ϳ� IO�� �����Ͽ� ����
				DUOLGraphicsEngine::Mesh* pMesh = _graphicsEngine->CreateMesh(meshStringID, meshPath);

				_meshIDMap.insert({ meshStringID, pMesh });
			}
		}
	}

	DUOLGraphicsEngine::Mesh* ResourceManager::GetMeshByID(const DUOLCommon::tstring& meshID) const
	{
		// �׳� ���� �������� �͵� ���� �� ������ .. ���� ���������� �����̴ϱ�
		return _meshIDMap.contains(meshID) ? _meshIDMap.at(meshID) : nullptr;
	}

	DUOLGraphicsEngine::Material* ResourceManager::GetMaterialByID(const DUOLCommon::tstring& materialID) const
	{
		return _graphicsEngine->LoadMaterial(materialID);
		//return _materialIDMap.contains(materialID) ? _materialIDMap.at(materialID) : nullptr;
	}

	void ResourceManager::Initialize(const EngineSpecification& gameSpec,
	                                 std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine> graphicsEngine)
	{
		_graphicsEngine = std::move(graphicsEngine);

		const DUOLCommon::tstring& projectPath =  gameSpec.projectPath;

		// 1. Mesh Table�� �����Ͽ� �ε��մϴ�.
		LoadMeshTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));
	}

	void ResourceManager::UnInitialize()
	{
		// Mesh Delete �ع������ ���� ������ ���� �� ���� �� ����.
		
	}

	void ResourceManager::Update(float deltaTime)
	{

	}
}