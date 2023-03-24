#include "DUOLClient/BehaviorTreeNode/Action/Action_BoidsMoveTo.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

#include "DUOLClient/ECS/Component/EnemyGroupController.h"
#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
DUOLClient::Action_BoidsMoveTo::Action_BoidsMoveTo(const std::string& name, const BT::NodeConfig& config) :
	StatefulActionNode(name, config), _gameObject(nullptr), _targetTransform(nullptr),
	_navMeshAgent(nullptr)
{
}

BT::NodeStatus DUOLClient::Action_BoidsMoveTo::onStart()
{
	if (_gameObject == nullptr)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_navMeshAgent = _gameObject->GetComponent<DUOLGameEngine::NavMeshAgent>();

		_ai = _gameObject->GetComponent<AI_Enemy>();

		_enemyGroupController = _ai->GetGroupController();

		_animator = _gameObject->GetComponent<DUOLGameEngine::Animator>();
	}

	// �Ź� �ʱ�ȭ�� �ϴ� ������ Ÿ���� ����� ���ɼ��� �ֱ� �����̴�.
	if (getInput<DUOLGameEngine::Transform*>("TargetTransform").has_value())
		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();
	else
		DUOL_TRACE(DUOL_CONSOLE, "has not data in black borad");

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

	auto pos = _gameObject->GetTransform()->GetWorldPosition();

	DUOLMath::Vector3 direction; // ������ �������� ����.

	DUOLMath::Vector3 separation; // ���� �־������� ����
	DUOLMath::Vector3 alignment; // ���� �������� ����
	DUOLMath::Vector3 cohesion; // ������ ��� ��ġ�� ���� �̵�. �߽����� �̵�

	DUOLMath::Vector3 enemyGroupCenter; // ������ �߽� ã��.

	//DUOLMath::Vector3 pushOutDir; // �ʹ� �÷��̾�� ������ �ٰ����� ��������.
	//float pushOutForce;

	direction = targetPos - pos;

	//pushOutDir = -direction;

	//pushOutForce = 10 - direction.Length();

	//pushOutForce = std::clamp(pushOutForce, 0.0f, 10.0f);

	int count = 0;
	for (auto& iter : groupEnemys)
	{
		const auto otherPos = iter->GetGameObject()->GetTransform()->GetWorldPosition();

		enemyGroupCenter += otherPos;

		auto otherPosToPos = pos - otherPos;

		float length = otherPosToPos.Length();

		otherPosToPos.Normalize();

		float weight = 5.0f - length;

		if (weight > 0)
		{
			separation += otherPosToPos * weight;
			count++;
		}
		else
			continue;
	}
	enemyGroupCenter /= groupEnemys.size();
	separation /= (float)count;

	cohesion = enemyGroupCenter - pos;
	alignment = targetPos - enemyGroupCenter;
	auto alingmentTemp = alignment;

	cohesion.y = 0;
	alignment.y = 0;
	separation.y = 0;

	direction.Normalize();
	cohesion.Normalize();
	alignment.Normalize();
	separation.Normalize();

	cohesion = cohesion * _enemyGroupController->GetCohesion();
	alignment = alignment * _enemyGroupController->GetAlignment();
	separation = separation * _enemyGroupController->GetSeparation();

	pos.y = targetPos.y;
	const auto result = direction + cohesion + alignment + separation;

	if (alingmentTemp.Length() < 1.0f)
		return BT::NodeStatus::SUCCESS;

	_navMeshAgent->SetDestination(pos + result);

	DUOLMath::Vector3 curVelocity = _navMeshAgent->GetVelocity();

	if (curVelocity.Length() > 2.0f) // �ӵ��� ���� ��
	{
		_animator->SetFloat(TEXT("MoveSpeed"), 1.0f);

		const DUOLMath::Vector3 lookWay = pos + curVelocity;

		_gameObject->GetTransform()->LookAt(lookWay);
	}
	else // �ӵ��� ���� ��
	{
		const DUOLMath::Vector3 lookWay = pos + direction;
		// ���� ����
		if (lookWay != pos)
			_gameObject->GetTransform()->LookAt(lookWay);

		curVelocity.Normalize();

		const auto lookDotDir = direction.Dot(curVelocity);

		if (abs(lookDotDir) > 0.5f) // �¿� ����.
		{
			const auto isRight = _gameObject->GetTransform()->GetRight().Dot(direction);

			isRight > 0 ?
				_animator->SetBool(TEXT("IsWalkRight"), true)
				: _animator->SetBool(TEXT("IsWalkLeft"), true);
		}
		else if (lookDotDir > 0) // ����
			_animator->SetFloat(TEXT("MoveSpeed"), 0.5f);
		else // �ް���
			_animator->SetBool(TEXT("IsWalkBack"), true);
	}




	if (DUOLMath::Vector3::Distance(pos, targetPos) >= 2.0f)
		return BT::NodeStatus::RUNNING;

	return BT::NodeStatus::SUCCESS;

}

void DUOLClient::Action_BoidsMoveTo::onHalted()
{
	if (getInput<DUOLGameEngine::GameObject*>("GameObject").value())
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
BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform")
	};
	return result;
}
