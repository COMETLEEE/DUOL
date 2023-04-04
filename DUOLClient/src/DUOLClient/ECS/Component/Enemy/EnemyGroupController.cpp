#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/AI_Enemy_Near.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::EnemyGroupController>("EnemyGroupController")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_radius",&DUOLClient::EnemyGroupController::_radius)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_count",&DUOLClient::EnemyGroupController::_count)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_tokkenCount", &DUOLClient::EnemyGroupController::_tokkenCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	).property("_cohesion", &DUOLClient::EnemyGroupController::_cohesion)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	).property("_alignment", &DUOLClient::EnemyGroupController::_alignment)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	).property("_separation", &DUOLClient::EnemyGroupController::_separation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);
}



DUOLClient::EnemyGroupController::EnemyGroupController(DUOLGameEngine::GameObject* owner,
	const DUOLCommon::tstring& name) :MonoBehaviourBase(owner, name),
	_enemys(), _radius(0), _count(0), _tokkenCount(0),
	_targetPos(), _isGroupCheck(false),
	_cohesion(1.0f), _alignment(1.0f), _separation(1.0f)
{
}


const std::vector<DUOLClient::AI_EnemyBase*>& DUOLClient::EnemyGroupController::GetGroupEnemys()
{
	return _enemys;
}

void DUOLClient::EnemyGroupController::Initialize(float radius, float count)
{
	_radius = radius;
	_count = count;
	_targetPos = GetTransform()->GetWorldPosition();
}

void DUOLClient::EnemyGroupController::CreateEnemy()
{
	auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

	_enemys.resize(_count);

	_targetPos = GetTransform()->GetWorldPosition();

	for (int i = 0; i < _count; i++)
	{
		auto gameObj = scene->CreateFromFBXModel(TEXT("monster"));

		//gameObj->AddComponent<DUOLGameEngine::CapsuleCollider>();

		DUOLMath::Vector3 randVec = DUOLMath::Vector3(
			DUOLMath::MathHelper::RandF(-_radius, _radius),
			0,
			DUOLMath::MathHelper::RandF(-_radius, _radius));

		gameObj->GetTransform()->SetPosition(_targetPos + randVec);

		_enemys[i] = gameObj->AddComponent<AI_Enemy_Near>();

		_enemys[i]->SetGroupController(this);
	}
}

void DUOLClient::EnemyGroupController::OnAwake()
{
}

void DUOLClient::EnemyGroupController::OnStart()
{

}

void DUOLClient::EnemyGroupController::OnUpdate(float deltaTime)
{
	if (!_testinit)
	{
		_testinit = true;
		CreateEnemy();
	}
	else
	{
		if (_tokkenCount > 0)
		{
			for (auto& iter : _enemys)
			{
				if (!iter->_enemy->GetIsDie() && !iter->GetIsToken())
				{
					iter->TakeToken();
					_tokkenCount--;
					break;
				}
			}
		}
	}

}
