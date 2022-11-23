#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLPhysics/System/PhysicsSystem.h"
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/ECS/Object/PhysicsMaterial.h"

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

				std::shared_ptr<DUOLGameEngine::Mesh> engineMesh = std::make_shared<DUOLGameEngine::Mesh>(meshStringID);
 
				engineMesh->SetPrimitiveMesh(pMesh);

				_meshIDMap.insert({ meshStringID, engineMesh });
			}
		}
	}

	void ResourceManager::LoadMaterialTable(const DUOLCommon::tstring& path)
	{
		// TODO : �ϴ� Joy�� �ε��ϱ� ���� �ϵ��ڵ��Դϴ� .. ���͸��� ���� ������ �� �ʿ��� �� .. ��
		DUOLGraphicsEngine::Material* mat;

		std::shared_ptr<DUOLGameEngine::Material> sMat;

		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Scarf_MAT"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Scarf_MAT"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Boy01_Scarf_MAT"), sMat });


		mat = _graphicsEngine->LoadMaterial(_T("Boy01_LowerBody_MAT"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_LowerBody_MAT"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Boy01_LowerBody_MAT") , sMat });


		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Hair_MAT"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Hair_MAT"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Boy01_Hair_MAT") , sMat });


		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Shoes_MAT"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Shoes_MAT"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Boy01_Shoes_MAT") , sMat });


		mat = _graphicsEngine->LoadMaterial(_T("Boy01_UpperBody_MAT"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_UpperBody_MAT"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Boy01_UpperBody_MAT") , sMat });


		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Hands_MAT"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Hands_MAT"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Boy01_Hands_MAT") , sMat });


		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Head_MAT"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Head_MAT"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Boy01_Head_MAT") , sMat });
	}

	void ResourceManager::LoadPhysicsMaterialTable(const DUOLCommon::tstring& path)
	{
		// TODO : Physics Material �� �� �ƴ϶� ��� ���ҽ��� Asset������ �Ͽ�ȭ ..
		const DUOLPhysics::PhysicsMaterialDesc matDesc { 0.5f, 0.5f, 0.5f };

		std::weak_ptr<DUOLPhysics::PhysicsMaterial> pMat = _physicsSystem->CreateMaterial(TEXT("Default"), matDesc);

		std::shared_ptr<DUOLGameEngine::PhysicsMaterial> pMatEngine = std::make_shared<DUOLGameEngine::PhysicsMaterial>(TEXT("Default"));

		pMatEngine->SetPhysicsMaterial(pMat);

		_physicsMaterialIDMap.insert({ TEXT("Default"), pMatEngine });
	}

	const std::shared_ptr<DUOLGameEngine::Mesh>& ResourceManager::GetMesh(const DUOLCommon::tstring& meshID) const
	{
		return _meshIDMap.contains(meshID) ? _meshIDMap.at(meshID) : nullptr;
	}

	const std::shared_ptr<DUOLGameEngine::Material>& ResourceManager::GetMaterial(
		const DUOLCommon::tstring& materialID) const
	{
		return _materialIDMap.contains(materialID) ? _materialIDMap.at(materialID) : nullptr;
	}

	const std::shared_ptr<DUOLGameEngine::PhysicsMaterial>& ResourceManager::GetPhysicsMaterial(
		const DUOLCommon::tstring& physicsMaterialID) const
	{
		return _physicsMaterialIDMap.contains(physicsMaterialID) ? _physicsMaterialIDMap.at(physicsMaterialID) : nullptr;
	}

	void ResourceManager::Initialize(const EngineSpecification& gameSpec
	                                 , const std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine>& graphicsEngine
	                                 , const std::shared_ptr<DUOLPhysics::PhysicsSystem>& physicsSystem)
	{
		_graphicsEngine = graphicsEngine;

		_physicsSystem = physicsSystem;

		const DUOLCommon::tstring& projectPath =  gameSpec.projectPath;

		// 1. Mesh Table�� �����Ͽ� �ε��մϴ�.
		LoadMeshTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));

		// 2. Material Table�� �����Ͽ� �ε��մϴ�. => �ٵ� �̰� �³� ..? FBX ���Ͽ� �̹� Material list �� �� �ִµ� ..
		LoadMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/MaterialTable.json"));

		// 3. Physics Material Table�� �����Ͽ� �ε��մϴ�.
		LoadPhysicsMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/PhysicsMaterialTable.json"));
	}

	void ResourceManager::UnInitialize()
	{
		// Mesh Delete �ع������ ���� ������ ���� �� ���� �� ����.

		// TODO : ���� ���� ���ο��� ����ϰ� �ִ� Resource Load - UnLoad ����.
		
	}

	void ResourceManager::Update(float deltaTime)
	{

	}
}