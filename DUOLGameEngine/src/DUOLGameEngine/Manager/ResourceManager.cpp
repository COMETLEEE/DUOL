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

				//TODO 수정필요한 곳!!

				DUOLGraphicsEngine::Model* pMesh = _graphicsEngine->CreateModelFromFBX(meshStringID, meshPath);

				//// 게임 엔진에서 참조하는 Mesh 리소스 형태로 래핑.
				//std::shared_ptr<DUOLGameEngine::Mesh> engineMesh = std::make_shared<DUOLGameEngine::Mesh>(meshStringID);

				//engineMesh->SetPrimitiveMesh(pMesh);

				//_meshIDMap.insert({ meshStringID, engineMesh });
			}
		}
	}

	void ResourceManager::LoadMaterialTable(const DUOLCommon::tstring& path)
	{
		// TODO : 일단 Joy를 로드하기 위한 하드코딩입니다 .. 머터리얼 관련 정리는 꼭 필요할 듯 .. ㅠ
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
		// TODO : Physics Material 뿐 만 아니라 모든 리소스들 Asset으로의 일원화 ..
		const DUOLPhysics::PhysicsMaterialDesc matDesc { 0.5f, 0.5f, 0.5f };

		std::weak_ptr<DUOLPhysics::PhysicsMaterial> pMat = _physicsSystem->CreateMaterial(TEXT("Default"), matDesc);

		std::shared_ptr<DUOLGameEngine::PhysicsMaterial> pMatEngine = std::make_shared<DUOLGameEngine::PhysicsMaterial>(TEXT("Default"));

		pMatEngine->SetPhysicsMaterial(pMat);

		_physicsMaterialIDMap.insert({ TEXT("Default"), pMatEngine });
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
		// 1. FBX Table을 참조하여 로드합니다.
		LoadFBXTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));

		// 2. Material Table을 참조하여 로드합니다. => 근데 이게 맞나 ..? FBX 파일에 이미 Material list 다 들어가 있는데 ..
		LoadMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/MaterialTable.json"));

		// 3. Physics Material Table을 참조하여 로드합니다.
		LoadPhysicsMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/PhysicsMaterialTable.json"));
#pragma endregion
	}

	void ResourceManager::UnInitialize()
	{
		// Mesh Delete 해버리기는 조금 문제가 생길 수 있을 것 같다.

		// TODO : 게임 엔진 내부에서 사용하고 있는 Resource Load - UnLoad 관리.
		
	}

	void ResourceManager::Update(float deltaTime)
	{

	}
}