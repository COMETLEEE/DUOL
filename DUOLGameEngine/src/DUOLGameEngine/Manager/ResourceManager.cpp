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

				// Graphics의 CreateMesh 호출 / 받은 해당 포인터와 IO를 매핑하여 보관
				DUOLGraphicsEngine::Mesh* pMesh = _graphicsEngine->CreateMesh(meshStringID, meshPath);

				_meshIDMap.insert({ meshStringID, pMesh });
			}
		}
	}

	DUOLGraphicsEngine::Mesh* ResourceManager::GetMeshByID(const DUOLCommon::tstring& meshID) const
	{
		// 그냥 에러 내버리는 것도 좋을 것 같은데 .. 완전 비정상적인 접근이니까
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

		// 1. Mesh Table을 참조하여 로드합니다.
		LoadMeshTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));
	}

	void ResourceManager::UnInitialize()
	{
		// Mesh Delete 해버리기는 조금 문제가 생길 수 있을 것 같다.
		
	}

	void ResourceManager::Update(float deltaTime)
	{

	}
}