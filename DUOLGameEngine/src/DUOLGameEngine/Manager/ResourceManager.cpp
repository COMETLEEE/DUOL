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

				// TODO: 그래픽스 리소스 자체 포맷화 후 이 모델이라는 개념이 계속 남아 있을 것인가 ..?
				_modelIDMap.insert({ modelStringID, pModel });

#pragma region MESH
				// 총 메쉬가 몇 개인지에 대해서 ..
				unsigned meshCount = pModel->GetMeshCount();

				for (unsigned i = 0; i < meshCount; i++)
				{
					DUOLGraphicsEngine::MeshBase* pMesh = pModel->GetMesh(i);

					// 서브 메쉬들도 분명 있을텐데 ? 내가 알 필요는 없나 ?
					std::shared_ptr<DUOLGameEngine::Mesh> engineMesh = std::make_shared<DUOLGameEngine::Mesh>(pMesh->_meshName);

					engineMesh->SetPrimitiveMesh(pMesh);

					_meshIDMap.insert({ pMesh->_meshName, engineMesh });
				}
#pragma endregion

#pragma region AVATAR
				// 로드한 FBX Model이 Skinning Model 이라면 ..
				if (pModel->IsSkinningModel())
				{
					// 해당 모델의 본 정보를 이용해서
					// Avatar resource object를 만듭니다. (일단 단순히 본 정보를 기억하고 있음)
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
		//TODO : 만인의 편의성을 위해 알잘딱 코드를 생성합니다..^-^.. by 오경민
		//임시코드입니다.. 나중엔 꼭 지웁시다

		for(auto mesh : _meshIDMap)
		{
			auto primitvieMesh = mesh.second->GetPrimitiveMesh();

			for(int subMeshIndex = 0; subMeshIndex < primitvieMesh->GetSubMeshCount(); subMeshIndex++)
			{
				DUOLGraphicsEngine::Material* mat = _graphicsEngine->LoadMaterial(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName);

				std::shared_ptr<DUOLGameEngine::Material> sMat = std::make_shared<DUOLGameEngine::Material>(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName);

				sMat->SetPrimitiveMaterial(mat);

				_materialIDMap.insert({ primitvieMesh->GetSubMesh(subMeshIndex)->_materialName, sMat });
			}
		}

		//기타 머테리얼들
		{
			DUOLGraphicsEngine::Material* mat;

			std::shared_ptr<DUOLGameEngine::Material> sMat;
#pragma region particle
			mat = _graphicsEngine->LoadMaterial(_T("Particle"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("Particle"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("Particle") , sMat });
#pragma endregion
#pragma region debug
			mat = _graphicsEngine->LoadMaterial(_T("Debug"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("Debug"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("Debug") , sMat });
#pragma endregion
		}

//
//		// TODO : 일단 Joy를 로드하기 위한 하드코딩입니다 .. 머터리얼 관련 정리는 꼭 필요할 듯 .. ㅠ
//		DUOLGraphicsEngine::Material* mat;
//
//		std::shared_ptr<DUOLGameEngine::Material> sMat;
//
//#pragma region JOY_MATERIAL
//		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Scarf_MAT"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Scarf_MAT"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("Boy01_Scarf_MAT"), sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("Boy01_LowerBody_MAT"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_LowerBody_MAT"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("Boy01_LowerBody_MAT") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Hair_MAT"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Hair_MAT"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("Boy01_Hair_MAT") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Shoes_MAT"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Shoes_MAT"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("Boy01_Shoes_MAT") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("Boy01_UpperBody_MAT"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_UpperBody_MAT"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("Boy01_UpperBody_MAT") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Hands_MAT"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Hands_MAT"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("Boy01_Hands_MAT") , sMat });
//
//
//		mat = _graphicsEngine->LoadMaterial(_T("Boy01_Head_MAT"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Boy01_Head_MAT"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("Boy01_Head_MAT") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("Material.001"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Material.001"));
//
//		sMat->SetPrimitiveMaterial(mat);
//#pragma endregion
//
//		_materialIDMap.insert({ _T("Material.001") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("Material"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("Material"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("Material") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("MariaMat"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("MariaMat"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("MariaMat") , sMat });
//
//
//
//#pragma region BUTTON_FLOOR
//		mat = _graphicsEngine->LoadMaterial(_T("button_metallic"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("button_metallic"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("button_metallic") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("base case"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("base case"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("base case") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("screen sign"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("screen sign"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("screen sign") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("glowing stripes"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("glowing stripes"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("glowing stripes") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("fixator_case"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("fixator_case"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("fixator_case") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("fixator_tube"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("fixator_tube"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("fixator_tube") , sMat });
//
//		mat = _graphicsEngine->LoadMaterial(_T("fixator_glass"));
//
//		sMat = std::make_shared<DUOLGameEngine::Material>(_T("fixator_glass"));
//
//		sMat->SetPrimitiveMaterial(mat);
//
//		_materialIDMap.insert({ _T("fixator_glass") , sMat });
//#pragma endregion

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
		// TODO : Physics Material 뿐 만 아니라 모든 리소스들 Asset으로의 일원화 ..
		const DUOLPhysics::PhysicsMaterialDesc matDesc{ 0.5f, 0.5f, 0.5f };

		std::weak_ptr<DUOLPhysics::PhysicsMaterial> pMat = _physicsSystem->CreateMaterial(TEXT("Default"), matDesc);

		std::shared_ptr<DUOLGameEngine::PhysicsMaterial> pMatEngine = std::make_shared<DUOLGameEngine::PhysicsMaterial>(TEXT("Default"));

		pMatEngine->SetPhysicsMaterial(pMat);

		DUOLCommon::tstring temp = TEXT("Default");

		_physicsMaterialIDMap.insert({ TEXT("Default"), pMatEngine });
	}

	void ResourceManager::LoadAnimationClipTable(const DUOLCommon::tstring& path)
	{
		// TODO : 일단 Hard Coding인데 .. 3종 자체 포맷화가 되면 다 Table로 쪼개자 ..
		DUOLGraphicsEngine::AnimationClip* animClip = _graphicsEngine->LoadAnimationClip(TEXT("mixamo.com"));
		// DUOLGraphicsEngine::AnimationClip* animClip = _graphicsEngine->LoadAnimationClip(TEXT("Armature|Armature|mixamo.com|Layer0"));

		std::shared_ptr<DUOLGameEngine::AnimationClip> engineClip = std::make_shared<DUOLGameEngine::AnimationClip>(TEXT("mixamo.com"));

		engineClip->SetPrimitiveAnimationClip(animClip);

		_animationClipIDMap.insert({ TEXT("mixamo.com"), engineClip });

		animClip = _graphicsEngine->LoadAnimationClip(TEXT("Armature|Run"));
		// DUOLGraphicsEngine::AnimationClip* animClip = _graphicsEngine->LoadAnimationClip(TEXT("Armature|Armature|mixamo.com|Layer0"));

		engineClip = std::make_shared<DUOLGameEngine::AnimationClip>(TEXT("Run"));

		engineClip->SetPrimitiveAnimationClip(animClip);

		_animationClipIDMap.insert({ TEXT("Run"), engineClip });

		animClip = _graphicsEngine->LoadAnimationClip(TEXT("Armature|Idle"));
		// DUOLGraphicsEngine::AnimationClip* animClip = _graphicsEngine->LoadAnimationClip(TEXT("Armature|Armature|mixamo.com|Layer0"));

		engineClip = std::make_shared<DUOLGameEngine::AnimationClip>(TEXT("Idle"));

		engineClip->SetPrimitiveAnimationClip(animClip);

		_animationClipIDMap.insert({ TEXT("Idle"), engineClip });


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

	bool ResourceManager::GetMeshInfo(const DUOLCommon::tstring& meshID, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo) const
	{
		return _graphicsEngine->ReadMeshInfo(meshID, vertexInfo, indexInfo);
	}

	bool ResourceManager::GetMeshInfo(const DUOLGameEngine::Mesh* mesh, std::vector<DUOLMath::Vector3>& vertexInfo,
		std::vector<UINT32>& indexInfo) const
	{
		return _graphicsEngine->ReadMeshInfo(mesh->GetPrimitiveMesh(), vertexInfo, indexInfo);
	}

	DUOLGameEngine::Material* ResourceManager::CreateMaterial(const DUOLCommon::tstring& materialID, const DUOLCommon::tstring& textureID, const DUOLCommon::tstring& pipelineState)
	{
		DUOLGraphicsEngine::MaterialDesc material;

		material._albedoMap = textureID;
		material._pipelineState = pipelineState;

		auto mat = _graphicsEngine->CreateMaterial(materialID, material);

		std::shared_ptr<DUOLGameEngine::Material> sMat = std::make_shared<DUOLGameEngine::Material>(materialID);

		sMat->SetPrimitiveMaterial(mat);

		DUOLCommon::tstring targetName = materialID;

		_materialIDMap.insert({ targetName, sMat});

		return sMat.get();
	}
	

	void ResourceManager::Initialize(const EngineSpecification& gameSpec
	                                 , const std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine>& graphicsEngine
	                                 , const std::shared_ptr<DUOLPhysics::PhysicsSystem>& physicsSystem)
	{
		_graphicsEngine = graphicsEngine;

		_physicsSystem = physicsSystem;

		const DUOLCommon::tstring& projectPath = gameSpec.projectPath;

#pragma region CLIENT_CODE
		// 1. FBX Table을 참조하여 로드합니다.
		LoadFBXTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));

		// 2. Material Table을 참조하여 로드합니다. => 근데 이게 맞나 ..? FBX 파일에 이미 Material list 다 들어가 있는데 ..
		LoadMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/MaterialTable.json"));

		// 3. Physics Material Table을 참조하여 로드합니다.
		LoadPhysicsMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/PhysicsMaterialTable.json"));

		// 4. Load Table을 참조하여 로드합니다.
		LoadAnimationClipTable(gameSpec.projectPath + TEXT("Asset/DataTable/AnimationClipTable.json"));
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