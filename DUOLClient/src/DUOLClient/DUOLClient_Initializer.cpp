#include "DUOLClient/DUOLClient_Initializer.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

#include "DUOLClient/BehaviorTreeNode/Action/Action_AroundPatrol.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_BoidsMoveTo.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_Die.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_Hit.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_UseTokenAttack.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_NavMeshAgentOn_RigidbodyOff.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_SuperArmorUpdate.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_EliteMonsterNormalOrHeavyAttack.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_EliteEnemyRandomPattern.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_RushPattern.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_SetNavOnRigidbodyOff.h"
#include "DUOLClient/BehaviorTreeNode/Action/Action_BossEnemyRandomPattern.h"

#include "DUOLClient/BehaviorTreeNode/Condition/Condition_CurrentAnimStateCheck.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_HasToken.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsClose.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsGroupCheck.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInAttackRange.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsLookTarget.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsAirborne.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsChaseRange.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInPatrolRange.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsSuperArmor.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsTargetDie.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_CloseEnemyRayCast.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

namespace DUOLClient
{

	bool DUOLClient_Initializer::_isInit = false;

	void DUOLClient_Initializer::DUOLClient_Initialize()
	{
		if (_isInit) return;

		_isInit = true;

		DUOLCommon::LogHelper::Initialize();

		BehaviorTreeFactory_Initialize();

		AnimatorController_Initialize();

		Overdrive_Sword_Initialize();

		Overdrive_Fist_Initialize();
	}

	void DUOLClient_Initializer::BehaviorTreeFactory_Initialize()
	{
		auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

		treeFactory->RegisterNodeType<Condition_IsInAttackRange>("Condition_IsInAttackRange");
		treeFactory->RegisterNodeType<Condition_IsLookTarget>("Condition_IsLookTarget");
		treeFactory->RegisterNodeType<Condition_IsGroupCheck>("Condition_IsGroupCheck");
		treeFactory->RegisterNodeType<Condition_IsClose>("Condition_IsClose");
		treeFactory->RegisterNodeType<Condition_HasToken>("Condition_HasToken");
		treeFactory->RegisterNodeType<Condition_IsAirborne>("Condition_IsAirborne");
		treeFactory->RegisterNodeType<Condition_CurrentAnimStateCheck>("Condition_CurrentAnimStateCheck");
		treeFactory->RegisterNodeType<Condition_IsInPatrolRange>("Condition_IsInPatrolRange");
		treeFactory->RegisterNodeType<Condition_IsSuperArmor>("Condition_IsSuperArmor");
		treeFactory->RegisterNodeType<Condition_IsTargetDie>("Condition_IsTargetDie");
		treeFactory->RegisterNodeType<Condition_CloseEnemyRayCast>("Condition_CloseEnemyRayCast");

		treeFactory->RegisterNodeType<Action_AroundPatrol>("Action_AroundPatrol");
		treeFactory->RegisterNodeType<Action_UseTokenAttack>("Action_UseTokenAttack");
		treeFactory->RegisterNodeType<Action_Die>("Action_Die");
		treeFactory->RegisterNodeType<Action_Hit>("Action_Hit");
		treeFactory->RegisterNodeType<Action_BoidsMoveTo>("Action_BoidsMoveTo");
		treeFactory->RegisterNodeType<Action_NavMeshAgentOn_RigidbodyOff>("Action_NavMeshAgentOn_RigidbodyOff");
		treeFactory->RegisterNodeType<Action_SuperArmorUpdate>("Action_SuperArmorUpdate");
		treeFactory->RegisterNodeType<Action_EliteMonsterNormalOrHeavyAttack>("Action_EliteMonsterNormalOrHeavyAttack");
		treeFactory->RegisterNodeType<Action_EliteEnemyRandomPattern>("Action_EliteEnemyRandomPattern");
		treeFactory->RegisterNodeType<Action_RushPattern>("Action_RushPattern");
		treeFactory->RegisterNodeType<Action_SetNavOnRigidbodyOff>("Action_SetNavOnRigidbodyOff");
		treeFactory->RegisterNodeType<Action_BossEnemyRandomPattern>("Action_BossEnemyRandomPattern");

		treeFactory->RegisterNodeType<Condition_IsChaseRange>("Condition_IsChaseRange");

		treeFactory->Initialize();
	}

	void DUOLClient_Initializer::AnimatorController_Initialize()
	{
		using namespace DUOLGameEngine;

		auto playerNormalAnimCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Player_NormalAnimatorController"));

		auto playerNormalStateMachine = playerNormalAnimCon->AddStateMachine(TEXT("PlayerNormalStateMachine"));

		// 플레이어 상태
		playerNormalAnimCon->AddParameter(TEXT("IsMove"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsRun"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsLockOn"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsDash"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsHit1"), AnimatorControllerParameterType::Bool);				// 약공격 1

		playerNormalAnimCon->AddParameter(TEXT("IsHit2"), AnimatorControllerParameterType::Bool);				// 약공격 2

		playerNormalAnimCon->AddParameter(TEXT("IsHeavyHit"), AnimatorControllerParameterType::Bool);			// 강공격

		playerNormalAnimCon->AddParameter(TEXT("IsDown"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsDownup"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsOverdriveSwordEnter"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsOverdriveFistEnter"), AnimatorControllerParameterType::Bool);

		// 방향 여부
		playerNormalAnimCon->AddParameter(TEXT("IsLeft"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsRight"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsFront"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsBack"), AnimatorControllerParameterType::Bool);

		// 공격 유형
		playerNormalAnimCon->AddParameter(TEXT("IsSword"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsFist"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("AttackCount"), AnimatorControllerParameterType::Int);

		// 애니메이션 재생 속도 배속
		playerNormalAnimCon->AddParameter(TEXT("AnimationSpeed"), AnimatorControllerParameterType::Float);

		// Idle
		auto playerIdle = playerNormalStateMachine->AddState(TEXT("Player_Idle"));
		playerIdle->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_idle")));

		// Move
		auto playerMove = playerNormalStateMachine->AddState(TEXT("Player_Move"));
		playerMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_move")));

		// Run
		auto playerRun = playerNormalStateMachine->AddState(TEXT("Player_Run"));
		playerRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_run")));

		// Dash
		auto playerDash = playerNormalStateMachine->AddState(TEXT("Player_Dash"));
		auto playerDashClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_dash"));
		playerDashClip->SetIsRootMotion(true);
		playerDash->SetAnimationClip(playerDashClip);

		// Lock-On Move and Run
		auto playerLockOnLeftMove = playerNormalStateMachine->AddState(TEXT("Player_LockOnLeftMove"));
		playerLockOnLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_left")));

		auto playerLockOnRightMove = playerNormalStateMachine->AddState(TEXT("Player_LockOnRightMove"));
		playerLockOnRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_right")));

		auto playerLockOnFrontMove = playerNormalStateMachine->AddState(TEXT("Player_LockOnFrontMove"));
		playerLockOnFrontMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_front")));

		auto playerLockOnBackMove = playerNormalStateMachine->AddState(TEXT("Player_LockOnBackMove"));
		playerLockOnBackMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_back")));

		auto playerLockOnFrontLeftMove = playerNormalStateMachine->AddState(TEXT("Player_LockOnFrontLeftMove"));
		playerLockOnFrontLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_leftfront")));

		auto playerLockOnFrontRightMove = playerNormalStateMachine->AddState(TEXT("Player_LockOnFrontRightMove"));
		playerLockOnFrontRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_rightfront")));

		auto playerLockOnBackLeftMove = playerNormalStateMachine->AddState(TEXT("Player_LockOnBackLeftMove"));
		playerLockOnBackLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_leftback")));

		auto playerLockOnBackRightMove = playerNormalStateMachine->AddState(TEXT("Player_LockOnBackRightMove"));
		playerLockOnBackRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_rightback")));

		auto playerLockOnLeftRun = playerNormalStateMachine->AddState(TEXT("Player_LockOnLeftRun"));
		playerLockOnLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_left")));

		auto playerLockOnRightRun = playerNormalStateMachine->AddState(TEXT("Player_LockOnRightRun"));
		playerLockOnRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_right")));

		auto playerLockOnFrontRun = playerNormalStateMachine->AddState(TEXT("Player_LockOnFrontRun"));
		playerLockOnFrontRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_front")));

		auto playerLockOnBackRun = playerNormalStateMachine->AddState(TEXT("Player_LockOnBackRun"));
		playerLockOnBackRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_back")));

		auto playerLockOnFrontLeftRun = playerNormalStateMachine->AddState(TEXT("Player_LockOnFrontLeftRun"));
		playerLockOnFrontLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_leftfront")));

		auto playerLockOnFrontRightRun = playerNormalStateMachine->AddState(TEXT("Player_LockOnFrontRightRun"));
		playerLockOnFrontRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_rightfront")));

		auto playerLockOnBackLeftRun = playerNormalStateMachine->AddState(TEXT("Player_LockOnBackLeftRun"));
		playerLockOnBackLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_leftback")));

		auto playerLockOnBackRightRun = playerNormalStateMachine->AddState(TEXT("Player_LockOnBackRightRun"));
		playerLockOnBackRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_rightback")));

		// Hit
		auto playerHit1 = playerNormalStateMachine->AddState(TEXT("Player_Hit1"));
		auto playerHit1Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_hit_1"));
		playerHit1Clip->SetIsUseEventInTransition(false);
		playerHit1->SetAnimationClip(playerHit1Clip);

		auto playerHit2 = playerNormalStateMachine->AddState(TEXT("Player_Hit2"));
		auto playerHit2Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_hit_2"));
		playerHit2Clip->SetIsUseEventInTransition(false);
		playerHit2->SetAnimationClip(playerHit2Clip);

		auto playerHeavyHit = playerNormalStateMachine->AddState(TEXT("Player_HeavyHit"));			// 강공격
		auto playerHit3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_heavyhit"));
		playerHit3Clip->SetIsRootMotion(true);
		playerHit3Clip->SetIsUseEventInTransition(false);
		playerHeavyHit->SetAnimationClip(playerHit3Clip);

		// Down
		auto playerDown = playerNormalStateMachine->AddState(TEXT("Player_Down"));					// 다운
		auto playerDownClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_down"));
		playerDownClip->SetIsRootMotion(true);
		playerDown->SetAnimationClip(playerDownClip);

		auto playerDownup = playerNormalStateMachine->AddState(TEXT("Player_Downup"));				// 다운 후 기상
		auto playerDownupClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_downup"));
		playerDownup->SetAnimationClip(playerDownupClip);

		// Die
		auto playerDieStart = playerNormalStateMachine->AddState(TEXT("Player_DieStart"));			// 죽음 시작
		auto playerDieStartClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_diestart"));
		playerDieStart->SetAnimationClip(playerDieStartClip);

		auto playerDie = playerNormalStateMachine->AddState(TEXT("Player_Die"));						// 죽음 계속
		auto playerDieClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_dieloop"));
		playerDie->SetAnimationClip(playerDieClip);

		// Attacks
		auto playerSword = playerNormalStateMachine->AddState(TEXT("Player_Sword"));					// 기본 소드 공격
		playerSword->SetSpeedParameterActive(true);
		playerSword->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerSwordClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_sword"));
		playerSwordClip->SetIsRootMotion(true);
		playerSword->SetAnimationClip(playerSwordClip);

		auto playerSwordCombo1_2 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo1_2"));
		playerSwordCombo1_2->SetSpeedParameterActive(true);
		playerSwordCombo1_2->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerSwordCombo1_2Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo1_2"));
		playerSwordCombo1_2Clip->SetIsRootMotion(true);
		playerSwordCombo1_2->SetAnimationClip(playerSwordCombo1_2Clip);

		auto playerSwordCombo1_3 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo1_3"));
		playerSwordCombo1_3->SetSpeedParameterActive(true);
		playerSwordCombo1_3->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerSwordCombo1_3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo1_3"));
		playerSwordCombo1_3Clip->SetIsRootMotion(true);
		playerSwordCombo1_3->SetAnimationClip(playerSwordCombo1_3Clip);

		auto playerSwordCombo2_3 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo2_3"));
		playerSwordCombo2_3->SetSpeedParameterActive(true);
		playerSwordCombo2_3->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerSwordCombo2_3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo2_3"));
		playerSwordCombo2_3Clip->SetIsRootMotion(true);
		playerSwordCombo2_3->SetAnimationClip(playerSwordCombo2_3Clip);

		auto playerSwordCombo2_4 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo2_4"));
		playerSwordCombo2_4->SetSpeedParameterActive(true);
		playerSwordCombo2_4->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerSwordCombo2_4Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo2_4"));
		playerSwordCombo2_4Clip->SetIsRootMotion(true);
		playerSwordCombo2_4->SetAnimationClip(playerSwordCombo2_4Clip);

		auto playerSwordCombo3_4 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo3_4"));
		auto playerSwordCombo3_4Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo3_4"));
		playerSwordCombo3_4Clip->SetIsRootMotion(true);
		playerSwordCombo3_4->SetAnimationClip(playerSwordCombo3_4Clip);

		auto playerFist = playerNormalStateMachine->AddState(TEXT("Player_Fist"));										// 기본 펀치 공격
		playerFist->SetSpeedParameterActive(true);
		playerFist->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerFistClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_fist"));
		playerFistClip->SetIsRootMotion(true);
		playerFist->SetAnimationClip(playerFistClip);

		auto playerFistCombo1_2 = playerNormalStateMachine->AddState(TEXT("Player_FistCombo1_2"));
		playerFistCombo1_2->SetSpeedParameter(TEXT("AnimationSpeed"));
		playerFistCombo1_2->SetSpeedParameterActive(true);
		auto playerFistCombo1_2Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_noraml_fistcombo1_2"));
		playerFistCombo1_2Clip->SetIsRootMotion(true);
		playerFistCombo1_2->SetAnimationClip(playerFistCombo1_2Clip);

		auto playerFistCombo1_3 = playerNormalStateMachine->AddState(TEXT("Player_FistCombo1_3"));
		playerFistCombo1_3->SetSpeedParameterActive(true);
		playerFistCombo1_3->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerFistCombo1_3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_fistcombo1_3"));
		playerFistCombo1_3Clip->SetIsRootMotion(true);
		playerFistCombo1_3->SetAnimationClip(playerFistCombo1_3Clip);

		auto playerFistCombo2_3 = playerNormalStateMachine->AddState(TEXT("Player_FistCombo2_3"));
		playerFistCombo2_3->SetSpeedParameter(TEXT("AnimationSpeed"));
		playerFistCombo2_3->SetSpeedParameterActive(true);
		auto playerFistCombo2_3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_fistcombo2_3"));
		playerFistCombo2_3Clip->SetIsRootMotion(true);
		playerFistCombo2_3->SetAnimationClip(playerFistCombo2_3Clip);

		auto playerFistCombo2_4 = playerNormalStateMachine->AddState(TEXT("Player_FistCombo2_4"));
		playerFistCombo2_4->SetSpeedParameterActive(true);
		playerFistCombo2_4->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerFistCombo2_4Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_fistcombo2_4"));
		playerFistCombo2_4Clip->SetIsRootMotion(true);
		playerFistCombo2_4->SetAnimationClip(playerFistCombo2_4Clip);

		// Enter overdrive
		auto playerOverdriveSwordEnter = playerNormalStateMachine->AddState(TEXT("Player_OverdriveSwordEnter"));
		auto playerOverdriveSwordEnterClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_enter"));
		playerOverdriveSwordEnter->SetAnimationClip(playerOverdriveSwordEnterClip);

		auto playerOverdriveFistEnter = playerNormalStateMachine->AddState(TEXT("Player_OverdriveFistEnter"));
		auto playerOverdriveFistEnterClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_enter"));
		playerOverdriveFistEnter->SetAnimationClip(playerOverdriveFistEnterClip);

#pragma region SWORD_ANIMATION_EVENT
		DUOLGameEngine::AnimationEvent swordEvent;

		swordEvent._eventName = TEXT("StartCancleFrame");

		swordEvent._targetFrame = 10.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("StartCancleFrame");

		swordEvent._targetFrame = 36.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("StartCancleFrame");

		swordEvent._targetFrame = 82.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("StartSlowFrame");

		swordEvent._targetFrame = 20.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("StartSlowFrame");

		swordEvent._targetFrame = 44.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("StartSlowFrame");

		swordEvent._targetFrame = 100.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndCancleFrame");

		swordEvent._targetFrame = 28.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndCancleFrame");

		swordEvent._targetFrame = 55.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndCancleFrame");

		swordEvent._targetFrame = 109.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndAttack");

		swordEvent._targetFrame = 226.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("WaveHit");

		swordEvent._targetFrame = 153.f;

		playerSwordClip->AddEvent(swordEvent);

		// 타격 프레임
		swordEvent._eventName = TEXT("StartSwordAttackFrame");

		swordEvent._targetFrame = 10.f;

		playerSwordClip->AddEvent(swordEvent);
		
		swordEvent._targetFrame = 33.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 93.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndSwordAttackFrame");

		swordEvent._targetFrame = 17.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 41.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 104.f;

		playerSwordClip->AddEvent(swordEvent);

		// 기본 상태 소드 공격 이펙트 프레임
		swordEvent._eventName = TEXT("StartSwordTrailFrame");

		swordEvent._targetFrame = 6.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 26.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 81.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 149.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndSwordTrailFrame");

		swordEvent._targetFrame = 21.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 41.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 101.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 153.f;

		playerSwordClip->AddEvent(swordEvent);
#pragma endregion

#pragma region FIST_ANIMATION_EVENT
		DUOLGameEngine::AnimationEvent fistEvent;

		fistEvent._eventName = TEXT("StartCancleFrame");

		fistEvent._targetFrame = 8.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("StartCancleFrame");

		fistEvent._targetFrame = 30.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("StartSlowFrame");

		fistEvent._targetFrame = 15.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("StartSlowFrame");

		fistEvent._targetFrame = 38.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("EndCancleFrame");

		fistEvent._targetFrame = 24.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("EndCancleFrame");

		fistEvent._targetFrame = 47.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("EndAttack");

		fistEvent._targetFrame = 110.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("FistHit");

		fistEvent._targetFrame = 13.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._targetFrame = 36.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("WaveHit");

		fistEvent._targetFrame = 65.f;

		playerFistClip->AddEvent(fistEvent);
#pragma endregion

#pragma region SWORD_COMBO_ANIMATION_EVENT
		DUOLGameEngine::AnimationEvent swordComboEvent;

		swordComboEvent._eventName = TEXT("FistHit");

		swordComboEvent._targetFrame = 13.f;

		playerSwordCombo1_2Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("StartCancleFrame");

		swordComboEvent._targetFrame = 13.f;

		playerSwordCombo1_2Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("StartSlowFrame");

		swordComboEvent._targetFrame = 22.f;

		playerSwordCombo1_2Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndCancleFrame");

		swordComboEvent._targetFrame = 31.f;

		playerSwordCombo1_2Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("WaveHit");

		swordComboEvent._targetFrame = 15.f;

		playerSwordCombo1_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndAttack");

		swordComboEvent._targetFrame = 61.f;

		playerSwordCombo1_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("FistHit");

		swordComboEvent._targetFrame = 35.f;

		playerSwordCombo2_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("StartCancleFrame");

		swordComboEvent._targetFrame = 38.f;

		playerSwordCombo2_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("StartSlowFrame");

		swordComboEvent._targetFrame = 48.f;

		playerSwordCombo2_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndCancleFrame");

		swordComboEvent._targetFrame = 61.f;

		playerSwordCombo2_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("WaveHit");

		swordComboEvent._targetFrame = 47.f;

		playerSwordCombo2_4Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndAttack");

		swordComboEvent._targetFrame = 98.f;

		playerSwordCombo2_4Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("WaveHit");

		swordComboEvent._targetFrame = 56.f;

		playerSwordCombo3_4Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndAttack");

		swordComboEvent._targetFrame = 110.f;

		playerSwordCombo3_4Clip->AddEvent(swordComboEvent);
#pragma endregion

#pragma region FIST_COMBO_ANIMATION_EVENT
		AnimationEvent fistComboEvent;

		fistComboEvent._eventName = TEXT("StartCancleFrame");

		fistComboEvent._targetFrame = 17.f;

		playerFistCombo1_2Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("StartSlowFrame");

		fistComboEvent._targetFrame = 32.f;

		playerFistCombo1_2Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("EndCancleFrame");

		fistComboEvent._targetFrame = 42.f;

		playerFistCombo1_2Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("WaveHit");

		fistComboEvent._targetFrame = 23.f;

		playerFistCombo1_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("EndAttack");

		fistComboEvent._targetFrame = 36.f;

		playerFistCombo1_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("StartCancleFrame");

		fistComboEvent._targetFrame = 10.f;

		playerFistCombo2_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("StartSlowFrame");

		fistComboEvent._targetFrame = 20.f;

		playerFistCombo2_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("EndCancleFrame");

		fistComboEvent._targetFrame = 28.f;

		playerFistCombo2_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("WaveHit");

		fistComboEvent._targetFrame = 54.f;

		playerFistCombo2_4Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("EndAttack");

		fistComboEvent._targetFrame = 104.f;

		playerFistCombo2_4Clip->AddEvent(fistComboEvent);

		// 타격 프레임
		fistComboEvent._eventName = TEXT("StartSwordAttackFrame");

		fistComboEvent._targetFrame = 23.f;

		playerFistCombo1_2Clip->AddEvent(fistComboEvent);

		fistComboEvent._targetFrame = 16.f;

		playerFistCombo2_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("EndSwordAttackFrame");

		fistComboEvent._targetFrame = 29.f;

		playerFistCombo1_2Clip->AddEvent(fistComboEvent);

		fistComboEvent._targetFrame = 22.f;

		playerFistCombo2_3Clip->AddEvent(fistComboEvent);

		// 이펙트 프레임
		fistComboEvent._eventName = TEXT("StartSwordTrailFrame");

		fistComboEvent._targetFrame = 23.f;

		playerFistCombo1_2Clip->AddEvent(fistComboEvent);

		fistComboEvent._targetFrame = 20.f;

		playerFistCombo1_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._targetFrame = 14.f;

		playerFistCombo2_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._targetFrame = 49.f;

		playerFistCombo2_4Clip->AddEvent(fistComboEvent);

		fistComboEvent._eventName = TEXT("EndSwordTrailFrame");

		fistComboEvent._targetFrame = 29.f;

		playerFistCombo1_2Clip->AddEvent(fistComboEvent);

		fistComboEvent._targetFrame = 27.f;

		playerFistCombo1_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._targetFrame = 22.f;

		playerFistCombo2_3Clip->AddEvent(fistComboEvent);

		fistComboEvent._targetFrame = 56.f;

		playerFistCombo2_4Clip->AddEvent(fistComboEvent);
#pragma endregion

#pragma region DASH_ANIMATION_EVENT
		DUOLGameEngine::AnimationEvent dashEvent;

		dashEvent._eventName = TEXT("StartDash");

		dashEvent._targetFrame = 1.f;

		playerDashClip->AddEvent(dashEvent);

		dashEvent._eventName = TEXT("EndDash");

		dashEvent._targetFrame = 26.f;

		playerDashClip->AddEvent(dashEvent);
#pragma endregion

#pragma region HIT_EVENT
		AnimationEvent hitEvent;

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 40.f;

		playerHit1Clip->AddEvent(hitEvent);

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 40.f;

		playerHit2Clip->AddEvent(hitEvent);

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 50.f;

		playerHit3Clip->AddEvent(hitEvent);
#pragma endregion

#pragma region DOWN_EVENT
		AnimationEvent downEvent;

		downEvent._eventName = TEXT("EndDown");

		downEvent._targetFrame = 64.f;

		playerDownClip->AddEvent(downEvent);

		downEvent._eventName = TEXT("EndDownup");

		downEvent._targetFrame = 64.f;

		playerDownupClip->AddEvent(downEvent);
#pragma endregion

#pragma region DIE_EVENT
		AnimationEvent dieEvent;

		dieEvent._eventName = TEXT("EndDieStart");

		dieEvent._targetFrame = 53.f;

		playerDieStartClip->AddEvent(dieEvent);
#pragma endregion

#pragma region OVERDRIVE_ENTER_EVENT
		AnimationEvent enterEvent;

		enterEvent._eventName = TEXT("EquipOverdriveWeaponSword");

		enterEvent._targetFrame = 0.f;

		playerOverdriveSwordEnterClip->AddEvent(enterEvent);

		enterEvent._eventName = TEXT("EndOverdriveEnter");

		enterEvent._targetFrame = 85.f;

		playerOverdriveSwordEnterClip->AddEvent(enterEvent);

		enterEvent._targetFrame = 75.f;

		playerOverdriveFistEnterClip->AddEvent(enterEvent);
#pragma endregion

#pragma region DIE_START_END
		auto playerIdleToDieStart = playerIdle->AddTransition(playerDieStart);
		playerIdleToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerMoveToDieStart = playerMove->AddTransition(playerDieStart);
		playerMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerRunToDieStart = playerRun->AddTransition(playerDieStart);
		playerRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerLockOnLeftMoveToDieStart = playerLockOnLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightMoveToDieStart = playerLockOnRightMove->AddTransition(playerDieStart);
		playerLockOnRightMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnFrontMoveToDieStart = playerLockOnFrontMove->AddTransition(playerDieStart);
		playerLockOnFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnBackMoveToDieStart = playerLockOnBackMove->AddTransition(playerDieStart);
		playerLockOnBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftFrontMoveToDieStart = playerLockOnFrontLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftBackMoveToDieStart = playerLockOnBackLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightFrontMoveToDieStart = playerLockOnFrontRightMove->AddTransition(playerDieStart);
		playerLockOnRightFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightBackMoveToDieStart = playerLockOnFrontRightMove->AddTransition(playerDieStart);
		playerLockOnRightBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerLockOnLeftRunToDieStart = playerLockOnLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightRunToDieStart = playerLockOnRightRun->AddTransition(playerDieStart);
		playerLockOnRightRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnFrontRunToDieStart = playerLockOnFrontRun->AddTransition(playerDieStart);
		playerLockOnFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnBackRunToDieStart = playerLockOnBackRun->AddTransition(playerDieStart);
		playerLockOnBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftFrontRunToDieStart = playerLockOnFrontLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftBackRunToDieStart = playerLockOnBackLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightFrontRunToDieStart = playerLockOnFrontRightRun->AddTransition(playerDieStart);
		playerLockOnRightFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightBackRunToDieStart = playerLockOnFrontRightRun->AddTransition(playerDieStart);
		playerLockOnRightBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerSwordToDieStart = playerSword->AddTransition(playerDieStart);
		playerSwordToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerFistToDieStart = playerFist->AddTransition(playerDieStart);
		playerFistToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerSwordCombo1_2ToDieStart = playerSwordCombo1_2->AddTransition(playerDieStart);
		playerSwordCombo1_2ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerSwordCombo1_3ToDieStart = playerSwordCombo1_3->AddTransition(playerDieStart);
		playerSwordCombo1_3ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerSwordCombo2_3ToDieStart = playerSwordCombo2_3->AddTransition(playerDieStart);
		playerSwordCombo2_3ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerSwordCombo2_4ToDieStart = playerSwordCombo2_4->AddTransition(playerDieStart);
		playerSwordCombo2_4ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerSwordCombo3_4ToDieStart = playerSwordCombo3_4->AddTransition(playerDieStart);
		playerSwordCombo3_4ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerFistCombo1_2ToDieStart = playerFistCombo1_2->AddTransition(playerDieStart);
		playerFistCombo1_2ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerFistCombo1_3ToDieStart = playerFistCombo1_3->AddTransition(playerDieStart);
		playerFistCombo1_3ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerFistCombo2_3ToDieStart = playerFistCombo2_3->AddTransition(playerDieStart);
		playerFistCombo2_3ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerFistCombo2_4ToDieStart = playerFistCombo2_4->AddTransition(playerDieStart);
		playerFistCombo2_4ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHit1ToDieStart = playerHit1->AddTransition(playerDieStart);
		playerHit1ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHit2ToDieStart = playerHit2->AddTransition(playerDieStart);
		playerHit2ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHeavyHitToDieStart = playerHeavyHit->AddTransition(playerDieStart);
		playerHeavyHitToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerDieStartToDie = playerDieStart->AddTransition(playerDie);
		playerDieStartToDie->SetTransitionDuration(0.01f);
		playerDieStartToDie->AddCondition(TEXT("IsDie"), AnimatorConditionMode::False);					// 최종 죽음
#pragma endregion

#pragma region DASH_START
		auto playerIdleToDash = playerIdle->AddTransition(playerDash);
		playerIdleToDash->SetTransitionDuration(0.005f);
		playerIdleToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerMoveToDash = playerMove->AddTransition(playerDash);
		playerMoveToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerRunToDash = playerRun->AddTransition(playerDash);
		playerRunToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordToDash = playerSword->AddTransition(playerDash);
		playerSwordToDash->SetTransitionDuration(0.01f);
		playerSwordToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerFistToDash = playerFist->AddTransition(playerDash);
		playerFistToDash->SetTransitionDuration(0.01f);
		playerFistToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo1_2ToDash = playerSwordCombo1_2->AddTransition(playerDash);
		playerSwordCombo1_2ToDash->SetTransitionDuration(0.01f);
		playerSwordCombo1_2ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo1_3ToDash = playerSwordCombo1_3->AddTransition(playerDash);
		playerSwordCombo1_3ToDash->SetTransitionDuration(0.01f);
		playerSwordCombo1_3ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo2_3ToDash = playerSwordCombo2_3->AddTransition(playerDash);
		playerSwordCombo2_3ToDash->SetTransitionDuration(0.01f);
		playerSwordCombo2_3ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo2_4ToDash = playerSwordCombo2_4->AddTransition(playerDash);
		playerSwordCombo2_4ToDash->SetTransitionDuration(0.01f);
		playerSwordCombo2_4ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo3_4ToDash = playerSwordCombo3_4->AddTransition(playerDash);
		playerSwordCombo3_4ToDash->SetTransitionDuration(0.01f);
		playerSwordCombo3_4ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerFistCombo1_2ToDash = playerFistCombo1_2->AddTransition(playerDash);
		playerFistCombo1_2ToDash->SetTransitionDuration(0.01f);
		playerFistCombo1_2ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerFistCombo1_3ToDash = playerFistCombo1_3->AddTransition(playerDash);
		playerFistCombo1_3ToDash->SetTransitionDuration(0.01f);
		playerFistCombo1_3ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerFistCombo2_3ToDash = playerFistCombo2_3->AddTransition(playerDash);
		playerFistCombo2_3ToDash->SetTransitionDuration(0.01f);
		playerFistCombo2_3ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerFistCombo2_4ToDash = playerFistCombo2_4->AddTransition(playerDash);
		playerFistCombo2_4ToDash->SetTransitionDuration(0.01f);
		playerFistCombo2_4ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DASH_END
		auto playerDashToIdle = playerDash->AddTransition(playerIdle);
		playerDashToIdle->SetTransitionDuration(0.2f);
		playerDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		auto playerDashToMove = playerDash->AddTransition(playerMove);
		playerDashToMove->SetTransitionDuration(0.2f);
		playerDashToMove->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		playerDashToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		auto playerDashToRun = playerDash->AddTransition(playerRun);
		playerDashToRun->SetTransitionDuration(0.2f);
		playerDashToRun->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		playerDashToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DOWN_START_END
		auto playerIdleToDown = playerIdle->AddTransition(playerDown);
		playerIdleToDown->SetTransitionDuration(0.0001f);
		playerIdleToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerMoveToDown = playerMove->AddTransition(playerDown);
		playerMoveToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerRunToDown = playerRun->AddTransition(playerDown);
		playerRunToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerSwordToDown = playerSword->AddTransition(playerDown);
		playerSwordToDown->SetTransitionDuration(0.01f);									// 공격이 기니까 ..
		playerSwordToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerFistToDown = playerFist->AddTransition(playerDown);
		playerFistToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerSwordCombo1_2ToDown = playerSwordCombo1_2->AddTransition(playerDown);
		playerSwordCombo1_2ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);
		auto playerSwordCombo1_3ToDown = playerSwordCombo1_3->AddTransition(playerDown);
		playerSwordCombo1_3ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);
		auto playerSwordCombo2_3ToDown = playerSwordCombo2_3->AddTransition(playerDown);
		playerSwordCombo2_3ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);
		auto playerSwordCombo2_4ToDown = playerSwordCombo2_4->AddTransition(playerDown);
		playerSwordCombo2_4ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);
		auto playerSwordCombo3_4ToDown = playerSwordCombo3_4->AddTransition(playerDown);
		playerSwordCombo3_4ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerFistCombo1_2ToDown = playerFistCombo1_2->AddTransition(playerDown);
		playerFistCombo1_2ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);
		auto playerFistCombo1_3ToDown = playerFistCombo1_3->AddTransition(playerDown);
		playerFistCombo1_3ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);
		auto playerFistCombo2_3ToDown = playerFistCombo2_3->AddTransition(playerDown);
		playerFistCombo2_3ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);
		auto playerFistCombo2_4ToDown = playerFistCombo2_4->AddTransition(playerDown);
		playerFistCombo2_4ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHit1ToDown = playerHit1->AddTransition(playerDown);
		playerHit1ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHit2ToDown = playerHit2->AddTransition(playerDown);
		playerHit2ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHeavyHitToDown = playerHeavyHit->AddTransition(playerDown);
		playerHeavyHitToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		// 애니메이션 시간이 다 끝날 때 Transition 하면 됩니다 ..!
		auto playerDownToDownup = playerDown->AddTransition(playerDownup);
		playerDownToDownup->AddCondition(TEXT("IsDown"), AnimatorConditionMode::False);
		playerDownToDownup->AddCondition(TEXT("IsDownup"), AnimatorConditionMode::True);

		auto playerDownupToIdle = playerDownup->AddTransition(playerIdle);
		playerDownupToIdle->SetTransitionDuration(0.05f);
		playerDownupToIdle->AddCondition(TEXT("IsDownup"), AnimatorConditionMode::False);
#pragma endregion

#pragma region HIT_START
		auto playerIdleToHit1 = playerIdle->AddTransition(playerHit1);
		playerIdleToHit1->SetTransitionDuration(0.01f);
		playerIdleToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerIdleToHit2 = playerIdle->AddTransition(playerHit2);
		playerIdleToHit2->SetTransitionDuration(0.01f);
		playerIdleToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerIdleToHit3 = playerIdle->AddTransition(playerHeavyHit);
		playerIdleToHit3->SetTransitionDuration(0.01f);
		playerIdleToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerMoveToHit1 = playerMove->AddTransition(playerHit1);
		playerMoveToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerMoveToHit2 = playerMove->AddTransition(playerHit2);
		playerMoveToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerMoveToHit3 = playerMove->AddTransition(playerHeavyHit);
		playerMoveToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerRunToHit1 = playerRun->AddTransition(playerHit1);
		playerRunToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerRunToHit2 = playerRun->AddTransition(playerHit2);
		playerRunToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerRunToHit3 = playerRun->AddTransition(playerHeavyHit);
		playerRunToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerSwordToHit1 = playerSword->AddTransition(playerHit1);
		playerSwordToHit1->SetTransitionDuration(0.005f);
		playerSwordToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordToHit2 = playerSword->AddTransition(playerHit2);
		playerSwordToHit2->SetTransitionDuration(0.005f);
		playerSwordToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordToHit3 = playerSword->AddTransition(playerHeavyHit);
		playerSwordToHit3->SetTransitionDuration(0.005f);
		playerSwordToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerFistToHit1 = playerFist->AddTransition(playerHit1);
		playerFistToHit1->SetTransitionDuration(0.005f);
		playerFistToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerFistToHit2 = playerFist->AddTransition(playerHit2);
		playerFistToHit2->SetTransitionDuration(0.005f);
		playerFistToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerFistToHit3 = playerFist->AddTransition(playerHeavyHit);
		playerFistToHit3->SetTransitionDuration(0.005f);
		playerFistToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerSwordCombo1_2ToHit1 = playerSwordCombo1_2->AddTransition(playerHit1);
		playerSwordCombo1_2ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordCombo1_3ToHit1 = playerSwordCombo1_3->AddTransition(playerHit1);
		playerSwordCombo1_3ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordCombo1_2ToHit2 = playerSwordCombo1_2->AddTransition(playerHit2);
		playerSwordCombo1_2ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordCombo1_3ToHit2 = playerSwordCombo1_3->AddTransition(playerHit2);
		playerSwordCombo1_3ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordCombo1_2ToHit3 = playerSwordCombo1_2->AddTransition(playerHeavyHit);
		playerSwordCombo1_2ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerSwordCombo1_3ToHit3 = playerSwordCombo1_3->AddTransition(playerHeavyHit);
		playerSwordCombo1_3ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerSwordCombo2_3ToHit1 = playerSwordCombo2_3->AddTransition(playerHit1);
		playerSwordCombo2_3ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordCombo2_4ToHit1 = playerSwordCombo2_4->AddTransition(playerHit1);
		playerSwordCombo2_4ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordCombo2_3ToHit2 = playerSwordCombo2_3->AddTransition(playerHit2);
		playerSwordCombo2_3ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordCombo2_4ToHit2 = playerSwordCombo2_4->AddTransition(playerHit2);
		playerSwordCombo2_4ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordCombo2_3ToHit3 = playerSwordCombo2_3->AddTransition(playerHeavyHit);
		playerSwordCombo2_3ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerSwordCombo2_4ToHit3 = playerSwordCombo2_4->AddTransition(playerHeavyHit);
		playerSwordCombo2_4ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerSwordCombo3_4ToHit1 = playerSwordCombo3_4->AddTransition(playerHit1);
		playerSwordCombo3_4ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordCombo3_4ToHit2 = playerSwordCombo3_4->AddTransition(playerHit2);
		playerSwordCombo3_4ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordCombo3_4ToHit3 = playerSwordCombo3_4->AddTransition(playerHeavyHit);
		playerSwordCombo3_4ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerFistCombo2_3ToHit1 = playerFistCombo2_3->AddTransition(playerHit1);
		playerFistCombo2_3ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerFistCombo2_4ToHit1 = playerFistCombo2_4->AddTransition(playerHit1);
		playerFistCombo2_4ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerFistCombo2_3ToHit2 = playerFistCombo2_3->AddTransition(playerHit2);
		playerFistCombo2_3ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerFistCombo2_4ToHit2 = playerFistCombo2_4->AddTransition(playerHit2);
		playerFistCombo2_4ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerFistCombo2_3ToHit3 = playerFistCombo2_3->AddTransition(playerHeavyHit);
		playerFistCombo2_3ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerFistCombo2_4ToHit3 = playerFistCombo2_4->AddTransition(playerHeavyHit);
		playerFistCombo2_4ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit1ToHit2 = playerHit1->AddTransition(playerHit2);
		playerHit1ToHit2->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerHit1ToHit3 = playerHit1->AddTransition(playerHeavyHit);
		playerHit1ToHit3->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit2ToHit1 = playerHit2->AddTransition(playerHit1);
		playerHit2ToHit1->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerHit2ToHit3 = playerHit2->AddTransition(playerHeavyHit);
		playerHit2ToHit3->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit3ToHit1 = playerHeavyHit->AddTransition(playerHit1);
		playerHit3ToHit1->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
		playerHit3ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerHit3ToHit2 = playerHeavyHit->AddTransition(playerHit2);
		playerHit3ToHit2->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
		playerHit3ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);
#pragma endregion

#pragma region HIT_END
		auto playerHit1ToIdle = playerHit1->AddTransition(playerIdle);
		playerHit1ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit1ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);

		auto playerHit2ToIdle = playerHit2->AddTransition(playerIdle);
		playerHit2ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit2ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);

		auto playerHit3ToIdle = playerHeavyHit->AddTransition(playerIdle);
		playerHit3ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit3ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit3ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
#pragma endregion

#pragma region OVERDRIVE_ENTER
		auto playerIdleToOverdriveSwordEnter = playerIdle->AddTransition(playerOverdriveSwordEnter);
		playerIdleToOverdriveSwordEnter->AddCondition(TEXT("IsOverdriveSwordEnter"), AnimatorConditionMode::True);

		auto playerMoveToOverdriveSwordEnter = playerMove->AddTransition(playerOverdriveSwordEnter);
		playerMoveToOverdriveSwordEnter->AddCondition(TEXT("IsOverdriveSwordEnter"), AnimatorConditionMode::True);

		auto playerRunToOverdriveSwordEnter = playerRun->AddTransition(playerOverdriveSwordEnter);
		playerRunToOverdriveSwordEnter->AddCondition(TEXT("IsOverdriveSwordEnter"), AnimatorConditionMode::True);

		auto playerIdleToOverdriveFistEnter = playerIdle->AddTransition(playerOverdriveFistEnter);
		playerIdleToOverdriveFistEnter->SetTransitionDuration(0.05f);
		playerIdleToOverdriveFistEnter->AddCondition(TEXT("IsOverdriveFistEnter"), AnimatorConditionMode::True);

		auto playerMoveToOverdriveFistEnter = playerMove->AddTransition(playerOverdriveFistEnter);
		playerMoveToOverdriveFistEnter->AddCondition(TEXT("IsOverdriveFistEnter"), AnimatorConditionMode::True);

		auto playerRunToOverdriveFistEnter = playerRun->AddTransition(playerOverdriveFistEnter);
		playerRunToOverdriveFistEnter->AddCondition(TEXT("IsOverdriveFistEnter"), AnimatorConditionMode::True);
#pragma endregion

#pragma region ATTACK_START
		auto playerIdleToSword = playerIdle->AddTransition(playerSword);
		playerIdleToSword->SetTransitionDuration(0.01f);
		playerIdleToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerIdleToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);

		auto playerIdleToFist = playerIdle->AddTransition(playerFist);
		playerIdleToFist->SetTransitionDuration(0.01f);
		playerIdleToFist->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerIdleToFist->AddCondition(TEXT("IsFist"), AnimatorConditionMode::True);

		auto playerMoveToSword = playerMove->AddTransition(playerSword);
		playerMoveToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerMoveToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);

		auto playerMoveToFist = playerMove->AddTransition(playerFist);
		playerMoveToFist->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerMoveToFist->AddCondition(TEXT("IsFist"), AnimatorConditionMode::True);

		auto playerRunToSword = playerRun->AddTransition(playerSword);
		playerRunToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerRunToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);

		auto playerRunToFist = playerRun->AddTransition(playerFist);
		playerRunToFist->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerRunToFist->AddCondition(TEXT("IsFist"), AnimatorConditionMode::True);
#pragma endregion

#pragma region ATTACK_TO_ATTACK
		auto playerSwordToSwordCombo1_2 = playerSword->AddTransition(playerSwordCombo1_2);
		playerSwordToSwordCombo1_2->SetTransitionDuration(0.01f);
		playerSwordToSwordCombo1_2->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerSwordToSwordCombo1_2->AddCondition(TEXT("IsSword"), AnimatorConditionMode::False);
		playerSwordToSwordCombo1_2->AddCondition(TEXT("IsFist"), AnimatorConditionMode::True);
		playerSwordToSwordCombo1_2->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 2);

		auto playerSwordCombo1_2ToSwordCombo1_3 = playerSwordCombo1_2->AddTransition(playerSwordCombo1_3);
		playerSwordCombo1_2ToSwordCombo1_3->SetTransitionOffset(0.1267f);				// 시작 프레임 다르게 요청
		playerSwordCombo1_2ToSwordCombo1_3->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerSwordCombo1_2ToSwordCombo1_3->AddCondition(TEXT("IsFist"), AnimatorConditionMode::True);
		playerSwordCombo1_2ToSwordCombo1_3->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 3);

		auto playerSwordToSwordCombo2_3 = playerSword->AddTransition(playerSwordCombo2_3);
		playerSwordToSwordCombo2_3->SetTransitionDuration(0.01f);
		playerSwordToSwordCombo2_3->SetTransitionOffset(0.2424f);						// 시작 프레임 다르게 요청
		playerSwordToSwordCombo2_3->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerSwordToSwordCombo2_3->AddCondition(TEXT("IsSword"), AnimatorConditionMode::False);
		playerSwordToSwordCombo2_3->AddCondition(TEXT("IsFist"), AnimatorConditionMode::True);
		playerSwordToSwordCombo2_3->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 3);

		auto playerSwordCombo2_3ToSwordCombo2_4 = playerSwordCombo2_3->AddTransition(playerSwordCombo2_4);
		playerSwordCombo2_3ToSwordCombo2_4->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerSwordCombo2_3ToSwordCombo2_4->AddCondition(TEXT("IsFist"), AnimatorConditionMode::True);
		playerSwordCombo2_3ToSwordCombo2_4->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 4);

		auto playerSwordToSwordCombo3_4 = playerSword->AddTransition(playerSwordCombo3_4);
		playerSwordToSwordCombo3_4->SetTransitionDuration(0.01f);
		playerSwordToSwordCombo3_4->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerSwordToSwordCombo3_4->AddCondition(TEXT("IsSword"), AnimatorConditionMode::False);
		playerSwordToSwordCombo3_4->AddCondition(TEXT("IsFist"), AnimatorConditionMode::True);
		playerSwordToSwordCombo3_4->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 4);

		auto playerFistToFistCombo1_2 = playerFist->AddTransition(playerFistCombo1_2);
		playerFistToFistCombo1_2->SetTransitionDuration(0.01f);
		playerFistToFistCombo1_2->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerFistToFistCombo1_2->AddCondition(TEXT("IsFist"), AnimatorConditionMode::False);
		playerFistToFistCombo1_2->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);
		playerFistToFistCombo1_2->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 2);

		auto playerFistCombo1_2ToFistCombo1_3 = playerFistCombo1_2->AddTransition(playerFistCombo1_3);
		playerFistCombo1_2ToFistCombo1_3->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerFistCombo1_2ToFistCombo1_3->AddCondition(TEXT("IsFist"), AnimatorConditionMode::False);
		playerFistCombo1_2ToFistCombo1_3->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);
		playerFistCombo1_2ToFistCombo1_3->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 3);


		auto playerFistToFistCombo2_3 = playerFist->AddTransition(playerFistCombo2_3);
		playerFistToFistCombo2_3->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerFistToFistCombo2_3->AddCondition(TEXT("IsFist"), AnimatorConditionMode::False);
		playerFistToFistCombo2_3->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);
		playerFistToFistCombo2_3->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 3);

		auto playerFistCombo2_3ToFistCombo2_4 = playerFistCombo2_3->AddTransition(playerFistCombo2_4);
		playerFistCombo2_3ToFistCombo2_4->SetTransitionDuration(0.01f);
		playerFistCombo2_3ToFistCombo2_4->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerFistCombo2_3ToFistCombo2_4->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);
		playerFistCombo2_3ToFistCombo2_4->AddCondition(TEXT("AttackCount"), AnimatorConditionMode::Equals, 4);
#pragma endregion

#pragma region ATTACK_END
		auto playerSwordToIdle = playerSword->AddTransition(playerIdle);
		playerSwordToIdle->SetTransitionDuration(0.01f);
		playerSwordToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerFistToIdle = playerFist->AddTransition(playerIdle);
		playerFistToIdle->SetTransitionDuration(0.05f);
		playerFistToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerSwordCombo1_2ToIdle = playerSwordCombo1_2->AddTransition(playerIdle);
		playerSwordCombo1_2ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerSwordCombo1_3ToIdle = playerSwordCombo1_3->AddTransition(playerIdle);
		playerSwordCombo1_3ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerSwordCombo2_3ToIdle = playerSwordCombo2_3->AddTransition(playerIdle);
		playerSwordCombo2_3ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerSwordCombo2_4ToIdle = playerSwordCombo2_4->AddTransition(playerIdle);
		playerSwordCombo2_4ToIdle->SetTransitionDuration(0.05f);
		playerSwordCombo2_4ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerSwordCombo3_4ToIdle = playerSwordCombo3_4->AddTransition(playerIdle);
		playerSwordCombo3_4ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerFistCombo1_2ToIdle = playerFistCombo1_2->AddTransition(playerIdle);
		playerFistCombo1_2ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerFistCombo1_3ToIdle = playerFistCombo1_3->AddTransition(playerIdle);
		playerFistCombo1_3ToIdle->SetTransitionDuration(0.35f);
		playerFistCombo1_3ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerFistCombo2_3ToIdle = playerFistCombo2_3->AddTransition(playerIdle);
		playerFistCombo2_3ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerFistCombo2_4ToIdle = playerFistCombo2_4->AddTransition(playerIdle);
		playerFistCombo2_4ToIdle->SetTransitionDuration(0.05f);
		playerFistCombo2_4ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);
#pragma endregion

#pragma region LOCK_ON_STATE_MOVEMENT
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

#pragma region MOVE_RUN_IDLE
		auto playerIdleToRun = playerIdle->AddTransition(playerRun);
		playerIdleToRun->SetTransitionDuration(0.01f);
		playerIdleToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerRunToIdle = playerRun->AddTransition(playerIdle);
		playerRunToIdle->SetTransitionDuration(0.2f);
		playerRunToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerRunToIdle->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerIdleToMove = playerIdle->AddTransition(playerMove);
		playerIdleToMove->SetTransitionDuration(0.01f);
		playerIdleToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		auto playerMoveToIdle = playerMove->AddTransition(playerIdle);
		playerMoveToIdle->SetTransitionDuration(0.2f);
		playerMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		auto playerMoveToRun = playerMove->AddTransition(playerRun);
		playerMoveToRun->SetTransitionDuration(0.3f);
		playerMoveToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerMoveToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerRunToMove = playerRun->AddTransition(playerMove);
		playerRunToMove->SetTransitionDuration(0.3f);
		playerRunToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerRunToMove->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);
#pragma endregion

		DUOLGameEngine::ResourceManager::GetInstance()->AddAnimatorController(playerNormalAnimCon);
	}

	void DUOLClient_Initializer::Overdrive_Sword_Initialize()
	{
		using namespace DUOLGameEngine;

		auto playerOverdriveSwordCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Player_Overdrive_SwordAnimatorController"));

		auto playerOverdriveSwordStateMachine = playerOverdriveSwordCon->AddStateMachine(TEXT("PlayerOverdriveSwordStateMachine"));

		// 플레이어 상태
		playerOverdriveSwordCon->AddParameter(TEXT("IsMove"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsRun"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsLockOn"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsDash"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsHit1"), AnimatorControllerParameterType::Bool);				// 약공격 1

		playerOverdriveSwordCon->AddParameter(TEXT("IsHit2"), AnimatorControllerParameterType::Bool);				// 약공격 2

		playerOverdriveSwordCon->AddParameter(TEXT("IsHit3"), AnimatorControllerParameterType::Bool);				// 약공격 3

		playerOverdriveSwordCon->AddParameter(TEXT("IsHeavyHit"), AnimatorControllerParameterType::Bool);			// 강공격

		playerOverdriveSwordCon->AddParameter(TEXT("IsDown"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsDownup"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsUltimate"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsOverdriveExit"), AnimatorControllerParameterType::Bool);

		// 방향 여부
		playerOverdriveSwordCon->AddParameter(TEXT("IsLeft"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsRight"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsFront"), AnimatorControllerParameterType::Bool);

		playerOverdriveSwordCon->AddParameter(TEXT("IsBack"), AnimatorControllerParameterType::Bool);

		// 공격 숫자
		playerOverdriveSwordCon->AddParameter(TEXT("AttackCount"), AnimatorControllerParameterType::Int);

		// 애니메이션 재생 속도 배속
		playerOverdriveSwordCon->AddParameter(TEXT("AnimationSpeed"), AnimatorControllerParameterType::Float);

		// Idle
		auto playerIdle = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Idle"));
		playerIdle->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_idle")));

		// Move
		auto playerMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Move"));
		playerMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_move")));

		// Run
		auto playerRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Run"));
		playerRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_run")));

		// Dash
		auto playerDash = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Dash"));
		auto playerDashClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_dash"));
		playerDashClip->SetIsRootMotion(true);
		playerDash->SetAnimationClip(playerDashClip);

		// Lock-On Move and Run
		auto playerLockOnLeftMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnLeftMove"));
		playerLockOnLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_left")));

		auto playerLockOnRightMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnRightMove"));
		playerLockOnRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_right")));

		auto playerLockOnFrontMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnFrontMove"));
		playerLockOnFrontMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_front")));

		auto playerLockOnBackMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnBackMove"));
		playerLockOnBackMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_back")));

		auto playerLockOnFrontLeftMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnFrontLeftMove"));
		playerLockOnFrontLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_leftfront")));

		auto playerLockOnFrontRightMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnFrontRightMove"));
		playerLockOnFrontRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_rightfront")));

		auto playerLockOnBackLeftMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnBackLeftMove"));
		playerLockOnBackLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_leftback")));

		auto playerLockOnBackRightMove = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnBackRightMove"));
		playerLockOnBackRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_rightback")));

		auto playerLockOnLeftRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnLeftRun"));
		playerLockOnLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_run_left")));

		auto playerLockOnRightRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnRightRun"));
		playerLockOnRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_run_right")));

		auto playerLockOnFrontRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnFrontRun"));
		playerLockOnFrontRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_run_front")));

		auto playerLockOnBackRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnBackRun"));
		playerLockOnBackRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_run_back")));

		auto playerLockOnFrontLeftRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnFrontLeftRun"));
		playerLockOnFrontLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_run_leftfront")));

		auto playerLockOnFrontRightRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnFrontRightRun"));
		playerLockOnFrontRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_run_rightfront")));

		auto playerLockOnBackLeftRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnBackLeftRun"));
		playerLockOnBackLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_run_leftback")));

		auto playerLockOnBackRightRun = playerOverdriveSwordStateMachine->AddState(TEXT("Player_LockOnBackRightRun"));
		playerLockOnBackRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_lock_run_rightback")));

		// Hit
		auto playerHit1 = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Hit1"));
		auto playerHit1Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_hit_1"));
		playerHit1Clip->SetIsUseEventInTransition(false);
		playerHit1->SetAnimationClip(playerHit1Clip);

		auto playerHit2 = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Hit2"));
		auto playerHit2Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_hit_2"));
		playerHit2Clip->SetIsUseEventInTransition(false);
		playerHit2->SetAnimationClip(playerHit2Clip);

		auto playerHit3 = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Hit3"));
		auto playerHit3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_hit_3"));
		playerHit3Clip->SetIsUseEventInTransition(false);
		playerHit3Clip->SetIsRootMotion(true);
		playerHit3->SetAnimationClip(playerHit3Clip);

		auto playerHeavyHit = playerOverdriveSwordStateMachine->AddState(TEXT("Player_HeavyHit"));			// 강공격
		auto playerHeavyHitClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_heavyhit"));
		playerHeavyHitClip->SetIsRootMotion(true);
		playerHeavyHitClip->SetIsUseEventInTransition(false);
		playerHeavyHit->SetAnimationClip(playerHeavyHitClip);

		// Down
		auto playerDown = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Down"));					// 다운
		auto playerDownClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_down"));
		playerDownClip->SetIsRootMotion(true);
		playerDown->SetAnimationClip(playerDownClip);

		auto playerDownup = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Downup"));				// 다운 후 기상
		auto playerDownupClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_downup"));
		playerDownup->SetAnimationClip(playerDownupClip);

		// Die
		auto playerDieStart = playerOverdriveSwordStateMachine->AddState(TEXT("Player_DieStart"));			// 죽음 시작
		auto playerDieStartClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_diestart"));
		playerDieStart->SetAnimationClip(playerDieStartClip);

		auto playerDie = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Die"));					// 죽음 계속
		auto playerDieClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_dieloop"));
		playerDie->SetAnimationClip(playerDieClip);

		// Attacks
		auto playerSword = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Sword"));				// 기본 소드 공격
		playerSword->SetSpeedParameterActive(true);
		playerSword->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerSwordClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword"));
		playerSwordClip->SetIsRootMotion(true);
		playerSword->SetAnimationClip(playerSwordClip);

		// Ultimate
		auto playerSwordUlt = playerOverdriveSwordStateMachine->AddState(TEXT("Player_Overdrive_Ultimate"));
		playerSwordUlt->SetSpeedParameterActive(true);
		playerSwordUlt->SetSpeedParameter(TEXT("AnimationSpeed"));
		auto playerSwordUltAnim = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_ultimate_sword"));
		playerSwordUltAnim->SetIsLoop(false);
		playerSwordUltAnim->SetIsUseEventInTransition(false);
		playerSwordUlt->SetAnimationClip(playerSwordUltAnim);

		// Overdrive Exit.
		auto playerOverdriveSwordExit = playerOverdriveSwordStateMachine->AddState(TEXT("Player_OverdriveSwordExit"));
		auto playerOverdriveSwordExitClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_sword_exit"));
		playerOverdriveSwordExit->SetAnimationClip(playerOverdriveSwordExitClip);

#pragma region SWORD_ANIMATION_EVENT
		DUOLGameEngine::AnimationEvent swordEvent;

		swordEvent._eventName = TEXT("StartCancleFrame");

		swordEvent._targetFrame = 20.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("StartCancleFrame");

		swordEvent._targetFrame = 70.f;

		playerSwordClip->AddEvent(swordEvent);
		
		swordEvent._eventName = TEXT("StartSlowFrame");

		swordEvent._targetFrame = 44.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("StartSlowFrame");

		swordEvent._targetFrame = 93.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndSlowFrame");

		swordEvent._targetFrame = 54.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndSlowFrame");

		swordEvent._targetFrame = 103.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndCancleFrame");

		swordEvent._targetFrame = 48.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndCancleFrame");

		swordEvent._targetFrame = 103.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndAttack");

		swordEvent._targetFrame = 161.f;

		playerSwordClip->AddEvent(swordEvent);

		// 마지막 타격은 원형 충격파
		swordEvent._eventName = TEXT("AreaWaveHit");

		swordEvent._targetFrame = 112.f;

		playerSwordClip->AddEvent(swordEvent);

		// 타격 프레임
		swordEvent._eventName = TEXT("StartSwordAttackFrame");

		swordEvent._targetFrame = 21.f;
		
		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 36.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 70.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 83.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._eventName = TEXT("EndSwordAttackFrame");

		swordEvent._targetFrame = 25.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 42.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 75.f;

		playerSwordClip->AddEvent(swordEvent);

		swordEvent._targetFrame = 89.f;

		playerSwordClip->AddEvent(swordEvent);
#pragma endregion

#pragma region HIT_EVENT
		AnimationEvent hitEvent;

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 35.f;

		playerHit1Clip->AddEvent(hitEvent);

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 35.f;

		playerHit2Clip->AddEvent(hitEvent);

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 45.f;

		playerHit3Clip->AddEvent(hitEvent);

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 52.f;

		playerHeavyHitClip->AddEvent(hitEvent);
#pragma endregion

#pragma region DOWN_EVENT
		AnimationEvent downEvent;

		downEvent._eventName = TEXT("EndDown");

		downEvent._targetFrame = 64.f;

		playerDownClip->AddEvent(downEvent);

		downEvent._eventName = TEXT("EndDownup");

		downEvent._targetFrame = 64.f;

		playerDownupClip->AddEvent(downEvent);
#pragma endregion

#pragma region DIE_EVENT
		AnimationEvent dieEvent;

		dieEvent._eventName = TEXT("EndDieStart");

		dieEvent._targetFrame = 53.f;

		playerDieStartClip->AddEvent(dieEvent);
#pragma endregion

#pragma region OVERDRIVE_EXIT_EVENT
		AnimationEvent exitEvent;

		exitEvent._eventName = TEXT("UnequipOverdriveWeaponSword");

		exitEvent._targetFrame = 39.f;

		playerOverdriveSwordExitClip->AddEvent(exitEvent);

		exitEvent._eventName = TEXT("EndOverdriveExit");

		exitEvent._targetFrame = 68.f;

		playerOverdriveSwordExitClip->AddEvent(exitEvent);
#pragma endregion

#pragma region DIE_START_END
		auto playerIdleToDieStart = playerIdle->AddTransition(playerDieStart);
		playerIdleToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerMoveToDieStart = playerMove->AddTransition(playerDieStart);
		playerMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerRunToDieStart = playerRun->AddTransition(playerDieStart);
		playerRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerLockOnLeftMoveToDieStart = playerLockOnLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightMoveToDieStart = playerLockOnRightMove->AddTransition(playerDieStart);
		playerLockOnRightMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnFrontMoveToDieStart = playerLockOnFrontMove->AddTransition(playerDieStart);
		playerLockOnFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnBackMoveToDieStart = playerLockOnBackMove->AddTransition(playerDieStart);
		playerLockOnBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftFrontMoveToDieStart = playerLockOnFrontLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftBackMoveToDieStart = playerLockOnBackLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightFrontMoveToDieStart = playerLockOnFrontRightMove->AddTransition(playerDieStart);
		playerLockOnRightFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightBackMoveToDieStart = playerLockOnFrontRightMove->AddTransition(playerDieStart);
		playerLockOnRightBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerLockOnLeftRunToDieStart = playerLockOnLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightRunToDieStart = playerLockOnRightRun->AddTransition(playerDieStart);
		playerLockOnRightRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnFrontRunToDieStart = playerLockOnFrontRun->AddTransition(playerDieStart);
		playerLockOnFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnBackRunToDieStart = playerLockOnBackRun->AddTransition(playerDieStart);
		playerLockOnBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftFrontRunToDieStart = playerLockOnFrontLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftBackRunToDieStart = playerLockOnBackLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightFrontRunToDieStart = playerLockOnFrontRightRun->AddTransition(playerDieStart);
		playerLockOnRightFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightBackRunToDieStart = playerLockOnFrontRightRun->AddTransition(playerDieStart);
		playerLockOnRightBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerSwordToDieStart = playerSword->AddTransition(playerDieStart);
		playerSwordToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHit1ToDieStart = playerHit1->AddTransition(playerDieStart);
		playerHit1ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHit2ToDieStart = playerHit2->AddTransition(playerDieStart);
		playerHit2ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHit3ToDieStart = playerHit3->AddTransition(playerDieStart);
		playerHit3ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHeavyHitToDieStart = playerHeavyHit->AddTransition(playerDieStart);
		playerHeavyHitToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerDieStartToDie = playerDieStart->AddTransition(playerDie);
		playerDieStartToDie->SetTransitionDuration(0.01f);
		playerDieStartToDie->AddCondition(TEXT("IsDie"), AnimatorConditionMode::False);					// 최종 죽음
#pragma endregion

#pragma region ULTIMATIE_EVENT
		AnimationEvent ultEvent;

		//프레임 종료를 알림
		ultEvent._eventName = TEXT("EndUltimate");
		ultEvent._targetFrame = playerSwordUltAnim->GetMaxFrame()-1;

		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("BulletTimeInUltimate");
		ultEvent._targetFrame = 5;

		playerSwordUltAnim->AddEvent(ultEvent);

		//1타
		ultEvent._eventName = TEXT("StartUltimateSwordTrail");
		ultEvent._targetFrame = 113;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndUltimateSwordTrail");
		ultEvent._targetFrame = 120;

		playerSwordUltAnim->AddEvent(ultEvent);
		
		//2타
		ultEvent._eventName = TEXT("StartUltimateSwordTrail");
		ultEvent._targetFrame = 133;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndUltimateSwordTrail");
		ultEvent._targetFrame = 139;
		playerSwordUltAnim->AddEvent(ultEvent);

		//3타
		ultEvent._eventName = TEXT("StartUltimateSwordTrail");
		ultEvent._targetFrame = 156;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndUltimateSwordTrail");
		ultEvent._targetFrame = 165;
		playerSwordUltAnim->AddEvent(ultEvent);

		//4타
		ultEvent._eventName = TEXT("StartUltimateSwordTrail");
		ultEvent._targetFrame = 180;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndUltimateSwordTrail");
		ultEvent._targetFrame = 186;
		playerSwordUltAnim->AddEvent(ultEvent);

		//5타
		ultEvent._eventName = TEXT("StartUltimateSwordTrail");
		ultEvent._targetFrame = 203;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndUltimateSwordTrail");
		ultEvent._targetFrame = 213;
		playerSwordUltAnim->AddEvent(ultEvent);

		//6타
		ultEvent._eventName = TEXT("StartUltimateSwordTrail");
		ultEvent._targetFrame = 230;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndUltimateSwordTrail");
		ultEvent._targetFrame = 240;
		playerSwordUltAnim->AddEvent(ultEvent);

		//타격프레임 설정
		//1타
		ultEvent._eventName = TEXT("StartSwordAttackFrame");
		ultEvent._targetFrame = 113;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndSwordAttackFrame");
		ultEvent._targetFrame = 122;
		playerSwordUltAnim->AddEvent(ultEvent);

		//2타
		ultEvent._eventName = TEXT("StartSwordAttackFrame");
		ultEvent._targetFrame = 132;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndSwordAttackFrame");
		ultEvent._targetFrame = 140;
		playerSwordUltAnim->AddEvent(ultEvent);

		//3타
		ultEvent._eventName = TEXT("StartSwordAttackFrame");
		ultEvent._targetFrame = 155;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndSwordAttackFrame");
		ultEvent._targetFrame = 163;
		playerSwordUltAnim->AddEvent(ultEvent);

		//4타
		ultEvent._eventName = TEXT("StartSwordAttackFrame");
		ultEvent._targetFrame = 179;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndSwordAttackFrame");
		ultEvent._targetFrame = 187;
		playerSwordUltAnim->AddEvent(ultEvent);

		//5타
		ultEvent._eventName = TEXT("StartSwordAttackFrame");
		ultEvent._targetFrame = 204;
		playerSwordUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndSwordAttackFrame");
		ultEvent._targetFrame = 213;
		playerSwordUltAnim->AddEvent(ultEvent);

		//6타
		ultEvent._eventName = TEXT("SwordWaveHit");
		ultEvent._targetFrame = 238;
		playerSwordUltAnim->AddEvent(ultEvent);


#pragma endregion

#pragma region OVERDRIVE_EXIT
		auto playerIdleToExit = playerIdle->AddTransition(playerOverdriveSwordExit);
		playerIdleToExit->SetTransitionDuration(0.005f);
		playerIdleToExit->AddCondition(TEXT("IsOverdriveExit"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DASH_START
		auto playerIdleToDash = playerIdle->AddTransition(playerDash);
		playerIdleToDash->SetTransitionDuration(0.005f);
		playerIdleToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerMoveToDash = playerMove->AddTransition(playerDash);
		playerMoveToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerRunToDash = playerRun->AddTransition(playerDash);
		playerRunToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordToDash = playerSword->AddTransition(playerDash);
		playerSwordToDash->SetTransitionDuration(0.01f);
		playerSwordToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DASH_END
		auto playerDashToIdle = playerDash->AddTransition(playerIdle);
		playerDashToIdle->SetTransitionDuration(0.2f);
		playerDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		auto playerDashToMove = playerDash->AddTransition(playerMove);
		playerDashToMove->SetTransitionDuration(0.2f);
		playerDashToMove->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		playerDashToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		auto playerDashToRun = playerDash->AddTransition(playerRun);
		playerDashToRun->SetTransitionDuration(0.2f);
		playerDashToRun->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		playerDashToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DOWN_START_END
		auto playerIdleToDown = playerIdle->AddTransition(playerDown);
		playerIdleToDown->SetTransitionDuration(0.0001f);
		playerIdleToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerMoveToDown = playerMove->AddTransition(playerDown);
		playerMoveToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerRunToDown = playerRun->AddTransition(playerDown);
		playerRunToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerSwordToDown = playerSword->AddTransition(playerDown);
		playerSwordToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHit1ToDown = playerHit1->AddTransition(playerDown);
		playerHit1ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHit2ToDown = playerHit2->AddTransition(playerDown);
		playerHit2ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHit3ToDown = playerHit3->AddTransition(playerDown);
		playerHit3ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHeavyHitToDown = playerHeavyHit->AddTransition(playerDown);
		playerHeavyHitToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		// 애니메이션 시간이 다 끝날 때 Transition 하면 됩니다 ..!
		auto playerDownToDownup = playerDown->AddTransition(playerDownup);
		playerDownToDownup->AddCondition(TEXT("IsDown"), AnimatorConditionMode::False);
		playerDownToDownup->AddCondition(TEXT("IsDownup"), AnimatorConditionMode::True);

		auto playerDownupToIdle = playerDownup->AddTransition(playerIdle);
		playerDownupToIdle->SetTransitionDuration(0.05f);
		playerDownupToIdle->AddCondition(TEXT("IsDownup"), AnimatorConditionMode::False);
#pragma endregion

#pragma region HIT_START
		auto playerIdleToHit1 = playerIdle->AddTransition(playerHit1);
		playerIdleToHit1->SetTransitionDuration(0.01f);
		playerIdleToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerIdleToHit2 = playerIdle->AddTransition(playerHit2);
		playerIdleToHit2->SetTransitionDuration(0.01f);
		playerIdleToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerIdleToHit3 = playerIdle->AddTransition(playerHit3);
		playerIdleToHit3->SetTransitionDuration(0.01f);
		playerIdleToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerIdleToHeavyHit = playerIdle->AddTransition(playerHeavyHit);
		playerIdleToHeavyHit->SetTransitionDuration(0.01f);
		playerIdleToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerMoveToHit1 = playerMove->AddTransition(playerHit1);
		playerMoveToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerMoveToHit2 = playerMove->AddTransition(playerHit2);
		playerMoveToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerMoveToHit3 = playerMove->AddTransition(playerHit3);
		playerMoveToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerMoveToHeavyHit = playerMove->AddTransition(playerHeavyHit);
		playerMoveToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerRunToHit1 = playerRun->AddTransition(playerHit1);
		playerRunToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerRunToHit2 = playerRun->AddTransition(playerHit2);
		playerRunToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerRunToHit3 = playerRun->AddTransition(playerHit3);
		playerRunToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerRunToHeavyHit = playerRun->AddTransition(playerHeavyHit);
		playerRunToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerSwordToHit1 = playerSword->AddTransition(playerHit1);
		playerSwordToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordToHit2 = playerSword->AddTransition(playerHit2);
		playerSwordToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordToHit3 = playerSword->AddTransition(playerHit3);
		playerSwordToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);
		
		auto playerSwordToHeavyHit = playerSword->AddTransition(playerHeavyHit);
		playerSwordToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit1ToHit2 = playerHit1->AddTransition(playerHit2);
		playerHit1ToHit2->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerHit1ToHit3 = playerHit1->AddTransition(playerHit3);
		playerHit1ToHit3->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerHit1ToHeavyHit = playerHit1->AddTransition(playerHeavyHit);
		playerHit1ToHeavyHit->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit2ToHit1 = playerHit2->AddTransition(playerHit1);
		playerHit2ToHit1->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerHit2ToHit3 = playerHit2->AddTransition(playerHit3);
		playerHit2ToHit3->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerHit2ToHeavyHit = playerHit2->AddTransition(playerHeavyHit);
		playerHit2ToHeavyHit->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit3ToHit1 = playerHit3->AddTransition(playerHit1);
		playerHit3ToHit1->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit3ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerHit3ToHit2 = playerHit3->AddTransition(playerHit2);
		playerHit3ToHit2->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit3ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerHit3ToHeavyHit = playerHit3->AddTransition(playerHeavyHit);
		playerHit3ToHeavyHit->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit3ToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHeavyHitToHit1 = playerHeavyHit->AddTransition(playerHit1);
		playerHeavyHitToHit1->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
		playerHeavyHitToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerHeavyHitToHit2 = playerHeavyHit->AddTransition(playerHit2);
		playerHeavyHitToHit2->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
		playerHeavyHitToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerHeavyHitToHit3 = playerHeavyHit->AddTransition(playerHit3);
		playerHeavyHitToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
		playerHeavyHitToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);
#pragma endregion

#pragma region HIT_END
		auto playerHit1ToIdle = playerHit1->AddTransition(playerIdle);
		playerHit1ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit1ToIdle->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit1ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);

		auto playerHit2ToIdle = playerHit2->AddTransition(playerIdle);
		playerHit2ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit2ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToIdle->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit2ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);

		auto playerHit3ToIdle = playerHit3->AddTransition(playerIdle);
		playerHit3ToIdle->SetTransitionDuration(0.05f);
		playerHit3ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit3ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit3ToIdle->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit3ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);

		auto playerHeavyHitToIdle = playerHeavyHit->AddTransition(playerIdle);
		playerHeavyHitToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHeavyHitToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHeavyHitToIdle->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHeavyHitToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
#pragma endregion

#pragma region ATTACK_START
		auto playerIdleToSword = playerIdle->AddTransition(playerSword);
		playerIdleToSword->SetTransitionDuration(0.01f);
		playerIdleToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerIdleToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);
		
		auto playerMoveToSword = playerMove->AddTransition(playerSword);
		playerMoveToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerMoveToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);

		auto playerRunToSword = playerRun->AddTransition(playerSword);
		playerRunToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerRunToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);
#pragma endregion

#pragma region ATTACK_END
		auto playerSwordToIdle = playerSword->AddTransition(playerIdle);
		playerSwordToIdle->SetTransitionDuration(0.03f);
		playerSwordToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);
#pragma endregion

#pragma region LOCK_ON_STATE_MOVEMENT
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

#pragma region MOVE_RUN_IDLE
		auto playerIdleToRun = playerIdle->AddTransition(playerRun);
		playerIdleToRun->SetTransitionDuration(0.01f);
		playerIdleToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerRunToIdle = playerRun->AddTransition(playerIdle);
		playerRunToIdle->SetTransitionDuration(0.2f);
		playerRunToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerRunToIdle->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerIdleToMove = playerIdle->AddTransition(playerMove);
		playerIdleToMove->SetTransitionDuration(0.01f);
		playerIdleToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		auto playerMoveToIdle = playerMove->AddTransition(playerIdle);
		playerMoveToIdle->SetTransitionDuration(0.2f);
		playerMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		auto playerMoveToRun = playerMove->AddTransition(playerRun);
		playerMoveToRun->SetTransitionDuration(0.3f);
		playerMoveToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerMoveToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerRunToMove = playerRun->AddTransition(playerMove);
		playerRunToMove->SetTransitionDuration(0.3f);
		playerRunToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerRunToMove->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);
#pragma endregion

#pragma region ULTIMATIE
		auto playerIdleToUlt = playerIdle->AddTransition(playerSwordUlt);
		playerIdleToUlt->AddCondition(TEXT("IsUltimate"), AnimatorConditionMode::True);

		auto playerRunToUlt = playerRun->AddTransition(playerSwordUlt);
		playerRunToUlt->AddCondition(TEXT("IsUltimate"), AnimatorConditionMode::True);

		auto playerMoveToUlt = playerMove->AddTransition(playerSwordUlt);
		playerMoveToUlt->AddCondition(TEXT("IsUltimate"), AnimatorConditionMode::True);

		auto playerUltToIdle = playerSwordUlt->AddTransition(playerIdle);
		playerUltToIdle->AddCondition(TEXT("IsUltimate"), AnimatorConditionMode::False);

#pragma endregion

		DUOLGameEngine::ResourceManager::GetInstance()->AddAnimatorController(playerOverdriveSwordCon);
	}

	void DUOLClient_Initializer::Overdrive_Fist_Initialize()
	{
		using namespace DUOLGameEngine;

		auto playerOverdriveFistCon = std::make_shared<DUOLGameEngine::AnimatorController>(TEXT("Player_Overdrive_FistAnimatorController"));

		auto playerOverdriveFistStateMachine = playerOverdriveFistCon->AddStateMachine(TEXT("PlayerOverdriveFistStateMachine"));

		// 플레이어 상태
		playerOverdriveFistCon->AddParameter(TEXT("IsMove"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsRun"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsAttack"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsLockOn"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsDash"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsHit1"), AnimatorControllerParameterType::Bool);				// 약공격 1

		playerOverdriveFistCon->AddParameter(TEXT("IsHit2"), AnimatorControllerParameterType::Bool);				// 약공격 2

		playerOverdriveFistCon->AddParameter(TEXT("IsHit3"), AnimatorControllerParameterType::Bool);				// 약공격 3

		playerOverdriveFistCon->AddParameter(TEXT("IsHeavyHit"), AnimatorControllerParameterType::Bool);			// 강공격

		playerOverdriveFistCon->AddParameter(TEXT("IsDown"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsDownup"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsOverdriveExit"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsUltimate"), AnimatorControllerParameterType::Bool);

		// 방향 여부
		playerOverdriveFistCon->AddParameter(TEXT("IsLeft"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsRight"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsFront"), AnimatorControllerParameterType::Bool);

		playerOverdriveFistCon->AddParameter(TEXT("IsBack"), AnimatorControllerParameterType::Bool);

		// 공격 숫자
		playerOverdriveFistCon->AddParameter(TEXT("AttackCount"), AnimatorControllerParameterType::Int);

		// 애니메이션 재생 속도 배속
		playerOverdriveFistCon->AddParameter(TEXT("AnimationSpeed"), AnimatorControllerParameterType::Float);

		// Idle
		auto playerIdle = playerOverdriveFistStateMachine->AddState(TEXT("Player_Idle"));
		playerIdle->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_idle")));

		// Move
		auto playerMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_Move"));
		playerMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_move")));

		// Run
		auto playerRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_Run"));
		playerRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_run")));

		// Dash
		auto playerDash = playerOverdriveFistStateMachine->AddState(TEXT("Player_Dash"));
		auto playerDashClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_dash"));
		playerDashClip->SetIsRootMotion(true);
		playerDash->SetAnimationClip(playerDashClip);

		// Lock-On Move and Run
		auto playerLockOnLeftMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnLeftMove"));
		playerLockOnLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_left")));

		auto playerLockOnRightMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnRightMove"));
		playerLockOnRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_right")));

		auto playerLockOnFrontMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnFrontMove"));
		playerLockOnFrontMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_front")));

		auto playerLockOnBackMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnBackMove"));
		playerLockOnBackMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_back")));

		auto playerLockOnFrontLeftMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnFrontLeftMove"));
		playerLockOnFrontLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_leftfront")));

		auto playerLockOnFrontRightMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnFrontRightMove"));
		playerLockOnFrontRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_rightfront")));

		auto playerLockOnBackLeftMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnBackLeftMove"));
		playerLockOnBackLeftMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_leftback")));

		auto playerLockOnBackRightMove = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnBackRightMove"));
		playerLockOnBackRightMove->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_rightback")));

		auto playerLockOnLeftRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnLeftRun"));
		playerLockOnLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_left")));

		auto playerLockOnRightRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnRightRun"));
		playerLockOnRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_right")));

		auto playerLockOnFrontRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnFrontRun"));
		playerLockOnFrontRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_front")));

		auto playerLockOnBackRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnBackRun"));
		playerLockOnBackRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_back")));

		auto playerLockOnFrontLeftRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnFrontLeftRun"));
		playerLockOnFrontLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_leftfront")));

		auto playerLockOnFrontRightRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnFrontRightRun"));
		playerLockOnFrontRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_rightfront")));

		auto playerLockOnBackLeftRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnBackLeftRun"));
		playerLockOnBackLeftRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_leftback")));

		auto playerLockOnBackRightRun = playerOverdriveFistStateMachine->AddState(TEXT("Player_LockOnBackRightRun"));
		playerLockOnBackRightRun->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_lock_run_rightback")));

		// Hit
		auto playerHit1 = playerOverdriveFistStateMachine->AddState(TEXT("Player_Hit1"));
		auto playerHit1Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_hit_1"));
		playerHit1Clip->SetIsUseEventInTransition(false);
		playerHit1->SetAnimationClip(playerHit1Clip);

		auto playerHit2 = playerOverdriveFistStateMachine->AddState(TEXT("Player_Hit2"));
		auto playerHit2Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_hit_2"));
		playerHit2Clip->SetIsUseEventInTransition(false);
		playerHit2->SetAnimationClip(playerHit2Clip);

		auto playerHit3 = playerOverdriveFistStateMachine->AddState(TEXT("Player_Hit3"));
		auto playerHit3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_hit_3"));
		playerHit3Clip->SetIsUseEventInTransition(false);
		playerHit3Clip->SetIsRootMotion(true);
		playerHit3->SetAnimationClip(playerHit3Clip);
		
		auto playerHeavyHit = playerOverdriveFistStateMachine->AddState(TEXT("Player_HeavyHit"));			// 강공격
		auto playerHeavyHitClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_heavyhit"));
		playerHeavyHitClip->SetIsRootMotion(true);
		playerHeavyHitClip->SetIsUseEventInTransition(false);
		playerHeavyHit->SetAnimationClip(playerHeavyHitClip);

		// Down
		auto playerDown = playerOverdriveFistStateMachine->AddState(TEXT("Player_Down"));					// 다운
		auto playerDownClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_down"));
		playerDownClip->SetIsRootMotion(true);
		playerDown->SetAnimationClip(playerDownClip);

		auto playerDownup = playerOverdriveFistStateMachine->AddState(TEXT("Player_Downup"));				// 다운 후 기상
		auto playerDownupClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_downup"));
		playerDownup->SetAnimationClip(playerDownupClip);

		// Die
		auto playerDieStart = playerOverdriveFistStateMachine->AddState(TEXT("Player_DieStart"));			// 죽음 시작
		auto playerDieStartClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_diestart"));
		playerDieStart->SetAnimationClip(playerDieStartClip);

		auto playerDie = playerOverdriveFistStateMachine->AddState(TEXT("Player_Die"));					// 죽음 계속
		auto playerDieClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_dieloop"));
		playerDie->SetAnimationClip(playerDieClip);

		// Attacks
		auto playerFist = playerOverdriveFistStateMachine->AddState(TEXT("Player_Fist"));				// 기본 피스트 공격
		auto playerFistClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist"));
		playerFistClip->SetIsRootMotion(true);
		playerFist->SetAnimationClip(playerFistClip);

		// Overdrive Exit.
		auto playerOverdriveFistExit = playerOverdriveFistStateMachine->AddState(TEXT("Player_OverdriveFistExit"));
		auto playerOverdriveFistExitClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_overdrive_fist_exit"));
		playerOverdriveFistExit->SetAnimationClip(playerOverdriveFistExitClip);

		// Ultimate
		auto playerFistUlt = playerOverdriveFistStateMachine->AddState(TEXT("Player_Overdrive_Ultimate"));
		auto playerFistUltAnim = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_ultimate_fist"));
		playerFistUltAnim->SetIsLoop(false);
		playerFistUlt->SetAnimationClip(playerFistUltAnim);

#pragma region FIST_ANIMATION_EVENT
		DUOLGameEngine::AnimationEvent fistEvent;

		fistEvent._eventName = TEXT("StartCancleFrame");

		fistEvent._targetFrame = 10.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("StartCancleFrame");

		fistEvent._targetFrame = 37.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("StartSlowFrame");

		fistEvent._targetFrame = 18.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("StartSlowFrame");

		fistEvent._targetFrame = 47.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("EndSlowFrame");

		fistEvent._targetFrame = 28.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("EndSlowFrame");

		fistEvent._targetFrame = 57.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("EndCancleFrame");

		fistEvent._targetFrame = 28.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("EndCancleFrame");

		fistEvent._targetFrame = 57.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._eventName = TEXT("EndAttack");

		fistEvent._targetFrame = 132.f;

		playerFistClip->AddEvent(fistEvent);

		// 타격 프레임
		fistEvent._eventName = TEXT("FistHit");

		fistEvent._targetFrame = 15.f;

		playerFistClip->AddEvent(fistEvent);

		fistEvent._targetFrame = 41.f;

		playerFistClip->AddEvent(fistEvent);

		// 마지막 타격은 원형 웨이브 타격
		fistEvent._eventName = TEXT("AreaWaveHit");

		fistEvent._targetFrame = 90.f;

		playerFistClip->AddEvent(fistEvent);
#pragma endregion

#pragma region HIT_EVENT
		AnimationEvent hitEvent;

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 35.f;

		playerHit1Clip->AddEvent(hitEvent);

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 35.f;

		playerHit2Clip->AddEvent(hitEvent);

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 41.f;

		playerHit3Clip->AddEvent(hitEvent);

		hitEvent._eventName = TEXT("EndHit");

		hitEvent._targetFrame = 52.f;

		playerHeavyHitClip->AddEvent(hitEvent);
#pragma endregion

#pragma region DOWN_EVENT
		AnimationEvent downEvent;

		downEvent._eventName = TEXT("EndDown");

		downEvent._targetFrame = 84.f;

		playerDownClip->AddEvent(downEvent);

		downEvent._eventName = TEXT("EndDownup");

		downEvent._targetFrame = 71.f;

		playerDownupClip->AddEvent(downEvent);
#pragma endregion

#pragma region DIE_EVENT
		AnimationEvent dieEvent;

		dieEvent._eventName = TEXT("EndDieStart");

		dieEvent._targetFrame = 53.f;

		playerDieStartClip->AddEvent(dieEvent);
#pragma endregion

#pragma region OVERDRIVE_EXIT_EVENT
		AnimationEvent exitEvent;

		exitEvent._eventName = TEXT("EndOverdriveExit");

		exitEvent._targetFrame = 54.f;

		playerOverdriveFistExitClip->AddEvent(exitEvent);
#pragma endregion

#pragma region DIE_START_END
		auto playerIdleToDieStart = playerIdle->AddTransition(playerDieStart);
		playerIdleToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerMoveToDieStart = playerMove->AddTransition(playerDieStart);
		playerMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerRunToDieStart = playerRun->AddTransition(playerDieStart);
		playerRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerLockOnLeftMoveToDieStart = playerLockOnLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightMoveToDieStart = playerLockOnRightMove->AddTransition(playerDieStart);
		playerLockOnRightMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnFrontMoveToDieStart = playerLockOnFrontMove->AddTransition(playerDieStart);
		playerLockOnFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnBackMoveToDieStart = playerLockOnBackMove->AddTransition(playerDieStart);
		playerLockOnBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftFrontMoveToDieStart = playerLockOnFrontLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftBackMoveToDieStart = playerLockOnBackLeftMove->AddTransition(playerDieStart);
		playerLockOnLeftBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightFrontMoveToDieStart = playerLockOnFrontRightMove->AddTransition(playerDieStart);
		playerLockOnRightFrontMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightBackMoveToDieStart = playerLockOnFrontRightMove->AddTransition(playerDieStart);
		playerLockOnRightBackMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerLockOnLeftRunToDieStart = playerLockOnLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightRunToDieStart = playerLockOnRightRun->AddTransition(playerDieStart);
		playerLockOnRightRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnFrontRunToDieStart = playerLockOnFrontRun->AddTransition(playerDieStart);
		playerLockOnFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnBackRunToDieStart = playerLockOnBackRun->AddTransition(playerDieStart);
		playerLockOnBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftFrontRunToDieStart = playerLockOnFrontLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnLeftBackRunToDieStart = playerLockOnBackLeftRun->AddTransition(playerDieStart);
		playerLockOnLeftBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightFrontRunToDieStart = playerLockOnFrontRightRun->AddTransition(playerDieStart);
		playerLockOnRightFrontRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
		auto playerLockOnRightBackRunToDieStart = playerLockOnFrontRightRun->AddTransition(playerDieStart);
		playerLockOnRightBackRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerSwordToDieStart = playerFist->AddTransition(playerDieStart);
		playerSwordToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHit1ToDieStart = playerHit1->AddTransition(playerDieStart);
		playerHit1ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHit2ToDieStart = playerHit2->AddTransition(playerDieStart);
		playerHit2ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHit3ToDieStart = playerHit3->AddTransition(playerDieStart);
		playerHit3ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerHeavyHitToDieStart = playerHeavyHit->AddTransition(playerDieStart);
		playerHeavyHitToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerDieStartToDie = playerDieStart->AddTransition(playerDie);
		playerDieStartToDie->SetTransitionDuration(0.01f);
		playerDieStartToDie->AddCondition(TEXT("IsDie"), AnimatorConditionMode::False);					// 최종 죽음
#pragma endregion

#pragma region OVERDRIVE_EXIT
		auto playerIdleToExit = playerIdle->AddTransition(playerOverdriveFistExit);
		playerIdleToExit->SetTransitionDuration(0.005f);
		playerIdleToExit->AddCondition(TEXT("IsOverdriveExit"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DASH_START
		auto playerIdleToDash = playerIdle->AddTransition(playerDash);
		playerIdleToDash->SetTransitionDuration(0.005f);
		playerIdleToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerMoveToDash = playerMove->AddTransition(playerDash);
		playerMoveToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerRunToDash = playerRun->AddTransition(playerDash);
		playerRunToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordToDash = playerFist->AddTransition(playerDash);
		playerSwordToDash->SetTransitionDuration(0.01f);
		playerSwordToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DASH_END
		auto playerDashToIdle = playerDash->AddTransition(playerIdle);
		playerDashToIdle->SetTransitionDuration(0.2f);
		playerDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		auto playerDashToMove = playerDash->AddTransition(playerMove);
		playerDashToMove->SetTransitionDuration(0.2f);
		playerDashToMove->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		playerDashToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		auto playerDashToRun = playerDash->AddTransition(playerRun);
		playerDashToRun->SetTransitionDuration(0.2f);
		playerDashToRun->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		playerDashToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DOWN_START_END
		auto playerIdleToDown = playerIdle->AddTransition(playerDown);
		playerIdleToDown->SetTransitionDuration(0.0001f);
		playerIdleToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerMoveToDown = playerMove->AddTransition(playerDown);
		playerMoveToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerRunToDown = playerRun->AddTransition(playerDown);
		playerRunToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerSwordToDown = playerFist->AddTransition(playerDown);
		playerSwordToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHit1ToDown = playerHit1->AddTransition(playerDown);
		playerHit1ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHit2ToDown = playerHit2->AddTransition(playerDown);
		playerHit2ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHit3ToDown = playerHit3->AddTransition(playerDown);
		playerHit3ToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerHeavyHitToDown = playerHeavyHit->AddTransition(playerDown);
		playerHeavyHitToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		// 애니메이션 시간이 다 끝날 때 Transition 하면 됩니다 ..!
		auto playerDownToDownup = playerDown->AddTransition(playerDownup);
		playerDownToDownup->AddCondition(TEXT("IsDown"), AnimatorConditionMode::False);
		playerDownToDownup->AddCondition(TEXT("IsDownup"), AnimatorConditionMode::True);

		auto playerDownupToIdle = playerDownup->AddTransition(playerIdle);
		playerDownupToIdle->SetTransitionDuration(0.1f);
		playerDownupToIdle->AddCondition(TEXT("IsDownup"), AnimatorConditionMode::False);
#pragma endregion

#pragma region HIT_START
		auto playerIdleToHit1 = playerIdle->AddTransition(playerHit1);
		playerIdleToHit1->SetTransitionDuration(0.01f);
		playerIdleToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerIdleToHit2 = playerIdle->AddTransition(playerHit2);
		playerIdleToHit2->SetTransitionDuration(0.01f);
		playerIdleToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerIdleToHit3 = playerIdle->AddTransition(playerHit3);
		playerIdleToHit3->SetTransitionDuration(0.01f);
		playerIdleToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerIdleToHeavyHit = playerIdle->AddTransition(playerHeavyHit);
		playerIdleToHeavyHit->SetTransitionDuration(0.01f);
		playerIdleToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerMoveToHit1 = playerMove->AddTransition(playerHit1);
		playerMoveToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerMoveToHit2 = playerMove->AddTransition(playerHit2);
		playerMoveToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerMoveToHit3 = playerMove->AddTransition(playerHit3);
		playerMoveToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerMoveToHeavyHit = playerMove->AddTransition(playerHeavyHit);
		playerMoveToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerRunToHit1 = playerRun->AddTransition(playerHit1);
		playerRunToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerRunToHit2 = playerRun->AddTransition(playerHit2);
		playerRunToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerRunToHit3 = playerRun->AddTransition(playerHit3);
		playerRunToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerRunToHeavyHit = playerRun->AddTransition(playerHeavyHit);
		playerRunToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerSwordToHit1 = playerFist->AddTransition(playerHit1);
		playerSwordToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordToHit2 = playerFist->AddTransition(playerHit2);
		playerSwordToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordToHit3 = playerFist->AddTransition(playerHit3);
		playerSwordToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerSwordToHeavyHit = playerFist->AddTransition(playerHeavyHit);
		playerSwordToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit1ToHit2 = playerHit1->AddTransition(playerHit2);
		playerHit1ToHit2->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerHit1ToHit3 = playerHit1->AddTransition(playerHit3);
		playerHit1ToHit3->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerHit1ToHeavyHit = playerHit1->AddTransition(playerHeavyHit);
		playerHit1ToHeavyHit->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit2ToHit1 = playerHit2->AddTransition(playerHit1);
		playerHit2ToHit1->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerHit2ToHit3 = playerHit2->AddTransition(playerHit3);
		playerHit2ToHit3->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);

		auto playerHit2ToHeavyHit = playerHit2->AddTransition(playerHeavyHit);
		playerHit2ToHeavyHit->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHit3ToHit1 = playerHit3->AddTransition(playerHit1);
		playerHit3ToHit1->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit3ToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerHit3ToHit2 = playerHit3->AddTransition(playerHit2);
		playerHit3ToHit2->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit3ToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerHit3ToHeavyHit = playerHit3->AddTransition(playerHeavyHit);
		playerHit3ToHeavyHit->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit3ToHeavyHit->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerHeavyHitToHit1 = playerHeavyHit->AddTransition(playerHit1);
		playerHeavyHitToHit1->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
		playerHeavyHitToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerHeavyHitToHit2 = playerHeavyHit->AddTransition(playerHit2);
		playerHeavyHitToHit2->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
		playerHeavyHitToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerHeavyHitToHit3 = playerHeavyHit->AddTransition(playerHit3);
		playerHeavyHitToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
		playerHeavyHitToHit3->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::True);
#pragma endregion

#pragma region HIT_END
		auto playerHit1ToIdle = playerHit1->AddTransition(playerIdle);
		playerHit1ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit1ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit1ToIdle->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit1ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);

		auto playerHit2ToIdle = playerHit2->AddTransition(playerIdle);
		playerHit2ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit2ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit2ToIdle->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit2ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);

		auto playerHit3ToIdle = playerHit3->AddTransition(playerIdle);
		playerHit3ToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHit3ToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHit3ToIdle->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHit3ToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);

		auto playerHeavyHitToIdle = playerHeavyHit->AddTransition(playerIdle);
		playerHeavyHitToIdle->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::False);
		playerHeavyHitToIdle->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::False);
		playerHeavyHitToIdle->AddCondition(TEXT("IsHit3"), AnimatorConditionMode::False);
		playerHeavyHitToIdle->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::False);
#pragma endregion

#pragma region ATTACK_START
		auto playerIdleToSword = playerIdle->AddTransition(playerFist);
		playerIdleToSword->SetTransitionDuration(0.01f);
		playerIdleToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerIdleToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);

		auto playerMoveToSword = playerMove->AddTransition(playerFist);
		playerMoveToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerMoveToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);

		auto playerRunToSword = playerRun->AddTransition(playerFist);
		playerRunToSword->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::True);
		playerRunToSword->AddCondition(TEXT("IsSword"), AnimatorConditionMode::True);
#pragma endregion

#pragma region ATTACK_END
		auto playerSwordToIdle = playerFist->AddTransition(playerIdle);
		playerSwordToIdle->SetTransitionDuration(0.01f);
		playerSwordToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);
#pragma endregion

#pragma region LOCK_ON_STATE_MOVEMENT
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

#pragma region MOVE_RUN_IDLE
		auto playerIdleToRun = playerIdle->AddTransition(playerRun);
		playerIdleToRun->SetTransitionDuration(0.01f);
		playerIdleToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerIdleToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerRunToIdle = playerRun->AddTransition(playerIdle);
		playerRunToIdle->SetTransitionDuration(0.2f);
		playerRunToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerRunToIdle->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerIdleToMove = playerIdle->AddTransition(playerMove);
		playerIdleToMove->SetTransitionDuration(0.01f);
		playerIdleToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		auto playerMoveToIdle = playerMove->AddTransition(playerIdle);
		playerMoveToIdle->SetTransitionDuration(0.2f);
		playerMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		auto playerMoveToRun = playerMove->AddTransition(playerRun);
		playerMoveToRun->SetTransitionDuration(0.3f);
		playerMoveToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerMoveToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerRunToMove = playerRun->AddTransition(playerMove);
		playerRunToMove->SetTransitionDuration(0.3f);
		playerRunToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerRunToMove->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);
#pragma endregion

#pragma region ULTIMATIE_EVENT
		AnimationEvent ultEvent;

		ultEvent._eventName = TEXT("EndUltimate");
		ultEvent._targetFrame = playerFistUltAnim->GetMaxFrame() - 1;

		playerFistUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("ChargingFist");
		ultEvent._targetFrame = 22;

		playerFistUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("EndCharging");
		ultEvent._targetFrame = 41;

		playerFistUltAnim->AddEvent(ultEvent);

		ultEvent._eventName = TEXT("FistWaveHit");
		ultEvent._targetFrame = 105;

		playerFistUltAnim->AddEvent(ultEvent);
#pragma endregion

#pragma region ULTIMATIE
		auto playerIdleToUlt = playerIdle->AddTransition(playerFistUlt);
		playerIdleToUlt->AddCondition(TEXT("IsUltimate"), AnimatorConditionMode::True);

		auto playerRunToUlt = playerRun->AddTransition(playerFistUlt);
		playerRunToUlt->AddCondition(TEXT("IsUltimate"), AnimatorConditionMode::True);

		auto playerMoveToUlt = playerMove->AddTransition(playerFistUlt);
		playerMoveToUlt->AddCondition(TEXT("IsUltimate"), AnimatorConditionMode::True);

		auto playerUltToIdle = playerFistUlt->AddTransition(playerIdle);
		playerUltToIdle->AddCondition(TEXT("IsUltimate"), AnimatorConditionMode::False);

#pragma endregion


		DUOLGameEngine::ResourceManager::GetInstance()->AddAnimatorController(playerOverdriveFistCon);
	}
}