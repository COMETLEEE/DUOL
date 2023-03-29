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
#pragma region PLAYER_ANIMATOR_CONTROLLER (진)
		auto playerSwordAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Player_SwordAnimatorController"));

		auto playerStateMachine = playerSwordAnimCon->AddStateMachine(TEXT("PlayerStateMachine"));

		// Move State 통제
		playerSwordAnimCon->AddParameter(TEXT("IsMove"), AnimatorControllerParameterType::Bool);

		playerSwordAnimCon->AddParameter(TEXT("IsRun"), AnimatorControllerParameterType::Bool);

		// Attack State 통제
		playerSwordAnimCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);

		playerSwordAnimCon->AddParameter(TEXT("IsLockOn"), AnimatorControllerParameterType::Bool);

		playerSwordAnimCon->AddParameter(TEXT("IsDash"), AnimatorControllerParameterType::Bool);

		playerSwordAnimCon->AddParameter(TEXT("IsLeft"), AnimatorControllerParameterType::Bool);

		playerSwordAnimCon->AddParameter(TEXT("IsRight"), AnimatorControllerParameterType::Bool);

		playerSwordAnimCon->AddParameter(TEXT("IsFront"), AnimatorControllerParameterType::Bool);

		playerSwordAnimCon->AddParameter(TEXT("IsBack"), AnimatorControllerParameterType::Bool);


		// Sword_Idle
		auto playerIdle = playerStateMachine->AddState(TEXT("Idle"));

		playerIdle->SetAnimationClip(GetAnimationClip(TEXT("player_sword_idle")));
		
		// Sword_Move
		auto playerMove = playerStateMachine->AddState(TEXT("Move"));

		playerMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_run")));

		auto playerRun = playerStateMachine->AddState(TEXT("Run"));

		playerRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_run")));

		// Sword_BasicCombo
		auto playerBasicCombo = playerStateMachine->AddState(TEXT("BasicCombo"));

		auto basicComboClip = GetAnimationClip(TEXT("player_sword_basiccombo"));

		basicComboClip->SetIsRootMotion(true);

		basicComboClip->SetIsUseEventInTransition(false);

#pragma region SWORD_BASICCOMBO_EVENTS
		AnimationEvent swordBasicFirstCancleStart;

		swordBasicFirstCancleStart._eventName = TEXT("SwordFirstCancleStart");

		swordBasicFirstCancleStart._targetFrame = 10.f;

		AnimationEvent swordBasicFirstHit;

		swordBasicFirstHit._eventName = TEXT("SwordFirstHit");

		swordBasicFirstHit._targetFrame = 8.f;

		AnimationEvent swordBasicFirstCancleEnd;

		swordBasicFirstCancleEnd._eventName = TEXT("SwordFirstCancleEnd");

		swordBasicFirstCancleEnd._targetFrame = 28.f;

		AnimationEvent swordBasicSecondCancleStart;

		swordBasicSecondCancleStart._eventName = TEXT("SwordSecondCancleStart");

		swordBasicSecondCancleStart._targetFrame = 36.f;

		AnimationEvent swordBasicSecondHit;

		swordBasicSecondHit._eventName = TEXT("SwordSecondHit");

		swordBasicSecondHit._targetFrame = 32.f;

		AnimationEvent swordBasicSecondCancleEnd;

		swordBasicSecondCancleEnd._eventName = TEXT("SwordSecondCancleEnd");

		swordBasicSecondCancleEnd._targetFrame = 55.f;

		AnimationEvent swordBasicThirdCancleStart;

		swordBasicThirdCancleStart._eventName = TEXT("SwordThirdCancleStart");

		swordBasicThirdCancleStart._targetFrame = 82.f;

		AnimationEvent swordBasicThirdHit;

		swordBasicThirdHit._eventName = TEXT("SwordThirdHit");

		swordBasicThirdHit._targetFrame = 92.f;

		AnimationEvent swordBasicThirdCancleEnd;

		swordBasicThirdCancleEnd._eventName = TEXT("SwordThirdCancleEnd");

		swordBasicThirdCancleEnd._targetFrame = 109.f;

		AnimationEvent swordBasicFourthHit;

		swordBasicFourthHit._eventName = TEXT("SwordFourthHit");

		swordBasicFourthHit._targetFrame = 152.f;

		AnimationEvent swordBasicComboEnd;

		swordBasicComboEnd._eventName = TEXT("SwordBasicComboEnd");

		swordBasicComboEnd._targetFrame = 226.f;

		// Cancel frames.
		basicComboClip->AddEvent(swordBasicFirstCancleStart);
		basicComboClip->AddEvent(swordBasicSecondCancleStart);
		basicComboClip->AddEvent(swordBasicThirdCancleStart);

		basicComboClip->AddEvent(swordBasicFirstCancleEnd);
		basicComboClip->AddEvent(swordBasicSecondCancleEnd);
		basicComboClip->AddEvent(swordBasicThirdCancleEnd);

		basicComboClip->AddEvent(swordBasicComboEnd);

		// Hit frames.
		basicComboClip->AddEvent(swordBasicFirstHit);
		basicComboClip->AddEvent(swordBasicSecondHit);
		basicComboClip->AddEvent(swordBasicThirdHit);
		basicComboClip->AddEvent(swordBasicFourthHit);

		playerBasicCombo->SetAnimationClip(basicComboClip);
#pragma endregion

		// basic Dash
		auto playerDash = playerStateMachine->AddState(TEXT("Dash"));

		auto basicDashClip = GetAnimationClip(TEXT("player_sword_dash"));

		basicDashClip->SetIsRootMotion(true);

		playerDash->SetAnimationClip(basicDashClip);

#pragma region DASH_EVENTS
		AnimationEvent startDash;

		startDash._eventName = TEXT("Player_StartDash");
		startDash._targetFrame = 1.f;

		AnimationEvent endDash;

		endDash._eventName = TEXT("Player_EndDash");
		endDash._targetFrame = 75.f;

		basicDashClip->AddEvent(startDash);

		basicDashClip->AddEvent(endDash);
#pragma endregion

		// Sword_Lock On Movement
		auto playerLockOnLeftMove = playerStateMachine->AddState(TEXT("LockOnLeftMove"));
		playerLockOnLeftMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_left")));		

		auto playerLockOnRightMove = playerStateMachine->AddState(TEXT("LockOnRightMove"));
		playerLockOnRightMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_right")));

		auto playerLockOnFrontMove = playerStateMachine->AddState(TEXT("LockOnFrontMove"));
		playerLockOnFrontMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_front")));

		auto playerLockOnBackMove = playerStateMachine->AddState(TEXT("LockOnBackMove"));
		playerLockOnBackMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_back")));

		auto playerLockOnLeftRun = playerStateMachine->AddState(TEXT("LockOnLeftRun"));
		playerLockOnLeftRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_left")));

		auto playerLockOnRightRun = playerStateMachine->AddState(TEXT("LockOnRightRun"));
		playerLockOnRightRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_right")));

		auto playerLockOnFrontRun = playerStateMachine->AddState(TEXT("LockOnFrontRun"));
		playerLockOnFrontRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_run")));

		auto playerLockOnBackRun = playerStateMachine->AddState(TEXT("LockOnBackRun"));
		playerLockOnBackRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_back")));


		auto playerLockOnFrontLeftMove = playerStateMachine->AddState(TEXT("LockOnFrontLeftMove"));
		playerLockOnFrontLeftMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_frontleft")));

		auto playerLockOnFrontRightMove = playerStateMachine->AddState(TEXT("LockOnFrontRightMove"));
		playerLockOnFrontRightMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_frontright")));

		auto playerLockOnBackLeftMove = playerStateMachine->AddState(TEXT("LockOnBackLeftMove"));
		playerLockOnBackLeftMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_backleft")));

		auto playerLockOnBackRightMove = playerStateMachine->AddState(TEXT("LockOnBackRightMove"));
		playerLockOnBackRightMove->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_backright")));

		auto playerLockOnFrontLeftRun = playerStateMachine->AddState(TEXT("LockOnFrontLeftRun"));
		playerLockOnFrontLeftRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_frontleft")));

		auto playerLockOnFrontRightRun = playerStateMachine->AddState(TEXT("LockOnFrontRightRun"));
		playerLockOnFrontRightRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_frontright")));

		auto playerLockOnBackLeftRun = playerStateMachine->AddState(TEXT("LockOnBackLeftRun"));
		playerLockOnBackLeftRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_backleft")));

		auto playerLockOnBackRightRun = playerStateMachine->AddState(TEXT("LockOnBackRightRun"));
		playerLockOnBackRightRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_backright")));


		// Sword_Die
		auto playerDie = playerStateMachine->AddState(TEXT("Die"));
		playerDie->SetAnimationClip(GetAnimationClip(TEXT("player_sword_die")));

#pragma region COMBO
		auto playerIdleToBasicCombo = playerIdle->AddTransition(playerBasicCombo);
		playerIdleToBasicCombo->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);

		playerIdleToBasicCombo->SetTransitionDuration(0.01f);
		playerIdleToBasicCombo->SetTransitionOffset(0.f);

		auto playerBasicComboToIdle = playerBasicCombo->AddTransition(playerIdle);

		playerBasicComboToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		playerBasicComboToIdle->SetTransitionDuration(0.01f);
		playerBasicComboToIdle->SetTransitionOffset(0.f);

		auto playerRunToBasicCombo = playerMove->AddTransition(playerBasicCombo);

		playerRunToBasicCombo->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerRunToBasicCombo->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		playerRunToBasicCombo->SetTransitionDuration(0.01f);
		playerRunToBasicCombo->SetTransitionOffset(0.f);

		auto playerBasicComboToRun = playerBasicCombo->AddTransition(playerMove);

		playerBasicComboToRun->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);
		playerBasicComboToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		playerRunToBasicCombo->SetTransitionDuration(0.01f);
		playerRunToBasicCombo->SetTransitionOffset(0.f);
#pragma endregion

#pragma region DASH
		auto playerMoveToDash = playerMove->AddTransition(playerDash);

		playerMoveToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerDashToMove = playerDash->AddTransition(playerMove);

		playerDashToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		playerDashToMove->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		auto playerIdleToDash = playerIdle->AddTransition(playerDash);

		playerIdleToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		playerIdleToDash->SetTransitionDuration(0.01f);

		auto playerDashToIdle = playerDash->AddTransition(playerIdle);

		playerDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		auto playerAttackToDash = playerBasicCombo->AddTransition(playerDash);

		playerAttackToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		playerAttackToDash->SetTransitionDuration(0.01f);
#pragma endregion

#pragma region LOCK_ON_MOVEMENT
		auto playerIdleToLockOnFrontLeft = playerIdle->AddTransition(playerLockOnFrontLeftMove);
		playerIdleToLockOnFrontLeft->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToLockOnFrontLeft->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerIdleToLockOnFrontLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerIdleToLockOnFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		playerIdleToLockOnFrontLeft->SetTransitionDuration(0.01f);

		auto playerIdleToLockOnFrontRight = playerIdle->AddTransition(playerLockOnFrontRightMove);
		playerIdleToLockOnFrontRight->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToLockOnFrontRight->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerIdleToLockOnFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerIdleToLockOnFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		playerIdleToLockOnFrontRight->SetTransitionDuration(0.01f);

		auto playerIdleToLockOnBackLeft = playerIdle->AddTransition(playerLockOnBackLeftMove);
		playerIdleToLockOnBackLeft->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToLockOnBackLeft->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerIdleToLockOnBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerIdleToLockOnBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		playerIdleToLockOnBackLeft->SetTransitionDuration(0.01f);

		auto playerIdleToLockOnBackRight = playerIdle->AddTransition(playerLockOnBackRightMove);
		playerIdleToLockOnBackRight->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToLockOnBackRight->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerIdleToLockOnBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerIdleToLockOnBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		playerIdleToLockOnBackRight->SetTransitionDuration(0.01f);

		auto playerLockOnFrontLeftToIdle = playerLockOnFrontLeftMove->AddTransition(playerIdle);
		playerLockOnFrontLeftToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		auto playerLockOnFrontRightToIdle = playerLockOnFrontRightMove->AddTransition(playerIdle);
		playerLockOnFrontRightToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		auto playerLockOnBackLeftToIdle = playerLockOnBackLeftMove->AddTransition(playerIdle);
		playerLockOnBackLeftToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		auto playerLockOnBackRightToIdle = playerLockOnBackRightMove->AddTransition(playerIdle);
		playerLockOnBackRightToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);


		auto playerMoveToLockOnFrontLeft = playerMove->AddTransition(playerLockOnFrontLeftMove);
		playerMoveToLockOnFrontLeft->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerMoveToLockOnFrontLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerMoveToLockOnFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerMoveToLockOnFrontRight = playerMove->AddTransition(playerLockOnFrontRightMove);
		playerMoveToLockOnFrontRight->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerMoveToLockOnFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerMoveToLockOnFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerMoveToLockOnBackLeft = playerMove->AddTransition(playerLockOnBackLeftMove);
		playerMoveToLockOnBackLeft->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerMoveToLockOnBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerMoveToLockOnBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerMoveToLockOnBackRight = playerMove->AddTransition(playerLockOnBackRightMove);
		playerMoveToLockOnBackRight->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerMoveToLockOnBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerMoveToLockOnBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnFrontLeftToMove = playerLockOnFrontLeftMove->AddTransition(playerMove);
		playerLockOnFrontLeftToMove->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnFrontRightToMove = playerLockOnFrontRightMove->AddTransition(playerMove);
		playerLockOnFrontRightToMove->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnBackLeftToMove = playerLockOnBackLeftMove->AddTransition(playerMove);
		playerLockOnBackLeftToMove->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnBackRightToMove = playerLockOnBackRightMove->AddTransition(playerMove);
		playerLockOnBackRightToMove->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);






		auto playerLockOnFrontLeftToFrontRight = playerLockOnFrontLeftMove->AddTransition(playerLockOnFrontRightMove);
		playerLockOnFrontLeftToFrontRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnFrontLeftToFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
		
		auto playerLockOnFrontLeftToBackLeft = playerLockOnFrontLeftMove->AddTransition(playerLockOnBackLeftMove);
		playerLockOnFrontLeftToBackLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnFrontLeftToBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnFrontLeftToBackRight = playerLockOnFrontLeftMove->AddTransition(playerLockOnBackRightMove);
		playerLockOnFrontLeftToBackRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToBackRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnFrontLeftToBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnFrontLeftToFront = playerLockOnFrontLeftMove->AddTransition(playerLockOnFrontMove);
		playerLockOnFrontLeftToFront->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnFrontLeftToFront->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);

		auto playerLockOnFrontLeftToBack = playerLockOnFrontLeftMove->AddTransition(playerLockOnBackMove);
		playerLockOnFrontLeftToBack->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToBack->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		
		auto playerLockOnFrontLeftToLeft = playerLockOnFrontLeftMove->AddTransition(playerLockOnLeftMove);
		playerLockOnFrontLeftToLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnFrontLeftToRight = playerLockOnFrontLeftMove->AddTransition(playerLockOnRightMove);
		playerLockOnFrontLeftToRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnFrontLeftToRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightToFrontLeft = playerLockOnFrontRightMove->AddTransition(playerLockOnFrontLeftMove);
		playerLockOnFrontRightToFrontLeft->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightToFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightToBackLeft = playerLockOnFrontRightMove->AddTransition(playerLockOnBackLeftMove);
		playerLockOnFrontRightToBackLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightToBackLeft->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightToBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnFrontRightToBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightToBackRight = playerLockOnFrontRightMove->AddTransition(playerLockOnBackRightMove);
		playerLockOnFrontRightToBackRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightToBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
		playerLockOnFrontRightToBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightToFront = playerLockOnFrontRightMove->AddTransition(playerLockOnFrontMove);
		playerLockOnFrontRightToFront->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);

		auto playerLockOnFrontRightToBack = playerLockOnFrontRightMove->AddTransition(playerLockOnBackMove);
		playerLockOnFrontRightToBack->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightToBack->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightToBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightToLeft = playerLockOnFrontRightMove->AddTransition(playerLockOnLeftMove);
		playerLockOnFrontRightToLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightToLeft->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightToLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightToRight = playerLockOnFrontRightMove->AddTransition(playerLockOnRightMove);
		playerLockOnFrontRightToRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightToRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftToFrontLeft = playerLockOnBackLeftMove->AddTransition(playerLockOnFrontLeftMove);
		playerLockOnBackLeftToFrontLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftToFrontLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnBackLeftToFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftToFrontRight = playerLockOnBackLeftMove->AddTransition(playerLockOnFrontRightMove);
		playerLockOnBackLeftToFrontRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftToFrontRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftToFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnBackLeftToFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftToBackRight = playerLockOnBackLeftMove->AddTransition(playerLockOnBackRightMove);
		playerLockOnBackLeftToBackRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftToBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnBackLeftToBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftToFront = playerLockOnBackLeftMove->AddTransition(playerLockOnFrontMove);
		playerLockOnBackLeftToFront->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftToFront->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftToFront->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftToBack = playerLockOnBackLeftMove->AddTransition(playerLockOnBackMove);
		playerLockOnBackLeftToBack->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftToBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftToLeft = playerLockOnBackLeftMove->AddTransition(playerLockOnLeftMove);
		playerLockOnBackLeftToLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftToLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);
		
		auto playerLockOnBackLeftToRight = playerLockOnBackLeftMove->AddTransition(playerLockOnRightMove);
		playerLockOnBackLeftToRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftToRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftToRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnBackRightToFrontLeft = playerLockOnBackRightMove->AddTransition(playerLockOnFrontLeftMove);
		playerLockOnBackRightToFrontLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightToFrontLeft->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightToFrontLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnBackRightToFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnBackRightToFrontRight = playerLockOnBackRightMove->AddTransition(playerLockOnFrontRightMove);		
		playerLockOnBackRightToFrontRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightToFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
		playerLockOnBackRightToFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

		auto playerLockOnBackRightToBackLeft = playerLockOnBackRightMove->AddTransition(playerLockOnBackLeftMove);
		playerLockOnBackRightToBackLeft->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightToBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnBackRightToBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnBackRightToFront = playerLockOnBackRightMove->AddTransition(playerLockOnFrontMove);
		playerLockOnBackRightToFront->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightToFront->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightToFront->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

		auto playerLockOnBackRightToBack = playerLockOnBackRightMove->AddTransition(playerLockOnBackMove);
		playerLockOnBackRightToBack->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightToBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

		auto playerLockOnBackRightToLeft = playerLockOnBackRightMove->AddTransition(playerLockOnLeftMove);
		playerLockOnBackRightToLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightToLeft->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightToLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnBackRightToRight = playerLockOnBackRightMove->AddTransition(playerLockOnRightMove);
		playerLockOnBackRightToRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightToRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);






		auto playerRunToLockOnFrontLeftRun = playerRun->AddTransition(playerLockOnFrontLeftRun);
		playerRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerRunToLockOnFrontLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerRunToLockOnFrontRightRun = playerRun->AddTransition(playerLockOnFrontRightRun);
		playerRunToLockOnFrontRightRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerRunToLockOnFrontRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerRunToLockOnFrontRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerRunToLockOnBackLeftRun = playerRun->AddTransition(playerLockOnBackLeftRun);
		playerRunToLockOnBackLeftRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerRunToLockOnBackLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerRunToLockOnBackLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerRunToLockOnBackRightRun = playerRun->AddTransition(playerLockOnBackRightRun);
		playerRunToLockOnBackRightRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerRunToLockOnBackRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerRunToLockOnBackRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnFrontLeftRunToRun = playerLockOnFrontLeftRun->AddTransition(playerRun);
		playerLockOnFrontLeftRunToRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnFrontRightRunToRun = playerLockOnFrontRightRun->AddTransition(playerRun);
		playerLockOnFrontRightRunToRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnBackLeftRunToRun = playerLockOnBackLeftRun->AddTransition(playerRun);
		playerLockOnBackLeftRunToRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnBackRightRunToRun = playerLockOnBackRightRun->AddTransition(playerRun);
		playerLockOnBackRightRunToRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);


		auto playerLockOnFrontLeftRunToLockOnFrontRightRun = playerLockOnFrontLeftRun->AddTransition(playerLockOnFrontRightRun);
		playerLockOnFrontLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsLest"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnFrontLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);


		auto playerLockOnFrontLeftRunToLockOnBackLeftRun = playerLockOnFrontLeftRun->AddTransition(playerLockOnBackLeftRun);
		playerLockOnFrontLeftRunToLockOnBackLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnBackLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnFrontLeftRunToLockOnBackLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);


		auto playerLockOnFrontLeftRunToLockOnBackRightRun = playerLockOnFrontLeftRun->AddTransition(playerLockOnBackRightRun);
		playerLockOnFrontLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnFrontLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);


		auto playerLockOnFrontLeftRunToLockOnFrontRun = playerLockOnFrontLeftRun->AddTransition(playerLockOnFrontRun);
		playerLockOnFrontLeftRunToLockOnFrontRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnFrontLeftRunToLockOnFrontRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);


		auto playerLockOnFrontLeftRunToLockOnBack = playerLockOnFrontLeftRun->AddTransition(playerLockOnBackRun);
		playerLockOnFrontLeftRunToLockOnBack->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnBack->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

		auto playerLockOnFrontLeftRunToLockOnLeft = playerLockOnFrontLeftRun->AddTransition(playerLockOnLeftRun);
		playerLockOnFrontLeftRunToLockOnLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);


		auto playerLockOnFrontLeftRunToLockOnRight = playerLockOnFrontLeftRun->AddTransition(playerLockOnRightRun);
		playerLockOnFrontLeftRunToLockOnRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontLeftRunToLockOnRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightRunToLockOnFrontLeftRun = playerLockOnFrontRightRun->AddTransition(playerLockOnFrontLeftRun);
		playerLockOnFrontRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnFrontRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);


		auto playerLockOnFrontRightRunToLockOnBackLeftRun = playerLockOnFrontRightRun->AddTransition(playerLockOnBackLeftRun);
		playerLockOnFrontRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnFrontRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightRunToLockOnBackRightRun = playerLockOnFrontRightRun->AddTransition(playerLockOnBackRightRun);
		playerLockOnFrontRightRunToLockOnBackRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnBackRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnFrontRightRunToLockOnBackRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
		


		auto playerLockOnFrontRightRunToLockOnFrontRun = playerLockOnFrontRightRun->AddTransition(playerLockOnFrontRun);
		playerLockOnFrontRightRunToLockOnFrontRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnFrontRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);


		auto playerLockOnFrontRightRunToLockOnBackRun = playerLockOnFrontRightRun->AddTransition(playerLockOnBackRun);
		playerLockOnFrontRightRunToLockOnBackRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnBackRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnBackRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);


		auto playerLockOnFrontRightRunToLockOnLeftRun = playerLockOnFrontRightRun->AddTransition(playerLockOnLeftRun);
		playerLockOnFrontRightRunToLockOnLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnLeftRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightRunToLockOnRightRun = playerLockOnFrontRightRun->AddTransition(playerLockOnRightRun);
		playerLockOnFrontRightRunToLockOnRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
		playerLockOnFrontRightRunToLockOnRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);



		auto playerLockOnBackLeftRunToLockOnFrontLeftRun = playerLockOnBackLeftRun->AddTransition(playerLockOnFrontLeftRun);
		playerLockOnBackLeftRunToLockOnFrontLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnFrontLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnBackLeftRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftRunToLockOnFrontRightRun = playerLockOnBackLeftRun->AddTransition(playerLockOnFrontRightRun);
		playerLockOnBackLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnBackLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);


		auto playerLockOnBackLeftRunToLockOnBackRightRun = playerLockOnBackLeftRun->AddTransition(playerLockOnBackRightRun);
		playerLockOnBackLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnBackLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);


		auto playerLockOnBackLeftRunToLockOnFrontRun = playerLockOnBackLeftRun->AddTransition(playerLockOnFrontRun);
		playerLockOnBackLeftRunToLockOnFrontRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnFrontRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnFrontRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftRunToLockOnBackRun = playerLockOnBackLeftRun->AddTransition(playerLockOnBackRun);
		playerLockOnBackLeftRunToLockOnBackRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnBackRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		

		auto playerLockOnBackLeftRunToLockOnLeftRun = playerLockOnBackLeftRun->AddTransition(playerLockOnLeftRun);
		playerLockOnBackLeftRunToLockOnLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftRunToLockOnRightRun = playerLockOnBackLeftRun->AddTransition(playerLockOnRightRun);
		playerLockOnBackLeftRunToLockOnRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnRightRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
		playerLockOnBackLeftRunToLockOnRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);


		auto playerLockOnBackRightRunToLockOnFrontLeftRun = playerLockOnBackRightRun->AddTransition(playerLockOnFrontLeftRun);
		playerLockOnBackRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnBackRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnBackRightRunToLockOnFrontRightRun = playerLockOnBackRightRun->AddTransition(playerLockOnFrontRightRun);
		playerLockOnBackRightRunToLockOnFrontRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnFrontRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerLockOnBackRightRunToLockOnFrontRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
		

		auto playerLockOnBackRightRunToLockOnBackLeftRun = playerLockOnBackRightRun->AddTransition(playerLockOnBackLeftRun);
		playerLockOnBackRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerLockOnBackRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);



		auto playerLockOnBackRightRunToLockOnFrontRun = playerLockOnBackRightRun->AddTransition(playerLockOnFrontRun);
		playerLockOnBackRightRunToLockOnFrontRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnFrontRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnFrontRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);


		auto playerLockOnBackRightRunToLockOnBackRun = playerLockOnBackRightRun->AddTransition(playerLockOnBackRun);
		playerLockOnBackRightRunToLockOnBackRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnBackRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);



		auto playerLockOnBackRightRunToLockOnLeftRun = playerLockOnBackRightRun->AddTransition(playerLockOnLeftRun);
		playerLockOnBackRightRunToLockOnLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnLeftRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerLockOnBackRightRunToLockOnRightRun = playerLockOnBackRightRun->AddTransition(playerLockOnRightRun);
		playerLockOnBackRightRunToLockOnRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
		playerLockOnBackRightRunToLockOnRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);




















		auto playerIdleToLockOnFront = playerIdle->AddTransition(playerLockOnFrontMove);
		playerIdleToLockOnFront->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToLockOnFront->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
		playerIdleToLockOnFront->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);

		playerIdleToLockOnFront->SetTransitionDuration(0.01f);
		playerIdleToLockOnFront->SetTransitionOffset(0.01f);

		auto playerIdleToLockOnBack = playerIdle->AddTransition(playerLockOnBackMove);
		playerIdleToLockOnBack->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToLockOnBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
		playerIdleToLockOnBack->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);

		playerIdleToLockOnBack->SetTransitionDuration(0.01f);
		playerIdleToLockOnBack->SetTransitionOffset(0.01f);

		auto playerIdleToLockOnLeft = playerIdle->AddTransition(playerLockOnLeftMove);
		playerIdleToLockOnLeft->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToLockOnLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);
		playerIdleToLockOnLeft->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);

		playerIdleToLockOnLeft->SetTransitionDuration(0.01f);
		playerIdleToLockOnLeft->SetTransitionOffset(0.01f);

		auto playerIdleToLockOnRight = playerIdle->AddTransition(playerLockOnRightMove);
		playerIdleToLockOnRight->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToLockOnRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
		playerIdleToLockOnRight->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);

		playerIdleToLockOnRight->SetTransitionDuration(0.01f);
		playerIdleToLockOnRight->SetTransitionOffset(0.01f);

		auto playerLockOnFrontMoveToIdle = playerLockOnFrontMove->AddTransition(playerIdle);
		playerLockOnFrontMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		playerLockOnFrontMoveToIdle->SetTransitionDuration(0.01f);
		playerLockOnFrontMoveToIdle->SetTransitionOffset(0.01f);

		auto playerLockOnBackMoveToIdle = playerLockOnBackMove->AddTransition(playerIdle);
		playerLockOnBackMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		playerLockOnBackMoveToIdle->SetTransitionDuration(0.01f);
		playerLockOnBackMoveToIdle->SetTransitionOffset(0.01f);

		auto playerLockOnLeftMoveToIdle = playerLockOnLeftMove->AddTransition(playerIdle);
		playerLockOnLeftMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		playerLockOnLeftMoveToIdle->SetTransitionDuration(0.01f);
		playerLockOnLeftMoveToIdle->SetTransitionOffset(0.01f);

		auto playerLockOnRightMoveToIdle = playerLockOnRightMove->AddTransition(playerIdle);
		playerLockOnRightMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		playerLockOnRightMoveToIdle->SetTransitionDuration(0.01f);
		playerLockOnRightMoveToIdle->SetTransitionOffset(0.01f);







		auto playerMoveToLockOnFront = playerMove->AddTransition(playerLockOnFrontMove);
		playerMoveToLockOnFront->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerMoveToLockOnFront->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

		auto playerMoveToLockOnBack = playerMove->AddTransition(playerLockOnBackMove);
		playerMoveToLockOnBack->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerMoveToLockOnBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

		auto playerMoveToLockOnLeft = playerMove->AddTransition(playerLockOnLeftMove);
		playerMoveToLockOnLeft->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerMoveToLockOnLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerMoveToLockOnRight = playerMove->AddTransition(playerLockOnRightMove);
		playerMoveToLockOnRight->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerMoveToLockOnRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnFrontToMove = playerLockOnFrontMove->AddTransition(playerMove);
		playerLockOnFrontToMove->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnBackToMove = playerLockOnBackMove->AddTransition(playerMove);
		playerLockOnBackToMove->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnLeftToMove = playerLockOnLeftMove->AddTransition(playerMove);
		playerLockOnLeftToMove->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnRightToMove = playerLockOnRightMove->AddTransition(playerMove);
		playerLockOnRightToMove->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);




		auto playerRunToLockOnFrontRun = playerRun->AddTransition(playerLockOnFrontRun);
		playerRunToLockOnFrontRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerRunToLockOnFrontRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
		playerRunToLockOnFrontRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

		auto playerRunToLockOnBackRun = playerRun->AddTransition(playerLockOnBackRun);
		playerRunToLockOnBackRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
		playerRunToLockOnBackRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerRunToLockOnBackRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

		auto playerRunToLockOnLeftRun = playerRun->AddTransition(playerLockOnLeftRun);
		playerRunToLockOnLeftRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
		playerRunToLockOnLeftRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerRunToLockOnLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		auto playerRunToLockOnRightRun = playerRun->AddTransition(playerLockOnRightRun);
		playerRunToLockOnRightRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
		playerRunToLockOnRightRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::True);
		playerRunToLockOnRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		auto playerLockOnFrontRunToRun = playerLockOnFrontRun->AddTransition(playerRun);
		playerLockOnFrontRunToRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnBackRunToRun = playerLockOnBackRun->AddTransition(playerRun);
		playerLockOnBackRunToRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnLeftRunToRun = playerLockOnLeftRun->AddTransition(playerRun);
		playerLockOnLeftRunToRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);

		auto playerLockOnRightRunToRun = playerLockOnRightRun->AddTransition(playerRun);
		playerLockOnRightRunToRun->AddCondition(TEXT("IsLockOn"), AnimatorConditionMode::False);





		auto playerLockOnFrontLeftToLockOnFrontLeftRun = playerLockOnFrontLeftMove->AddTransition(playerLockOnFrontLeftRun);
		playerLockOnFrontLeftToLockOnFrontLeftRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerLockOnFrontRightToLockOnFrontRightRun = playerLockOnFrontRightMove->AddTransition(playerLockOnFrontRightRun);
		playerLockOnFrontRightToLockOnFrontRightRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerLockOnBackLeftToLockOnBackLeftRun = playerLockOnBackLeftMove->AddTransition(playerLockOnBackLeftRun);
		playerLockOnBackLeftToLockOnBackLeftRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerLockOnBackRightToLockOnBackRightRun = playerLockOnBackRightMove->AddTransition(playerLockOnBackRightRun);
		playerLockOnBackRightToLockOnBackRightRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);


		auto playerLockOnFrontToLockOnFrontRun = playerLockOnFrontMove->AddTransition(playerLockOnFrontRun);
		playerLockOnFrontToLockOnFrontRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerLockOnBackToLockOnBackRun = playerLockOnBackMove->AddTransition(playerLockOnBackRun);
		playerLockOnBackToLockOnBackRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerLockOnLeftToLockOnLeftRun = playerLockOnLeftMove->AddTransition(playerLockOnLeftRun);
		playerLockOnLeftToLockOnLeftRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerLockOnRightToLockOnRightRun = playerLockOnRightMove->AddTransition(playerLockOnRightRun);
		playerLockOnRightToLockOnRightRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);



		auto playerLockOnFrontLeftRunToLockOnFrontLeft = playerLockOnFrontLeftRun->AddTransition(playerLockOnFrontLeftMove);
		playerLockOnFrontLeftRunToLockOnFrontLeft->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerLockOnFrontRightRunToLockOnFrontRight = playerLockOnFrontRightRun->AddTransition(playerLockOnFrontRightMove);
		playerLockOnFrontRightRunToLockOnFrontRight->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerLockOnBackLeftRunToLockOnBackLeft = playerLockOnBackLeftRun->AddTransition(playerLockOnBackLeftMove);
		playerLockOnBackLeftRunToLockOnBackLeft->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerLockOnBackRightRunToLockOnBackRight = playerLockOnBackRightRun->AddTransition(playerLockOnBackRightMove);
		playerLockOnBackRightRunToLockOnBackRight->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);


		auto playerLockOnFrontRunToLockOnFront = playerLockOnFrontRun->AddTransition(playerLockOnFrontMove);
		playerLockOnFrontRunToLockOnFront->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerLockOnBackRunToLockOnBack = playerLockOnBackRun->AddTransition(playerLockOnBackMove);
		playerLockOnBackRunToLockOnBack->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerLockOnLeftRunToLockOnLeft = playerLockOnLeftRun->AddTransition(playerLockOnLeftMove);
		playerLockOnLeftRunToLockOnLeft->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerLockOnRightRunToLockOnRight = playerLockOnRightRun->AddTransition(playerLockOnRightMove);
		playerLockOnRightRunToLockOnRight->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);


#pragma region LOCK_ON_TO_LOCK_ON_MOVEMENT
		{
			{
				auto playerLockOnFrontToLockOnFrontLeft = playerLockOnFrontMove->AddTransition(playerLockOnFrontLeftMove);
				playerLockOnFrontToLockOnFrontLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnFrontToLockOnFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnFrontToLockOnFrontRight = playerLockOnFrontMove->AddTransition(playerLockOnFrontRightMove);
				playerLockOnFrontToLockOnFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnFrontToLockOnFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnFrontToLockOnBackLeft = playerLockOnFrontMove->AddTransition(playerLockOnBackLeftMove);
				playerLockOnFrontToLockOnBackLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
				playerLockOnFrontToLockOnBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnFrontToLockOnBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnFrontToLockOnBackRight = playerLockOnFrontMove->AddTransition(playerLockOnBackRightMove);
				playerLockOnFrontToLockOnBackRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
				playerLockOnFrontToLockOnBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnFrontToLockOnBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnFrontToLockOnLeft = playerLockOnFrontMove->AddTransition(playerLockOnLeftMove);
				playerLockOnFrontToLockOnLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnFrontToLockOnRight = playerLockOnFrontMove->AddTransition(playerLockOnRightMove);
				playerLockOnFrontToLockOnRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnFrontToLockOnBack = playerLockOnFrontMove->AddTransition(playerLockOnBackMove);
				playerLockOnFrontToLockOnBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
			}


			{
				auto playerLockOnBackToLockOnFrontLeft = playerLockOnBackMove->AddTransition(playerLockOnFrontLeftMove);
				playerLockOnBackToLockOnFrontLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
				playerLockOnBackToLockOnFrontLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnBackToLockOnFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnBackToLockOnFrontRight = playerLockOnBackMove->AddTransition(playerLockOnFrontRightMove);
				playerLockOnBackToLockOnFrontRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
				playerLockOnBackToLockOnFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnBackToLockOnFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnBackToLockOnBackLeft = playerLockOnBackMove->AddTransition(playerLockOnBackLeftMove);
				playerLockOnBackToLockOnBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnBackToLockOnBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnBackToLockOnBackRight = playerLockOnBackMove->AddTransition(playerLockOnBackRightMove);
				playerLockOnBackToLockOnBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnBackToLockOnBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnBackToLockOnLeft = playerLockOnBackMove->AddTransition(playerLockOnLeftMove);
				playerLockOnBackToLockOnLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnBackToLockOnRight = playerLockOnBackMove->AddTransition(playerLockOnRightMove);
				playerLockOnBackToLockOnRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnBackToLockOnFront = playerLockOnBackMove->AddTransition(playerLockOnFrontMove);
				playerLockOnBackToLockOnFront->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
			}

			{
				auto playerLockOnLeftToLockOnFrontLeft = playerLockOnLeftMove->AddTransition(playerLockOnFrontLeftMove);
				playerLockOnLeftToLockOnFrontLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnLeftToLockOnFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnLeftToLockOnFrontRight = playerLockOnLeftMove->AddTransition(playerLockOnFrontRightMove);
				playerLockOnLeftToLockOnFrontRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
				playerLockOnLeftToLockOnFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnLeftToLockOnFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);


				auto playerLockOnLeftToLockOnBackLeft = playerLockOnLeftMove->AddTransition(playerLockOnBackLeftMove);
				playerLockOnLeftToLockOnBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnLeftToLockOnBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnLeftToLockOnBackRight = playerLockOnLeftMove->AddTransition(playerLockOnBackRightMove);
				playerLockOnLeftToLockOnBackRight->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
				playerLockOnLeftToLockOnBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnLeftToLockOnBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnLeftToLockOnFront = playerLockOnLeftMove->AddTransition(playerLockOnFrontMove);
				playerLockOnLeftToLockOnFront->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

				auto playerLockOnLeftToLockOnBack = playerLockOnLeftMove->AddTransition(playerLockOnBackMove);
				playerLockOnLeftToLockOnBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

				auto playerLockOnLeftToLockOnRight = playerLockOnLeftMove->AddTransition(playerLockOnRightMove);
				playerLockOnLeftToLockOnRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
			}

			{
				auto playerLockOnRightToLockOnFrontLeft = playerLockOnRightMove->AddTransition(playerLockOnFrontLeftMove);
				playerLockOnRightToLockOnFrontLeft->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
				playerLockOnRightToLockOnFrontLeft->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnRightToLockOnFrontLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnRightToLockOnFrontRight = playerLockOnRightMove->AddTransition(playerLockOnFrontRightMove);
				playerLockOnRightToLockOnFrontRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
				playerLockOnRightToLockOnFrontRight->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

				auto playerLockOnRightToLockOnBackLeft = playerLockOnRightMove->AddTransition(playerLockOnBackLeftMove);
				playerLockOnRightToLockOnBackLeft->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
				playerLockOnRightToLockOnBackLeft->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnRightToLockOnBackLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnRightToLockOnBackRight = playerLockOnRightMove->AddTransition(playerLockOnBackRightMove);
				playerLockOnRightToLockOnBackRight->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnRightToLockOnBackRight->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnRightToLockOnFront = playerLockOnRightMove->AddTransition(playerLockOnFrontMove);
				playerLockOnRightToLockOnFront->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

				auto playerLockOnRightToLockOnBack = playerLockOnRightMove->AddTransition(playerLockOnFrontMove);
				playerLockOnRightToLockOnBack->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

				auto playerLockOnRightToLockOnLeft = playerLockOnRightMove->AddTransition(playerLockOnLeftMove);
				playerLockOnRightToLockOnLeft->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);
			}

			{
				auto playerLockOnFrontRunToLockOnFrontLeftRun = playerLockOnFrontRun->AddTransition(playerLockOnFrontLeftRun);
				playerLockOnFrontRunToLockOnFrontLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnFrontRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnFrontRunToLockOnFrontRightRun = playerLockOnFrontRun->AddTransition(playerLockOnFrontRightRun);
				playerLockOnFrontRunToLockOnFrontRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnFrontRunToLockOnFrontRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnFrontRunToLockOnBackLeftRun = playerLockOnFrontRun->AddTransition(playerLockOnBackLeftRun);
				playerLockOnFrontRunToLockOnBackLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
				playerLockOnFrontRunToLockOnBackLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnFrontRunToLockOnBackLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnFrontRunToLockOnBackRightRun = playerLockOnFrontRun->AddTransition(playerLockOnBackRightRun);
				playerLockOnFrontRunToLockOnBackRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::False);
				playerLockOnFrontRunToLockOnBackRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnFrontRunToLockOnBackRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);


				auto playerLockOnFrontRunToLockOnBackRun = playerLockOnFrontRun->AddTransition(playerLockOnBackRun);
				playerLockOnFrontRunToLockOnBackRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnFrontRunToLockOnBackRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

				auto playerLockOnFrontRunToLockOnLeftRun = playerLockOnFrontRun->AddTransition(playerLockOnLeftRun);
				playerLockOnFrontRunToLockOnLeftRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnFrontRunToLockOnLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnFrontRunToLockOnRightRun = playerLockOnFrontRun->AddTransition(playerLockOnRightRun);
				playerLockOnFrontRunToLockOnRightRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnFrontRunToLockOnRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
			}

			{
				auto playerLockOnBackRunToLockOnFrontLeftRun = playerLockOnBackRun->AddTransition(playerLockOnFrontLeftRun);
				playerLockOnBackRunToLockOnFrontLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
				playerLockOnBackRunToLockOnFrontLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnBackRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);


				auto playerLockOnBackRunToLockOnFrontRightRun = playerLockOnBackRun->AddTransition(playerLockOnFrontRightRun);
				playerLockOnBackRunToLockOnFrontRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::False);
				playerLockOnBackRunToLockOnFrontRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnBackRunToLockOnFrontRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnBackRunToLockOnBackLeftRun = playerLockOnBackRun->AddTransition(playerLockOnBackLeftRun);
				playerLockOnBackRunToLockOnBackLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnBackRunToLockOnBackLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnBackRunToLockOnBackRightRun = playerLockOnBackRun->AddTransition(playerLockOnBackRightRun);
				playerLockOnBackRunToLockOnBackRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnBackRunToLockOnBackRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);


				auto playerLockOnBackRunToLockOnFrontRun = playerLockOnBackRun->AddTransition(playerLockOnFrontRun);
				playerLockOnBackRunToLockOnFrontRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnBackRunToLockOnFrontRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

				auto playerLockOnBackRunToLockOnLeftRun = playerLockOnBackRun->AddTransition(playerLockOnLeftRun);
				playerLockOnBackRunToLockOnLeftRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnBackRunToLockOnLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

				auto playerLockOnBackRunToLockOnRightRun = playerLockOnBackRun->AddTransition(playerLockOnRightRun);
				playerLockOnBackRunToLockOnRightRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnBackRunToLockOnRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
			}

			{
				auto playerLockOnLeftRunToLockOnFrontLeftRun = playerLockOnLeftRun->AddTransition(playerLockOnFrontLeftRun);
				playerLockOnLeftRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);
				playerLockOnLeftRunToLockOnFrontLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

				auto playerLockOnLeftRunToLockOnFrontRightRun = playerLockOnLeftRun->AddTransition(playerLockOnFrontRightRun);
				playerLockOnLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
				playerLockOnLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnLeftRunToLockOnFrontRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnLeftRunToLockOnBackLeftRun = playerLockOnLeftRun->AddTransition(playerLockOnBackLeftRun);
				playerLockOnLeftRunToLockOnBackLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);
				playerLockOnLeftRunToLockOnBackLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

				auto playerLockOnLeftRunToLockOnBackRightRun = playerLockOnLeftRun->AddTransition(playerLockOnBackRightRun);
				playerLockOnLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::False);
				playerLockOnLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnLeftRunToLockOnBackRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

				auto playerLockOnLeftRunToLockOnFrontRun = playerLockOnLeftRun->AddTransition(playerLockOnFrontRun);
				playerLockOnLeftRunToLockOnFrontRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnLeftRunToLockOnFrontRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

				auto playerLockOnLeftRunToLockOnBackRun = playerLockOnLeftRun->AddTransition(playerLockOnBackRun);
				playerLockOnLeftRunToLockOnBackRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnLeftRunToLockOnBackRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

				auto playerLockOnLeftRunToLockOnRightRun = playerLockOnLeftRun->AddTransition(playerLockOnRightRun);
				playerLockOnLeftRunToLockOnRightRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnLeftRunToLockOnRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
			}

			{
				auto playerLockOnRightRunToLockOnFrontLeftRun = playerLockOnRightRun->AddTransition(playerLockOnFrontLeftRun);
				playerLockOnRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
				playerLockOnRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnRightRunToLockOnFrontLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);
				

				auto playerLockOnRightRunToLockOnFrontRightRun = playerLockOnRightRun->AddTransition(playerLockOnFrontRightRun);
				playerLockOnRightRunToLockOnFrontRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);
				playerLockOnRightRunToLockOnFrontRightRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

				auto playerLockOnRightRunToLockOnBackLeftRun = playerLockOnRightRun->AddTransition(playerLockOnBackLeftRun);
				playerLockOnRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
				playerLockOnRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);
				playerLockOnRightRunToLockOnBackLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);


				auto playerLockOnRightRunToLockOnBackRightRun = playerLockOnRightRun->AddTransition(playerLockOnBackRightRun);
				playerLockOnRightRunToLockOnBackRightRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);
				playerLockOnRightRunToLockOnBackRightRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);


				auto playerLockOnRightRunToLockOnFrontRun = playerLockOnRightRun->AddTransition(playerLockOnFrontRun);
				playerLockOnRightRunToLockOnFrontRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnRightRunToLockOnFrontRun->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);
				playerLockOnRightRunToLockOnFrontRun->AddCondition(TEXT("IsRight"), AnimatorConditionMode::False);

				auto playerLockOnRightRunToLockOnBackRun = playerLockOnRightRun->AddTransition(playerLockOnBackRun);
				playerLockOnRightRunToLockOnBackRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnRightRunToLockOnBackRun->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

				auto playerLockOnRightRunToLockOnLeftRun = playerLockOnRightRun->AddTransition(playerLockOnLeftRun);
				playerLockOnRightRunToLockOnLeftRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
				playerLockOnRightRunToLockOnLeftRun->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);
			}
		}
#pragma endregion

#pragma region IDLE_TO_MOVE_AND_RUN
		{
			auto playerIdleToRun = playerIdle->AddTransition(playerRun);
			playerIdleToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
			playerIdleToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

			playerIdleToRun->SetTransitionDuration(0.01f);
			playerIdleToRun->SetTransitionOffset(0.f);

			auto playerRunToIdle = playerRun->AddTransition(playerIdle);
			playerRunToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
			playerRunToIdle->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

			playerRunToIdle->SetTransitionDuration(0.01f);
			playerRunToIdle->SetTransitionOffset(0.f);

			auto playerIdleToMove = playerIdle->AddTransition(playerMove);
			playerIdleToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

			playerIdleToMove->SetTransitionDuration(0.01f);
			playerIdleToMove->SetTransitionOffset(0.f);

			auto playerMoveToIdle = playerMove->AddTransition(playerIdle);
			playerMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

			playerMoveToIdle->SetTransitionDuration(0.01f);
			playerMoveToIdle->SetTransitionOffset(0.f);

			auto playerMoveToRun = playerMove->AddTransition(playerRun);
			playerMoveToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
			playerMoveToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

			playerMoveToRun->SetTransitionDuration(0.01f);
			playerMoveToRun->SetTransitionOffset(0.f);

			auto playerRunToMove = playerRun->AddTransition(playerMove);
			playerRunToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
			playerRunToMove->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

			playerRunToMove->SetTransitionDuration(0.01f);
			playerRunToMove->SetTransitionOffset(0.f);
		}
#pragma endregion

#pragma endregion
		_animatorControllerIDMap.insert({ playerSwordAnimCon->GetName(), playerSwordAnimCon });

		_resourceUUIDMap.insert({ playerSwordAnimCon->GetUUID(), playerSwordAnimCon.get() });

		DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(playerSwordAnimCon.get(), TEXT("Asset/AnimatorController/Player_SwordAnimatorController.dcontroller"));
#pragma endregion

#pragma region MONSTER_CONTROLLER
		{
			auto monsterAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Monster_AnimatorController"));

			auto mosterStateMachine = monsterAnimCon->AddStateMachine(TEXT("MonsterStateMachine"));

			// Parameter
			monsterAnimCon->AddParameter(TEXT("MoveSpeed"), AnimatorControllerParameterType::Float);
			monsterAnimCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkRight"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkLeft"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkBack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsJump_Backward"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHit_Front"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHit_Back"), AnimatorControllerParameterType::Bool);

			// State & AnimClip
			std::vector<AnimatorState*> allState;

			auto monsterIdle = mosterStateMachine->AddState(TEXT("Idle"));
			monsterIdle->SetAnimationClip(GetAnimationClip(TEXT("idle_normally")));
			allState.push_back(monsterIdle);

			auto monsterWalk = mosterStateMachine->AddState(TEXT("Walk"));
			monsterWalk->SetAnimationClip(GetAnimationClip(TEXT("walk_front")));
			allState.push_back(monsterWalk);

			auto monsterWalk_Right = mosterStateMachine->AddState(TEXT("Walk_Right"));
			monsterWalk_Right->SetAnimationClip(GetAnimationClip(TEXT("walk_right")));
			allState.push_back(monsterWalk_Right);

			auto monsterWalk_Left = mosterStateMachine->AddState(TEXT("Walk_Left"));
			monsterWalk_Left->SetAnimationClip(GetAnimationClip(TEXT("walk_left")));
			allState.push_back(monsterWalk_Left);

			auto monsterWalk_Back = mosterStateMachine->AddState(TEXT("Walk_Back"));
			monsterWalk_Back->SetAnimationClip(GetAnimationClip(TEXT("walk_back")));
			allState.push_back(monsterWalk_Back);

			auto monsterRun = mosterStateMachine->AddState(TEXT("Run"));
			monsterRun->SetAnimationClip(GetAnimationClip(TEXT("run")));
			allState.push_back(monsterRun);

			auto monsterAttack = mosterStateMachine->AddState(TEXT("Attack"));
			monsterAttack->SetAnimationClip(GetAnimationClip(TEXT("attack_close")));
			allState.push_back(monsterAttack);

			auto monsterJump_BackWard = mosterStateMachine->AddState(TEXT("Jump_Backward"));
			GetAnimationClip(TEXT("jump_backward"))->SetIsRootMotion(true);
			GetAnimationClip(TEXT("jump_backward"))->SetRootMotionTargetIndex(1);								// 이 애니메이션은 1번 본이 루트 모션 타겟입니다.
			monsterJump_BackWard->SetAnimationClip(GetAnimationClip(TEXT("jump_backward")));
			allState.push_back(monsterJump_BackWard);

			auto monsterHit_Back = mosterStateMachine->AddState(TEXT("Hit_Back"));
			monsterHit_Back->SetAnimationClip(GetAnimationClip(TEXT("hit_back")));
			allState.push_back(monsterHit_Back);

			auto monsterHit_Front = mosterStateMachine->AddState(TEXT("Hit_Front"));
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

			auto monsterJump_BackWardToIdel = monsterJump_BackWard->AddTransition(monsterIdle);

			auto monsterHit_BackToIdle = monsterHit_Back->AddTransition(monsterIdle);
			auto monsterHit_FrontToIdle = monsterHit_Front->AddTransition(monsterIdle);

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

			_animatorControllerIDMap.insert({ monsterAnimCon->GetName(), monsterAnimCon });

			_resourceUUIDMap.insert({ monsterAnimCon->GetUUID(), monsterAnimCon.get() });

			DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(monsterAnimCon.get(), TEXT("Asset/AnimatorController/Monster_AnimatorController.dcontroller"));
		}
		//monsterIdle->SetAnimationClip(GetAnimationClip(TEXT("attack_close")));
		//monsterIdle->SetAnimationClip(GetAnimationClip(TEXT("attack_far")));

		//monsterIdle->SetAnimationClip(GetAnimationClip(TEXT("idle_angry")));
		//monsterIdle->SetAnimationClip(GetAnimationClip(TEXT("idle_far")));

#pragma endregion
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

	const std::unordered_map<DUOLCommon::tstring, std::shared_ptr<DUOLGameEngine::AnimatorController>>& ResourceManager::GetAllAnimatorControllers() const
	{
		return _animatorControllerIDMap;
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

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const DUOLCommon::tstring& textureID)
	{
		return _graphicsEngine->CreateTexture(textureID);
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const DUOLCommon::tstring& textureID, float width, float height, int size, void* initialData)
	{
		return _graphicsEngine->CreateTexture(textureID, width, height, size, initialData);
	}

	bool ResourceManager::DeleteLight(const uint64_t& lightID) const
	{
		return _graphicsEngine->DeleteLight(lightID);
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