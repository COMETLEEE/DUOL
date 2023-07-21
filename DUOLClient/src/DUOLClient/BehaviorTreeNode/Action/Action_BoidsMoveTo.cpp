#include "DUOLClient/BehaviorTreeNode/Action/Action_BoidsMoveTo.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

DUOLClient::Action_BoidsMoveTo::Action_BoidsMoveTo(const std::string& name, const BT::NodeConfig& config) :
	StatefulActionNode(name, config), _targetTransform(nullptr),
	_navMeshAgent(nullptr), _transform(nullptr),
	_enemyGroupController(nullptr)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();

	float temp = getInput<float>("TargetToDistance").value();

	_navMeshAgent = _ai->GetNavMeshAgent();

	_animator = _ai->GetAnimator();

	_targetTransform = _ai->GetTargetTransform();

	_transform = _ai->GetParentTransform();

	_targetDistance = std::max(1.0f, temp);
}

BT::NodeStatus DUOLClient::Action_BoidsMoveTo::onStart()
{
	if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::FAILURE;

	_enemyGroupController = _ai->GetGroupController();

	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_BoidsMoveTo::onRunning()
{
	if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::FAILURE;


	_ai->SetNavOnRigidbodyOff();
	_navMeshAgent->SetMaxSpeed(_ai->GetParameter<float>(TEXT("MaxSpeed")));
	_navMeshAgent->SetSeparation(true);

	_animator->SetFloat(TEXT("MoveSpeed"), 0);
	_animator->SetBool(TEXT("IsWalkRight"), false);
	_animator->SetBool(TEXT("IsWalkLeft"), false);
	_animator->SetBool(TEXT("IsWalkBack"), false);

	/* 제 생각에는 플레이어의 트랜스폼이 잘못 들어왔거나 혹은
	 * 몬스터 그룹에 이상한 녀석에 같이 속해 있을 가능성이 높다고 봅니다...!
	 * 이 점 참고해서 디버깅 해주시면 감사하겠습니다.
	 */

	// 몬스터 이상하게 걷는 이유 찾으러 오신거라면 이 아래쪽을 보시면 됩니다..!!
	// 몬스터 이상하게 걷는 이유 찾으러 오신거라면 이 아래쪽을 보시면 됩니다..!!
	// 몬스터 이상하게 걷는 이유 찾으러 오신거라면 이 아래쪽을 보시면 됩니다..!!
	// 몬스터 이상하게 걷는 이유 찾으러 오신거라면 이 아래쪽을 보시면 됩니다..!!

	const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

	const auto groupEnemys = _enemyGroupController->GetGroupEnemys();

	auto pos = _transform->GetWorldPosition(); // Todo : Player의 트랜스폼이 이상할 가능성이 있다...!

	DUOLMath::Vector3 direction; // 기존에 몬스터가 가려고한 방향.

	DUOLMath::Vector3 separation; // 서로 멀어지려는 성질
	DUOLMath::Vector3 alignment; // 같은 방향으로 조향
	DUOLMath::Vector3 cohesion; // 무리의 평균 위치를 향해 이동. 중심지로 이동

	DUOLMath::Vector3 enemyGroupCenter = _enemyGroupController->GetGroupCenterPos(); // 무리의 중심 찾기.
	// Todo : 무리의 중심이 이상한 값일지도..?
	// 몬스터 한마리가 어디로 날라가서 값이 확 튀어 버렸을 확률도 있다...!

	direction = targetPos - pos; // 몬스터에서 플레이어를 가르키는 벡터를 구한다.

	int count = 0; // Count는 그룹에 속한 몬스터 중에 separation 값의 영향을 줄 수 있는 몬스터의 수를 세기 위한 카운트이다.
	// 만약 거리가 너무 멀다면 제외한다.

	for (auto& [key, iter] : groupEnemys) // Todo : 그룹에 이상한 녀석이 속해 있을 가능성이 있다...!!
	{
		const auto otherPos = iter->GetGameObject()->GetTransform()->GetWorldPosition();

		auto otherPosToPos = pos - otherPos;

		float length = otherPosToPos.Length();

		otherPosToPos.Normalize();

		float weight = 10.0f - length;

		if (weight > 0)
		{
			separation += otherPosToPos * weight;
			count++;
		}
		else
			continue;
	}
	if (count > 0)
		separation /= (float)count;

	cohesion = enemyGroupCenter - pos; // 무리의 중심으로 가는 벡터
	alignment = targetPos - enemyGroupCenter; // 그룹 전체가 플레이어에게 향하는 방향.


	// y를 0으로 초기화 하는 이유는 플레이어와 몬스터가 수평상에 존재할 것이라고 가정을 하고 있기 때문...!
	cohesion.y = 0;
	alignment.y = 0;
	separation.y = 0;
	// y를 0으로 초기화 하는 이유는 

	// 각 펙터의 보정 값을 곱해준다.
	cohesion = cohesion * _enemyGroupController->GetCohesion();
	alignment = alignment * _enemyGroupController->GetAlignment();
	separation = separation * _enemyGroupController->GetSeparation();
	// 각 펙터의 보정 값을 곱해준다.

	// Y 값을 플레이어의 Y로 바꾸는 이유는 위의 y를 0으로 초기화 하는 이유와 같다.
	auto saveY = pos.y;
	pos.y = targetPos.y;
	// Y 값을 플레이어의 Y로 바꾸는 이유는 위의 y를 0으로 초기화 하는 이유와 같다.

	// 모든 값을 더해준다.
	auto result = cohesion + alignment + separation + direction;


	// 이동하고자 하는 목표 거리에 도달한다면 이동을 중지한다.
	if (DUOLMath::Vector3::Distance(pos, targetPos) <= _targetDistance)
	{
		_animator->SetFloat(TEXT("MoveSpeed"), 0.0f);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));
		return BT::NodeStatus::SUCCESS;
	}

	result /= 4;


	// 이동하려는 거리가 너무 작다면 이동을 중지한다.
	if (result.Length() < 1.0f)
	{
		_animator->SetFloat(TEXT("MoveSpeed"), 0.0f);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		return BT::NodeStatus::SUCCESS;
	}

	_navMeshAgent->SetDestination(pos + result);


	// 몬스터 이상하게 걷는 이유 찾으러 오신거라면 이 위쪽을 보시면 됩니다..!!
	// 몬스터 이상하게 걷는 이유 찾으러 오신거라면 이 위쪽을 보시면 됩니다..!!
	// 몬스터 이상하게 걷는 이유 찾으러 오신거라면 이 위쪽을 보시면 됩니다..!!
	// 몬스터 이상하게 걷는 이유 찾으러 오신거라면 이 위쪽을 보시면 됩니다..!!



	pos.y = saveY;

	DUOLMath::Vector3 curVelocity = _navMeshAgent->GetVelocity();

	if (curVelocity.Length() > 2.0f) // 속도가 빠를 때
	{
		_animator->SetFloat(TEXT("MoveSpeed"), 1.0f);

		curVelocity.y = 0;
		const DUOLMath::Vector3 lookWay = pos + curVelocity;

		_transform->LookAt(lookWay);
	}
	else // 속도가 느릴 때
	{

		const DUOLMath::Vector3 lookWay = pos + DUOLMath::Vector3(direction.x, 0, direction.z);
		// 에러 제거
		if (lookWay != pos)
			_transform->LookAt(lookWay);

		curVelocity.Normalize();

		const auto lookDotDir = direction.Dot(curVelocity);

		if (90.0f - acos(lookDotDir) > 30.0f) // 좌우 걸음.
		{
			const auto isRight = _transform->GetRight().Dot(direction);

			isRight > 0 ?
				_animator->SetBool(TEXT("IsWalkRight"), true)
				: _animator->SetBool(TEXT("IsWalkLeft"), true);
		}
		else if (lookDotDir > 0) // 정면
			_animator->SetFloat(TEXT("MoveSpeed"), 0.5f);
		else // 뒷걸음
			_animator->SetBool(TEXT("IsWalkBack"), true);
	}




	return BT::NodeStatus::RUNNING;

}

void DUOLClient::Action_BoidsMoveTo::onHalted()
{
	if (getInput<AI_EnemyBasic*>("AI").value())
	{
		_animator->SetFloat(TEXT("MoveSpeed"), 0.0f);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		if (_navMeshAgent->GetIsEnabled())
		{
			_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));
		}
	}
}

BT::PortsList DUOLClient::Action_BoidsMoveTo::providedPorts()
{
	BT::PortsList result =
	{
		BT::InputPort<AI_EnemyBasic*>("AI"),
		BT::InputPort<float>("TargetToDistance")
	};

	return result;
}
