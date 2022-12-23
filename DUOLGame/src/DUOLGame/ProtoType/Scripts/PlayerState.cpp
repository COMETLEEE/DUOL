#include "DUOLGame/ProtoType/Scripts/PlayerState.h"

#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"

/* States */
#include "DUOLGame/ProtoType/PlayerStates/PlayerIdle.h"
#include "DUOLGame/ProtoType/PlayerStates/PlayerMove.h"
#include "DUOLGame/ProtoType/PlayerStates/PlayerAttack.h"
#include "DUOLGame/ProtoType/PlayerStates/PlayerJump.h"

/* Condition */
#include "DUOLGame/ProtoType/PlayerStates/PlayerGroundCondition.h"

/* ETC */
#include "DUOLGame/ProtoType/Scripts/CameraController.h"
#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	PlayerState::PlayerState(const std::weak_ptr<GameObject>& owner) :
		MonoBehaviourBase(owner)
		, _moveSpeed(50.0f)
		, _jumpPower(500.0f)
		, _isGround(false)
	{

	}

	PlayerState::~PlayerState()
	{

	}

	void PlayerState::OnAwake()
	{
		_bt = std::make_shared<BehaviorTree>(_T("PlayerBehaviorTree"));
		_bb = std::make_shared<BlackBoard>();

		_bt->RegistBlackBoard(_bb);

		/* root */
		auto root = _bt->GetRoot().lock();

		if (root == nullptr)
			return;

		auto playerState = root->SetNode<SelectorNode>(_T("PlayerState")).lock();

		/* Ground */
		auto groundSequencer = playerState->AddChild<SequenceNode>(_T("GroundSequencer")).lock();
		auto groundCondition = groundSequencer->AddChild<PlayerGroundCondition>().lock();
		auto ground = groundSequencer->AddChild<SelectorNode>(_T("Ground")).lock();

		ground->AddChild<PlayerJump>();
		ground->AddChild<PlayerAttack>();
		ground->AddChild<PlayerMove>();
		ground->AddChild<PlayerIdle>();

		/* Air */
		auto air = playerState->AddChild<SelectorNode>(_T("Air")).lock();

		auto airIdle = air->AddChild<PlayerIdle>().lock();

		airIdle->AddEvent(NodeBase::PreEvent([](float deltaTime, NodeBase* object, NodeState current)
			{
				static float coolTime = 0.0f;

				if (coolTime <= 0.0f)
				{
					coolTime = 1.0f;

					DUOL_INFO("AIR");
				}
				else
				{
					coolTime -= TimeManager::GetInstance()->GetDeltaTime();
				}
			}));
	}

	void PlayerState::OnStart()
	{
		/* Data */
		_bb->Push<GameObject*>(GetGameObject(), _T("Player"));
		_bb->Push<GameObject*>(GetGameObject()->GetComponent<CameraController>()->GetCameraController(), _T("CameraController"));
		_bb->Push<float>(_moveSpeed, _T("MoveSpeed"));
		_bb->Push<float>(_jumpPower, _T("JumpPower"));
		_bb->Push<bool>(_isGround, _T("GroundCondition"));

		auto capsuleCollider = GetGameObject()->GetComponent<CapsuleCollider>();

		capsuleCollider->SetCenter(DUOLMath::Vector3::Zero);
		capsuleCollider->SetHeight(1.0f);
		capsuleCollider->SetRadius(0.8f);
	}

	void PlayerState::OnUpdate(float deltaTime)
	{
		_bb->Push<float>(deltaTime, _T("DeltaTime"));

		bool beforeIsGround = _bb->Get<bool>(_T("GroundCondition"));

		if (beforeIsGround == false && _isGround == true)
			GetGameObject()->GetComponent<Rigidbody>()->SetLinearVelocity(DUOLMath::Vector3::Zero);

		_bb->Push<bool>(_isGround, _T("GroundCondition"));

		_bt->Execute();
	}
}