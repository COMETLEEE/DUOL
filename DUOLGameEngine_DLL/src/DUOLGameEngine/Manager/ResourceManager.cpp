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
#include "DUOLCommon/Util/Hash.h"

namespace DUOLGameEngine
{
	ResourceManager::ResourceManager() :
		_isThread(false)
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
		std::vector<std::string> meshnames;

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("Asset/BinaryData/Mesh"))
		{
			// .DUOL
			std::string meshNamePath = entry.path().filename().generic_string();

			size_t lastDotPos = meshNamePath.find_last_of(".");
			std::string meshName = meshNamePath.substr(0, lastDotPos);

			//std::string meshPath = entry.path().generic_string();

			meshnames.emplace_back(meshName);
		}

		//_graphicsEngine->SetDataName(_materialNameList, _animationNameList);
		for (int modelCount = 0; modelCount < meshnames.size(); modelCount++)
		{
			DUOLGraphicsEngine::Model* pModel = _graphicsEngine->CreateModelFromFBXWithMultiThread(DUOLCommon::StringHelper::ToTString(meshnames[modelCount]));

			_modelIDMap.insert({ DUOLCommon::StringHelper::ToTString(meshnames[modelCount]), pModel });

#pragma region MESH
			unsigned meshCount = pModel->GetMeshCount();

			for (unsigned i = 0; i < meshCount; i++)
			{
				DUOLGraphicsEngine::MeshBase* pMesh = pModel->GetMesh(i);

				std::shared_ptr<DUOLGameEngine::Mesh> engineMesh = std::make_shared<DUOLGameEngine::Mesh>(pMesh->_meshName);

				engineMesh->SetPrimitiveMesh(pMesh);

				_meshIDMap.insert({ pMesh->_meshName, engineMesh });

				_resourceUUIDMap.insert({ engineMesh->GetUUID(), engineMesh.get() });
			}
#pragma endregion

#pragma region AVATAR
			if (pModel->IsSkinningModel())
			{
				std::shared_ptr<DUOLGameEngine::Avatar> avatar = std::make_shared<Avatar>(DUOLCommon::StringHelper::ToTString(meshnames[modelCount]));

				avatar->SetPrimitiveBones(&pModel->GetBones());

				_avatarIDMap.insert({ DUOLCommon::StringHelper::ToTString(meshnames[modelCount]), avatar });

				_resourceUUIDMap.insert({ avatar->GetUUID(), avatar.get() });
			}
#pragma endregion
		}

		_graphicsEngine->LoadTexturesWithMultiThread();
	}

	//void ResourceManager::SetUseData(uint64 meshid, std::pair<std::vector<uint64>, std::vector<uint64>>& modeldata)
	//{
	//	for (auto& perfab : _perfabsIDList)
	//	{
	//		if (perfab.first == meshid)
	//		{
	//			modeldata = perfab.second;

	//			break;
	//		}
	//	}
	//}

	void ResourceManager::LoadAnimatorController_CloseEnemy()
	{
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
			GetAnimationClip(TEXT("attack_close"))->SetIsLoop(false);
			GetAnimationClip(TEXT("attack_close"))->SetIsUseEventInTransition(false);
			allState.push_back(monsterAttack);

			auto monsterJump_BackWard = monsterStateMachine->AddState(TEXT("Jump_Backward"));
			GetAnimationClip(TEXT("jump_backward"))->SetIsRootMotion(true);
			GetAnimationClip(TEXT("jump_backward"))->SetRootMotionTargetIndex(1);								// �� �ִϸ��̼��� 1�� ���� ��Ʈ ��� Ÿ���Դϴ�.
			monsterJump_BackWard->SetAnimationClip(GetAnimationClip(TEXT("jump_backward")));
			allState.push_back(monsterJump_BackWard);

			auto monsterWakeUp = monsterStateMachine->AddState(TEXT("WakeUp"));
			monsterWakeUp->SetAnimationClip(GetAnimationClip(TEXT("get_ up"))); // ���� �׽�Ʈ ������ �־�״�..! ���߿� ��������.
			allState.push_back(monsterWakeUp);

			auto monsterHit_Back = monsterStateMachine->AddState(TEXT("Hit_Back"));
			monsterHit_Back->SetAnimationClip(GetAnimationClip(TEXT("hit_back")));
			GetAnimationClip(TEXT("hit_back"))->SetIsLoop(false);
			GetAnimationClip(TEXT("hit_back"))->SetIsUseEventInTransition(false);
			allState.push_back(monsterHit_Back);

			auto monsterHit_Front = monsterStateMachine->AddState(TEXT("Hit_Front"));
			monsterHit_Front->SetAnimationClip(GetAnimationClip(TEXT("hit_front")));
			GetAnimationClip(TEXT("hit_front"))->SetIsLoop(false);
			GetAnimationClip(TEXT("hit_front"))->SetIsUseEventInTransition(false);
			allState.push_back(monsterHit_Front);

			auto monsterDie_Ground = monsterStateMachine->AddState(TEXT("Die_Ground"));
			monsterDie_Ground->SetAnimationClip(GetAnimationClip(TEXT("die_ground")));

			auto monsterDie_Air = monsterStateMachine->AddState(TEXT("Die_Air"));
			monsterDie_Air->SetAnimationClip(GetAnimationClip(TEXT("die_air")));

			auto monsterHit_BackToDieGround = monsterHit_Back->AddTransition(monsterDie_Ground);
			auto monsterHit_BackToDieAir = monsterHit_Back->AddTransition(monsterDie_Air);

			auto monsterHit_FrontToDieGround = monsterHit_Front->AddTransition(monsterDie_Ground);
			auto monsterHit_FrontToDieAir = monsterHit_Front->AddTransition(monsterDie_Air);

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

			// RandOffset

			monsterIdle->SetOffsetParameter(TEXT("RandOffset"));
			monsterIdle->SetOffsetParameterActive(true);

			monsterHit_Front->SetOffsetParameter(TEXT("RandOffset"));
			monsterHit_Front->SetOffsetParameterActive(true);

			monsterHit_Back->SetOffsetParameter(TEXT("RandOffset"));
			monsterHit_Back->SetOffsetParameterActive(true);

			// Transition // Ʈ�������� �켱������ ���� ����Ѽ��̴�.
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

			auto monsterAttackToJump_BackWard = monsterAttack->AddTransition(monsterJump_BackWard);
			auto monsterAttackToIdle = monsterAttack->AddTransition(monsterIdle);

			auto monsterJump_BackWardToIdel = monsterJump_BackWard->AddTransition(monsterIdle);

			auto monsterHit_BackToIdle = monsterHit_Back->AddTransition(monsterIdle);

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

			monsterAttackToIdle->SetTransitionDuration(0.1f);
			monsterAttackToIdle->SetTransitionOffset(0.f);
			monsterAttackToJump_BackWard->AddCondition(TEXT("IsJump_Backward"), AnimatorConditionMode::True);
			monsterAttackToJump_BackWard->SetTransitionDuration(0.01f);
			monsterAttackToJump_BackWard->SetTransitionOffset(0.0f);

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

			attackEvent._eventName = TEXT("SetBool_IsAttack_False");
			attackEvent._targetFrame = 1.0f;

			attackClip->AddEvent(attackEvent);

			attackEvent._eventName = TEXT("Attack_Close");
			attackEvent._targetFrame = 49.0f;

			attackClip->AddEvent(attackEvent);

			attackEvent._eventName = TEXT("PlaySound_CloseAttack_True_0");
			attackEvent._targetFrame = 1.0f;

			attackClip->AddEvent(attackEvent);

			attackEvent._eventName = TEXT("PlayVoiceSound_Monster03_True_0");
			attackEvent._targetFrame = 1.0f;

			attackClip->AddEvent(attackEvent);

			auto dieGroundClip = GetAnimationClip(TEXT("die_ground"));
			AnimationEvent dieGroundEvent;

			dieGroundEvent._eventName = TEXT("StopAnimator");
			dieGroundEvent._targetFrame = 42.0f;

			dieGroundClip->AddEvent(dieGroundEvent);

			auto dieAirClip = GetAnimationClip(TEXT("die_air"));
			AnimationEvent dieAirEvent;

			dieAirEvent._eventName = TEXT("StopAnimator");
			dieAirEvent._targetFrame = 27.0f;

			dieAirClip->AddEvent(dieAirEvent);

			AnimationEvent wakeUpEndEvent;

			wakeUpEndEvent._eventName = TEXT("SetBool_IsWakeUpToIdle_True");
			wakeUpEndEvent._targetFrame = 46.0f;

			auto getUpClip = GetAnimationClip(TEXT("get_ up"));
			getUpClip->AddEvent(wakeUpEndEvent);

			// ------------------------------ Event Registe ---------------------------

			_animatorControllerIDMap.insert({ monsterAnimCon->GetName(), monsterAnimCon });

			_resourceUUIDMap.insert({ monsterAnimCon->GetUUID(), monsterAnimCon.get() });

			DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(monsterAnimCon.get(), TEXT("Asset/AnimatorController/Monster_AnimatorController.dcontroller"));
		}
	}

	void ResourceManager::LoadAnimatorController_FarEnemy()
	{
		///  -------------------------------------------------------------------------------------- ���Ÿ� ����
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
			GetAnimationClip(TEXT("attack_far"))->SetIsLoop(false);
			GetAnimationClip(TEXT("attack_far"))->SetIsUseEventInTransition(false);
			allState.push_back(monsterAttack);

			auto monsterJump_BackWard = monsterStateMachine->AddState(TEXT("Jump_Backward"));
			GetAnimationClip(TEXT("jump_backward"))->SetIsRootMotion(true);
			GetAnimationClip(TEXT("jump_backward"))->SetRootMotionTargetIndex(1);								// �� �ִϸ��̼��� 1�� ���� ��Ʈ ��� Ÿ���Դϴ�.
			monsterJump_BackWard->SetAnimationClip(GetAnimationClip(TEXT("jump_backward")));
			allState.push_back(monsterJump_BackWard);

			auto monsterWakeUp = monsterStateMachine->AddState(TEXT("WakeUp"));
			monsterWakeUp->SetAnimationClip(GetAnimationClip(TEXT("get_ up"))); // ���� �׽�Ʈ ������ �־�״�..! ���߿� ��������.
			allState.push_back(monsterWakeUp);

			auto monsterHit_Back = monsterStateMachine->AddState(TEXT("Hit_Back"));
			monsterHit_Back->SetAnimationClip(GetAnimationClip(TEXT("hit_back")));
			GetAnimationClip(TEXT("hit_back"))->SetIsLoop(false);
			GetAnimationClip(TEXT("hit_back"))->SetIsUseEventInTransition(false);
			allState.push_back(monsterHit_Back);

			auto monsterHit_Front = monsterStateMachine->AddState(TEXT("Hit_Front"));
			monsterHit_Front->SetAnimationClip(GetAnimationClip(TEXT("hit_front")));
			GetAnimationClip(TEXT("hit_front"))->SetIsLoop(false);
			GetAnimationClip(TEXT("hit_front"))->SetIsUseEventInTransition(false);
			allState.push_back(monsterHit_Front);

			auto monsterDie_Ground = monsterStateMachine->AddState(TEXT("Die_Ground"));
			GetAnimationClip(TEXT("die_ground"))->SetIsLoop(false);
			GetAnimationClip(TEXT("die_ground"))->SetIsUseEventInTransition(false);
			monsterDie_Ground->SetAnimationClip(GetAnimationClip(TEXT("die_ground")));

			auto monsterDie_Air = monsterStateMachine->AddState(TEXT("Die_Air"));
			GetAnimationClip(TEXT("die_air"))->SetIsLoop(false);
			GetAnimationClip(TEXT("die_air"))->SetIsUseEventInTransition(false);
			monsterDie_Air->SetAnimationClip(GetAnimationClip(TEXT("die_air")));

			auto monsterHit_BackToDieGround = monsterHit_Back->AddTransition(monsterDie_Ground);
			auto monsterHit_BackToDieAir = monsterHit_Back->AddTransition(monsterDie_Air);

			auto monsterHit_FrontToDieGround = monsterHit_Front->AddTransition(monsterDie_Ground);
			auto monsterHit_FrontToDieAir = monsterHit_Front->AddTransition(monsterDie_Air);

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


			// RandOffset

			monsterIdle->SetOffsetParameter(TEXT("RandOffset"));
			monsterIdle->SetOffsetParameterActive(true);

			monsterHit_Front->SetOffsetParameter(TEXT("RandOffset"));
			monsterHit_Front->SetOffsetParameterActive(true);

			monsterHit_Back->SetOffsetParameter(TEXT("RandOffset"));
			monsterHit_Back->SetOffsetParameterActive(true);

			// Transition // Ʈ�������� �켱������ ���� ����Ѽ��̴�.
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

			auto monsterHit_BackToIdle = monsterHit_Back->AddTransition(monsterIdle);

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

			attackEvent._eventName = TEXT("PlaySound_Simple_Shot_True_0");
			attackEvent._targetFrame = 1.0f;

			attackClip->AddEvent(attackEvent);

			attackEvent._eventName = TEXT("PlayVoiceSound_Monster03_True_0");
			attackEvent._targetFrame = 1.0f;

			attackClip->AddEvent(attackEvent);

			auto dieGroundClip = GetAnimationClip(TEXT("die_ground"));
			AnimationEvent dieGroundEvent;

			dieGroundEvent._eventName = TEXT("StopAnimator");
			dieGroundEvent._targetFrame = 43.0f;

			dieGroundClip->AddEvent(dieGroundEvent);

			auto dieAirClip = GetAnimationClip(TEXT("die_air"));
			AnimationEvent dieAirEvent;

			dieAirEvent._eventName = TEXT("StopAnimator");
			dieAirEvent._targetFrame = 29.0f;

			dieAirClip->AddEvent(dieAirEvent);

			AnimationEvent wakeUpEndEvent;

			wakeUpEndEvent._eventName = TEXT("SetBool_IsWakeUpToIdle_True");
			wakeUpEndEvent._targetFrame = 46.0f;

			auto getUpClip = GetAnimationClip(TEXT("get_ up"));
			getUpClip->AddEvent(wakeUpEndEvent);

			// ------------------------------ CommonSound Registe ---------------------------

			auto hitBackClip = GetAnimationClip(TEXT("hit_back"));
			auto hitFrontClip = GetAnimationClip(TEXT("hit_front"));

			AnimationEvent animEvent;

			animEvent._eventName = TEXT("PlayVoiceSound_Monster05_True_1");
			animEvent._targetFrame = 1.0f;

			hitBackClip->AddEvent(attackEvent);
			hitFrontClip->AddEvent(attackEvent);

			animEvent._eventName = TEXT("PlayVoiceSound_Monster07_True_1");
			animEvent._targetFrame = 1.0f;

			dieAirClip->AddEvent(animEvent);
			// ------------------------------ Event Registe ---------------------------

			_animatorControllerIDMap.insert({ monsterAnimCon->GetName(), monsterAnimCon });

			_resourceUUIDMap.insert({ monsterAnimCon->GetUUID(), monsterAnimCon.get() });

			DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(monsterAnimCon.get(), TEXT("Asset/AnimatorController/Monster_AnimatorController_far.dcontroller"));
		}
	}

	void ResourceManager::LoadAnimatorController_EliteEnemy()
	{
		///  ---------------------------------------------------------- EliteEnemy
		{
			auto monsterAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Monster_AnimatorController_Elite"));

			auto monsterStateMachine = monsterAnimCon->AddStateMachine(TEXT("MonsterStateMachine"));

			const wchar_t* jumpAttack_str = TEXT("elite_attack_jump");
			const wchar_t* attack_str = TEXT("elite_attack_normal");
			const wchar_t* heavyAttack_str = TEXT("elite_attack_strong");
			const wchar_t* rush_str = TEXT("elite_rush");
			const wchar_t* standingCry_str = TEXT("elite_roar");
			const wchar_t* seriousPunch_str = TEXT("elite_attack_charge");
			const wchar_t* walk_str = TEXT("elite_walk_front");

			const wchar_t* run_str = TEXT("elite_run_front");
			const wchar_t* walkRight_str = TEXT("elite_walk_right");
			const wchar_t* walkLeft_str = TEXT("elite_walk_left");
			const wchar_t* walkBack_str = TEXT("elite_walk_back");


			const wchar_t* die_str = TEXT("elite_die");
			const wchar_t* hit_str = TEXT("elite_hit_back");
			const wchar_t* idle_str = TEXT("elite_idle");

			const wchar_t* groggy_str = TEXT("elite_groggy");

			// Parameter
			monsterAnimCon->AddParameter(TEXT("IsRush"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsGroggy"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsJumpAttack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHeavyAttack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsSeriousAttack"), AnimatorControllerParameterType::Bool);

			monsterAnimCon->AddParameter(TEXT("IsHit_Front"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHit_Back"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("MoveSpeed"), AnimatorControllerParameterType::Float);

			monsterAnimCon->AddParameter(TEXT("IsWalkRight"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkLeft"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkBack"), AnimatorControllerParameterType::Bool);

			// State & AnimClip
			std::vector<AnimatorState*> allState;

			auto monsterIdle = monsterStateMachine->AddState(TEXT("Idle"));
			monsterIdle->SetAnimationClip(GetAnimationClip(idle_str));
			allState.push_back(monsterIdle);

			auto monsterWalk = monsterStateMachine->AddState(TEXT("Walk"));
			monsterWalk->SetAnimationClip(GetAnimationClip(walk_str));
			allState.push_back(monsterWalk);

			auto monsterWalk_Right = monsterStateMachine->AddState(TEXT("Walk_Right"));
			monsterWalk_Right->SetAnimationClip(GetAnimationClip(walkRight_str));
			allState.push_back(monsterWalk_Right);

			auto monsterWalk_Left = monsterStateMachine->AddState(TEXT("Walk_Left"));
			monsterWalk_Left->SetAnimationClip(GetAnimationClip(walkLeft_str));
			allState.push_back(monsterWalk_Left);

			auto monsterWalk_Back = monsterStateMachine->AddState(TEXT("Walk_Back"));
			monsterWalk_Back->SetAnimationClip(GetAnimationClip(walkBack_str));
			allState.push_back(monsterWalk_Back);

			auto monsterRun = monsterStateMachine->AddState(TEXT("Run"));
			monsterRun->SetAnimationClip(GetAnimationClip(run_str));
			allState.push_back(monsterRun);

			auto monsterAttack_Normal = monsterStateMachine->AddState(TEXT("Attack"));
			monsterAttack_Normal->SetAnimationClip(GetAnimationClip(attack_str));
			allState.push_back(monsterAttack_Normal);

			auto monsterAttack_HeavyAttack = monsterStateMachine->AddState(TEXT("HeavyAttack"));
			GetAnimationClip(heavyAttack_str)->SetIsRootMotion(true);
			GetAnimationClip(heavyAttack_str)->SetRootMotionTargetIndex(0);
			monsterAttack_HeavyAttack->SetAnimationClip(GetAnimationClip(heavyAttack_str));
			allState.push_back(monsterAttack_HeavyAttack);

			auto monsterHit_Front = monsterStateMachine->AddState(TEXT("Hit_Front"));
			monsterHit_Front->SetAnimationClip(GetAnimationClip(hit_str));
			allState.push_back(monsterHit_Front);

			auto monsterHit_Back = monsterStateMachine->AddState(TEXT("Hit_Back"));
			monsterHit_Back->SetAnimationClip(GetAnimationClip(hit_str));
			allState.push_back(monsterHit_Back);

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

			auto monsterAttack_JumpAttack = monsterStateMachine->AddState(TEXT("JumpAttack"));
			monsterAttack_JumpAttack->SetAnimationClip(GetAnimationClip(jumpAttack_str));
			allState.push_back(monsterAttack_JumpAttack);

			auto monsterAttack_StandingCry = monsterStateMachine->AddState(TEXT("StandingCry"));
			monsterAttack_StandingCry->SetAnimationClip(GetAnimationClip(standingCry_str));
			allState.push_back(monsterAttack_StandingCry);

			auto monsterAttack_Rush = monsterStateMachine->AddState(TEXT("Rush"));
			monsterAttack_Rush->SetAnimationClip(GetAnimationClip(rush_str));
			allState.push_back(monsterAttack_Rush);

			auto monsterAttack_SeriousPunch = monsterStateMachine->AddState(TEXT("SeriousPunch"));
			monsterAttack_SeriousPunch->SetAnimationClip(GetAnimationClip(seriousPunch_str));
			allState.push_back(monsterAttack_SeriousPunch);

			auto monsterDie = monsterStateMachine->AddState(TEXT("Die"));
			GetAnimationClip(die_str)->SetIsRootMotion(true);
			GetAnimationClip(die_str)->SetRootMotionTargetIndex(0);
			monsterDie->SetAnimationClip(GetAnimationClip(die_str));

			auto monsterGroggy = monsterStateMachine->AddState(TEXT("Groggy"));
			monsterGroggy->SetAnimationClip(GetAnimationClip(groggy_str));
			allState.push_back(monsterGroggy);

			for (auto& iter : allState)
			{
				auto transition = iter->AddTransition(monsterDie);
				transition->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
				transition->SetTransitionDuration(0.01f);
				transition->SetTransitionOffset(0.f);
			}



			// Transition // Ʈ�������� �켱������ ���� ����Ѽ��̴�.

			auto monsterIdleToWalk_Right = monsterIdle->AddTransition(monsterWalk_Right);
			auto monsterIdleToWalk_Left = monsterIdle->AddTransition(monsterWalk_Left);
			auto monsterIdleToWalk_Back = monsterIdle->AddTransition(monsterWalk_Back);
			auto monsterIdleToWalk = monsterIdle->AddTransition(monsterWalk);
			auto monsterIdleToGroggy = monsterIdle->AddTransition(monsterGroggy);

			auto monsterGroogyToIdle = monsterGroggy->AddTransition(monsterIdle);

			auto monsterWalkToIdle = monsterWalk->AddTransition(monsterIdle);
			auto monsterWalkToRun = monsterWalk->AddTransition(monsterRun);

			auto monsterWalk_RightToIdle = monsterWalk_Right->AddTransition(monsterIdle);
			auto monsterWalk_LeftToIdle = monsterWalk_Left->AddTransition(monsterIdle);
			auto monsterWalk_BackToIdle = monsterWalk_Back->AddTransition(monsterIdle);

			auto monsterIdleToAttack = monsterIdle->AddTransition(monsterAttack_Normal);
			auto monsterIdleToHeavyAttack = monsterIdle->AddTransition(monsterAttack_HeavyAttack);
			auto monsterIdleToJumpAttack = monsterIdle->AddTransition(monsterAttack_JumpAttack);
			auto monsterIdleToStanding = monsterIdle->AddTransition(monsterAttack_StandingCry);
			auto monsterStandingCryToRushAttack = monsterAttack_StandingCry->AddTransition(monsterAttack_Rush);
			auto monsterIdleToSeriousAttack = monsterIdle->AddTransition(monsterAttack_SeriousPunch);

			auto monsterAttackToIdle = monsterAttack_Normal->AddTransition(monsterIdle);
			auto monsterHeavyAttackToIdle = monsterAttack_HeavyAttack->AddTransition(monsterIdle);
			auto monsterJumpAttackToIdle = monsterAttack_JumpAttack->AddTransition(monsterIdle);
			auto monsterRushAttackToIdle = monsterAttack_Rush->AddTransition(monsterIdle);
			auto monsterSeriousAttackToIdle = monsterAttack_SeriousPunch->AddTransition(monsterIdle);
			auto monsterHitBackToIdle = monsterHit_Back->AddTransition(monsterIdle);
			auto monsterHitFrontToIdle = monsterHit_Front->AddTransition(monsterIdle);


			monsterIdleToGroggy->AddCondition(TEXT("IsGroggy"), AnimatorConditionMode::True);
			monsterIdleToGroggy->SetTransitionDuration(0.1f);
			monsterIdleToGroggy->SetTransitionOffset(0.f);

			monsterGroogyToIdle->AddCondition(TEXT("IsGroggy"), AnimatorConditionMode::False);
			monsterGroogyToIdle->SetTransitionDuration(0.2f);
			monsterGroogyToIdle->SetTransitionOffset(0.f);

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

			monsterStandingCryToRushAttack->SetTransitionDuration(0.01f);
			monsterStandingCryToRushAttack->SetTransitionDuration(0.f);

			monsterHitBackToIdle->SetTransitionDuration(0.01f);
			monsterHitBackToIdle->SetTransitionOffset(0.f);

			monsterHitFrontToIdle->SetTransitionDuration(0.01f);
			monsterHitFrontToIdle->SetTransitionOffset(0.f);

			monsterIdleToAttack->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
			monsterIdleToAttack->SetTransitionDuration(0.01f);
			monsterIdleToAttack->SetTransitionOffset(0.f);

			monsterIdleToHeavyAttack->AddCondition(TEXT("IsHeavyAttack"), AnimatorConditionMode::True);
			monsterIdleToHeavyAttack->SetTransitionDuration(0.01f);
			monsterIdleToHeavyAttack->SetTransitionOffset(0.f);

			monsterIdleToJumpAttack->AddCondition(TEXT("IsJumpAttack"), AnimatorConditionMode::True);
			monsterIdleToJumpAttack->SetTransitionDuration(0.01f);
			monsterIdleToJumpAttack->SetTransitionOffset(0.f);

			monsterIdleToStanding->AddCondition(TEXT("IsRush"), AnimatorConditionMode::True);
			monsterIdleToStanding->SetTransitionDuration(0.01f);
			monsterIdleToStanding->SetTransitionOffset(0.f);

			monsterIdleToSeriousAttack->AddCondition(TEXT("IsSeriousAttack"), AnimatorConditionMode::True);
			monsterIdleToSeriousAttack->SetTransitionDuration(0.01f);
			monsterIdleToSeriousAttack->SetTransitionOffset(0.f);

			monsterAttackToIdle->SetTransitionDuration(0.01f);
			monsterAttackToIdle->SetTransitionOffset(0.f);

			monsterHeavyAttackToIdle->AddCondition(TEXT("IsHeavyAttack"), AnimatorConditionMode::False);
			monsterHeavyAttackToIdle->SetTransitionDuration(0.01f);
			monsterHeavyAttackToIdle->SetTransitionOffset(0.f);

			monsterJumpAttackToIdle->SetTransitionDuration(0.01f);
			monsterJumpAttackToIdle->SetTransitionOffset(0.f);

			monsterRushAttackToIdle->AddCondition(TEXT("IsRush"), AnimatorConditionMode::False);
			monsterRushAttackToIdle->SetTransitionDuration(0.01f);
			monsterRushAttackToIdle->SetTransitionOffset(0.f);

			monsterSeriousAttackToIdle->SetTransitionDuration(0.01f);
			monsterSeriousAttackToIdle->SetTransitionOffset(0.f);

			// ------------------------------ Event Registe ---------------------------
			AnimationEvent animationEvent;

			auto idleClip = GetAnimationClip(idle_str);

			animationEvent._eventName = TEXT("LerpLookTarget");
			for (int i = 0; i < 122; i++)
			{
				animationEvent._targetFrame = static_cast<float>(i);
				idleClip->AddEvent(animationEvent);
			}

			auto attackClip_Smash = GetAnimationClip(attack_str);

			animationEvent._eventName = TEXT("SetBool_IsAttack_False");
			animationEvent._targetFrame = 1.0f;
			attackClip_Smash->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("SmashAttack");
			animationEvent._targetFrame = 80.0f;
			attackClip_Smash->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("GroggyOff_OnTimer");
			animationEvent._targetFrame = 0.0f;
			GetAnimationClip(groggy_str)->AddEvent(animationEvent);

			auto attackClip_Combo = GetAnimationClip(heavyAttack_str);

			animationEvent._eventName = TEXT("SetBool_IsHeavyAttack_False");
			animationEvent._targetFrame = 208.0f;
			attackClip_Combo->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("ComboAttack1");
			animationEvent._targetFrame = 43.0f;
			attackClip_Combo->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("ComboAttack2");
			animationEvent._targetFrame = 93.0f;
			attackClip_Combo->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("ComboAttack3");
			animationEvent._targetFrame = 141.0f;
			attackClip_Combo->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("SetNavOffRigidbodyOn");
			animationEvent._targetFrame = 1.0f;
			attackClip_Combo->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("SetNavOnRigidbodyOff");
			animationEvent._targetFrame = 205.0f;
			attackClip_Combo->AddEvent(animationEvent);

			auto dieClip = GetAnimationClip(die_str);

			animationEvent._eventName = TEXT("SetNavOffRigidbodyOn");
			animationEvent._targetFrame = 1.0f;
			dieClip->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("StopAnimator");
			animationEvent._targetFrame = 100.0f;
			dieClip->AddEvent(animationEvent);



			animationEvent._eventName = TEXT("SetNavOnRigidbodyOff");
			animationEvent._targetFrame = 100.0f;
			dieClip->AddEvent(animationEvent);

			auto JumpAttackClip = GetAnimationClip(jumpAttack_str);

			animationEvent._eventName = TEXT("SetBool_IsJumpAttack_False");
			animationEvent._targetFrame = 1.0f;
			JumpAttackClip->AddEvent(animationEvent);

			for (int i = 0; i < 35; i++)
			{
				animationEvent._eventName = TEXT("LerpLookTarget");
				animationEvent._targetFrame = static_cast<float>(i);
				JumpAttackClip->AddEvent(animationEvent);
			}

			animationEvent._eventName = TEXT("JumpAttackStart");
			animationEvent._targetFrame = 35.0f;
			JumpAttackClip->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("JumpAttackEnd");
			animationEvent._targetFrame = 107.0f;
			JumpAttackClip->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("SuperArmorOff_OnTimer");
			animationEvent._targetFrame = 107.0f;
			JumpAttackClip->AddEvent(animationEvent);

			auto rushClip = GetAnimationClip(rush_str);

			animationEvent._eventName = TEXT("RushParticlePlay");
			animationEvent._targetFrame = 11.0f;
			rushClip->AddEvent(animationEvent);
			animationEvent._targetFrame = 27.0f;
			rushClip->AddEvent(animationEvent);

			for (int i = 0; i < 33; i++)
			{
				animationEvent._eventName = TEXT("RushAndHit");
				animationEvent._targetFrame = static_cast<float>(i);
				rushClip->AddEvent(animationEvent);
			}

			auto seriousPunchClip = GetAnimationClip(seriousPunch_str);

			animationEvent._eventName = TEXT("SetBool_IsSeriousPunch_False");
			animationEvent._targetFrame = 1.0f;
			seriousPunchClip->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("SeriousPunch");

			animationEvent._targetFrame = 74.0f;

			seriousPunchClip->AddEvent(animationEvent);

			animationEvent._eventName = TEXT("SuperArmorOff_OnTimer");
			animationEvent._targetFrame = 125.0f;
			seriousPunchClip->AddEvent(animationEvent);

			for (int i = 0; i < 70; i++)
			{
				animationEvent._eventName = TEXT("LerpLookTarget");
				animationEvent._targetFrame = static_cast<float>(i);
				seriousPunchClip->AddEvent(animationEvent);
			}


			auto standingCry = GetAnimationClip(standingCry_str);

			for (int i = 0; i < 50; i++)
			{
				animationEvent._eventName = TEXT("LerpLookTarget");
				animationEvent._targetFrame = static_cast<float>(120 + i);
				standingCry->AddEvent(animationEvent);
			}

			animationEvent._eventName = TEXT("StandingCryParticle");
			animationEvent._targetFrame = static_cast<float>(33);
			standingCry->AddEvent(animationEvent);
			// ------------------------------ Event Registe ---------------------------

			AnimationEvent soundEvent;

			soundEvent._eventName = TEXT("PlaySound_EliteCloseAttack_True_0");
			soundEvent._targetFrame = 1.0f;

			GetAnimationClip(attack_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlayVoiceSound_EliteMonsterHit_True_0");
			soundEvent._targetFrame = 1.0f;

			GetAnimationClip(hit_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlayVoiceSound_EliteMonster_Die_True_0");
			soundEvent._targetFrame = 1.0f;

			GetAnimationClip(die_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlayVoiceSound_MiddleBossRoar_True_0");
			soundEvent._targetFrame = 30.0f;

			GetAnimationClip(standingCry_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlaySound_MiddleBossFootStep01_True_0");
			soundEvent._targetFrame = 10.0f;

			GetAnimationClip(rush_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlaySound_MiddleBossFootStep02_True_0");
			soundEvent._targetFrame = 26.0f;

			GetAnimationClip(rush_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlaySound_MiddleBossPunchCharge_True_0");
			soundEvent._targetFrame = 1.0f;

			GetAnimationClip(seriousPunch_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlaySound_MiddleBossPunch_True_0");
			soundEvent._targetFrame = 76.0f;

			GetAnimationClip(seriousPunch_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlayVoiceSound_EliteMonsterJump_True_0");
			soundEvent._targetFrame = 1.0f;

			GetAnimationClip(jumpAttack_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlaySound_MidBossCrack_True_0");
			soundEvent._targetFrame = 106.0f;

			GetAnimationClip(jumpAttack_str)->AddEvent(soundEvent);

			GetAnimationClip(attack_str)->SetIsLoop(false);
			GetAnimationClip(attack_str)->SetIsUseEventInTransition(false);

			GetAnimationClip(hit_str)->SetIsLoop(false);
			GetAnimationClip(hit_str)->SetIsUseEventInTransition(false);

			GetAnimationClip(die_str)->SetIsLoop(false);
			GetAnimationClip(die_str)->SetIsUseEventInTransition(false);

			GetAnimationClip(seriousPunch_str)->SetIsLoop(false);
			GetAnimationClip(seriousPunch_str)->SetIsUseEventInTransition(false);

			GetAnimationClip(jumpAttack_str)->SetIsLoop(false);
			GetAnimationClip(jumpAttack_str)->SetIsUseEventInTransition(false);

			// ------------------------------ SoundEventRegist ------------------------

			_animatorControllerIDMap.insert({ monsterAnimCon->GetName(), monsterAnimCon });

			_resourceUUIDMap.insert({ monsterAnimCon->GetUUID(), monsterAnimCon.get() });

			DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(monsterAnimCon.get(), TEXT("Asset/AnimatorController/Monster_AnimatorController_WeakElite.dcontroller"));
		}

	}

	void ResourceManager::LoadAnimatorController_WeakEliteEnemy()
	{
		///  ---------------------------------------------------------- Weak EliteEnemy
		{
			auto monsterAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Monster_AnimatorController_WeakElite"));

			auto monsterStateMachine = monsterAnimCon->AddStateMachine(TEXT("MonsterStateMachine"));

			const wchar_t* die_str = TEXT("weak_elite_die");
			const wchar_t* hit_str = TEXT("weak_elite_hit_back");
			const wchar_t* comboAttack_str = TEXT("weak_elite_attack_strong");
			const wchar_t* smash_str = TEXT("weak_elite_attack_normal");
			const wchar_t* idle_str = TEXT("weak_elite_idle");
			const wchar_t* run_str = TEXT("weak_elite_run_front");
			const wchar_t* walk_str = TEXT("weak_elite_walk_front");
			const wchar_t* walkRight_str = TEXT("weak_elite_walk_right");
			const wchar_t* walkLeft_str = TEXT("weak_elite_walk_left");
			const wchar_t* walkBack_str = TEXT("weak_elite_walk_back");

			// Parameter
			monsterAnimCon->AddParameter(TEXT("MoveSpeed"), AnimatorControllerParameterType::Float);

			monsterAnimCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsComboAttack"), AnimatorControllerParameterType::Bool);

			monsterAnimCon->AddParameter(TEXT("IsWalkRight"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkLeft"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsWalkBack"), AnimatorControllerParameterType::Bool);

			monsterAnimCon->AddParameter(TEXT("IsHit_Front"), AnimatorControllerParameterType::Bool);
			monsterAnimCon->AddParameter(TEXT("IsHit_Back"), AnimatorControllerParameterType::Bool);

			monsterAnimCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);
			// State & AnimClip
			std::vector<AnimatorState*> allState;

			auto monsterIdle = monsterStateMachine->AddState(TEXT("Idle"));
			monsterIdle->SetAnimationClip(GetAnimationClip(idle_str));
			allState.push_back(monsterIdle);

			auto monsterWalk = monsterStateMachine->AddState(TEXT("Walk"));
			monsterWalk->SetAnimationClip(GetAnimationClip(walk_str));
			allState.push_back(monsterWalk);

			auto monsterWalk_Right = monsterStateMachine->AddState(TEXT("Walk_Right"));
			monsterWalk_Right->SetAnimationClip(GetAnimationClip(walkRight_str));
			allState.push_back(monsterWalk_Right);

			auto monsterWalk_Left = monsterStateMachine->AddState(TEXT("Walk_Left"));
			monsterWalk_Left->SetAnimationClip(GetAnimationClip(walkLeft_str));
			allState.push_back(monsterWalk_Left);

			auto monsterWalk_Back = monsterStateMachine->AddState(TEXT("Walk_Back"));
			monsterWalk_Back->SetAnimationClip(GetAnimationClip(walkBack_str));
			allState.push_back(monsterWalk_Back);

			auto monsterRun = monsterStateMachine->AddState(TEXT("Run"));
			monsterRun->SetAnimationClip(GetAnimationClip(run_str));
			allState.push_back(monsterRun);

			auto monsterAttack_Smash = monsterStateMachine->AddState(TEXT("Attack"));
			monsterAttack_Smash->SetAnimationClip(GetAnimationClip(smash_str));
			allState.push_back(monsterAttack_Smash);

			auto monsterAttack_Combo = monsterStateMachine->AddState(TEXT("Attack_Combo"));
			GetAnimationClip(comboAttack_str)->SetIsRootMotion(true);
			GetAnimationClip(comboAttack_str)->SetRootMotionTargetIndex(0);
			monsterAttack_Combo->SetAnimationClip(GetAnimationClip(comboAttack_str));
			allState.push_back(monsterAttack_Combo);

			auto monsterHit_Front = monsterStateMachine->AddState(TEXT("Hit_Front"));
			monsterHit_Front->SetAnimationClip(GetAnimationClip(hit_str));
			allState.push_back(monsterHit_Front);

			auto monsterHit_Back = monsterStateMachine->AddState(TEXT("Hit_Back"));
			monsterHit_Back->SetAnimationClip(GetAnimationClip(hit_str));
			allState.push_back(monsterHit_Back);

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

			for (auto& iter : allState)
			{
				if (monsterAttack_Combo == iter) continue;
				auto transition = iter->AddTransition(monsterAttack_Combo);
				transition->AddCondition(TEXT("IsComboAttack"), AnimatorConditionMode::True);
				transition->SetTransitionDuration(0.01f);
				transition->SetTransitionOffset(0.f);
			}

			auto monsterDie = monsterStateMachine->AddState(TEXT("Die"));
			GetAnimationClip(die_str)->SetIsRootMotion(true);
			GetAnimationClip(die_str)->SetRootMotionTargetIndex(0);
			monsterDie->SetAnimationClip(GetAnimationClip(die_str));

			for (auto& iter : allState)
			{
				auto transition = iter->AddTransition(monsterDie);
				transition->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
				transition->SetTransitionDuration(0.01f);
				transition->SetTransitionOffset(0.f);
			}





			// Transition // Ʈ�������� �켱������ ���� ����Ѽ��̴�.
			auto monsterIdleToWalk = monsterIdle->AddTransition(monsterWalk);
			auto monsterIdleToWalk_Right = monsterIdle->AddTransition(monsterWalk_Right);
			auto monsterIdleToWalk_Left = monsterIdle->AddTransition(monsterWalk_Left);
			auto monsterIdleToWalk_Back = monsterIdle->AddTransition(monsterWalk_Back);
			auto monsterIdelToAttack_Smash = monsterIdle->AddTransition(monsterAttack_Smash);

			auto monsterWalkToIdle = monsterWalk->AddTransition(monsterIdle);
			auto monsterWalkToRun = monsterWalk->AddTransition(monsterRun);

			auto monsterWalk_RightToIdle = monsterWalk_Right->AddTransition(monsterIdle);
			auto monsterWalk_LeftToIdle = monsterWalk_Left->AddTransition(monsterIdle);
			auto monsterWalk_BackToIdle = monsterWalk_Back->AddTransition(monsterIdle);

			auto monsterRunToWalk = monsterRun->AddTransition(monsterWalk);

			auto monsterAttack_SmashToIdle = monsterAttack_Smash->AddTransition(monsterIdle);

			auto monsterAttack_ComboToIdle = monsterAttack_Combo->AddTransition(monsterIdle);

			auto monsterHit_FrontToIdle = monsterHit_Front->AddTransition(monsterIdle);

			auto monsterHit_BackToIdle = monsterHit_Back->AddTransition(monsterIdle);

			auto monsterDieToIdle = monsterDie->AddTransition(monsterIdle);

			monsterHit_FrontToIdle->SetTransitionDuration(0.01f);
			monsterHit_FrontToIdle->SetTransitionOffset(0.f);

			monsterHit_BackToIdle->SetTransitionDuration(0.01f);
			monsterHit_BackToIdle->SetTransitionOffset(0.f);

			monsterDieToIdle->AddCondition(TEXT("IsDie"), AnimatorConditionMode::False);
			monsterDieToIdle->SetTransitionDuration(0.01f);
			monsterDieToIdle->SetTransitionOffset(0.f);

			monsterAttack_SmashToIdle->SetTransitionDuration(0.1f);
			monsterAttack_SmashToIdle->SetTransitionOffset(0.f);

			monsterAttack_ComboToIdle->SetTransitionDuration(0.1f);
			monsterAttack_ComboToIdle->SetTransitionOffset(0.f);

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

			monsterIdelToAttack_Smash->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
			monsterIdelToAttack_Smash->SetTransitionDuration(0.1f);
			monsterIdelToAttack_Smash->SetTransitionOffset(0.f);


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

			auto attackClip_Combo = GetAnimationClip(comboAttack_str);
			AnimationEvent attack_ComboEvent;

			attack_ComboEvent._eventName = TEXT("ComboAttack1");
			attack_ComboEvent._targetFrame = 43.0f;
			attackClip_Combo->AddEvent(attack_ComboEvent);

			attack_ComboEvent._eventName = TEXT("ComboAttack2");
			attack_ComboEvent._targetFrame = 93.0f;
			attackClip_Combo->AddEvent(attack_ComboEvent);

			attack_ComboEvent._eventName = TEXT("ComboAttack3");
			attack_ComboEvent._targetFrame = 141.0f;
			attackClip_Combo->AddEvent(attack_ComboEvent);

			attack_ComboEvent._eventName = TEXT("SetBool_IsComboAttack_False");
			attack_ComboEvent._targetFrame = 208.0f;
			attackClip_Combo->AddEvent(attack_ComboEvent);

			attack_ComboEvent._eventName = TEXT("SetNavOffRigidbodyOn");
			attack_ComboEvent._targetFrame = 1.0f;
			attackClip_Combo->AddEvent(attack_ComboEvent);

			for (int i = 0; i < 10; i++)
			{
				attack_ComboEvent._eventName = TEXT("LerpLookTarget");
				attack_ComboEvent._targetFrame = 50.0f + (float)i;
				attackClip_Combo->AddEvent(attack_ComboEvent);
			}

			for (int i = 0; i < 10; i++)
			{
				attack_ComboEvent._eventName = TEXT("LerpLookTarget");
				attack_ComboEvent._targetFrame = 100.0f + (float)i;
				attackClip_Combo->AddEvent(attack_ComboEvent);
			}


			AnimationEvent animationEvent;

			auto idleClip = GetAnimationClip(idle_str);

			animationEvent._eventName = TEXT("LerpLookTarget");
			for (int i = 0; i < 122; i++)
			{
				animationEvent._targetFrame = static_cast<float>(i);
				idleClip->AddEvent(animationEvent);
			}

			auto attackClip_Smash = GetAnimationClip(smash_str);

			AnimationEvent attack_SmashEvent;

			attack_SmashEvent._eventName = TEXT("SetBool_IsAttack_False");
			attack_SmashEvent._targetFrame = 1.0f;
			attackClip_Smash->AddEvent(attack_SmashEvent);

			attack_SmashEvent._eventName = TEXT("SmashAttack");
			attack_SmashEvent._targetFrame = 80.0f;

			attackClip_Smash->AddEvent(attack_SmashEvent);

			auto dieClip = GetAnimationClip(die_str);
			AnimationEvent dieGroundEvent;

			dieGroundEvent._eventName = TEXT("SetNavOffRigidbodyOn");
			dieGroundEvent._targetFrame = 1.0f;
			dieClip->AddEvent(dieGroundEvent);

			dieGroundEvent._eventName = TEXT("StopAnimator");
			dieGroundEvent._targetFrame = 100.0f;
			dieClip->AddEvent(dieGroundEvent);

			dieGroundEvent._eventName = TEXT("SetNavOnRigidbodyOff");
			dieGroundEvent._targetFrame = 100.0f;
			dieClip->AddEvent(dieGroundEvent);


			// ------------------------------ Event Registe ---------------------------


			AnimationEvent soundEvent;

			soundEvent._eventName = TEXT("PlaySound_EliteCloseAttack_True_0");
			soundEvent._targetFrame = 1.0f;

			GetAnimationClip(smash_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlayVoiceSound_EliteMonsterHit_True_0");
			soundEvent._targetFrame = 1.0f;

			GetAnimationClip(hit_str)->AddEvent(soundEvent);

			soundEvent._eventName = TEXT("PlayVoiceSound_EliteMonster_Die_True_0");
			soundEvent._targetFrame = 1.0f;

			GetAnimationClip(die_str)->AddEvent(soundEvent);


			GetAnimationClip(smash_str)->SetIsLoop(false);
			GetAnimationClip(smash_str)->SetIsUseEventInTransition(false);

			GetAnimationClip(hit_str)->SetIsLoop(false);
			GetAnimationClip(hit_str)->SetIsUseEventInTransition(false);

			GetAnimationClip(die_str)->SetIsLoop(false);
			GetAnimationClip(die_str)->SetIsUseEventInTransition(false);

			// ------------------------------ SoundEventRegist ------------------------

			_animatorControllerIDMap.insert({ monsterAnimCon->GetName(), monsterAnimCon });

			_resourceUUIDMap.insert({ monsterAnimCon->GetUUID(), monsterAnimCon.get() });

			DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(monsterAnimCon.get(), TEXT("Asset/AnimatorController/Monster_AnimatorController_WeakElite.dcontroller"));
		}
	}

	void ResourceManager::LoadAnimatorController_BossEnemy()
	{
		auto monsterAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Monster_AnimatorController_Boss"));

		auto monsterStateMachine = monsterAnimCon->AddStateMachine(TEXT("MonsterStateMachine"));


		const wchar_t* normalIdle_str = TEXT("boss_normal_idle");
		const wchar_t* swordIdle_str = TEXT("boss_overdrive_sword_idle");
		const wchar_t* fistIdle_str = TEXT("boss_overdrive_fist_idle");
		const wchar_t* fistEnter_str = TEXT("boss_overdrive_fist_enter");
		const wchar_t* SwordEnter_str = TEXT("boss_overdrive_sword_enter");
		const wchar_t* dash_str = TEXT("boss_normal_dash");

		const wchar_t* hit1_str = TEXT("boss_normal_hit_1");
		const wchar_t* hit2_str = TEXT("boss_normal_hit_2");

		const wchar_t* sword_Walk_str = TEXT("boss_overdrive_sword_lock_front");
		const wchar_t* sword_Walk_left_str = TEXT("boss_overdrive_sword_lock_left");
		const wchar_t* sword_Walk_right_str = TEXT("boss_overdrive_sword_lock_right");
		const wchar_t* sword_Walk_back_str = TEXT("boss_overdrive_sword_lock_back");

		const wchar_t* fist_Walk_str = TEXT("boss_normal_lock_front");
		const wchar_t* fist_Walk_left_str = TEXT("boss_normal_lock_left");
		const wchar_t* fist_Walk_right_str = TEXT("boss_normal_lock_right");
		const wchar_t* fist_Walk_back_str = TEXT("boss_normal_lock_back");

		const wchar_t* sword_Run_str = TEXT("boss_overdrive_sword_lock_run_front");
		const wchar_t* fist_Run_str = TEXT("boss_normal_run");


		const wchar_t* fist_RandomPattern1_str = TEXT("boss_normal_swordcombo1_2");
		const wchar_t* fist_RandomPattern2_str = TEXT("boss_normal_swordcombo1_3");
		const wchar_t* fist_RandomPattern3_str = TEXT("boss_normal_swordcombo3_4");

		const wchar_t* sword_RandomPattern1_str = TEXT("boss_noraml_fistcombo1_2");
		const wchar_t* sword_RandomPattern2_str = TEXT("boss_normal_fistcombo1_3");
		const wchar_t* sword_RandomPattern3_str = TEXT("boss_normal_fistcombo2_4");

		const wchar_t* fist_HeavyAttack_str = TEXT("boss_overdrive_fist");
		const wchar_t* sword_HeavyAttack_str = TEXT("boss_overdrive_sword");

		// Parameter
		monsterAnimCon->AddParameter(TEXT("MoveSpeed"), AnimatorControllerParameterType::Float);
		monsterAnimCon->AddParameter(TEXT("IsWalkRight"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsWalkLeft"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsWalkBack"), AnimatorControllerParameterType::Bool);

		monsterAnimCon->AddParameter(TEXT("IsDash"), AnimatorControllerParameterType::Bool);

		monsterAnimCon->AddParameter(TEXT("IsHit_Front"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsHit_Back"), AnimatorControllerParameterType::Bool);

		monsterAnimCon->AddParameter(TEXT("IsFistForm"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsSwordForm"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsFormChange"), AnimatorControllerParameterType::Bool);

		monsterAnimCon->AddParameter(TEXT("IsSwordPattern1"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsSwordPattern2"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsSwordPattern3"), AnimatorControllerParameterType::Bool);

		monsterAnimCon->AddParameter(TEXT("IsFistPattern1"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsFistPattern2"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsFistPattern3"), AnimatorControllerParameterType::Bool);

		monsterAnimCon->AddParameter(TEXT("IsAirBorne"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);
		monsterAnimCon->AddParameter(TEXT("IsHeavyAttack"), AnimatorControllerParameterType::Bool);

		std::vector<AnimatorState*> allState;

		auto monsterNormalIdle = monsterStateMachine->AddState(TEXT("NormalIdle"));
		monsterNormalIdle->SetAnimationClip(GetAnimationClip(normalIdle_str));
		allState.push_back(monsterNormalIdle);

		auto monsterSwordIdle = monsterStateMachine->AddState(TEXT("SwordIdle"));
		monsterSwordIdle->SetAnimationClip(GetAnimationClip(swordIdle_str));
		allState.push_back(monsterSwordIdle);

		auto monsterFistIdle = monsterStateMachine->AddState(TEXT("FistIdle"));
		monsterFistIdle->SetAnimationClip(GetAnimationClip(fistIdle_str));
		allState.push_back(monsterFistIdle);

		auto monsterFistEnter = monsterStateMachine->AddState(TEXT("FistEnter"));
		monsterFistEnter->SetAnimationClip(GetAnimationClip(fistEnter_str));

		auto monsterSwordEnter = monsterStateMachine->AddState(TEXT("SwordEnter"));
		monsterSwordEnter->SetAnimationClip(GetAnimationClip(SwordEnter_str));

		auto monsterHit_Front = monsterStateMachine->AddState(TEXT("Hit_Front"));
		monsterHit_Front->SetAnimationClip(GetAnimationClip(hit1_str));
		allState.push_back(monsterHit_Front);

		auto monsterHit_Back = monsterStateMachine->AddState(TEXT("Hit_Back"));
		monsterHit_Back->SetAnimationClip(GetAnimationClip(hit2_str));
		allState.push_back(monsterHit_Back);

		auto monsterSwordWalk = monsterStateMachine->AddState(TEXT("SwordWalk"));
		monsterSwordWalk->SetAnimationClip(GetAnimationClip(sword_Walk_str));
		allState.push_back(monsterSwordWalk);

		auto monsterSwordWalk_Left = monsterStateMachine->AddState(TEXT("SwordWalk_Left"));
		monsterSwordWalk_Left->SetAnimationClip(GetAnimationClip(sword_Walk_left_str));
		allState.push_back(monsterSwordWalk_Left);

		auto monsterSwordWalk_Right = monsterStateMachine->AddState(TEXT("SwordWalk_Right"));
		monsterSwordWalk_Right->SetAnimationClip(GetAnimationClip(sword_Walk_right_str));
		allState.push_back(monsterSwordWalk_Right);

		auto monsterSwordWalk_Back = monsterStateMachine->AddState(TEXT("SwordWalk_Back"));
		monsterSwordWalk_Back->SetAnimationClip(GetAnimationClip(sword_Walk_back_str));
		allState.push_back(monsterSwordWalk_Back);

		auto monsterFistWalk = monsterStateMachine->AddState(TEXT("FistWalk"));
		monsterFistWalk->SetAnimationClip(GetAnimationClip(fist_Walk_str));
		allState.push_back(monsterFistWalk);

		auto monsterFistWalk_Left = monsterStateMachine->AddState(TEXT("FistWalk_Left"));
		monsterFistWalk_Left->SetAnimationClip(GetAnimationClip(fist_Walk_left_str));
		allState.push_back(monsterFistWalk_Left);

		auto monsterFistWalk_Right = monsterStateMachine->AddState(TEXT("FistWalk_Right"));
		monsterFistWalk_Right->SetAnimationClip(GetAnimationClip(fist_Walk_right_str));
		allState.push_back(monsterFistWalk_Right);

		auto monsterFistWalk_Back = monsterStateMachine->AddState(TEXT("FistWalk_Back"));
		monsterFistWalk_Back->SetAnimationClip(GetAnimationClip(fist_Walk_back_str));
		allState.push_back(monsterFistWalk_Back);

		auto monsterSwordRun = monsterStateMachine->AddState(TEXT("SwordRun"));
		monsterSwordRun->SetAnimationClip(GetAnimationClip(sword_Run_str));
		allState.push_back(monsterSwordRun);

		auto monsterFistRun = monsterStateMachine->AddState(TEXT("FistRun"));
		monsterFistRun->SetAnimationClip(GetAnimationClip(fist_Run_str));
		allState.push_back(monsterFistRun);

		auto monstFistPattern1 = monsterStateMachine->AddState(TEXT("FistPattern1"));
		monstFistPattern1->SetAnimationClip(GetAnimationClip(fist_RandomPattern1_str));
		allState.push_back(monstFistPattern1);

		auto monstFistPattern2 = monsterStateMachine->AddState(TEXT("FistPattern2"));
		monstFistPattern2->SetAnimationClip(GetAnimationClip(fist_RandomPattern2_str));
		allState.push_back(monstFistPattern2);

		auto monstFistPattern3 = monsterStateMachine->AddState(TEXT("FistPattern3"));
		monstFistPattern3->SetAnimationClip(GetAnimationClip(fist_RandomPattern3_str));
		allState.push_back(monstFistPattern3);

		auto monstSwordPattern1 = monsterStateMachine->AddState(TEXT("SwordPattern1"));
		monstSwordPattern1->SetAnimationClip(GetAnimationClip(sword_RandomPattern1_str));
		allState.push_back(monstSwordPattern1);

		auto monstSwordPattern2 = monsterStateMachine->AddState(TEXT("SwordPattern2"));
		monstSwordPattern2->SetAnimationClip(GetAnimationClip(sword_RandomPattern2_str));
		allState.push_back(monstSwordPattern2);

		auto monstSwordPattern3 = monsterStateMachine->AddState(TEXT("SwordPattern3"));
		monstSwordPattern3->SetAnimationClip(GetAnimationClip(sword_RandomPattern3_str));
		allState.push_back(monstSwordPattern3);

		auto monsterDash = monsterStateMachine->AddState(TEXT("Dash"));
		monsterDash->SetAnimationClip(GetAnimationClip(dash_str));
		GetAnimationClip(dash_str)->SetIsRootMotion(true);


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

		for (auto& iter : allState)
		{
			auto iterToDash = iter->AddTransition(monsterDash);

			iterToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
			iterToDash->SetTransitionDuration(0.00f);
			iterToDash->SetTransitionOffset(0.f);
		}

		auto heavyAttack_Fist = monsterStateMachine->AddState(TEXT("HeavyAttack_Fist"));
		heavyAttack_Fist->SetAnimationClip(GetAnimationClip(fist_HeavyAttack_str));
		GetAnimationClip(fist_HeavyAttack_str)->SetIsRootMotion(true);
		allState.push_back(heavyAttack_Fist);

		auto heavyAttack_Sword = monsterStateMachine->AddState(TEXT("HeavyAttack_Sword"));
		heavyAttack_Sword->SetAnimationClip(GetAnimationClip(sword_HeavyAttack_str));
		GetAnimationClip(sword_HeavyAttack_str)->SetIsRootMotion(true);
		allState.push_back(heavyAttack_Sword);

		auto funcCommonMoveTransition = [&](AnimatorState* idle
			, AnimatorState* walk
			, AnimatorState* walk_left
			, AnimatorState* walk_right
			, AnimatorState* walk_back
			, AnimatorState* run
			, AnimatorState* heavyAttack)
		{

			auto idleToHeavyAttack = idle->AddTransition(heavyAttack);
			auto idleToWalk = idle->AddTransition(walk);
			auto idleToWalk_Left = idle->AddTransition(walk_left);
			auto idleToWalk_Right = idle->AddTransition(walk_right);
			auto idleToWalk_Back = idle->AddTransition(walk_back);

			auto walkToRun = walk->AddTransition(run);

			auto walkToIdle = walk->AddTransition(idle);
			auto walk_LeftToIdle = walk_left->AddTransition(idle);
			auto walk_RightToIdle = walk_right->AddTransition(idle);
			auto walk_BackToIdle = walk_back->AddTransition(idle);
			auto heavyAttackToIdle = heavyAttack->AddTransition(idle);
			auto runToWalk = run->AddTransition(idle);

			idleToWalk->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Greater, 0.5f);
			idleToWalk->SetTransitionDuration(0.01f);
			idleToWalk->SetTransitionOffset(0.f);

			walkToIdle->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Less, 0.49f);
			walkToIdle->SetTransitionDuration(0.01f);
			walkToIdle->SetTransitionOffset(0.f);

			idleToWalk_Right->AddCondition(TEXT("IsWalkRight"), AnimatorConditionMode::True);
			idleToWalk_Right->SetTransitionDuration(0.01f);
			idleToWalk_Right->SetTransitionOffset(0.f);

			idleToWalk_Left->AddCondition(TEXT("IsWalkLeft"), AnimatorConditionMode::True);
			idleToWalk_Left->SetTransitionDuration(0.01f);
			idleToWalk_Left->SetTransitionOffset(0.f);

			idleToWalk_Back->AddCondition(TEXT("IsWalkBack"), AnimatorConditionMode::True);
			idleToWalk_Back->SetTransitionDuration(0.01f);
			idleToWalk_Back->SetTransitionOffset(0.f);

			idleToHeavyAttack->AddCondition(TEXT("IsHeavyAttack"), AnimatorConditionMode::True);
			idleToHeavyAttack->SetTransitionDuration(0.01f);
			idleToHeavyAttack->SetTransitionOffset(0.f);

			walk_RightToIdle->AddCondition(TEXT("IsWalkRight"), AnimatorConditionMode::False);
			walk_RightToIdle->SetTransitionDuration(0.01f);
			walk_RightToIdle->SetTransitionOffset(0.f);

			walk_LeftToIdle->AddCondition(TEXT("IsWalkLeft"), AnimatorConditionMode::False);
			walk_LeftToIdle->SetTransitionDuration(0.01f);
			walk_LeftToIdle->SetTransitionOffset(0.f);

			walk_BackToIdle->AddCondition(TEXT("IsWalkBack"), AnimatorConditionMode::False);
			walk_BackToIdle->SetTransitionDuration(0.01f);
			walk_BackToIdle->SetTransitionOffset(0.f);

			walkToRun->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Greater, 1.0f);
			walkToRun->SetTransitionDuration(0.01f);
			walkToRun->SetTransitionOffset(0.f);

			heavyAttackToIdle->AddCondition(TEXT("IsHeavyAttack"), AnimatorConditionMode::False);
			heavyAttackToIdle->SetTransitionDuration(0.01f);
			heavyAttackToIdle->SetTransitionOffset(0.f);

			runToWalk->AddCondition(TEXT("MoveSpeed"), AnimatorConditionMode::Less, 0.99f);
			runToWalk->SetTransitionDuration(0.01f);
			runToWalk->SetTransitionOffset(0.f);

		};
		auto dashToNormalIdle = monsterDash->AddTransition(monsterNormalIdle);
		auto monsterSwordIdleToNormalIdle = monsterSwordIdle->AddTransition(monsterNormalIdle);
		auto monsterFistIdleToNormalIdle = monsterFistIdle->AddTransition(monsterNormalIdle);

		auto monsterNormalIdleToSwordEnter = monsterNormalIdle->AddTransition(monsterSwordEnter);
		auto monsterNormalIdleToFistEnter = monsterNormalIdle->AddTransition(monsterFistEnter);

		auto monsterNormalIdleToSwordIdle = monsterNormalIdle->AddTransition(monsterSwordIdle);
		auto monsterNormalIdleToFistIdle = monsterNormalIdle->AddTransition(monsterFistIdle);

		auto monsterSwordEnterToSwordIdle = monsterSwordEnter->AddTransition(monsterSwordIdle);
		auto monsterFistEnterToFistIdle = monsterFistEnter->AddTransition(monsterFistIdle);

		auto monsterHit_BackToNormalIdle = monsterHit_Back->AddTransition(monsterNormalIdle);
		auto monsterHit_FrontToNormalIdle = monsterHit_Front->AddTransition(monsterNormalIdle);

		auto monsterFistIdleToFistPattern1 = monsterFistIdle->AddTransition(monstFistPattern1);
		auto monsterFistIdleToFistPattern2 = monsterFistIdle->AddTransition(monstFistPattern2);
		auto monsterFistIdleToFistPattern3 = monsterFistIdle->AddTransition(monstFistPattern3);
		auto monsterSwordIdleToSwordPattern1 = monsterSwordIdle->AddTransition(monstSwordPattern1);
		auto monsterSwordIdleToSwordPattern2 = monsterSwordIdle->AddTransition(monstSwordPattern2);
		auto monsterSwordIdleToSwordPattern3 = monsterSwordIdle->AddTransition(monstSwordPattern3);

		auto FistPattern1ToNormalIdle = monstFistPattern1->AddTransition(monsterNormalIdle);
		auto FistPattern2ToNormalIdle = monstFistPattern2->AddTransition(monsterNormalIdle);
		auto FistPattern3ToNormalIdle = monstFistPattern3->AddTransition(monsterNormalIdle);
		auto SwordPattern1ToNormalIdle = monstSwordPattern1->AddTransition(monsterNormalIdle);
		auto SwordPattern2ToNormalIdle = monstSwordPattern2->AddTransition(monsterNormalIdle);
		auto SwordPattern3ToNormalIdle = monstSwordPattern3->AddTransition(monsterNormalIdle);

		dashToNormalIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		dashToNormalIdle->SetTransitionDuration(0.01f);
		dashToNormalIdle->SetTransitionOffset(0.0f);

		monsterFistIdleToFistPattern1->AddCondition(TEXT("IsFistPattern1"), AnimatorConditionMode::True);
		monsterFistIdleToFistPattern1->SetTransitionDuration(0.01f);
		monsterFistIdleToFistPattern1->SetTransitionOffset(0.0f);

		monsterFistIdleToFistPattern1->AddCondition(TEXT("IsFistPattern1"), AnimatorConditionMode::True);
		monsterFistIdleToFistPattern1->SetTransitionDuration(0.01f);
		monsterFistIdleToFistPattern1->SetTransitionOffset(0.0f);

		monsterFistIdleToFistPattern2->AddCondition(TEXT("IsFistPattern2"), AnimatorConditionMode::True);
		monsterFistIdleToFistPattern2->SetTransitionDuration(0.01f);
		monsterFistIdleToFistPattern2->SetTransitionOffset(0.0f);

		monsterFistIdleToFistPattern3->AddCondition(TEXT("IsFistPattern3"), AnimatorConditionMode::True);
		monsterFistIdleToFistPattern3->SetTransitionDuration(0.01f);
		monsterFistIdleToFistPattern3->SetTransitionOffset(0.0f);

		monsterSwordIdleToSwordPattern1->AddCondition(TEXT("IsSwordPattern1"), AnimatorConditionMode::True);
		monsterSwordIdleToSwordPattern1->SetTransitionDuration(0.01f);
		monsterSwordIdleToSwordPattern1->SetTransitionOffset(0.0f);

		monsterSwordIdleToSwordPattern2->AddCondition(TEXT("IsSwordPattern2"), AnimatorConditionMode::True);
		monsterSwordIdleToSwordPattern2->SetTransitionDuration(0.01f);
		monsterSwordIdleToSwordPattern2->SetTransitionOffset(0.0f);

		monsterSwordIdleToSwordPattern3->AddCondition(TEXT("IsSwordPattern3"), AnimatorConditionMode::True);
		monsterSwordIdleToSwordPattern3->SetTransitionDuration(0.01f);
		monsterSwordIdleToSwordPattern3->SetTransitionOffset(0.0f);

		FistPattern1ToNormalIdle->AddCondition(TEXT("IsFistPattern1"), AnimatorConditionMode::False);
		FistPattern1ToNormalIdle->SetTransitionDuration(0.01f);
		FistPattern1ToNormalIdle->SetTransitionOffset(0.0f);

		FistPattern2ToNormalIdle->AddCondition(TEXT("IsFistPattern2"), AnimatorConditionMode::False);
		FistPattern2ToNormalIdle->SetTransitionDuration(0.01f);
		FistPattern2ToNormalIdle->SetTransitionOffset(0.0f);

		FistPattern3ToNormalIdle->AddCondition(TEXT("IsFistPattern3"), AnimatorConditionMode::False);
		FistPattern3ToNormalIdle->SetTransitionDuration(0.01f);
		FistPattern3ToNormalIdle->SetTransitionOffset(0.0f);

		SwordPattern1ToNormalIdle->AddCondition(TEXT("IsSwordPattern1"), AnimatorConditionMode::False);
		SwordPattern1ToNormalIdle->SetTransitionDuration(0.01f);
		SwordPattern1ToNormalIdle->SetTransitionOffset(0.0f);

		SwordPattern2ToNormalIdle->AddCondition(TEXT("IsSwordPattern2"), AnimatorConditionMode::False);
		SwordPattern2ToNormalIdle->SetTransitionDuration(0.01f);
		SwordPattern2ToNormalIdle->SetTransitionOffset(0.0f);

		SwordPattern3ToNormalIdle->AddCondition(TEXT("IsSwordPattern3"), AnimatorConditionMode::False);
		SwordPattern3ToNormalIdle->SetTransitionDuration(0.01f);
		SwordPattern3ToNormalIdle->SetTransitionOffset(0.0f);

		monsterHit_BackToNormalIdle->SetTransitionDuration(0.0f);
		monsterHit_BackToNormalIdle->SetTransitionOffset(0.0f);

		monsterHit_FrontToNormalIdle->SetTransitionDuration(0.0f);
		monsterHit_FrontToNormalIdle->SetTransitionOffset(0.0f);

		monsterSwordIdleToNormalIdle->AddCondition(TEXT("IsSwordForm"), AnimatorConditionMode::False);
		monsterSwordIdleToNormalIdle->SetTransitionDuration(0.01f);
		monsterSwordIdleToNormalIdle->SetTransitionOffset(0.0f);

		monsterFistIdleToNormalIdle->AddCondition(TEXT("IsFistForm"), AnimatorConditionMode::False);
		monsterFistIdleToNormalIdle->SetTransitionDuration(0.01f);
		monsterFistIdleToNormalIdle->SetTransitionOffset(0.0f);

		monsterNormalIdleToSwordEnter->AddCondition(TEXT("IsSwordForm"), AnimatorConditionMode::True);
		monsterNormalIdleToSwordEnter->AddCondition(TEXT("IsFormChange"), AnimatorConditionMode::True);
		monsterNormalIdleToSwordEnter->SetTransitionDuration(0.01f);
		monsterNormalIdleToSwordEnter->SetTransitionOffset(0.0f);

		monsterNormalIdleToFistEnter->AddCondition(TEXT("IsFistForm"), AnimatorConditionMode::True);
		monsterNormalIdleToFistEnter->AddCondition(TEXT("IsFormChange"), AnimatorConditionMode::True);
		monsterNormalIdleToFistEnter->SetTransitionDuration(0.01f);
		monsterNormalIdleToFistEnter->SetTransitionOffset(0.0f);

		monsterNormalIdleToSwordIdle->AddCondition(TEXT("IsSwordForm"), AnimatorConditionMode::True);
		monsterNormalIdleToSwordIdle->SetTransitionDuration(0.01f);
		monsterNormalIdleToSwordIdle->SetTransitionOffset(0.0f);

		monsterNormalIdleToFistIdle->AddCondition(TEXT("IsFistForm"), AnimatorConditionMode::True);
		monsterNormalIdleToFistIdle->SetTransitionDuration(0.01f);
		monsterNormalIdleToFistIdle->SetTransitionOffset(0.0f);

		monsterSwordEnterToSwordIdle->SetTransitionDuration(0.01f);
		monsterSwordEnterToSwordIdle->SetTransitionOffset(0.0f);

		monsterFistEnterToFistIdle->SetTransitionDuration(0.01f);
		monsterFistEnterToFistIdle->SetTransitionOffset(0.0f);

		funcCommonMoveTransition(monsterSwordIdle, monsterSwordWalk, monsterSwordWalk_Left, monsterSwordWalk_Right, monsterSwordWalk_Back, monsterSwordRun, heavyAttack_Sword);
		funcCommonMoveTransition(monsterFistIdle, monsterFistWalk, monsterFistWalk_Left, monsterFistWalk_Right, monsterFistWalk_Back, monsterFistRun, heavyAttack_Fist);

		// ------------------------------ Event Registe ---------------------------

		AnimationEvent animEvent;

		auto idleClip = GetAnimationClip(fistIdle_str);

		animEvent._eventName = TEXT("LerpLookTarget");
		for (int i = 0; i < 180; i++)
		{
			animEvent._targetFrame = static_cast<float>(i);
			idleClip->AddEvent(animEvent);
		}
		idleClip = GetAnimationClip(swordIdle_str);
		for (int i = 0; i < 271; i++)
		{
			animEvent._targetFrame = static_cast<float>(i);
			idleClip->AddEvent(animEvent);
		}

		{
			animEvent._eventName = TEXT("SetBool_IsFormChange_False");
			animEvent._targetFrame = 70.0f;
			GetAnimationClip(SwordEnter_str)->AddEvent(animEvent);
			animEvent._targetFrame = 75.0f;
			GetAnimationClip(fistEnter_str)->AddEvent(animEvent);
		}
		{
			animEvent._eventName = TEXT("SetNavOffRigidbodyOn");
			animEvent._targetFrame = 0.0f;
			GetAnimationClip(sword_HeavyAttack_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetBool_IsHeavyAttack_False");
			animEvent._targetFrame = 160.0f;
			GetAnimationClip(sword_HeavyAttack_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetNavOnRigidbodyOff");
			animEvent._targetFrame = 160.0f;
			GetAnimationClip(sword_HeavyAttack_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SuperArmorOff_OnTimer");
			animEvent._targetFrame = 160.0f;
			GetAnimationClip(sword_HeavyAttack_str)->AddEvent(animEvent);
		}
		{
			animEvent._eventName = TEXT("SetNavOffRigidbodyOn");
			animEvent._targetFrame = 0.0f;
			GetAnimationClip(fist_HeavyAttack_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetBool_IsHeavyAttack_False");
			animEvent._targetFrame = 137.0f;
			GetAnimationClip(fist_HeavyAttack_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetNavOnRigidbodyOff");
			animEvent._targetFrame = 137.0f;
			GetAnimationClip(fist_HeavyAttack_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SuperArmorOff_OnTimer");
			animEvent._targetFrame = 137.0f;
			GetAnimationClip(fist_HeavyAttack_str)->AddEvent(animEvent);
		}
		{
			animEvent._eventName = TEXT("SetBool_IsSwordPattern1_False");
			animEvent._targetFrame = 65.0f;
			GetAnimationClip(sword_RandomPattern1_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetBool_IsSwordPattern2_False");
			animEvent._targetFrame = 59.0f;
			GetAnimationClip(sword_RandomPattern2_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetBool_IsSwordPattern3_False");
			animEvent._targetFrame = 100.0f;
			GetAnimationClip(sword_RandomPattern3_str)->AddEvent(animEvent);
		}
		{
			animEvent._eventName = TEXT("SetBool_IsFistPattern1_False");
			animEvent._targetFrame = 58.0f;
			GetAnimationClip(fist_RandomPattern1_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetBool_IsFistPattern2_False");
			animEvent._targetFrame = 67.0f;
			GetAnimationClip(fist_RandomPattern2_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetBool_IsFistPattern3_False");
			animEvent._targetFrame = 110.0f;
			GetAnimationClip(fist_RandomPattern3_str)->AddEvent(animEvent);
		}
		{
			animEvent._eventName = TEXT("SetNavOffRigidbodyOn");
			animEvent._targetFrame = 0.0f;
			GetAnimationClip(dash_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("RandomLookAt");
			animEvent._targetFrame = 0.0f;
			GetAnimationClip(dash_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetBool_IsDash_False");
			animEvent._targetFrame = 28.0f;
			GetAnimationClip(dash_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("SetNavOnRigidbodyOff");
			animEvent._targetFrame = 28.0f;
			GetAnimationClip(dash_str)->AddEvent(animEvent);
		}
		{
			animEvent._eventName = TEXT("HoldSword");
			animEvent._targetFrame = 0.0f;
			GetAnimationClip(SwordEnter_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("DisablingPatternStart");
			animEvent._targetFrame = 0.0f;
			GetAnimationClip(SwordEnter_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("HouseSword");
			animEvent._targetFrame = 0.0f;
			GetAnimationClip(fistEnter_str)->AddEvent(animEvent);

			animEvent._eventName = TEXT("DisablingPatternEnd");
			animEvent._targetFrame = 0.0f;
			GetAnimationClip(fistEnter_str)->AddEvent(animEvent);
		}

		_animatorControllerIDMap.insert({ monsterAnimCon->GetName(), monsterAnimCon });

		_resourceUUIDMap.insert({ monsterAnimCon->GetUUID(), monsterAnimCon.get() });

		DUOLGameEngine::SerializeManager::GetInstance()->SerializeAnimatorController(monsterAnimCon.get(), TEXT("Asset/AnimatorController/Monster_AnimatorController_WeakElite.dcontroller"));
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
					// GraphicsEngine�� ������ ���͸����� ���� ������ �����ϴ� ����..! 
					sMat = std::make_shared<DUOLGameEngine::Material>(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn"));

					sMat->SetPrimitiveMaterial(mat);

					_materialIDMap.insert({ primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn"), sMat });

					_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });

				}

				mat = _graphicsEngine->LoadMaterial(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn_DownUp"));
				if (mat)
				{
					// GraphicsEngine�� ������ ���͸����� ���� ������ �����ϴ� ����..! 
					sMat = std::make_shared<DUOLGameEngine::Material>(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn_DownUp"));

					sMat->SetPrimitiveMaterial(mat);

					_materialIDMap.insert({ primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn_DownUp"), sMat });

					_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });
				}
			}
		}


#pragma region PARTICLE // ���߿� Json�� �о���� �������� �ٲ���...!
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
	}

	void ResourceManager::LoadPhysicsMaterialTable(const DUOLCommon::tstring& path)
	{
		const DUOLPhysics::PhysicsMaterialDesc matDesc{ 0.6f, 0.5f, 0.f };

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
		LoadAnimatorController_CloseEnemy();
		LoadAnimatorController_FarEnemy();
		LoadAnimatorController_EliteEnemy();
		LoadAnimatorController_WeakEliteEnemy();
		LoadAnimatorController_BossEnemy();
#pragma endregion
	}

	void ResourceManager::LoadAudioClipTable(const DUOLCommon::tstring& path)
	{
		PlayerAudioClipTable();
	}



	void ResourceManager::PlayerAudioClipTable()
	{
		// TODO : ������ ��ü ���� .. ��Ÿ �����͸� ���� ������Ʈ �н������� �ڵ� ����ȭ�� �ʿ��ϴ� ..
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

	//void ResourceManager::LoadPrefabTable(const DUOLCommon::tstring& path)
	//{
	//	auto jsonReader = DUOLJson::JsonReader::GetInstance();

	//	auto modelTable = jsonReader->LoadJson(path);

	//	const TCHAR* id = TEXT("MeshID");

	//	const TCHAR* materialid = TEXT("MaterialID");

	//	const TCHAR* animationid = TEXT("AnimationID");

	//	uint64 modelID;
	//	std::vector<uint64> materialID;
	//	std::vector<uint64> animationID;

	//	for (auto& model : modelTable->GetArray())
	//	{
	//		for (auto& datas : model.GetArray())
	//		{
	//			if (datas.HasMember(id) && datas.HasMember(materialid) && datas.HasMember(animationid))
	//			{
	//				modelID = datas[id].GetInt64();

	//				if (datas[materialid].IsArray())
	//				{
	//					for (auto iter = datas[materialid].Begin(); iter != datas[materialid].End(); iter++)
	//					{
	//						uint64 matID = (*iter).GetInt64();

	//						materialID.emplace_back(matID);
	//					}
	//				}
	//				if (datas[animationid].IsArray())
	//				{
	//					for (auto iter = datas[animationid].Begin(); iter != datas[animationid].End(); iter++)
	//					{
	//						uint64 animID = (*iter).GetInt64();

	//						animationID.emplace_back(animID);
	//					}
	//				}
	//			}

	//			// Prefab input
	//			_perfabsIDList.emplace_back(std::make_pair(modelID, std::make_pair(materialID, animationID)));
	//			materialID.clear();
	//			animationID.clear();
	//		}
	//	}
	//}

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
		_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });

		return sMat.get();
	}

	DUOLGameEngine::Material* ResourceManager::CreateMaterial(const DUOLCommon::tstring& materialID,
		DUOLGraphicsEngine::MaterialDesc& materialDesc)
	{
		auto foundMat = _materialIDMap.find(materialID);

		if (_materialIDMap.end() != foundMat)
		{
			return foundMat->second.get();
		}

		auto mat = _graphicsEngine->CreateMaterial(materialID, materialDesc);

		std::shared_ptr<DUOLGameEngine::Material> sMat = std::make_shared<DUOLGameEngine::Material>(materialID);

		sMat->SetPrimitiveMaterial(mat);

		DUOLCommon::tstring targetName = materialID;

		_materialIDMap.insert({ targetName, sMat });
		_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });

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

	DUOLGameEngine::ObjectBase* ResourceManager::GetResourceByName(const DUOLCommon::tstring& name,
		const DUOLCommon::tstring& type) const
	{

		DUOLGameEngine::ObjectBase* ret = nullptr;

		if (type == L"_mesh")
		{
			// Check Mesh
			ret = GetMesh(name);
		}
		else if (type == L"AnimationClip*")
		{
			// Check Animation Clip
			ret = GetAnimationClip(name);
		}
		else if (type == L"Material*")
		{
			// Check Material
			ret = GetMaterial(name);
		}
		else if (type == L"PhysicsMaterial*")
		{
			// Check PhysicsMaterial
			ret = GetPhysicsMaterial(name);
		}
		else if (type == L"Avatar*")
		{
			ret = GetAvatar(name);
		}
		else if (type == L"AnimationController*")
		{
			// Check AnimatorController
			ret = GetAnimatorController(name);
		}
		else if (type == L"Sprite*")
		{
			// Insert Sprite Name
			ret = GetSprite(name);
		}
		else if (type == L"AudioClip*")
		{
			// Check AudioClip
			ret = GetAudioClip(name);
		}

		if (ret != nullptr)
			return ret;

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

		const DUOLCommon::tstring& projectPath = gameSpec._projectPath;

		// Physics�� Audio�� ó���θ��� �ҷ��ش�.
		LoadPhysicsMaterialTable(gameSpec._projectPath + TEXT("Asset/DataTable/PhysicsMaterialTable.json"));

		LoadAudioClipTable(gameSpec._projectPath + TEXT("Asset/DataTable/AudioClipTable.json"));

		InitializeMaterial();

		DUOL_INFO(DUOL_FILE, "ResourceManager Initialize Success !");
	}

	void ResourceManager::LateInitialize(const EngineSpecification& gameSpec)
	{
#pragma region CLIENT_CODE
		/// LoadFBXTable�� �θ��� ���� �ҷ�����մϴ�.
		// 1. LoadPerfab Table
		//LoadPrefabTable(TEXT("Asset/DataTable/Prefab.json"));

		// 2. LoadMaterial Table
		//LoadDataNameTable(TEXT("Asset/DataTable/Material.json"), true);

		// 3. Animation Table
		//LoadDataNameTable(TEXT("Asset/DataTable/Animation.json"), false);

		LoadFBXTable(TEXT("Asset/DataTable/MeshTable.json"));

		LoadMaterialTable(TEXT("Asset/DataTable/MaterialTable.json"));

		LoadAnimationClipTable(TEXT("Asset/DataTable/AnimationClipTable.json"));

		LoadAnimatorControllerTable(TEXT("Asset/DataTable/AnimatorControllerTable.json"));
#pragma endregion

		_isThread = true;
	}

	void ResourceManager::InitializeMaterial()
	{
		DUOLGraphicsEngine::Material* mat;

		std::shared_ptr<DUOLGameEngine::Material> sMat;

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

		mat = _graphicsEngine->LoadMaterial(_T("SkinnedDefault_RED"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("SkinnedDefault_RED"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("SkinnedDefault_RED") , sMat });

		_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });

		mat = _graphicsEngine->LoadMaterial(_T("SkinnedDefault_WHITE"));

		sMat = std::make_shared<DUOLGameEngine::Material>(_T("SkinnedDefault_WHITE"));

		sMat->SetPrimitiveMaterial(mat);

		_materialIDMap.insert({ _T("SkinnedDefault_WHITE") , sMat });

		_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });
	}

	void ResourceManager::UnInitialize()
	{
		_graphicsEngine.reset();
	}

	void ResourceManager::Update(float deltaTime)
	{
		// TODO : ������ �÷��� ..? �� �� �� ������ .. �� Ŭ���̾�Ʈ ��û�� ���� ���ҽ� �ε�, ��ε� ���� ..?
	}
}