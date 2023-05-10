#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::AI_EnemyBasic>("AI_Enemy_Near")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}



DUOLClient::AI_EnemyBasic::AI_EnemyBasic(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name), _enemyGroupController(nullptr), _enemy(nullptr),
	_parentGameObject(nullptr), _parentTransform(nullptr),
	_targetTransform(nullptr)
{
}

DUOLClient::AI_EnemyBasic::~AI_EnemyBasic()
{
	GetBehaviorTreeController()->SetBlackBoard<AI_EnemyBasic*>("AI", nullptr);
}

void DUOLClient::AI_EnemyBasic::Initialize()
{
	if (!_enemy)
		_enemy = GetGameObject()->GetComponent<Enemy>();

	auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

	auto tree = treeFactory->CreateTree(_enemy->GetEnemyData()->_behaviorTreeName);

	auto rootBlackBoard = tree.rootBlackboard();

	rootBlackBoard->set<AI_EnemyBasic*>("AI", this);
	rootBlackBoard->set<DUOLGameEngine::Animator*>("Animator", GetAnimator());
	rootBlackBoard->set<DUOLGameEngine::GameObject*>("ParentObject", GetGameObject()->GetTransform()->GetParent()->GetGameObject());
	rootBlackBoard->set<float>("AttackDelayTime", _enemy->GetAttackDelayTime());
	rootBlackBoard->set<float>("AttackCancelTime", _enemy->GetAttackCancelTime());

	auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

	SetTarget(EnemyManager::GetInstance()->GetPlayerCharacterGameObject());

	_parentTransform = GetTransform()->GetParent();

	_parentGameObject = _parentTransform->GetGameObject();

	rootBlackBoard->set<DUOLGameEngine::Transform*>("TargetTransform", GetTarget()->GetTransform());

	_targetTransform = GetTarget()->GetTransform();

	GetBehaviorTreeController()->Initialize(std::move(tree));
}

void DUOLClient::AI_EnemyBasic::SetAnimConditionReset()
{
	_enemy->_animator->AllParamReset();
}

bool DUOLClient::AI_EnemyBasic::GetIsHitCheck()
{
	return _enemy->_isHit;
}

void DUOLClient::AI_EnemyBasic::SetIsHit(bool isHit)
{
	_enemy->_isHit = isHit;
}

bool DUOLClient::AI_EnemyBasic::GetIsGroupCheck()
{
	if (_enemyGroupController)
		return _enemyGroupController->GetIsGroupCheck();
	return false;
}

void DUOLClient::AI_EnemyBasic::SetIsGroupCheck()
{
	if (_enemyGroupController)
		_enemyGroupController->SetIsGroupCheck();
}

void DUOLClient::AI_EnemyBasic::SetGroupController(EnemyGroupController* enemyGroupController)
{
	_enemyGroupController = enemyGroupController;
}

void DUOLClient::AI_EnemyBasic::SetIsDie()
{
	UseToken();
	GetGroupController()->EraseEnemy(GetGameObject()->GetUUID());
	GetAnimator()->SetBool(TEXT("IsDie"), true);
	ChangeMaterial(EnemyMaterial::DIE);

	std::function<DUOLGameEngine::CoroutineHandler(void)> func =
		std::bind(
			[](AI_EnemyBasic* ai)->DUOLGameEngine::CoroutineHandler
			{
				co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(0.3f);

				while (true)
				{
					if (!ai->GetIsAirborne() && ai->GetAnimator()->GetSpeed() <= 0.0f)
					{
						ai->SetColliderEnable(false);
						co_return;
					}
					co_yield nullptr;
				}
			}, this);
	StartCoroutine(func);
}

bool DUOLClient::AI_EnemyBasic::GetIsDie() const
{
	return _enemy->GetIsDie();
}

bool DUOLClient::AI_EnemyBasic::GetIsAirborne() const
{
	return _enemy->GetIsAirBorne();
}

bool DUOLClient::AI_EnemyBasic::GetIsChase() const
{
	auto length = _enemy->_target->GetTransform()->GetWorldPosition() - GetGroupController()->GetGroupCenterPos();
	return _enemy->_chaseRange > length.Length();
}

float DUOLClient::AI_EnemyBasic::GetLookRange() const
{
	return _enemy->GetLookRange();
}

float DUOLClient::AI_EnemyBasic::GetAttackRange() const
{
	return _enemy->GetAttackRange();
}

float DUOLClient::AI_EnemyBasic::GetPatrolRange() const
{
	return _enemy->GetPatrolOffset();
}

bool DUOLClient::AI_EnemyBasic::GetIsSuperArmor() const
{
	return _enemy->GetIsSuperArmor();
}

void DUOLClient::AI_EnemyBasic::AddSuperArmorGauge(float addGauge)
{
	_enemy->AddSuperArmorGauge(addGauge);
}

float DUOLClient::AI_EnemyBasic::GetCurrentSuperArmorGauge() const
{
	return _enemy->GetCurrentSuperArmorGauge();
}

float DUOLClient::AI_EnemyBasic::GetMaxSuperArmorGauge() const
{
	return _enemy->GetMaxSuperArmorGauge();
}

void DUOLClient::AI_EnemyBasic::ChangeMaterial(EnemyMaterial enemyMaterial)
{
	_enemy->ChangeMaterial(EnemyMaterial::DIE);
}

DUOLGameEngine::GameObject* DUOLClient::AI_EnemyBasic::GetParentGameObject() const
{
	return _parentGameObject;
}

DUOLGameEngine::Transform* DUOLClient::AI_EnemyBasic::GetParentTransform() const
{
	return _parentTransform;
}

void DUOLClient::AI_EnemyBasic::UseToken()
{
	if (_enemy->_isToken)
	{
		_enemy->_isToken = false;
		_enemyGroupController->RetureTokken();
		// 토큰을 사용하고 무리에 다시 반납하여야 한다.
	}

}

void DUOLClient::AI_EnemyBasic::TakeToken()
{
	_enemy->_isToken = true;
}

DUOLGameEngine::GameObject* DUOLClient::AI_EnemyBasic::GetTarget() const
{
	return _enemy->_target;
}

DUOLGameEngine::Transform* DUOLClient::AI_EnemyBasic::GetTargetTransform() const
{
	return _targetTransform;
}

void DUOLClient::AI_EnemyBasic::SetTarget(DUOLGameEngine::GameObject* target)
{
	_enemy->_target = target;

}

void DUOLClient::AI_EnemyBasic::SetNavOnRigidbodyOff()
{
	_enemy->SetNavOnRigidbodyOff();
}

void DUOLClient::AI_EnemyBasic::SetNavOffRigidbodyOn()
{
	_enemy->SetNavOffRigidbodyOn();
}

void DUOLClient::AI_EnemyBasic::SetSuperArmor(bool isSuperArmor, float time)
{
	_enemy->_currentSuperArmorGauge = 0;

	_enemy->SetSuperArmor(isSuperArmor);

	auto lamdafunc = [](Enemy* enemy, bool isSuperArmor, float time)->DUOLGameEngine::CoroutineHandler
	{
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(time);

		enemy->SetSuperArmor(!isSuperArmor);

		enemy->SetIsCanSuperArmor(false);
	};
	std::function<DUOLGameEngine::CoroutineHandler()> func = std::bind(lamdafunc, _enemy, isSuperArmor, time);

	StartCoroutine(func);
}

void DUOLClient::AI_EnemyBasic::SetColliderEnable(bool isBool)
{
	_enemy->SetColiiderEnable(isBool);
}

bool DUOLClient::AI_EnemyBasic::GetIsToken() const
{
	return _enemy->_isToken;
}

DUOLClient::EnemyGroupController* DUOLClient::AI_EnemyBasic::GetGroupController() const
{
	return _enemyGroupController;
}

DUOLGameEngine::NavMeshAgent* DUOLClient::AI_EnemyBasic::GetNavMeshAgent() const
{
	return _enemy->_navMeshAgent;
}

DUOLGameEngine::Animator* DUOLClient::AI_EnemyBasic::GetAnimator() const
{
	return _enemy->_animator;
}

DUOLGameEngine::BehaviortreeController* DUOLClient::AI_EnemyBasic::GetBehaviorTreeController() const
{
	return _behaviortreeController;
}

void DUOLClient::AI_EnemyBasic::OnAwake()
{
	GetGameObject()->SetTag(TEXT("Enemy"));

	if (_behaviortreeController == nullptr)
		_behaviortreeController = GetGameObject()->AddComponent<DUOLGameEngine::BehaviortreeController>();
}
