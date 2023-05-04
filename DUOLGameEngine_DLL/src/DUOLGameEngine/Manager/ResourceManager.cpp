#include "../../../../DUOLGraphicsEngine/src/DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include <filesystem>
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

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
#include "DUOLGameEngine/Manager/SoundManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/PerlinNoise.h"
#include "DUOLGameEngine/ECS/Object/Sprite.h"

#include "DUOLGameEngine/ECS/Object/AudioClip.h"
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

		for (auto& [key, res] : _renderingData_ParticleIDMap)
		{
			for (auto& iter : res)
				iter.reset();
		}

		for (auto& [key, res] : _spriteIDMap)
			res.reset();

		_modelIDMap.clear();

		_avatarIDMap.clear();

		_materialIDMap.clear();

		_physicsMaterialIDMap.clear();

		_animationClipIDMap.clear();

		_animatorControllerIDMap.clear();

		_spriteIDMap.clear();
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

				break;
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

				// PaperBurn TEST
				mat = _graphicsEngine->LoadMaterial(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn"));
				if (mat)
				{
					// GraphicsEngine에 생성한 머터리얼을 게임 엔진과 연결하는 과정..! 
					sMat = std::make_shared<DUOLGameEngine::Material>(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn"));

					sMat->SetPrimitiveMaterial(mat);

					_materialIDMap.insert({ primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn"), sMat });

					_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });
				}
			}
		}

		{
			DUOLGraphicsEngine::Material* mat;

			std::shared_ptr<DUOLGameEngine::Material> sMat;

#pragma region PARTICLE // 나중에 Json을 읽어오는 형식으로 바꾸자...!
			using std::filesystem::directory_iterator;
			auto directory_path = "Asset\\Particle";
			for (auto const& entry : directory_iterator(directory_path))
			{
				if (entry.path().extension() == ".dfx" || entry.path().extension() == ".DFX")
				{
					CreateParticleMaterial(DUOLCommon::StringHelper::StringToWString(entry.path().string()));
				}
			}
#pragma endregion

#pragma region DEBUG
			mat = _graphicsEngine->LoadMaterial(_T("DebugPointDepthOn"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("DebugPointDepthOn"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("DebugPointDepthOn") , sMat });

			_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });


			mat = _graphicsEngine->LoadMaterial(_T("DebugPointDepthOff"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("DebugPointDepthOff"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("DebugPointDepthOff") , sMat });

			_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });


			mat = _graphicsEngine->LoadMaterial(_T("DebugLineDepthOn"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("DebugLineDepthOn"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("DebugLineDepthOn") , sMat });

			_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });


			mat = _graphicsEngine->LoadMaterial(_T("DebugLineDepthOff"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("DebugLineDepthOff"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("DebugLineDepthOff") , sMat });

			_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });


			mat = _graphicsEngine->LoadMaterial(_T("DebugTriangleDepthOn"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("DebugTriangleDepthOn"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("DebugTriangleDepthOn") , sMat });

			_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });


			mat = _graphicsEngine->LoadMaterial(_T("DebugTriangleDepthOff"));

			sMat = std::make_shared<DUOLGameEngine::Material>(_T("DebugTriangleDepthOff"));

			sMat->SetPrimitiveMaterial(mat);

			_materialIDMap.insert({ _T("DebugTriangleDepthOff") , sMat });

			_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });
#pragma endregion
		}
	}

	void ResourceManager::LoadPhysicsMaterialTable(const DUOLCommon::tstring& path)
	{
		const DUOLPhysics::PhysicsMaterialDesc matDesc{ 0.3f, 0.3f, 0.f };

		std::weak_ptr<DUOLPhysics::PhysicsMaterial> pMat = _physicsSystem->CreateMaterial(TEXT("Default"), matDesc);

		std::shared_ptr<DUOLGameEngine::PhysicsMaterial> pMatEngine = std::make_shared<DUOLGameEngine::PhysicsMaterial>(TEXT("Default"));

		pMatEngine->SetPhysicsMaterial(pMat);

		DUOLCommon::tstring temp = TEXT("Default");

		_physicsMaterialIDMap.insert({ TEXT("Default"), pMatEngine });

		_resourceUUIDMap.insert({ pMatEngine->GetUUID(), pMatEngine.get() });
	}

	void ResourceManager::LoadAnimationClipTable(const DUOLCommon::tstring& path)
	{
		for (int index = 0; index < _graphicsEngine->LoadAnimationClipSize(); ++index)
		{
			auto animationClip = _graphicsEngine->LoadAnimationClip(index);
			auto animationName = DUOLCommon::StringHelper::ToTString(animationClip->_animationName);
			auto engineAnimationClip = std::shared_ptr<DUOLGameEngine::AnimationClip>(new AnimationClip(animationName));
			engineAnimationClip->SetPrimitiveAnimationClip(animationClip);
			_animationClipIDMap.insert({ engineAnimationClip->GetName(), engineAnimationClip });
		}
	}

	void ResourceManager::LoadAnimatorControllerTable(const DUOLCommon::tstring& path)
	{
#pragma region MONSTER_CONTROLLER
		{
			auto monsterAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Monster_AnimatorController"));

			auto monsterStateMachine = monsterAnimCon->AddStateMachine(TEXT("MonsterStateMachine"));

			// Parameter
			monsterAnimCon->AddParameter(TEXT("MoveSpeed"), AnimatorControllerParameterType::Float);
			monsterAnimCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkRight"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkLeft"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkBack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsJump_Backward"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHit_Front"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHit_Back"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsAirBorne"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("RandOffset"), AnimatorControllerParameterType::Float);
			monsterAnimCon->AddParameter(TEXT("IsWakeUpToIdle"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsDieToWakeUp"), AnimatorControllerParameterType::Bool);

			// State & AnimClip
			std::vector<AnimatorState*> allState;
			auto monsterIdle = monsterStateMachine->AddState(TEXT("Idle"));
			monsterIdle->SetAnimationClip(GetAnimationClip(TEXT("idle_angry")));
			allState.push_back(monsterIdle);

			auto monsterWalk = monsterStateMachine->AddState(TEXT("Walk"));
			monsterWalk->SetAnimationClip(GetAnimationClip(TEXT("walk_front")));
			allState.push_back(monsterWalk);

			auto monsterWalk_Right = monsterStateMachine->AddState(TEXT("Walk_Right"));
			monsterWalk_Right->SetAnimationClip(GetAnimationClip(TEXT("walk_right")));
			allState.push_back(monsterWalk_Right);

			auto monsterWalk_Left = monsterStateMachine->AddState(TEXT("Walk_Left"));
			monsterWalk_Left->SetAnimationClip(GetAnimationClip(TEXT("walk_left")));
			allState.push_back(monsterWalk_Left);

			auto monsterWalk_Back = monsterStateMachine->AddState(TEXT("Walk_Back"));
			monsterWalk_Back->SetAnimationClip(GetAnimationClip(TEXT("walk_back")));
			allState.push_back(monsterWalk_Back);

			auto monsterRun = monsterStateMachine->AddState(TEXT("Run"));
			monsterRun->SetAnimationClip(GetAnimationClip(TEXT("run")));
			allState.push_back(monsterRun);

			auto monsterAttack = monsterStateMachine->AddState(TEXT("Attack"));
			monsterAttack->SetAnimationClip(GetAnimationClip(TEXT("attack_close")));
			allState.push_back(monsterAttack);

			auto monsterJump_BackWard = monsterStateMachine->AddState(TEXT("Jump_Backward"));
			GetAnimationClip(TEXT("jump_backward"))->SetIsRootMotion(true);
			GetAnimationClip(TEXT("jump_backward"))->SetRootMotionTargetIndex(1);								// 이 애니메이션은 1번 본이 루트 모션 타겟입니다.
			monsterJump_BackWard->SetAnimationClip(GetAnimationClip(TEXT("jump_backward")));
			allState.push_back(monsterJump_BackWard);

			auto monsterWakeUp = monsterStateMachine->AddState(TEXT("WakeUp"));
			monsterWakeUp->SetAnimationClip(GetAnimationClip(TEXT("get_ up"))); // 지금 테스트 용으로 넣어뒀다..! 나중에 변경하자.
			allState.push_back(monsterWakeUp);

			auto monsterHit_Back = monsterStateMachine->AddState(TEXT("Hit_Back"));
			monsterHit_Back->SetAnimationClip(GetAnimationClip(TEXT("hit_back")));
			allState.push_back(monsterHit_Back);

			auto monsterHit_Front = monsterStateMachine->AddState(TEXT("Hit_Front"));
			monsterHit_Front->SetAnimationClip(GetAnimationClip(TEXT("hit_front")));
			allState.push_back(monsterHit_Front);

			for (auto& iter : allState)
			{
				if (monsterHit_Front != iter)
				{
					auto transition = iter->AddTransition(monsterHit_Front);
					transition->AddCondition(TEXT("IsHit_Front"), AnimatorConditionMode::True);
					transition->SetTransitionDuration(0.01f);
					transition->SetTransitionOffset(0.f);
				}
				if (monsterHit_Back != iter)
				{
					auto transition = iter->AddTransition(monsterHit_Back);
					transition->AddCondition(TEXT("IsHit_Back"), AnimatorConditionMode::True);
					transition->SetTransitionDuration(0.01f);
					transition->SetTransitionOffset(0.f);
				}
			}


			auto monsterDie_Ground = monsterStateMachine->AddState(TEXT("Die_Ground"));
			monsterDie_Ground->SetAnimationClip(GetAnimationClip(TEXT("die_ground")));

			auto monsterDie_Air = monsterStateMachine->AddState(TEXT("Die_Air"));
			monsterDie_Air->SetAnimationClip(GetAnimationClip(TEXT("die_air")));
			// RandOffset

			monsterIdle->SetOffsetParameter(TEXT("RandOffset"));
			monsterIdle->SetOffsetParameterActive(true);

			monsterHit_Front->SetOffsetParameter(TEXT("RandOffset"));
			monsterHit_Front->SetOffsetParameterActive(true);

			monsterHit_Back->SetOffsetParameter(TEXT("RandOffset"));
			monsterHit_Back->SetOffsetParameterActive(true);

			// Transition // 트랜지션의 우선순위는 먼저 등록한순이다.
			auto monsterIdleToWalk = monsterIdle->AddTransition(monsterWalk);
			auto monsterIdleToWalk_Right = monsterIdle->AddTransition(monsterWalk_Right);
			auto monsterIdleToWalk_Left = monsterIdle->AddTransition(monsterWalk_Left);
			auto monsterIdleToWalk_Back = monsterIdle->AddTransition(monsterWalk_Back);
			auto monsterIdleToAttack = monsterIdle->AddTransition(monsterAttack);
			auto monsterIdleToJump_Backward = monsterIdle->AddTransition(monsterJump_BackWard);
			auto monsterIdleToGroundDie = monsterIdle->AddTransition(monsterDie_Ground);


			auto monsterWalkToIdle = monsterWalk->AddTransition(monsterIdle);
			auto monsterWalkToRun = monsterWalk->AddTransition(monsterRun);

			auto monsterWalk_RightToIdle = monsterWalk_Right->AddTransition(monsterIdle);
			auto monsterWalk_LeftToIdle = monsterWalk_Left->AddTransition(monsterIdle);
			auto monsterWalk_BackToIdle = monsterWalk_Back->AddTransition(monsterIdle);

			auto monsterRunToJump_Backward = monsterRun->AddTransition(monsterJump_BackWard);
			auto monsterRunToWalk = monsterRun->AddTransition(monsterWalk);

			auto monsterAttackToIdle = monsterAttack->AddTransition(monsterIdle);
			auto monsterAttackToJump_BackWard = monsterAttack->AddTransition(monsterJump_BackWard);

			auto monsterJump_BackWardToIdel = monsterJump_BackWard->AddTransition(monsterIdle);

			auto monsterHit_BackToDieGround = monsterHit_Back->AddTransition(monsterDie_Ground);
			auto monsterHit_BackToDieAir = monsterHit_Back->AddTransition(monsterDie_Air);
			auto monsterHit_BackToIdle = monsterHit_Back->AddTransition(monsterIdle);

			auto monsterHit_FrontToDieGround = monsterHit_Front->AddTransition(monsterDie_Ground);
			auto monsterHit_FrontToDieAir = monsterHit_Front->AddTransition(monsterDie_Air);
			auto monsterHit_FrontToIdle = monsterHit_Front->AddTransition(monsterIdle);

			auto monsterDie_GroundToIdle = monsterDie_Ground->AddTransition(monsterIdle);
			auto monsterDie_GroundToDieAir = monsterDie_Ground->AddTransition(monsterDie_Air);
			auto monsterDie_AirToWakeUp = monsterDie_Air->AddTransition(monsterWakeUp);

			auto monsterWake_UpToIdle = monsterWakeUp->AddTransition(monsterIdle);

			monsterWake_UpToIdle->AddCondition(TEXT("IsWakeUpToIdle"), AnimatorConditionMode::True);
			monsterWake_UpToIdle->SetTransitionDuration(0.f);
			monsterWake_UpToIdle->SetTransitionOffset(0.f);

			monsterDie_GroundToDieAir->AddCondition(TEXT("IsAirBorne"), AnimatorConditionMode::True);
			monsterDie_GroundToDieAir->SetTransitionDuration(0.01f);
			monsterDie_GroundToDieAir->SetTransitionOffset(0.f);

			monsterHit_BackToDieAir->AddCondition(TEXT("IsAirBorne"), AnimatorConditionMode::True);
			monsterHit_BackToDieAir->SetTransitionDuration(0.01f);
			monsterHit_BackToDieAir->SetTransitionOffset(0.f);

			monsterHit_FrontToDieAir->AddCondition(TEXT("IsAirBorne"), AnimatorConditionMode::True);
			monsterHit_FrontToDieAir->SetTransitionDuration(0.01f);
			monsterHit_FrontToDieAir->SetTransitionOffset(0.f);

			monsterIdleToGroundDie->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
			monsterIdleToGroundDie->SetTransitionDuration(0.01f);
			monsterIdleToGroundDie->SetTransitionOffset(0.f);

			monsterHit_BackToDieGround->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
			monsterHit_BackToDieGround->SetTransitionDuration(0.01f);
			monsterHit_BackToDieGround->SetTransitionOffset(0.f);

			monsterHit_FrontToDieGround->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
			monsterHit_FrontToDieGround->SetTransitionDuration(0.01f);
			monsterHit_FrontToDieGround->SetTransitionOffset(0.f);

			monsterDie_GroundToIdle->AddCondition(TEXT("IsDie"), AnimatorConditionMode::False);
			monsterDie_GroundToIdle->SetTransitionDuration(0.01f);
			monsterDie_GroundToIdle->SetTransitionOffset(0.f);

			monsterDie_AirToWakeUp->AddCondition(TEXT("IsAirBorne"), AnimatorConditionMode::False);
			monsterDie_AirToWakeUp->AddCondition(TEXT("IsDie"), AnimatorConditionMode::False);
			monsterDie_AirToWakeUp->SetTransitionDuration(0.f);
			monsterDie_AirToWakeUp->SetTransitionOffset(0.f);

			monsterHit_BackToIdle->SetTransitionDuration(0.01f);
			monsterHit_BackToIdle->SetTransitionOffset(0.f);

			monsterHit_FrontToIdle->SetTransitionDuration(0.01f);
			monsterHit_FrontToIdle->SetTransitionOffset(0.f);

			monsterJump_BackWardToIdel->SetTransitionDuration(0.01f);
			monsterJump_BackWardToIdel->SetTransitionOffset(0.f);

			monsterRunToJump_Backward->AddCondition(TEXT("IsJump_Backward"), AnimatorConditionMode::True);
			monsterRunToJump_Backward->SetTransitionDuration(0.01f);
			monsterRunToJump_Backward->SetTransitionOffset(0.f);

			monsterIdleToJump_Backward->AddCondition(TEXT("IsJump_Backward"), AnimatorConditionMode::True);
			monsterIdleToJump_Backward->SetTransitionDuration(0.01f);
			monsterIdleToJump_Backward->SetTransitionOffset(0.f);

			monsterIdleToAttack->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
			monsterIdleToAttack->SetTransitionDuration(0.01f);
			monsterIdleToAttack->SetTransitionOffset(0.f);

			monsterAttackToJump_BackWard->AddCondition(TEXT("IsJump_Backward"), AnimatorConditionMode::True);
			monsterAttackToJump_BackWard->SetTransitionDuration(0.01f);
			monsterAttackToJump_BackWard->SetTransitionOffset(0.0f);
			monsterAttackToIdle->SetTransitionDuration(0.1f);
			monsterAttackToIdle->SetTransitionOffset(0.f);

			monsterIdleToWalk->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Greater, 0.5f);
			monsterIdleToWalk->SetTransitionDuration(0.1f);
			monsterIdleToWalk->SetTransitionOffset(0.f);

			monsterWalkToIdle->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Less, 0.49f);
			monsterWalkToIdle->SetTransitionDuration(0.1f);
			monsterWalkToIdle->SetTransitionOffset(0.f);

			monsterIdleToWalk_Right->AddCondition(TEXT("IsWalkRight"), AnimatorConditionMode::True);
			monsterIdleToWalk_Right->SetTransitionDuration(0.1f);
			monsterIdleToWalk_Right->SetTransitionOffset(0.f);

			monsterIdleToWalk_Left->AddCondition(TEXT("IsWalkLeft"), AnimatorConditionMode::True);
			monsterIdleToWalk_Left->SetTransitionDuration(0.1f);
			monsterIdleToWalk_Left->SetTransitionOffset(0.f);

			monsterIdleToWalk_Back->AddCondition(TEXT("IsWalkBack"), AnimatorConditionMode::True);
			monsterIdleToWalk_Back->SetTransitionDuration(0.1f);
			monsterIdleToWalk_Back->SetTransitionOffset(0.f);

			monsterWalk_RightToIdle->AddCondition(TEXT("IsWalkRight"), AnimatorConditionMode::False);
			monsterWalk_RightToIdle->SetTransitionDuration(0.1f);
			monsterWalk_RightToIdle->SetTransitionOffset(0.f);

			monsterWalk_LeftToIdle->AddCondition(TEXT("IsWalkLeft"), AnimatorConditionMode::False);
			monsterWalk_LeftToIdle->SetTransitionDuration(0.1f);
			monsterWalk_LeftToIdle->SetTransitionOffset(0.f);

			monsterWalk_BackToIdle->AddCondition(TEXT("IsWalkBack"), AnimatorConditionMode::False);
			monsterWalk_BackToIdle->SetTransitionDuration(0.1f);
			monsterWalk_BackToIdle->SetTransitionOffset(0.f);

			monsterWalkToRun->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Greater, 1.0f);
			monsterWalkToRun->SetTransitionDuration(0.1f);
			monsterWalkToRun->SetTransitionOffset(0.f);

			monsterRunToWalk->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Less, 0.99f);
			monsterRunToWalk->SetTransitionDuration(0.1f);
			monsterRunToWalk->SetTransitionOffset(0.f);

			// ------------------------------ Event Registe ---------------------------

			auto attackClip = GetAnimationClip(TEXT("attack_close"));
			AnimationEvent attackEvent;

			attackEvent._eventName = TEXT("Attack_Close");
			attackEvent._targetFrame = 49.0f;

			attackClip->AddEvent(attackEvent);

			auto dieGroundClip = GetAnimationClip(TEXT("die_ground"));
			AnimationEvent dieGroundEvent;

			dieGroundEvent._eventName = TEXT("Die");
			dieGroundEvent._targetFrame = 42.0f;

			dieGroundClip->AddEvent(dieGroundEvent);

			auto dieAirClip = GetAnimationClip(TEXT("die_air"));
			AnimationEvent dieAirEvent;

			dieAirEvent._eventName = TEXT("Die");
			dieAirEvent._targetFrame = 27.0f;

			dieAirClip->AddEvent(dieAirEvent);

			AnimationEvent wakeUpEndEvent;

			wakeUpEndEvent._eventName = TEXT("WakeUpEnd");
			wakeUpEndEvent._targetFrame = 46.0f;

			auto getUpClip = GetAnimationClip(TEXT("get_ up"));
			getUpClip->AddEvent(wakeUpEndEvent);

			// ------------------------------ Event Registe ---------------------------

			_animatorControllerIDMap.insert({ monsterAnimCon->GetName(), monsterAnimCon });

			_resourceUUIDMap.insert({ monsterAnimCon->GetUUID(), monsterAnimCon.get() });

			DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(monsterAnimCon.get(), TEXT("Asset/AnimatorController/Monster_AnimatorController.dcontroller"));
		}
		///  -------------------------------------------------------------------------------------- 원거리 몬스터
		{
			auto monsterAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Monster_AnimatorController_Far"));

			auto monsterStateMachine = monsterAnimCon->AddStateMachine(TEXT("MonsterStateMachine"));

			// Parameter
			monsterAnimCon->AddParameter(TEXT("MoveSpeed"), AnimatorControllerParameterType::Float);
			monsterAnimCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkRight"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkLeft"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkBack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsJump_Backward"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHit_Front"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHit_Back"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsAirBorne"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("RandOffset"), AnimatorControllerParameterType::Float);
			// State & AnimClip
			std::vector<AnimatorState*> allState;

			auto monsterIdle = monsterStateMachine->AddState(TEXT("Idle"));
			monsterIdle->SetAnimationClip(GetAnimationClip(TEXT("idle_far")));
			allState.push_back(monsterIdle);

			auto monsterWalk = monsterStateMachine->AddState(TEXT("Walk"));
			monsterWalk->SetAnimationClip(GetAnimationClip(TEXT("walk_front")));
			allState.push_back(monsterWalk);

			auto monsterWalk_Right = monsterStateMachine->AddState(TEXT("Walk_Right"));
			monsterWalk_Right->SetAnimationClip(GetAnimationClip(TEXT("walk_right")));
			allState.push_back(monsterWalk_Right);

			auto monsterWalk_Left = monsterStateMachine->AddState(TEXT("Walk_Left"));
			monsterWalk_Left->SetAnimationClip(GetAnimationClip(TEXT("walk_left")));
			allState.push_back(monsterWalk_Left);

			auto monsterWalk_Back = monsterStateMachine->AddState(TEXT("Walk_Back"));
			monsterWalk_Back->SetAnimationClip(GetAnimationClip(TEXT("walk_back")));
			allState.push_back(monsterWalk_Back);

			auto monsterRun = monsterStateMachine->AddState(TEXT("Run"));
			monsterRun->SetAnimationClip(GetAnimationClip(TEXT("run")));
			allState.push_back(monsterRun);

			auto monsterAttack = monsterStateMachine->AddState(TEXT("Attack"));
			monsterAttack->SetAnimationClip(GetAnimationClip(TEXT("attack_far")));
			allState.push_back(monsterAttack);

			auto monsterJump_BackWard = monsterStateMachine->AddState(TEXT("Jump_Backward"));
			GetAnimationClip(TEXT("jump_backward"))->SetIsRootMotion(true);
			GetAnimationClip(TEXT("jump_backward"))->SetRootMotionTargetIndex(1);								// 이 애니메이션은 1번 본이 루트 모션 타겟입니다.
			monsterJump_BackWard->SetAnimationClip(GetAnimationClip(TEXT("jump_backward")));
			allState.push_back(monsterJump_BackWard);

			auto monsterWakeUp = monsterStateMachine->AddState(TEXT("WakeUp"));
			monsterWakeUp->SetAnimationClip(GetAnimationClip(TEXT("get_ up"))); // 지금 테스트 용으로 넣어뒀다..! 나중에 변경하자.
			allState.push_back(monsterWakeUp);

			auto monsterHit_Back = monsterStateMachine->AddState(TEXT("Hit_Back"));
			monsterHit_Back->SetAnimationClip(GetAnimationClip(TEXT("hit_back")));
			allState.push_back(monsterHit_Back);

			auto monsterHit_Front = monsterStateMachine->AddState(TEXT("Hit_Front"));
			monsterHit_Front->SetAnimationClip(GetAnimationClip(TEXT("hit_front")));
			allState.push_back(monsterHit_Front);

			for (auto& iter : allState)
			{
				if (monsterHit_Front == iter) continue;
				auto transition = iter->AddTransition(monsterHit_Front);
				transition->AddCondition(TEXT("IsHit_Front"), AnimatorConditionMode::True);
				transition->SetTransitionDuration(0.01f);
				transition->SetTransitionOffset(0.f);
			}

			for (auto& iter : allState)
			{
				if (monsterHit_Back == iter) continue;
				auto transition = iter->AddTransition(monsterHit_Back);
				transition->AddCondition(TEXT("IsHit_Back"), AnimatorConditionMode::True);
				transition->SetTransitionDuration(0.01f);
				transition->SetTransitionOffset(0.f);
			}

			auto monsterDie_Ground = monsterStateMachine->AddState(TEXT("Die_Ground"));
			monsterDie_Ground->SetAnimationClip(GetAnimationClip(TEXT("die_ground")));

			auto monsterDie_Air = monsterStateMachine->AddState(TEXT("Die_Air"));
			monsterDie_Air->SetAnimationClip(GetAnimationClip(TEXT("die_air")));
			// RandOffset

			monsterIdle->SetOffsetParameter(TEXT("RandOffset"));
			monsterIdle->SetOffsetParameterActive(true);

			monsterHit_Front->SetOffsetParameter(TEXT("RandOffset"));
			monsterHit_Front->SetOffsetParameterActive(true);

			monsterHit_Back->SetOffsetParameter(TEXT("RandOffset"));
			monsterHit_Back->SetOffsetParameterActive(true);

			// Transition // 트랜지션의 우선순위는 먼저 등록한순이다.
			auto monsterIdleToWalk = monsterIdle->AddTransition(monsterWalk);
			auto monsterIdleToWalk_Right = monsterIdle->AddTransition(monsterWalk_Right);
			auto monsterIdleToWalk_Left = monsterIdle->AddTransition(monsterWalk_Left);
			auto monsterIdleToWalk_Back = monsterIdle->AddTransition(monsterWalk_Back);
			auto monsterIdelToAttack = monsterIdle->AddTransition(monsterAttack);
			auto monsterIdelToJump_Backward = monsterIdle->AddTransition(monsterJump_BackWard);

			auto monsterWalkToIdle = monsterWalk->AddTransition(monsterIdle);
			auto monsterWalkToRun = monsterWalk->AddTransition(monsterRun);

			auto monsterWalk_RightToIdle = monsterWalk_Right->AddTransition(monsterIdle);
			auto monsterWalk_LeftToIdle = monsterWalk_Left->AddTransition(monsterIdle);
			auto monsterWalk_BackToIdle = monsterWalk_Back->AddTransition(monsterIdle);

			auto monsterRunToJump_Backward = monsterRun->AddTransition(monsterJump_BackWard);
			auto monsterRunToWalk = monsterRun->AddTransition(monsterWalk);

			auto monsterAttackToIdle = monsterAttack->AddTransition(monsterIdle);
			auto monsterAttackToJump_BackWard = monsterAttack->AddTransition(monsterJump_BackWard);

			auto monsterJump_BackWardToIdel = monsterJump_BackWard->AddTransition(monsterIdle);

			auto monsterHit_BackToDieGround = monsterHit_Back->AddTransition(monsterDie_Ground);
			auto monsterHit_BackToDieAir = monsterHit_Back->AddTransition(monsterDie_Air);
			auto monsterHit_BackToIdle = monsterHit_Back->AddTransition(monsterIdle);

			auto monsterHit_FrontToDieGround = monsterHit_Front->AddTransition(monsterDie_Ground);
			auto monsterHit_FrontToDieAir = monsterHit_Front->AddTransition(monsterDie_Air);
			auto monsterHit_FrontToIdle = monsterHit_Front->AddTransition(monsterIdle);

			auto monsterDie_GroundToIdle = monsterDie_Ground->AddTransition(monsterIdle);
			auto monsterDie_GroundToDieAir = monsterDie_Ground->AddTransition(monsterDie_Air);
			auto monsterDie_AirToWakeUp = monsterDie_Air->AddTransition(monsterWakeUp);

			auto monsterWake_UpToIdle = monsterWakeUp->AddTransition(monsterIdle);

			monsterWake_UpToIdle->SetTransitionDuration(0.01f);
			monsterWake_UpToIdle->SetTransitionOffset(0.f);

			monsterDie_GroundToDieAir->AddCondition(TEXT("IsAirBorne"), AnimatorConditionMode::True);
			monsterDie_GroundToDieAir->SetTransitionDuration(0.01f);
			monsterDie_GroundToDieAir->SetTransitionOffset(0.f);

			monsterHit_BackToDieAir->AddCondition(TEXT("IsAirBorne"), AnimatorConditionMode::True);
			monsterHit_BackToDieAir->SetTransitionDuration(0.01f);
			monsterHit_BackToDieAir->SetTransitionOffset(0.f);

			monsterHit_FrontToDieAir->AddCondition(TEXT("IsAirBorne"), AnimatorConditionMode::True);
			monsterHit_FrontToDieAir->SetTransitionDuration(0.01f);
			monsterHit_FrontToDieAir->SetTransitionOffset(0.f);

			monsterHit_BackToDieGround->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
			monsterHit_BackToDieGround->SetTransitionDuration(0.01f);
			monsterHit_BackToDieGround->SetTransitionOffset(0.f);

			monsterHit_FrontToDieGround->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
			monsterHit_FrontToDieGround->SetTransitionDuration(0.01f);
			monsterHit_FrontToDieGround->SetTransitionOffset(0.f);

			monsterDie_GroundToIdle->AddCondition(TEXT("IsDie"), AnimatorConditionMode::False);
			monsterDie_GroundToIdle->SetTransitionDuration(0.01f);
			monsterDie_GroundToIdle->SetTransitionOffset(0.f);

			monsterDie_AirToWakeUp->AddCondition(TEXT("IsAirBorne"), AnimatorConditionMode::False);
			monsterDie_AirToWakeUp->AddCondition(TEXT("IsDie"), AnimatorConditionMode::False);
			monsterDie_AirToWakeUp->SetTransitionDuration(0.01f);
			monsterDie_AirToWakeUp->SetTransitionOffset(0.f);

			monsterHit_BackToIdle->SetTransitionDuration(0.01f);
			monsterHit_BackToIdle->SetTransitionOffset(0.f);

			monsterHit_FrontToIdle->SetTransitionDuration(0.01f);
			monsterHit_FrontToIdle->SetTransitionOffset(0.f);

			monsterJump_BackWardToIdel->SetTransitionDuration(0.01f);
			monsterJump_BackWardToIdel->SetTransitionOffset(0.f);

			monsterRunToJump_Backward->AddCondition(TEXT("IsJump_Backward"), AnimatorConditionMode::True);
			monsterRunToJump_Backward->SetTransitionDuration(0.01f);
			monsterRunToJump_Backward->SetTransitionOffset(0.f);

			monsterIdelToJump_Backward->AddCondition(TEXT("IsJump_Backward"), AnimatorConditionMode::True);
			monsterIdelToJump_Backward->SetTransitionDuration(0.01f);
			monsterIdelToJump_Backward->SetTransitionOffset(0.f);

			monsterIdelToAttack->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
			monsterIdelToAttack->SetTransitionDuration(0.01f);
			monsterIdelToAttack->SetTransitionOffset(0.f);

			monsterAttackToJump_BackWard->AddCondition(TEXT("IsJump_Backward"), AnimatorConditionMode::True);
			monsterAttackToJump_BackWard->SetTransitionDuration(0.01f);
			monsterAttackToJump_BackWard->SetTransitionOffset(0.0f);
			monsterAttackToIdle->SetTransitionDuration(0.1f);
			monsterAttackToIdle->SetTransitionOffset(0.f);

			monsterIdleToWalk->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Greater, 0.5f);
			monsterIdleToWalk->SetTransitionDuration(0.1f);
			monsterIdleToWalk->SetTransitionOffset(0.f);

			monsterWalkToIdle->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Less, 0.49f);
			monsterWalkToIdle->SetTransitionDuration(0.1f);
			monsterWalkToIdle->SetTransitionOffset(0.f);

			monsterIdleToWalk_Right->AddCondition(TEXT("IsWalkRight"), AnimatorConditionMode::True);
			monsterIdleToWalk_Right->SetTransitionDuration(0.1f);
			monsterIdleToWalk_Right->SetTransitionOffset(0.f);

			monsterIdleToWalk_Left->AddCondition(TEXT("IsWalkLeft"), AnimatorConditionMode::True);
			monsterIdleToWalk_Left->SetTransitionDuration(0.1f);
			monsterIdleToWalk_Left->SetTransitionOffset(0.f);

			monsterIdleToWalk_Back->AddCondition(TEXT("IsWalkBack"), AnimatorConditionMode::True);
			monsterIdleToWalk_Back->SetTransitionDuration(0.1f);
			monsterIdleToWalk_Back->SetTransitionOffset(0.f);

			monsterWalk_RightToIdle->AddCondition(TEXT("IsWalkRight"), AnimatorConditionMode::False);
			monsterWalk_RightToIdle->SetTransitionDuration(0.1f);
			monsterWalk_RightToIdle->SetTransitionOffset(0.f);

			monsterWalk_LeftToIdle->AddCondition(TEXT("IsWalkLeft"), AnimatorConditionMode::False);
			monsterWalk_LeftToIdle->SetTransitionDuration(0.1f);
			monsterWalk_LeftToIdle->SetTransitionOffset(0.f);

			monsterWalk_BackToIdle->AddCondition(TEXT("IsWalkBack"), AnimatorConditionMode::False);
			monsterWalk_BackToIdle->SetTransitionDuration(0.1f);
			monsterWalk_BackToIdle->SetTransitionOffset(0.f);

			monsterWalkToRun->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Greater, 1.0f);
			monsterWalkToRun->SetTransitionDuration(0.1f);
			monsterWalkToRun->SetTransitionOffset(0.f);

			monsterRunToWalk->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Less, 0.99f);
			monsterRunToWalk->SetTransitionDuration(0.1f);
			monsterRunToWalk->SetTransitionOffset(0.f);

			// ------------------------------ Event Registe ---------------------------

			auto attackClip = GetAnimationClip(TEXT("attack_far"));
			AnimationEvent attackEvent;

			attackEvent._eventName = TEXT("Attack_Far");
			attackEvent._targetFrame = 10.0f;

			attackClip->AddEvent(attackEvent);

			auto dieGroundClip = GetAnimationClip(TEXT("die_ground"));
			AnimationEvent dieGroundEvent;

			dieGroundEvent._eventName = TEXT("Die");
			dieGroundEvent._targetFrame = 43.0f;

			dieGroundClip->AddEvent(dieGroundEvent);

			auto dieAirClip = GetAnimationClip(TEXT("die_air"));
			AnimationEvent dieAirEvent;

			dieAirEvent._eventName = TEXT("Die");
			dieAirEvent._targetFrame = 29.0f;

			dieAirClip->AddEvent(dieAirEvent);

			// ------------------------------ Event Registe ---------------------------

			_animatorControllerIDMap.insert({ monsterAnimCon->GetName(), monsterAnimCon });

			_resourceUUIDMap.insert({ monsterAnimCon->GetUUID(), monsterAnimCon.get() });

			DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(monsterAnimCon.get(), TEXT("Asset/AnimatorController/Monster_AnimatorController_far.dcontroller"));
		}
#pragma endregion
	}

	void ResourceManager::LoadAudioClipTable(const DUOLCommon::tstring& path)
	{
		// TODO : 뭐든지 자체 포맷 .. 메타 데이터를 통한 프로젝트 패스에서의 자동 참조화가 필요하다 ..
		DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/drumloop.wav"), TEXT("DrumLoop"));

		DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/SwordSlash.wav"), TEXT("SwordSlashWav"));

		DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/SwordSlash.mp3"), TEXT("SwordSlashMp3"));
	}

	DUOLGameEngine::AnimatorController* ResourceManager::LoadAnimatorController(const DUOLCommon::tstring& path)
	{
		auto animCon = DUOLGameEngine::SerializeManager::GetInstance()->
			DeserializeAnimatorController(path);

		if (animCon == nullptr)
			return nullptr;

		if (_animatorControllerIDMap.contains(animCon->GetName()))
			return _animatorControllerIDMap.at(animCon->GetName()).get();

		_animatorControllerIDMap.insert({ animCon->GetName(), animCon });

		_resourceUUIDMap.insert({ animCon->GetUUID(), animCon.get() });

		return animCon.get();
	}

	const std::vector<std::shared_ptr<DUOLGraphicsEngine::RenderingData_Particle>>* ResourceManager::LoadRenderingData_Particle(
		const DUOLCommon::tstring& path)
	{
		if (_renderingData_ParticleIDMap.contains(path))
			return &_renderingData_ParticleIDMap.at(path);
		else
			return nullptr;
	}

	std::vector<std::shared_ptr<DUOLGraphicsEngine::RenderingData_Particle>> ResourceManager::CreateRenderingData_Particle(
		const DUOLCommon::tstring& path)
	{
		std::ifstream fr(path);

		std::vector<DUOLGraphicsEngine::RenderingData_Particle> readDatas;

		std::vector<std::shared_ptr<DUOLGraphicsEngine::RenderingData_Particle>> OutputDatas;

		if (fr.is_open())
		{
			boost::archive::text_iarchive inArchive(fr);

			inArchive >> readDatas;

			fr.close();
		}
		else
		{
			DUOL_CONSOLE("CreateRenderingData_Particle Fail..");

			return std::vector<std::shared_ptr<DUOLGraphicsEngine::RenderingData_Particle>>();
		}

		for (int i = 0; i < readDatas.size(); i++)
		{
			OutputDatas.push_back(std::make_shared<DUOLGraphicsEngine::RenderingData_Particle>(readDatas[i]));
		}

		_renderingData_ParticleIDMap.insert({ path, OutputDatas });

		return OutputDatas;
	}

	void ResourceManager::AddAnimatorController(std::shared_ptr<DUOLGameEngine::AnimatorController> animCon)
	{
		if (_animatorControllerIDMap.contains(animCon->GetName()) || _resourceUUIDMap.contains(animCon->GetUUID()))
			return;

		_animatorControllerIDMap.insert({ animCon->GetName(), animCon });

		_resourceUUIDMap.insert({ animCon->GetUUID(), animCon.get() });
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

	const std::unordered_map<DUOLCommon::tstring, std::shared_ptr<DUOLGameEngine::Mesh>>& ResourceManager::GetAllMeshes() const
	{
		return _meshIDMap;
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

	DUOLGameEngine::Sprite* ResourceManager::GetSprite(const DUOLCommon::tstring& spriteID) const
	{
		return _spriteIDMap.contains(spriteID) ? _spriteIDMap.at(spriteID).get() : nullptr;
	}

	DUOLGameEngine::AudioClip* ResourceManager::GetAudioClip(const DUOLCommon::tstring& audioClipID) const
	{
		return DUOLGameEngine::SoundManager::GetInstance()->GetAudioClip(audioClipID);
	}

	const std::unordered_map<DUOLCommon::tstring, std::unique_ptr<DUOLGameEngine::AudioClip>>& ResourceManager::GetAllAudioClips() const
	{
		return DUOLGameEngine::SoundManager::GetInstance()->_audioClips;
	}

	void ResourceManager::InsertSprite(const DUOLCommon::tstring& spriteName)
	{
		auto sprite = std::make_shared<DUOLGameEngine::Sprite>();

		sprite->SetSpriteName(spriteName);

		std::string path = "UI/" + DUOLCommon::StringHelper::ToString(spriteName);

		auto texture = DUOLGameEngine::ResourceManager::GetInstance()->CreateTexture(DUOLCommon::StringHelper::ToTString(path));

		sprite->GetSprite()->_texture = texture;

		_spriteIDMap.insert({ spriteName, sprite });
	}

	const std::unordered_map<DUOLCommon::tstring, std::shared_ptr<DUOLGameEngine::AnimatorController>>& ResourceManager::GetAllAnimatorControllers() const
	{
		return _animatorControllerIDMap;
	}

	const std::unordered_map<DUOLCommon::tstring, std::shared_ptr<DUOLGameEngine::Material>>& ResourceManager::
		GetAllMaterialMap() const
	{
		return _materialIDMap;
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

	DUOLGraphicsEngine::Light* ResourceManager::CreateLight(const uint64_t& lightId) const
	{
		return _graphicsEngine->CreateLight(lightId);
	}

	DUOLGraphicsLibrary::IFont* ResourceManager::CreateIFont(const DUOLCommon::tstring& fontPath) const
	{
		return _graphicsEngine->CreateIFont(fontPath);
	}

	DUOLGraphicsLibrary::ICanvas* ResourceManager::CreateCanvas(const DUOLGraphicsLibrary::CanvasRenderMode renderMode, const DUOLCommon::tstring& canvasName, int width, int height) const
	{
		return _graphicsEngine->CreateCanvas(renderMode, canvasName, width, height);
	}

	bool ResourceManager::DeleteCanvas(const DUOLCommon::tstring& canvasname)
	{
		return _graphicsEngine->DeleteCanvas(canvasname);
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

	void ResourceManager::CreateParticleMaterial(const DUOLCommon::tstring& materialID)
	{
		auto datas = CreateRenderingData_Particle(materialID);

		for (int i = 0; i < datas.size(); i++)
		{
			auto texturePath = DUOLCommon::StringHelper::ToTString(datas[i]->_renderer._texturePath);
			auto trailTexturePath = DUOLCommon::StringHelper::ToTString(datas[i]->_renderer._traillTexturePath);

			auto textureID = texturePath.substr(texturePath.find_first_of(TEXT("/\\")) + 1);

			auto trailID = trailTexturePath.substr(trailTexturePath.find_first_of(TEXT("/\\")) + 1);

			auto mat = CreateMaterial(materialID + DUOLCommon::StringHelper::ToTString(i), textureID, trailID, _T(""), _T("BasicParticle_CS"));

			//Create NoiseMap
			if (datas[i]->GetFlag() & static_cast<unsigned>(DUOLGraphicsEngine::ParticleFlags::Noise))
			{
				DUOLCommon::tstring noiseMapName = _T("NoiseMap");

				noiseMapName += datas[i]->_noise._frequency;
				noiseMapName += datas[i]->_noise._octaves;
				noiseMapName += datas[i]->_noise._octaveMultiplier;

				mat->GetPrimitiveMaterial()->SetMetallicSmoothnessAOMap(
					CreateNoiseTexture(noiseMapName,
						datas[i]->_noise._frequency,
						datas[i]->_noise._octaves,
						datas[i]->_noise._octaveMultiplier,
						100,
						100));
			}
		}
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const DUOLCommon::tstring& textureID)
	{
		return _graphicsEngine->CreateTexture(textureID);
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const DUOLCommon::tstring& textureID, float width, float height, int size, void* initialData)
	{
		return _graphicsEngine->CreateTexture(textureID, width, height, size, initialData);
	}

	DUOLGameEngine::Sprite* ResourceManager::CreateSprite(const DUOLCommon::tstring& spriteID)
	{
		auto spriteObj = std::make_shared<Sprite>(spriteID);

		_spriteIDMap.insert({ spriteID, spriteObj });

		return spriteObj.get();
	}

	bool ResourceManager::DeleteLight(const uint64_t& lightID) const
	{
		return _graphicsEngine->DeleteLight(lightID);
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateNoiseTexture(const DUOLCommon::tstring& textureID,
		float frequency, int octaves, float octaveMultiplier, float sizeX, float sizeY)
	{
		uint32_t seed = 0;

		const siv::PerlinNoise perlin0{ seed };
		const siv::PerlinNoise perlin1{ seed + 1 };
		const siv::PerlinNoise perlin2{ seed + 2 };
		const double fx = (frequency / sizeX);
		const double fy = (frequency / sizeY);

		std::vector<DUOLMath::Vector4> colors(sizeX * sizeY);

		for (std::int32_t y = 0; y < sizeY; ++y)
		{
			for (std::int32_t x = 0; x < sizeX; ++x)
			{
				int index = sizeX * y + x;

				colors[index].x = perlin0.octave2D_01((x * fx), (y * fy), octaves, octaveMultiplier);
				colors[index].y = perlin1.octave2D_01((x * fx), (y * fy), octaves, octaveMultiplier);
				colors[index].z = perlin2.octave2D_01((x * fx), (y * fy), octaves, octaveMultiplier);
				colors[index].w = 1.0f;
			}
		}
		return CreateTexture(textureID, sizeX, sizeY, sizeX * sizeY * sizeof(DUOLMath::Vector4), colors.data());
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

		// Insert Sprite Name
		ret = GetSprite(name);

		if (ret != nullptr)
			return ret;

		// Check AudioClip
		ret = GetAudioClip(name);

		if (ret != nullptr)
			return ret;

		return nullptr;
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

		LoadAudioClipTable(gameSpec.projectPath + TEXT("Asset/DataTable/AudioClipTable.json"));
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