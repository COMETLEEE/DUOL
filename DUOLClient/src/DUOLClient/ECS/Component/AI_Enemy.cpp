#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include <rttr/registration>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::AI_Enemy>("AI_Enemy")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::AI_Enemy::AI_Enemy(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)
{
}

DUOLClient::AI_Enemy::~AI_Enemy()
{
}

void DUOLClient::AI_Enemy::OnAwake()
{
	_behaviortreeController = GetGameObject()->GetComponent<DUOLGameEngine::BehaviortreeController>();

	if (_behaviortreeController == nullptr)
		_behaviortreeController = GetGameObject()->AddComponent<DUOLGameEngine::BehaviortreeController>();

	// 트리 팩토리를 초기화 해 줄 클래스가 필요하다..!
	auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

	treeFactory->Initialize();
	// 트리 팩토리를 초기화 해 줄 클래스가 필요하다..!

	auto tree = treeFactory->CreateTree("Test");

	/// ---------------------------- test Code ----------------------------------
	auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

	DUOLGameEngine::GameObject* target = nullptr;
	for (auto gameObject : allGameObjects)
	{
		if (gameObject->GetName() == TEXT("PlayerCharacter"))
		{
			target = gameObject;
		}
	}
	if (target)
		tree.rootBlackboard()->set<DUOLGameEngine::Transform*>("TargetTransform", target->GetTransform());

	/// ---------------------------- test Code ----------------------------------

	_behaviortreeController->Initialize(std::move(tree));
}

void DUOLClient::AI_Enemy::OnStart()
{

}
