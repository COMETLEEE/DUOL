#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLPhysics/System/PhysicsSystem.h"
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

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

	void ResourceManager::LoadFBXTable(const DUOLCommon::tstring& path)
	{
		auto jsonReader = DUOLJson::JsonReader::GetInstance();

		auto modelTable = jsonReader->LoadJson(path);

		const TCHAR* id = TEXT("ID");

		const TCHAR* resourcePath = TEXT("ResourcePath");

		for (auto& model : modelTable->GetArray())
		{
			if (model.HasMember(id) && model.HasMember(resourcePath))
			{
				const DUOLCommon::tstring& modelStringID = model[id].GetString();

				const DUOLCommon::tstring& modelPath = model[resourcePath].GetString();

				DUOLGraphicsEngine::Model* pModel = _graphicsEngine->CreateModelFromFBX(modelStringID, modelPath);

#pragma region MESH
				// �� �޽��� �� ������
				unsigned meshCount = pModel->GetMeshCount();

				for (unsigned i = 0 ; i < meshCount ; i++)
				{
					DUOLGraphicsEngine::MeshBase* pMesh = pModel->GetMesh(i);

					// ���� �޽��鵵 �и� �����ٵ� ? ���� �� �ʿ�� ���� ?
					std::shared_ptr <DUOLGameEngine::Mesh> engineMesh = std::make_shared<DUOLGameEngine::Mesh>(pMesh->_meshName);

					engineMesh->SetPrimitiveMesh(pMesh);

					_meshIDMap.insert({ pMesh->_meshName, engineMesh });
				}
#pragma endregion
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

		mat = _graphicsEngine->LoadMaterial(_T("Material.001"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Material.001"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Material.001") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("Material"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Material"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Material") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("MariaMat"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("MariaMat"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("MariaMat") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("Debug"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Debug"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("Debug") , sMat });
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

	void ResourceManager::LoadAnimationClipTable(const DUOLCommon::tstring& path)
	{
		// ������
	}

	DUOLGameEngine::Mesh* ResourceManager::GetMesh(const DUOLCommon::tstring& meshID) const
	{
		return _meshIDMap.contains(meshID) ? _meshIDMap.at(meshID).get() : nullptr;
	}

	DUOLGameEngine::Material* ResourceManager::GetMaterial(
		const DUOLCommon::tstring& materialID) const
	{
		return _materialIDMap.contains(materialID) ? _materialIDMap.at(materialID).get() : nullptr;
	}

	DUOLGameEngine::PhysicsMaterial* ResourceManager::GetPhysicsMaterial(
		const DUOLCommon::tstring& physicsMaterialID) const
	{
		return _physicsMaterialIDMap.contains(physicsMaterialID) ? _physicsMaterialIDMap.at(physicsMaterialID).get() : nullptr;
	}

	void ResourceManager::Initialize(const EngineSpecification& gameSpec
	                                 , const std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine>& graphicsEngine
	                                 , const std::shared_ptr<DUOLPhysics::PhysicsSystem>& physicsSystem)
	{
		_graphicsEngine = graphicsEngine;

		_physicsSystem = physicsSystem;

		const DUOLCommon::tstring& projectPath =  gameSpec.projectPath;

#pragma region CLIENT_CODE
		// 1. FBX Table�� �����Ͽ� �ε��մϴ�.
		LoadFBXTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));

		// 2. Material Table�� �����Ͽ� �ε��մϴ�. => �ٵ� �̰� �³� ..? FBX ���Ͽ� �̹� Material list �� �� �ִµ� ..
		LoadMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/MaterialTable.json"));

		// 3. Physics Material Table�� �����Ͽ� �ε��մϴ�.
		LoadPhysicsMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/PhysicsMaterialTable.json"));

		// 4. Load Table�� �����Ͽ� �ε��մϴ�.
		// LoadAnimationClipTable(gameSpec.projectPath + TEXT(""));
#pragma endregion
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