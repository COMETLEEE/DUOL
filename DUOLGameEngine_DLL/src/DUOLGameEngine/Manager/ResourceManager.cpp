#include "../../../../DUOLGraphicsEngine/src/DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
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
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"

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
		_graphicsEngine->SetDataName(_materialNameList, _animationNameList);

		auto jsonReader = DUOLJson::JsonReader::GetInstance();

		auto modelTable = jsonReader->LoadJson(path);

		const TCHAR* name = TEXT("Name");

		const TCHAR* id = TEXT("ID");

		std::pair<std::vector<uint64>, std::vector<uint64>> modeldatas;

		for (auto& model : modelTable->GetArray())
		{
			for (auto& datas : model.GetArray())
			{
				if (datas.HasMember(name) && datas.HasMember(id))
				{
					const DUOLCommon::tstring& modelStringID = datas[name].GetString();

					uint64 modelKey = datas[id].GetUint64();

					SetUseData(modelKey, modeldatas);

					DUOLGraphicsEngine::Model* pModel = _graphicsEngine->CreateModelFromFBX(modelStringID, modeldatas);

					_modelIDMap.insert({ modelStringID, pModel });

#pragma region MESH
					unsigned meshCount = pModel->GetMeshCount();

					for (unsigned i = 0; i < meshCount; i++)
					{
						DUOLGraphicsEngine::MeshBase* pMesh = pModel->GetMesh(i);

						// ???? ????? ?и? ??????? ? ???? ?? ???? ???? ?
						std::shared_ptr<DUOLGameEngine::Mesh> engineMesh = std::make_shared<DUOLGameEngine::Mesh>(pMesh->_meshName);

						engineMesh->SetPrimitiveMesh(pMesh);

						_meshIDMap.insert({ pMesh->_meshName, engineMesh });

						_resourceUUIDMap.insert({ engineMesh->GetUUID(), engineMesh.get() });
					}
#pragma endregion

#pragma region AVATAR
					if (pModel->IsSkinningModel())
					{
						std::shared_ptr<DUOLGameEngine::Avatar> avatar = std::make_shared<Avatar>(modelStringID);

						avatar->SetPrimitiveBones(&pModel->GetBones());

						_avatarIDMap.insert({ modelStringID, avatar });

						_resourceUUIDMap.insert({ avatar->GetUUID(), avatar.get() });
					}
#pragma endregion
				}
			}
		}
	}

	void ResourceManager::SetUseData(uint64 meshid, std::pair<std::vector<uint64>, std::vector<uint64>>& modeldata)
	{
		for (auto& perfab : _perfabsIDList)
		{
			if (perfab.first == meshid)
			{
				modeldata = perfab.second;
			}
		}
	}

	void ResourceManager::LoadMaterialTable(const DUOLCommon::tstring& path)
	{
		for (auto mesh : _meshIDMap)
		{
			auto primitvieMesh = mesh.second->GetPrimitiveMesh();

			for (int subMeshIndex = 0; subMeshIndex < primitvieMesh->GetSubMeshCount(); subMeshIndex++)
			{
				DUOLCommon::tstring& name = primitvieMesh->GetSubMesh(subMeshIndex)->_materialName;

				DUOLGraphicsEngine::Material* mat = _graphicsEngine->LoadMaterial(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName);

				std::shared_ptr<DUOLGameEngine::Material> sMat = std::make_shared<DUOLGameEngine::Material>(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName);

				sMat->SetPrimitiveMaterial(mat);

				_materialIDMap.insert({ primitvieMesh->GetSubMesh(subMeshIndex)->_materialName, sMat });

				_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });
			}
		}

		{
			DUOLGraphicsEngine::Material* mat;

			std::shared_ptr<DUOLGameEngine::Material> sMat;

#pragma region PARTICLE
			mat = _graphicsEngine->LoadMaterial(_T("Particle"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("Particle"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("Particle") , sMat });

			_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });
#pragma endregion

#pragma region DEBUG
			mat = _graphicsEngine->LoadMaterial(_T("Debug"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("Debug"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("Debug") , sMat });

			_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });
#pragma endregion
		}
	}

	void ResourceManager::LoadPhysicsMaterialTable(const DUOLCommon::tstring& path)
	{
		const DUOLPhysics::PhysicsMaterialDesc matDesc{ 0.3f, 0.3f, 0.3f };

		std::weak_ptr<DUOLPhysics::PhysicsMaterial> pMat = _physicsSystem->CreateMaterial(TEXT("Default"), matDesc);

		std::shared_ptr<DUOLGameEngine::PhysicsMaterial> pMatEngine = std::make_shared<DUOLGameEngine::PhysicsMaterial>(TEXT("Default"));

		pMatEngine->SetPhysicsMaterial(pMat);

		DUOLCommon::tstring temp = TEXT("Default");

		_physicsMaterialIDMap.insert({ TEXT("Default"), pMatEngine });

		_resourceUUIDMap.insert({ pMatEngine->GetUUID(), pMatEngine.get() });
	}

	void ResourceManager::LoadAnimationClipTable(const DUOLCommon::tstring& path)
	{
		auto animClip = DUOLGameEngine::SerializeManager::GetInstance()->
			DeserializeAnimationClip(TEXT("Asset/AnimationClip/DrunkRun.dclip"));

		_animationClipIDMap.insert({ animClip->GetName(), animClip });

		_resourceUUIDMap.insert({ animClip->GetUUID(), animClip.get()});

		animClip = DUOLGameEngine::SerializeManager::GetInstance()->
			DeserializeAnimationClip(TEXT("Asset/AnimationClip/DrunkIdle.dclip"));

		_animationClipIDMap.insert({ animClip->GetName(), animClip });

		_resourceUUIDMap.insert({ animClip->GetUUID(), animClip.get() });



#pragma region PROTOTYPE_ANIMATION_CLIP
		auto protoIdle = _graphicsEngine->LoadAnimationClip(TEXT("Proto_Idle"));
		auto protoWalk = _graphicsEngine->LoadAnimationClip(TEXT("Proto_Walk"));
		auto protoRun = _graphicsEngine->LoadAnimationClip(TEXT("Proto_Run"));
		auto protoRoll = _graphicsEngine->LoadAnimationClip(TEXT("Proto_Roll"));

		auto engineProtoIdle = std::shared_ptr<DUOLGameEngine::AnimationClip>(new AnimationClip(TEXT("Proto_Idle")));
		engineProtoIdle->SetPrimitiveAnimationClip(protoIdle);
		_animationClipIDMap.insert({ engineProtoIdle->GetName(), engineProtoIdle });

		auto engineProtoWalk = std::shared_ptr<DUOLGameEngine::AnimationClip>(new AnimationClip(TEXT("Proto_Walk")));
		engineProtoWalk->SetPrimitiveAnimationClip(protoWalk);
		_animationClipIDMap.insert({ engineProtoWalk->GetName(), engineProtoWalk });

		auto engineProtoRun = std::shared_ptr<DUOLGameEngine::AnimationClip>(new AnimationClip(TEXT("Proto_Run")));
		engineProtoRun->SetPrimitiveAnimationClip(protoRun);
		_animationClipIDMap.insert({ engineProtoRun->GetName(), engineProtoRun });

		auto engineProtoRoll = std::shared_ptr<DUOLGameEngine::AnimationClip>(new AnimationClip(TEXT("Proto_Roll")));
		engineProtoRoll->SetPrimitiveAnimationClip(protoRoll);
		_animationClipIDMap.insert({ engineProtoRoll->GetName(), engineProtoRoll });
#pragma endregion
	}

	void ResourceManager::LoadAnimatorControllerTable(const DUOLCommon::tstring& path)
	{
		auto AnimCon = DUOLGameEngine::SerializeManager::GetInstance()->
			DeserializeAnimatorController(TEXT("Asset/AnimatorController/TestAnimCon.dcontroller"));

		_animatorControllerIDMap.insert({ AnimCon->GetName(), AnimCon });

		_resourceUUIDMap.insert({ AnimCon->GetUUID(), AnimCon.get() });

#pragma region CREATE_LOAD_ANIMATOR_CONTROLLER
		auto protoAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("ProtoAnimCon"));

		// 스피드로 Idle, Walk, Run
		protoAnimCon->AddParameter(TEXT("Speed"), AnimatorControllerParameterType::Float);

		// 구르기
		protoAnimCon->AddParameter(TEXT("Rolling"), AnimatorControllerParameterType::Bool);

		auto protoStateMachine = protoAnimCon->AddStateMachine(TEXT("ProtoStateMachine"));

		// Idle
		auto protoIdle = protoStateMachine->AddState(TEXT("Idle"));

		protoIdle->SetAnimationClip(GetAnimationClip(TEXT("Proto_Idle")));

		// Walk
		auto protoWalk = protoStateMachine->AddState(TEXT("Walk"));

		protoWalk->SetAnimationClip(GetAnimationClip(TEXT("Proto_Walk")));

		// Run
		auto protoRun = protoStateMachine->AddState(TEXT("Run"));

		protoRun->SetAnimationClip(GetAnimationClip(TEXT("Proto_Run")));

		// Roll
		auto protoRoll = protoStateMachine->AddState(TEXT("Roll"));

		protoRoll->SetAnimationClip(GetAnimationClip(TEXT("Proto_Roll")));




		// Idle To Walk
		auto idleToWalk = protoIdle->AddTransition(protoWalk);

		idleToWalk->SetTransitionOffset(0.0f);

		idleToWalk->SetTransitionDuration(0.1f);

		idleToWalk->AddCondition(TEXT("Speed"), AnimatorConditionMode::Greater, 0.25f);

		// Walk To Idle
		auto walkToIdle = protoWalk->AddTransition(protoIdle);

		walkToIdle->SetTransitionOffset(0.0f);

		walkToIdle->SetTransitionDuration(0.1f);

		walkToIdle->AddCondition(TEXT("Speed"), AnimatorConditionMode::Less, 0.25f);

		// Walk To Run
		auto walkToRun = protoWalk->AddTransition(protoRun);

		walkToRun->SetTransitionOffset(0.1f);

		walkToRun->SetTransitionDuration(0.4f);

		walkToRun->AddCondition(TEXT("Speed"), AnimatorConditionMode::Greater, 4.f);

		// Run To Walk
		auto runToWalk = protoRun->AddTransition(protoWalk);

		runToWalk->SetTransitionOffset(0.1f);

		runToWalk->SetTransitionDuration(0.4f);

		runToWalk->AddCondition(TEXT("Speed"), AnimatorConditionMode::Less, 4.f);

		_animatorControllerIDMap.insert({ protoAnimCon->GetName(), protoAnimCon });
#pragma endregion
	}

	void ResourceManager::LoadPrefabTable(const DUOLCommon::tstring& path)
	{
		auto jsonReader = DUOLJson::JsonReader::GetInstance();

		auto modelTable = jsonReader->LoadJson(path);

		const TCHAR* id = TEXT("MeshID");

		const TCHAR* materialid = TEXT("MaterialID");

		const TCHAR* animationid = TEXT("AnimationID");

		uint64 modelID;
		std::vector<uint64> materialID;
		std::vector<uint64> animationID;

		for (auto& model : modelTable->GetArray())
		{
			for (auto& datas : model.GetArray())
			{
				if (datas.HasMember(id) && datas.HasMember(materialid) && datas.HasMember(animationid))
				{
					modelID = datas[id].GetInt64();

					if (datas[materialid].IsArray())
					{
						for (auto iter = datas[materialid].Begin(); iter != datas[materialid].End(); iter++)
						{
							uint64 matID = (*iter).GetInt64();

							materialID.emplace_back(matID);
						}
					}
					if (datas[animationid].IsArray())
					{
						for (auto iter = datas[animationid].Begin(); iter != datas[animationid].End(); iter++)
						{
							uint64 animID = (*iter).GetInt64();

							animationID.emplace_back(animID);
						}
					}
				}

				// Prefab input
				_perfabsIDList.emplace_back(std::make_pair(modelID, std::make_pair(materialID, animationID)));
				materialID.clear();
				animationID.clear();
			}
		}
	}

	void ResourceManager::LoadDataNameTable(const DUOLCommon::tstring& path, bool ismaterial)
	{
		auto jsonReader = DUOLJson::JsonReader::GetInstance();

		auto DataTable = jsonReader->LoadJson(path);

		const TCHAR* id = TEXT("ID");

		const TCHAR* name = TEXT("Name");

		for (auto& arraydatas : DataTable->GetArray())
		{
			for (auto& datas : arraydatas.GetArray())
			{
				if (datas.HasMember(id) && datas.HasMember(name))
				{
					uint64 dataID = datas[id].GetInt64();

					const DUOLCommon::tstring& dataName = datas[name].GetString();

					if (ismaterial)
						_materialNameList.emplace_back(std::make_pair(dataID, dataName));
					else
						_animationNameList.emplace_back(std::make_pair(dataID, dataName));
				}
			}
		}
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

	DUOLGraphicsLibrary::IFont* ResourceManager::GetFont(const DUOLCommon::tstring& fontName) const
	{
		return _graphicsEngine->GetFont(fontName);
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::GetTexture(const DUOLCommon::tstring& textureName) const
	{
		return _graphicsEngine->LoadTexture(textureName);
	}

	DUOLGraphicsLibrary::IFont* ResourceManager::CreateIFont(const DUOLCommon::tstring& fontPath) const
	{
		return _graphicsEngine->CreateIFont(fontPath);
	}

	DUOLGraphicsLibrary::ICanvas* ResourceManager::CreateCanvas(
		const DUOLGraphicsLibrary::CanvasRenderMode renderMode) const
	{
		return _graphicsEngine->CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::BackBuffer);
	}

	DUOLGameEngine::Material* ResourceManager::CreateMaterial(const DUOLCommon::tstring& materialID, const DUOLCommon::tstring& textureID, const DUOLCommon::tstring& normal, const DUOLCommon::tstring& metalroughhnessao, const DUOLCommon::tstring& pipelineState)
	{
		DUOLGraphicsEngine::MaterialDesc material;

		material._albedoMap = textureID;
		material._normalMap = normal;
		material._metallicRoughnessMap = metalroughhnessao;

		material._pipelineState = pipelineState;

		auto mat = _graphicsEngine->CreateMaterial(materialID, material);

		std::shared_ptr<DUOLGameEngine::Material> sMat = std::make_shared<DUOLGameEngine::Material>(materialID);

		sMat->SetPrimitiveMaterial(mat);

		DUOLCommon::tstring targetName = materialID;

		_materialIDMap.insert({ targetName, sMat });

		return sMat.get();
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const DUOLCommon::tstring& textureID)
	{
		return _graphicsEngine->CreateTexture(textureID);
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const DUOLCommon::tstring& textureID, float width, float height, int size, void* initialData)
	{
		return _graphicsEngine->CreateTexture(textureID, width, height, size, initialData);
	}

	DUOLGameEngine::ObjectBase* ResourceManager::GetResourceByUUID(const DUOLCommon::UUID uuid) const
	{
		return _resourceUUIDMap.contains(uuid) ? _resourceUUIDMap.at(uuid) : nullptr;
	}

	DUOLGameEngine::ObjectBase* ResourceManager::GetResourceByName(const DUOLCommon::tstring& name) const
	{
		DUOLGameEngine::ObjectBase* ret = nullptr;

		// Check Mesh
		ret = GetMesh(name);

		if (ret != nullptr)
			return ret;

		// Check Animation Clip
		ret = GetAnimationClip(name);

		if (ret != nullptr)
			return ret;

		// Check Material
		ret = GetMaterial(name);

		if (ret != nullptr)
			return ret;

		// Check PhysicsMaterial
		ret = GetPhysicsMaterial(name);

		if (ret != nullptr)
			return ret;

		// Check Avatar
		ret = GetAvatar(name);

		if (ret != nullptr)
			return ret;

		// Check AnimatorController
		ret = GetAnimatorController(name);

		if (ret != nullptr)
			return ret;
	}

	void ResourceManager::Initialize(const EngineSpecification& gameSpec
	                                 , const std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine>& graphicsEngine
	                                 , const std::shared_ptr<DUOLPhysics::PhysicsSystem>& physicsSystem)
	{
		_graphicsEngine = graphicsEngine;

		_physicsSystem = physicsSystem;

		if (_graphicsEngine == nullptr || _physicsSystem == nullptr)
			DUOL_CRITICAL("GraphicsEngine or physicsSystem nullptr");

		const DUOLCommon::tstring& projectPath = gameSpec.projectPath;

#pragma region CLIENT_CODE
		/// LoadFBXTable을 부르기 전에 불러줘야합니다.
		// 1. LoadPerfab Table
		LoadPrefabTable(gameSpec.projectPath + TEXT("Asset/DataTable/Prefab.json"));

		// 2. LoadMaterial Table
		LoadDataNameTable(gameSpec.projectPath + TEXT("Asset/DataTable/Material.json"), true);

		// 3. Animation Table
		LoadDataNameTable(gameSpec.projectPath + TEXT("Asset/DataTable/Animation.json"), false);

		LoadFBXTable(gameSpec.projectPath + TEXT("Asset/DataTable/MeshTable.json"));

		LoadMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/MaterialTable.json"));

		LoadPhysicsMaterialTable(gameSpec.projectPath + TEXT("Asset/DataTable/PhysicsMaterialTable.json"));

		LoadAnimationClipTable(gameSpec.projectPath + TEXT("Asset/DataTable/AnimationClipTable.json"));

		LoadAnimatorControllerTable(gameSpec.projectPath + TEXT("Asset/DataTable/AnimatorControllerTable.json"));
#pragma endregion

		DUOL_INFO(DUOL_FILE, "ResourceManager Initialize Success !");
	}

	void ResourceManager::UnInitialize()
	{
		_graphicsEngine.reset();
	}

	void ResourceManager::Update(float deltaTime)
	{
		// TODO : 가비지 컬렉팅 ..? 뭘 할 수 있을까 .. 아 클리이언트 요청에 따른 리소스 로드, 언로드 정도 ..?
	}
}