#include "DUOLClient/BehaviorTreeNode/Action/Action_BoidsMoveTo.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
DUOLClient::Action_BoidsMoveTo::Action_BoidsMoveTo(const std::string& name, const BT::NodeConfig& config) :
	StatefulActionNode(name, config), _targetTransform(nullptr),
	_navMeshAgent(nullptr), _transform(nullptr)
{
}

BT::NodeStatus DUOLClient::Action_BoidsMoveTo::onStart()
{
	if (_ai == nullptr)
	{
		_ai = getInput<AI_EnemyBasic*>("AI").value();

		_navMeshAgent = _ai->GetNavMeshAgent();

		_enemyGroupController = _ai->GetGroupController();

		_animator = _ai->GetAnimator();

		_targetTransform = _ai->GetTargetTransform();

		_transform = _ai->GetParentTransform();
	}

	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_BoidsMoveTo::onRunning()
{
	if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::FAILURE;

	if (!_navMeshAgent->GetIsEnabled()) return BT::NodeStatus::FAILURE;

	_animator->SetFloat(TEXT("MoveSpeed"), 0);
	_animator->SetBool(TEXT("IsWalkRight"), false);
	_animator->SetBool(TEXT("IsWalkLeft"), false);
	_animator->SetBool(TEXT("IsWalkBack"), false);

	const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

	const auto groupEnemys = _enemyGroupController->GetGroupEnemys();

	auto pos = _transform->GetWorldPosition();

	DUOLMath::Vector3 direction; // 기존에 가려고한 방향.

	DUOLMath::Vector3 separation; // 서로 멀어지려는 성질
	DUOLMath::Vector3 alignment; // 같은 방향으로 조향
	DUOLMath::Vector3 cohesion; // 무리의 평균 위치를 향해 이동. 중심지로 이동

	DUOLMath::Vector3 enemyGroupCenter = _enemyGroupController->GetGroupCenterPos(); // 무리의 중심 찾기.

	//DUOLMath::Vector3 pushOutDir; // 너무 플레이어에게 가까이 다가가면 물러선다.
	//float pushOutForce;

	direction = targetPos - pos;

	//pushOutDir = -direction;

	//pushOutForce = 10 - direction.Length();

	//pushOutForce = std::clamp(pushOutForce, 0.0f, 10.0f);

	int count = 0;
	for (auto& [key, iter] : groupEnemys)
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

	cohesion = enemyGroupCenter - pos;
	alignment = targetPos - enemyGroupCenter;
	auto alingmentTemp = alignment;

	cohesion.y = 0;
	alignment.y = 0;
	separation.y = 0;

	cohesion = cohesion * _enemyGroupController->GetCohesion();
	alignment = alignment * _enemyGroupController->GetAlignment();
	separation = separation * _enemyGroupController->GetSeparation();

	auto saveY = pos.y;
	pos.y = targetPos.y;
	auto result = cohesion + alignment + separation;

	result /= 3;

	if (alingmentTemp.Length() < 1.0f)
		return BT::NodeStatus::SUCCESS;

	_navMeshAgent->SetDestination(pos + result);

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

		if (abs(lookDotDir) > 0.5f) // 좌우 걸음.
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




	if (DUOLMath::Vector3::Distance(pos, targetPos) >= 2.0f)
		return BT::NodeStatus::RUNNING;

	return BT::NodeStatus::SUCCESS;

}

void DUOLClient::Action_BoidsMoveTo::onHalted()
{
	if (getInput<AI_EnemyBasic*>("AI").value())
	{
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		if (_navMeshAgent->GetIsEnabled())
			_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));
	}
}

BT::PortsList DUOLClient::Action_BoidsMoveTo::providedPorts()
{
	BT::PortsList result = {
BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
BT::InputPort<DUOLGameEngine::Animator*>("Animator"),
BT::InputPort<DUOLGameEngine::NavMeshAgent*>("NavMeshAgent"),
BT::InputPort<AI_EnemyBasic*>("AI"),
	};
	return result;
}
