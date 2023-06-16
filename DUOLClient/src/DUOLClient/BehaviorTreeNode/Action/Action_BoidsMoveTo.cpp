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

	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_BoidsMoveTo::onRunning()
{
	if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::FAILURE;

	if (!_enemyGroupController)
		_enemyGroupController = _ai->GetGroupController();

	_ai->SetNavOnRigidbodyOff();
	_navMeshAgent->SetMaxSpeed(_ai->GetParameter<float>(TEXT("MaxSpeed")));
	_navMeshAgent->SetSeparation(true);


	_animator->SetFloat(TEXT("MoveSpeed"), 0);
	_animator->SetBool(TEXT("IsWalkRight"), false);
	_animator->SetBool(TEXT("IsWalkLeft"), false);
	_animator->SetBool(TEXT("IsWalkBack"), false);

	const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

	const auto groupEnemys = _enemyGroupController->GetGroupEnemys();

	auto pos = _transform->GetWorldPosition();

	DUOLMath::Vector3 direction; // ������ �������� ����.

	DUOLMath::Vector3 separation; // ���� �־������� ����
	DUOLMath::Vector3 alignment; // ���� �������� ����
	DUOLMath::Vector3 cohesion; // ������ ��� ��ġ�� ���� �̵�. �߽����� �̵�

	DUOLMath::Vector3 enemyGroupCenter = _enemyGroupController->GetGroupCenterPos(); // ������ �߽� ã��.

	//DUOLMath::Vector3 pushOutDir; // �ʹ� �÷��̾�� ������ �ٰ����� ��������.
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
	auto result = cohesion + alignment + separation + direction;

	if (DUOLMath::Vector3::Distance(pos, targetPos) <= _targetDistance)
	{
		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));
		_navMeshAgent->SetSeparation(false);
		return BT::NodeStatus::SUCCESS;
	}

	result /= 4;

	/*if (alingmentTemp.Length() < 1.0f)
		return BT::NodeStatus::SUCCESS;*/

		//if (result.Length() > 0.3f) return BT::NodeStatus::SUCCESS;

	_navMeshAgent->SetDestination(pos + result);

	pos.y = saveY;

	DUOLMath::Vector3 curVelocity = _navMeshAgent->GetVelocity();

	if (curVelocity.Length() > 2.0f) // �ӵ��� ���� ��
	{
		_animator->SetFloat(TEXT("MoveSpeed"), 1.0f);

		curVelocity.y = 0;
		const DUOLMath::Vector3 lookWay = pos + curVelocity;

		_transform->LookAt(lookWay);
	}
	else // �ӵ��� ���� ��
	{

		const DUOLMath::Vector3 lookWay = pos + DUOLMath::Vector3(direction.x, 0, direction.z);
		// ���� ����
		if (lookWay != pos)
			_transform->LookAt(lookWay);

		curVelocity.Normalize();

		const auto lookDotDir = direction.Dot(curVelocity);

		if (90.0f - acos(lookDotDir) > 30.0f) // �¿� ����.
		{
			const auto isRight = _transform->GetRight().Dot(direction);

			isRight > 0 ?
				_animator->SetBool(TEXT("IsWalkRight"), true)
				: _animator->SetBool(TEXT("IsWalkLeft"), true);
		}
		else if (lookDotDir > 0) // ����
			_animator->SetFloat(TEXT("MoveSpeed"), 0.5f);
		else // �ް���
			_animator->SetBool(TEXT("IsWalkBack"), true);
	}




	return BT::NodeStatus::RUNNING;

}

void DUOLClient::Action_BoidsMoveTo::onHalted()
{
	if (getInput<AI_EnemyBasic*>("AI").value())
	{
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		if (_navMeshAgent->GetIsEnabled())
		{
			_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));
			_navMeshAgent->SetSeparation(false);
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
