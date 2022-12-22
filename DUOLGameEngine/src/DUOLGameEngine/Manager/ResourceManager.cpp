#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLPhysics/System/PhysicsSystem.h"
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/ECS/Object/PhysicsMaterial.h"
#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGameEngine/ECS/Object/Avatar.h"

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

				// TODO: �׷��Ƚ� ���ҽ� ��ü ����ȭ �� �� ���̶�� ������ ��� ���� ���� ���ΰ� ..?
				_modelIDMap.insert({ modelStringID, pModel });

#pragma region MESH
				// �� �޽��� �� �������� ���ؼ� ..
				unsigned meshCount = pModel->GetMeshCount();

				for (unsigned i = 0 ; i < meshCount ; i++)
				{
					DUOLGraphicsEngine::MeshBase* pMesh = pModel->GetMesh(i);

					// ���� �޽��鵵 �и� �����ٵ� ? ���� �� �ʿ�� ���� ?
					std::shared_ptr<DUOLGameEngine::Mesh> engineMesh = std::make_shared<DUOLGameEngine::Mesh>(pMesh->_meshName);

					engineMesh->SetPrimitiveMesh(pMesh);

					_meshIDMap.insert({ pMesh->_meshName, engineMesh });
				}
#pragma endregion

#pragma region AVATAR
				// �ε��� FBX Model�� Skinning Model �̶�� ..
				if (pModel->GetIsSkinningModel())
				{
					// �ش� ���� �� ������ �̿��ؼ�
					// Avatar resource object�� ����ϴ�. (�ϴ� �ܼ��� �� ������ ����ϰ� ����)
					std::shared_ptr<DUOLGameEngine::Avatar> avatar = std::make_shared<Avatar>(modelStringID);

					avatar->SetPrimitiveBones(&pModel->GetBones());

					_avatarIDMap.insert({ modelStringID, avatar });
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

#pragma region JOY_MATERIAL
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
#pragma endregion

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


#pragma region BUTTON_FLOOR
		mat = _graphicsEngine->LoadMaterial(_T("button_metallic"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("button_metallic"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("button_metallic") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("base case"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("base case"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("base case") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("screen sign"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("screen sign"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("screen sign") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("glowing stripes"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("glowing stripes"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("glowing stripes") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("fixator_case"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("fixator_case"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("fixator_case") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("fixator_tube"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("fixator_tube"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("fixator_tube") , sMat });

		mat = _graphicsEngine->LoadMaterial(_T("fixator_glass"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("fixator_glass"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("fixator_glass") , sMat });
#pragma endregion

//#pragma region DUOLDATA
//		mat = _graphicsEngine->LoadMaterial(_T("WorldGridMaterial"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("WorldGridMaterial"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("WorldGridMaterial") , sMat });

//#pragma endregion

//#pragma region A_DUOLDATA2
//		mat = _graphicsEngine->LoadMaterial(_T("steel"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("steel"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("steel") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("hwan"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("hwan"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("hwan") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("STONE"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("STONE"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("STONE") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("GLASS"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("GLASS"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("GLASS") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("BLOCK"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("BLOCK"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("BLOCK") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("stone.002"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("stone.002"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("stone.002") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("image.006"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("image.006"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("image.006") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("image.003"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("image.003"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("image.003") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("image.002"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("image.002"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("image.002") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("image.005"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("image.005"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("image.005") , sMat });
//
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("block.002"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("block.002"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("block.002") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("image.004"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("image.004"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("image.004") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("image"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("image"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("image") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("block.001"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("block.001"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("block.001") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("image.001"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("image.001"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("image.001") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("house1.001"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("house1.001"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("house1.001") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("house1"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("house1"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("house1") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("stone.001"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("stone.001"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("stone.001") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("stone.003"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("stone.003"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("stone.003") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("block"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("block"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("block") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("stone"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("stone"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("stone") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("wood"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("wood"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("wood") , sMat });
//
//#pragma  endregion

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
		// TODO : �ϴ� Hard Coding�ε� .. 3�� ��ü ����ȭ�� �Ǹ� �� Table�� �ɰ��� ..
		DUOLGraphicsEngine::AnimationClip* animClip = _graphicsEngine->LoadAnimationClip(TEXT("mixamo.com"));
		// DUOLGraphicsEngine::AnimationClip* animClip = _graphicsEngine->LoadAnimationClip(TEXT("Armature | Armature | mixamo.com|Layer0"));

		std::shared_ptr<DUOLGameEngine::AnimationClip> engineClip = std::make_shared<DUOLGameEngine::AnimationClip>(TEXT("mixamo.com"));

		engineClip->SetPrimitiveAnimationClip(animClip);

		_animationClipIDMap.insert({ TEXT("mixamo.com"), engineClip });
	}

	DUOLGameEngine::Mesh* ResourceManager::GetMesh(const DUOLCommon::tstring& meshID) const
	{
		return _meshIDMap.contains(meshID) ? _meshIDMap.at(meshID).get() : nullptr;
	}

	DUOLGameEngine::Avatar* ResourceManager::GetAvatar(const DUOLCommon::tstring& avatarID) const
	{
		return _avatarIDMap.contains(avatarID) ? _avatarIDMap.at(avatarID).get() : nullptr;
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

	DUOLGameEngine::AnimationClip* ResourceManager::GetAnimationClip(
		const DUOLCommon::tstring& animationClipID) const
	{
		return _animationClipIDMap.contains(animationClipID) ? _animationClipIDMap.at(animationClipID).get() : nullptr;
	}

	DUOLGraphicsEngine::Model* ResourceManager::GetModel(const DUOLCommon::tstring& modelID) const
	{
		return _modelIDMap.contains(modelID) ? _modelIDMap.at(modelID) : nullptr;
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
		LoadAnimationClipTable(gameSpec.projectPath + TEXT("Asset/DataTable/AnimationClipTable.json"));
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