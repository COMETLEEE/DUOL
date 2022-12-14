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

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

#include "DUOLJson/JsonReader.h"

namespace DUOLGameEngine
{
	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{
		for (auto& [key, res] : _meshIDMap)
			res.reset();

		for (auto& [key, res] : _avatarIDMap)
			res.reset();

		for (auto& [key, res] : _materialIDMap)
			res.reset();

		for (auto& [key, res] : _physicsMaterialIDMap)
			res.reset();

		for (auto& [key, res] : _animationClipIDMap)
			res.reset();

		for (auto& [key, res] : _animatorControllerIDMap)
			res.reset();

		_meshIDMap.clear();

		_modelIDMap.clear();

		_avatarIDMap.clear();

		_materialIDMap.clear();

		_physicsMaterialIDMap.clear();

		_animationClipIDMap.clear();

		_animatorControllerIDMap.clear();
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

				// TODO: ?????? ????? ??u ????? ?? ?? ?????? ?????? ??? ???? ???? ????? ..?
				_modelIDMap.insert({ modelStringID, pModel });

#pragma region MESH
				// ?? ????? ?? ???????? ????? ..
				unsigned meshCount = pModel->GetMeshCount();

				for (unsigned i = 0; i < meshCount; i++)
				{
					DUOLGraphicsEngine::MeshBase* pMesh = pModel->GetMesh(i);

					// ???? ????? ???? ??????? ? ???? ?? ???? ???? ?
					std::shared_ptr<DUOLGameEngine::Mesh> engineMesh = std::make_shared<DUOLGameEngine::Mesh>(pMesh->_meshName);

					engineMesh->SetPrimitiveMesh(pMesh);

					_meshIDMap.insert({ pMesh->_meshName, engineMesh });
				}
#pragma endregion

#pragma region AVATAR
				// ?????? FBX Model?? Skinning Model ???? ..
				if (pModel->IsSkinningModel())
				{
					// ??? ???? ?? ?????? ??????
					// Avatar resource object?? ??????. (??? ????? ?? ?????? ?????? ????)
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
		//TODO : ?????? ??????? ???? ????? ??? ????????..^-^.. by ?????
		//??????????.. ????? ?? ????o?

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

		//??? ????????
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
	}

	void ResourceManager::LoadPhysicsMaterialTable(const DUOLCommon::tstring& path)
	{
		// TODO : Physics Material ?? ?? ???? ??? ??????? Asset?????? ???? ..
		const DUOLPhysics::PhysicsMaterialDesc matDesc{ 0.5f, 0.5f, 0.5f };

		std::weak_ptr<DUOLPhysics::PhysicsMaterial> pMat = _physicsSystem->CreateMaterial(TEXT("Default"), matDesc);

		std::shared_ptr<DUOLGameEngine::PhysicsMaterial> pMatEngine = std::make_shared<DUOLGameEngine::PhysicsMaterial>(TEXT("Default"));

		pMatEngine->SetPhysicsMaterial(pMat);

		DUOLCommon::tstring temp = TEXT("Default");

		_physicsMaterialIDMap.insert({ TEXT("Default"), pMatEngine });
	}

	void ResourceManager::LoadAnimationClipTable(const DUOLCommon::tstring& path)
	{
		// TODO : ??? Hard Coding???? .. 3?? ??u ??????? ??? ?? Table?? ????? ..
		DUOLGraphicsEngine::AnimationClip* animClip;
		std::shared_ptr<DUOLGameEngine::AnimationClip> engineClip;

		animClip = _graphicsEngine->LoadAnimationClip(TEXT("Armature|Armature|mixamo.com|DrunkRun"));

		engineClip = std::make_shared<DUOLGameEngine::AnimationClip>(TEXT("DrunkRun"));

		// TODO - ??? ????
		engineClip->AddEvent({ TEXT("OnLeftFoot"), 17.f });
		engineClip->AddEvent({ TEXT("OnLeftFoot"), 72.f });

		// TODO - ?????? ????
		engineClip->AddEvent({ TEXT("OnRightFoot"), 45.f });
		engineClip->AddEvent({ TEXT("OnRightFoot"), 100.f });

		engineClip->SetPrimitiveAnimationClip(animClip);

		_animationClipIDMap.insert({ TEXT("DrunkRun"), engineClip });

		animClip = _graphicsEngine->LoadAnimationClip(TEXT("Armature|Armature|mixamo.com|Layer0|DrunkIdle"));

		engineClip = std::make_shared<DUOLGameEngine::AnimationClip>(TEXT("DrunkIdle"));

		engineClip->SetPrimitiveAnimationClip(animClip);

		_animationClipIDMap.insert({ TEXT("DrunkIdle"), engineClip });
	}

	void ResourceManager::LoadAnimatorControllerTable(const DUOLCommon::tstring& path)
	{
		// TODO : ?????? ???? Hard Coding???? .. ???? AnimatorController ???? Asset???? ?????? ???? ..!
		std::shared_ptr<DUOLGameEngine::AnimatorController> animCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("TestAnimCon"));

		animCon->AddParameter(TEXT("TrueIsIdle"), AnimatorControllerParameterType::Bool);

		// AnimatorController ??????? .. ???? ??????? ?? ????? ??? ?? ?????? ..
		DUOLGameEngine::AnimatorState* idleState = animCon->AddMotion(GetAnimationClip(TEXT("DrunkIdle")));

		DUOLGameEngine::AnimatorState* runState = animCon->AddMotion(GetAnimationClip(TEXT("DrunkRun")));
		
		DUOLGameEngine::AnimatorStateTransition* idleToRun = idleState->AddTransition(runState);

		idleToRun->AddCondition(TEXT("TrueIsIdle"), AnimatorConditionMode::False);

		DUOLGameEngine::AnimatorStateTransition* runToIdle = runState->AddTransition(idleState);

		runToIdle->AddCondition(TEXT("TrueIsIdle"), AnimatorConditionMode::True);

		_animatorControllerIDMap.insert({ TEXT("TestAnimCon"), animCon });
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

	DUOLGameEngine::AnimatorController* ResourceManager::GetAnimatorController(const DUOLCommon::tstring& animatorControllerID) const
	{
		return _animatorControllerIDMap.contains(animatorControllerID) ? _animatorControllerIDMap.at(animatorControllerID).get() : nullptr;
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
		// 1. FBX Table?? ??????? ????????.
		LoadFBXTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));

		// 2. Material Table?? ??????? ????????. => ??? ??? ???? ..? FBX ????? ??? Material list ?? ??? ????? ..
		LoadMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/MaterialTable.json"));

		// 3. Physics Material Table?? ??????? ????????.
		LoadPhysicsMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/PhysicsMaterialTable.json"));

		// 4. Animation Clip Table?? ??????? ????????.
		LoadAnimationClipTable(gameSpec.projectPath + TEXT("Asset/DataTable/AnimationClipTable.json"));

		// 5. AnimatorController Table?? ??????? ????????.
		LoadAnimatorControllerTable(gameSpec.projectPath + TEXT("Asset/DataTable/AnimatorControllerTable.json"));
#pragma endregion
	}

	void ResourceManager::UnInitialize()
	{
		// Mesh Delete ???????? ???? ?????? ???? ?? ???? ?? ????.

		// TODO : ???? ???? ???????? ?????? ??? Resource Load - UnLoad ????.

	}

	void ResourceManager::Update(float deltaTime)
	{

	}
}