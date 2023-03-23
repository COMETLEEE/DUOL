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

		// Sword_Dash
		auto playerLeftDash = playerStateMachine->AddState(TEXT("LeftDash"));
		auto playerLeftDashClip = GetAnimationClip(TEXT("player_sword_lock_run_left"));
		playerLeftDash->SetAnimationClip(playerLeftDashClip);

		auto playerRightDash = playerStateMachine->AddState(TEXT("RightDash"));
		auto playerRightDashClip = GetAnimationClip(TEXT("player_sword_lock_run_right"));
		playerRightDash->SetAnimationClip(playerRightDashClip);

		auto playerFrontDash = playerStateMachine->AddState(TEXT("FrontDash"));
		auto playerFrontDashClip = GetAnimationClip(TEXT("player_sword_idle"));
		playerFrontDash->SetAnimationClip(playerFrontDashClip);

		auto playerBackDash = playerStateMachine->AddState(TEXT("BackDash"));
		auto playerBackDashClip = GetAnimationClip(TEXT("player_sword_lock_run_back"));
		playerBackDash->SetAnimationClip(playerBackDashClip);

#pragma region DASH_EVENTS
		AnimationEvent startDash;

		startDash._eventName = TEXT("Player_StartDash");
		startDash._targetFrame = 1.f;

		AnimationEvent endDash;

		endDash._eventName = TEXT("Player_EndDash");
		endDash._targetFrame = 10.f;

		playerLeftDashClip->AddEvent(startDash);
		playerRightDashClip->AddEvent(startDash);
		playerFrontDashClip->AddEvent(startDash);
		playerBackDashClip->AddEvent(startDash);

		playerLeftDashClip->AddEvent(endDash);
		playerRightDashClip->AddEvent(endDash);
		playerFrontDashClip->AddEvent(endDash);
		playerBackDashClip->AddEvent(endDash);
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
		playerLockOnFrontRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_front")));

		auto playerLockOnBackRun = playerStateMachine->AddState(TEXT("LockOnBackRun"));
		playerLockOnBackRun->SetAnimationClip(GetAnimationClip(TEXT("player_sword_lock_run_back")));

		// Sword_Die
		auto playerDie = playerStateMachine->AddState(TEXT("Die"));
		playerDie->SetAnimationClip(GetAnimationClip(TEXT("player_sword_die")));



		// TODO : Transition
		auto playerIdleToRun = playerIdle->AddTransition(playerMove);

		playerIdleToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		playerIdleToRun->SetTransitionDuration(0.01f);
		playerIdleToRun->SetTransitionOffset(0.f);

		auto playerRunToIdle = playerMove->AddTransition(playerIdle);

		playerRunToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		playerRunToIdle->SetTransitionDuration(0.01f);
		playerRunToIdle->SetTransitionOffset(0.f);

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

		auto playerMoveToLeftDash = playerMove->AddTransition(playerLeftDash);
		playerMoveToLeftDash->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerMoveToLeftDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
		playerMoveToLeftDash->AddCondition(TEXT("IsLeft"), AnimatorConditionMode::True);

		playerMoveToLeftDash->SetTransitionDuration(0.01f);
		playerMoveToLeftDash->SetTransitionOffset(0.01f);

		auto playerMoveToRightDash = playerMove->AddTransition(playerRightDash);
		playerMoveToRightDash->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerMoveToRightDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
		playerMoveToRightDash->AddCondition(TEXT("IsRight"), AnimatorConditionMode::True);

		playerMoveToRightDash->SetTransitionDuration(0.01f);
		playerMoveToRightDash->SetTransitionOffset(0.01f);

		auto playerMoveToFrontDash = playerMove->AddTransition(playerFrontDash);
		playerMoveToFrontDash->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerMoveToFrontDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
		playerMoveToFrontDash->AddCondition(TEXT("IsFront"), AnimatorConditionMode::True);

		playerMoveToFrontDash->SetTransitionDuration(0.01f);
		playerMoveToFrontDash->SetTransitionOffset(0.01f);

		auto playerMoveToBackDash = playerMove->AddTransition(playerBackDash);
		playerMoveToBackDash->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerMoveToBackDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
		playerMoveToBackDash->AddCondition(TEXT("IsBack"), AnimatorConditionMode::True);

		playerMoveToBackDash->SetTransitionDuration(0.01f);
		playerMoveToBackDash->SetTransitionOffset(0.01f);


		auto playerLeftDashToIdle = playerMove->AddTransition(playerIdle);
		playerLeftDashToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerLeftDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		playerLeftDashToIdle->SetTransitionDuration(0.01f);
		playerLeftDashToIdle->SetTransitionOffset(0.01f);

		auto playerRightDashToIdle = playerMove->AddTransition(playerIdle);
		playerRightDashToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerRightDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		playerRightDashToIdle->SetTransitionDuration(0.01f);
		playerRightDashToIdle->SetTransitionOffset(0.01f);

		auto playerFrontDashToIdle = playerMove->AddTransition(playerIdle);
		playerFrontDashToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerFrontDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		playerFrontDashToIdle->SetTransitionDuration(0.01f);
		playerFrontDashToIdle->SetTransitionOffset(0.01f);

		auto playerBackDashToIdle = playerMove->AddTransition(playerIdle);
		playerBackDashToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerBackDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		playerBackDashToIdle->SetTransitionDuration(0.01f);
		playerBackDashToIdle->SetTransitionOffset(0.01f);


		auto playerMoveToLockOnFront = playerMove->AddTransition(playerLockOnFrontMove);
		auto playerMoveToLockOnBack = playerMove->AddTransition(playerLockOnBackMove);
		auto playerMoveToLockOnLeft = playerMove->AddTransition(playerLockOnLeftMove);
		auto playerMoveToLockOnRight = playerMove->AddTransition(playerLockOnRightMove);

		auto playerLockOnFrontToMove = playerLockOnFrontMove->AddTransition(playerMove);
		auto playerLockOnBackToMove = playerLockOnBackMove->AddTransition(playerMove);
		auto playerLockOnLeftToMove = playerLockOnLeftMove->AddTransition(playerMove);
		auto playerLockOnRightToMove = playerLockOnRightMove->AddTransition(playerMove);

		auto playerRunToLockOnFrontRun = playerRun->AddTransition(playerLockOnFrontRun);
		auto playerRunToLockOnBackRun = playerRun->AddTransition(playerLockOnBackRun);
		auto playerRunToLockOnLeftRun = playerRun->AddTransition(playerLockOnLeftRun);
		auto playerRunToLockOnRightRun = playerRun->AddTransition(playerLockOnRightRun);

		auto playerLockOnFrontRunToRun = playerLockOnFrontRun->AddTransition(playerRun);
		auto playerLockOnBackRunToRun = playerLockOnBackRun->AddTransition(playerRun);
		auto playerLockOnLeftRunToRun = playerLockOnLeftRun->AddTransition(playerRun);
		auto playerLockOnRightRunToRun = playerLockOnRightRun->AddTransition(playerRun);

		auto playerLockOnFrontToLockOnFrontRun = playerLockOnFrontMove->AddTransition(playerLockOnFrontRun);
		auto playerLockOnBackToLockOnBackRun = playerLockOnBackMove->AddTransition(playerLockOnBackRun);



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