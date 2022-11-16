#include "PlayerBehaviorTree.h"

/* Attack */
#include "PlayerAttack.h"
#include "IsPlayerAttackable.h"

/* Jump */
#include "PlayerJump.h"
#include "IsPlayerJumpable.h"

/* Dash */
#include "PlayerDash.h"
#include "IsPlayerDashable.h"

/* Move */
#include "PlayerMove.h"
#include "IsPlayerMoveable.h"

/* Idle */
#include "PlayerIdle.h"

int location[2] = {};

void PlayerBehaviorTree::Init()
{
	bt = std::make_shared<BehaviorTree>(_T("PlayerBehaviorTree"));
	bb = std::make_shared<BlackBoard>();

	bt->RegistBlackBoard(bb);

	/* Player Data */
	bb->Push<int*>(location, _T("PlayerLocation"));
	bb->Push<int>(0, _T("PlayerVelocityX"));
	bb->Push<int>(0, _T("PlayerVelocityY"));
	bb->Push<int>(2, _T("PlayerSpeed"));
	bb->Push<int>(20, _T("MoveRange"));
	bb->Push<int>(0, _T("AttackCombo"));

	/* Root */
	std::weak_ptr<SubTree> root = bt->GetRoot();

	/* Flow Controller */
	auto flowController = root.lock()->SetNode<SelectorNode>(_T("PlayerSelector"));

	/* Is Ground */
	auto groundFlow = flowController.lock()->AddChild<SelectorNode>(_T("IsGround"));

	/* Attack */
	auto playerAttack = groundFlow.lock()->AddChild<SequenceNode>(_T("PlayerAttackSequence"));

	playerAttack.lock()->AddChild<IsPlayerAttackable>();

	playerAttack.lock()->AddChild<PlayerAttack>();

	/* Jump */
	auto playerJump = groundFlow.lock()->AddChild<SequenceNode>(_T("PlayerJumpSequence"));

	playerJump.lock()->AddChild<IsPlayerJumpable>();

	playerJump.lock()->AddChild<PlayerJump>();

	playerJump.lock()->AddEvent(NodeBase::PostEvent([](float, NodeBase*, NodeState, NodeState current)
		{
			if (current == NodeState::SUCCESS)
				std::cout << "Jump PostEvent" << std::endl;
		}));

	auto jumpMover = playerJump.lock()->AddChild<AlwaysSuccessNode>(_T("JumpMover"));

	jumpMover.lock()->SetNode<PlayerMove>();

	/* Dash */
	auto playerDash = groundFlow.lock()->AddChild<SequenceNode>(_T("PlayerDashSequence"));

	playerDash.lock()->AddChild<IsPlayerDashable>();

	auto dashRepeater = playerDash.lock()->AddChild<RepeatNode>(_T("PlayerDashRepeater"), 5);

	dashRepeater.lock()->SetNode<PlayerDash>();

	/* Move */
	auto playerMove = groundFlow.lock()->AddChild<SequenceNode>(_T("PlayerMoveSequence"));

	playerMove.lock()->AddChild<IsPlayerMoveable>();

	playerMove.lock()->AddChild<PlayerMove>();

	/* Idle */
	groundFlow.lock()->AddChild<PlayerIdle>();
}

void PlayerBehaviorTree::Update(float tick)
{
	bt->Execute();
}