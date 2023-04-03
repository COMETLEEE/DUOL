#include "DUOLClient/ECS/Component/Enemy/AI_Enemy_Near.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLCommon/MetaDataType.h"

#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::AI_Enemy_Near>("AI_Enemy_Near")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLClient
{
	AI_Enemy_Near::AI_Enemy_Near(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		AI_EnemyBase(owner, name)
	{
	}

	void AI_Enemy_Near::OnAwake()
	{
		AI_EnemyBase::OnAwake();

		// 몬스터 인덱스 번호 부여.
		_enemy->SetEnemyCode(EnemyCode::Near);

		GetGameObject()->SetName(_enemy->GetEnemyData()->_name);

		auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

		auto tree = treeFactory->CreateTree(_enemy->GetEnemyData()->_behaviorTreeName);

		tree.rootBlackboard()->set<AI_EnemyBase*>("AI", this);

		tree.rootBlackboard()->set<DUOLGameEngine::Animator*>("Animator", GetAnimator());

		auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : allGameObjects)
		{
			if (gameObject->GetTag() == TEXT("Player"))
			{
				SetTarget(gameObject);
			}
		}
		if (GetTarget())
			tree.rootBlackboard()->set<DUOLGameEngine::Transform*>("TargetTransform", GetTarget()->GetTransform());
		else
			tree.rootBlackboard()->set<DUOLGameEngine::Transform*>("TargetTransform", nullptr);

		GetBehaviorTreeController()->Initialize(std::move(tree));

	}
}
