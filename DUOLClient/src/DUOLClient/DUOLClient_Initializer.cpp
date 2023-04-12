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

#include "DUOLClient/BehaviorTreeNode/Condition/Condition_CurrentAnimStateCheck.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_HasToken.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsClose.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsGroupCheck.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInAttackRange.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsLookTarget.h"
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
	}

	void DUOLClient_Initializer::BehaviorTreeFactory_Initialize()
	{
		auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

		treeFactory->RegisterNodeType<Condition_IsInAttackRange>("Condition_IsInAttackRange");
		treeFactory->RegisterNodeType<Condition_IsLookTarget>("Condition_IsLookTarget");
		treeFactory->RegisterNodeType<Condition_IsGroupCheck>("Condition_IsGroupCheck");
		treeFactory->RegisterNodeType<Condition_IsClose>("Condition_IsClose");
		treeFactory->RegisterNodeType<Condition_HasToken>("Condition_HasToken");

		treeFactory->RegisterNodeType<Condition_CurrentAnimStateCheck>("Condition_CurrentAnimStateCheck");

		treeFactory->RegisterNodeType<Action_AroundPatrol>("Action_AroundPatrol");
		treeFactory->RegisterNodeType<Action_UseTokenAttack>("Action_UseTokenAttack");
		treeFactory->RegisterNodeType<Action_Die>("Action_Die");
		treeFactory->RegisterNodeType<Action_Hit>("Action_Hit");
		treeFactory->RegisterNodeType<Action_BoidsMoveTo>("Action_BoidsMoveTo");

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

		playerNormalAnimCon->AddParameter(TEXT("IsHeavyHit"), AnimatorControllerParameterType::Bool);				// 강공격

		playerNormalAnimCon->AddParameter(TEXT("IsDown"), AnimatorControllerParameterType::Bool);

		playerNormalAnimCon->AddParameter(TEXT("IsDie"), AnimatorControllerParameterType::Bool);

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
		playerHit1->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_hit_1")));

		auto playerHit2 = playerNormalStateMachine->AddState(TEXT("Player_Hit2"));
		playerHit2->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_hit_2")));

		auto playerHeavyHit = playerNormalStateMachine->AddState(TEXT("Player_HeavyHit"));			// 강공격
		playerHeavyHit->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_heavyhit")));

		// Down
		auto playerDown = playerNormalStateMachine->AddState(TEXT("Player_Down"));					// 다운
		playerDown->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_down")));

		auto playerDownup = playerNormalStateMachine->AddState(TEXT("Player_Downup"));				// 다운 후 기상
		playerDownup->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_downup")));

		// Die
		auto playerDieStart = playerNormalStateMachine->AddState(TEXT("Player_DieStart"));			// 죽음 시작
		playerDieStart->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_diestart")));

		auto playerDie = playerNormalStateMachine->AddState(TEXT("Player_Die"));						// 죽음 계속
		playerDie->SetAnimationClip(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_die")));

		// Attacks
		auto playerSword = playerNormalStateMachine->AddState(TEXT("Player_Sword"));					// 기본 소드 공격
		auto playerSwordClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_sword"));
		playerSwordClip->SetIsRootMotion(true);
		playerSword->SetAnimationClip(playerSwordClip);

		auto playerSwordCombo1_2 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo1_2"));
		auto playerSwordCombo1_2Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo1_2"));
		playerSwordCombo1_2Clip->SetIsRootMotion(true);
		playerSwordCombo1_2->SetAnimationClip(playerSwordCombo1_2Clip);

		auto playerSwordCombo1_3 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo1_3"));
		auto playerSwordCombo1_3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo1_3"));
		playerSwordCombo1_3Clip->SetIsRootMotion(true);
		playerSwordCombo1_3->SetAnimationClip(playerSwordCombo1_3Clip);

		auto playerSwordCombo2_3 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo2_3"));
		auto playerSwordCombo2_3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo2_3"));
		playerSwordCombo2_3Clip->SetIsRootMotion(true);
		playerSwordCombo2_3->SetAnimationClip(playerSwordCombo2_3Clip);

		auto playerSwordCombo2_4 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo2_4"));
		auto playerSwordCombo2_4Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo2_4"));
		playerSwordCombo2_4Clip->SetIsRootMotion(true);
		playerSwordCombo2_4->SetAnimationClip(playerSwordCombo2_4Clip);

		auto playerSwordCombo3_4 = playerNormalStateMachine->AddState(TEXT("Player_SwordCombo3_4"));
		auto playerSwordCombo3_4Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_swordcombo3_4"));
		playerSwordCombo3_4Clip->SetIsRootMotion(true);
		playerSwordCombo3_4->SetAnimationClip(playerSwordCombo3_4Clip);

		auto playerFist = playerNormalStateMachine->AddState(TEXT("Player_Fist"));										// 기본 펀치 공격
		auto playerFistClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_fist"));
		playerFistClip->SetIsRootMotion(true);
		playerFist->SetAnimationClip(playerFistClip);

		auto playerFistCombo1_2 = playerNormalStateMachine->AddState(TEXT("Player_FistCombo1_2"));
		auto playerFistCombo1_2Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_noraml_fistcombo1_2"));
		playerFistCombo1_2Clip->SetIsRootMotion(true);
		playerFistCombo1_2->SetAnimationClip(playerFistCombo1_2Clip);

		auto playerFistCombo1_3 = playerNormalStateMachine->AddState(TEXT("Player_FistCombo1_3"));
		auto playerFistCombo1_3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_fistcombo1_3"));
		playerFistCombo1_3Clip->SetIsRootMotion(true);
		playerFistCombo1_3->SetAnimationClip(playerFistCombo1_3Clip);

		auto playerFistCombo2_3 = playerNormalStateMachine->AddState(TEXT("Player_FistCombo2_3"));
		auto playerFistCombo2_3Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_fistcombo2_3"));
		playerFistCombo2_3Clip->SetIsRootMotion(true);
		playerFistCombo2_3->SetAnimationClip(playerFistCombo2_3Clip);

		auto playerFistCombo2_4 = playerNormalStateMachine->AddState(TEXT("Player_FistCombo2_4"));
		auto playerFistCombo2_4Clip = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimationClip(TEXT("player_normal_fistcombo2_4"));
		playerFistCombo2_4Clip->SetIsRootMotion(true);
		playerFistCombo2_4->SetAnimationClip(playerFistCombo2_4Clip);

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

		swordEvent._targetFrame = 157.f;

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

		swordComboEvent._eventName = TEXT("StartCancleFrame");

		swordComboEvent._targetFrame = 13.f;

		playerSwordCombo1_2Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndCancleFrame");

		swordComboEvent._targetFrame = 31.f;

		playerSwordCombo1_2Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndAttack");

		swordComboEvent._targetFrame = 61.f;

		playerSwordCombo1_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("StartCancleFrame");

		swordComboEvent._targetFrame = 38.f;

		playerSwordCombo2_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndCancleFrame");

		swordComboEvent._targetFrame = 61.f;

		playerSwordCombo2_3Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndAttack");

		swordComboEvent._targetFrame = 98.f;

		playerSwordCombo2_4Clip->AddEvent(swordComboEvent);

		swordComboEvent._eventName = TEXT("EndAttack");

		swordComboEvent._targetFrame = 108.f;

		playerSwordCombo3_4Clip->AddEvent(swordComboEvent);
#pragma endregion

#pragma region FIST_COMBO_ANIMATION_EVENT

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

#pragma region DIE_START_END
		auto playerIdleToDieStart = playerIdle->AddTransition(playerDieStart);
		playerIdleToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerMoveToDieStart = playerMove->AddTransition(playerDieStart);
		playerMoveToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerRunToDieStart = playerRun->AddTransition(playerDieStart);
		playerRunToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

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

		auto playerFistCombo2_3ToDieStart = playerFistCombo2_3->AddTransition(playerDieStart);
		playerFistCombo2_3ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerFistCombo2_4ToDieStart = playerFistCombo2_4->AddTransition(playerDieStart);
		playerFistCombo2_4ToDieStart->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);

		auto playerDieStartToDie = playerDieStart->AddTransition(playerDie);
		playerDieStartToDie->AddCondition(TEXT("IsDie"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DASH_START
		auto playerIdleToDash = playerIdle->AddTransition(playerDash);
		playerIdleToDash->SetTransitionDuration(0.01f);
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
		playerSwordCombo1_2ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo1_3ToDash = playerSwordCombo1_3->AddTransition(playerDash);
		playerSwordCombo1_3ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo2_3ToDash = playerSwordCombo2_3->AddTransition(playerDash);
		playerSwordCombo2_3ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo2_4ToDash = playerSwordCombo2_4->AddTransition(playerDash);
		playerSwordCombo2_4ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerSwordCombo3_4ToDash = playerSwordCombo3_4->AddTransition(playerDash);
		playerSwordCombo3_4ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerFistCombo2_3ToDash = playerFistCombo2_3->AddTransition(playerDash);
		playerFistCombo2_3ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);

		auto playerFistCombo2_4ToDash = playerFistCombo2_4->AddTransition(playerDash);
		playerFistCombo2_4ToDash->AddCondition(TEXT("IsDash"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DASH_END
		auto playerDashToIdle = playerDash->AddTransition(playerIdle);
		playerDashToIdle->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);

		auto playerDashToMove = playerDash->AddTransition(playerMove);
		playerDashToMove->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		playerDashToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		auto playerDashToRun = playerDash->AddTransition(playerRun);
		playerDashToRun->AddCondition(TEXT("IsDash"), AnimatorConditionMode::False);
		playerDashToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);
#pragma endregion

#pragma region DOWN_START_END
		auto playerIdleToDown = playerIdle->AddTransition(playerDown);
		playerIdleToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerMoveToDown = playerMove->AddTransition(playerDash);
		playerMoveToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerRunToDown = playerRun->AddTransition(playerDash);
		playerRunToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerSwordToDown = playerSword->AddTransition(playerDash);
		playerSwordToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		auto playerFistToDown = playerFist->AddTransition(playerDash);
		playerFistToDown->AddCondition(TEXT("IsDown"), AnimatorConditionMode::True);

		// 애니메이션 시간이 다 끝날 때 Transition 하면 됩니다 ..!
		auto playerDownToDownup = playerDown->AddTransition(playerDownup);
		playerDownToDownup->AddCondition(TEXT("IsDown"), AnimatorConditionMode::False);
#pragma endregion

#pragma region HIT_START
		auto playerIdleToHit1 = playerIdle->AddTransition(playerHit1);
		playerIdleToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerIdleToHit2 = playerIdle->AddTransition(playerHit2);
		playerIdleToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerIdleToHit3 = playerIdle->AddTransition(playerHeavyHit);
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
		playerSwordToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerSwordToHit2 = playerSword->AddTransition(playerHit2);
		playerSwordToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerSwordToHit3 = playerSword->AddTransition(playerHeavyHit);
		playerSwordToHit3->AddCondition(TEXT("IsHeavyHit"), AnimatorConditionMode::True);

		auto playerFistToHit1 = playerFist->AddTransition(playerHit1);
		playerFistToHit1->AddCondition(TEXT("IsHit1"), AnimatorConditionMode::True);

		auto playerFistToHit2 = playerFist->AddTransition(playerHit2);
		playerFistToHit2->AddCondition(TEXT("IsHit2"), AnimatorConditionMode::True);

		auto playerFistToHit3 = playerFist->AddTransition(playerHeavyHit);
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

		auto playerFistCombo2_3ToIdle = playerFistCombo2_3->AddTransition(playerIdle);
		playerFistCombo2_3ToIdle->AddCondition(TEXT("IsAttack"), AnimatorConditionMode::False);

		auto playerFistCombo2_4ToIdle = playerFistCombo2_4->AddTransition(playerIdle);
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
		playerRunToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);
		playerRunToIdle->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);

		auto playerIdleToMove = playerIdle->AddTransition(playerMove);
		playerIdleToMove->SetTransitionDuration(0.01f);
		playerIdleToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);

		auto playerMoveToIdle = playerMove->AddTransition(playerIdle);
		playerMoveToIdle->AddCondition(TEXT("IsMove"), AnimatorConditionMode::False);

		auto playerMoveToRun = playerMove->AddTransition(playerRun);
		playerMoveToRun->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerMoveToRun->AddCondition(TEXT("IsRun"), AnimatorConditionMode::True);

		auto playerRunToMove = playerRun->AddTransition(playerMove);
		playerRunToMove->AddCondition(TEXT("IsMove"), AnimatorConditionMode::True);
		playerRunToMove->AddCondition(TEXT("IsRun"), AnimatorConditionMode::False);
#pragma endregion

		DUOLGameEngine::ResourceManager::GetInstance()->AddAnimatorController(playerNormalAnimCon);
	}
}