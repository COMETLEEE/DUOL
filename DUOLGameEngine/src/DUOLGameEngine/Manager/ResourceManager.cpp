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

				// TODO: �׷��Ƚ� ���ҽ� ��ü ����ȭ �� �� ���̶�� ������ ��� ���� ���� ���ΰ� ..?
				_modelIDMap.insert({ modelStringID, pModel });

#pragma region MESH
				// �� �޽��� �� �������� ���ؼ� ..
				unsigned meshCount = pModel->GetMeshCount();

				for (unsigned i = 0; i < meshCount; i++)
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
				if (pModel->IsSkinningModel())
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
		//TODO : ������ ���Ǽ��� ���� ���ߵ� �ڵ带 �����մϴ�..^-^.. by �����
		//�ӽ��ڵ��Դϴ�.. ���߿� �� ����ô�

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

		//��Ÿ ���׸����
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
		// TODO : Physics Material �� �� �ƴ϶� ��� ���ҽ��� Asset������ �Ͽ�ȭ ..
		const DUOLPhysics::PhysicsMaterialDesc matDesc{ 0.5f, 0.5f, 0.5f };

		std::weak_ptr<DUOLPhysics::PhysicsMaterial> pMat = _physicsSystem->CreateMaterial(TEXT("Default"), matDesc);

		std::shared_ptr<DUOLGameEngine::PhysicsMaterial> pMatEngine = std::make_shared<DUOLGameEngine::PhysicsMaterial>(TEXT("Default"));

		pMatEngine->SetPhysicsMaterial(pMat);

		DUOLCommon::tstring temp = TEXT("Default");

		_physicsMaterialIDMap.insert({ TEXT("Default"), pMatEngine });
	}

	void ResourceManager::LoadAnimationClipTable(const DUOLCommon::tstring& path)
	{
		// TODO : �ϴ� Hard Coding�ε� .. 3�� ��ü ����ȭ�� �Ǹ� �� Table�� �ɰ��� ..
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

	void ResourceManager::LoadAnimatorControllerTable(const DUOLCommon::tstring& path)
	{
		// TODO : �׽�Ʈ�� ���� Hard Coding�ε� .. ���� AnimatorController ���� Asset���� ��ȯ�ϰ� ���� ..!
		std::shared_ptr<DUOLGameEngine::AnimatorController> animCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("TestAnimCon"));

		animCon->AddParameter(TEXT("TrueIsIdle"), AnimatorControllerParameterType::Bool);

		// AnimatorController �����ϱ� .. ���� �����Ϳ� �� ����� �� �� ������ ..
		DUOLGameEngine::AnimatorState* idleState = animCon->AddMotion(GetAnimationClip(TEXT("Idle")));

		DUOLGameEngine::AnimatorState* runState = animCon->AddMotion(GetAnimationClip(TEXT("Run")));

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
		// 1. FBX Table�� �����Ͽ� �ε��մϴ�.
		LoadFBXTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));

		// 2. Material Table�� �����Ͽ� �ε��մϴ�. => �ٵ� �̰� �³� ..? FBX ���Ͽ� �̹� Material list �� �� �ִµ� ..
		LoadMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/MaterialTable.json"));

		// 3. Physics Material Table�� �����Ͽ� �ε��մϴ�.
		LoadPhysicsMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/PhysicsMaterialTable.json"));

		// 4. Animation Clip Table�� �����Ͽ� �ε��մϴ�.
		LoadAnimationClipTable(gameSpec.projectPath + TEXT("Asset/DataTable/AnimationClipTable.json"));

		// 5. AnimatorController Table�� �����Ͽ� �ε��մϴ�.
		LoadAnimatorControllerTable(gameSpec.projectPath + TEXT("Asset/DataTable/AnimatorControllerTable.json"));
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